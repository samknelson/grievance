<?php

/**
 * Ledger QA Review Queue
 *
 * One report covering every payment-record condition the finance team
 * reviews. Emits one row per ISSUE, not per payment: a payment that trips
 * two categories appears twice, so each row is a discrete work item.
 *
 * Categories
 *   A  MONTH_DISCREPANCY   Cleared payment whose received month and posted
 *                          month differ (America/Los_Angeles).
 *   B  CANCELED_WITH_LEDGER  Canceled payment whose ledger rows net to a
 *                          nonzero balance. Rows that net to zero already
 *                          have an offsetting correction posted and are
 *                          not reported.
 *   C  CLEARED_NO_LEDGER   Cleared non-Stripe payment with no ledger rows.
 *   D  STALE_PENDING       Pending/Received payment received before the
 *      STALE_RECEIVED      operator-supplied "stale before" date.
 *   E  CLEARED_NO_TYPE     Cleared payment with no payment type at all and
 *                          no ledger rows.
 *
 * pks():   one small query per selected category, emitting "nid|code" pks.
 *          Keeps each category's SQL independently readable and avoids one
 *          unreviewable monster query.
 * batch(): node_load per payment plus TWO grouped queries per 100-row chunk
 *          (ledger aggregate, revision author) rather than per-payment
 *          lookups.
 *
 * Payments parked against the PAYMENTS PENDING payer are excluded by
 * default: that is a known allowable holding state, usually status
 * Received, and would otherwise dominate category D.
 *
 * Note on sirius_ledger_ar: ledger_reference is polymorphic (payperiods,
 * payments, json definitions, grievance shops). Matching on a payment nid
 * is still unambiguous because nids are globally unique. ledger_key is NULL
 * for payment rows and is NOT usable as a discriminator.
 */

class Sirius_Smf_Report_Ledger_Qa extends Report_Cache_Report_Generator {
  public $type = 'sirius-smf-ledger-qa';

  public $tz_display = 'America/Los_Angeles';

  public $pp_nids;

