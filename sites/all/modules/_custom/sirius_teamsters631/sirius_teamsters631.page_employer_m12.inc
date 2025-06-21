<?php

function sirius_teamsters631_node_tab_employer_m12_form($form, &$form_state, $employer_node) {
	$m12 = Sirius_Teamsters631_M12::getInstance();

	$form_state['employer_nid'] = $employer_node->nid;

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Current Report'),
	);

  $search_params = array();
  $search_params['employer_nid'] = $employer_node->nid;
  $search_params['limit'] = 100;
  $m12_nodes = $m12->search($search_params);

  if ($m12_nodes) {
  	$html = '<h2>' . t('Prior Reports') . '</h2>';

  	$status_options = $m12->status_options();

  	$rows = array();
	  foreach ($m12_nodes as $m12_nid => $m12_node) {
	  	$m12_json = sirius_json_get($m12_node);
	  	$status = $m12->get_status($m12_node);

	  	$tools = array();
	  	$tools[] = l('View', "node/" . $m12_node->nid . "/sirius_log_teamsters631_m12", array('attributes' => array('class' => array('tool-button'))));
	  	$row = array();
	  	$row[] = $m12_json['teamsters631_m12']['month'] . ' / ' . $m12_json['teamsters631_m12']['year'];
	  	//$row[] = $m12_json['teamsters631_m12']['employer_name'];
	  	//$row[] = $status_options[$status];
	  	$row[] = join('', $tools);
	  	$rows[] = $row;
	  }

	  $header = array();
	  $header[] = t('Date');
	  //$header[] = t('Employer');
	  //$header[] = t('Status');
	  $header[] = t('Tools');

	  $html .= sirius_table_render($header, $rows);
	  $form['prior_reports'] = array('#markup' => $html);
	}

	return $form;
}

function sirius_teamsters631_node_tab_employer_m12_form_submit($form, &$form_state) {
	$m12 = Sirius_Teamsters631_M12::getInstance();
	$employer_nid =$form_state['employer_nid'];
	$m12_node = $m12->fetch($employer_nid, date('Y'), date('n'), TRUE);
	drupal_goto("node/" . $m12_node->nid . "/sirius_log_teamsters631_m12");
}