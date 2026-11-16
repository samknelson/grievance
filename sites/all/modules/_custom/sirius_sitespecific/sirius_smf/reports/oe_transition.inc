<?php

/**
 * OE Transition: Summary
 *
 * How subscribers moved between plans across an open-enrollment boundary,
 * for trustees. Compares each subscriber's elected plan on the last day of
 * one plan year (Dec 31) with their elected plan on the first day of the
 * next (Jan 1), per benefit type, and counts the moves.
 *
 * One row = one From-plan -> To-plan pair within a benefit type, plus an
 * ALL PLANS total row per benefit type. One table per run (Sirius reports
 * produce a single sheet). Rows are always grouped by benefit type in the
 * fixed order of $type_order (Medical, then Dental) with ALL PLANS last in
 * each type; the "Sort By" parameter picks the order within a type:
 *   pct   - highest % of From-Plan first (default)
 *   alpha - From plan, then To plan, A-Z
 * Run it twice for both views.
 *
 * Row order: the framework stores rows keyed by report_row_pk and both the
 * preview and the CSV download read them back with no ORDER BY, so they
 * come out in primary-key order, i.e. sorted by row key. Row keys here are
 * zero-padded sort keys ('TT-NNNNN') so that order IS the sort above.
 * Do not change the key format without keeping it lexically sortable.
 *
 * Rules (carried over unchanged from the ad hoc query Jaron reviewed)
 *   - Source is the ELECTION (sirius_trust_worker_election), not WMB
 *     coverage: the question is what people elected at OE, not what
 *     coverage was granted. An election is "on" a date when
 *     start <= date and (end is null or end >= date), compared on the
 *     date part only.
 *   - A subscriber holding two plans of one type on a date is shown as
 *     "Plan A + Plan B". Such combinations count in the From-plan base and
 *     the ALL PLANS base, but are never counted as a move: the pair rows
 *     list single plans only.
 *   - Subscribers with no election before (new) or none after are in the
 *     base only when they had a plan before, and never count as moved.
 *   - Legacy plans (title starting with one of $legacy_prefixes) are
 *     ignored when "Exclude legacy plans" is on.
 *   - % of From-Plan = moved / everyone on the From-plan before (including
 *     those who stayed). Shown as text like '12.50%'.
 *
 * pks():   a single key; the summary is an aggregate across all workers,
 *          so it cannot be split over chunks.
 * batch(): one query for every in-scope election overlapping the two
 *          dates; snapshotting, pairing and counting run in PHP.
 */

class Sirius_Smf_Report_Oe_Transition extends Report_Cache_Report_Generator {
  public $type = 'sirius-smf-oe-transition';

  // Benefit types offered, in display order. Benefit type IDs (field_sirius_id).
  public $type_order = array('MEDICAL', 'DENTAL');

  // Legacy plans, matched case-insensitively on the start of the plan title.
  public $legacy_prefixes = array('Placeholder Historical', 'United Concordia', 'EHS');

