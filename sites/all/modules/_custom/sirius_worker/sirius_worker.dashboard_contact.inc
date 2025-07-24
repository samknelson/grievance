<?php

function sirius_worker_dashboard_contact_data($prefs, $options = array()) {
	$rows = array();

	$worker_node = sirius_worker_get();
	if (!$worker_node) { return array(); }

	$details = '';
	$details .= '<div>' . $worker_node->title . '</div>';

	if ($worker_node->field_sirius_address['und'][0]['thoroughfare']) {
		$details .= sirius_addr_format_html($worker_node->field_sirius_address['und'][0]);
	}

  $contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);
  $accuracy = $contact_node->field_sirius_address_accuracy['und'][0]['value'];
  if ($accuracy && ($accuracy != 'ROOFTOP')) {
  	$details .= '<div><em>(Address not geocoded)</em></div>';
  }


	if ($worker_node->field_sirius_phone['und'][0]['value']) {
		$details .= '<div>' . $worker_node->field_sirius_phone['und'][0]['value'] . '</div>';
	}

	$email = $worker_node->field_sirius_email['und'][0]['value'];
	$dummy_domain = sirius_domain_variable_get('sirius_worker_dummy_domain', '');
	if ($email && !($dummy_domain && preg_match("/$dummy_domain/", $email))) {
		$details .= "<div>$email</div>";
	}

	$tools = l('Update', 'node/' . $worker_node->nid . '/sirius_worker_contact', array('attributes' => array('class' => array('tool-button'))));

	$record = array(
		'count' => $total,
		'tools' => $tools,
		'details' => $details,
	);

	return array($record);
}
