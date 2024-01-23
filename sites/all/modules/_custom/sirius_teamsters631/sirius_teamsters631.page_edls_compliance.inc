<?php

function sirius_edls_page_631compliance() {
	drupal_add_css(drupal_get_path('module', 'report_cache') . '/css/report_cache.css');
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/form.css');
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');

	$result = sirius_edls_employer_switch();
	$html = $result['markup'];
	$employer_node = $result['employer_node'];
	if (!$employer_node) {
		$html .= sirius_page_error('Please select an employer.');
		return $html;
	}

	$date_input = $_REQUEST['date'];
	if ($_REQUEST['date_other']) { 
		$date_other_input = $_REQUEST['date_other']['date'];
	} else {
		$date_other_input = NULL;
	}
	if ($date_input) {
		if ($date_input == 'other') {
			if ($date_other_input) {
				$date = date('Y-m-d', strtotime($date_other_input));
			} else {
				$date = date('Y-m-d');
			}
		} else {
			$date = date('Y-m-d', strtotime($date_input));
		}
	} else {
		$date = date('Y-m-d');
	}
	if (!$date_input) { $date_input = $date; }

  $report_type = 'sirius-teamsters631-edls-compliance';
  $info = report_cache_info($report_type);
  $generator = $info['generator'];
  $reports = $generator->search(array('limit' => 1, 'parameters' => array('employer' => $employer_node->nid, 'date' => $date)));
  $report_id = array_keys($reports)[0];

  if ($report_id) { $parameters = $reports[$report_id];	} else { $parameters = array(); }

	$html .= '<div class="sirius_callout_basic" style="width: 40%; float: left;">';
	$html .= drupal_render(drupal_get_form('sirius_edls_page_631compliance_form', $employer_node, $date, $parameters));
	$html .= '</div>';

	$html .= '<div class="sirius_callout_basic" style="width: 40%; float: right;">';
	$html .= drupal_render(drupal_get_form('sirius_edls_page_631compliance_date_form', $employer_node, $date, $date_input, $date_other_input));
	$html .= '</div>';

	if (!$report_id) { return $html; }

	$html .= '<br style="clear: both;">';

  $date_render = date('l, F j, Y', strtotime($date));
  $html .= "<h2>Compliance Report for $date_render</h2>";

  $report_type = 'sirius-teamsters631-edls-compliance';
  $info = report_cache_info($report_type);
  $generator = $info['generator'];
  $reports = $generator->search(array('limit' => 1, 'parameters' => array('employer' => $employer_node->nid, 'date' => $date)));
  $report_id = array_keys($reports)[0];

	$result = $generator->callback_wrapper_render('render', $report_id, 'page', array());

	$html .= '<div class="report_cache_render_body">';

	if (is_array($result)) {
		if ($result['header']) {
			$result['attributes']['class'][] = 'report_cache_table';
			$html .= sirius_table_render($result['header'], $result['rows'], $result['attributes']);
		}
	} else {
		$html .= $result;
	}

	$html .= '</div>';

	$html .= report_cache_render_meta($parameters);

	return $html;
}

function sirius_edls_page_631compliance_date_form($form, &$form_state, $employer_node, $date, $date_input, $date_other_input) {
	$form['#method'] = 'get';
	$form['#prefix'] = '<div class="sirius_form_inline">';
	$form['#suffix'] = '</div>';

	$form['employer_nid'] = array('#type' => 'hidden', '#value' => $employer_node->nid);

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

  $form['date'] = array(
	  '#type' => 'select',
	  '#title' => t('Date'),
	  '#options' => $date_options,
		'#default_value' => $date_input,
		'#id' => 'date_input',
  );

  $form['date_other'] = array(
	  '#type' => 'date_popup',
	  '#title' => t('Other Date'),
		'#attributes' => array(
			'autocomplete' => 'off',
		),
		'#date_format' => 'm/d/Y',
		'#default_value' => $date,
		'#states' => array('visible' => array(":input[id=\"date_input\"]" => array('value' => 'other'))),
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Switch Date'));

  return $form;
}

function sirius_edls_page_631compliance_form($form, &$form_state, $employer_node, $date, $last_run_parameters) {
  $form['#tree'] = TRUE;
  $form_state['employer_nid'] = $employer_node->nid;

  $ts = $last_run_parameters['meta']['ts']['completed'];
  $msg = '<p>';
  if ($ts) {
  	$msg = 'The compliance report was last run on <strong>';
  	$msg .= date('m/d/Y h:i a', $ts);
		$interval = time() - $ts;
		$msg .= ' (' . format_interval($interval, 2) . ' ago)';
		$msg .= '</strong>';
	} else {
		$msg = 'The compliance report has not been run yet.';
	}
	$msg .= '</p>';

	$form['msg'] = array('#markup' => $msg);

  $report_type = 'sirius-teamsters631-edls-compliance';
  $info = report_cache_info($report_type);
  $generator = $info['generator'];
  report_cache_form_prepare($report_type, $form, $form_state);

  // We hide some of the report_cache paraphernalia
  unset($form['report_cache']);
  $form['report_type'] = array('#type' => 'value', '#value' => $report_type);

  // Instead of displaying the parameters form, we set them ourself.
	// $form['parameters'] = $info['generator']->form();
	$form['parameters']['employer']['#type'] = 'value';
	$form['parameters']['employer']['#value'] = $employer_node->nid;
	$form['parameters']['status_options']['#type'] = 'value';
	$form['parameters']['status_options']['#value'] = array('request', 'lock');
	$form['parameters']['date']['#type'] = 'value';
	$form['parameters']['date']['#value'] = $date;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Run the Compliance Report',
  );

	$form['results'] = array('#markup' => $html);

  return $form;
}

function sirius_edls_page_631compliance_form_submit($form, &$form_state) {
	$form_state['values']['redirect'] = array(
		'path' => current_path(),
		'query' => array(
			'date' => $_GET['date'],
			'date_other' => $_GET['date_other'],
			'employer_nid' => $_GET['employer_nid'],
		),
	);
	$form_state['values']['success_msg'] = 'The compliance report has been generated.';
  return report_cache_form_submit($form, $form_state);
}

