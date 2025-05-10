<?php

function sirius_edls_page_worker_sts() {
	drupal_add_css(drupal_get_path('module', 'report_cache') . '/css/report_cache.css');
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/form.css');
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');

	// Get an employer
	$result = sirius_edls_employer_switch();
	$html = $result['markup'];
	$employer_node = $result['employer_node'];
	if (!$employer_node) {
		$html .= sirius_page_error('Please select an employer.');
		return $html;
	}

	// Form to re-run the report
  $report_type = 'sirius-edls-sts';
  $info = report_cache_info($report_type);
  $generator = $info['generator'];
  $reports = $generator->search(array('limit' => 1, 'parameters' => array('employer' => $employer_node->nid)));
  $report_id = array_keys($reports)[0];

  if ($report_id) { $parameters = $reports[$report_id];	} else { $parameters = array(); }

  $html .= '<div class="sirius_callout_fullwidth">';
	$html .= drupal_render(drupal_get_form('sirius_edls_page_worker_sts_form', $employer_node, $parameters));
	$html .= '</div>';

	// Form the show the results
	if (!$report_id) { return $html; }
  $form = drupal_get_form('report_cache_preview_form', $report_type, $report_id, $info);
  unset($form['filters']); // We don't need filters on this view
  $html .= drupal_render($form);

	return $html;
}

function sirius_edls_page_worker_sts_form($form, &$form_state, $employer_node, $last_run_parameters) {
  $form['#tree'] = TRUE;
  $form_state['employer_nid'] = $employer_node->nid;

  $ts = $last_run_parameters['meta']['ts']['completed'];
  $msg = '<p>';
  if ($ts) {
  	$msg = 'The report was last run on <strong>';
  	$msg .= date('m/d/Y h:i a', $ts);
		$interval = time() - $ts;
		$msg .= ' (' . format_interval($interval, 2) . ' ago)';
		$msg .= '</strong>';
	} else {
		$msg = 'The report has not been run yet.';
	}
	$msg .= '</p>';

	$form['msg'] = array('#markup' => $msg);

  $report_type = 'sirius-edls-sts';
  $info = report_cache_info($report_type);
  $generator = $info['generator'];
  report_cache_form_prepare($report_type, $form, $form_state);

  // We hide some of the report_cache paraphernalia
  unset($form['report_cache']);
  $form['report_type'] = array('#type' => 'value', '#value' => $report_type);

  // Instead of displaying the parameters form, we set them ourself.
	$form['parameters'] = $info['generator']->form();
	$form['parameters']['employer']['#type'] = 'value';
	$form['parameters']['employer']['#value'] = $employer_node->nid;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Run the Report',
  );

  return $form;
}

function sirius_edls_page_worker_sts_form_submit($form, &$form_state) {
	$form_state['values']['redirect'] = array(
		'path' => current_path(),
		'query' => array(
			'employer_nid' => $_GET['employer_nid'],
		),
	);
	$form_state['values']['success_msg'] = 'The report has been generated.';
  return report_cache_form_submit($form, $form_state);
}

