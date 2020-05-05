<?php

function sirius_event_report_wwe_form($form, &$form_state, $report_type) {
  report_cache_form_prepare($report_type, $form, $form_state);

	$event_type_options = sirius_taxonomy_load('sirius_event_type', 'tid', 'name', TRUE);
  $ms_options = sirius_taxonomy_load('sirius_member_status', 'tid', 'name', TRUE);

  $all_levels = sirius_training_levels_fetch_all();
  $level_options = array();
  $level_options[NULL] = t('-- Select --');
  foreach ($all_levels as $ms_tid => $records) {
    foreach ($records as $level_code => $record) {
      $level_options["$ms_tid|$level_code"] = $ms_options[$ms_tid] . ': ' . $record['title'];
    }
  }

  $form['ms_tid'] = array(
    '#title' => t('Search for active workers with this work status ...'),
    '#type' => 'select',
    '#options' => $ms_options,
    '#required' => TRUE,
  );

  $form['level'] = array(
    '#title' => t('... and this level ... '),
    '#type' => 'select',
    '#options' => $level_options,
  );

  $form['event_type_tid'] = array(
  	'#title' => t('... Who have NOT attented at least one event with this type'),
  	'#type' => 'select',
  	'#options' => $event_type_options,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function sirius_event_report_wwe_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }

function sirius_event_report_wwe_fields() {
	$fields = array();
	$fields['worker_nid'] = 'Worker NID';
  $fields['worker_id'] = 'Worker ID';
  $fields['worker_name'] = 'Worker Name';
  $fields['worker_level'] = 'Worker Level';
	return $fields;
}

function sirius_event_report_wwe_pks(&$form_state) {
  $ws_terms = sirius_taxonomy_load('sirius_work_status', 'tid', 'full');
  $active_ws_tids = array();
  foreach ($ws_terms as $ws_tid => $ws_term) {
    if ($ws_term->field_sirius_member_active['und'][0]['value'] == 'Yes') { $active_ws_tids[] = $ws_tid; }
  }
  if (!$active_ws_tids) { return array(); }

  $sql_args = array();
  $sql_args[':ms_tid'] = $form_state['values']['ms_tid'];
  $sql_args[':active_ws_tids'] = $active_ws_tids;
  $sql = "select distinct node.nid ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_sirius_member_status on field_data_field_sirius_member_status.entity_type = 'node' and field_data_field_sirius_member_status.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_work_status on field_data_field_sirius_work_status.entity_type = 'node' and field_data_field_sirius_work_status.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_worker' ";
  $sql .= "and field_sirius_member_status_tid = :ms_tid ";
  $sql .= "and field_sirius_work_status_tid in (:active_ws_tids) ";
  $domain_nid = sirius_domain_nid();
  if ($domain_nid) {
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  }
  $sql .= "order by node.nid ";

  $stmt = sirius_sql_query($sql, $sql_args);
  $pks = array();
  while ($hr = $stmt->fetchAssoc()) { $pks[] = $hr['nid']; }
  return array_chunk($pks, 100, TRUE);
}

function sirius_event_report_wwe_batch($worker_nids, $values, $report_id, $report_action, &$context) {
  // Get the contacts from the workers. $sql for speed
  $sql_args = array();
  $sql_args[':worker_nids'] = $worker_nids;
  $sql = "select nid, field_sirius_contact_target_id ";
  $sql .= "from node ";
  $sql .= "join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
  $sql .= "where node.nid in (:worker_nids) ";
  $stmt = sirius_sql_query($sql, $sql_args);
  $contact_to_worker_map = array();
  while ($hr = $stmt->fetchAssoc()) {
    $worker_to_contact_map[$hr['nid']] = $hr['field_sirius_contact_target_id'];
  }

  // Search for participant records
  $sql_args = array();
  $sql_args[':contact_nids'] = array_values($worker_to_contact_map);
  $sql_args[':event_type_tid'] = $values['event_type_tid'];
  $sql = "select nid, field_sirius_contact_target_id, field_sirius_event_type_tid ";
  $sql .= "from node ";
  $sql .= "join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
  $sql .= "join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = node.nid ";
  $sql .= "join field_data_field_sirius_event_type on field_data_field_sirius_event_type.entity_type = 'node' and field_data_field_sirius_event_type.entity_id = field_sirius_event_target_id ";
  $sql .= "where node.type = 'sirius_event_participant' ";
  $sql .= "and field_sirius_contact_target_id in (:contact_nids) ";
  $sql .= "and field_sirius_event_type_tid = :event_type_tid ";
  $stmt = sirius_sql_query($sql, $sql_args);
  $contacts_with_events = array();
  while ($hr = $stmt->fetchAssoc()) {
    $contacts_with_events[$hr['field_sirius_contact_target_id']] = $hr['nid'];
  }

  // Walk the workers
  $ms_tid = $values['ms_tid'];
  $required_level = $values['level'];
  list ($ignore, $required_level_code) = explode('|', $required_level);
  $rows = array('is_multiple' => TRUE, 'rows' => array());
  foreach ($worker_nids as $worker_nid) {
    $contact_nid = $worker_to_contact_map[$worker_nid];
    if (!$contact_nid) { continue; }
    if ($contacts_with_events[$contact_nid]) { continue; }

    $sql_args = array();
    $sql_args[':worker_nid'] = $worker_nid;

    $worker_node = node_load($worker_nid); 
    $sql = "select title, field_sirius_id_value, field_sirius_json_value ";
    $sql .= "from node ";
    $sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
    $sql .= "where node.type = 'sirius_worker' ";
    $sql .= "and node.nid = :worker_nid";
    $stmt = sirius_sql_query($sql, $sql_args);
    $hr = $stmt->fetchAssoc();
    $json = json_decode($hr['field_sirius_json_value'], TRUE);
    $level = $level_code = '';
    $level_record = $json['training']['levels'][$ms_tid];
    if ($level_record) {
      $level_code = $level_record['code'];
      $level = sirius_training_levels_fetch_title($ms_tid, $level_code);
    }

    if ($required_level_code && ($required_level_code != $level_code)) { continue; }

    $row['worker_nid'] = $worker_nid;
    $row['worker_id'] = $hr['field_sirius_id_value'];
    $row['worker_name'] = $hr['title'];
    $row['worker_level'] = $level;
    $rows['rows'][$worker_nid] = $row;
  }

  return $rows; 
}

function sirius_event_report_wwe_view($report_type, $report_id) {
  // Load the parameters
  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type . ' :: PARAMETERS');
  $stmt = $query->execute();
  $hr = $stmt->fetchAssoc();
  $parameters = unserialize($hr['report_data']);
  $ms_term = taxonomy_term_load($parameters['ms_tid']);
  $event_type_term = taxonomy_term_load($parameters['event_type_tid']);
  $required_level = $parameters['level'];
  list ($ignore, $required_level_code) = explode('|', $required_level);
  $level_title = sirius_training_levels_fetch_title($ignore, $required_level_code);

  $html = '';
  $html .= '<h2>';
  $ms = $ms_term->name;
  if ($level_title) { $ms .= ' (' . $level_title . ')';}
  $html .= t('Active Workers with member status @ms who have NOT attended an event of type @event_type', array('@ms' => $ms, '@event_type' => $event_type_term->name));
  $html .= '</h2>';

  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $data = array();
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);
    $data[] = $hr;
  }

  $fields = sirius_event_report_wwe_fields();
  $fields['tools'] = t('Tools');

  $rows = array();
  $count = 0;
  foreach ($data as $identifier => $subdata) {
    $subdata['tools'] = l('View', 'node/' . $subdata['worker_nid'] . '', array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
    $rows[] = $subdata;
    ++$count;
    if ($count > 1000) { 
      $html .= '<p>(Limiting output to the first 1000 rows.)</p>';
      break;
    }
  }

  $header = array_values($fields);

  $attributes = array();
  $html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));
  return $html;
}