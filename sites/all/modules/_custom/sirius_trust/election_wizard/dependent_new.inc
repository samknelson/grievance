<?php

function sirius_trust_election_wizard_dependent_new_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$access = Sirius_Trust_Access::getInstance();

	$policy_node = sirius_trust_election_wizard_get_policy($data);
	$policy_json = sirius_json_get($policy_node);

	$which = preg_replace('/dependent_new_/', '', $current_step);
	if (!($which > 0)) { return sirius_form_error("Don't know which dependent."); }

	$form['prefix'] = array('#markup' => $policy_json['elections']['dependents']['new']['prefix']);

	// $relationship_type_lookup = sirius_taxonomy_load('sirius_contact_relationship_types', 'tid', 'full');
	$relationship_type_options = sirius_taxonomy_load('sirius_contact_relationship_types', 'tid', 'name');

	$form['dependents']['new'][$which]['reltype'] = array(
		'#title' => t('Your Relationship to the Dependent'),
		'#type' => 'select',
		'#options' => array(NULL => t('-- Select --')) + $relationship_type_options,
		'#default_value' => $data['dependents']['new'][$which]['reltype'],
		'#required' => TRUE,
	);

	$form['dependents']['new'][$which]['name_first'] = array(
		'#title' => t('First Name'),
		'#type' => 'textfield',
		'#default_value' => $data['dependents']['new'][$which]['name_first'],
		'#required' => TRUE,
	);

	$form['dependents']['new'][$which]['name_last'] = array(
		'#title' => t('Last Name'),
		'#type' => 'textfield',
		'#default_value' => $data['dependents']['new'][$which]['name_last'],
		'#required' => TRUE,
	);

	$form['dependents']['new'][$which]['ssn'] = array(
		'#title' => t('SSN'),
		'#type' => 'textfield',
		'#default_value' => $data['dependents']['new'][$which]['ssn'],
		'#required' => TRUE,
		'#size' => 11,
	);

	$form['dependents']['new'][$which]['dob'] = array(
		'#title' => t('Date of Birth'),
		'#type' => 'date_popup',
		'#default_value' => $data['dependents']['new'][$which]['dob'],
		'#required' => TRUE,
	   '#attributes' => array(
	      'autocomplete' => 'off',
	    ),
    '#required' => TRUE,
    '#date_format' => 'Y-m-d',
	);

	$form['dependents']['new'][$which]['phone'] = array(
		'#title' => t('Phone'),
		'#type' => 'textfield',
		'#default_value' => $data['dependents']['new'][$which]['phone'],
		'#size' => 11,
	);


	$form['file_intro'] = array(
		'#markup' => '<br /><br /><h2>' . t('Vital Document') . '</h2><p>' . t('Use the field below to upload documentation verifying the new dependent\'s status.') . '</p>',
	);
	$fid = $data['dependents']['new'][$which]['file']['fid'];
	if ($fid) {
		$file = (array) file_load($fid);
		$details = '<strong>Current File:</strong> ';
		$url = file_create_url($file['uri']);

		$details .= $file['origname'];
		$details .= ' (';
		$details .= format_size($file['filesize']);
		$details .= ')';
		if ($access->access('staff')) {
			$url = file_create_url($file['uri']);
			$details .= "&nbsp;&nbsp;[<a href=\"$url\" target=\"_blank\">View</a>]";
		}

		$form['current'] = array('#markup' => $details);
		$upload_title = "Replace With";
	} else {
		$upload_title = "Upload";
	}

	$form['file'] = array(
		'#type' => 'file',
		'#title' => $upload_title,
		// '#required' => TRUE,
	);

	$form['suffix'] = array('#markup' => $policy_json['elections']['dependents']['new']['suffix']);

	return sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_trust_election_wizard_dependent_new_form_validate($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$current_step = $form_state['current_step'];
	$wizard_node = $form_state['wizard_node'];
	$which = preg_replace('/dependent_new_/', '', $current_step);

	unset($form_state['storage']["file"]);

	$ssn = $form_state['values']['dependents']['new'][$which]['ssn'];
	if (!sirius_ssn_validate($ssn)) {
		form_set_error("dependents][new][$which][ssn", 'Please enter a valid SSN.');
	}
	$probe = sirius_worker_get_by_ssn($ssn);
	if ($probe) {
		form_set_error('dependents][new][$which][ssn', 'There is already a member with this SSN in the database. A staff member must create this relationship from the member page.');
	}
	$ssn = sirius_ssn_format($ssn);

	if ($data['dependents']['new']) {
		foreach ($data['dependents']['new'] as $probe_which => $probe_dependent) {
			if ($probe_which == $which) { continue; }
			if ($probe_dependent['ssn'] == $ssn) {
				form_set_error("dependents][new][$which][ssn", 'Please enter a unique SSN for each dependent.');
			}
		}
	}


	if ($form_state['values']['dependents']['new'][$which]['phone'] && !sirius_phone_validate($form_state['values']['dependents']['new'][$which]['phone'])) {
		form_set_error("dependents][new][$which][phone", 'Please enter a valid phone number.');
	}

	$ts = strtotime($form_state['values']['dependents']['new'][$which]['dob']);
	if ($ts === FALSE) {
		form_set_error("dependents][new][$which][dob", 'Please enter a valid birth date.');
	}
	if ($ts >= time()) {
		form_set_error("dependents][new][$which][dob", 'Birth date must be in the past!');	
	}

	$info = field_info_instance('node', 'field_sirius_attachments', 'sirius_log');
	$extensions = $info['settings']['file_extensions'];
  $file = file_save_upload("file", array(
    'file_validate_extensions' => array($extensions),
  ));
  if ($file) {
    $name = rand();
    if ($file = file_move($file, 'private://attach_' . rand())) {
      $form_state['storage']["file"] = $file;
    }
    else {
      form_set_error("file", t("Failed to write the uploaded file to the site's file folder."));
    }
  } else {
  	if (!$data['dependents']['new'][$which]['file']) {
			form_set_error("file", t("Please upload a file."));
		}
  }
}

