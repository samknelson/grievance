<?php

function sirius_node_tab_entity_contacts_add_new($entity_node) {
	return drupal_get_form('sirius_node_tab_entity_contacts_add_new_form', $entity_node);
}

function sirius_node_tab_entity_contacts_add_new_form($form, &$form_state, $entity_node) {
	$form_state['entity_nid'] = $entity_node->nid;

	$form['#tree'] = TRUE;

  $contact_types = sirius_taxonomy_load('grievance_contact_types', 'tid', 'name');
  if ($contact_types) {
	  $form['type'] = array(
	  	'#type' => 'checkboxes',
	  	'#title' => 'Contact Type',
	  	'#options' => $contact_types,
	  );
	}

  $form['name_first'] = array(
    '#type' => 'textfield',
    '#title' => 'First Name',
  );

  $form['name_last'] = array(
    '#type' => 'textfield',
    '#title' => 'Last Name',
  );

  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => 'Email Address',
  );

  $form['phone'] = array(
    '#type' => 'textfield',
    '#title' => 'Phone',
  );

  $form['address'] = array(
    '#type' => 'textfield',
    '#title' => 'Address',
  );

  $form['address2'] = array(
    '#type' => 'textfield',
    '#title' => 'Address 2',
  );

  $form['city'] = array(
    '#type' => 'textfield',
    '#title' => 'City',
  );

  $form['state'] = array(
    '#type' => 'textfield',
    '#title' => 'State',
    '#size' => 2,
  );

  $form['zip'] = array(
    '#type' => 'textfield',
    '#title' => 'Zip',
    '#size' => 5,
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_entity_contacts_add_new_form_submit($form, &$form_state) {
  $entity_node = node_load($form_state['entity_nid']);

  $contact_node = new stdClass();
  $contact_node->type = 'sirius_contact';
  $contact_node->no_recurse = TRUE;
  $contact_node->language = $worker_node->language;
  $contact_node->uid = $worker_node->uid;
  $contact_node->status = 1;
  $contact_node->promote = 0;
  $contact_node->comment = 0;
  $contact_node->title = $worker_node->title;
  $need_save = TRUE;

  $domain_nid = sirius_domain_nid();
  if ($domain_nid) { $contact_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }

  $contact_node->field_sirius_name['und'][0]['given'] = $form_state['values']['name_first'];
  $contact_node->field_sirius_name['und'][0]['family'] = $form_state['values']['name_last'];
  $contact_node->field_sirius_phone['und'][0]['value'] = sirius_phone_format($form_state['values']['phone']);
  $contact_node->field_sirius_email['und'][0]['value'] = $form_state['values']['email'];
  $contact_node->field_sirius_address['und'][0]['thoroughfare'] = $form_state['values']['address'];
  $contact_node->field_sirius_address['und'][0]['premise'] = $form_state['values']['premise'];
  $contact_node->field_sirius_address['und'][0]['locality'] = $form_state['values']['city'];
  $contact_node->field_sirius_address['und'][0]['administrative_area'] = $form_state['values']['state'];
  $contact_node->field_sirius_address['und'][0]['postal_code'] = $form_state['values']['zip'];
  $contact_node->field_sirius_address['und'][0]['country'] = 'US';

  node_object_prepare($contact_node);
  $contact_node = node_submit($contact_node);
  node_save($contact_node);

  $type_tids = array();
  if ($form_state['values']['type']) {
    $type_tids = array_filter(array_values($form_state['values']['type']));
  }
  sirius_contact_entity_add($entity_node, $contact_node->nid, $type_tids);
  drupal_set_message("Contact added.");
}
