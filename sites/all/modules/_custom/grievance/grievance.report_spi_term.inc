<?php

function grievance_report_spi_term() {
  return drupal_get_form('grievance_report_spi_term_form');
}

function grievance_report_spi_term_form($form, &$form_state) {
  $report_type = 'grievance-report-spi-term';
  report_cache_form_prepare($report_type, $form, $form_state);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function grievance_report_spi_term_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }

function grievance_report_spi_term_fields() {
  return array(
  	t('Shop'),
  	t('Grievant'), 
  	t('Grievance ID'), 
  	t('Violations'),
  	t('BOA Date'),
  	t('Sub Arb Date'),
  	t('Schedule Arb Date'),
  	t('Last Called Date'),
  	t('Closed Date'),
  	t('Action Date'),
  );
}

function grievance_report_spi_term_pks() {
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance');
  $query->propertyOrderBy('nid');
	$stmt = $query->execute();
	if (!$stmt['node']) { return array(); }

	return array_chunk(array_keys($stmt['node']), 100, TRUE);
}

function grievance_report_spi_term_batch($grievance_nids, $values, $report_id, $report_action, &$context) {

	$lookups = grievance_report_spi_term_lookups();

	$rows = array(
		'is_multiple' => TRUE,
		'rows' => array(),
	);
	foreach ($grievance_nids as $grievance_nid) {
		$row = grievance_report_spi_term_batch_nid($grievance_nid, $values, $report_id, $report_action, $context);
		if ($row) {
			$rows['rows'][$grievance_nid] = $row;
		}
	}
	return $rows;
}

function grievance_report_spi_term_batch_nid($grievance_nid, $lookups) {
	// Load the grievance
	$grievance_node = node_load($grievance_nid);
	// $context['message'] = t('Processing: @title', array('@title' => $grievance_node->title));

	// Load all the violations
	foreach ($grievance_node->field_grievance_clauseref['und'] as $tmp) {
		$clause_nid = $tmp['target_id'];
		$clause = node_load($clause_nid);
	}

	$shop_nid = $grievance_node->field_grievance_shop['und'][0]['target_id'];
	if (!$shop_nid) { return NULL; }
	$shop_node = node_load($shop_nid);
	if (!$shop_node) { return NULL; }

	$row = array();
	$row[] = $shop_node->title;
	$row[] = $grievance_node->field_grievance_last_name['und'][0]['value'] . ', ' . $grievance_node->field_grievance_first_name['und'][0]['value'];
	$row[] = $grievance_node->field_grievance_id['und'][0]['value'];

	return $row;
}

function grievance_report_spi_term_lookups() {
  // Get the list of open and closed statuses
  $status_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_status');
  $status_terms = array();
  $tmp = taxonomy_get_tree($status_vocabulary->vid);
  foreach ($tmp as $status_term) {
    $status_terms[$status_term->tid] = taxonomy_term_load($status_term->tid);
  }
  $open_status_tids = array(-999);
  $closed_status_tids = array(-999);
  $arbitration_status_tids = array(-999);
  $fmcs_status_tids = array(-999);
  $adr_status_tids = array(-999);
  $formal_arb_held_status_tids = array(-999);
  $expedited_arb_held_status_tids = array(-999);
  $victory_status_tids = array(-999);
  $boa_held_status_tids = array(-999);

  foreach ($status_terms as $tid => $term) {
    if ($term->field_grievance_open['und'][0]['value'] == 1) {
      $open_status_tids[] = $tid;
    }
    if ($term->field_grievance_open['und'][0]['value'] == 0) {
      $closed_status_tids[] = $tid;
    }
    if (preg_match('/arbitration/i', $term->name)) {
      $arbitration_status_tids[] = $tid;
    }
    if (preg_match('/FMCS Held/i', $term->name)) {
      $fmcs_status_tids[] = $tid;
    }
    if (preg_match('/ADR Held/i', $term->name)) {
      $adr_status_tids[] = $tid;
    }
    if (preg_match('/Formal Arbitration Held/i', $term->name)) {
      $formal_arb_held_status_tids[] = $tid;
    }
    if (preg_match('/Expedited Arbitration Held/i', $term->name)) {
      $expedited_arb_held_status_tids[] = $tid;
    }
    if (preg_match('/(resolved|won)/i', $term->name)) {
      $victory_status_tids[] = $tid;
    }
    if (preg_match('/BOA Held/i', $term->name)) {
      $boa_held_status_tids[] = $tid;
    }
  }

  // Get the list of "SPI" nids
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_contract_clause');
  $query->propertyCondition('title', '%Suspension Pending Investigation%', 'like');
  $stmt = $query->execute();
  $spi_nids = array(-999);
  if ($stmt['node']) {
    $spi_nids = array_keys($stmt['node']);
  }

  // Get the list of "Term" nids
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_contract_clause');
  $query->propertyCondition('title', '%Discharge%', 'like');
  $stmt = $query->execute();
  $term_nids = array(-999);
  if ($stmt['node']) {
    $term_nids = array_keys($stmt['node']);
  }

  return array(
		'open_status_tids' 								=> $open_status_tids,
		'closed_status_tids'							=> $closed_status_tids,
		'arbitration_status_tids'					=> $arbitration_status_tids,
		'fmcs_status_tids' 								=> $fmcs_status_tids,
		'adr_status_tids' 								=> $adr_status_tids,
		'formal_arb_held_status_tids' 		=> $formal_arb_held_status_tids,
		'expedited_arb_held_status_tids' 	=> $expedited_arb_held_status_tids,
		'victory_status_tids' 						=> $victory_status_tids,
		'boa_held_status_tids' 						=> $boa_held_status_tids,
		'spi_nids' 												=> $spi_nids,
		'term_nids' 											=> $term_nids,
  );
}