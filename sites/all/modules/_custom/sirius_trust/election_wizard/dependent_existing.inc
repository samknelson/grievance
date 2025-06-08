<?php

function sirius_trust_election_wizard_dependent_existing_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$access = Sirius_Trust_Access::getInstance();

	$policy_node = sirius_trust_election_wizard_get_policy($data);
	$policy_json = sirius_json_get($policy_node);

	$worker_node = sirius_trust_election_wizard_get_worker($wizard_node);

	// Which relationship are we looking at?
	$relationship_nid = preg_replace('/dependent_existing_/', '', $current_step);
	if (!($relationship_nid > 0)) { return sirius_form_error("Don't know relationship_nid dependent."); }
	$relationship_node = node_load($relationship_nid);
	$dependent_contact_nid = $relationship_node->field_sirius_contact_alt['und'][0]['target_id'];
	if (!$dependent_contact_nid) { return sirius_form_error("Contact not found."); }

	// Were they covered previously?
	$prior_election_node = sirius_trust_worker_election($worker_node);
	$prior_relationship_nids = sirius_fieldvals($prior_election_node, 'field_sirius_contact_relations', 'target_id');
	$prior_coverage = FALSE;	
	if ($prior_relationship_nids && in_array($relationship_nid, $prior_relationship_nids)) {
		$prior_coverage = TRUE;
	}
	$form['dependents']['existing'][$relationship_nid]['prior_coverage'] = array('#type' => 'value', '#value' => $prior_coverage);

	// Eligibility
	$eligibility_data_by_benefit = $data['dependents']['eligibility'][$relationship_nid];
	if (!$eligibility_data_by_benefit) { $eligibility_data_by_benefit = array(); }
	$at_least_one = $eligibility_data_by_benefit['at_least_one'];
	unset($eligibility_data_by_benefit['at_least_one']);

	$intro = sirius_fakerender_field('Dependent Name:', sirius_node_title($dependent_contact_nid));
	$reltype_term = taxonomy_term_load($relationship_node->field_sirius_contact_reltype['und'][0]['tid']);
	if ($reltype_term) {
		$intro .= sirius_fakerender_field('Relationship:', $reltype_term->field_sirius_name_alt['und'][0]['value']);
	}
	if ($eligibility_data_by_benefit) {
		$intro .= '<br /><strong>' . t('This dependent is eligible for the following benefits.') . '</strong><br />';
		foreach ($eligibility_data_by_benefit as $benefit_nid => $result) {
			$intro .= sirius_fakerender_field(
				sirius_node_title($benefit_nid) . ': ',
				$result['success'] ? 'Yes' : 'No'
			);
		}
	}
	$form['dependents']['existing'][$relationship_nid]['intro'] = array('#markup' => $intro);

	// Do you want to add (or continue) coverage?
	$default = $data['dependents']['existing'][$relationship_nid]['active'];
	if ($prior_coverage) {
		if (!$default) { $default = 'Yes'; }

		$form['dependents']['existing'][$relationship_nid]['active'] = array(
			'#title' => t('Do you want to continue coverage for this dependent?'),
			'#type' => 'select',
			'#options' => array(
				'Yes' => t('Yes'),
				'No' => t('No'),
			),
			'#required' => TRUE,
			'#default_value' => $default,
			'#id' => 'relationship_active',
			'#prefix' => '<div>' . t('Based on your previous elections, you <strong>are currently providing coverage</strong> for this dependent.') . '</div>',
		);
	} else {
		if (!$default) { $default = 'No'; }

		$form['dependents']['existing'][$relationship_nid]['active'] = array(
			'#title' => t('Do you want to add coverage for this dependent?'),
			'#type' => 'select',
			'#options' => array(
				'Yes' => t('Yes'),
				'No' => t('No'),
			),
			'#required' => TRUE,
			'#default_value' => $default,
			'#id' => 'relationship_active',
			'#prefix' => '<div>' . t('Based on your previous elections, you <strong>are not currently providing coverage</strong> for this dependent.') . '</div>',
		);

		if (!$at_least_one) {
			$form['dependents']['existing'][$relationship_nid]['active']['#disabled'] = TRUE;
			$form['dependents']['existing'][$relationship_nid]['active']['#default_value'] = 'No';
		}
	}

	// If you are not continuing coverage, you *must* upload a document showing the life event.
	if ($prior_coverage) {
		$intro = '<p>';
		$intro .= t('If you are dropping coverage for a dependent, you <strong>are required</strong> to upload a document providing backup for the life event that permits coverage to be dropped.');
		$intro .= '</p>';

		$fid = $data['dependents']['existing'][$relationship_nid]['file']['fid'];
		if ($fid) {
			$file = (array) file_load($fid);
			$url = file_create_url($file['uri']);

			$intro .= '<p>';
			$intro .= '<strong>Current File:</strong> ';
			$intro .= $file['origname'];
			$intro .= ' (';
			$intro .= format_size($file['filesize']);
			$intro .= ')';
			if ($access->access('staff')) {
				$url = file_create_url($file['uri']);
				$intro .= "&nbsp;&nbsp;[<a href=\"$url\" target=\"_blank\">View</a>]";
			}
			$intro .= '</p>';

			$upload_title = "Replace With";
		} else {
			$upload_title = "Upload";
		}

		$form['file_intro'] = array(
			'#type' => 'container',
			'#states' => array('visible' => array(":input[id=\"relationship_active\"]" => array('value' => 'No'))),
		);
		$form['file_intro']['markup'] = array('#markup' => $intro);
		$form['file'] = array(
			'#type' => 'file',
			'#title' => $upload_title,
			'#states' => array('visible' => array(":input[id=\"relationship_active\"]" => array('value' => 'No'))),
		);
	}

	// For staff, show me the reason that the dependent is or is not eligible
	if ($access->access('staff', $access_context)) {
		foreach ($eligibility_data_by_benefit as $benefit_nid => $result) {
			$benefit_node = node_load($benefit_nid);

			if (!$result['plugins']) {
				$paths = array(
					"node/$policy_nid/sirius_trust_policy_benefits/$benefit_nid" => 'Eligibility Settings',
				);
				$tools = array();
				foreach ($paths as $path => $label) {
					if (drupal_valid_path($path)) {
						$tools[] = l($label, $path, array('attributes' => array('class' => array('tool-button'))));
					}
				}
				$class = 'ineligible';
				$row = array();
				$row[] = $benefit_node->title;
				$row[] = 'OVERALL';
				$row[] = 'No';
				$row[] = 'There are no eligibility plugins for this benefit.';
				$row[] = join('', $tools);
				$rows[] = array('data' => $row, 'class' => array($class, 'summary'));
				continue;
			} 

			$subrows = array();
			$summary_eligible = TRUE;
			$summary_failed = array();
			foreach ($result['plugins'] as $plugin_key => $sub_result) {
				$plugin_info = sirius_trust_eligibility_plugin_info($plugin_key);

				$paths = array();
				if ($plugin_info['callbacks']['settings']) { $paths["node/$policy_nid/sirius_trust_policy_benefits/$benefit_nid/$plugin_key"] = 'Settings'; }

				$tools = array();
				foreach ($paths as $path => $label) {
					if (drupal_valid_path($path)) {
						$tools[] = l($label, $path, array('attributes' => array('class' => array('tool-button'))));
					}
				}

				$class = 'eligible';

				$msg = $sub_result['msg'];
				if ($sub_result['success']) {
					if ($sub_result['warning']) {
						$class = 'warning';
						$eligible = 'Warning';
						$summary_warning = TRUE;
					} else {
						$class = 'green';
						$eligible = 'Yes';
					}
				} else {
					$class = 'red';
					$eligible = 'No';
					$summary_eligible = FALSE;
					$summary_failed[] = $plugin_info['name'];
				}
				if ($sub_result['tools']) { $tools = array_merge($tools, $sub_result['tools']); }

				$row = array();
				$row[] = '';
				$row[] = $plugin_info['name'];
				$row[] = $eligible;
				$row[] = $sub_result['msg'];
				$row[] = join('', $tools);
				$subrows[] = array('data' => $row, 'class' => array($class));
			}

			if ($summary_eligible) {
				if ($summary_warning) {
					$class = 'warning';
					$eligible = 'Warning';
					$msg = 'The member or dependent is eligible for this benefit, but is at risk of losing eligibility.';
				} else {
					$class = 'green';
					$eligible = 'Yes';
					$msg = 'The member or dependent is eligible for this benefit.';
				}
			} else {
				$class = 'red';
				$eligible = 'No';
				$msg = 'Failed tests: ' . join(', ', $summary_failed);
			}

			$paths = array(
				"node/$policy_nid/sirius_trust_policy_benefits/$benefit_nid" => 'Eligibility Settings',
			);
			$tools = array();
			foreach ($paths as $path => $label) {
				if (drupal_valid_path($path)) {
					$tools[] = l($label, $path, array('attributes' => array('class' => array('tool-button'))));
				}
			}

			$row = array();
			$row[] = $benefit_node->title;
			$row[] = 'OVERALL';
			$row[] = $eligible;
			$row[] = $msg;
			$row[] = join('', $tools);
			$rows[] = array('data' => $row, 'class' => array($class, 'summary'));

			foreach ($subrows as $row) { $rows[] = $row; }
		}

		$header = array(
			'Benefit',
			'Eligibility Test',
			'Result',
			'Notes',
			'Tools',
		);

		$form['eligibility_details'] = array(
			'#type' => 'fieldset',
			'#title' => 'Staff Only - Eligibility Details',
			'#collapsible' => TRUE,
			'#collapsed' => TRUE,
		);

		$form['eligibility_details']['details'] = array('#markup' => sirius_table_render($header, $rows, array('class' => array('sirius_table_align'))));
	}

	return sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_trust_election_wizard_dependent_existing_form_validate($form, &$form_state) {

	$info = $form_state['info'];
	$data = $form_state['data'];
	$current_step = $form_state['current_step'];
	$wizard_node = $form_state['wizard_node'];
	$relationship_nid = preg_replace('/dependent_existing_/', '', $current_step);

	// We don't care if they don't have prior coverage.
	if (!$form_state['values']['dependents']['existing'][$relationship_nid]['prior_coverage']) { return; }

	// We don't care if they are contiuing coverage
	if ($form_state['values']['dependents']['existing'][$relationship_nid]['active'] != 'No') { return; }

	// Process the file
	unset($form_state['storage']["file"]);

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
  	if ($form_state['values']['dependents']['existing'][$relationship_nid]['active'] == 'No') {
	  	if (!$data['dependents']['existing'][$relationship_nid]['file']) {
				form_set_error("file", t("Please upload a file."));
			}
		}
  }
}


