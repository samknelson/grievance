<?php

function sirius_trust_election_wizard_info() {
  $items = array();
  $items['trust_election'] = array(
    'name' => 'trust_election',
    'title' => 'Trust Election',
    'description' => '',
    'callback_step_list' => 'sirius_trust_election_wizard_steps',
  );
  return $items;
}

function sirius_trust_election_wizard_steps($data, $wizard_node) {
	$steps = &drupal_static(__FUNCTION__);
	if (isset($steps)) { return $steps; }

  $steps = array();

	if (user_access('sirius trust staff')) {
  	$steps['policy'] = array(
  		'label' => 'Policy', 
  		'callback' => 'sirius_trust_election_wizard_policy_form',
  		'file' => 'sirius_trust.election_wizard.policy.inc',
  		'module' => 'sirius_trust',
  	);
  }

	$result = sirius_trust_election_wizard_get_policy($data);
	if (!$result['success']) { drupal_set_message($result['msg'], 'error'); }
	$policy_rnode = $result['data'];

	if ($policy_rnode) {
		$policy_json = sirius_json_get($policy_rnode);
		$intro = $policy_json['elections']['intro']['body'];
		if (strlen($intro) > 10) {
	  	$steps['intro'] = array(
	  		'label' => 'Intro', 
	  		'callback' => 'sirius_trust_election_wizard_intro_form',
	  		'file' => 'sirius_trust.election_wizard.intro.inc',
	  		'module' => 'sirius_trust',
	  	);
	  }

  	$steps['addr'] = array(
  		'label' => 'Address', 
  		'callback' => 'sirius_trust_election_wizard_addr_form',
  		'file' => 'sirius_trust.election_wizard.addr.inc',
  		'module' => 'sirius_trust',
  	);

  	$steps['phone'] = array(
  		'label' => 'Phone', 
  		'callback' => 'sirius_trust_election_wizard_phone_form',
  		'file' => 'sirius_trust.election_wizard.phone.inc',
  		'module' => 'sirius_trust',
  	);

  	$steps['email'] = array(
  		'label' => 'Email', 
  		'callback' => 'sirius_trust_election_wizard_email_form',
  		'file' => 'sirius_trust.election_wizard.email.inc',
  		'module' => 'sirius_trust',
  	);

	  $disclaimer_rnids = array();
	  if ($policy_json['elections']['disclaimer']['nids']) {
	  	$disclaimer_rnids = array_filter(array_values($policy_json['elections']['disclaimer']['nids']));
	  }
	  if ($disclaimer_rnids) {
	  	$steps['disclaimers'] = array(
	  		'label' => 'Disclaimer', 
	  		'callback' => 'sirius_trust_election_wizard_disclaimers_form',
	  		'file' => 'sirius_trust.election_wizard.disclaimers.inc',
	  		'module' => 'sirius_trust',
	  	);
	  }

	  $steps['benefits'] = array(
	  	'label' => 'Benefits', 
	  	'callback' => 'sirius_trust_election_wizard_benefits_form',
  		'file' => 'sirius_trust.election_wizard.benefits.inc',
  		'module' => 'sirius_trust',
	  );
  	$steps['dependents'] = array(
  		'label' => 'Dependents', 
  		'callback' => 'sirius_trust_election_wizard_dependents_form',
  		'file' => 'sirius_trust.election_wizard.dependents.inc',
  		'module' => 'sirius_trust',
  	);
	}

	if ($data['dependents']['general']['count_new'] > 0) {
		for ($which=1; $which<=$data['dependents']['general']['count_new']; ++$which) {
			if ($data['dependents']['new'][$which]['name_first']) {
				$label .= "Dependent " . $data['dependents']['new'][$which]['name_first'] . ' ' . $data['dependents']['new'][$which]['name_last'];
			} else {
				$label .= "New Dependent #$which";
			}
			$steps["dependent_$which"] = array(
				'label' => $label, 
				'callback' => 'sirius_trust_election_wizard_dependent_new_form',
	  		'file' => 'sirius_trust.election_wizard.dependent.inc',
	  		'module' => 'sirius_trust',
			);
		}
	}

	if ($policy_json) {
	  if ($policy_json['elections']['esignature']['def']) {
			$steps['signature'] = array(
				'label' => 'Signature', 
				'callback' => 'sirius_trust_election_wizard_signature_form',
	  		'file' => 'sirius_trust.election_wizard.signature.inc',
	  		'module' => 'sirius_trust',
			);
		}
	}

  $steps['push'] = array(
  	'label' => 'Submit', 
  	'callback' => 'sirius_trust_election_wizard_push_form',
		'file' => 'sirius_trust.election_wizard.push.inc',
		'module' => 'sirius_trust',
  );

  if (user_access('sirius trust staff')) {
		$steps['post'] = array(
			'label' => 'Post', 
			'callback' => 'sirius_trust_election_wizard_post_form',
			'file' => 'sirius_trust.election_wizard.post.inc',
			'module' => 'sirius_trust',
		);
	}

  return $steps;

  $required_attachments = $event_json['intake']['attachments']['required'];
  if ($required_attachments) {
  	$required_attachments = array_filter(array_values($required_attachments));
  	foreach ($required_attachments as $tid) {
  		$attachment_term = taxonomy_term_load($tid);
  		$steps['attachment_' . $tid] = array('label' => $attachment_term->name, 'callback' => 'sirius_trust_election_wizard_attachment', 'callback_args' => array('tid' => $tid));
  	}
  } else {
  	$required_attachments = array();
  }

  $optional_attachments = $event_json['intake']['attachments']['optional'];
  if ($optional_attachments) {
  	$optional_attachments = array_filter(array_values($optional_attachments));
  	$optional_attachments = array_diff($optional_attachments, $required_attachments);
  	if ($optional_attachments) {
  		if ($required_attachments) { $label = 'Other Documents'; } else { $label = 'Documents'; }
	  	$steps['optional_attachments'] = array('label' => $label, 'callback' => 'sirius_trust_election_wizard_attachments_optional');
	  }
	}

  return $steps;
}

