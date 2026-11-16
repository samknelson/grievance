<?php

/**
 * Elections Missing AD&D / Life
 *
 * One row per ACTIVE worker election that is missing AD&D, Life, or both, where the election's policy enables the
 * missing benefit. Elections only: this report says nothing about worker-benefit (WMB) records. AD&D and Life are
 * not waivable, so an active election should always list both; a row here is a data problem to correct, not a
 * member choice. (Whether the worker was actually granted the benefit is a separate, eligibility-driven question
 * and is deliberately out of scope.)
 *
 * Intended as a monthly check for the missing/misaligned benefits review until S2, after the one-time backfill
 * that added AD&D and Life to every active election (Nov 2026). Rows should be rare and recent; a spike points at
 * an import, a script, or a wizard regression.
 *
 * Rules
 *   - "Active" is the election's own Active flag (field_sirius_active = 'Yes'), the same test the rest of the
 *     system uses to find a worker's current election. This report looks at the present only, so the flag (which
 *     is derived from the election's date window relative to today) is the right test here.
 *   - Published elections only (node status = 1).
 *   - COBRA is excluded. COBRA members do not get AD&D or Life (fund ruling), and the COBRA policy does not enable
 *     either benefit. The test is done by policy, not by name: a missing benefit is only reported if the
 *     election's policy enables it (policy JSON benefits.enabled). That also drops the "Inactive" policy.
 *     Disability workers DO get both and are not excluded.
 *   - Elections with no policy are excluded. About 188 old import stubs (no policy, no benefits) are known junk
 *     that will not transfer to S2; they would drown out real findings.
 *   - The election start date shown is the ENROLLMENT date, not the coverage start date.
 *   - No worker names: Worker NID only.
 *
 * Parameters
 *   - Which benefit is missing: either (default) / AD&D / Life / both.
 *   - Hours filter: any (default) / hours since the July 2025 transition / hours in the last 3 months.
 *     "Had hours" = a sirius_hours_cache row with hours > 0 for a month on or after the cutoff, any employer.
 *
 * pks():   one population query (active, published, has-policy elections missing either benefit, optional hours
 *          test), then policy filtering in PHP. Key: 'election_nid|policy_nid|ADD, LIFE or ADD+LIFE'.
 *          Newest elections first.
 * batch(): one grouped query for the chunk's election facts, one for titles, one for each worker's latest hours
 *          month. No node_load, no per-record queries.
 */

class Sirius_Smf_Report_Elections_Missing_Addlife extends Report_Cache_Report_Generator {
  public $type = 'sirius-smf-elections-missing-addlife';

  /** First month of the post-transition period (S1 went live on the new rules in July 2025). */
  const TRANSITION_YM = 202507;

  protected $policy_enabled_cache = array();
  protected $title_cache = array();

