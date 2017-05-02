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

	return array_keys($stmt['node']);
}

function grievance_report_spi_term_batch($grievance_nid, $values, $report_id, $report_action, &$context) {
	$grievance_node = node_load($grievance_nid);
	$context['message'] = t('Generating statement for: @title', array('@title' => $grievance_node->title));

	return array(
		$grievance_node->field_grievance_id['und'][0]['value']
	);

	return NULL;
}
