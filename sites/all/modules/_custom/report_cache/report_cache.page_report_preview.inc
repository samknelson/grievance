<?php

function report_cache_preview($report_type, $report_id, $info) {
	return drupal_get_form('report_cache_preview_form', $report_type, $report_id, $info);
}

function report_cache_preview_form($form, &$form_state, $report_type, $report_id, $info) {
	$form_state['report']['report_type'] = $report_type;
	$form_state['report']['report_id'] = $report_id;
	$form_state['report']['info'] = $info;

	$form['#tree'] = TRUE;

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

  $rows_per_page = $info['generator']->preview_rows_per_page($form, $form_state, $report_id, $info);
  $total_count = $info['generator']->preview_query($form, $form_state, $report_id, $info, TRUE);
  $pages = floor($total_count / $rows_per_page) + 1;

	$parameters_render = $info['generator']->render_parameters($parameters);
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

	if ($pages > 1) {
		$page_options = array();
		for ($i=0; $i<$pages; ++$i) {
			$page_options[$i] = ($i+1);
		}

		$form['filters']['page'] = array(
			'#type' => 'select',
			'#title' => t('Page'),
			'#options' => $page_options,
		);
	}

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
	if ($info['preview']['preview_links']) { $header[] = t('Tools'); }

	$stmt = $info['generator']->preview_query($form, $form_state, $report_id, $info);
	$rows = array();
	$count = 0;
	
	while ($hr = $stmt->fetchAssoc()) {
		$hr = $info['generator']->row_decode($hr['report_data']);
		$row = array();
		foreach ($display_fields as $name) {
			$row[] = $hr[$name];
		}

		if ($info['preview']['preview_links']) {
			$links = $info['generator']->preview_links($hr);
			$links_html = '';
			foreach ($links as $path => $link) {
				// Legacy supoprt
				if (!is_array($link)) { $link = array('name' => $link); }

				if ($link['type'] == 'modal') {
					if (!$link['name']) {
						$link['name'] = '&neArr;&nbsp;Details';
					}
					$path = current_path() . '/modal';;
					$query = array();
					$query['report_id'] = $report_id;
					$query['action'] = 'preview_modal';
					$query['data'] = $hr;

					$offset = $count;
					if ($_GET['page']) { $offset += $rows_per_page * $form_state['values']['page']; }
					$query['offset'] = $offset;

					$links_html .= l(
						$link['name'], 
						$path, 
						array(
							'html' => TRUE,
							'attributes' => array('class' => array('tool-button')),
							'query' => $query,
						)
					);
				} else {
					if (!drupal_valid_path($path)) { continue; }
					$links_html .= l($link['name'], $path, array('attributes' => array('class' => array('tool-button'))));
				}
			}
			$row[] = $links_html;
		}
		$rows[] = $row;
		++$count;
	}

	$body_html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
	$page = $form_state['values']['filters']['page'];
	if (!$page) { $page = 0; }
	$first_record = $rows_per_page * $page + 1;
	$last_record = $first_record + $count - 1;
	$body_html .= '<p>';
	$body_html .= "Showing records <strong>$first_record</strong> through <strong>$last_record</strong> of <strong>$total_count</strong> ";
	$body_html .= '<p>';

	$form['body'] = array('#markup' => $body_html);

	$form['meta'] = array('#markup' => report_cache_render_meta($parameters));

	$info['generator']->preview_form_alter($form, $form_state, $report_id, $info);

	return $form;
}

function report_cache_preview_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}