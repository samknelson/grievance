<?php

/**
 * Dual Coverage: Election
 *
 * People who hold their own active election AND are covered as a dependent
 * on someone else's active election, where the two elections carry an
 * overlapping benefit TYPE (medical or dental). One row per
 * own-election / dependent-election pair.
 *
 * Terms for a reader with no fund knowledge:
 *   election  - a worker's enrollment record; carries one or more benefits
 *   dependent - a person covered under another worker's election, linked
 *               through a contact-relationship record
 *   WMB       - worker-month-benefit; the record proving coverage was
 *               actually granted, as opposed to merely elected
 *
 * pks():   candidate pairs plus the type-overlap test, as EXISTS rather
 *          than GROUP_CONCAT + HAVING LIKE, so the match on the benefit
 *          type ID is exact.
 * batch(): two grouped queries per chunk (elected benefits by election,
 *          granted WMB benefit types by election); the set intersections
 *          run in PHP instead of a per-row correlated subquery.
 *
 * The "granted in the past year" lookback is fixed at 12 months, carried
 * over from the source query. If that window is ever a fund decision
 * rather than a convention, it becomes a form parameter.
 */

class Sirius_Smf_Report_Dual_Coverage extends Report_Cache_Report_Generator {
  public $type = 'sirius-smf-dual-coverage';

  // Benefit type IDs that constitute a real overlap.
  public $overlap_types = array('MEDICAL', 'DENTAL');

  // Lookback for "was matching coverage actually granted?"
  public $granted_lookback_months = 12;

