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
		  'file' => 'election_wizard/policy.inc',
  		'module' => 'sirius_trust',
  	);
  }

	$policy_node = sirius_trust_election_wizard_get_policy($data);

	if ($policy_node) {
		$policy_json = sirius_json_get($policy_node);
		$intro = $policy_json['elections']['intro']['body'];
		if (strlen($intro) > 10) {
	  	$steps['intro'] = array(
	  		'label' => 'Intro', 
	  		'callback' => 'sirius_trust_election_wizard_intro_form',
	  		'file' => 'election_wizard/intro.inc',
	  		'module' => 'sirius_trust',
	  	);
	  }

  	$steps['name'] = array(
  		'label' => 'Name', 
  		'callback' => 'sirius_trust_election_wizard_name_form',
  		'callback_render' => 'sirius_trust_election_wizard_name_render',
  		'callback_post' => 'sirius_trust_election_wizard_name_post',
		  'file' => 'election_wizard/name.inc',
  		'module' => 'sirius_trust',
  	);

  	$steps['addr'] = array(
  		'label' => 'Address', 
  		'callback' => 'sirius_trust_election_wizard_addr_form',
  		'callback_render' => 'sirius_trust_election_wizard_addr_render',
  		'callback_post' => 'sirius_trust_election_wizard_addr_post',
		  'file' => 'election_wizard/addr.inc',
  		'module' => 'sirius_trust',
  	);

  	$steps['phone'] = array(
  		'label' => 'Phone', 
  		'callback' => 'sirius_trust_election_wizard_phone_form',
  		'callback_render' => 'sirius_trust_election_wizard_phone_render',
  		'callback_post' => 'sirius_trust_election_wizard_phone_post',
		  'file' => 'election_wizard/phone.inc',
  		'module' => 'sirius_trust',
  	);

  	$steps['email'] = array(
  		'label' => 'Email', 
  		'callback' => 'sirius_trust_election_wizard_email_form',
  		'callback_render' => 'sirius_trust_election_wizard_email_render',
  		'callback_post' => 'sirius_trust_election_wizard_email_post',
		  'file' => 'election_wizard/email.inc',
  		'module' => 'sirius_trust',
  	);

	  $disclaimer_nids = array();
	  if ($policy_json['elections']['disclaimer']['nids']) {
	  	$disclaimer_nids = array_filter(array_values($policy_json['elections']['disclaimer']['nids']));
	  }
	  if ($disclaimer_nids) {
	  	$steps['disclaimers'] = array(
	  		'label' => 'Disclaimer', 
	  		'callback' => 'sirius_trust_election_wizard_disclaimers_form',
	  		// 'callback_render' => 'sirius_trust_election_wizard_disclaimers_render',
			  'file' => 'election_wizard/disclaimers.inc',
	  		'module' => 'sirius_trust',
	  	);
	  }

	  $steps['benefits'] = array(
	  	'label' => 'Benefits', 
	  	'callback' => 'sirius_trust_election_wizard_benefits_form',
  		'callback_render' => 'sirius_trust_election_wizard_benefits_render',
		  'file' => 'election_wizard/benefits.inc',
  		'module' => 'sirius_trust',
	  );

	  // Add any cardchecks
	  $benefits_bytype = $data['benefits']['by_type'];
	  if (!$benefits_bytype) { $benefits_bytype = array(); }
  	foreach ($benefits_bytype as $type_tid => $benefit_nid) {
  		if (!$benefit_nid) { continue; }
  		$benefit_node = node_load($benefit_nid);
	  	$plugins = sirius_trust_eligibility_plugins_enabled($policy_node, $benefit_node);
	  	if (!in_array('cardcheck', $plugins)) { continue; }
			$settings = sirius_trust_eligibility_plugin_settings($policy_node, $benefit_node, 'cardcheck');
			$entries = $settings['entries'];
			if (!$entries) { $entries = array(); }
			foreach ($entries as $delta => $entry) {
				$cardcheck_definition_nid = $entry['definition_nid'];
				if (!$cardcheck_definition_nid) { continue; }

			  $steps['cardcheck_' . $cardcheck_definition_nid] = array(
			  	'label' => sirius_node_title($cardcheck_definition_nid), 
			  	'callback' => 'sirius_trust_election_wizard_cardcheck_form',
		  		// 'callback_render' => 'sirius_trust_election_wizard_type_render',
					// 'callback_post' => 'sirius_trust_election_wizard_cardcheck_post',
				  'file' => 'election_wizard/cardcheck.inc',
		  		'module' => 'sirius_trust',
			  );
			}
		}

		/*
		// We no longer explicitly pick the election type.
	  $steps['type'] = array(
	  	'label' => 'Election Type', 
	  	'callback' => 'sirius_trust_election_wizard_type_form',
  		'callback_render' => 'sirius_trust_election_wizard_type_render',
		  'file' => 'election_wizard/type.inc',
  		'module' => 'sirius_trust',
	  );
	  */
	}

	/*
	$steps['dependents'] = array(
		'label' => 'Dependents', 
		'callback' => 'sirius_trust_election_wizard_dependents_form',
		'callback_render' => 'sirius_trust_election_wizard_dependents_render',
		'callback_post' => 'sirius_trust_election_wizard_dependents_post',
	  'file' => 'election_wizard/dependents.inc',
		'module' => 'sirius_trust',
	);
	*/


	if ($data['dependents']['eligibility']) {
		foreach ($data['dependents']['eligibility'] as $relationship_nid => $eligibility_data) {
			$relationship_node = node_load($relationship_nid);
			$contact_nid = $relationship_node->field_sirius_contact_alt['und'][0]['target_id'];
			$label = "Dependent: " . sirius_node_title($contact_nid);
			$steps["dependent_existing_$relationship_nid"] = array(
				'label' => $label, 
				'callback' => 'sirius_trust_election_wizard_dependent_existing_form',
				'callback_render' => 'sirius_trust_election_wizard_dependent_existing_render',
			  'file' => 'election_wizard/dependent_existing.inc',
	  		'module' => 'sirius_trust',
			);
		}
	}

  $steps['dependent_new_count'] = array(
  	'label' => 'New Dependents', 
  	'callback' => 'sirius_trust_election_wizard_dependent_new_count_form',
	  'file' => 'election_wizard/dependent_new_count.inc',
		'module' => 'sirius_trust',
  );

	// Need a page for each new relationship which you want to add.
	if ($data['dependents']['general']['count_new'] > 0) {
		for ($which=1; $which<=$data['dependents']['general']['count_new']; ++$which) {
			$label = "New Dependent #$which";
			$steps["dependent_new_$which"] = array(
				'label' => $label, 
				'callback' => 'sirius_trust_election_wizard_dependent_new_form',
	  		'callback_render' => 'sirius_trust_election_wizard_dependent_new_render',
			  'file' => 'election_wizard/dependent_new.inc',
	  		'module' => 'sirius_trust',
			);
		}
	}

	$steps['beneficiaries'] = array(
		'label' => 'Beneficiaries', 
		'callback' => 'sirius_trust_election_wizard_beneficiaries_form',
		'callback_render' => 'sirius_trust_election_wizard_beneficiaries_render',
		'callback_post' => 'sirius_trust_election_wizard_beneficiaries_post',
	  'file' => 'election_wizard/beneficiaries.inc',
		'module' => 'sirius_trust',
	);

	if ($policy_json) {
	  if ($policy_json['elections']['esignature']['def']) {
			$steps['signature'] = array(
				'label' => 'Signature', 
				'callback' => 'sirius_trust_election_wizard_signature_form',
				'callback_render' => 'sirius_trust_election_wizard_signature_render',
			  'file' => 'election_wizard/signature.inc',
	  		'module' => 'sirius_trust',
			);
		}
	}

  if (user_access('sirius trust staff')) {
		$steps['post'] = array(
			'label' => 'Post', 
			'callback' => 'sirius_trust_election_wizard_post_form',
			'callback_post' => 'sirius_trust_election_wizard_post_post',
		  'file' => 'election_wizard/post.inc',
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
	return sirius_log_handler($wizard_node, 'sirius_worker');
}

function sirius_trust_election_wizard_get_policy($data) {
	$policy_nid = $data['policy']['policy_nid'];
	return node_load($policy_nid);
}

function sirius_trust_election_set_title(&$wizard_node) {
	$worker_node = sirius_trust_election_wizard_get_worker($wizard_node);
	$title = 'Election: ' . $worker_node->title;
	if ($title != $wizard_node->title) {
		$wizard_node->title = substr($title, 0, 255);
		node_save($wizard_node);
	}
}

function sirius_trust_election_wizard_get_employer($data) {
	return node_load($data['employer']['employer_nid']);
}

function sirius_trust_election_wizard_get_election_type($data) {
	return taxonomy_term_load($data['election_type']['tid']);
}

function sirius_trust_election_wizard_form_prep(&$form, &$form_state, $info, $data, $wizard_node, $current_step) {
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');

	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['wizard_node'] = $wizard_node;
	$form_state['current_step'] = $current_step;

	$form['#tree'] = TRUE;

	$form['survey_wrap_open'] = array('#markup' => '<div id="survey_wrap" class="survey_wrap_inpage sirius-form">', '#weight' => -10);
	$form['survey_wrap_close'] = array('#markup' => '</div>', '#weight' => 10);

	$status = sirius_trust_election_wizard_get_status($wizard_node);

	if (sirius_trust_election_wizard_locked($wizard_node)) {
		$form['#disabled'] = TRUE; 
	} else if ($current_step == 'post' && $status == 'posted') {
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


function sirius_trust_election_wizard_get_status($wizard_node) {
	return $wizard_node->field_sirius_type['und'][0]['value'];
}