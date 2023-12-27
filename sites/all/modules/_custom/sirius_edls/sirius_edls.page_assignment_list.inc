<?php

function sirius_edls_page_assignment_list() {
	$result = sirius_edls_employer_switch();
	$html = $result['markup'];
	$employer_node = $result['employer_node'];
	if (!$employer_node) {
		$html .= sirius_page_error('Please select an employer.');
		return $html;
	}

	$html .= drupal_render(drupal_get_form('sirius_edls_page_assignment_list_form', $employer_node));
	return $html;
}

function sirius_edls_page_assignment_list_form($form, &$form_state, $employer_node) {
  $form['#tree'] = TRUE;

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

	$form['filters']['grievance_shop'] = array(
		'#type' => 'value',
		'#value' => $employer_node->nid,
	);

  $form['filters']['supervisor'] = array(
	  '#type' => 'select',
	  '#title' => t('Supervisor'),
		'#options' => array(NULL => t('-- Select --')) + sirius_edls_supervisor_list($employer_node->nid),
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
		'#options' => array(NULL => t('-- Select --')) + sirius_edls_departments($employer_node),
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
  	if ($params['status'] == 'trash') { $params['include_trash'] = TRUE; }
  } else {
  	$params = array();
  }
	$params['date'] = $date;
	$params['grievance_shop'] = $employer_node->nid;
  $assignments = sirius_edls_assignment_search($params);

  $render_options = array();
  $render_options['hide_employer'] = TRUE;
  $form['results'] = array('#markup' => sirius_edls_assignment_list_render($assignments, $render_options));

  return $form;
}

function sirius_edls_page_assignment_list_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}
