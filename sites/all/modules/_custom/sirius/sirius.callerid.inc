<?php

function sirius_page_callerid() {
	return drupal_get_form('sirius_page_callerid_form');
}

function sirius_page_callerid_form($form, &$form_state) {
	$domain_nid = sirius_domain_nid();

	$active_options = $inactive_options = $all_options = array(NULL => t('-- Select --'));

  $callerid_nodes = sirius_callerid_list();
  if ($callerid_nodes) {
  	$is_admin = user_access('sirius admin');
	  $html = '<h2>' . t('Current Caller IDs') . '</h2>';

  	$header = array(
  		t('Name'),
  		t('Phone Number'),
  		t('Display Name'),
  		t('Active?'),
  	);
  	if ($is_admin) { $header[] = t('Tools'); }
  	$rows = array();


  	foreach ($callerid_nodes as $callerid_nid => $callerid_node) {
  		$row = array();
  		$row[] = $callerid_node->title;
  		$row[] = $callerid_node->field_sirius_phone['und'][0]['value'];
  		$row[] = $callerid_node->field_sirius_name_display['und'][0]['value'];
  		$row[] = $callerid_node->field_sirius_active['und'][0]['value'];
  		if ($is_admin) {
  			$row[] = l(t('Edit'), "node/$callerid_nid/edit", array('query' => array('destination' => current_path()), 'attributes' => array('class' => array('tool-button'))));
  		}
  		$rows[] = $row;

  		$all_options[$callerid_nid] = $callerid_node->title;
  		if ($callerid_node->field_sirius_active['und'][0]['value'] == 'Yes') { 
  			$active_options[$callerid_nid] = $callerid_node->title;
  		} else { 
  			$inactive_options[$callerid_nid] = $callerid_node->title;
  		}
  	}
  	$html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
  	$form['intro'] = array('#markup' => $html);
  }

  $form['action'] = array(
  	'#type' => 'select',
  	'#title' => t('Action'),
  	'#options' => array(
  		'refresh' => t('Refresh this list'),
  		'create' => t('Create a new entry'),
  		'edit' => t('Edit an existing entry'),
  		'revalidate' => t('Re-send validation code'),
  		'delete' => t('Delete an entry'),
  		'move' => t('Move an entry'),
  	),
  );

  $form['create_title'] = array(
  	'#type' => 'textfield',
  	'#title' => t('Title'),
    '#states' => array(
      'visible' => array(
        array(':input[name="action"]' => array('value' => 'create')),
      )
    ),
  );

  $form['create_phone'] = array(
  	'#type' => 'textfield',
  	'#title' => t('Phone Number'),
    '#states' => array(
      'visible' => array(
        array(':input[name="action"]' => array('value' => 'create')),
      )
    ),
  );

  $form['create_name_display'] = array(
  	'#type' => 'textfield',
  	'#title' => t('Display Name'),
    '#states' => array(
      'visible' => array(
        array(':input[name="action"]' => array('value' => 'create')),
      )
    ),
  );

  $form['revalidate_nid'] = array(
  	'#type' => 'select',
  	'#title' => t('Revalidate'),
  	'#options' => $inactive_options,
    '#states' => array(
      'visible' => array(
        array(':input[name="action"]' => array('value' => 'revalidate')),
      )
    ),
  );

  $form['edit_nid'] = array(
  	'#type' => 'select',
  	'#title' => t('Edit'),
  	'#options' => $active_options,
    '#states' => array(
      'visible' => array(
        array(':input[name="action"]' => array('value' => 'edit')),
      )
    ),
  );

  $form['edit_title'] = array(
  	'#type' => 'textfield',
  	'#title' => t('Title'),
    '#states' => array(
      'visible' => array(
        array(':input[name="action"]' => array('value' => 'edit')),
      )
    ),
  );

  $form['edit_name_display'] = array(
  	'#type' => 'textfield',
  	'#title' => t('Display Name'),
    '#states' => array(
      'visible' => array(
        array(':input[name="action"]' => array('value' => 'edit')),
      )
    ),
  );

  $form['delete_nid'] = array(
  	'#type' => 'select',
  	'#title' => t('Delete'),
  	'#options' => $all_options,
    '#states' => array(
      'visible' => array(
        array(':input[name="action"]' => array('value' => 'delete')),
      )
    ),
  );

  $form['move_nid'] = array(
  	'#type' => 'select',
  	'#title' => t('Move'),
  	'#options' => $all_options,
    '#states' => array(
      'visible' => array(
        array(':input[name="action"]' => array('value' => 'move')),
      )
    ),
  );

  $domain_root = sirius_domain_root();
  $domain_options = sirius_domain_tree_render_as_select($domain_root);
  $form['move_domain_nid'] = array(
  	'#type' => 'select',
  	'#title' => t('Move To'),
  	'#options' => $domain_options,
    '#states' => array(
      'visible' => array(
        array(':input[name="action"]' => array('value' => 'move')),
      )
    ),
  );

  $form['submit'] = array(
  	'#type' => 'submit',
  	'#value' => t('Submit'),
  );
  return $form;
}

