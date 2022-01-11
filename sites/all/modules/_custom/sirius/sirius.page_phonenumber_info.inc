<?php

function sirius_node_tab_phonenumber_info($phonenumber_node) {
	$phonenumber_json = sirius_json_get($phonenumber_node);

	$html = sirius_fakerender_field('SMS Possible:',  $phonenumber_node->field_sirius_sms_possible['und'][0]['value']);
	$html .= sirius_fakerender_field('Voice Possible:',  $phonenumber_node->field_sirius_voice_possible['und'][0]['value']);
	$html .= sirius_phonenumber_render_last_sms($phonenumber_node);
	$html .= sirius_phonenumber_render_last_sms_incoming($phonenumber_node);
	$html .= sirius_phonenumber_render_scan($phonenumber_node);
	$html .= sirius_phonenumber_render_user_block($phonenumber_node);
	$html .= sirius_phonenumber_render_manual_block($phonenumber_node);

	$node_types = array(
		'sirius_worker' => 'Workers',
		'sirius_contact' => 'Contacts',
	);

	$domain_nid = $phonenumber_node->field_sirius_domain['und'][0]['target_id'];
	$phone = $phonenumber_node->field_sirius_id['und'][0]['value'];
	foreach ($node_types as $node_type => $node_type_name) {
		$sql = "select nid, title from node ";
		$sql .= "left join field_data_field_sirius_phone on field_data_field_sirius_phone.entity_type = 'node' and field_data_field_sirius_phone.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
		$sql .= "where node.type = :node_type ";
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql .= "and field_sirius_phone_value = :phone ";
		$sql .= "order by node.title ";
		$stmt = sirius_sql_query($sql, array(':domain_nid' => $domain_nid, ':node_type' => $node_type, ':phone' => $phone));

		$rows = array();
		while ($hr = $stmt->fetchAssoc()) {
			$row = array();
			$row[] = $hr['title'];
			$row[] = l('View', 'node/' . $hr['nid'], array('attributes' => array('class' => array('tool-button'))));
			$rows[] = $row;
		}

		if (!$rows) { continue; }
		$header = array(
			t('Record'), 
			t('Tools')
		);

		$html .= '<h2>' . $node_type_name . '</h2>';
		$html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
	}

	return $html;
}