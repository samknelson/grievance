<?php

function sirius_node_tab_event_bullpen_self_access($event_node) {
	$access = Sirius_Event_Bullpen_Access::getInstance();
	$context = array('event_node' => $event_node);
	return $access->access('self_view', $context);
}

function sirius_node_tab_event_bullpen_self($event_node) {
  sirius_event_disclaimer_redirect($event_node);
  global $user;

  $user_full = user_load($user->uid);
  $contact_node = node_load($user_full->field_sirius_contact['und'][0]['target_id']);
  if (!$contact_node) { return sirius_page_error("There is no contact associated with your user."); }

  $participant_node = sirius_event_participant_get($event_node->nid, $contact_node->nid);
  if ($participant_node) { 
  	return drupal_get_form('sirius_node_tab_event_bullpen_self_edit_form', $event_node, $participant_node);
  } else {
		return drupal_get_form('sirius_node_tab_event_bullpen_self_register_form', $event_node);
  }
}

function sirius_node_tab_event_bullpen_self_register_form($form, &$form_state, $event_node) {
	$access = Sirius_Event_Bullpen_Access::getInstance();
	$context = array();
	$context['event_node'] = $event_node;
	$context['participant_node'] = $participant_node;
	if (!$access->access('self_register', $context)) { 
		return sirius_form_error("You cannot sign yourself into the bullpen at this time: " . '<strong>' . $access->error_msg() . '</strong>');
	}

	$form_state['event_nid'] = $event_node->nid;
	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#required' => TRUE,
		'#title' => t('Yes, I want to sign in to this bullpen.'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);
	return $form;
}

function sirius_node_tab_event_bullpen_self_register_form_submit($form, &$form_state) {
	$event_nid = $form_state['event_nid'];
  $event_node = node_load($event_nid);
  global $user;
  $user_full = user_load($user->uid);
  $contact_nid = $user_full->field_sirius_contact['und'][0]['target_id'];
  $event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
  $json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
	list ($prole_tid, $pstatus_tid) = explode('|', $json['bullpen']['pre_dispatch']);

  if (!$prole_tid || !$pstatus_tid) {
    drupal_set_messge(t('The event is misconfigured; no prole or pstatus set.'), 'error');
    return;
  }

	$result = sirius_event_participant_register($event_nid, $contact_nid, $prole_tid, $pstatus_tid);
  if ($result['success']) { drupal_set_message(t('You have been signed in to this bullpen.')); } else { drupal_set_message($result['msg'], 'error'); }
}









function sirius_node_tab_event_bullpen_self_edit_form($form, &$form_state, $event_node, $participant_node) {
	$form_state['event_nid'] = $event_node->nid;
	$form_state['participant_nid'] = $participant_node->nid;

  $pstatus_term = taxonomy_term_load($participant_node->field_sirius_event_pstatus['und'][0]['tid']);
  $prole_term = taxonomy_term_load($participant_node->field_sirius_event_prole['und'][0]['tid']);

  $form['intro'] = array(
  	'#markup' => '<div class="sirius_form_intro">' . 
  		t('You are currently registered for this bullpen as: ') . 
  		sirius_fakerender_field('Role:', $prole_term->name) . 
  		sirius_fakerender_field('Status:', $pstatus_term->name) . 
  		'</div>',
  );

  $event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
  $json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
	list ($checkin_prole_tid, $checkin_pstatus_tid) = explode('|', $json['bullpen']['pre_dispatch']);
	list ($checkout_prole_tid, $checkout_pstatus_tid) = explode('|', $json['bullpen']['checkout']);


	$access = Sirius_Event_Bullpen_Access::getInstance();
	$context = array();
	$context['event_node'] = $event_node;
	$context['participant_node'] = $participant_node;

	if ($checkin_prole_tid && $checkin_pstatus_tid && $checkout_prole_tid && $checkout_pstatus_tid) {

		if ($prole_term->tid == $checkin_prole_tid && $pstatus_term->tid == $checkin_pstatus_tid) {

			if (!$access->access('self_signout', $context)) { 
				$form['msg'] = sirius_form_error("You cannot sign yourself out of the bullpen at this time: " . '<strong>' . $access->error_msg() . '</strong>');
			} else {
				$form['paccess'] = array(
					'#type' => 'value',
					'#value' => "$checkout_prole_tid|$checkout_pstatus_tid",
				);
				$form['submit'] = array(
					'#type' => 'submit',
					'#value' => t('Sign out of this bullpen'),
				);
			}


		} else if ($prole_term->tid == $checkout_prole_tid && $pstatus_term->tid == $checkout_pstatus_tid) {
			if (!$access->access('self_signin', $context)) { 
				$form['msg'] = sirius_form_error("You cannot sign yourself into the bullpen at this time: " . '<strong>' . $access->error_msg() . '</strong>');
			} else {
				$form['paccess'] = array(
					'#type' => 'value',
					'#value' => "$checkin_prole_tid|$checkin_pstatus_tid",
				);
				$form['submit'] = array(
					'#type' => 'submit',
					'#value' => t('Sign back in to this bullpen'),
				);
			}
		}
	}

	return $form;
}

function sirius_node_tab_event_bullpen_self_edit_form_submit($form, &$form_state) {
	$participant_node = node_load($form_state['participant_nid']);
	list ($prole_tid, $pstatus_tid) = explode('|', $form_state['values']['paccess']);
  sirius_event_participant_pstatus_set($participant_node, $prole_tid, $pstatus_tid);
	drupal_set_message(t('Status updated.'));
}



