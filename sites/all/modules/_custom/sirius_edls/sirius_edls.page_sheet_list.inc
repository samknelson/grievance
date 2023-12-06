<?php

function sirius_edls_page_sheet_list() {
	return drupal_get_form('sirius_edls_page_sheet_list_form');
}

function sirius_edls_page_sheet_list_form($form, &$form_state) {
  $my_employer_node = sirius_employer_get();
  $form['#tree'] = TRUE;

	$form['header_links'] = array(
		'#markup' => '<p>' . l('Create a New Sheet', 'sirius/edls/sheet/create', array('attributes' => array('class' => array('tool-button')))) . '</p>',
	);

	if ($form_state['submitted']) {
		if ($form_state['values']['filters']['date'] == 'other') {
			if ($form_state['values']['filters']['date_other']) {
				$date = date('Y-m-d', strtotime($form_state['values']['filters']['date_other']));
			} else {
				$date = date('Y-m-d');
			}
		} else {
			$date = date('Y-m-d', strtotime($form_state['values']['filters']['date']));
		}
	} else {
		$date = date('Y-m-d');
	}
	drupal_set_title('EDLS Sheets for ' . date('l, F j, Y', strtotime($date)));

	$form['filters'] = array(
	  '#type' => 'fieldset',
	  '#title' => 'filters',
	  '#collapsible' => TRUE,
	  '#collapsed' => FALSE,
  );

  $form['filters']['status'] = array(
	  '#type' => 'select',
	  '#title' => t('Status'),
		'#options' => array(NULL => t('-- Select --')) + sirius_edls_sheet_status_options(),
  );

	$my_employer_node = sirius_employer_get();
	if ($my_employer_node) {
		$form['filters']['grievance_shop'] = array(
			'#type' => 'value',
			'#value' => $my_employer_node->nid,
		);
	} else {
		$employer_options = sirius_employer_list(TRUE);
		$form['filters']['grievance_shop'] = array(
			'#type' => 'select',
			'#title' => t('Employer'),
			'#options' => array(NULL => t('-- Select --')) + $employer_options,
		);
	}

  $form['filters']['supervisor'] = array(
	  '#type' => 'select',
	  '#title' => t('Supervisor'),
		'#options' => array(NULL => t('-- Select --')) + sirius_edls_supervisor_list($my_employer_node->nid),
  );

	$date_options = array();
	foreach (array("-1 days", "today", "+1 day", "+2 day", "+3 days", "+4 days", "+5 days", "+6 days") as $d) {
		$t = strtotime($d);

		if ($d == '-1 days') { $name = t('Yesterday'); }
		else if ($d == 'today') { $name = t('Today'); }
		else if ($d == '+1 day') { $name = t('Tomorrow'); }
		else { $name = date('l (j F)', $t); }

		$date_options[date('Y-m-d', $t)] = $name;
	}
	$date_options['other'] = t('Other Date');

  $form['filters']['date'] = array(
	  '#type' => 'select',
	  '#title' => t('Date'),
	  '#options' => $date_options,
		'#default_value' => $date,
  );

  $form['filters']['date_other'] = array(
	  '#type' => 'date_popup',
	  '#title' => t('Date'),
		'#attributes' => array(
			'autocomplete' => 'off',
		),
		'#date_format' => 'm/d/Y',
		'#default_value' => $date,
		'#states' => array('visible' => array(":input[name=\"filters[date]\"]" => array('value' => 'other'))),
  );

  $form['filters']['department'] = array(
	  '#type' => 'select',
	  '#title' => t('Department'),
		'#options' => array(NULL => t('-- Select --')) + sirius_taxonomy_load('grievance_department', 'tid', 'name'),
  );

  $form['filters']['job_number'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Job Number'),
	  '#size' => 10,
  );

  $form['filters']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  if ($form_state['submitted']) {
  	$params = $form_state['values']['filters'];
  	$params['date'] = $date;

  	if ($params['status'] == 'trash') { $params['include_trash'] = TRUE; }
  } else {
  	$params = array();

  	$params['date'] = $date;
  	if ($my_employer_node) { $params['grievance_shop'] = $my_employer_node->nid; }
  }
  $sheet_nodes = sirius_edls_sheet_search($params);

  $render_options = array();
  $render_options['hide_date'] = TRUE;
  if ($my_employer_node) { $render_options['hide_employer'] = TRUE; }
  $form['results'] = array('#markup' => sirius_edls_sheet_list_render($sheet_nodes, $render_options));

	$form['footer_links'] = $form['header_links'];

  return $form;
}

function sirius_edls_page_sheet_list_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}
