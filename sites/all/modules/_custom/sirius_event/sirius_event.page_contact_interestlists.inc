<?php

function sirius_event_node_tab_contact_interestlists_access($contact_node) {
	if (!sirius_event_interestlist_enabled()) { return FALSE; }
	return TRUE;
}

function sirius_event_node_tab_contact_interestlists($contact_node) {
	return drupal_get_form('sirius_event_node_tab_contact_interestlists_form', $contact_node);
}

function sirius_event_node_tab_contact_interestlists_form($form, &$form_state, $contact_node) {
	$form_state['contact_nid'] = $contact_node->nid;
	$form['#tree'] = TRUE;

	$event_type_terms = sirius_taxonomy_load('sirius_event_type', 'tid', 'full');
	if (!$event_type_terms) { return 'There are no event types.'; }
	$contact_nid = $contact_node->nid;

	$rows = array();
	$default = array();
	foreach ($event_type_terms as $event_type_tid => $event_type_term) {
		$enabled = sirius_event_interestlist_enabled_byterm($event_type_term);
		if (!$enabled) { continue; }
		$my_signup = sirius_event_interestlist_lookup($event_type_term, $contact_nid);

		$tools = '';
		$paths = array();
		$paths["taxonomy/term/$event_type_tid/sirius_event_type_interestlist/list"] = 'List';
		foreach ($paths as $path => $title) {
			if (drupal_valid_path($path)) {
				$tools .= l($title, $path, ['attributes' => ['class' => ['tool-button']]]);
			}
		}

		if ($my_signup) {
			$datestr = date('Y-m-d h:i:s a', $my_signup->created);
			$default[$event_type_tid] = $event_type_tid;
		} else {
			$datestr = '';
		}

		$row = array();
		$row[] = $event_type_term->name;
		$row[] = $datestr;
		$row[] = $tools;
		$rows[$event_type_tid] = $row;
	}

	$header = array(
		t('Name'),
		t('Date'),
		t('Tools'),
	);

	$form_state['default'] = $default;

  $form['event_type_tids'] = array(
    '#type' => 'tableselect',
    '#title' => 'Interest Lists',
    '#header' => $header,
    '#options' => $rows,
    '#default_value' => $default,
  );

  if (!$rows) {
  	drupal_set_message('There are no enabled interest lists.', 'warning');
  	return $form;
  }

  $form['submit'] = array(
  	'#type' => 'submit',
  	'#value' => t('Submit'),
  );

  return $form;
}

function sirius_event_node_tab_contact_interestlists_form_submit($form, &$form_state) {
	$contact_nid = $form_state['contact_nid'];
	$default = $form_state['default'];

	$event_type_tids = $form_state['values']['event_type_tids'];
	foreach ($event_type_tids as $event_type_tid => $enabled) {
		if ($enabled == $default[$event_type_tid]) { continue; }
		
		$event_type_term = taxonomy_term_load($event_type_tid);
		if ($enabled) { 
			$result = sirius_event_interestlist_signup($event_type_term, $contact_nid, array());
		} else {
			$result = sirius_event_interestlist_cancel($event_type_term, $contact_nid);
		}

		if ($result['success']) { 
			drupal_set_message($event_type_term->name . ': ' . $result['msg']);
		} else {
			drupal_set_message($event_type_term->name . ': ' . $result['msg'], 'error');
		}
	}
}