  public function info($result = array()) {
    $result = array(
      'name' => t('Dual Coverage: Election'),
      'description' => t('People holding their own election while also covered as a dependent on another election, with overlapping medical or dental coverage.'),
      'access' => 'sirius trust staff',
      'preview' => array(
        'default_display_fields' => array(
          'person',
          'relationship',
          'own_election_type',
          'own_types',
          'dep_subscriber',
          'dep_election_type',
          'dep_types',
          'overlapping_types',
          'granted_overlap_past_year',
        ),
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'reports/dual_coverage.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {
    $fields = array();
    $fields['person_nid']        = 'Person NID';
    $fields['person']            = 'Person';
    $fields['relationship']      = 'Relationship';
    $fields['relationship_nid']  = 'Relationship NID';
    $fields['own_election_nid']  = 'Own Election NID';
    $fields['own_election_type'] = 'Own Election Type';
    $fields['own_benefits']      = 'Own Benefits';
    $fields['own_types']         = 'Own Types';
    $fields['dep_election_nid']  = 'Dep Election NID';
    $fields['dep_election_type'] = 'Dep Election Type';
    $fields['dep_subscriber_nid'] = 'Dep Subscriber NID';
    $fields['dep_subscriber']    = 'Dep Subscriber';
    $fields['dep_benefits']      = 'Dep Benefits';
    $fields['dep_types']         = 'Dep Types';
    $fields['overlapping_types'] = 'Overlapping Types';
    $fields['granted_overlap_past_year'] = 'Granted Overlap (Past Year)';
    return $fields;
  }

  public function preview_links($row) {
    $links = array();
    if ($row['person_nid'])         { $links['node/' . $row['person_nid']] = 'Person'; }
    if ($row['dep_subscriber_nid']) { $links['node/' . $row['dep_subscriber_nid']] = 'Subscriber'; }
    if ($row['own_election_nid'])   { $links['node/' . $row['own_election_nid']] = 'Own Election'; }
    if ($row['dep_election_nid'])   { $links['node/' . $row['dep_election_nid']] = 'Dep Election'; }
    return $links;
  }

  public function form() {
    $form['granted_only'] = array(
      '#type' => 'checkbox',
      '#title' => t('Only rows with granted medical or dental overlap'),
      '#description' => t('Off by default. When on, a row is reported only if coverage of the SAME medical or dental type was actually granted on both elections within the past year — not merely elected on paper.'),
      '#default_value' => 0,
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
   * Candidate pairs: an active election owned by the person, and an active
   * election belonging to someone else on which the person appears as a
   * dependent, where the two share a medical or dental benefit type.
   */
  public function pks(&$form_state) {
    $types = array();
    foreach ($this->overlap_types as $t) { $types[] = "'" . db_escape_table($t) . "'"; }
    $types = implode(',', $types);

    $sql = "select distinct ";
    $sql .= "ow.field_sirius_worker_target_id as person_nid, ";
    $sql .= "ow.entity_id as own_election_nid, ";
    $sql .= "cr.entity_id as dep_election_nid, ";
    $sql .= "ra.entity_id as relationship_nid, ";
    $sql .= "dw.field_sirius_worker_target_id as dep_subscriber_nid ";

    $sql .= "from field_data_field_sirius_worker ow ";

    // The person's own election must be active.
    $sql .= "join field_data_field_sirius_active oa ";
    $sql .= "  on oa.entity_type = 'node' and oa.entity_id = ow.entity_id and oa.deleted = 0 ";
    $sql .= "  and oa.field_sirius_active_value = 'Yes' ";

    // Person -> their contact record.
    $sql .= "join field_data_field_sirius_contact wc ";
    $sql .= "  on wc.entity_type = 'node' and wc.bundle = 'sirius_worker' and wc.deleted = 0 ";
    $sql .= "  and wc.entity_id = ow.field_sirius_worker_target_id ";

    // Contact appears as the dependent side of a relationship record.
    $sql .= "join field_data_field_sirius_contact_alt ra ";
    $sql .= "  on ra.entity_type = 'node' and ra.bundle = 'sirius_contact_relationship' and ra.deleted = 0 ";
    $sql .= "  and ra.field_sirius_contact_alt_target_id = wc.field_sirius_contact_target_id ";

    // The dependent relationship itself must still be active. Relationship
    // records carry their own active flag, which auto-clears once the
    // relationship's end date has passed -- same convention as elections.
    $sql .= "join field_data_field_sirius_active ras ";
    $sql .= "  on ras.entity_type = 'node' and ras.entity_id = ra.entity_id and ras.deleted = 0 ";
    $sql .= "  and ras.field_sirius_active_value = 'Yes' ";

    // An election that claims that relationship as a covered dependent.
    $sql .= "join field_data_field_sirius_contact_relations cr ";
    $sql .= "  on cr.entity_type = 'node' and cr.bundle = 'sirius_trust_worker_election' and cr.deleted = 0 ";
    $sql .= "  and cr.field_sirius_contact_relations_target_id = ra.entity_id ";

    // That election must also be active.
    $sql .= "join field_data_field_sirius_active da ";
    $sql .= "  on da.entity_type = 'node' and da.entity_id = cr.entity_id and da.deleted = 0 ";
    $sql .= "  and da.field_sirius_active_value = 'Yes' ";

    // ...and belong to a different worker.
    $sql .= "join field_data_field_sirius_worker dw ";
    $sql .= "  on dw.entity_type = 'node' and dw.entity_id = cr.entity_id and dw.deleted = 0 ";

    $sql .= "where ow.entity_type = 'node' ";
    $sql .= "and ow.bundle = 'sirius_trust_worker_election' ";
    $sql .= "and ow.deleted = 0 ";
    $sql .= "and dw.field_sirius_worker_target_id <> ow.field_sirius_worker_target_id ";

    // Both elections carry a benefit of the same medical/dental type.
    $sql .= "and exists ( ";
    $sql .= "  select 1 ";
    $sql .= "  from field_data_field_sirius_trust_benefits ob ";
    $sql .= "  join field_data_field_sirius_trust_benefit_type obty ";
    $sql .= "    on obty.entity_type = 'node' and obty.deleted = 0 ";
    $sql .= "    and obty.entity_id = ob.field_sirius_trust_benefits_target_id ";
    $sql .= "  join field_data_field_sirius_id obt ";
    $sql .= "    on obt.entity_type = 'taxonomy_term' and obt.deleted = 0 ";
    $sql .= "    and obt.entity_id = obty.field_sirius_trust_benefit_type_tid ";
    $sql .= "  where ob.entity_type = 'node' and ob.entity_id = ow.entity_id and ob.deleted = 0 ";
    $sql .= "  and obt.field_sirius_id_value in ($types) ";
    $sql .= "  and exists ( ";
    $sql .= "    select 1 ";
    $sql .= "    from field_data_field_sirius_trust_benefits db ";
    $sql .= "    join field_data_field_sirius_trust_benefit_type dbty ";
    $sql .= "      on dbty.entity_type = 'node' and dbty.deleted = 0 ";
    $sql .= "      and dbty.entity_id = db.field_sirius_trust_benefits_target_id ";
    $sql .= "    join field_data_field_sirius_id dbt ";
    $sql .= "      on dbt.entity_type = 'taxonomy_term' and dbt.deleted = 0 ";
    $sql .= "      and dbt.entity_id = dbty.field_sirius_trust_benefit_type_tid ";
    $sql .= "    where db.entity_type = 'node' and db.entity_id = cr.entity_id and db.deleted = 0 ";
    $sql .= "    and dbt.field_sirius_id_value = obt.field_sirius_id_value ";
    $sql .= "  ) ";
    $sql .= ") ";

    $sql .= "order by ow.field_sirius_worker_target_id, ow.entity_id, cr.entity_id ";

    $stmt = sirius_sql_query($sql, array());

    $pks = array();
    while ($hr = $stmt->fetchAssoc()) {
      $pks[] = implode('|', array(
        $hr['person_nid'] + 0,
        $hr['own_election_nid'] + 0,
        $hr['dep_election_nid'] + 0,
        $hr['relationship_nid'] + 0,
        $hr['dep_subscriber_nid'] + 0,
      ));
    }

    return array_chunk($pks, 100);
  }

  public function batch($pks, $values, &$context) {
    $parameters = $values['parameters'];
    $granted_only = $parameters['granted_only'] ? TRUE : FALSE;

    // Every election referenced by this chunk, either side.
    $election_nids = array();
    $parsed = array();
    foreach ($pks as $pk) {
      list ($person_nid, $own_nid, $dep_nid, $rel_nid, $sub_nid) = explode('|', $pk);
      $parsed[$pk] = array(
        'person_nid' => $person_nid + 0,
        'own_nid' => $own_nid + 0,
        'dep_nid' => $dep_nid + 0,
        'rel_nid' => $rel_nid + 0,
        'sub_nid' => $sub_nid + 0,
      );
      $election_nids[$own_nid + 0] = $own_nid + 0;
      $election_nids[$dep_nid + 0] = $dep_nid + 0;
    }

    $elected = $this->elected_benefits($election_nids);
    $granted = $this->granted_types($election_nids);

    $node_lookup = array();
    $term_lookup = array();

    $rows = array();
    foreach ($pks as $pk) {
      $p = $parsed[$pk];

      $row = array();
      $row['person_nid'] = $p['person_nid'];
      $row['person'] = $this->node_title($p['person_nid'], $node_lookup);
      $row['relationship_nid'] = $p['rel_nid'];
      $row['own_election_nid'] = $p['own_nid'];
      $row['dep_election_nid'] = $p['dep_nid'];
      $row['dep_subscriber_nid'] = $p['sub_nid'];
      $row['dep_subscriber'] = $this->node_title($p['sub_nid'], $node_lookup);

      // Relationship type
      $rel_node = $this->node_get($p['rel_nid'], $node_lookup);
      if ($rel_node) {
        $tid = $rel_node->field_sirius_contact_reltype['und'][0]['tid'];
        $row['relationship'] = $this->term_name($tid, $term_lookup);
      }

      // Election types
      $own_node = $this->node_get($p['own_nid'], $node_lookup);
      if ($own_node) {
        $tid = $own_node->field_sirius_trust_election_type['und'][0]['tid'];
        $row['own_election_type'] = $this->term_name($tid, $term_lookup);
      }
      $dep_node = $this->node_get($p['dep_nid'], $node_lookup);
      if ($dep_node) {
        $tid = $dep_node->field_sirius_trust_election_type['und'][0]['tid'];
        $row['dep_election_type'] = $this->term_name($tid, $term_lookup);
      }

      // Elected benefits and their types
      $own_titles = $elected[$p['own_nid']]['titles'] ? $elected[$p['own_nid']]['titles'] : array();
      $dep_titles = $elected[$p['dep_nid']]['titles'] ? $elected[$p['dep_nid']]['titles'] : array();
      $own_types = $elected[$p['own_nid']]['types'] ? $elected[$p['own_nid']]['types'] : array();
      $dep_types = $elected[$p['dep_nid']]['types'] ? $elected[$p['dep_nid']]['types'] : array();

      sort($own_titles);
      sort($dep_titles);
      sort($own_types);
      sort($dep_types);

      $row['own_benefits'] = implode(', ', $own_titles);
      $row['dep_benefits'] = implode(', ', $dep_titles);
      $row['own_types'] = implode(', ', $own_types);
      $row['dep_types'] = implode(', ', $dep_types);

      $overlap = array_values(array_intersect($own_types, $dep_types));
      sort($overlap);
      $row['overlapping_types'] = implode(', ', $overlap);

      // Was matching coverage actually granted on both sides recently?
      $own_granted = $granted[$p['own_nid']] ? $granted[$p['own_nid']] : array();
      $dep_granted = $granted[$p['dep_nid']] ? $granted[$p['dep_nid']] : array();
      $granted_overlap = array_values(array_intersect($own_granted, $dep_granted));
      sort($granted_overlap);
      $row['granted_overlap_past_year'] = implode(', ', $granted_overlap);

      // Optional: drop paper-only overlaps. The granted overlap can include
      // types outside the medical/dental scope, so test against those
      // explicitly rather than just checking for a non-empty list.
      if ($granted_only) {
        $granted_scoped = array_intersect($granted_overlap, $this->overlap_types);
        if (!$granted_scoped) { continue; }
      }

      $rows[$pk] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }

  /**
   * election_nid => array('titles' => [...], 'types' => [...])
   * One query for the whole chunk.
   */
  public function elected_benefits($election_nids) {
    $out = array();
    if (!$election_nids) { return $out; }

    $list = $this->int_list($election_nids);

    $sql = "select b.entity_id as election_nid, ";
    $sql .= "bn.title as benefit_title, ";
    $sql .= "bt.field_sirius_id_value as type_id ";
    $sql .= "from field_data_field_sirius_trust_benefits b ";
    $sql .= "join node bn on bn.nid = b.field_sirius_trust_benefits_target_id ";
    $sql .= "left join field_data_field_sirius_trust_benefit_type bty ";
    $sql .= "  on bty.entity_type = 'node' and bty.entity_id = bn.nid and bty.deleted = 0 ";
    $sql .= "left join field_data_field_sirius_id bt ";
    $sql .= "  on bt.entity_type = 'taxonomy_term' and bt.deleted = 0 ";
    $sql .= "  and bt.entity_id = bty.field_sirius_trust_benefit_type_tid ";
    $sql .= "where b.entity_type = 'node' and b.deleted = 0 ";
    $sql .= "and b.entity_id in ($list) ";

    $stmt = sirius_sql_query($sql, array());
    while ($hr = $stmt->fetchAssoc()) {
      $nid = $hr['election_nid'] + 0;
      if (!isset($out[$nid])) { $out[$nid] = array('titles' => array(), 'types' => array()); }
      if ($hr['benefit_title'] && !in_array($hr['benefit_title'], $out[$nid]['titles'])) {
        $out[$nid]['titles'][] = $hr['benefit_title'];
      }
      if ($hr['type_id'] && !in_array($hr['type_id'], $out[$nid]['types'])) {
        $out[$nid]['types'][] = $hr['type_id'];
      }
    }

    return $out;
  }

  /**
   * election_nid => array of benefit type IDs actually GRANTED (a WMB
   * exists) that started on or before today and had not ended more than
   * the lookback window ago. One query for the whole chunk.
   */
  public function granted_types($election_nids) {
    $out = array();
    if (!$election_nids) { return $out; }

    $list = $this->int_list($election_nids);
    $months = intval($this->granted_lookback_months);

    $sql = "select te.field_sirius_trust_election_target_id as election_nid, ";
    $sql .= "bt.field_sirius_id_value as type_id ";
    $sql .= "from field_data_field_sirius_trust_election te ";
    $sql .= "join node wb on wb.nid = te.entity_id ";
    $sql .= "  and wb.type = 'sirius_trust_worker_benefit' and wb.status = 1 ";
    $sql .= "join field_data_field_sirius_trust_benefit tb ";
    $sql .= "  on tb.entity_type = 'node' and tb.entity_id = wb.nid and tb.deleted = 0 ";
    $sql .= "join field_data_field_sirius_trust_benefit_type bty ";
    $sql .= "  on bty.entity_type = 'node' and bty.deleted = 0 ";
    $sql .= "  and bty.entity_id = tb.field_sirius_trust_benefit_target_id ";
    $sql .= "join field_data_field_sirius_id bt ";
    $sql .= "  on bt.entity_type = 'taxonomy_term' and bt.deleted = 0 ";
    $sql .= "  and bt.entity_id = bty.field_sirius_trust_benefit_type_tid ";
    $sql .= "left join field_data_field_sirius_date_start ds ";
    $sql .= "  on ds.entity_type = 'node' and ds.entity_id = wb.nid and ds.deleted = 0 ";
    $sql .= "left join field_data_field_sirius_date_end de ";
    $sql .= "  on de.entity_type = 'node' and de.entity_id = wb.nid and de.deleted = 0 ";
    $sql .= "where te.entity_type = 'node' ";
    $sql .= "and te.bundle = 'sirius_trust_worker_benefit' and te.deleted = 0 ";
    $sql .= "and te.field_sirius_trust_election_target_id in ($list) ";
    $sql .= "and date(ds.field_sirius_date_start_value) <= curdate() ";
    $sql .= "and (de.field_sirius_date_end_value is null ";
    $sql .= "  or date(de.field_sirius_date_end_value) >= date_sub(curdate(), interval $months month)) ";
    $sql .= "group by te.field_sirius_trust_election_target_id, bt.field_sirius_id_value ";

    $stmt = sirius_sql_query($sql, array());
    while ($hr = $stmt->fetchAssoc()) {
      $nid = $hr['election_nid'] + 0;
      if (!isset($out[$nid])) { $out[$nid] = array(); }
      if ($hr['type_id']) { $out[$nid][] = $hr['type_id']; }
    }

    return $out;
  }

  public function int_list($nids) {
    $list = array();
    foreach ($nids as $nid) { $list[] = intval($nid); }
    return implode(',', $list);
  }

  public function node_get($nid, &$lookup) {
    if (!$nid) { return NULL; }
    if (!array_key_exists($nid, $lookup)) { $lookup[$nid] = node_load($nid); }
    return $lookup[$nid];
  }

  public function node_title($nid, &$lookup) {
    $node = $this->node_get($nid, $lookup);
    return $node ? $node->title : '';
  }

  public function term_name($tid, &$lookup) {
    if (!$tid) { return ''; }
    if (!array_key_exists($tid, $lookup)) {
      $term = taxonomy_term_load($tid);
      $lookup[$tid] = $term ? $term->name : '';
    }
    return $lookup[$tid];
  }
}
