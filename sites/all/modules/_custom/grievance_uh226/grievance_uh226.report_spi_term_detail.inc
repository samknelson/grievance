<?php

function grievance_uh226_report_spi_term_detail() {
  return drupal_get_form('grievance_uh226_report_spi_term_detail_form');
}

function grievance_uh226_report_spi_term_detail_form($form, &$form_state) {
  $report_type = 'grievance-uh226-report-spi-term-detail';
  report_cache_form_prepare($report_type, $form, $form_state);

  $form['from_date'] = array(
    '#type' => 'date_popup',
    '#title' => 'Start Date',
    '#date_format' => 'm/d/Y',
  );

  $form['to_date'] = array(
    '#type' => 'date_popup',
    '#title' => 'End Date',
    '#date_format' => 'm/d/Y',
  );

  $form['open_only'] = array(
    '#type' => 'select',
    '#title' => 'Open Grievances Only?',
    '#required' => TRUE,
    '#options' => array(
      'yes' => 'Yes - Include only open grievances', 
      'no' => 'No - Include both open and closed grievances'
    ),
  );

  $form['organizer'] = array(
    '#type' => 'entityreference',
    '#title' => t('Specialist'),
    '#era_entity_type' => 'user',
    '#era_bundles' => array(),
    '#era_query_settings' => array(),
      '#description' => t('Leave blank to select all specialists.'),
    '#prefix' => '<div class="form-start">',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function grievance_uh226_report_spi_term_detail_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }

function grievance_uh226_report_spi_term_detail_fields() {
  return array(
    'employer_name' => 'Employer Name',
    'specialist_name' => 'Specialist Name',
    'grievant_name' => 'Grievant',
    'grievance_id' => 'Griev#',
    'spi_date' => 'SPI Date',
    'term_date' => 'Term Date',
    'boa_date' => 'BOA Date',
    'arb_sub_date' => 'Sub Arb Date',
    'arb_held_date' => 'Arb Date',
    'last_call_date' => 'Last Called Date',
  );
}

function grievance_uh226_report_spi_term_detail_pks(&$form_state) {
  $values = array();

  if ($form_state['values']['from_date']) {
    $from_ts = strtotime($form_state['values']['from_date']);
    $values[':from_ts'] = $from_ts;
  }
  if ($form_state['values']['to_date']) {
    $to_ts = strtotime($form_state['values']['to_date']);
    $values[':to_ts'] = $to_ts;
  }

  $lookups = grievance_uh226_report_spi_term_detail_lookups();

  $sql = "select distinct node.nid ";
  $sql .= "from field_data_field_grievance_log_type, field_data_field_grievance, node ";
  if ($form_state['values']['organizer']['entity_id']) {
    $sql .= ", field_data_field_grievance_rep_organizer ";
  }
  if ($form_state['values']['open_only'] == 'yes') {
    $sql .= ", field_data_field_grievance_status ";
  }
  $sql .= "where field_data_field_grievance.entity_type = 'node' ";

  $sql .= "and field_data_field_grievance_log_type.entity_type = 'node' ";
  $sql .= "and field_data_field_grievance.entity_id = field_data_field_grievance_log_type.entity_id ";
  $sql .= "and node.nid = field_data_field_grievance.field_grievance_target_id ";
  $sql .= "and field_data_field_grievance_log_type.field_grievance_log_type_tid in ($lookups[spi_log_type_tid], $lookups[term_log_type_tid]) ";

  $sql .= "and node.type = 'grievance' ";
  if ($from_ts) {
    $sql .= "and node.created >= :from_ts ";
  }
  if ($to_ts) {
    $sql .= "and node.created <= :to_ts ";
  }

  if ($form_state['values']['open_only'] == 'yes') {
    $sql .= "and field_data_field_grievance_status.entity_type = 'node' ";
    $sql .= "and field_data_field_grievance_status.entity_id = node.nid ";
    $sql .= "and field_data_field_grievance_status.field_grievance_status_tid in (" . join(',', $lookups[open_status_tids]) . ") ";
  }

  if ($form_state['values']['organizer']['entity_id']) {
    $sql .= "and field_data_field_grievance_rep_organizer.entity_type = 'node' ";
    $sql .= "and field_data_field_grievance_rep_organizer.entity_id = node.nid ";
    $sql .= "and field_data_field_grievance_rep_organizer.field_grievance_rep_organizer_target_id = :organizer_id ";
    $values[':organizer_id'] = $form_state['values']['organizer']['entity_id'];
  }

  $sql .= "order by node.nid ";

  $stmt = db_query($sql, $values);
  $nids = array();
  while ($hr = $stmt->fetchAssoc()) { $nids[] = $hr['nid']; }

	return array_chunk($nids, 100, TRUE);
}

function grievance_uh226_report_spi_term_detail_batch($grievance_nids, $values, $report_id, $report_action, &$context) {
	$rows = array(
		'is_multiple' => TRUE,
		'rows' => array(),
	);

  $lookups = grievance_uh226_report_spi_term_detail_lookups();
	foreach ($grievance_nids as $grievance_nid) {
		$row = grievance_uh226_report_spi_term_detail_nid($grievance_nid, $lookups, $values);
		if ($row) {
			$rows['rows'][$grievance_nid] = $row;
		}
	}
	return $rows;
}

function grievance_uh226_report_spi_term_detail_nid($grievance_nid, $lookups, $values) {
  // Our result row
  $row = array();
  $row['grievance_nid'] = $grievance_nid;

	// Load the grievance
	$grievance_node = node_load($grievance_nid);

  // Specialist name
  // @todo user_load is expensive.
  if ($grievance_node->field_grievance_rep_organizer['und'][0]['target_id']) {
    $user = user_load($grievance_node->field_grievance_rep_organizer['und'][0]['target_id']);
    $row['specialist_name'] = $user->name;
  }

  // Employer name
  // @todo node_load is expensive
  if ($grievance_node->field_grievance_shop['und'][0]['target_id']) {
    $employer_node = node_load($grievance_node->field_grievance_shop['und'][0]['target_id']);
    $row['employer_name'] = $employer_node->title;
  }

  // Grievant name, grievance ID
  $row['grievant_name'] = $grievance_node->field_grievance_first_name['und'][0]['value'] . ' ' . $grievance_node->field_grievance_last_name['und'][0]['value'];
  $row['grievance_id'] = $grievance_node->field_grievance_id['und'][0]['value'];

  // SPI date
  if ($values['from_date']) {
    $from_date = date('Y-m-d 00:00:00', strtotime($values['from_date']));
  }
  if ($values['to_date']) {
    $to_date = date('Y-m-d 00:00:00', strtotime($values['to_date']));
  }
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $lookups['spi_log_type_tid']);
  if ($from_date) {
    $query->fieldCondition('field_grievance_status_date', 'value', $from_date, '>=');
  }
  if ($to_date) {
    $query->fieldCondition('field_grievance_status_date', 'value', $to_date, '<=');
  }
  $query->fieldOrderBy('field_grievance_status_date', 'value');
  $query->range(0, 1);
  $stmt = $query->execute();
  if ($stmt['node']) {
    $log_nids = array_keys($stmt['node']);
    $log_nid = $log_nids[0];
    $log_node = node_load($log_nid);
    $row['spi_date'] = strtotime($log_node->field_grievance_status_date['und'][0]['value']);
  }


  // Term date
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $lookups['term_log_type_tid']);
  if ($from_date) {
    $query->fieldCondition('field_grievance_status_date', 'value', $from_date, '>=');
  }
  if ($to_date) {
    $query->fieldCondition('field_grievance_status_date', 'value', $to_date, '<=');
  }
  $query->fieldOrderBy('field_grievance_status_date', 'value');
  $query->range(0, 1);
  $stmt = $query->execute();
  if ($stmt['node']) {
    $log_nids = array_keys($stmt['node']);
    $log_nid = $log_nids[0];
    $log_node = node_load($log_nid);
    $row['term_date'] = strtotime($log_node->field_grievance_status_date['und'][0]['value']);
  }

  // 
  // if there is a written warning, final warning, or suspension *after* the SPI or Term,
  // then this row can be excluded from the report.
  //

  $exclude_start_ts = max($row['term_date'], $row['spi_date']);
  if ($exclude_start_ts) {
    $written_warning_log_type_tid = $lookups['written_warning_log_type_tid'];
    $final_warning_log_type_tid = $lookups['final_warning_log_type_tid'];
    $suspension_log_type_tid = $lookups['suspension_log_type_tid'];

    $exclude_start_date = date('Y-m-d H:i:s', $exclude_start_ts);
    $query = new EntityFieldQuery;
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'grievance_log');
    $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
    $query->fieldCondition('field_grievance_log_type', 'tid', array($written_warning_log_type_tid, $final_warning_log_type_tid, $suspension_log_type_tid), 'in');
    if ($from_date) {
      $query->fieldCondition('field_grievance_status_date', 'value', $exclude_start_date, '>');
    }
    $query->range(0, 1);
    $stmt = $query->execute();
    if ($stmt['node']) {
      return array();
    }
  }

  // BOA date
  $log_update_tids = array(
    variable_get('grievance_log_insert_type', ''), 
    variable_get('grievance_log_update_type_with_status_change', ''), 
  );
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $log_update_tids, 'in');
  $query->fieldCondition('field_grievance_status', 'tid', $lookups['boa_held_status_tids'], 'in');
  $query->fieldOrderBy('field_grievance_status_date', 'value');
  $query->range(0, 1);
  $stmt = $query->execute();
  if ($stmt['node']) {
    $log_nids = array_keys($stmt['node']);
    $log_nid = $log_nids[0];
    $log_node = node_load($log_nid);
    $row['boa_date'] = strtotime($log_node->field_grievance_status_date['und'][0]['value']);
  }

  // Sub Arb Date
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $log_update_tids, 'in');
  $query->fieldCondition('field_grievance_status', 'tid', $lookups['arb_sub_status_tids'], 'in');
  $query->fieldOrderBy('field_grievance_status_date', 'value');
  $query->range(0, 1);
  $stmt = $query->execute();
  if ($stmt['node']) {
    $log_nids = array_keys($stmt['node']);
    $log_nid = $log_nids[0];
    $log_node = node_load($log_nid);
    $row['arb_sub_date'] = strtotime($log_node->field_grievance_status_date['und'][0]['value']);
  }

  // Held Arb Date
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $log_update_tids, 'in');
  $query->fieldCondition('field_grievance_status', 'tid', $lookups['arb_held_status_tids'], 'in');
  $query->fieldOrderBy('field_grievance_status_date', 'value');
  $query->range(0, 1);
  $stmt = $query->execute();
  if ($stmt['node']) {
    $log_nids = array_keys($stmt['node']);
    $log_nid = $log_nids[0];
    $log_node = node_load($log_nid);
    $row['arb_held_date'] = strtotime($log_node->field_grievance_status_date['und'][0]['value']);
  }

  // Last called date
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $lookups['communication_log_type_tids'], 'in');
  $query->fieldOrderBy('field_grievance_status_date', 'value', 'desc');
  $query->range(0, 1);
  $stmt = $query->execute();
  if ($stmt['node']) {
    $log_nids = array_keys($stmt['node']);
    $log_nid = $log_nids[0];
    $log_node = node_load($log_nid);
    $row['last_call_date'] = strtotime($log_node->field_grievance_status_date['und'][0]['value']);
  }

  return $row;
}

