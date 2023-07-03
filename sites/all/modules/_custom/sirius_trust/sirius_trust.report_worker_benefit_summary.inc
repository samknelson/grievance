<?php

function sirius_trust_report_worker_benefit_summary_info() {
  $result = array(
    'name' => t('Coverage - Summary'),
    'description' => '',
    'access' => 'sirius trust staff',
    'fields' => sirius_trust_report_worker_benefit_summary_fields(),
    'parameters_form_callback' => 'sirius_trust_report_worker_benefit_summary_form',
    'pk_list_callback' => 'sirius_trust_report_worker_benefit_summary_pks',
    'batch_callback' => 'sirius_trust_report_worker_benefit_summary_batch',
    'render_callbacks' => array(
      'View' => 'sirius_trust_report_worker_benefit_summary_view',
    ),
    'dashboard_callback' => 'sirius_trust_report_worker_benefit_summary_dashboard',
    'module' => 'sirius_trust',
    'file' => 'sirius_trust.report_worker_benefit_summary.inc',
    'dashboard' => TRUE,
  );

  return $result;
}

function sirius_trust_report_worker_benefit_summary_fields() {
  return array(
    'key' => 'Key',
    'car' => 'Current / Add / Remove',
    'car_name' => 'Current / Add / Remove Name',
    'reltype' => 'Relationship Type TID',
    'reltype_name' => 'Relationship Type Name',
    'employer' => 'Employer',
    'employer_name' => 'Employer Name',
    'benefit' => 'Benefit',
    'benefit_name' => 'Benefit Name',
    'record_count' => 'Records',
    'subscriber_count' => 'Subscriber Count',
    'dependent_count' => 'Dependent Count',
  );
}

function sirius_trust_report_worker_benefit_summary_breakout_options() {
  return array(
    'car' => t('Current / Add / Remove'),
    'employer' => t('Employer'),
    'benefit' => t('Benefit'),
    'reltype' => t('Relationship Type'),
  );
}

