<?php

class Sirius_Trust_Report_Relationship_Reciprocal extends Report_Cache_Report_Generator {
  public $type = 'sirius-trust-relationship-reciprocal';

  public function info($result = array()) {
    $result = array(
      'name' => t('Relationships - Reciprocal'),
      'description' => '',
      'access' => 'sirius trust staff',
      'preview' => array(
        'default_display_fields' => array(
          'contact_1_name',
          'contact_2_name',
          'relationship_1_start',
          'relationship_2_start',
          'relationship_1_end',
          'relationship_2_end',
          'relationship_1_type',
          'relationship_2_type',
        ),
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'sirius_trust.report_relationship_reciprocal.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {
    $fields = array();
    $fields['relationship_1_nid'] = 'Relationship NID 1 NID';
    $fields['relationship_2_nid'] = 'Relationship NID 2 NID';
    $fields['contact_1_nid'] = 'Contact 1 NID';
    $fields['contact_2_nid'] = 'Contact 2 NID';
    $fields['contact_1_name'] = 'Contact 1 Name';
    $fields['contact_2_name'] = 'Contact 2 Name';
    $fields['worker_1_nid'] = 'Worker 1 NID';
    $fields['worker_2_nid'] = 'Worker 2 NID';
    $fields['relationship_1_start'] = 'Relationship 1 Start Date';
    $fields['relationship_2_start'] = 'Relationship 2 Start Date';
    $fields['relationship_1_end'] = 'Relationship 1 End Date';
    $fields['relationship_2_end'] = 'Relationship 2 End Date';
    $fields['relationship_1_tid'] = 'Relationship 1 TID';
    $fields['relationship_2_tid'] = 'Relationship 2 TID';
    $fields['relationship_1_type'] = 'Relationship 1 Type';
    $fields['relationship_2_type'] = 'Relationship 2 Type';

    return $fields;
  }

  public function preview_links($row) {
    $links = array();
    $links['node/' . $row['relationship_1_nid']] = 'Relationship 1';
    $links['node/' . $row['relationship_2_nid']] = 'Relationship 2';

    if ($row['worker_1_nid']) {
      $links['node/' . $row['relationship_1_nid'] . "/sirius_worker_relationships"] = 'Worker 1';
    }
    if ($row['worker_2_nid']) {
      $links['node/' . $row['relationship_2_nid'] . "/sirius_worker_relationships"] = 'Worker 2';
    }

    return $links;
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

  public function render_parameters($parameters) {
    $parameters = $parameters['parameters'];

    $html = sirius_jsonfield_util_render_all_values_new(
      $this->form(), 
      $parameters['parameters'], 
      array(
        'skip_empty' => TRUE,
        'fieldset_title_prefix' => '<h2>',
        'fieldset_title_suffix' => '</h2>',
        'no_fieldsets' => TRUE,
        'separator' => '<br />',
      )
    );
    return $html;
  }

  public function pks(&$form_state) {
    $parameters = $form_state['values']['parameters'];

    $sql = "select node1.nid as nid1, node2.nid as nid2 ";
    $sql .= "from node node1 ";
    $sql .= "left join node node2 on node1.nid != node2.nid ";

    $sql .= "left join field_data_field_sirius_contact contact1a on contact1a.entity_type = 'node' and contact1a.entity_id = node1.nid ";
    $sql .= "left join field_data_field_sirius_contact contact2a on contact2a.entity_type = 'node' and contact2a.entity_id = node2.nid ";

    $sql .= "left join field_data_field_sirius_contact_alt contact1b on contact1b.entity_type = 'node' and contact1b.entity_id = node1.nid ";
    $sql .= "left join field_data_field_sirius_contact_alt contact2b on contact2b.entity_type = 'node' and contact2b.entity_id = node2.nid ";

    $sql .= "left join field_data_field_sirius_domain domain1 on domain1.entity_type = 'node' and domain1.entity_id = node1.nid ";
    $sql .= "left join field_data_field_sirius_domain domain2 on domain2.entity_type = 'node' and domain2.entity_id = node2.nid ";

    $sql .= "left join field_data_field_sirius_active active1 on active1.entity_type = 'node' and active1.entity_id = node1.nid ";
    $sql .= "left join field_data_field_sirius_active active2 on active2.entity_type = 'node' and active2.entity_id = node2.nid ";

    $sql .= "left join field_data_field_sirius_date_start start1 on start1.entity_type = 'node' and start1.entity_id = node1.nid ";
    $sql .= "left join field_data_field_sirius_date_start start2 on start2.entity_type = 'node' and start2.entity_id = node2.nid ";

    $sql .= "left join field_data_field_sirius_date_end end1 on end1.entity_type = 'node' and end1.entity_id = node1.nid ";
    $sql .= "left join field_data_field_sirius_date_end end2 on end2.entity_type = 'node' and end2.entity_id = node2.nid ";

    // Two relationships
    $sql .= "where node1.type = 'sirius_contact_relationship' ";
    $sql .= "and   node1.type = 'sirius_contact_relationship' ";

    // Both are reciprocal
    $sql .= "and contact1a.field_sirius_contact_target_id = contact2b.field_sirius_contact_alt_target_id ";
    $sql .= "and contact2a.field_sirius_contact_target_id = contact1b.field_sirius_contact_alt_target_id ";

    // Get each pair just once
    $sql .= "and contact1a.field_sirius_contact_target_id < contact1b.field_sirius_contact_alt_target_id ";

    // In the right domain
    $sql .= "and domain1.field_sirius_domain_target_id = :domain_nid ";
    $sql .= "and domain2.field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = sirius_domain_nid();

    // At least one is active
    $sql .= "and (";
    $sql .= "     active1.field_sirius_active_value = 'Yes' ";
    $sql .= "  OR active2.field_sirius_active_value = 'Yes' ";
    $sql .= ") ";

    // Dates don't match
    $sql .= "and (";
    $sql .= "     start1.field_sirius_date_start_value != start2.field_sirius_date_start_value ";
    $sql .= "  OR end1.field_sirius_date_end_value != end1.field_sirius_date_end_value ";
    $sql .= ") ";

    // $sql .= "limit 10";

    $stmt = sirius_sql_query($sql, $sql_args);
    $pks = array();
    while ($hr = $stmt->fetchAssoc()) {
      $pk = $hr['nid1'] . '|' . $hr['nid2'];
      $pks[] = $pk;
    }
    return array_chunk($pks, 100);
  }

  public function batch($pks, $values, &$context) {
    $types = sirius_taxonomy_load('sirius_contact_relationship_types', 'tid', 'name');
    $rows = array();
    foreach ($pks as $pk) {
      list ($relationship_1_nid, $relationship_2_nid) = explode('|', $pk);

      $row = array();

      $relationship_1_node = node_load($relationship_1_nid);
      $relationship_2_node = node_load($relationship_2_nid);

      $contact_1_nid = $relationship_1_node->field_sirius_contact['und'][0]['target_id'];
      $contact_2_nid = $relationship_2_node->field_sirius_contact['und'][0]['target_id'];

      $worker_1_nid = sirius_worker_get_by_contact_nid($contact_1_nid, TRUE);
      $worker_2_nid = sirius_worker_get_by_contact_nid($contact_2_nid, TRUE);

      $contact_1_name = sirius_node_title($contact_1_nid);
      $contact_2_name = sirius_node_title($contact_2_nid);

      $relationship_1_start = $relationship_1_node->field_sirius_date_start['und'][0]['value'];
      if ($relationship_1_start) { $relationship_1_start = date('m/d/Y', strtotime($relationship_1_start)); }

      $relationship_2_start = $relationship_2_node->field_sirius_date_start['und'][0]['value'];
      if ($relationship_2_start) { $relationship_2_start = date('m/d/Y', strtotime($relationship_2_start)); }

      $relationship_1_end = $relationship_1_node->field_sirius_date_end['und'][0]['value'];
      if ($relationship_1_end) { $relationship_1_end = date('m/d/Y', strtotime($relationship_1_end)); }

      $relationship_2_end = $relationship_1_node->field_sirius_date_end['und'][0]['value'];
      if ($relationship_2_end) { $relationship_2_end = date('m/d/Y', strtotime($relationship_2_end)); }

      $relationship_1_tid = $relationship_1_node->field_sirius_contact_reltype['und'][0]['tid'];
      $relationship_2_tid = $relationship_2_node->field_sirius_contact_reltype['und'][0]['tid'];
      $relationship_1_type = $types[$relationship_1_tid];
      $relationship_2_type = $types[$relationship_2_tid];

      $row['relationship_1_nid'] = $relationship_1_nid;
      $row['relationship_2_nid'] = $relationship_2_nid;
      $row['contact_1_nid'] = $contact_1_nid;
      $row['contact_2_nid'] = $contact_2_nid;
      $row['contact_1_name'] = $contact_1_name;
      $row['contact_2_name'] = $contact_2_name;
      $row['worker_1_nid'] = $worker_1_nid;
      $row['worker_2_nid'] = $worker_2_nid;
      $row['relationship_1_start'] = $relationship_1_start;
      $row['relationship_2_start'] = $relationship_2_start;
      $row['relationship_1_end'] = $relationship_1_end;
      $row['relationship_2_end'] = $relationship_2_end;
      $row['relationship_1_tid'] = $relationship_1_tid;
      $row['relationship_2_tid'] = $relationship_2_tid;
      $row['relationship_1_type'] = $relationship_1_type;
      $row['relationship_2_type'] = $relationship_2_type;

      $rows[$pk] = $row;
      sirius_minilog_reset();
    }
    return array('is_multiple' => TRUE, 'rows' => $rows);
  }
}