function sirius_trust_election_wizard_dependent_existing_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$current_step = $form_state['current_step'];
	$wizard_node = $form_state['wizard_node'];
	$relationship_nid = preg_replace('/dependent_existing_/', '', $current_step);

	$old_file = $data['dependents']['existing'][$relationship_nid]['file'];
	$old_fid = $data['dependents']['existing'][$relationship_nid]['file']['fid'];
	$data['dependents']['existing'][$relationship_nid] = $form_state['values']['dependents']['existing'][$relationship_nid];

  $file = $form_state['storage']["file"];
  if ($file) {
		$file = (array) $file;
		$file['display'] = 1;
		unset($form_state['storage']["attach_$delta"]);
		unset($file['destination']);
		unset($file['uri']);

		// Remove the previous file
		$old_fid = $data['dependents']['existing'][$relationship_nid]['file']['fid'];
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
		$data['dependents']['existing'][$relationship_nid]['file'] = $file;
	} else {
		$data['dependents']['existing'][$relationship_nid]['file'] = $old_file;
	}

	sirius_trust_election_set_title($wizard_node);
	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => FALSE));
}

function sirius_trust_election_wizard_dependent_existing_render($info, &$data, $wizard_node, $current_step) {
	$relationship_nid = preg_replace('/dependent_existing_/', '', $current_step);
	if (!($relationship_nid > 0)) { return ''; }
	$relationship_node = node_load($relationship_nid);
	$dependent_contact_nid = $relationship_node->field_sirius_contact_alt['und'][0]['target_id'];
	if (!$dependent_contact_nid) { return ''; }

	// If they're not covered, go away
	if ($data['dependents']['existing'][$relationship_nid]['active'] != 'Yes') { return ''; }

	// Eligibility
	$eligibility_data_by_benefit = $data['dependents']['eligibility'][$relationship_nid];
	if (!$eligibility_data_by_benefit) { $eligibility_data_by_benefit = array(); }
	$at_least_one = $eligibility_data_by_benefit['at_least_one'];
	unset($eligibility_data_by_benefit['at_least_one']);

	$html = sirius_fakerender_field('Dependent Name:', sirius_node_title($dependent_contact_nid));
	$reltype_term = taxonomy_term_load($relationship_node->field_sirius_contact_reltype['und'][0]['tid']);
	if ($reltype_term) {
		$html .= sirius_fakerender_field('Relationship:', $reltype_term->field_sirius_name_alt['und'][0]['value']);
	}
	if ($eligibility_data_by_benefit) {
		foreach ($eligibility_data_by_benefit as $benefit_nid => $result) {
			$html .= sirius_fakerender_field(
				sirius_node_title($benefit_nid) . ': ',
				$result['success'] ? 'Yes' : 'No'
			);
		}
	}

	return $html;
}

