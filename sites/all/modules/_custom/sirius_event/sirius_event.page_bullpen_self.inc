<?php

function sirius_node_tab_event_bullpen_self_access($event_node) {
	if (!sirius_node_tab_event_bullpen_access($event_node)) { return FALSE; }

  // Must have a worker
  $worker_node = sirius_worker_get();
  if (!$worker_node) { 
    return FALSE;
  }

	return TRUE;
}

function sirius_node_tab_event_bullpen_self($event_node) {
  global $user;

  $user_full = user_load($user->uid);
  $contact_node = node_load($user_full->field_sirius_contact['und'][0]['target_id']);
  if (!$contact_node) { return "There is no contact associated with your user.";}
  $participant_node = sirius_event_participant_get($event_node->nid, $contact_node->nid);
  if ($participant_node) { 
  	return drupal_get_form('sirius_node_tab_event_bullpen_self_edit_form', $event_node, $participant_node);
  } 

	$possible = sirius_event_bullpen_selfsignin_possible($event_node);
	if (!$possible['success']) {
		$html = "You cannot sign yourself into the bullpen at this time: " . '<strong>' . $possible['msg'] . '</strong><br /><br />';
		return $html;
	}

	return drupal_get_form('sirius_node_tab_event_bullpen_self_register_form', $event_node);
}

function sirius_node_tab_event_bullpen_self_register_form($form, &$form_state, $event_node) {
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

	$possible = sirius_event_bullpen_selfsignin_possible($event_node);
	if (!$possible['success']) {
	  $form['locked'] = array(
	  	'#markup' => '<div class="sirius_form_intro">' . 
	  		t('You cannot edit this information at this time: ') . $possible['msg'],
	  );
	} else {

	  $event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	  $json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
		list ($checkin_prole_tid, $checkin_pstatus_tid) = explode('|', $json['bullpen']['pre_dispatch']);
		list ($checkout_prole_tid, $checkout_pstatus_tid) = explode('|', $json['bullpen']['checkout']);

		if ($checkin_prole_tid && $checkin_pstatus_tid && $checkout_prole_tid && $checkout_pstatus_tid) {
			if ($prole_term->tid == $checkin_prole_tid && $pstatus_term->tid == $checkin_pstatus_tid) {
				$form['paccess'] = array(
					'#type' => 'value',
					'#value' => "$checkout_prole_tid|$checkout_pstatus_tid",
				);
				$form['submit'] = array(
					'#type' => 'submit',
					'#value' => t('Sign out of this bullpen'),
				);
			} else if ($prole_term->tid == $checkout_prole_tid && $pstatus_term->tid == $checkout_pstatus_tid) {
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
	$participant_node->field_sirius_event_prole['und'][0]['tid'] = $prole_tid;
	$participant_node->field_sirius_event_pstatus['und'][0]['tid'] = $pstatus_tid;
	node_save($participant_node);
	drupal_set_message(t('Status updated.'));
}

function sirius_event_bullpen_selfsignin_possible($event_node) {
  // Self sign-in must be open
  $ts = sirius_event_bullpen_ts($event_node);
  $datestr = 'g:i A, l, F j';
  if ($ts['signin_opens_self'] > time()) {
		// return array('success' => FALSE, 'msg' => 'Self sign-in opens on ' . date($datestr, $ts['signin_opens_self']));
  }

  // Must have a worker
  $worker_node = sirius_worker_get();
  if (!$worker_node) {
    return array('success' => FALSE, 'msg' => 'There is no worker associated with your user.');
  }

  // Must have a contact
  $contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);
  if (!$contact_node) {
    return array('success' => FALSE, 'msg' => 'There is no contact associated with your user.');
  }

  // Must be eligible for the host job
  $job_node = sirius_event_bullpen_job($event_node->nid);
  $result = sirius_dispatch_job_get_dispatchable_workers_explain($job_node, $worker_node->nid, array('ignore_skill' => TRUE, 'ignore_department' => TRUE, 'ignore_dispatch_status' => TRUE, 'ignore_event' => TRUE));
  if (!$result['success']) {
    return array('success' => FALSE, 'msg' => $result['msg']);
  }

  // Must be employed by the current employer
  $current_employer_nid = $worker_node->field_grievance_shop['und'][0]['target_id'];
  if ($current_employer_nid != $job_node->field_grievance_shop['und'][0]['target_id']) {
    return array('success' => FALSE, 'msg' => "You can only sign yourself into bullpens offered by your current employer.");
  }

  // Must be EBA on the correct date
  /*
  if ($worker_node->field_sirius_dispatch_eba['und'][0]['value'] != 'Yes') {
    return array('success' => FALSE, 'msg' => "You can only sign yourself into this bullpen if you have indicated that you are employed-but-available for this date. Please check your " . l('dispatch preferences.', 'sirius/worker/me/sirius_worker_dispatch_preferences'));
  }
  $found = FALSE;
  $eba_dates = sirius_fieldvals($worker_node, 'field_sirius_dispatch_eba_dates');
  $job_date = date('Y-m-d', strtotime(sirius_fieldval($job_node, 'field_sirius_datetime')));
  foreach ($eba_dates as $eba_date) {
    $eba_date = date('Y-m-d', strtotime($eba_date));
    if ($eba_date == $job_date) {
      $found = TRUE;
      break;
    }
  }
  if (!$found) {
    // return array('success' => FALSE, 'msg' => "You can only sign yourself into this bullpen if you have indicated that you are employed-but-available for this date. Please check your " . l('dispatch preferences.', 'sirius/worker/me/sirius_worker_dispatch_preferences'));
  }
  */

  return array('success' => TRUE, 'msg' => 'Self sign-in here.');

}