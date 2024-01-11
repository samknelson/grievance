<?php

function report_cache_render($report_type, $report_id, $info, $callback_name) {
	return drupal_get_form('report_cache_render_form', $report_type, $report_id, $info, $callback_name);
}

function report_cache_render_form($form, &$form_state, $report_type, $report_id, $info, $callback_name) {
	$form['#tree'] = TRUE;

	$callback_record = $info['render_callbacks'][$callback_name];
	if (!$callback_record) { 
		drupal_set_message(t('Unable to find callback named "@name"', array('@name' => $callback_name)), 'error');
		return NULL;
	}

	$csv = $callback_record['csv'];
	$render_callback = $callback_record['method'];

	$parameters = report_cache_parameters($report_type, $report_id);

	if ($csv) {
		if ($_REQUEST['output'] == 'csv') {
			$filter_values = $_REQUEST['filters'];

			$data = $info['generator']->callback_wrapper_render($render_callback, $report_id, 'csv', $filter_values);

	    $filename = "${report_type}_";
	    $r = $report_id;
	    $r = preg_replace('/[^a-zA-Z0-9_ ]/', '', $r);
	    $r = preg_replace('/ +/', '_', $r);
	    $filename .= $r;
	    $filename .= ".csv";

	    drupal_add_http_header('Content-Type', 'text/csv');
	    drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
	    $fp = fopen('php://output', 'w');
	    fputcsv($fp, $data['header']);
	    foreach ($data['rows'] as $row) {
	    	if ($row['data']) { $row = $row['data']; }
	      foreach ($row as $delta => $val) { $row[$delta] = strip_tags($val); }
	      fputcsv($fp, $row);
	    }
	    drupal_exit();
		}

		$csv_link = '<div class="report_cache_csv_link">';
		$tmp = $_REQUEST;
		$tmp['output'] = 'csv';
		$tmp['filters'] = $form_state['values']['filters'];
		$csv_link .= l('Download', current_path(), array('query' => $tmp, 'attributes' => array('class' => array('tool-button'))));
		$csv_link .= '</div>';
	}

	$parameters_render = $info['generator']->render_parameters($parameters);

	if ($parameters_render) {
		$parameters_html = '<div class="report_cache_parameters">';
		$parameters_html .= '<h2>' . t('Parameters') . '</h2>';
		$parameters_html .= $parameters_render;
		
		$parameters_html .= sirius_fakerender_field('Instance:', $report_id);
		if ($callback_name) {
			$parameters_html .= sirius_fakerender_field('Display:', $callback_name);
		}
	  $parameters_html .= '</div>';
	  $form['parameters_html'] = array('#markup' => $parameters_html);
	}

	$result = $info['generator']->callback_wrapper_render($render_callback, $report_id, 'page', $filter_values);

	if (is_array($result) && $result['filters']) {
		$form['filters'] = array(
			'#type' => 'fieldset',
			'#title' => t('Filters'),
			'#collapsible' => TRUE,
			'#collapsed' => FALSE,
		);

		$form['filters'] += $result['filters'];

		$form['filters']['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);
	}

	$form['csv_link_top'] = array('#markup' => $csv_link);

	$body_html = '<div class="report_cache_render_body">';

	if (is_array($result)) {
		if ($result['header']) {
			$result['attributes']['class'][] = 'report_cache_table';
			$body_html .= sirius_table_render($result['header'], $result['rows'], $result['attributes']);
		}
	} else {
		$body_html .= $result;
	}

	$body_html .= '</div>';
	$form['body'] = array('#markup' => $body_html);

	$form['csv_link_bottom'] = array('#markup' => $csv_link);

	$form['meta'] = array('#markup' => report_cache_render_meta($parameters));

	return $form;
}

function report_cache_render_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}
