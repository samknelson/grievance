<?php

function sirius_smf_node_tab_automsg_config_access($cron_node) {
	// @todo: Hardcoding MLK, because for now we only know of one automsg
	if ($cron_node->field_sirius_type['und'][0]['value'] != 'cron:scan:smf_automsg_mlk') { return FALSE; }

	return TRUE;
}

function sirius_smf_node_tab_automsg_config_form($form, &$form_state, $cron_node) {
  drupal_add_css(drupal_get_path('module', 'sirius') . '/css/table.css');

	$cron_key = 'smf_automsg_mlk';

	$form_state['cron_nid'] = $cron_node->nid;
	$form_state['cron_key'] = $cron_key;
	$form['#tree'] = TRUE;

	$cron_info = sirius_cron_scan_info($cron_key);
	$instance = $cron_info['instance'];

	$json = sirius_json_get($cron_node);

	$domain_nid = sirius_domain_nid();
	$automsg_json = $json['smf'][$domain_nid]['automsg'];

	$msg_info_all = $instance->msg_info_all();

	$form['msgs']['open'] = array(
		'#markup' => '<table class="sirius_table">' .
			'<tr>' . 
			'<th>' . t('Key') . '</th>' .
			'<th>' . t('Prior Key') . '</th>' .
			'<th>' . t('Title') . '</th>' .
			'<th>' . t('Description') . '</th>' .
			'<th>' . t('Body') . '</th>' .
			'<th>' . t('Enable?') . '</th>' .
			'</tr>'
	);

	foreach ($msg_info_all as $key => $info) {
		if ($automsg_json['msgs'][$key]['enabled'] == 'Yes') { $class = 'green'; } else { $class = 'gray'; }

		$form['msgs'][$key]['open'] = array('#markup' => "<tr class=\"$class\">");

		$form['msgs'][$key]['key'] = array(
			'#markup' => '<td>' . $key . '</td>',
		);

		$form['msgs'][$key]['prior'] = array(
			'#markup' => '<td>' . $info['prior'] . '</td>',
		);

		$form['msgs'][$key]['title'] = array(
			'#markup' => '<td>' . $info['title'] . '</td>',
		);

		$form['msgs'][$key]['description'] = array(
			'#markup' => '<td>' . $info['description'] . '</td>',
		);

		$body = '';
		if ($info['sms_attach']) {
			$body .= "<img src=\"$info[sms_attach][url]\" width=\"300\"><br>";
		}
		$body .= sirius_text_to_html($info['msg']['body']);
		$form['msgs'][$key]['body'] = array(
			'#markup' => '<td>' . $body . '</td>',
		);

		$form['msgs'][$key]['enabled'] = array(
			'#type' => 'select',
			'#options' => array(
				NULL => t('-- Select --'),
				'No' => t('No'),
				'Yes' => t('Yes'),
			),
			'#default_value' => $automsg_json['msgs'][$key]['enabled'],
			'#prefix' => '<td>',
			'#suffix' => '</td>',
		);

		$form['msgs'][$key]['close'] = array('#markup' => '</tr>');
	}

	$form['msgs']['close'] = array(
		'#markup' => '</table>'
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_smf_node_tab_automsg_config_form_submit($form, &$form_state) {
	$cron_key = $form_state['cron_key'];
	$cron_info = sirius_cron_scan_info($cron_key);
	$instance = $cron_info['instance'];
	$cron_node = node_load($form_state['cron_nid']);

	$json = sirius_jsonfield_util_node_json_get($domain_node);
	$domain_nid = sirius_domain_nid();
	$json['smf'][$domain_nid]['automsg']['msgs'] = $form_state['values']['msgs'];

	sirius_jsonfield_util_node_json_set($cron_node, $json);
	node_save($cron_node);
	drupal_set_message("Settings saved.");
}
