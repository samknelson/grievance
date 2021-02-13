<?php

function sirius_office_page_operator() {
	$is_admin = user_access('sirius office operator admin');
	$operators = sirius_office_operator_list();

	if ($operators) {
		$rows = array();
		foreach ($operators as $operator_nid => $operator_node) {
			$paths = array(
				"node/$operator_nid/edit" => t('Edit'),
				"node/$operator_nid/sirius_operator_users" => t('Users'),
				"node/$operator_nid/sirius_operator_status" => t('Status'),
				"node/$operator_nid/sirius_operator_calls" => t('Incoming Calls'),
			);

			$tools = '';
			foreach ($paths as $path => $title) {
				if (!drupal_valid_path($path)) { continue; }
				if ($tools) { $tools .= ' '; }
				$tools .= l($title, $path, array('attributes' => array('class' => array('tool-button'))));
			}
			$json = json_decode($operator_node->field_sirius_json['und'][0]['value'], TRUE);
			$status = $json['status']['status'];
			$which = $json['status']['which'];
			$msg = $json['status']["msg_$which"];
			$status_msg = $status;
			if ($status == 'closed') { $status_msg .= ' -- ' . $json['status']["msg_$which"]; }

			$row = array();
			$row[] = $operator_node->title;
			$row[] = $operator_node->field_sirius_phone['und'][0]['value'];
			$row[] = $status_msg;
			$row[] = $tools; 
			$rows[] = $row;
		}

		$header = array(
			t('Operator'),
			t('Phone Number'),
			t('Status'),
			t('Tools'),
		);

		$html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
	} else {
		$html .= t('There are no operators available.');
	}

	$path = 'node/add/sirius-operator';
	if (drupal_valid_path($path)) {
		$html .= l(t('Add a New Operator'), $path, array('attributes' => array('class' => array('tool-button'))));
	}

	return $html;
}