function sirius_trust_election_wizard_locked($wizard_node) {
	// Not locked if it's in draft status
	if ($wizard_node->field_sirius_type['und'][0]['value'] == 'draft') { return FALSE; }

	// Not locked if we're an admin
	if (user_access('sirius trust staff')) { return FALSE; }

	// Oh well.
	return TRUE;
}

function sirius_trust_election_wizard_get_worker($wizard_node) {
	$ssn = $wizard_node->field_sirius_notes['und'][0]['value'];
	if (!$ssn) { return array('success' => FALSE, 'msg' => 'There is no SSN for this election'); }
	$result = sirius_proxy_call('sirius_worker_get_by_ssn', $ssn);
	return $result;
}

function sirius_trust_election_wizard_get_policy($data) {
	$policy_rnid = $data['policy']['policy_rnid'];
	if (!$policy_rnid) { return array('success' => FALSE, 'msg' => 'No policy set for this election.'); }

	// Need to get the policy. Which means a proxy call to node_load(), which we don't really want to allow. @todo.
	// return array('success' => FALSE, 'msg' => "How do we load a proxy_nid like Maria # $policy_rnid?");
	$result = sirius_proxy_call('sirius_proxy_node_load', $policy_rnid);
	return $result;
}

function sirius_trust_election_set_title(&$wizard_node) {
	$result = sirius_trust_election_wizard_get_worker($wizard_node);
	if (!$result['success']) { return; }
	$worker_rnode = $result['data'];
	$title = 'Election: ' . $worker_rnode->title;
	if ($title != $wizard_node->title) {
		$wizard_node->title = substr($title, 0, 255);
		node_save($wizard_node);
	}
}

function sirius_trust_election_wizard_form_prep(&$form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['wizard_node'] = $wizard_node;
	$form_state['current_step'] = $current_step;

	$form['#tree'] = TRUE;

	$form['survey_wrap_open'] = array('#markup' => '<div id="survey_wrap" class="survey_wrap_inpage sirius-form">', '#weight' => -10);
	$form['survey_wrap_close'] = array('#markup' => '</div>', '#weight' => 10);


	if (sirius_trust_election_wizard_locked($wizard_node)) {
		$form['#disabled'] = TRUE; 
	} else {
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
			'#weight' => 20,
		);
	}

	return sirius_form_prep($form);
}
