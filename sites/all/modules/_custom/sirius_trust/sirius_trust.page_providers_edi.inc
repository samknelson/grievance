<?php

function sirius_trust_page_providers_edi() {
	sirius_require('sirius_trust', 'sirius_trust.provider_edi.root.inc');
	$providers = sirius_trust_providers();
	$edi = Sirius_Trust_Provider_Edi::getInstance();

	$rows = array();
	foreach ($providers as $provider_nid => $provider_node) {
		$plugins = $edi->list_plugins($provider_node);
		$is_first = TRUE;
		foreach ($plugins as $plugin_uuid => $plugin) {

			$tools = array();
			$tools["node/$provider_nid/sirius_trust_provider_edi_plugins/$plugin_uuid"] = t('Settings');
			$tools["node/$provider_nid/sirius_trust_provider_edi_plugins/$plugin_uuid/generate"] = t('Generate');
			$tools["node/$provider_nid/sirius_trust_provider_edi_plugins/$plugin_uuid/download"] = t('Download');
			$tools["node/$provider_nid/sirius_trust_provider_edi_plugins/$plugin_uuid/push"] = t('Push');

			$tool_html = '';
			foreach ($tools as $path => $title) {
				$tool_html .= l($title, $path, array('attributes' => array('class' => array('tool-button'))));
			}

			$push_log = $edi->last_log_entry($provider_node, $plugin_uuid, 'trust:edi:push');
			$generate_log = $edi->last_log_entry($provider_node, $plugin_uuid, 'trust:edi:generate');

			$row = array();
			if ($is_first) {
				$row[] = $provider_node->title;
				$is_first = FALSE;
			} else {
				$row[] = '';
			}
			$row[] = $plugin['title'];

			foreach (array($generate_log, $push_log) as $log_node) {
				if ($log_node) {
					if ($log_node->field_sirius_type['und'][0]['value'] == 'success') {
						$color = 'green';
						$msg = 'Success';
					} else {
						$color = 'red';
						$msg = 'Error';
					}
					$row[] = array(
						'class' => $color, 
						'data' => $msg . '<br> ' . 
							// date('m/d/Y', $log_node->created) . '<br>' . 
							format_interval(time() - $log_node->created, 2) . ' ago'
					);
					$row[] = array(
						'class' => $color,
						'data' => $log_node->field_sirius_message['und'][0]['value']
					);
				} else {
					$row[] = '';
					$row[] = '';
				}
			}

			$row[] = $tool_html;

			$rows[] = $row;
		}
	}


	$header = array();
	$header[] = t('Provider');
	$header[] = t('Feed');
	$header[] = t('Last Generate');
	$header[] = '';
	$header[] = t('Last Push');
	$header[] = '';
	$header[] = t('Tools');
	return sirius_table_render($header, $rows);
}