function grievance_uh226_report_spi_term_detail_lookups() {
  // Get the list of open and closed statuses
  $status_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_status');
  $status_terms = array();
  $tmp = taxonomy_get_tree($status_vocabulary->vid);
  foreach ($tmp as $status_term) {
    $status_terms[$status_term->tid] = taxonomy_term_load($status_term->tid);
  }
  $open_status_tids = array(-999);
  $closed_status_tids = array(-999);
  $rslvd_status_tids = array(-999);
  $nfa_status_tids = array(-999);
  $won_status_tids = array(-999);
  $lost_status_tids = array(-999);
  $arb_sub_status_tids = array(-999);
  $arb_held_status_tids = array(-999);
  $boa_held_status_tids = array(-999);

  foreach ($status_terms as $tid => $term) {
    if ($term->field_grievance_open['und'][0]['value'] == 1) {
      $open_status_tids[] = $tid;
    }
    if ($term->field_grievance_open['und'][0]['value'] == 0) {
      $closed_status_tids[] = $tid;
    }
    if (preg_match('/resolved/i', $term->name)) {
      $rslvd_status_tids[] = $tid;
    }
    if (preg_match('/nfa/i', $term->name)) {
      $nfa_status_tids[] = $tid;
    }
    if (preg_match('/BOA Held/i', $term->name)) {
      $boa_held_status_tids[] = $tid;
    }
    if (preg_match('/Won/i', $term->name)) {
      $won_tids[] = $tid;
    }
    if (preg_match('/Lost/i', $term->name)) {
      $lost_tids[] = $tid;
    }
    if (preg_match('/referred/i', $term->name)) {
      $arb_sub_status_tids[] = $tid;
    }
    if (preg_match('/held/i', $term->name)) {
      $arb_held_status_tids[] = $tid;
    }
  }

  // Get the list of taxonomy terms which begin "Communication - Grievant".  
  $log_type_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_log_types');
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'taxonomy_term');
  $query->propertyCondition('vid', $log_type_vocabulary->vid);
  $query->propertyCondition('name', 'Communication - Grievant%', 'LIKE');
  $stmt = $query->execute();
  $communication_log_type_tids = array();
  if ($stmt['taxonomy_term']) {
    $communication_log_type_tids = array_keys($stmt['taxonomy_term']);
  }

  $result = array(
		'open_status_tids' 								=> $open_status_tids,
		'closed_status_tids'							=> $closed_status_tids,
		'arbitration_status_tids'					=> $arbitration_status_tids,
    'rslvd_status_tids'               => $rslvd_status_tids,
    'nfa_status_tids'                 => $nfa_status_tids,
    'arb_sub_status_tids'             => $arb_sub_status_tids,
    'arb_held_status_tids'            => $arb_held_status_tids,
    'won_tids'                        => $won_tids,
    'lost_tids'                       => $lost_tids,
		'boa_held_status_tids' 						=> $boa_held_status_tids,
    'spi_log_type_tid'                => variable_get('grievance_log_spi_type', ''),
    'term_log_type_tid'               => variable_get('grievance_log_term_type', ''),
    'written_warning_log_type_tid'    => variable_get('grievance_log_written_warning_type', ''),
    'final_warning_log_type_tid'      => variable_get('grievance_log_final_warning_type', ''),
    'suspension_log_type_tid'         => variable_get('grievance_log_suspension_type', ''),
    'communication_log_type_tids'     => $communication_log_type_tids,
  );

  return $result;
}

