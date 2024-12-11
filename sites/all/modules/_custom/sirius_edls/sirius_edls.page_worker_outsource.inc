<?php

function sirius_edls_page_worker_outsource($ms_key = NULL) {
	$result = sirius_edls_employer_switch();
	$html = $result['markup'];
	$employer_node = $result['employer_node'];
	if (!$employer_node) {
		$html .= sirius_page_error('Please select an employer.');
		return $html;
	}

	if (!sirius_access_instance('edls')->access('employer_config_outsource_workers', array('employer_nid' => $employer_node->nid))) {
		return sirius_page_error('Access denied.');
	}

	$html .= drupal_render(drupal_get_form('sirius_edls_page_worker_outsource_form', $employer_node, $ms_key));

	return $html;
}

function sirius_edls_page_worker_outsource_quicknav($employer_node) {
	$ms_options = sirius_edls_outsource_ms_list($employer_node);

	$base_path = 'sirius/edls/workers/outsource';

	$links = array();
	foreach ($ms_options as $ms_key => $ms_name) {
		$link = array();
		$link['title'] = $ms_name;
		$link['path'] = $base_path . '/' . $ms_key;
		$links[] = $link;
	}

	return sirius_quicknav($links);
}

function sirius_edls_page_worker_outsource_form($form, &$form_state, $employer_node, $ms_key = NULL) {
	$form['#tree'] = TRUE;
	$form_state['employer_nid'] = $employer_node->nid;
	$form_state['ms_key'] = $ms_key;

	$form['quicknav'] = array('#markup' => sirius_edls_page_worker_outsource_quicknav($employer_node));

	$employer_json = sirius_json_get($employer_node);
	if ($ms_key && ($employer_json['edls']['client']['active'] == 'Yes')) {
		if (is_array($employer_json['edls']['client']['active_ms_list'])) {
			if (in_array($ms_key, array_filter(array_values($employer_json['edls']['client']['active_ms_list'])))) {
				$form['warning'] = array(
					'#markup' => '<p><strong>' . t('WARNING: This member status has been set to sync automatically. Any changes made to this list will be overwritten at the next sync.') . '</strong></p>'
				);
			}
		}
	}

	$ms_options = sirius_edls_outsource_ms_list($employer_node);
	$ms_keys = array_keys($ms_options);
	if (!$ms_keys) { return sirius_form_error("Please configure at least one outsource member status.", 'error'); }

	if (!$ms_key) { 
		$form['err'] = array('#markup' => '<p><strong>' . t('Please select a member status from the menu above.') . '</strong></p>');
		return $form;
	}

	$html = '<p>';
	$html .= t('Enter the outsource workers one per line. Each line should include the ID, the member status, and the name, separated by a pipe, i.e.:');
	$html .= '<pre>';
	$html .= "12343|Flintstone, Wilma\n";
	$html .= "12344|Rubble, Betty\n";
	$html .= "12345|Flintstone, Frederick\n";
	$html .= "12346|Rubble, Barey";
	$html .= '</pre>';
	$html .= '</p>';
	$form['intro'] = array('#markup' => $html);

	$workers = sirius_edls_outsource_list($employer_node->nid, NULL, array('ms' => $ms_key));
	$workers_encode = sirius_edls_outsource_encode($workers);
	$form['workers'] = array(
		'#type' => 'textarea',
		'#rows' => 20,
		'#title' => t('Outsource Workers'),
		'#description' => t(""),
		'#default_value' => $workers_encode,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_edls_page_worker_outsource_form_submit($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);
	$ms_key = $form_state['ms_key'];

	$workers_encode = $form_state['values']['workers'];
	$result = sirius_edls_outsource_decode($workers_encode);
	if (!$result['success']) {
		drupal_set_message("Error: " . $result['msg']);
		$form_state['rebuild'] = TRUE;
		return;
	}

	$workers = $result['workers'];

	$result = sirius_edls_outsource_set_all_by_ms($employer_node, $workers, $ms_key);
	if (!$result['success']) {
		drupal_set_message("Error: " . $result['msg']);
		$form_state['rebuild'] = TRUE;
		return;
	}

	drupal_set_message("Outsource workers set.");
}