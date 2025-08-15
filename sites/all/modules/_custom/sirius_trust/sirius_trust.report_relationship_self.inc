<?php

class Sirius_Trust_Report_Relationship_Self extends Report_Cache_Report_Generator {
  public $type = 'sirius-trust-relationship-self';

  public function info($result = array()) {
    $result = array(
      'name' => t('Relationships - Self'),
      'description' => '',
      'access' => 'sirius trust staff',
      'preview' => array(
        'default_display_fields' => array(
          'relationship_nid',
          'contact_name',
          'relationship_type'
        ),
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'sirius_trust.report_relationship_self.inc',
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
    $fields['contact_name'] = 'Contact Name';
    $fields['relationship_tid'] = 'Relationship TID';
    $fields['relationship_type'] = 'Relationship Type';

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

    $sql = "select nid from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_contact_alt on field_data_field_sirius_contact_alt.entity_type = 'node' and field_data_field_sirius_contact_alt.entity_id = node.nid ";
    $sql .= "where node.type = 'sirius_contact_relationship' ";
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = sirius_domain_nid();
    $sql .= "and field_sirius_contact_target_id = field_sirius_contact_alt_target_id ";
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
    foreach ($relationship_nids as $relationship_nid) {

      $row = array();
      $relationship_node = node_load($relationship_nid);
      $contact_nid = $relationship_node->field_sirius_contact['und'][0]['target_id'];
      $worker_nid = sirius_worker_get_by_contact_nid($contact_nid, TRUE);
      $contact_name = sirius_node_title($contact_nid);

      $relationship_tid = $relationship_node->field_sirius_contact_reltype['und'][0]['tid'];
      $relationship_type = $types[$relationship_tid];

      $row['relationship_nid'] = $relationship_nid;
      $row['contact_nid'] = $contact_nid;
      $row['worker_nid'] = $worker_nid;
      $row['contact_name'] = $contact_name;
      $row['relationship_tid'] = $relationship_tid;
      $row['relationship_type'] = $relationship_type;

      $rows[$relationship_nid] = $row;
      sirius_minilog_reset();
    }
    return array('is_multiple' => TRUE, 'rows' => $rows);
  }
}