function grievance_uh226_report_spi_term_detail_render_specialist($report_type, $report_id) {
  return grievance_uh226_report_spi_term_detail_render($report_type, $report_id, 'specialist');
}

function grievance_uh226_report_spi_term_detail_render_employer($report_type, $report_id) {
  return grievance_uh226_report_spi_term_detail_render($report_type, $report_id, 'employer');
}

function grievance_uh226_report_spi_term_detail_render($report_type, $report_id, $which) {
  // Load the parameters
  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type . ' :: PARAMETERS');
  $stmt = $query->execute();
  $hr = $stmt->fetchAssoc();
  $parameters = unserialize($hr['report_data']);

  // Construct the page title
  $html = '';
  $html .= '<div class="uh-226-report-spi-term-detail-render">';
  $html .= '<h2>';
  if ($parameters['from_date'] && $parameters['to_date']) {
    $html .= t('Grievances from @from_date to @to_date', array(
        '@from_date' => date('m/d/Y', strtotime($parameters['from_date'])),
        '@to_date' => date('m/d/Y', strtotime($parameters['to_date'])),
      )
    );
  } else if ($parameters['from_date']) {
    $html .= t('Grievances from @from_date', array(
        '@from_date' => date('m/d/Y', strtotime($parameters['from_date'])),
      )
    );
  } else if ($parameters['to_date']) {
    $html .= t('Grievances through @to_date', array(
        '@to_date' => date('m/d/Y', strtotime($parameters['to_date'])),
      )
    );
  } else {
    $html .= 'All Dates';
  }
  if ($parameters['open_only'] == 'yes') {
    $html .= ' -- Open Grievances Only';
  } else {
    $html .= ' -- All Grievances';
  }
  if ($parameters['organizer']['entity_label']) {
    $html .= ' -- ' . $parameters['organizer']['entity_label'];
  } else {
    $html .= ' -- All Specialists';
  }
  if ($which == 'specialist') {
    $html .= ' -- By Specialist';
  } else {
    $html .= ' -- By Employer';
  }
  $html .= '</h2>';

  $data = array();
  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $sub_totals = array();
  $totals = array();

  $data = array();
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);
    if ($which == 'specialist') {
      $key = $hr['specialist_name'];
    } else {
      $key = $hr['employer_name'];
    }
    $data[$key][] = $hr;
  }

  /*
  'employer_name' => 'Employer Name',
  'specialist_name' => 'Specialist Name',
  'grievant_name' => 'Grievant',
  'grievance_id' => 'Griev#',
  'spi_date' => 'SPI Date',
  'term_date' => 'Term Date',
  'boa_date' => 'BOA Date',
  'arb_sub_date' => 'Sub Arb Date',
  'arb_held_date' => 'Arb Date',
  'last_call_date' => 'Last Called Date',
  */

  $header = array();
  $header[] = 'Grievant';
  $header[] = 'Griev#';
  $header[] = 'Violations';
  $header[] = 'BOA Date';
  $header[] = 'Sub Arb Date';
  $header[] = 'Arb Date';
  $header[] = 'Last Called Date';

  $attributes = array();

  ksort($data);
  foreach ($data as $key => $sub_data) {
    $html .= '<h3>' . $key . '</h3>';

    $rows = array();
    foreach ($sub_data as $hr) {
      $row = array();
      $row[] = $hr['grievant_name'];
      $row[] = $hr['grievance_id'];

      $violations = '';
      if ($hr['spi_date']) {
        $violations .= 'SPI-' . date('m/d/Y', $hr['spi_date']);
      }
      if ($hr['term_date']) {
        if ($violations) { $violations .= '; '; }
        $violations .= 'TERM-' . date('m/d/Y', $hr['term_date']);
      }
      $row[] = $violations;

      if ($hr['boa_date']) {
        $row[] = date('m/d/Y', $hr['boa_date']);
      } else {
        $row[] = '';
      }

      if ($hr['arb_sub_date']) {
        $row[] = date('m/d/Y', $hr['arb_sub_date']);
      } else {
        $row[] = '';
      }

      if ($hr['arb_held_date']) {
        $row[] = date('m/d/Y', $hr['arb_held_date']);
      } else {
        $row[] = '';
      }

      if ($hr['last_call_date']) {
        $row[] = date('m/d/Y', $hr['last_call_date']);
      } else {
        $row[] = '';
      }


      $rows[] = $row;
    }
    $html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => $attributes));
  }
  return $html;
}