  public function info($result = array()) {
    $result = array(
      'name' => t('Elections Missing AD&D / Life'),
      'description' => t('Active elections that are missing AD&D and/or Life (COBRA excluded). Monthly data-quality check.'),
      'access' => 'sirius trust staff',
      'preview' => array(
        'default_display_fields' => array(
          'election_nid',
          'worker_nid',
          'policy',
          'employer',
          'missing',
          'election_start',
          'election_created',
          'last_hours_month',
        ),
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'reports/elections_missing_addlife.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {
    $fields = array();
    $fields['election_nid']          = 'Election NID';
    $fields['worker_nid']            = 'Worker NID';
    $fields['policy']                = 'Policy';
    $fields['employer']              = 'Employer';
    $fields['missing']               = 'Missing';
    $fields['election_start']        = 'Election Start (enrollment)';
    $fields['election_created']      = 'Election Created';
    $fields['created_by_uid']        = 'Created By (uid)';
    $fields['last_hours_month']      = 'Latest Hours Month';
    $fields['hours_since_transition'] = 'Hours Since July 2025';
    return $fields;
  }

  public function preview_links($row) {
    $links = array();
    if ($row['election_nid']) { $links['node/' . $row['election_nid']] = 'Election'; }
    if ($row['worker_nid']) {
      $links['node/' . $row['worker_nid']] = 'Worker';
      $links['node/' . $row['worker_nid'] . '/sirius_worker_hours'] = 'Hours';
    }
    return $links;
  }

  public function form() {
    $form['missing'] = array(
      '#type' => 'select',
      '#title' => t('Which benefit is missing'),
      '#options' => array(
        'either' => t('AD&D or Life (either)'),
        'add' => t('AD&D missing'),
        'life' => t('Life missing'),
        'both' => t('Both missing'),
      ),
      '#default_value' => 'either',
    );

    $form['hours'] = array(
      '#type' => 'select',
      '#title' => t('Worker had hours'),
      '#options' => array(
        'any' => t('Any (all active elections)'),
        'transition' => t('Since the July 2025 transition'),
        'recent' => t('In the last 3 months'),
      ),
      '#default_value' => 'any',
    );

    return $form;
  }

  public function form_validate($form, &$form_state) {
    // Both parameters are selects with fixed options; nothing to cross-check.
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

  public function pks(&$form_state) {
    $parameters = $form_state['values']['parameters'];
    $missing_filter = $parameters['missing'] ? $parameters['missing'] : 'either';
    $hours_filter = $parameters['hours'] ? $parameters['hours'] : 'any';

    // Look the benefits up by their ids rather than hardcoding nids.
    $add_nid = sirius_find_nid_by_id('ADD', 'sirius_trust_benefit');
    $life_nid = sirius_find_nid_by_id('2', 'sirius_trust_benefit');
    if (!$add_nid || !$life_nid) {
      drupal_set_message(t('Could not find the AD&D (id ADD) and Life (id 2) benefits.'), 'error');
      return array();
    }
    $add_nid = intval($add_nid);
    $life_nid = intval($life_nid);

    $sql_args = array();
    $sql = "select distinct n.nid as election_nid, n.created, ";
    $sql .= "  pol.field_sirius_trust_policy_target_id as policy_nid, ";
    $sql .= "  (ba.entity_id is not null) as has_add, ";
    $sql .= "  (bl.entity_id is not null) as has_life ";
    $sql .= "from node n ";
    // Active election (the flag is computed from the date window; this report is about the present).
    $sql .= "join field_data_field_sirius_active act on act.entity_type = 'node' and act.bundle = 'sirius_trust_worker_election' and act.deleted = 0 and act.entity_id = n.nid and act.field_sirius_active_value = 'Yes' ";
    // Inner join: elections with no policy are known junk and are excluded.
    $sql .= "join field_data_field_sirius_trust_policy pol on pol.entity_type = 'node' and pol.bundle = 'sirius_trust_worker_election' and pol.deleted = 0 and pol.entity_id = n.nid and pol.field_sirius_trust_policy_target_id is not null and pol.field_sirius_trust_policy_target_id > 0 ";
    $sql .= "left join field_data_field_sirius_trust_benefits ba on ba.entity_type = 'node' and ba.bundle = 'sirius_trust_worker_election' and ba.deleted = 0 and ba.entity_id = n.nid and ba.field_sirius_trust_benefits_target_id = :add_nid ";
    $sql .= "left join field_data_field_sirius_trust_benefits bl on bl.entity_type = 'node' and bl.bundle = 'sirius_trust_worker_election' and bl.deleted = 0 and bl.entity_id = n.nid and bl.field_sirius_trust_benefits_target_id = :life_nid ";
    $sql_args[':add_nid'] = $add_nid;
    $sql_args[':life_nid'] = $life_nid;

    if ($hours_filter != 'any') {
      $sql .= "join field_data_field_sirius_worker w on w.entity_type = 'node' and w.bundle = 'sirius_trust_worker_election' and w.deleted = 0 and w.entity_id = n.nid ";
    }

    $sql .= "where n.type = 'sirius_trust_worker_election' ";
    $sql .= "and n.status = 1 ";
    $sql .= "and (ba.entity_id is null or bl.entity_id is null) ";

    if ($hours_filter != 'any') {
      $cutoff_ym = ($hours_filter == 'transition') ? self::TRANSITION_YM : intval(date('Ym', strtotime('-3 months')));
      $sql .= "and exists (select 1 from sirius_hours_cache h where h.worker_nid = w.field_sirius_worker_target_id and h.year * 100 + h.month >= :cutoff_ym and h.hours > 0) ";
      $sql_args[':cutoff_ym'] = $cutoff_ym;
    }

    $sql .= "order by n.created desc, n.nid desc ";

    $stmt = sirius_sql_query($sql, $sql_args);

    $pks = array();
    while ($hr = $stmt->fetchAssoc()) {
      $policy_nid = intval($hr['policy_nid']);
      $enabled = $this->policy_enabled($policy_nid);

      // Only count a benefit as missing if the election's policy actually enables it (drops COBRA and Inactive).
      $missing_add = (!$hr['has_add'] && in_array($add_nid, $enabled));
      $missing_life = (!$hr['has_life'] && in_array($life_nid, $enabled));
      if (!$missing_add && !$missing_life) { continue; }

      if ($missing_filter == 'add' && !$missing_add) { continue; }
      if ($missing_filter == 'life' && !$missing_life) { continue; }
      if ($missing_filter == 'both' && !($missing_add && $missing_life)) { continue; }

      $label = ($missing_add && $missing_life) ? 'ADD+LIFE' : ($missing_add ? 'ADD' : 'LIFE');
      $pks[] = $hr['election_nid'] . '|' . $policy_nid . '|' . $label;
    }

    return array_chunk($pks, 100);
  }

  public function batch($pks, $values, &$context) {
    $parsed = array();
    $election_nids = array();
    $policy_nids = array();
    foreach ($pks as $pk) {
      list($election_nid, $policy_nid, $missing) = explode('|', $pk);
      $parsed[$pk] = array(intval($election_nid), intval($policy_nid), $missing);
      $election_nids[intval($election_nid)] = TRUE;
      $policy_nids[intval($policy_nid)] = TRUE;
    }
    if (!$parsed) { return array('is_multiple' => TRUE, 'rows' => array()); }

    // One grouped query for the election facts.
    $sql = "select n.nid as election_nid, n.created, n.uid, ";
    $sql .= "  w.field_sirius_worker_target_id as worker_nid, ";
    $sql .= "  sh.field_grievance_shop_target_id as shop_nid, ";
    $sql .= "  s.field_sirius_date_start_value as start_date ";
    $sql .= "from node n ";
    $sql .= "left join field_data_field_sirius_worker w on w.entity_type = 'node' and w.bundle = 'sirius_trust_worker_election' and w.deleted = 0 and w.entity_id = n.nid ";
    $sql .= "left join field_data_field_grievance_shop sh on sh.entity_type = 'node' and sh.bundle = 'sirius_trust_worker_election' and sh.deleted = 0 and sh.entity_id = n.nid ";
    $sql .= "left join field_data_field_sirius_date_start s on s.entity_type = 'node' and s.bundle = 'sirius_trust_worker_election' and s.deleted = 0 and s.entity_id = n.nid ";
    $sql .= "where n.nid in (" . implode(', ', array_keys($election_nids)) . ") ";
    $stmt = sirius_sql_query($sql, array());
    $facts = array();
    $worker_nids = array();
    $shop_nids = array();
    while ($hr = $stmt->fetchAssoc()) {
      $facts[$hr['election_nid']] = $hr;
      if ($hr['worker_nid']) { $worker_nids[intval($hr['worker_nid'])] = TRUE; }
      if ($hr['shop_nid']) { $shop_nids[intval($hr['shop_nid'])] = TRUE; }
    }

    // Titles for policies and employers only (never workers).
    $this->load_titles(array_merge(array_keys($policy_nids), array_keys($shop_nids)));

    // Latest month with hours, per worker, in one query.
    $last_hours = array();
    if ($worker_nids) {
      $sql = "select worker_nid, max(year * 100 + month) as last_ym ";
      $sql .= "from sirius_hours_cache ";
      $sql .= "where worker_nid in (" . implode(', ', array_keys($worker_nids)) . ") ";
      $sql .= "and hours > 0 ";
      $sql .= "group by worker_nid ";
      $stmt = sirius_sql_query($sql, array());
      while ($hr = $stmt->fetchAssoc()) { $last_hours[$hr['worker_nid']] = intval($hr['last_ym']); }
    }

    $rows = array();
    foreach ($parsed as $pk => $p) {
      list($election_nid, $policy_nid, $missing) = $p;
      if (!isset($facts[$election_nid])) { continue; }
      $f = $facts[$election_nid];

      $last_ym = isset($last_hours[$f['worker_nid']]) ? $last_hours[$f['worker_nid']] : 0;

      $row = array();
      $row['election_nid'] = $election_nid;
      $row['worker_nid'] = $f['worker_nid'];
      $row['policy'] = isset($this->title_cache[$policy_nid]) ? $this->title_cache[$policy_nid] : '';
      $row['employer'] = ($f['shop_nid'] && isset($this->title_cache[intval($f['shop_nid'])])) ? $this->title_cache[intval($f['shop_nid'])] : '';
      $row['missing'] = $missing;
      $row['election_start'] = $f['start_date'] ? substr($f['start_date'], 0, 10) : '';
      $row['election_created'] = date('Y-m-d', $f['created']);
      $row['created_by_uid'] = $f['uid'];
      $row['last_hours_month'] = $last_ym ? (substr($last_ym, 0, 4) . '-' . substr($last_ym, 4, 2)) : '';
      $row['hours_since_transition'] = ($last_ym >= self::TRANSITION_YM) ? 'Yes' : 'No';

      $rows[$pk] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }

  /** Benefit nids a policy enables (policy JSON benefits.enabled), cached per policy. */
  protected function policy_enabled($policy_nid) {
    if (!isset($this->policy_enabled_cache[$policy_nid])) {
      $enabled = array();
      $policy_node = node_load($policy_nid);
      if ($policy_node) {
        $json = sirius_json_get($policy_node);
        if (!empty($json['benefits']['enabled'])) {
          $enabled = array_map('intval', array_filter(array_values($json['benefits']['enabled'])));
        }
      }
      $this->policy_enabled_cache[$policy_nid] = $enabled;
    }
    return $this->policy_enabled_cache[$policy_nid];
  }

  /** Fill the title cache for a list of nids with one query. */
  protected function load_titles($nids) {
    $need = array();
    foreach ($nids as $nid) {
      $nid = intval($nid);
      if ($nid && !isset($this->title_cache[$nid])) { $need[$nid] = $nid; }
    }
    if (!$need) { return; }

    $stmt = sirius_sql_query("select nid, title from node where nid in (" . implode(', ', $need) . ")", array());
    while ($hr = $stmt->fetchAssoc()) { $this->title_cache[intval($hr['nid'])] = $hr['title']; }
  }
}