function sirius_trust_report_worker_benefit_summary_form($report_type) {
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
    '#tree' => TRUE,
  );

  $form['general']['asof'] = array(
    '#title' => t('As Of'),
    '#type' => 'date_popup',
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
    '#date_format' => 'Y-m-d',
  );

  for ($i = 0; $i<3; ++$i) {
    $form['general']['breakout'][$i] = array(
      '#title' => t('Breakout #@k', array('@k' => $i+1)),
      '#type' => 'select',
      '#options' => array(NULL => t('-- Select --')) + sirius_trust_report_worker_benefit_summary_breakout_options(),
    );
  }

  //////////////////////////////////////////////////
  // Current / Add / Remove
  //////////////////////////////////////////////////

  $form['breakout_car'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter: Current / Add / Remove'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['breakout_car']['days'] = array(
    '#type' => 'textfield',
    '#size' => 4,
    '#title' => t('Days Back'),
    '#default_value' => 7,
  );

  $form['breakout_car']['filter'] = array(
    '#title' => t('Filter'),
    '#type' => 'checkboxes',
    '#options' => array(
      'current' => t('Records active as of the "as of" date'),
      'add' => t('Records added within "days back" days before the "as of" date'),
      'remove' => t('Records removed within "days back" days before the "as of" date'),
    ),
    '#checkall' => TRUE,
  );

  //////////////////////////////////////////////////
  // Relationship Type
  //////////////////////////////////////////////////

  $form['breakout_reltype'] = array(
    '#title' => t('Filter - Relationship Type'),
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $options = array('_self' => t('Self')) + sirius_taxonomy_load('sirius_contact_relationship_types', 'tid', 'name');
  $form['breakout_reltype']['filter'] = array(
    '#title' => t('Filter'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#checkall' => TRUE,
  );

  //////////////////////////////////////////////////
  // Employer
  //////////////////////////////////////////////////

  $form['breakout_employer'] = array(
    '#title' => t('Filter - Employer'),
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['breakout_employer']['filter'] = array(
    '#title' => t('Filter'),
    '#type' => 'checkboxes',
    '#options' => sirius_employer_list(),
    '#checkall' => TRUE,
  );

  //////////////////////////////////////////////////
  // Benefit
  //////////////////////////////////////////////////

  $form['breakout_benefit'] = array(
    '#title' => t('Filter - Benefit'),
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['breakout_benefit']['filter'] = array(
    '#title' => t('Filter'),
    '#type' => 'checkboxes',
    '#options' => sirius_trust_benefit_options(),
    '#checkall' => TRUE,
  );

  return $form;
}

function sirius_trust_report_worker_benefit_summary_pks(&$form_state) {
  $breakouts = array();
  for ($i = 0; $i<3; ++$i) {
    $breakout = $form_state['values']['parameters']['general']['breakout'][$i];
    if (!$breakout) { continue; }
    $breakouts[] = $breakout;
  }

  return _sirius_trust_report_worker_benefit_summary_pks_recursive($form_state['values']['parameters'], $breakouts, '', 0);
}

function _sirius_trust_report_worker_benefit_summary_pks_recursive($parameters, $breakouts, $prefix, $depth) {
  ++$depth;
  if ($depth > 100) { drupal_set_message("Infinite recursion, call the developer.", 'error'); return array(); }

  $pks = array();
  if ($breakouts) {
    $breakout = array_shift($breakouts);
    $values = array_filter(array_values($parameters["breakout_$breakout"]['filter']));
    if (!$values) { $values = array_filter(array_keys($parameters["breakout_$breakout"]['filter'])); }

    foreach ($values as $value) {
      $sub_prefix = "$prefix|$value";
      $pks = array_merge(
        $pks, 
        _sirius_trust_report_worker_benefit_summary_pks_recursive($parameters, $breakouts, $sub_prefix, $depth)
      );
    }
  }
  $pks[] = preg_replace('/^\|/', '', $prefix);
  return $pks;
}

function sirius_trust_report_worker_benefit_summary_breakouts($values) {
  return array_filter($values['parameters']['general']['breakout']);
}

function sirius_trust_report_worker_benefit_summary_breakout_keys($key, $values) {
  // Figure out our breakout options. This is some ugly code, designed to do this:
  // -- If our key is e.g. 123|456
  // -- and our breakouts are [employer] / [benefit]
  // -- Then our keys are $keys['employer'] => 123, $keys['benefit'] => 456
  $key_parts = explode('|', $key);
  $keys = array();
  $breakouts = sirius_trust_report_worker_benefit_summary_breakouts($values);
  foreach ($breakouts as $i => $breakout) {
    if ($breakout) { $keys[$breakout] = $key_parts[$i]; }
  }
  return $keys;
}

function sirius_trust_report_worker_benefit_summary_batch($key, $values, $report_id, $report_action, &$context) {
  $keys = sirius_trust_report_worker_benefit_summary_breakout_keys($key, $values);

  $row = array();
  $row['key'] = $key;

  $breakout_options = array_keys(sirius_trust_report_worker_benefit_summary_breakout_options());
  foreach ($breakout_options as $breakout_option) { $row[$breakout_option] = $keys[$breakout_option]; }
  if ($keys['employer']) { $row['employer_name'] = sirius_node_title($keys['employer']); }
  if ($keys['benefit']) { $row['benefit_name'] = sirius_node_title($keys['benefit']); }
  if ($keys['reltype']) {
    if ($keys['reltype'] == '_self') {
      $row['reltype_name'] = t('Self');
    } else {
      $term = taxonomy_term_load($keys['reltype']); 
      $row['reltype_name'] = $term->name;
    }
  }
  if ($keys['car']) { $row['car_name'] = $keys['car']; }

  $days = $values['parameters']['breakout_car']['days'];

  $asof = $values['parameters']['general']['asof'];
  if (!$asof) { $asof = date('Y-m-d'); }

  $stmt = sirius_trust_report_worker_benefit_query($asof, $days, $keys, TRUE);
  $hr = $stmt->fetchAssoc();

  if (!$hr['record_count']) { return NULL; }
  $row['record_count'] = $hr['record_count'];
  $row['subscriber_count'] = $hr['subscriber_count'];
  $row['dependent_count'] = $hr['dependent_count'];

  return $row;
}

function sirius_trust_report_worker_benefit_summary_load($report_type, $report_id) {
  $info = report_cache_info($report_type);
  $parameters = report_cache_parameters($report_type, $report_id);
  $breakouts = sirius_trust_report_worker_benefit_summary_breakouts($parameters);

  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $data = array();
  while ($hr = $stmt->fetchAssoc()) {
    $data[] = unserialize($hr['report_data']);
  }

  usort($data, function($a, $b) use ($breakouts) {
    foreach ($breakouts as $breakout) {
      $a_name = $a[$breakout . "_name"];
      $b_name = $b[$breakout . "_name"];
      if ($a_name == $b_name) { continue; }
      if (!$a_name) { return 1; }
      if (!$b_name) { return -1; }
      return strcmp($a_name, $b_name);
    }
  });

  return $data;
}

function sirius_trust_report_worker_benefit_summary_view($report_type, $report_id) {
  $info = report_cache_info($report_type);
  $parameters = report_cache_parameters($report_type, $report_id);
  $breakouts = sirius_trust_report_worker_benefit_summary_breakouts($parameters);
  $data = sirius_trust_report_worker_benefit_summary_load($report_type, $report_id);

  foreach ($data as $hr) {
    $row = array();
    foreach ($breakouts as $breakout) {
      $name = $hr[$breakout . "_name"];
      if (!$name) {
        if ($breakout == 'car') { $name = '[CURRENT]'; } else { $name = 'TOTAL'; }
      }
      $row[] = $name;
    }
    $row[] = $hr['record_count'];
    $row[] = $hr['subscriber_count'];
    $row[] = $hr['dependent_count'];
    $rows[] = $row;
  }

  $breakout_options = sirius_trust_report_worker_benefit_summary_breakout_options();
  foreach ($breakouts as $breakout) {
    $header[] = $breakout_options[$breakout];
  }
  $header[] = 'Record Count';
  $header[] = 'Subscriber Count';
  $header[] = 'Dependent Count';

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

  if ($parameters['general']['asof']) {
    $html .= '<h2>' . t('Data as of: ') . $parameters['general']['asof'] . '</h2>';
  }
  $attributes = array();

  $html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));
  return $html;
}


function sirius_trust_report_worker_benefit_summary_dashboard($prefs, $options, $report_type, $report_id) {
  $info = report_cache_info($report_type);
  $parameters = report_cache_parameters($report_type, $report_id);
  $breakouts = sirius_trust_report_worker_benefit_summary_breakouts($parameters);
  $data = sirius_trust_report_worker_benefit_summary_load($report_type, $report_id);

  $rows = array();
  foreach ($data as $hr) {
    $row = array();

    $names = array();
    foreach ($breakouts as $breakout) {
      $name = $hr[$breakout . "_name"];
      if (!$name) { 
        // if ($breakout == 'car') { $name = '[CURRENT]'; } else { $name = 'TOTAL'; }
      }
      if ($name) { $names[] = $name; }
    }
    if (!$names) { continue; }

    $details = join(' / ', $names);
    $details .= "<br />";
    $details .= "<strong>$hr[subscriber_count]</strong> subscribers / ";
    $details .= "<strong>$hr[dependent_count]</strong> dependents / ";
    $details .= "<strong>$hr[record_count]</strong> records";

    $row = array();
    $row['tools'] = l('View', "sirius/reports/$report_type", array('query' => array('report_id' => $report_id, 'action' => 'render', 'callback-name' => 'View'), 'attributes' => array('class' => array('tool-button'))));
    $count = $parameters['meta']['dashboard']['count'];
    $row['count'] = $hr['record_count'];
    $row['details'] = $details;

    $rows[] = $row;
  }

  return $rows;
}



function sirius_trust_report_worker_benefit_query($asof, $days, $keys, $is_summary) {
  if (!$asof) { $asof = date('Y-m-d'); }
  $car = $keys['car'];
  $asof .= ' 00:00:00';

  if (!$days) { $days = 0; }
  $cutoff = date('Y-m-d 00:00:00', strtotime("-$days days", strtotime($asof)));

  //////////////////////////////////////////////////
  // The Query!!!
  //////////////////////////////////////////////////

  if ($is_summary) {
    $sql = "select count(distinct node.nid) as record_count,  ";
    $sql .= "count(distinct dependent_node.nid) as dependent_count, ";
    $sql .= "count(distinct if(isnull(subscriber_node_other.nid), subscriber_node_self.nid, subscriber_node_other.nid)) as subscriber_count ";
  } else {
    $sql = "select distinct node.nid ";
  }
  $sql .= "from node ";

  // Join to the domain
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";

  // Join to the benefit
  $sql .= "left join field_data_field_sirius_trust_benefit on field_data_field_sirius_trust_benefit.entity_type = 'node' and field_data_field_sirius_trust_benefit.entity_id = node.nid ";

  // Join to the dependent
  $sql .= "left join field_data_field_sirius_worker dependent_worker on dependent_worker.entity_type = 'node' and dependent_worker.entity_id = node.nid ";
  $sql .= "left join node dependent_node on dependent_node.nid = dependent_worker.field_sirius_worker_target_id ";

  // Join to the relationship node
  $sql .= "left join field_data_field_sirius_contact_relation on field_data_field_sirius_contact_relation.entity_type = 'node' and field_data_field_sirius_contact_relation.entity_id = node.nid ";
  $sql .= "left join node relationship_node on relationship_node.nid = field_data_field_sirius_contact_relation.field_sirius_contact_relation_target_id ";

  // Join to the subscriber worker (other)
  $sql .= "left join field_data_field_sirius_contact subscriber_rel_contact on subscriber_rel_contact.entity_type = 'node' and subscriber_rel_contact.entity_id = relationship_node.nid ";
  $sql .= "left join node subscriber_contact_node on subscriber_contact_node.nid = subscriber_rel_contact.field_sirius_contact_target_id ";

  $sql .= "left join field_data_field_sirius_contact subscriber_wkr_contact on subscriber_wkr_contact.entity_type = 'node' and subscriber_wkr_contact.bundle = 'sirius_worker' and subscriber_wkr_contact.field_sirius_contact_target_id = subscriber_contact_node.nid ";
  $sql .= "left join node subscriber_node_other on subscriber_wkr_contact.entity_id is not null and subscriber_node_other.nid = subscriber_wkr_contact.entity_id ";

  // Join to the subscriber worker (self)
  $sql .= "left join node subscriber_node_self on subscriber_wkr_contact.entity_id is null and subscriber_node_self.nid = dependent_node.nid ";

  // Join to the relationship type
  $sql .= "left join field_data_field_sirius_contact_reltype on field_data_field_sirius_contact_reltype.entity_type = 'node' and field_data_field_sirius_contact_reltype.entity_id = relationship_node.nid ";

  // Join to the start and end dates
  $sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_date_end on field_data_field_sirius_date_end.entity_type = 'node' and field_data_field_sirius_date_end.entity_id = node.nid ";

  // Join to the employer
  $sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid ";


  $sql .= "where node.type = 'sirius_trust_worker_benefit' ";

  $domain_nid = sirius_domain_nid();
  if ($domain_nid) {
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  } else {
    $sql .= "and field_sirius_domain_target_id IS NULL ";
  }

  if ($keys['benefit']) {
    $sql .= 'and field_sirius_trust_benefit_target_id = :benefit ';
    $sql_args[':benefit'] = $keys['benefit'];
  }

  if ($keys['benefits']) {
    $sql .= 'and field_sirius_trust_benefit_target_id in (:benefits) ';
    $sql_args[':benefits'] = $keys['benefits'];
  }

  if ($keys['reltype']) {
    if ($keys['reltype'] == '_self') {
      $sql .= 'and field_sirius_contact_reltype_tid is null ';
    } else {
      $sql .= 'and field_sirius_contact_reltype_tid = :reltype ';
      $sql_args[':reltype'] = $keys['reltype'];
    }
  }

  if ($keys['reltypes']) {
    $sql .= 'and field_sirius_contact_reltype_tid in (:reltypes) ';
    $sql_args[':reltypes'] = $keys['reltypes'];
  }

  if ($keys['employer']) {
    $sql .= 'and field_grievance_shop_target_id = :employer ';
    $sql_args[':employer'] = $keys['employer'];
  }

  if ($keys['employers']) {
    $sql .= 'and field_grievance_shop_target_id in (:employers) ';
    $sql_args[':employers'] = $keys['employers'];
  }

  $sql_args[':asof'] = $asof;
  if (!$car || ($car == 'current')) {
    // drupal_set_message("... start_date <= $asof and (end date is null or end_date >= $asof)");
    $sql .= "and ( ";
    $sql .= "  field_sirius_date_start_value <= :asof ";
    $sql .= "  and (field_sirius_date_end_value is null or field_sirius_date_end_value >= :asof) ";
    $sql .= ") ";
  } else if ($car == 'add') {
    // drupal_set_message("... $cutoff <= start_date <= $asof ");
    $sql .= "and ( ";
    $sql .= "  field_sirius_date_start_value >= :cutoff ";
    $sql .= "  and field_sirius_date_start_value <= :asof ";
    $sql .= ") ";
    $sql_args[':cutoff'] = $cutoff;
  } else if ($car == 'remove') {
    // drupal_set_message("... $cutoff <= end_date <= $asof");
    $sql .= "and ( ";
    $sql .= "  field_sirius_date_end_value >= :cutoff ";
    $sql .= "  and field_sirius_date_end_value <= :asof ";
    $sql .= ") ";
    $sql_args[':cutoff'] = $cutoff;
  } else {
    drupal_set_mesasge("What is a $car?", 'error');
  }

  return sirius_sql_query($sql, $sql_args);
}