function sirius_page_callerid_form_submit($form, &$form_state) {
	$domain_nid = sirius_domain_nid();

	$action = $form_state['values']['action'];

	//
	// Refresh
	//

	if ($action == 'refresh') {
		$callerid_nodes = sirius_callerid_list();
		foreach ($callerid_nodes as $callerid_nid => $callerid_node) {
			$phone = $callerid_node->field_sirius_phone['und'][0]['value'];
			drupal_set_message(t('Scanning: @phone', array('@phone' => $phone)));
			$result = sirius_twilio_callerid_fetch_by_phone($callerid_node->field_sirius_phone['und'][0]['value']);

			$need_save = FALSE;

			if ($callerid_node->field_sirius_active['und'][0]['value'] != 'Yes' && $result['success']) {
				drupal_set_message(t('The caller ID has been validated, and may now be used: ') . $phone);
				$callerid_node->field_sirius_active['und'][0]['value'] = 'Yes';
				$need_save = TRUE;
			}

			if ($callerid_node->field_sirius_active['und'][0]['value'] == 'Yes' && !$result['success']) {
				$callerid_node->field_sirius_active['und'][0]['value'] = 'No';
				$need_save = TRUE;
				drupal_set_message(t('The caller ID is no longer valid, and may not be used: ') . $phone);
			}

			if ($result['success'] && ($result['name_display'] != $callerid_node->field_sirius_name_display['und'][0]['value'])) {
				$callerid_node->field_sirius_name_display['und'][0]['value'] = $result['name_display'];
				$need_save = TRUE;
				drupal_set_message(t('Reset the display name for: ') . $phone);
			}

			if ($need_save) { node_save($callerid_node); }
		}
	}

	//
	// Create
	//
	
	if ($action == 'create') {
		$name_display = $form_state['values']['create_name_display'];
		$phone = sirius_phone_format_e164($form_state['values']['create_phone']);
		$title = $form_state['values']['create_title'];

		// First, make sure it doesn't already exist in Sirius
		$query = new EntityFieldQuery();
	  $query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'sirius_callerid');
	  $query->fieldCondition('field_sirius_phone', 'value', $phone);
	  $stmt = $query->execute();
	  if ($stmt['node']) {
	  	drupal_set_message(t('The phone number @phone already has an entry in our system.', array('@phone' => $phone)), 'error');
	  	return;
	  }

	  // Next, find out if it exists in Twilio. If not, send a verification.
		$result = sirius_twilio_callerid_fetch_by_phone($phone);
		if ($result['success']) {
			drupal_set_message(t('The existing Twilio number has been added to our system.'));
			$active = 'Yes';
		} else {
			$result = sirius_twilio_callerid_verify($phone, $name_display);
			if (!$result['success']) {
				drupal_set_message(t('Request failed: ') . $result['msg'], 'error');
				return;
			}
			drupal_set_message(t('Verification request has been sent; please enter the code <strong>@code</strong>', array('@code' => $result['validation_code'])));
			$active = 'No';
		}

		// Save the node.
		global $user;
	  $callerid_node = new stdClass();
	  $callerid_node->type = 'sirius_callerid';
	  $callerid_node->title = $title;
	  $callerid_node->no_recurse = TRUE;
	  $callerid_node->language = LANGUAGE_NONE;
	  $callerid_node->uid = $user->uid;
	  $callerid_node->status = 1;
	  $callerid_node->promote = 0;
	  $callerid_node->comment = 0;
	  $callerid_node->field_sirius_phone['und'][0]['value'] = $phone;
	  $callerid_node->field_sirius_active['und'][0]['value'] = $active;
	  $callerid_node->field_sirius_name_display['und'][0]['value'] = $name_display;
	  if ($domain_nid) { $callerid_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
	  $callerid_node = node_submit($callerid_node);
	  node_object_prepare($callerid_node);
	  node_save($callerid_node);
	}

	//
	// Revalidate
	//
	
	if ($action == 'revalidate') {
		$callerid_nid = $form_state['values']['revalidate_nid'];
		if (!$callerid_nid) { 
			drupal_set_message(t('Please select which number to revalidate.'), 'error'); 
			return;
		}
		$callerid_node = node_load($callerid_nid);
		$result = sirius_twilio_callerid_verify($callerid_node->field_sirius_phone['und'][0]['value'], $callerid_node->field_sirius_name_display['und'][0]['value']);
		if (!$result['success']) {
			drupal_set_message(t('Request failed: ') . $result['msg'], 'error');
			return;
		}
		drupal_set_message(t('Verification request has been sent; please enter the code <strong>@code</strong>', array('@code' => $result['validation_code'])));
		$active = 'No';
	}

	//
	// Edit
	//

	if ($action == 'edit') {
		$callerid_nid = $form_state['values']['edit_nid'];
		if (!$callerid_nid) { 
			drupal_set_message(t('Please select which number to edit.'), 'error'); 
			return;
		}
		$callerid_node = node_load($callerid_nid);
		$phone = $callerid_node->field_sirius_phone['und'][0]['value'];
		$name_display = $form_state['values']['edit_name_display'];
		$title = $form_state['values']['edit_title'];
		$result = sirius_twilio_callerid_update($phone, $name_display);
		if (!$result['success']) {
			drupal_set_message(t('Request failed: ') . $result['msg'], 'error');
			return;
		}

		$callerid_node->field_sirius_name_display['und'][0]['value'] = $name_display;
		$callerid_node->field_sirius_title['und'][0]['value'] = $title;
		node_save($callerid_node);
	}

	//
	// Delete
	//

	if ($action == 'delete') {
		$callerid_nid = $form_state['values']['delete_nid'];
		if (!$callerid_nid) { 
			drupal_set_message(t('Please select which number to delete.'), 'error'); 
			return;
		}
		$callerid_node = node_load($callerid_nid);
		$phone = $callerid_node->field_sirius_phone['und'][0]['value'];
		$result = sirius_twilio_callerid_fetch_by_phone($phone);
		if ($result['success']) {
			$result = sirius_twilio_callerid_delete($phone);
			if (!$result['success']) {
				drupal_set_message(t('Request failed: ') . $result['msg'], 'error');
				return;
			}
		}
		node_delete($callerid_nid);
		drupal_set_message("Entry deleted.");
	}

	//
	// Move
	//

	if ($action == 'move') {
		$callerid_nid = $form_state['values']['move_nid'];
		if (!$callerid_nid) { 
			drupal_set_message(t('Please select which number to move.'), 'error'); 
			return;
		}
		$callerid_node = node_load($callerid_nid);
		$phone = $callerid_node->field_sirius_phone['und'][0]['value'];
		$move_domain_nid = $form_state['values']['move_domain_nid'];
		$move_domain_node = node_load($move_domain_nid);
		$callerid_node->field_sirius_domain['und'][0]['target_id'] = $move_domain_nid;
		node_save($callerid_node);
		drupal_set_message(t('Moved the CallerId @phone to the new domain @title', array('@phone' => $phone, '@title' => $move_domain_node->title)));		
	}
}

function sirius_callerid_sync($callerid_node) {
	// @todo handle different backends
	sirius_twilio_callerid_sync($callerid_node);
}

function sirius_callerid_list() {
	$domain_nid = sirius_domain_nid();
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_callerid');
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  $query->range(0,1000);
  $stmt = $query->execute();
  if (!$stmt['node']) { return array(); }
  return node_load_multiple(array_keys($stmt['node']));
}

function sirius_callerid_options() {
	$callerid_nodes = sirius_callerid_list();
	$callerid_options = array();
	foreach ($callerid_nodes as $callerid_nid => $callerid_node) {
		if ($callerid_node->field_sirius_active['und'][0]['value'] == 'Yes') {
			$callerid_options[$callerid_nid] = $callerid_node->title . ' (' . $callerid_node->field_sirius_phone['und'][0]['value'] . ')';
		}
	}
	return $callerid_options;
}