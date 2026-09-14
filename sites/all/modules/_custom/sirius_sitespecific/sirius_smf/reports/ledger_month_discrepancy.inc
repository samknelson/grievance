<?php

/**
 * Ledger Month Discrepancy
 *
 * Payments (sirius_payment nodes) whose received date (field_sirius_datetime_created)
 * and posted date (field_sirius_datetime) fall in different calendar months, evaluated
 * in America/Los_Angeles.
 *
 * pks():   one lean query over all payments (nid + the two raw UTC datetimes). Month
 *          comparison and month/day deltas are computed in PHP. Returns nids of the
 *          mismatches, sorted by |months apart| desc then nid, chunked.
 * batch(): node_load each payment and fill in the descriptive columns
 *          (type, status, amount, payer). Deltas are recomputed here from the same
 *          two fields so the row is self-contained.
 */

class Sirius_Smf_Report_Ledger_Month_Discrepancy extends Report_Cache_Report_Generator {
  public $type = 'sirius-smf-ledger-month-discrepancy';

  // Drupal stores datetime fields as UTC 'Y-m-d H:i:s'. Report displays in Pacific.
  public $tz_display = 'America/Los_Angeles';

  public function info($result = array()) {
    $result = array(
      'name' => t('Ledger Month Discrepancy'),
      'description' => t(''),
      'access' => 'sirius trust staff',
      'preview' => array(
        'default_display_fields' => array(
          'payment_nid',
          'title',
          'payment_type',
          'status',
          'amount',
          'payer',
          'received_pt',
          'posted_pt',
          'received_month',
          'posted_month',
          'months_apart',
          'days_apart',
        ),
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'reports/ledger_month_discrepancy.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {
    $fields = array();
    $fields['payment_nid']    = 'Payment NID';
    $fields['title']          = 'Title';
    $fields['payment_type']   = 'Payment Type';
    $fields['status']         = 'Status';
    $fields['amount']         = 'Amount';
    $fields['payer_nid']      = 'Payer NID';
    $fields['payer']          = 'Payer';
    $fields['received_pt']    = 'Received (PT)';
    $fields['posted_pt']      = 'Posted (PT)';
    $fields['received_month'] = 'Received Month';
    $fields['posted_month']   = 'Posted Month';
    $fields['months_apart']   = 'Months Apart';
    $fields['days_apart']     = 'Days Apart';
    return $fields;
  }

  public function preview_links($row) {
    $links = array();
    if ($row['payment_nid']) { $links['node/' . $row['payment_nid']] = 'Payment'; }
    if ($row['payer_nid'])   { $links['node/' . $row['payer_nid']]   = 'Payer'; }
    return $links;
  }

  /**
   * Optional posted-date window. Both blank = full population (matches the raw query).
   */
  public function form() {
    $form['posted_from'] = array(
      '#type' => 'date_popup',
      '#title' => t('Posted On or After'),
      '#attributes' => array('autocomplete' => 'off'),
      '#date_format' => 'Y-m-d',
      '#required' => FALSE,
      '#default_value' => '',
    );

    $form['posted_to'] = array(
      '#type' => 'date_popup',
      '#title' => t('Posted On or Before'),
      '#attributes' => array('autocomplete' => 'off'),
      '#date_format' => 'Y-m-d',
      '#required' => FALSE,
      '#default_value' => '',
    );

    return $form;
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
   * Population: every sirius_payment with both a received and a posted datetime.
   * Keep the SQL lean; the month comparison happens in PHP.
   */
  public function pks(&$form_state) {
    $parameters = $form_state['values']['parameters'];

    $sql = "select p.nid, ";
    $sql .= "pcre.field_sirius_datetime_created_value as received_utc, ";
    $sql .= "ppos.field_sirius_datetime_value as posted_utc ";
    $sql .= "from node p ";
    $sql .= "join field_data_field_sirius_datetime_created pcre ";
    $sql .= "  on pcre.entity_type = 'node' and pcre.entity_id = p.nid and pcre.deleted = 0 ";
    $sql .= "join field_data_field_sirius_datetime ppos ";
    $sql .= "  on ppos.entity_type = 'node' and ppos.entity_id = p.nid and ppos.deleted = 0 ";
    $sql .= "where p.type = 'sirius_payment' ";

    $args = array();

    // Optional posted-date window. Convert the Pacific day boundaries to UTC so the
    // comparison is against the stored value with no SQL-side CONVERT_TZ.
    if ($parameters['posted_from']) {
      $sql .= "and ppos.field_sirius_datetime_value >= :posted_from ";
      $args[':posted_from'] = $this->pt_day_to_utc($parameters['posted_from'], '00:00:00');
    }
    if ($parameters['posted_to']) {
      $sql .= "and ppos.field_sirius_datetime_value <= :posted_to ";
      $args[':posted_to'] = $this->pt_day_to_utc($parameters['posted_to'], '23:59:59');
    }

    $sql .= "order by p.nid ";

    $stmt = sirius_sql_query($sql, $args);

    $hits = array();
    while ($hr = $stmt->fetchAssoc()) {
      $delta = $this->delta($hr['received_utc'], $hr['posted_utc']);
      if (!$delta) { continue; }
      if ($delta['received_month'] == $delta['posted_month']) { continue; }

      $hits[] = array(
        'nid' => $hr['nid'] + 0,
        'abs_months' => abs($delta['months_apart']),
      );
    }

    // Mirror the raw query: ORDER BY ABS(months apart) DESC, nid ASC.
    usort($hits, array($this, 'sort_hits'));

    $pks = array();
    foreach ($hits as $hit) { $pks[] = $hit['nid']; }

    return array_chunk($pks, 100);
  }

  public function sort_hits($a, $b) {
    if ($a['abs_months'] != $b['abs_months']) {
      return ($a['abs_months'] > $b['abs_months']) ? -1 : 1;
    }
    if ($a['nid'] == $b['nid']) { return 0; }
    return ($a['nid'] < $b['nid']) ? -1 : 1;
  }

  /**
   * One chunk of payment nids in, one row per payment out.
   */
  public function batch($pks, $values, &$context) {
    $payer_lookup = array();
    $term_lookup = array();

    $rows = array();
    foreach ($pks as $payment_nid) {
      $payment_nid += 0;
      $payment_node = node_load($payment_nid);
      if (!$payment_node) { continue; }

      $row = array();
      $row['payment_nid'] = $payment_nid;
      $row['title'] = $payment_node->title;
      $row['status'] = $payment_node->field_sirius_payment_status['und'][0]['value'];
      $row['amount'] = $payment_node->field_sirius_dollar_amt['und'][0]['value'];

      // Payment type (taxonomy term name)
      $tid = $payment_node->field_sirius_payment_type['und'][0]['tid'];
      if ($tid) {
        if (!isset($term_lookup[$tid])) {
          $term = taxonomy_term_load($tid);
          $term_lookup[$tid] = $term ? $term->name : '';
        }
        $row['payment_type'] = $term_lookup[$tid];
      }

      // Payer (node title)
      $payer_nid = $payment_node->field_sirius_payer['und'][0]['target_id'];
      if ($payer_nid) {
        if (!isset($payer_lookup[$payer_nid])) {
          $payer_node = node_load($payer_nid);
          $payer_lookup[$payer_nid] = $payer_node ? $payer_node->title : '';
        }
        $row['payer_nid'] = $payer_nid;
        $row['payer'] = $payer_lookup[$payer_nid];
      }

      // Dates
      $received_utc = $payment_node->field_sirius_datetime_created['und'][0]['value'];
      $posted_utc = $payment_node->field_sirius_datetime['und'][0]['value'];
      $delta = $this->delta($received_utc, $posted_utc);
      if ($delta) {
        $row['received_pt'] = $delta['received_pt'];
        $row['posted_pt'] = $delta['posted_pt'];
        $row['received_month'] = $delta['received_month'];
        $row['posted_month'] = $delta['posted_month'];
        $row['months_apart'] = $delta['months_apart'];
        $row['days_apart'] = $delta['days_apart'];
      }

      $rows[$payment_nid] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }

  /**
   * Convert two UTC datetime strings to Pacific and compute the month/day deltas.
   *
   * months_apart mirrors MySQL TIMESTAMPDIFF(MONTH, received, posted): whole months
   * elapsed, signed (negative when posted precedes received).
   * days_apart mirrors DATEDIFF(posted, received): calendar days, ignoring time of day.
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

    // Whole months, signed.
    $diff = $received->diff($posted);
    $months = ($diff->y * 12) + $diff->m;
    if ($diff->invert) { $months = -$months; }
    $result['months_apart'] = $months;

    // Calendar days, signed: strip time of day before differencing.
    $received_day = date_create($received->format('Y-m-d'), $pt);
    $posted_day = date_create($posted->format('Y-m-d'), $pt);
    $day_diff = $received_day->diff($posted_day);
    $days = $day_diff->days;
    if ($day_diff->invert) { $days = -$days; }
    $result['days_apart'] = $days;

    return $result;
  }

  /**
   * A Pacific calendar day + clock time, expressed as the UTC string Drupal stores.
   */
  public function pt_day_to_utc($day, $clock) {
    $pt = new DateTimeZone($this->tz_display);
    $dt = date_create($day . ' ' . $clock, $pt);
    if (!$dt) { return NULL; }
    $dt->setTimezone(new DateTimeZone('UTC'));
    return $dt->format('Y-m-d H:i:s');
  }
}