  public function info($result = array()) {
    $result = array(
      'name' => t('OE Transition: Summary'),
      'description' => t('Subscribers moving between medical and dental plans across open enrollment (Dec 31 vs Jan 1), by From and To plan.'),
      'access' => 'sirius trust staff',
      'preview' => array(
        'default_display_fields' => array(
          'benefit_type',
          'from_plan',
          'to_plan',
          'moved',
          'base',
          'pct',
        ),
        'preview_links' => FALSE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'reports/oe_transition.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {
    $fields = array();
    $fields['benefit_type'] = 'Benefit Type';
    $fields['from_plan']    = 'From';
    $fields['to_plan']      = 'To';
    $fields['moved']        = 'Subscribers Moved';
    $fields['base']         = 'Subscribers on From-Plan Before';
    $fields['pct']          = '% of From-Plan';
    return $fields;
  }

  public function preview_links($row) {
    return array();
  }

  public function form() {
    $form['plan_year'] = array(
      '#type' => 'select',
      '#title' => t('New Plan Year'),
      '#description' => t('Compares elections on December 31 of the prior year with elections on January 1 of this year.'),
      '#options' => $this->plan_year_options(),
      '#default_value' => date('Y') + 1,
      '#required' => TRUE,
    );

    $form['types'] = array(
      '#type' => 'select',
      '#title' => t('Benefit Types'),
      '#options' => $this->type_options(),
      '#default_value' => $this->type_order,
      '#multiple' => TRUE,
      '#required' => TRUE,
    );

    $form['sort'] = array(
      '#type' => 'select',
      '#title' => t('Sort By'),
      '#description' => t('Rows are always grouped Medical first, then Dental, with ALL PLANS last in each group.'),
      '#options' => array(
        'pct' => t('Highest % of From-Plan'),
        'alpha' => t('From plan, then To plan (A-Z)'),
      ),
      '#default_value' => 'pct',
      '#required' => TRUE,
    );

    $form['exclude_legacy'] = array(
      '#type' => 'checkbox',
      '#title' => t('Exclude legacy plans'),
      '#description' => t('Ignores plans whose name starts with: @list.', array('@list' => implode(', ', $this->legacy_prefixes))),
      '#default_value' => 1,
    );

    return $form;
  }

  /** Three plan years back through next year. */
  public function plan_year_options() {
    $options = array();
    for ($y = date('Y') + 1; $y >= date('Y') - 3; --$y) {
      $options[$y] = $y . ' (Dec 31, ' . ($y - 1) . ' to Jan 1, ' . $y . ')';
    }
    return $options;
  }

  /** Benefit type ID => name, limited to $type_order, in that order. */
  public function type_options() {
    $all = sirius_taxonomy_load('sirius_trust_benefit_type', 'field_sirius_id', 'name');
    $options = array();
    foreach ($this->type_order as $id) {
      $options[$id] = !empty($all[$id]) ? $all[$id] : $id;
    }
    return $options;
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
    // One aggregate over the whole population; see the header.
    return array('summary');
  }

  public function batch($pk, $values, &$context) {
    $parameters = $values['parameters'];

    $year = intval($parameters['plan_year']);
    if ($year < 2000) { return NULL; }
    $before_dt = ($year - 1) . '-12-31';
    $after_dt  = $year . '-01-01';

    // Selected types, kept in $type_order so Medical sorts before Dental.
    $selected = $parameters['types'];
    if (!is_array($selected)) { $selected = array($selected); }
    $types = array();
    foreach ($this->type_order as $id) {
      if (in_array($id, $selected)) { $types[] = $id; }
    }
    if (!$types) { return NULL; }

    $sort = ($parameters['sort'] == 'alpha') ? 'alpha' : 'pct';
    $exclude_legacy = $parameters['exclude_legacy'] ? TRUE : FALSE;
    $type_names = $this->type_options();

    // ------------------------------------------------------------------
    // Snapshot: worker + type => plans held on each date.
    // ------------------------------------------------------------------
    $snap = array();
    foreach ($this->election_rows($types, $before_dt, $after_dt) as $hr) {
      if ($exclude_legacy && $this->is_legacy($hr['plan'])) { continue; }

      $start = substr($hr['d_start'], 0, 10);
      $end = $hr['d_end'] ? substr($hr['d_end'], 0, 10) : NULL;
      $on_before = ($start <= $before_dt) && ($end === NULL || $end >= $before_dt);
      $on_after  = ($start <= $after_dt)  && ($end === NULL || $end >= $after_dt);
      if (!$on_before && !$on_after) { continue; }

      // The SQL type match is case-insensitive; the PHP keys are not.
      $btype = strtoupper($hr['btype']);
      if (!in_array($btype, $types)) { continue; }

      $key = $hr['worker_nid'] . '|' . $btype;
      if (!isset($snap[$key])) {
        $snap[$key] = array('btype' => $btype, 'before' => array(), 'after' => array());
      }
      if ($on_before) { $snap[$key]['before'][$hr['plan']] = TRUE; }
      if ($on_after)  { $snap[$key]['after'][$hr['plan']] = TRUE; }
    }

    // ------------------------------------------------------------------
    // Count per type: single plans seen, base per From-plan, moves per pair.
    // ------------------------------------------------------------------
    $plans = array();   // btype => plan => TRUE (single plans only)
    $base  = array();   // btype => from_plan (may be a combo) => n
    $moves = array();   // btype => from => to => n
    $total_moved = array();
    $total_base  = array();
    foreach ($types as $t) {
      $plans[$t] = array();
      $base[$t] = array();
      $moves[$t] = array();
      $total_moved[$t] = 0;
      $total_base[$t] = 0;
    }

    foreach ($snap as $s) {
      $t = $s['btype'];
      $before = array_keys($s['before']);
      $after = array_keys($s['after']);
      usort($before, 'strcasecmp');
      usort($after, 'strcasecmp');
      $plan_before = implode(' + ', $before);
      $plan_after = implode(' + ', $after);

      if (count($before) == 1) { $plans[$t][$plan_before] = TRUE; }
      if (count($after) == 1)  { $plans[$t][$plan_after] = TRUE; }

      if ($before) {
        if (!isset($base[$t][$plan_before])) { $base[$t][$plan_before] = 0; }
        $base[$t][$plan_before]++;
        $total_base[$t]++;
      }

      // A move: on exactly one plan on each date, and they differ.
      if (count($before) == 1 && count($after) == 1 && $plan_before !== $plan_after) {
        if (!isset($moves[$t][$plan_before][$plan_after])) { $moves[$t][$plan_before][$plan_after] = 0; }
        $moves[$t][$plan_before][$plan_after]++;
        $total_moved[$t]++;
      }
    }

    // ------------------------------------------------------------------
    // Rows. Key 'TT-NNNNN' = type rank, position; see header.
    // ------------------------------------------------------------------
    $rows = array();
    foreach ($types as $rank => $t) {
      $type_label = !empty($type_names[$t]) ? $type_names[$t] : $t;

      $plan_list = array_keys($plans[$t]);
      usort($plan_list, 'strcasecmp');

      $pairs = array();
      foreach ($plan_list as $from) {
        foreach ($plan_list as $to) {
          if ($from === $to) { continue; }
          $moved = isset($moves[$t][$from][$to]) ? $moves[$t][$from][$to] : 0;
          $n = isset($base[$t][$from]) ? $base[$t][$from] : 0;
          $pairs[] = array(
            'from' => $from,
            'to' => $to,
            'moved' => $moved,
            'base' => $n,
            'pct' => $n ? (100 * $moved / $n) : NULL,
          );
        }
      }

      $all = array(
        'from' => 'ALL PLANS',
        'to' => 'ALL PLANS',
        'moved' => $total_moved[$t],
        'base' => $total_base[$t],
        'pct' => $total_base[$t] ? (100 * $total_moved[$t] / $total_base[$t]) : NULL,
      );

      // 'alpha': $pairs is already From, then To alphabetical (nested loops
      // over the sorted plan list), so leave it alone.
      // 'pct': highest % first; blank % (nobody on the From-plan) last; ties
      // by most moved, then From, To alphabetically.
      // Either way ALL PLANS goes last.
      if ($sort == 'pct') {
        usort($pairs, function ($a, $b) {
          if ($a['pct'] === NULL && $b['pct'] !== NULL) { return 1; }
          if ($b['pct'] === NULL && $a['pct'] !== NULL) { return -1; }
          if ($a['pct'] != $b['pct']) { return ($a['pct'] < $b['pct']) ? 1 : -1; }
          if ($a['moved'] != $b['moved']) { return ($a['moved'] < $b['moved']) ? 1 : -1; }
          $c = strcasecmp($a['from'], $b['from']);
          if ($c) { return $c; }
          return strcasecmp($a['to'], $b['to']);
        });
      }
      $seq = 0;
      foreach ($pairs as $p) {
        $rows[$this->row_key($rank, ++$seq)] = $this->row($type_label, $p);
      }
      $rows[$this->row_key($rank, ++$seq)] = $this->row($type_label, $all);
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }

  /**
   * Every election of a selected type whose window touches the two dates.
   * One row per election + benefit. Date tests use the date part only, as
   * the ad hoc query did with DATE(); the final on/off test runs in PHP.
   */
  public function election_rows($types, $before_dt, $after_dt) {
    $type_list = array();
    foreach ($types as $t) { $type_list[] = "'" . db_escape_table($t) . "'"; }
    $type_list = implode(',', $type_list);

    $sql = "select ew.field_sirius_worker_target_id as worker_nid, ";
    $sql .= "bt.field_sirius_id_value as btype, ";
    $sql .= "bn.title as plan, ";
    $sql .= "eds.field_sirius_date_start_value as d_start, ";
    $sql .= "ede.field_sirius_date_end_value as d_end ";
    $sql .= "from field_data_field_sirius_worker ew ";
    $sql .= "join node e on e.nid = ew.entity_id and e.type = 'sirius_trust_worker_election' ";
    $sql .= "join node wk on wk.nid = ew.field_sirius_worker_target_id and wk.type = 'sirius_worker' ";
    $sql .= "join field_data_field_sirius_trust_benefits eb ";
    $sql .= "  on eb.entity_type = 'node' and eb.entity_id = e.nid and eb.deleted = 0 ";
    $sql .= "join node bn on bn.nid = eb.field_sirius_trust_benefits_target_id ";
    $sql .= "join field_data_field_sirius_trust_benefit_type bty ";
    $sql .= "  on bty.entity_type = 'node' and bty.entity_id = bn.nid and bty.deleted = 0 ";
    $sql .= "join field_data_field_sirius_id bt ";
    $sql .= "  on bt.entity_type = 'taxonomy_term' and bt.deleted = 0 ";
    $sql .= "  and bt.entity_id = bty.field_sirius_trust_benefit_type_tid ";
    $sql .= "  and bt.field_sirius_id_value in ($type_list) ";
    $sql .= "join field_data_field_sirius_date_start eds ";
    $sql .= "  on eds.entity_type = 'node' and eds.entity_id = e.nid and eds.deleted = 0 ";
    $sql .= "left join field_data_field_sirius_date_end ede ";
    $sql .= "  on ede.entity_type = 'node' and ede.entity_id = e.nid and ede.deleted = 0 ";
    $sql .= "where ew.entity_type = 'node' ";
    $sql .= "and ew.bundle = 'sirius_trust_worker_election' ";
    $sql .= "and ew.deleted = 0 ";
    // Starts on or before the after-date (any time that day).
    $sql .= "and eds.field_sirius_date_start_value < :after_next ";
    // Not ended before the before-date.
    $sql .= "and (ede.field_sirius_date_end_value is null or ede.field_sirius_date_end_value >= :before_start) ";

    $stmt = sirius_sql_query($sql, array(
      ':after_next' => date('Y-m-d', strtotime($after_dt . ' +1 day')) . ' 00:00:00',
      ':before_start' => $before_dt . ' 00:00:00',
    ));

    $out = array();
    while ($hr = $stmt->fetchAssoc()) { $out[] = $hr; }
    return $out;
  }

  public function is_legacy($title) {
    foreach ($this->legacy_prefixes as $prefix) {
      if (stripos($title, $prefix) === 0) { return TRUE; }
    }
    return FALSE;
  }

  public function row_key($rank, $seq) {
    return sprintf('%02d-%05d', $rank, $seq);
  }

  public function row($type_label, $p) {
    return array(
      'benefit_type' => $type_label,
      'from_plan' => $p['from'],
      'to_plan' => $p['to'],
      'moved' => $p['moved'],
      'base' => $p['base'],
      'pct' => ($p['pct'] === NULL) ? '' : number_format($p['pct'], 2) . '%',
    );
  }
}
