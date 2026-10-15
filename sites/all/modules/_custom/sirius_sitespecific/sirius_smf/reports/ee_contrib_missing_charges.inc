<?php

/**
 * EE Contrib Missing Charges
 *
 * One row per worker + policy + employer + charge month where the worker is
 * covered under one of the EE-contribution benefits on the charge date but no
 * EE-contribution charge exists in the ledger for that worker and month. The
 * benefits column lists the benefits in force that month.
 *
 * Intended for retro review: the fund runs a range of months and confirms
 * every expected charge was created.
 *
 * How "has a charge" is decided
 *   Ledger account 2457513 holds one charge per worker per month, keyed
 *   '<worker nid>|<YYYY-MM>' in ledger_key. The key is per worker-month, NOT
 *   per benefit: a worker holding both benefits is covered by a single charge
 *   row. (Only a couple of workers hold both at once.) Rows with a NULL key on
 *   this account are payments (reference node type sirius_payment), not
 *   charges, and are correctly ignored. ledger_participant is not the worker
 *   nid and is not used.
 *
 * Rules
 *   - The charge is generated on the 5th. A month is only reported once its
 *     5th has passed.
 *   - The worker must have a published, in-force ELECTION under a selected
 *     policy on the charge date, AND a published, in-force worker-benefit
 *     record (WMB) as its own subscriber for one of the two benefits. The
 *     WMB is the coverage actually granted; the election supplies policy and
 *     employer. In force = start <= charge date and (no end or end > charge
 *     date). Ends fall on the 1st, 25th or month-end, never the 5th, so the
 *     boundary is not ambiguous. One-day stub WMBs end on the 1st and drop out
 *     of this test on their own.
 *   - Dependents are excluded two ways, matching the charge generator: the
 *     WMB's covered person must be its subscriber, AND the WMB must have no
 *     contact-relation. The generator's benefit test
 *     (Sirius_Ledger_Charge_Plugin_Monthly_Benefit) searches with
 *     relationship_nid = 'NULL'; coverage carrying a relation is dependent
 *     coverage to it and never yields a charge, so it is not "missing".
 *   - The employer's shop must have charge settings for the benefit
 *     (shop JSON charge_plugins.settings.<uuid>). Benefits without settings
 *     are skipped; a row with none left is dropped. Production showed this
 *     affects almost nothing; the filter is carried over from the original
 *     ad hoc query.
 *   - Review rows: the earliest rate in the shop's rate history is later
 *     than the first of the charge month (or there is no rate history). No
 *     charge is expected yet, so these are hidden unless the toggle is on.
 *   - ledger_status is not filtered; every row on this account is 'Cleared'.
 *
 * pks():   one query over months x elections x WMBs with the ledger
 *          anti-join, collapsed in PHP to one key per worker/policy/shop/month:
 *          'ym|policy_nid|worker_nid|shop_nid|benefit_nid,benefit_nid'.
 *          Sorted month desc, then policy, employer, worker title.
 * batch(): one titles query and one shop-JSON query per chunk. Rate dates and
 *          the review flag are computed in PHP (no JSON_EXTRACT in SQL).
 */

class Sirius_Smf_Report_Ee_Contrib_Missing_Charges extends Report_Cache_Report_Generator {
  public $type = 'sirius-smf-ee-contrib-missing-charges';

  // Ledger account holding EE contribution charges.
  public $ledger_account = 2457513;

  // Benefit nid => uuid of its entry in grievance_shop charge_plugins.settings.
  public $benefit_uuids = array(
    2457502 => '13c01f95-6d8c-4262-ab30-a65a2b16bdd9',
    2457510 => '059a34c0-0421-4efe-9295-e3c64e08ee2a',
  );

  // Policies selected by default.
  public $default_policies = array(8868663, 16968085, 16973310, 16973312);

  /** Lookup caches — many rows resolve the same handful of values. */
  protected $shop_json_cache = array();
  protected $title_cache = array();

