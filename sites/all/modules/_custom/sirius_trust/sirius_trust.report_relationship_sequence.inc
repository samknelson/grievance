<?php

function sirius_trust_report_relationship_sequence_info() {
  $result = array(
    'name' => t('Relationships - Sequence'),
    'description' => t('Data integrity report showing relationship sequence errors such as beneficiaries with the same sequence.'),
    'access' => 'sirius trust staff',
    'fields' => sirius_trust_report_relationship_sequence_fields(),
    'parameters_form_callback' => 'report_cache_parameters_form_callback_empty',
    'pk_list_callback' => 'sirius_trust_report_relationship_sequence_pks',
    'batch_callback' => 'sirius_trust_report_relationship_sequence_batch',
    'render_callbacks' => array(
      'View' => 'sirius_trust_report_relationship_sequence_view',
    ),
    'module' => 'sirius_trust',
    'file' => 'sirius_trust.report_relationship_sequence.inc',
    'dashboard' => TRUE,
  );

  return $result;
}

function sirius_trust_report_relationship_sequence_fields() {
  return array(
    'contact_nid' => 'Contact NID',
    'worker_nid' => 'Worker NID',
    'worker_nid' => 'Worker Name',
    'err' => 'Error Message',
  );
}

function sirius_trust_report_relationship_sequence_pks(&$form_state) {
  $domain_nid = sirius_domain_nid();
  $sql_args = array();

  $sql = "select distinct field_sirius_contact_target_id ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_contact_relationship' ";
  if ($domain_nid) { 
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  } else {
    $sql .= "and field_sirius_domain_target_id is NULL ";
  }
  $sql .= "order by node.nid ";

  $stmt = sirius_sql_query($sql, $sql_args);
  $contact_nids = array();
  while ($hr = $stmt->fetchAssoc()) {
    $contact_nids[] = $hr['field_sirius_contact_target_id'];
  }
  return array_chunk($contact_nids, 100, TRUE);
}

function sirius_trust_report_relationship_sequence_batch($contact_nids, $values, $report_id, $report_action, &$context) {
  $rows = array('is_multiple' => TRUE, 'rows' => array());

  foreach ($contact_nids as $contact_nid) {
    // Load all our existing relationships
    $relationships = sirius_contact_relationships(array('contact_nid' => $contact_nid));
    $by_seq = $by_target = array();

    $err = '';
    foreach ($relationships as $relationship_nid => $relationship_node) {
      // Who is the target of this relationship?
      $target_nid = $relationship_node->field_sirius_contact_alt['und'][0]['target_id'];
      if (!$target_nid) {
        $err = 'One of the relationships is missing a second contact.';
        break;
      }

      // What is the sequence number?
      $seq = $relationship_node->field_sirius_count['und'][0]['value'];
      if (!$seq) {
        $err = 'One of the relationships is missing a sequene number.'; 
        break;
      }

      // Duplicate?
      if ($by_seq[$seq] && ($by_seq[$seq] != $target_nid)) {
        $err = 'Two of the relationships have the same sequence number.'; 
        break;
      }
      $by_seq[$seq] = $target_nid;

      // Same target, different number?
      if ($by_target[$target_nid] && ($by_target[$target_nid] != $seq)) {
        $err = 'Two relationships have the same target, but different sequence numbers.'; 
        break;
      }
      $by_target[$target_nid] = $seq;

    }

    if (!$err) { continue; }

    $worker_nid = sirius_worker_get_by_contact_nid($contact_nid, TRUE);
    $worker_name = sirius_node_title($worker_nid);
    $row = array();
    $row['worker_nid'] = $worker_nid;
    $row['contact_nid'] = $contact_nid;
    $row['worker_name'] = $worker_name;
    $row['err'] = $err;

    $rows[$contact_nid] = $row;

    $rows['rows'][] = $row;

    $context['results']['dashboard']['count']++;
  }

  return $rows;
}

function sirius_trust_report_relationship_sequence_view($report_type, $report_id) {
  $info = report_cache_info($report_type);
  $parameters = report_cache_parameters($report_type, $report_id);

  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $data = array();
  while ($hr = $stmt->fetchAssoc()) {
    $data[] = unserialize($hr['report_data']);
  }

  foreach ($data as $hr) {
    $row = array();
    $row[] = $hr['worker_name'];
    $row[] = $hr['err'];
    $tools = array();
    $tools[] = l('Worker', 'node/' . $hr['worker_nid'] . '/sirius_worker_relationships', array('attributes' => array('class' => array('tool-button'))));
    $row[] = join('', $tools);
    $rows[] = $row;
  }

  $header[] = 'Worker';
  $header[] = 'Error';
  $header[] = 'Tools';

  if ($_REQUEST['output'] == 'csv') {
    $filename = "${report_type}_";
    $r = $report_id;
    $r = preg_replace('/[^a-zA-Z0-9_ ]/', '', $r);
    $r = preg_replace('/ +/', '_', $r);
    $filename .= $r;
    $filename .= ".csv";

    drupal_add_http_header('Content-Type', 'text/csv');
    drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
    $fp = fopen('php://output', 'w');
    fputcsv($fp, $header);
    foreach ($rows as $row) {
      foreach ($row as $delta => $val) { $row[$delta] = strip_tags($val); }
      fputcsv($fp, $row);
    }
    drupal_exit();
  }

  $attributes = array();
  $html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));
  return $html;
}