  public function info($result = array()) {
    $result = array(
      'name' => t('Ledger QA Review Queue'),
      'description' => t('Payment records needing finance review, categorized. One row per issue.'),
      'access' => 'sirius trust staff',
      'preview' => array(
        'default_display_fields' => array(
          'category_label',
          'payment_nid',
          'detail',
          'status',
          'payment_type',
          'amount',
          'payer',
          'received_pt',
          'posted_pt',
        ),
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'reports/ledger_qa.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {
    $fields = array();
    $fields['category']       = 'Category Code';
    $fields['category_label'] = 'Category';
    $fields['detail']         = 'Detail';
    $fields['payment_nid']    = 'Payment NID';
    $fields['title']          = 'Title';
    $fields['status']         = 'Status';
    $fields['payment_type']   = 'Payment Type';
    $fields['amount']         = 'Amount';
    $fields['payer_nid']      = 'Payer NID';
    $fields['payer']          = 'Payer';
    $fields['payer_type']     = 'Payer Type';
    $fields['account']        = 'Account';
    $fields['received_pt']    = 'Received (PT)';
    $fields['posted_pt']      = 'Posted (PT)';
    $fields['check_number']   = 'Check #';
    $fields['merchant']       = 'Merchant';
    $fields['ledger_rows']    = 'Ledger Rows';
    $fields['ledger_ids']     = 'Ledger IDs';
    $fields['ledger_memos']   = 'Ledger Memos';
    $fields['revised_by']     = 'Last Revised By';
    $fields['revised_at']     = 'Last Revised At';
    return $fields;
  }

  public function preview_links($row) {
    $links = array();
    if ($row['payment_nid']) { $links['node/' . $row['payment_nid']] = 'Payment'; }
    if ($row['payer_nid'])   { $links['node/' . $row['payer_nid']]   = 'Payer'; }
    return $links;
  }

  public function category_labels() {
    return array(
      'A' => t('A — Month discrepancy (cleared)'),
      'B' => t('B — Canceled with ledger entry'),
      'C' => t('C — Cleared non-Stripe, no ledger entry'),
      'D' => t('D — Stale pending / received'),
      'E' => t('E — Cleared, no payment type, no ledger entry'),
    );
  }

  public function form() {
    $form['categories'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Categories'),
      '#options' => $this->category_labels(),
      '#default_value' => array_keys($this->category_labels()),
    );

    $form['stale_before'] = array(
      '#type' => 'date_popup',
      '#title' => t('Stale Before'),
      '#description' => t('Category D only. Pending or Received payments received BEFORE this date are reported. Required if category D is selected.'),
      '#attributes' => array('autocomplete' => 'off'),
      '#date_format' => 'Y-m-d',
      '#required' => FALSE,
      '#default_value' => '',
    );

    $form['include_payments_pending'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include payments whose payer is PAYMENTS PENDING'),
      '#description' => t('Off by default. Payments parked against the PAYMENTS PENDING payer are a known, allowable holding state and usually carry status Received.'),
      '#default_value' => 0,
    );

    return $form;
  }

  public function form_validate($form, &$form_state) {
    $parameters = $form_state['values']['parameters'];

    $categories = array_filter((array) $parameters['categories']);
    if (!$categories) {
      form_set_error("parameters][categories", 'Please select at least one category.');
    }

    if ($categories['D'] && !$parameters['stale_before']) {
      form_set_error("parameters][stale_before", 'Category D requires a "Stale Before" date.');
    }
  }

  public function render_parameters($parameters) {
    $parameters = $parameters['parameters'];

    $html = sirius_jsonfield_util_render_all_values_new(
      $this->form(),
      $parameters,
      array(
        'skip_empty' => TRUE,
        'no_fieldsets' => TRUE,
      )
    );

    return $html;
  }

  /**
   * Nid of the PAYMENTS PENDING payer. Matched on exact title + bundle so
   * we don't hardcode a nid that differs between staging and production.
   * On production this resolves to the grievance_shop node 16838651; the
   * similarly-named sirius_log and sirius_payment nodes are NOT payers and
   * must not be matched.
   * Cached for the life of the request.
   */
  public function payments_pending_nids() {
    if (isset($this->pp_nids)) { return $this->pp_nids; }

    $sql = "select n.nid from node n ";
    $sql .= "where n.title = :title and n.type = :type ";
    $stmt = sirius_sql_query($sql, array(
      ':title' => 'PAYMENTS PENDING',
      ':type' => 'grievance_shop',
    ));

    $nids = array();
    while ($hr = $stmt->fetchAssoc()) { $nids[] = intval($hr['nid']); }

    $this->pp_nids = $nids;
    return $nids;
  }

  /**
   * Excludes payments parked against the PAYMENTS PENDING payer unless the
   * operator opted in. Returns an empty fragment when opted in, or when no
   * node with that title exists.
   */
  public function payer_exclusion($parameters) {
    if ($parameters['include_payments_pending']) { return ''; }

    $nids = $this->payments_pending_nids();
    if (!$nids) { return ''; }

    $list = implode(',', $nids);

    $sql = "and not exists ( ";
    $sql .= "  select 1 from field_data_field_sirius_payer pr_x ";
    $sql .= "  where pr_x.entity_type = 'node' and pr_x.entity_id = p.nid and pr_x.deleted = 0 ";
    $sql .= "  and pr_x.field_sirius_payer_target_id in ($list) ";
    $sql .= ") ";

    return $sql;
  }

  public function pks(&$form_state) {
    $parameters = $form_state['values']['parameters'];
    $categories = array_filter((array) $parameters['categories']);

    $pks = array();

    if ($categories['A']) { $pks = array_merge($pks, $this->pks_a($parameters)); }
    if ($categories['B']) { $pks = array_merge($pks, $this->pks_b($parameters)); }
    if ($categories['C']) { $pks = array_merge($pks, $this->pks_c($parameters)); }
    if ($categories['D']) { $pks = array_merge($pks, $this->pks_d($parameters)); }
    if ($categories['E']) { $pks = array_merge($pks, $this->pks_e($parameters)); }

    return array_chunk($pks, 100);
  }

  /**
   * A: cleared payments whose received and posted months differ.
   * Month comparison happens in PHP so the query stays index-friendly and
   * we don't depend on the MySQL timezone tables.
   */
  public function pks_a($parameters) {
    $args = array();

    $sql = "select p.nid, ";
    $sql .= "pcre.field_sirius_datetime_created_value as received_utc, ";
    $sql .= "ppos.field_sirius_datetime_value as posted_utc ";
    $sql .= "from node p ";
    $sql .= "join field_data_field_sirius_payment_status st ";
    $sql .= "  on st.entity_type = 'node' and st.entity_id = p.nid and st.deleted = 0 ";
    $sql .= "  and st.field_sirius_payment_status_value = 'Cleared' ";
    $sql .= "join field_data_field_sirius_datetime_created pcre ";
    $sql .= "  on pcre.entity_type = 'node' and pcre.entity_id = p.nid and pcre.deleted = 0 ";
    $sql .= "join field_data_field_sirius_datetime ppos ";
    $sql .= "  on ppos.entity_type = 'node' and ppos.entity_id = p.nid and ppos.deleted = 0 ";
    $sql .= "where p.type = 'sirius_payment' ";
    $sql .= $this->payer_exclusion($parameters);
    $sql .= "order by p.nid ";

    $stmt = sirius_sql_query($sql, $args);

    $hits = array();
    while ($hr = $stmt->fetchAssoc()) {
      $delta = $this->delta($hr['received_utc'], $hr['posted_utc']);
      if (!$delta) { continue; }
      if ($delta['received_month'] == $delta['posted_month']) { continue; }
      $hits[] = array('nid' => $hr['nid'] + 0, 'mag' => abs($delta['months_apart']));
    }

    return $this->order_and_tag($hits, 'A');
  }

  /**
   * B: canceled payments that still carry ledger rows.
   */
  public function pks_b($parameters) {
    $args = array();

    $sql = "select p.nid, sum(l.ledger_amount) as net ";
    $sql .= "from node p ";
    $sql .= "join field_data_field_sirius_payment_status st ";
    $sql .= "  on st.entity_type = 'node' and st.entity_id = p.nid and st.deleted = 0 ";
    $sql .= "  and st.field_sirius_payment_status_value = 'Canceled' ";
    $sql .= "join sirius_ledger_ar l on l.ledger_reference = p.nid ";
    $sql .= "where p.type = 'sirius_payment' ";
    $sql .= $this->payer_exclusion($parameters);
    $sql .= "group by p.nid ";

    $stmt = sirius_sql_query($sql, $args);

    // Rows that net to zero already have an offsetting correction posted;
    // there is nothing to review.
    $hits = array();
    while ($hr = $stmt->fetchAssoc()) {
      $net = $hr['net'] + 0;
      if (!$net) { continue; }
      $hits[] = array('nid' => $hr['nid'] + 0, 'mag' => abs($net));
    }

    return $this->order_and_tag($hits, 'B');
  }

  /**
   * C: cleared, has a payment type, type is not Stripe, no ledger rows.
   */
  public function pks_c($parameters) {
    $args = array();

    $sql = "select p.nid, pcre.field_sirius_datetime_created_value as received_utc ";
    $sql .= "from node p ";
    $sql .= "join field_data_field_sirius_payment_status st ";
    $sql .= "  on st.entity_type = 'node' and st.entity_id = p.nid and st.deleted = 0 ";
    $sql .= "  and st.field_sirius_payment_status_value = 'Cleared' ";
    $sql .= "join field_data_field_sirius_payment_type pty ";
    $sql .= "  on pty.entity_type = 'node' and pty.entity_id = p.nid and pty.deleted = 0 ";
    $sql .= "join taxonomy_term_data pmtype ";
    $sql .= "  on pmtype.tid = pty.field_sirius_payment_type_tid and pmtype.name <> 'Stripe' ";
    $sql .= "left join field_data_field_sirius_datetime_created pcre ";
    $sql .= "  on pcre.entity_type = 'node' and pcre.entity_id = p.nid and pcre.deleted = 0 ";
    $sql .= "where p.type = 'sirius_payment' ";
    $sql .= "and not exists ( ";
    $sql .= "  select 1 from sirius_ledger_ar l where l.ledger_reference = p.nid ";
    $sql .= ") ";
    $sql .= $this->payer_exclusion($parameters);
    $sql .= "order by pcre.field_sirius_datetime_created_value desc ";

    $stmt = sirius_sql_query($sql, $args);

    $pks = array();
    while ($hr = $stmt->fetchAssoc()) { $pks[] = ($hr['nid'] + 0) . '|C'; }
    return $pks;
  }

  /**
   * D: pending or received payments older than the stale-before date.
   */
  public function pks_d($parameters) {
    $args = array();
    $args[':stale_before'] = $this->pt_day_to_utc($parameters['stale_before'], '00:00:00');

    $sql = "select p.nid ";
    $sql .= "from node p ";
    $sql .= "join field_data_field_sirius_payment_status st ";
    $sql .= "  on st.entity_type = 'node' and st.entity_id = p.nid and st.deleted = 0 ";
    $sql .= "  and st.field_sirius_payment_status_value in ('Pending', 'Received') ";
    $sql .= "join field_data_field_sirius_datetime_created pcre ";
    $sql .= "  on pcre.entity_type = 'node' and pcre.entity_id = p.nid and pcre.deleted = 0 ";
    $sql .= "where p.type = 'sirius_payment' ";
    $sql .= "and pcre.field_sirius_datetime_created_value < :stale_before ";
    $sql .= $this->payer_exclusion($parameters);
    $sql .= "order by pcre.field_sirius_datetime_created_value asc ";

    $stmt = sirius_sql_query($sql, $args);

    $pks = array();
    while ($hr = $stmt->fetchAssoc()) { $pks[] = ($hr['nid'] + 0) . '|D'; }
    return $pks;
  }

  /**
   * E: cleared, NO payment type at all, no ledger rows. These fall through
   * category C's inner join on the payment type term.
   */
  public function pks_e($parameters) {
    $args = array();

    $sql = "select p.nid ";
    $sql .= "from node p ";
    $sql .= "join field_data_field_sirius_payment_status st ";
    $sql .= "  on st.entity_type = 'node' and st.entity_id = p.nid and st.deleted = 0 ";
    $sql .= "  and st.field_sirius_payment_status_value = 'Cleared' ";
    $sql .= "left join field_data_field_sirius_payment_type pty ";
    $sql .= "  on pty.entity_type = 'node' and pty.entity_id = p.nid and pty.deleted = 0 ";
    $sql .= "left join field_data_field_sirius_datetime_created pcre ";
    $sql .= "  on pcre.entity_type = 'node' and pcre.entity_id = p.nid and pcre.deleted = 0 ";
    $sql .= "where p.type = 'sirius_payment' ";
    $sql .= "and pty.field_sirius_payment_type_tid is null ";
    $sql .= "and not exists ( ";
    $sql .= "  select 1 from sirius_ledger_ar l where l.ledger_reference = p.nid ";
    $sql .= ") ";
    $sql .= $this->payer_exclusion($parameters);
    $sql .= "order by pcre.field_sirius_datetime_created_value desc ";

    $stmt = sirius_sql_query($sql, $args);

    $pks = array();
    while ($hr = $stmt->fetchAssoc()) { $pks[] = ($hr['nid'] + 0) . '|E'; }
    return $pks;
  }

  /**
   * Sort by magnitude descending, then nid, and tag with the category code.
   */
  public function order_and_tag($hits, $code) {
    usort($hits, array($this, 'sort_hits'));

    $pks = array();
    foreach ($hits as $hit) { $pks[] = $hit['nid'] . '|' . $code; }
    return $pks;
  }

  public function sort_hits($a, $b) {
    if ($a['mag'] != $b['mag']) { return ($a['mag'] > $b['mag']) ? -1 : 1; }
    if ($a['nid'] == $b['nid']) { return 0; }
    return ($a['nid'] < $b['nid']) ? -1 : 1;
  }

  public function batch($pks, $values, &$context) {
    $parameters = $values['parameters'];
    $labels = $this->category_labels();

    // Collect the nids in this chunk once.
    $nids = array();
    foreach ($pks as $pk) {
      list ($nid, $code) = explode('|', $pk, 2);
      $nids[$nid + 0] = $nid + 0;
    }

    $ledger = $this->ledger_summary($nids);
    $revisions = $this->revision_summary($nids);

    $term_lookup = array();
    $node_lookup = array();

    $rows = array();
    foreach ($pks as $pk) {
      list ($nid, $code) = explode('|', $pk, 2);
      $nid += 0;

      $payment_node = node_load($nid);
      if (!$payment_node) { continue; }

      $row = array();
      $row['category'] = $code;
      $row['category_label'] = $labels[$code];
      $row['payment_nid'] = $nid;
      $row['title'] = $payment_node->title;
      $row['status'] = $payment_node->field_sirius_payment_status['und'][0]['value'];
      $row['amount'] = $payment_node->field_sirius_dollar_amt['und'][0]['value'];
      $row['check_number'] = $payment_node->field_sirius_check_number['und'][0]['value'];
      $row['merchant'] = $payment_node->field_sirius_merchant_name['und'][0]['value'];

      // Payment type term
      $tid = $payment_node->field_sirius_payment_type['und'][0]['tid'];
      if ($tid) {
        if (!isset($term_lookup[$tid])) {
          $term = taxonomy_term_load($tid);
          $term_lookup[$tid] = $term ? $term->name : '';
        }
        $row['payment_type'] = $term_lookup[$tid];
      }

      // Payer
      $payer_nid = $payment_node->field_sirius_payer['und'][0]['target_id'];
      if ($payer_nid) {
        if (!isset($node_lookup[$payer_nid])) { $node_lookup[$payer_nid] = node_load($payer_nid); }
        if ($node_lookup[$payer_nid]) {
          $row['payer_nid'] = $payer_nid;
          $row['payer'] = $node_lookup[$payer_nid]->title;
          $row['payer_type'] = $node_lookup[$payer_nid]->type;
        }
      }

      // Ledger account
      $acct_nid = $payment_node->field_sirius_ledger_account['und'][0]['target_id'];
      if ($acct_nid) {
        if (!isset($node_lookup[$acct_nid])) { $node_lookup[$acct_nid] = node_load($acct_nid); }
        if ($node_lookup[$acct_nid]) { $row['account'] = $node_lookup[$acct_nid]->title; }
      }

      // Dates
      $received_utc = $payment_node->field_sirius_datetime_created['und'][0]['value'];
      $posted_utc = $payment_node->field_sirius_datetime['und'][0]['value'];
      $delta = $this->delta($received_utc, $posted_utc);
      if ($delta) {
        $row['received_pt'] = $delta['received_pt'];
        $row['posted_pt'] = $delta['posted_pt'];
      } else {
        $row['received_pt'] = $this->to_pt($received_utc);
        $row['posted_pt'] = $this->to_pt($posted_utc);
      }

      // Ledger. The net is not a column -- it only matters as evidence for
      // category B, where detail() spells it out.
      $net = 0;
      if ($ledger[$nid]) {
        $row['ledger_rows'] = $ledger[$nid]['count'];
        $row['ledger_ids'] = $ledger[$nid]['ids'];
        $row['ledger_memos'] = $ledger[$nid]['memos'];
        $net = $ledger[$nid]['net'];
      } else {
        $row['ledger_rows'] = 0;
      }

      // Revision
      if ($revisions[$nid]) {
        $row['revised_by'] = $revisions[$nid]['name'];
        $row['revised_at'] = $revisions[$nid]['at'];
      }

      $row['detail'] = $this->detail($code, $row, $delta, $net);

      // One row per issue: key on nid AND category.
      $rows[$nid . ':' . $code] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }

  /**
   * Category-specific one-liner explaining why this row is here.
   */
  public function detail($code, $row, $delta, $net) {
    if ($code == 'A') {
      if (!$delta) { return 'Missing received or posted date'; }
      return $delta['received_month'] . ' -> ' . $delta['posted_month']
        . ' (' . $delta['months_apart'] . ' mo, ' . $delta['days_apart'] . ' d)';
    }

    if ($code == 'B') {
      return 'LIVE — nonzero balance; ' . $row['ledger_rows'] . ' row(s), net ' . round($net, 2);
    }

    if ($code == 'C') {
      return 'Cleared ' . ($row['payment_type'] ? $row['payment_type'] : 'payment') . ', no ledger entry';
    }

    if ($code == 'D') {
      $days = $this->days_since($row['received_pt']);
      return $row['status'] . ' since ' . substr($row['received_pt'], 0, 10)
        . ($days === NULL ? '' : ' (' . $days . ' d)');
    }

    if ($code == 'E') {
      return 'Cleared with no payment type and no ledger entry';
    }

    return '';
  }

  public function days_since($pt_datetime) {
    if (!$pt_datetime) { return NULL; }
    $tz = new DateTimeZone($this->tz_display);
    $then = date_create(substr($pt_datetime, 0, 10), $tz);
    $now = date_create(date('Y-m-d'), $tz);
    if (!$then || !$now) { return NULL; }
    $diff = $then->diff($now);
    return $diff->invert ? -$diff->days : $diff->days;
  }

  /**
   * One grouped ledger query for the whole chunk.
   */
  public function ledger_summary($nids) {
    $out = array();
    if (!$nids) { return $out; }

    $list = array();
    foreach ($nids as $nid) { $list[] = intval($nid); }
    $list = implode(',', $list);

    $sql = "select l.ledger_reference as nid, ";
    $sql .= "count(l.ledger_id) as row_ct, ";
    $sql .= "sum(l.ledger_amount) as net, ";
    $sql .= "group_concat(l.ledger_id order by l.ledger_id separator ',') as ids, ";
    $sql .= "group_concat(l.ledger_memo order by l.ledger_id separator ' | ') as memos ";
    $sql .= "from sirius_ledger_ar l ";
    $sql .= "where l.ledger_reference in ($list) ";
    $sql .= "group by l.ledger_reference ";

    $stmt = sirius_sql_query($sql, array());
    while ($hr = $stmt->fetchAssoc()) {
      $out[$hr['nid'] + 0] = array(
        'count' => $hr['row_ct'] + 0,
        'net' => round($hr['net'] + 0, 2),
        'ids' => $hr['ids'],
        'memos' => $hr['memos'],
      );
    }

    return $out;
  }

  /**
   * One grouped revision-author query for the whole chunk.
   */
  public function revision_summary($nids) {
    $out = array();
    if (!$nids) { return $out; }

    $list = array();
    foreach ($nids as $nid) { $list[] = intval($nid); }
    $list = implode(',', $list);

    $sql = "select n.nid, rev.uid, rev.timestamp, u.name ";
    $sql .= "from node n ";
    $sql .= "join node_revision rev on rev.vid = n.vid ";
    $sql .= "left join users u on u.uid = rev.uid ";
    $sql .= "where n.nid in ($list) ";

    $stmt = sirius_sql_query($sql, array());
    while ($hr = $stmt->fetchAssoc()) {
      $out[$hr['nid'] + 0] = array(
        'name' => ($hr['uid'] == 0) ? '(system/anonymous)' : $hr['name'],
        'at' => $hr['timestamp'] ? date('Y-m-d H:i:s', $hr['timestamp']) : '',
      );
    }

    return $out;
  }

  public function to_pt($utc) {
    if (!$utc) { return ''; }
    $dt = date_create($utc, new DateTimeZone('UTC'));
    if (!$dt) { return ''; }
    $dt->setTimezone(new DateTimeZone($this->tz_display));
    return $dt->format('Y-m-d H:i:s');
  }

  /**
   * Received/posted converted to Pacific plus the month and day deltas.
   * months_apart mirrors TIMESTAMPDIFF(MONTH, received, posted);
   * days_apart mirrors DATEDIFF(posted, received).
   */
  public function delta($received_utc, $posted_utc) {
    if (!$received_utc || !$posted_utc) { return NULL; }

    $utc = new DateTimeZone('UTC');
    $pt = new DateTimeZone($this->tz_display);

    $received = date_create($received_utc, $utc);
    $posted = date_create($posted_utc, $utc);
    if (!$received || !$posted) { return NULL; }

    $received->setTimezone($pt);
    $posted->setTimezone($pt);

    $result = array();
    $result['received_pt'] = $received->format('Y-m-d H:i:s');
    $result['posted_pt'] = $posted->format('Y-m-d H:i:s');
    $result['received_month'] = $received->format('Y-m');
    $result['posted_month'] = $posted->format('Y-m');

    $diff = $received->diff($posted);
    $months = ($diff->y * 12) + $diff->m;
    if ($diff->invert) { $months = -$months; }
    $result['months_apart'] = $months;

    $received_day = date_create($received->format('Y-m-d'), $pt);
    $posted_day = date_create($posted->format('Y-m-d'), $pt);
    $day_diff = $received_day->diff($posted_day);
    $days = $day_diff->days;
    if ($day_diff->invert) { $days = -$days; }
    $result['days_apart'] = $days;

    return $result;
  }

  public function pt_day_to_utc($day, $clock) {
    if (!$day) { return NULL; }
    $pt = new DateTimeZone($this->tz_display);
    $dt = date_create($day . ' ' . $clock, $pt);
    if (!$dt) { return NULL; }
    $dt->setTimezone(new DateTimeZone('UTC'));
    return $dt->format('Y-m-d H:i:s');
  }
}
