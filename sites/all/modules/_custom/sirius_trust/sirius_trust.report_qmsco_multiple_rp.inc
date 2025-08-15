<?php

class Sirius_Trust_Report_Qmsco_Multiple_Rp extends Report_Cache_Report_Generator {
  public $type = 'sirius-trust-qmsco-multiple-rp';

  public function info($result = array()) {
    $result = array(
      'name' => t('QMSO - Multiple RP'),
      'description' => '',
      'access' => 'sirius trust staff',
      'preview' => array(
        'default_display_fields' => array(
          'relationship_nid',
          'worker_name',
        ),
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'sirius_trust.report_qmsco_multiple_rp.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function form() {
    /*
    $form['end_month'] = array(
      '#title' => t('End Month'),
      '#type' => 'textfield',
      '#size' => 2,
    );
    */

    return $form;
  }

  public function fields() {
    $fields = array();
    $fields['relationship_nid'] = 'Relationship NID';
    $fields['contact_nid'] = 'Contact NID';
    $fields['worker_nid'] = 'Worker NID';
    $fields['worker_name'] = 'Contact Name';

    return $fields;
  }

  public function preview_links($row) {
    $links = array();
    $links['node/' . $row['relationship_nid']] = 'Relationship';

    if ($row['worker_nid']) {
      $links['node/' . $row['worker_nid'] . "/sirius_worker_relationships"] = 'Worker';
    }

    return $links;
  }

  public function pks(&$form_state) {
    $parameters = $form_state['values']['parameters'];

    $qmsco = Sirius_Trust_Qmsco::getInstance();

    $qmsco_tid = $qmsco->tid();
    if (!$qmsco_tid) { return; }

    $sql = "select distinct nid from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_active on field_data_field_sirius_active.entity_type = 'node' and field_data_field_sirius_active.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_contact_reltype on field_data_field_sirius_contact_reltype.entity_type = 'node' and field_data_field_sirius_contact_reltype.entity_id = node.nid ";
    $sql .= "where node.type = 'sirius_contact_relationship' ";
    $sql .= "and field_sirius_active_value = 'Yes' ";
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = sirius_domain_nid();
    $sql .= "and field_sirius_contact_reltype_tid = :qmsco_tid ";
    $sql_args[':qmsco_tid'] = $qmsco_tid;
    $sql .= "order by nid ";

    $stmt = sirius_sql_query($sql, $sql_args);
    $pks = array();
    while ($hr = $stmt->fetchAssoc()) {
      $pks[] = $hr['nid'];
    }
    return array_chunk($pks, 100);
  }

  public function batch($relationship_nids, $values, &$context) {
    $types = sirius_taxonomy_load('sirius_contact_relationship_types', 'tid', 'name');
    $rows = array();

    $qmsco = Sirius_Trust_Qmsco::getInstance();

    foreach ($relationship_nids as $relationship_nid) {

      $row = array();
      $relationship_node = node_load($relationship_nid);
      $contact_nid = $relationship_node->field_sirius_contact_alt['und'][0]['target_id'];
      $worker_nid = sirius_worker_get_by_contact_nid($contact_nid, TRUE);
      $worker_node = node_load($worker_nid);
      if (!$worker_node) { continue; }

      $rp_relationship_nodes = $qmsco->rp_relationship_nodes($worker_node);
      if (count($rp_relationship_nodes) < 2) {
        continue;
      }

      $relationship_tid = $relationship_node->field_sirius_contact_reltype['und'][0]['tid'];
      $relationship_type = $types[$relationship_tid];

      $row['relationship_nid'] = $relationship_nid;
      $row['contact_nid'] = $contact_nid;
      $row['worker_nid'] = $worker_nid;
      $row['worker_name'] = $worker_node->title;

      $rows[$relationship_nid] = $row;
    }
    return array('is_multiple' => TRUE, 'rows' => $rows);
  }
}
