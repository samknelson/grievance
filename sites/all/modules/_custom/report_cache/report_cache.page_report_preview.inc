<?php

function report_cache_preview($report_type, $report_id, $info) {
	return drupal_get_form('report_cache_preview_form', $report_type, $report_id, $info);
}

function report_cache_preview_form($form, &$form_state, $report_type, $report_id, $info) {
	$form_state['report']['report_type'] = $report_type;
	$form_state['report']['report_id'] = $report_id;
	$form_state['report']['info'] = $info;

	$form['#tree'] = TRUE;

	$domain_nid = sirius_domain_nid(); if (!$domain_nid) { $domain_nid = 0; }
	$fields = $info['fields'];

	$parameters = report_cache_parameters($report_type, $report_id);

	// @todo
	/*
	if ($info['title_callback']) {
		$html .= $info['title_callback']($report_type, $report_id);
	} else {
		$html .= "$report_id Preview";
	}
	*/

	if ($info['generator']) {
		$parameters_render = $info['generator']->render_parameters($parameters);
	} else {
		// Legacy support
		$render_parameters_callback = $info['render_parameters_callback'];
		if ($render_parameters_callback) {
			$parameters_render = $render_parameters_callback($parameters);
		}
	}

	if ($parameters_render) {
		$parameters_html = '<div class="report_cache_parameters">';
		$parameters_html .= '<h2>' . t('Parameters') . '</h2>';
		$parameters_html .= $parameters_render;
		$parameters_html .= sirius_fakerender_field('Instance:', $report_id);
	  $parameters_html .= '</div>';
	  $form['parameters_html'] = array('#markup' => $parameters_html);
	}

	$default_display_fields = $info['preview']['default_display_fields'];
	if (!$default_display_fields) { $default_display_fields = array_keys($fields); }

	$form['filters'] = array(
		'#type' => 'fieldset',
		'#title' => t('Filters'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	if ($form_state['submitted']) {
		$display_fields = array_filter(array_values($form_state['values']['filters']['display_fields']));
	} else {
		$display_fields = $default_display_fields;
	}

	$form['filters']['display_fields'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Display Fields'),
		'#options' => $fields,
		'#checkall' => TRUE,
		'#default_value' => $default_display_fields,
	);

	$form['filters']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	$header = array();
	foreach ($display_fields as $field_name) {
		$header[] = $fields[$field_name];
	}
	if ($info['generator'] && method_exists($info['generator'], 'preview_links')) { $header[] = t('Tools'); }

	$query = db_select('report_cache');
	$query->fields('report_cache');
	$query->condition('report_id', $report_id);
	$query->condition('report_type', $report_type);
	$query->condition('report_domain_nid', $domain_nid);
	$query = $query->extend('PagerDefault');
	$query->limit(100);
	$stmt = $query->execute();

	$rows = array();
	$count = 0;
	
	while ($hr = $stmt->fetchAssoc()) {
		$hr = unserialize($hr['report_data']);
		$row = array();
		foreach ($display_fields as $name) {
			$row[] = $hr[$name];
		}

		if ($info['generator'] && method_exists($info['generator'], 'preview_links')) {
			$links = $info['generator']->preview_links($hr);
			$links_html = '';
			foreach ($links as $path => $name) {
				if (!drupal_valid_path($path)) { continue; }
				$has_links = TRUE;
				$links_html .= l($name, $path, array('attributes' => array('class' => array('tool-button'))));
			}
			$row[] = $links_html;
		}
		$rows[] = $row;
		++$count;
	}

	$body_html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
	$body_html .= theme('pager', array('tags' => array()));

	$form['body'] = array('#markup' => $body_html);

	$form['meta'] = array('#markup' => report_cache_render_meta($parameters));

	return $form;
}

function report_cache_preview_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}