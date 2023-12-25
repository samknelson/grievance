<?php

function sirius_edls_page_worker_list() {
	$result = sirius_edls_employer_switch();
	$html = $result['markup'];
	$employer_node = $result['employer_node'];
	if (!$employer_node) {
		$html .= sirius_page_error('Please select an employer.');
		return $html;
	}

	$html .= drupal_render(drupal_get_form('sirius_edls_page_worker_list_form', $employer_node));
	return $html;
}

function sirius_edls_page_worker_list_form($form, &$form_state, $employer_node) {
	$employer_nid = $employer_node->nid;

	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/form.css');

	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/table.css');
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/table.js', array('type' => 'file'));

	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/ux.js', array('type' => 'file'));

	drupal_add_js(drupal_get_path('module', 'sirius_edls') . '/js/sheet_workers.js', array('type' => 'file'));
	drupal_add_css(drupal_get_path('module', 'sirius_edls') . '/css/sheet_workers.css');

	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/sirius.popup.js');
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/sirius.popup.css');

	$js_settings = array();
	$js_settings['employer_nid'] = $employer_nid;
	$js_settings['date'] = date('Y-m-d');

	$classification_options = sirius_edls_worker_classification_options();
	$js_settings['classification_options'] = $classification_options;

	// js_settings for associative arrays don't preserve key order. Ick.
	$ms_list = sirius_edls_ms_list($employer_node, 'name');
	$js_settings['ms_name_lookup'] = $ms_list;
	$js_settings['ms_keys'] = array_values($ms_list);

	drupal_add_js(array('sirius_edls_sheet_workers' => $js_settings), 'setting');

	$form['#tree'] = TRUE;

	$form['available']['filters'] = sirius_worker_form_filters(array('ms_list' => $ms_list));
	$form['available']['workers'] = array('#markup' => sirius_worker_render_list(array('ms_list' => $ms_list)));

	return $form;
}
