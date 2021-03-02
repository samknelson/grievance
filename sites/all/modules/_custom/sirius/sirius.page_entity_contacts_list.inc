<?php

function sirius_node_tab_entity_contacts_list($entity_node) {
	$entity_json = sirius_jsonfield_util_node_json_get($entity_node);
	if (!$entity_json['contacts']) { 
		return 'No contacts have been added to ' . $entity_node->title . '.';
	}

	if ($_REQUEST['remove']) {
		sirius_contact_entity_remove($entity_node, $_REQUEST['remove']);
		drupal_set_message("Contact has been removed from " . $entity_node->title);
		drupal_goto(current_path());
	}

	$contact_nodes = node_load_multiple(array_keys($entity_json['contacts']));

	$contact_types = sirius_taxonomy_load('grievance_contact_types', 'tid', 'name');

	$rows = array();
	foreach ($contact_nodes as $contact_nid => $contact_node) {
		$row = array();

		$type_str = '';
		if ($entity_json['contacts'][$contact_nid]['type_tids']) {
			foreach ($entity_json['contacts'][$contact_nid]['type_tids'] as $type_tid) {
				if ($type_str) { $type_str .= ', '; }
				$type_str .= $contact_types[$type_tid];
			}
		}

		$tools = '';
		$tools .= l('View', 'node/' . $contact_nid, array('attributes' => array('class' => array('tool-button'))));
		$tools .= l('Remove', current_path(), array('query' => array('remove' => $contact_nid), 'attributes' => array('class' => array('tool-button'))));

		$address = '';
		if ($contact_node->field_sirius_address['und'][0]['thoroughfare']) {
			$renderable = field_view_value('node', $contact_node, 'field_sirius_address', $contact_node->field_sirius_address['und'][0], 'full');
			$address = drupal_render($renderable);
		}

		$row[] = $type_str;
		$row[] = $contact_node->title;
		$row[] = $contact_node->field_sirius_phone['und'][0]['value'];
		$row[] = $contact_node->field_sirius_email['und'][0]['value'];
		$row[] = $address;
		$row[] = $tools;
		$rows[] = $row;
	}

	$header = array(
		'Type',
		'Contact',
		'Phone',
		'Email',
		'Address',
		'Tools',
	);

	return theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
}