function sirius_trust_election_wizard_dependent_new_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$current_step = $form_state['current_step'];
	$wizard_node = $form_state['wizard_node'];
	$which = preg_replace('/dependent_new_/', '', $current_step);

	$old_file = $data['dependents']['new'][$which]['file'];
	$old_fid = $data['dependents']['new'][$which]['file']['fid'];
	$data['dependents']['new'][$which] = $form_state['values']['dependents']['new'][$which];
	$data['dependents']['new'][$which]['ssn'] = sirius_ssn_format($data['dependents']['new'][$which]['ssn']);

  $file = $form_state['storage']["file"];
  if ($file) {
		$file = (array) $file;
		$file['display'] = 1;
		unset($form_state['storage']["attach_$delta"]);
		unset($file['destination']);
		unset($file['uri']);

		// Remove the previous file
		$old_fid = $data['dependents']['new'][$which]['file']['fid'];
		if ($old_fid) {
			if ($wizard_node->field_sirius_attachments['und']) {
				foreach ($wizard_node->field_sirius_attachments['und'] as $delta => $entry) {
					if ($entry['fid'] == $old_fid) { 
						unset($wizard_node->field_sirius_attachments['und'][$delta]);
					}
				}
			}
		}

		// Add the new
		$wizard_node->field_sirius_attachments['und'][] = $file;
		$data['dependents']['new'][$which]['file'] = $file;
	} else {
		$data['dependents']['new'][$which]['file'] = $old_file;
	}

	sirius_trust_election_set_title($wizard_node);
	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => FALSE));
}

function sirius_trust_election_wizard_dependent_new_render($info, &$data, $wizard_node, $current_step) {
	$which = preg_replace('/dependent_new_/', '', $current_step);
	$dependent = $data['dependents']['new'][$which];

	$reltype = taxonomy_term_load($dependent['reltype']);
	$html .= sirius_fakerender_field('First Name:', $dependent['name_first']);
	$html .= sirius_fakerender_field('Last Name:', $dependent['name_last']);
	$html .= sirius_fakerender_field('Relationship:', $reltype->field_sirius_name_alt['und'][0]['value']);
	$html .= sirius_fakerender_field('SSN:', $dependent['ssn']);
	$html .= sirius_fakerender_field('DoB:', $dependent['dob']);
	$html .= sirius_fakerender_field('Phone:', $dependent['phone']);

	return $html;
}