  public function info($result = array()) {
    $result = array(
      'name' => t('EE Contrib Missing Charges'),
      'description' => t('Workers covered in a month with no EE contribution charge in the ledger.'),
      'access' => 'sirius trust staff',
      'preview' => array(
        'default_display_fields' => array(
          'charge_month',
          'policy',
          'worker_nid',
          'worker',
          'employer',
          'benefits',
          'rate_effective',
          'note',
        ),
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'reports/ee_contrib_missing_charges.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {
    $fields = array();
    $fields['charge_month']   = 'Charge Month';
    $fields['charge_date']    = 'Charge Date';
    $fields['policy_nid']     = 'Policy NID';
    $fields['policy']         = 'Policy';
    $fields['worker_nid']     = 'Worker NID';
    $fields['worker']         = 'Worker';
    $fields['employer_nid']   = 'Employer NID';
    $fields['employer']       = 'Employer';
    $fields['benefits']       = 'Benefits In Force';
    $fields['rate_effective'] = 'Rate Effective';
    $fields['note']           = 'Note';
    return $fields;
  }

  public function preview_links($row) {
    $links = array();
    if ($row['worker_nid'])   { $links['node/' . $row['worker_nid']]   = 'Worker'; }
    if ($row['employer_nid']) { $links['node/' . $row['employer_nid']] = 'Employer'; }
    return $links;
  }

  public function form() {
    $options = $this->month_options();

    $form['start_month'] = array(
      '#type' => 'select',
      '#title' => t('From Month'),
      '#options' => $options,
      '#default_value' => date('Y-m', strtotime('first day of this month -17 months')),
      '#required' => TRUE,
    );

    $form['end_month'] = array(
      '#type' => 'select',
      '#title' => t('To Month'),
      '#description' => t('Charges are generated on the 5th, so a month is only included once its 5th has passed. For all time, pick the earliest From Month.'),
      '#options' => $options,
      '#default_value' => date('Y-m'),
      '#required' => TRUE,
    );

    $form['policy_nids'] = array(
      '#type' => 'select',
      '#title' => t('Policies'),
      '#options' => $this->policy_options(),
      '#default_value' => $this->default_policies,
      '#multiple' => TRUE,
      '#required' => TRUE,
    );

    $form['include_review'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include review rows'),
      '#description' => t('Review rows are months before the employer\'s earliest rate takes effect (or with no rate history), where no charge is expected yet. Normally hidden.'),
      '#default_value' => 0,
    );

    return $form;
  }

  /**
   * 'YYYY-MM' => 'Month YYYY', January five years back through this month.
   * UI range only; widen if older history is ever needed.
   */
  public function month_options() {
    $options = array();
    $ts = mktime(0, 0, 0, 1, 1, date('Y') - 5);
    $last = strtotime(date('Y-m-01'));
    while ($ts <= $last) {
      $options[date('Y-m', $ts)] = date('F Y', $ts);
      $ts = strtotime('+1 month', $ts);
    }
    return $options;
  }

  /**
   * Every policy that any election references, so the list tracks the data.
   */
  public function policy_options() {
    $sql = "select distinct n.nid, n.title ";
    $sql .= "from field_data_field_sirius_trust_policy pol ";
    $sql .= "join node n on n.nid = pol.field_sirius_trust_policy_target_id ";
    $sql .= "where pol.entity_type = 'node' ";
    $sql .= "and pol.bundle = 'sirius_trust_worker_election' ";
    $sql .= "and pol.deleted = 0 ";
    $sql .= "order by n.title ";
    $stmt = sirius_sql_query($sql, array());

    $options = array();
    while ($hr = $stmt->fetchAssoc()) {
      $options[$hr['nid']] = $hr['title'];
    }
    return $options;
  }

  public function form_validate($form, &$form_state) {
    $parameters = $form_state['values']['parameters'];

    $start = $parameters['start_month'];
    $end = $parameters['end_month'];

    if (!preg_match('/^\d{4}-\d{2}$/', $start)) {
      form_set_error("parameters][start_month", 'Please select a from month.');
    }
    if (!preg_match('/^\d{4}-\d{2}$/', $end)) {
      form_set_error("parameters][end_month", 'Please select a to month.');
    }
    if ($start && $end && strcmp($end, $start) < 0) {
      form_set_error("parameters][end_month", 'To month cannot be before from month.');
    }
    if (!$this->selected_policies($parameters)) {
      form_set_error("parameters][policy_nids", 'Please select at least one policy.');
    }
  }

  public function render_parameters($parameters) {
    $parameters = $parameters['parameters'];

    return sirius_jsonfield_util_render_all_values_new(
      $this->form(),
      $parameters,
      array(
        'skip_empty' => TRUE,
        'no_fieldsets' => TRUE,
      )
    );
  }

  protected function selected_policies($parameters) {
    $nids = array();
    if (is_array($parameters['policy_nids'])) {
      foreach ($parameters['policy_nids'] as $nid) {
        $nid = intval($nid);
        if ($nid) { $nids[$nid] = $nid; }
      }
    }
    return array_values($nids);
  }

  /**
   * Months in the selected range whose charge date (the 5th) has passed.
   * Returns ym => 'YYYY-MM-05 00:00:00'.
   */
  protected function charge_months($start_month, $end_month) {
    $today = date('Y-m-d');
    $months = array();
    $ts = strtotime($start_month . '-01');
    $last = strtotime($end_month . '-01');
    while ($ts <= $last) {
      $ym = date('Y-m', $ts);
      if ($ym . '-05' < $today) {
        $months[$ym] = $ym . '-05 00:00:00';
      }
      $ts = strtotime('+1 month', $ts);
    }
    return $months;
  }

  public function pks(&$form_state) {
    $parameters = $form_state['values']['parameters'];

    $months = $this->charge_months($parameters['start_month'], $parameters['end_month']);
    $policies = $this->selected_policies($parameters);
    if (!$months || !$policies) { return array(); }

    $args = array();

    // Month list as a derived table. Each placeholder is used once.
    $month_sql = array();
    $i = 0;
    foreach ($months as $ym => $charge_ts) {
      $month_sql[] = "select :ym_$i as ym, :cd_$i as charge_date";
      $args[":ym_$i"] = $ym;
      $args[":cd_$i"] = $charge_ts;
      ++$i;
    }

    $policy_sql = array();
    foreach ($policies as $i => $policy_nid) {
      $policy_sql[] = ":policy_$i";
      $args[":policy_$i"] = $policy_nid;
    }

    $benefit_sql = array();
    $i = 0;
    foreach ($this->benefit_uuids as $benefit_nid => $uuid) {
      $benefit_sql[] = ":benefit_$i";
      $args[":benefit_$i"] = $benefit_nid;
      ++$i;
    }

    $args[':ledger_account'] = $this->ledger_account;

    $sql = "select distinct m.ym, ";
    $sql .= "pol.field_sirius_trust_policy_target_id as policy_nid, ";
    $sql .= "wk.nid as worker_nid, ";
    $sql .= "esh.field_grievance_shop_target_id as shop_nid, ";
    $sql .= "ben.field_sirius_trust_benefit_target_id as benefit_nid, ";
    $sql .= "pn.title as policy_title, sn.title as shop_title, wk.title as worker_title ";
    $sql .= "from (" . implode(' union all ', $month_sql) . ") m ";

    // Election: supplies policy and employer.
    $sql .= "join field_data_field_sirius_trust_policy pol ";
    $sql .= "  on pol.entity_type = 'node' and pol.bundle = 'sirius_trust_worker_election' and pol.deleted = 0 ";
    $sql .= "  and pol.field_sirius_trust_policy_target_id in (" . implode(', ', $policy_sql) . ") ";
    $sql .= "join node pn on pn.nid = pol.field_sirius_trust_policy_target_id ";
    $sql .= "join node e on e.nid = pol.entity_id and e.status = 1 ";
    $sql .= "join field_data_field_sirius_worker ew ";
    $sql .= "  on ew.entity_type = 'node' and ew.bundle = 'sirius_trust_worker_election' ";
    $sql .= "  and ew.entity_id = e.nid and ew.deleted = 0 ";
    $sql .= "join node wk on wk.nid = ew.field_sirius_worker_target_id ";
    $sql .= "join field_data_field_sirius_date_start eds ";
    $sql .= "  on eds.entity_type = 'node' and eds.entity_id = e.nid and eds.deleted = 0 ";
    $sql .= "left join field_data_field_sirius_date_end ede ";
    $sql .= "  on ede.entity_type = 'node' and ede.entity_id = e.nid and ede.deleted = 0 ";
    $sql .= "left join field_data_field_grievance_shop esh ";
    $sql .= "  on esh.entity_type = 'node' and esh.entity_id = e.nid and esh.deleted = 0 ";
    $sql .= "left join node sn on sn.nid = esh.field_grievance_shop_target_id ";

    // WMB: the worker's own coverage (covered person = subscriber).
    $sql .= "join field_data_field_sirius_worker bw ";
    $sql .= "  on bw.entity_type = 'node' and bw.bundle = 'sirius_trust_worker_benefit' ";
    $sql .= "  and bw.deleted = 0 and bw.field_sirius_worker_target_id = wk.nid ";
    $sql .= "join node wb on wb.nid = bw.entity_id and wb.status = 1 ";
    $sql .= "join field_data_field_sirius_trust_subscriber sub ";
    $sql .= "  on sub.entity_type = 'node' and sub.bundle = 'sirius_trust_worker_benefit' ";
    $sql .= "  and sub.entity_id = wb.nid and sub.deleted = 0 ";
    $sql .= "  and sub.field_sirius_trust_subscriber_target_id = wk.nid ";
    // The charge generator only counts coverage with no contact-relation
    // (sirius_trust_worker_benefits_search relationship_nid = 'NULL'). A record
    // that names its subscriber as the covered person but carries a relation is
    // dependent coverage to the generator, so it can never produce a charge.
    $sql .= "left join field_data_field_sirius_contact_relation rel ";
    $sql .= "  on rel.entity_type = 'node' and rel.entity_id = wb.nid and rel.deleted = 0 ";
    $sql .= "join field_data_field_sirius_trust_benefit ben ";
    $sql .= "  on ben.entity_type = 'node' and ben.entity_id = wb.nid and ben.deleted = 0 ";
    $sql .= "  and ben.field_sirius_trust_benefit_target_id in (" . implode(', ', $benefit_sql) . ") ";
    $sql .= "join field_data_field_sirius_date_start bds ";
    $sql .= "  on bds.entity_type = 'node' and bds.entity_id = wb.nid and bds.deleted = 0 ";
    $sql .= "left join field_data_field_sirius_date_end bde ";
    $sql .= "  on bde.entity_type = 'node' and bde.entity_id = wb.nid and bde.deleted = 0 ";

    // Anti-join: one charge per worker per month, keyed 'worker_nid|YYYY-MM'.
    $sql .= "left join sirius_ledger_ar ar ";
    $sql .= "  on ar.ledger_key = concat(wk.nid, '|', m.ym) and ar.ledger_account = :ledger_account ";

    $sql .= "where ar.ledger_id is null ";
    $sql .= "and rel.field_sirius_contact_relation_target_id is null ";
    $sql .= "and eds.field_sirius_date_start_value <= m.charge_date ";
    $sql .= "and (ede.field_sirius_date_end_value is null or ede.field_sirius_date_end_value > m.charge_date) ";
    $sql .= "and bds.field_sirius_date_start_value <= m.charge_date ";
    $sql .= "and (bde.field_sirius_date_end_value is null or bde.field_sirius_date_end_value > m.charge_date) ";

    $stmt = sirius_sql_query($sql, $args);

    // Collapse benefits to one entry per worker/policy/shop/month.
    $groups = array();
    while ($hr = $stmt->fetchAssoc()) {
      $gkey = $hr['ym'] . '|' . intval($hr['policy_nid']) . '|' . intval($hr['worker_nid']) . '|' . intval($hr['shop_nid']);
      if (!isset($groups[$gkey])) {
        $groups[$gkey] = array(
          'ym' => $hr['ym'],
          'policy_nid' => intval($hr['policy_nid']),
          'worker_nid' => intval($hr['worker_nid']),
          'shop_nid' => intval($hr['shop_nid']),
          'policy_title' => $hr['policy_title'],
          'shop_title' => $hr['shop_title'],
          'worker_title' => $hr['worker_title'],
          'benefits' => array(),
        );
      }
      $groups[$gkey]['benefits'][intval($hr['benefit_nid'])] = intval($hr['benefit_nid']);
    }

    usort($groups, array($this, 'sort_groups'));

    $pks = array();
    foreach ($groups as $g) {
      ksort($g['benefits']);
      $pks[] = $g['ym'] . '|' . $g['policy_nid'] . '|' . $g['worker_nid'] . '|' . $g['shop_nid'] . '|' . implode(',', $g['benefits']);
    }

    return array_chunk($pks, 100);
  }

  /** Month descending, then policy, employer, worker title. */
  public function sort_groups($a, $b) {
    $cmp = strcmp($b['ym'], $a['ym']);
    if ($cmp) { return $cmp; }
    $cmp = strcasecmp($a['policy_title'], $b['policy_title']);
    if ($cmp) { return $cmp; }
    $cmp = strcasecmp($a['shop_title'], $b['shop_title']);
    if ($cmp) { return $cmp; }
    $cmp = strcasecmp($a['worker_title'], $b['worker_title']);
    if ($cmp) { return $cmp; }
    return $a['worker_nid'] - $b['worker_nid'];
  }

  public function batch($pks, $values, &$context) {
    $parameters = $values['parameters'];
    $include_review = !empty($parameters['include_review']);

    // Split the keys and collect every nid we need a title for.
    $parsed = array();
    $title_nids = array();
    $shop_nids = array();
    foreach ($pks as $pk) {
      list($ym, $policy_nid, $worker_nid, $shop_nid, $benefit_list) = explode('|', $pk);
      $benefit_nids = array_map('intval', explode(',', $benefit_list));
      $parsed[$pk] = array($ym, intval($policy_nid), intval($worker_nid), intval($shop_nid), $benefit_nids);

      $title_nids[intval($policy_nid)] = TRUE;
      $title_nids[intval($worker_nid)] = TRUE;
      if ($shop_nid) {
        $title_nids[intval($shop_nid)] = TRUE;
        $shop_nids[intval($shop_nid)] = TRUE;
      }
      foreach ($benefit_nids as $b) { $title_nids[$b] = TRUE; }
    }
    $this->load_titles(array_keys($title_nids));
    $this->load_shop_json(array_keys($shop_nids));

    $rows = array();
    foreach ($parsed as $pk => $p) {
      list($ym, $policy_nid, $worker_nid, $shop_nid, $benefit_nids) = $p;

      // No employer on the election: no shop settings, so nothing to report
      // (matches the original query).
      if (!$shop_nid) { continue; }

      $month_start = $ym . '-01';
      $benefit_titles = array();
      $earliest = NULL;

      foreach ($benefit_nids as $benefit_nid) {
        $uuid = $this->benefit_uuids[$benefit_nid];
        $settings = $this->shop_settings($shop_nid, $uuid);
        if ($settings === NULL) { continue; }

        $benefit_titles[] = $this->title($benefit_nid);
        $rate_date = $this->earliest_rate_date($settings);
        if ($earliest === NULL || $rate_date < $earliest) { $earliest = $rate_date; }
      }
      if (!$benefit_titles) { continue; }
      sort($benefit_titles);

      // No rate history reads as 9999-12-31, which is also "not yet effective".
      $is_review = ($earliest > $month_start);
      if ($is_review && !$include_review) { continue; }

      $note = '';
      if ($is_review) {
        $note = ($earliest == '9999-12-31') ? 'no rate history - review' : 'rate not yet effective - review';
      }

      $row = array();
      $row['charge_month']   = $ym;
      $row['charge_date']    = $ym . '-05';
      $row['policy_nid']     = $policy_nid;
      $row['policy']         = $this->title($policy_nid);
      $row['worker_nid']     = $worker_nid;
      $row['worker']         = $this->title($worker_nid);
      $row['employer_nid']   = $shop_nid;
      $row['employer']       = $this->title($shop_nid);
      $row['benefits']       = implode(', ', $benefit_titles);
      $row['rate_effective'] = ($earliest == '9999-12-31') ? '' : $earliest;
      $row['note']           = $note;

      $rows[$pk] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }

  /** One grouped title query per chunk for ids not already cached. */
  protected function load_titles($nids) {
    $need = array();
    foreach ($nids as $nid) {
      if (!isset($this->title_cache[$nid])) { $need[] = $nid; }
    }
    if (!$need) { return; }

    $args = array();
    $in = array();
    foreach ($need as $i => $nid) {
      $in[] = ":nid_$i";
      $args[":nid_$i"] = $nid;
    }
    $stmt = sirius_sql_query("select nid, title from node where nid in (" . implode(', ', $in) . ")", $args);
    while ($hr = $stmt->fetchAssoc()) {
      $this->title_cache[$hr['nid']] = $hr['title'];
    }
  }

  protected function title($nid) {
    return isset($this->title_cache[$nid]) ? $this->title_cache[$nid] : '';
  }

  /** One grouped query per chunk for shop JSON not already cached. */
  protected function load_shop_json($shop_nids) {
    $need = array();
    foreach ($shop_nids as $nid) {
      if (!array_key_exists($nid, $this->shop_json_cache)) { $need[] = $nid; }
    }
    if (!$need) { return; }

    $args = array();
    $in = array();
    foreach ($need as $i => $nid) {
      $in[] = ":shop_$i";
      $args[":shop_$i"] = $nid;
      $this->shop_json_cache[$nid] = NULL;
    }
    $sql = "select sj.entity_id, sj.field_sirius_json_value ";
    $sql .= "from field_data_field_sirius_json sj ";
    $sql .= "where sj.entity_type = 'node' and sj.bundle = 'grievance_shop' and sj.deleted = 0 ";
    $sql .= "and sj.entity_id in (" . implode(', ', $in) . ") ";
    $stmt = sirius_sql_query($sql, $args);
    while ($hr = $stmt->fetchAssoc()) {
      $json = json_decode($hr['field_sirius_json_value'], TRUE);
      $this->shop_json_cache[$hr['entity_id']] = is_array($json) ? $json : NULL;
    }
  }

  /** charge_plugins.settings.<uuid> for a shop, or NULL if absent. */
  protected function shop_settings($shop_nid, $uuid) {
    $json = $this->shop_json_cache[$shop_nid];
    if (!is_array($json)) { return NULL; }
    $settings = $json['charge_plugins']['settings'][$uuid];
    return is_array($settings) ? $settings : NULL;
  }

  /**
   * Earliest 'date' in settings.rates.history as Y-m-d, or '9999-12-31' if
   * there is no usable history. The original query compared the first and
   * last entries; the minimum over all entries is the same when the history
   * is ordered either way.
   */
  protected function earliest_rate_date($settings) {
    $earliest = '9999-12-31';
    $history = $settings['rates']['history'];
    if (!is_array($history)) { return $earliest; }

    foreach ($history as $entry) {
      $date = substr(trim($entry['date']), 0, 10);
      if (!preg_match('/^\d{4}-\d{2}-\d{2}$/', $date)) { continue; }
      if ($date < $earliest) { $earliest = $date; }
    }
    return $earliest;
  }
}
