<?php

function sirius_node_tab_dispatch_view($dispatch_node) {
	if ($css = sirius_domain_variable_get('sirius_dispatch_view_css', '')) {
		drupal_add_css($css, 'inline');
	}

	// All users can "access" this page. But only certain users can access the details.
	if (sirius_node_tab_dispatch_worker_view_access_details($dispatch_node)) {
		return drupal_get_form('sirius_node_tab_dispatch_worker_view_form', $dispatch_node);
	} 

	return sirius_node_tab_dispatch_worker_view_verify_only($dispatch_node);
}

function sirius_node_tab_dispatch_worker_view_access_details($dispatch_node) {
	global $user;

	// Admins can get in
	if (user_access('sirius dispatch staff')) { return TRUE; }

	// Employers can get in. Though they might not see everything.
	if (user_access('sirius dispatch employer')) { 
		$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
		$employer_node = sirius_employer_get($user->uid);
		if ($employer_node->nid == $job_node->field_grievance_shop['und'][0]['target_id']) { return TRUE; }
	}

	// Access token gets us in
	if (sirius_aat_check($dispatch_node)) { return TRUE; }

	// This user must be a worker
	if (!$user) { return FALSE; }
	if (!$user->uid) { return FALSE; }
	$user_full = user_load($user->uid);
	$worker_nid = $user_full->field_sirius_worker['und'][0]['target_id'];
	if (!$worker_nid) { return FALSE; }

	// The worker must be associated with this dispatch, and it must have a status besides "requested"
	if ($worker_nid == $dispatch_node->field_sirius_worker['und'][0]['target_id'] && $dispatch_node->field_sirius_dispatch_status['und'][0]['value'] != 'requested') { return TRUE; }

	return FALSE;
}


/* ***************************************
 * Verify Only
 * ***************************************/

function sirius_node_tab_dispatch_worker_view_verify_only($dispatch_node) {
	$status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);

	drupal_set_title("Dispatch Status: $status");

	$form['qr'] = array(
		'#type' => 'markup',
		'#markup' => sirius_qr_url(),
	);

	$html .= '<section class="field field-name-dispatch-valid field-label-inline clearfix view-mode-full"><h2 class="field-label">';
	$html .= 'This dispatch is:&nbsp;'; 
	$html .= '</h2><div class="field-items"><div class="field-item">' . 'Valid' . '</div></div></section>';

	$html .= '<section class="field field-name-dispatch-id field-label-inline clearfix view-mode-full"><h2 class="field-label">';
	$html .= 'Dispatch ID:&nbsp;'; 
	$html .= '</h2><div class="field-items"><div class="field-item">' . $dispatch_node->nid . '</div></div></section>';

	$html .= '<section class="field field-name-dispatch-' . $status . ' field-label-inline clearfix view-mode-full"><h2 class="field-label">';
	$html .= 'Dispatch Status:&nbsp;'; 
	$html .= '</h2><div class="field-items"><div class="field-item">' . $status . '</div></div></section>';

	$html .= '<section class="field field-name-dispatch-signin field-label-inline clearfix view-mode-full"><h2 class="field-label">';
	$html .= "To see the dispatch details, please " . l('sign in.', 'user/login', array('query' => array('destination' => current_path())));
	$html .= '</h2></section>';

	return $html;
}

/* ***************************************
 * Full
 * ***************************************/

function sirius_node_tab_dispatch_worker_view_form($form, &$form_state, $dispatch_node) {
	drupal_set_title("Dispatch #" . $dispatch_node->nid);
	$form_state['dispatch_node'] = $dispatch_node;
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$job_policy = sirius_dispatch_job_policy($job_node);
	$worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);
	$facility_node = node_load($job_node->field_sirius_dispatch_facility['und'][0]['target_id']);
	$employer_node = node_load($job_node->field_grievance_shop['und'][0]['target_id']);
	$employer_json = json_decode($employer_node->field_sirius_json['und'][0]['value'], TRUE);

	// Get our permission type. We might have multiple. We're a worker if we either are logged in, or if we have an access token.
	if (user_access('sirius dispatch employer')) { $is_employer = TRUE; }
	if (user_access('sirius dispatch staff')) { $is_staff = TRUE; }
	if (user_access('sirius dispatch worker')) { $is_worker = TRUE; }
	if (sirius_aat_check($dispatch_node)) { $is_worker = TRUE; }

	if (sirius_domain_variable_get('sirius_dispatch_view_print_header', '')) { $form['print_header'] = array('#markup' => '<div class="sirius_dispatch_view_print_header">' . sirius_domain_variable_get('sirius_dispatch_view_print_header', '') . '</div>'); }
	if (sirius_domain_variable_get('sirius_dispatch_view_screen_header', '')) { $form['screen_header'] = array('#markup' => '<div class="sirius_dispatch_view_screen_header">' . sirius_domain_variable_get('sirius_dispatch_view_screen_header', '') . '</div>'); }

	$actions = array(NULL => t('-- Select --'));
	if ($is_staff || $is_worker) {
		// @todo: Can only accept or decline if you are not UEB!
		if (sirius_dispatch_accept_possible($dispatch_node)['success']) { $actions['accept'] = t('Accept'); }
		if (sirius_dispatch_decline_possible($dispatch_node)['success']) { $actions['decline'] = t('Decline'); }
	}

	if ($is_staff || $is_employer) {
		if (sirius_dispatch_terminate_possible($dispatch_node)['success']) { $actions['terminate'] = t('Layoff'); }
	}

	if ($is_staff || $is_worker) {
		if (sirius_dispatch_resign_possible($dispatch_node)['success']) { $actions['resign'] = t('Voluntary layoff'); }
	}

	if ($is_staff || $is_worker) {
		if (sirius_dispatch_vlri_possible($dispatch_node)['success']) { $actions['vlri'] = t('Employer Initiated Layoff'); }
	}

	$is_accepted = FALSE;
	$is_accepted = in_array($dispatch_node->field_sirius_dispatch_status['und'][0]['value'], array('accepted', 'accepted_secondary', 'resigned', 'vlri', 'terminated'));

	if (count($actions) > 1) {
		$form['action'] = array(
			'#title' => t('Action'),
			'#type' => 'select',
			'#options' => $actions,
			'#prefix' => '<div class="sirius_noprint">',
			'#required' => TRUE,
		);

		if ($is_staff) {
			$prefix = '<div class="sirius_field_intro">';
			$prefix .= "<p>Workers who are laid off within 24 hours automatically keep their senioriy. Select \"Yes\" to preserve seniority even if the layoff is more than 24 hours later than the report time.</p>";
			$availdate = $worker_node->field_sirius_dispatch_availdate['und'][0]['value'];
			if ($availdate) {
				$availdate = date('m/d/Y h:i a', strtotime($availdate . ' GMT'));
				$prefix .= '<p>' . t('The worker\'s <em>previous</em> seniority date was <strong>@date</strong>. ', array('@date' => $availdate)) . '</p>';
			}
			$prefix .= '</div>';

			$form['keep_seniority'] = array(
				'#title' => t('Keep Seniority?'),
				'#type' => 'select',
				'#options' => array(
					NULL => t('-- Select --'),
					'No' => t('No'),
					'Yes' => t('Yes'),
				),
				'#default_value' => 'No',
				'#states' => array(
					'visible' => array(
						array(':input[name="action"]' => array('value' => 'terminate')),
						array(':input[name="action"]' => array('value' => 'resign')),
						array(':input[name="action"]' => array('value' => 'vlri')),
					)
				),
				'#field_prefix' => $prefix,
			);
		}

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => 'Go',
			'#suffix' => '</div>',
		);
	}


	$form['dispatch_section_open'] = array('#markup' => '<div class="sirius_dispatch_view_dispatch_section">');
	$form['dispatch_section_qr'] = array('#markup' => sirius_qr_url());
	$form['dispatch_section_a'] = array('#markup' => sirius_dispatch_view_render_field($dispatch_node, 'field_sirius_dispatch_status', t('Dispatch Status')));
	$form['dispatch_section_b'] = array('#markup' => sirius_dispatch_view_render_field($dispatch_node, 'field_sirius_id', t('Request ID')));
	$form['dispatch_section_c'] = array('#markup' => sirius_dispatch_view_render_field($dispatch_node, 'field_sirius_dispatch_cbn', t('Call by Name?')));
	$form['dispatch_section_d'] = array('#markup' => sirius_dispatch_view_render_field($dispatch_node, 'field_sirius_payrate', t('Pay Rate')));

	// @TODO: Broken now
	$account_options = sirius_ledger_account_list();
	$account_nids = array_filter(array_values(sirius_domain_variable_get('sirius_dispatch_view_accounts', array())));
	$policy_node = sirius_ledger_policy_lookup($worker_node);
	if ($policy_node) {
		if ($account_nids) {
			$schedules = array('month' => 'Monthly', 'semimonth' => 'Semi-Monthly', 'hour' => 'Hourly');
			foreach ($schedules as $schedule_key => $schedule_name) {
				$policy_json = sirius_jsonfield_util_node_json_get($policy_node);
				$rates = sirius_ledger_policy_lookup_rates($worker_node, $policy_node, $schedule_key);
				foreach ($rates as $account_nid => $rate_record) {
					if ($rate_record['rate'] && in_array($account_nid, $account_nids)) {
						$title = $account_options[$account_nid] . ':';
						$value = '$' . number_format($rate_record['rate'], 2) . ' ' . $schedule_name;
						$form['dispatch_section_' . $account_nid . '_' . $schedule_key] = array('#markup' => sirius_fakerender_field($title, $value));
					}
				}
				// dpm($policy_node);
				// dpm($rates);
			}
		}
	}

	$form['dispatch_section_e'] = array('#markup' => sirius_fakerender_field('Report #', $dispatch_node->nid));
	$form['dispatch_section_close'] = array('#markup' => '</div>');

	$form['worker_section_open'] = array('#markup' => '<div class="sirius_dispatch_view_worker_section">');
	$form['worker_section_g'] = array('#markup' => sirius_dispatch_view_render_field($worker_node, 'field_sirius_headshot'));
	$form['worker_section_a'] = array('#markup' => sirius_dispatch_view_render_field($worker_node, 'field_sirius_name', t('Worker Name')));
	$form['worker_section_b'] = array('#markup' => sirius_dispatch_view_render_field($worker_node, 'field_sirius_phone', t('Phone')));
	$form['worker_section_c'] = array('#markup' => sirius_dispatch_view_render_field($worker_node, 'field_sirius_email', t('Email')));
	$form['worker_section_d'] = array('#markup' => sirius_dispatch_view_render_field($worker_node, 'field_sirius_id', t('Dispatch ID')));
	$form['worker_section_e'] = array('#markup' => sirius_dispatch_view_render_field($worker_node, 'field_sirius_id2', t('Titan ID')));
	$form['worker_section_f'] = array('#markup' => sirius_dispatch_view_render_field($worker_node, 'field_sirius_ssn', t('SSN')));

	// Find the member status corresponding to the dispatch.
	// @todo: This should really be a function, sirius_dispatch_find_ms()
	$job_industry_tids = sirius_fieldvals($job_node, 'field_sirius_industry', 'tid');
	$ms_tids = sirius_fieldvals($worker_node, 'field_sirius_member_status', 'tid');
	$ms_terms = taxonomy_term_load_multiple($ms_tids);
	foreach ($ms_terms as $probe_ms_tid => $probe_ms_term) {
		$status_industry_tids = sirius_fieldvals($probe_ms_term, 'field_sirius_industry', 'tid');
		if (array_intersect($job_industry_tids, $status_industry_tids)) {
			$ms_tid = $probe_ms_tid;
			$ms_term = $ms_terms[$ms_tid];
		}
	}
	$form['worker_section_h'] = array('#markup' => sirius_fakerender_field('Member Status:', $ms_term->name));

	// Find the level
	$levels = sirius_training_levels_fetch_worker($worker_node);
	if ($levels && $levels[$ms_tid]) {
		$level_code = $levels[$ms_tid]['code'];
		$level_info = sirius_training_levels_fetch_record($ms_tid, $level_code);
		if ($level_info) {
			$form['worker_section_h1'] = array('#markup' => sirius_fakerender_field('Level:', $level_info['title']));
		}
	}

	$contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);
	$contact_json = sirius_jsonfield_util_node_json_get($contact_node);
	$contact_results = sirius_jsonfield_util_results_get($contact_json, 'default');
	sirius_jsonfield_util_add_node_fields($contact_node, $contact_definition_form, $contact_results);
	$contact_definition_json = sirius_contact_definition();
	$contact_definition_form = sirius_jsonfield_util_form_get($contact_definition_json, 'default');
	$display_fields = sirius_contact_definition_display_get($contact_definition_json, 'dispatch-default');
	if ($display_fields) {
		$form['worker_section_custom'] = array('#markup' => sirius_jsonfield_util_render_values($contact_definition_form, $contact_results, $display_fields, array('show_title' => TRUE)));
	}

	$form['worker_section_close'] = array('#markup' => '</div>');


	$form['job_section_open'] = array('#markup' => '<div class="sirius_dispatch_view_job_section">');

	$form['job_employer_section_open'] = array('#markup' => '<div class="sirius_dispatch_view_job_employer_section">');


	$address_html = $employer_node->title;
	$contact_node = sirius_employer_contact_fetch($employer_node->nid, sirius_domain_variable_get('sirius_dispatch_view_contact_type', NULL));
	if ($contact_node) {
		$address_html .= '<br />' . $contact_node->field_grievance_co_address['und'][0]['value'];
		if ($contact_node->field_grievance_co_address_2) { $address_html .= '<br />' . $contact_node->field_grievance_co_address_2['und'][0]['value']; }
		$address_html .= '<br />' . $contact_node->field_grievance_co_city['und'][0]['value'] . ', ' . $contact_node->field_grievance_co_state['und'][0]['value'] . ' ' . $contact_node->field_grievance_co_zip['und'][0]['value'];
		$html .= $address_html;
	}
	$form['job_employer_section_a'] = array('#markup' => sirius_fakerender_field('Employer:', $address_html));
	$form['job_employer_section_close'] = array('#markup' => '</div>');

	$form['job_location_section_open'] = array('#markup' => '<div class="sirius_dispatch_view_job_location_section">');

	$job_tags = sirius_fieldvals($job_node, 'field_sirius_dispatch_job_tags', 'tid');

	if (sirius_domain_variable_get('sirius_dispatch_view_show_report_date', 'Yes') == 'Yes') {
		$form['job_location_section_a'] = array('#markup' => sirius_dispatch_view_render_field($job_node, 'field_sirius_datetime'));
		$form['job_location_section_a0'] = array('#markup' => sirius_dispatch_view_render_field($job_node, 'field_sirius_datetime_completed'));
	}

	if (sirius_domain_variable_get('sirius_dispatch_view_show_create_date', 'No') == 'Yes') {
		$form['job_location_section_a1'] = array('#markup' => sirius_dispatch_view_render_field($dispatch_node, 'field_sirius_datetime_created'));
	}

	if ($dispatch_node->field_sirius_dispatch_status['und'][0]['value'] == 'accepted_secondary' && $job_policy['workers']['must_be_rollover'] == 'Yes') {
		$rollover_log_nid = sirius_dispatch_rollover_nid($employer_node->nid, $worker_node->nid, sirius_date_ts($dispatch_node, 'field_sirius_datetime'));
		$rollover_log_node = node_load($rollover_log_nid);
		if ($rollover_log_node) {
			$json = json_decode($rollover_log_node->field_sirius_json['und'][0]['value'], TRUE);
			$rollover_end_ts = strtotime($json['end_date']);
			if ($rollover_end_ts > 1) {
				$rollover_end_date = date('l, F j, Y h:i A', $rollover_end_ts);
				$stupid_html = '<section class="field field-name-datetime field-label-inline clearfix view-mode-full"><h2 class="field-label">Rollover End Date</h2><div class="field-items"><div class="field-item">' . $rollover_end_date . '</div></div></section>';
				$form['job_location_section_aa'] = array('#markup' => $stupid_html);
			}
		}
	}

	if ($is_accepted) {
		$form['job_location_section_b'] = array('#markup' => sirius_dispatch_view_render_field($job_node, 'field_sirius_dispatch_facility'));
		$form['job_location_section_c'] = array('#markup' => sirius_dispatch_view_render_field($facility_node, 'field_sirius_address'));
		$form['job_location_section_d'] = array('#markup' => sirius_dispatch_view_render_field($job_node, 'field_sirius_dispatch_hall'));
		$form['job_location_section_e'] = array('#markup' => sirius_dispatch_view_render_field($job_node, 'field_sirius_address_notes'));
		$form['job_location_section_f'] = array('#markup' => sirius_dispatch_view_render_field($job_node, 'field_sirius_address_parking'));
	}
	$form['job_location_section_close'] = array('#markup' => '</div>');

	$form['job_detail_section_open'] = array('#markup' => '<div class="sirius_dispatch_view_job_detail_section">');
	$form['job_detail_section_a'] = array('#markup' => sirius_dispatch_view_render_field($job_node, 'field_sirius_skill'));
	$form['job_detail_section_b'] = array('#markup' => sirius_dispatch_view_render_field($job_node, 'field_grievance_departments'));
	$form['job_detail_section_c'] = array('#markup' => sirius_dispatch_view_render_field($job_node, 'field_sirius_dispatch_job_type'));
	$form['job_detail_section_d'] = array('#markup' => sirius_dispatch_view_render_field($job_node, 'field_sirius_notes'));
	if ($is_accepted) {
		$form['job_detail_section_e'] = array('#markup' => sirius_dispatch_view_render_field($job_node, 'field_sirius_attachments', 'Job Attachments'));
		if ($employer_json['dispatch']['job_files']) {
			$fids = array();
			foreach ($employer_json['dispatch']['job_files'] as $entry) { $fids[] = $entry['fid']; }
			if ($fids && $employer_node->field_sirius_attachments['und']) {
				$new_attachments = array();
				foreach ($employer_node->field_sirius_attachments['und'] as $tmp) {
					if (in_array($tmp['fid'], $fids)) {
						$new_attachments[] = $tmp;
					}
				}
				$employer_node->field_sirius_attachments['und'] = $new_attachments;
				$form['job_detail_section_f'] = array('#markup' => sirius_dispatch_view_render_field($employer_node, 'field_sirius_attachments', 'Employer Attachments'));
			}
		}
	}
	$form['job_location_section_close'] = array('#markup' => '</div>');

	$form['job_section_close'] = array('#markup' => '</div>');

	if (sirius_domain_variable_get('sirius_dispatch_view_print_footer', '')) { $form['print_footer'] = array('#markup' => '<div class="sirius_dispatch_view_print_footer">' . sirius_domain_variable_get('sirius_dispatch_view_print_footer', '') . '</div>'); }
	if (sirius_domain_variable_get('sirius_dispatch_view_screen_footer', '')) { $form['screen_footer'] = array('#markup' => '<div class="sirius_dispatch_view_screen_footer">' . sirius_domain_variable_get('sirius_dispatch_view_screen_footer', '') . '</div>'); }

	return $form;
}

function sirius_node_tab_dispatch_worker_view_form_submit($form, &$form_state) {
	$dispatch_node = $form_state['dispatch_node'];
	$action = $form_state['values']['action'];
	$notes = $form_state['values']['notes'];

	if ($form_state['values']['keep_seniority'] == 'Yes') {
		$worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);
		$json = sirius_json_get($worker_node);
		$json['dispatch']['keep_seniority'] = 'Yes';
		sirius_json_set($worker_node, $json);
		node_save($worker_node);
	}

	if ($action == 'accept') { $result = sirius_dispatch_accept($dispatch_node); }
	if ($action == 'decline') { $result = sirius_dispatch_decline($dispatch_node); }
	if ($action == 'terminate') { $result = sirius_dispatch_terminate($dispatch_node); }
	if ($action == 'resign') { $result = sirius_dispatch_resign($dispatch_node); }
	if ($action == 'vlri') { $result = sirius_dispatch_vlri($dispatch_node); }

	if ($result['success']) {
		drupal_set_message($result['msg']);
	} else {
		drupal_set_message(t('Error: ') . $result['msg'], 'error');
	}
}


function sirius_dispatch_view_render_field($node, $field_name, $title = '') {
	if (!$node) { return ''; }
	$mode = 'full';
	if ($field_name == 'field_sirius_ssn') { $mode = 'sirius_worker'; }
	$renderable = field_view_field('node', $node, $field_name, $mode);
	if ($title) {
		$renderable['#title'] = $title;
	}
	return render($renderable);
}

///////////////////////////////////////////////////////////////////
// Admin
///////////////////////////////////////////////////////////////////

function sirius_dispatch_view_admin() {
  $form = array();

  $var = sirius_domain_variable_name('sirius_dispatch_view_show_report_date');
  $form[$var] = array(
    '#type' => 'select',
    '#title' => t('Show Report Date?'),
    '#default_value' => variable_get($var, 'Yes'),
    '#options' => array(
    	NULL => t('-- Select --'),
    	'Yes' => t('Yes'),
    	'No' => t('No'),
    ),
  );

  $var = sirius_domain_variable_name('sirius_dispatch_view_show_create_date');
  $form[$var] = array(
    '#type' => 'select',
    '#title' => t('Show Create Date?'),
    '#default_value' => variable_get($var, 'No'),
    '#options' => array(
    	NULL => t('-- Select --'),
    	'Yes' => t('Yes'),
    	'No' => t('No'),
    ),
  );

  $var = sirius_domain_variable_name('sirius_dispatch_view_print_header');
  $form[$var] = array(
    '#type' => 'textarea',
    '#title' => t('Dispatch View Print Header'),
    '#default_value' => variable_get($var, ''),
  );

  $var = sirius_domain_variable_name('sirius_dispatch_view_screen_header');
  $form[$var] = array(
    '#type' => 'textarea',
    '#title' => t('Dispatch View Screen Header'),
    '#default_value' => variable_get($var, ''),
  );

  $var = sirius_domain_variable_name('sirius_dispatch_view_print_footer');
  $form[$var] = array(
    '#type' => 'textarea',
    '#title' => t('Dispatch View Print Footer'),
    '#default_value' => variable_get($var, ''),
  );

  $var = sirius_domain_variable_name('sirius_dispatch_view_screen_footer');
  $form[$var] = array(
    '#type' => 'textarea',
    '#title' => t('Dispatch View Screen Footer'),
    '#default_value' => variable_get($var, ''),
  );

  $var = sirius_domain_variable_name('sirius_dispatch_view_css');
  $form[$var] = array(
    '#type' => 'textarea',
    '#title' => t('Dispatch View CSS'),
    '#default_value' => variable_get($var, ''),
  );

  $contact_type_options = sirius_taxonomy_load('grievance_contact_types', 'tid', 'name', TRUE);
  if (count($contact_type_options) > 1) {
	  $var = sirius_domain_variable_name('sirius_dispatch_view_contact_type');
	  $form[$var] = array(
	    '#type' => 'select',
	    '#title' => t('Contact Type for Address'),
	    '#description' => t('If you wish to show an employer contact at the bottom of the dispatch, select which contact type should be shown'),
	    '#default_value' => variable_get($var, NULL),
	    '#options' => $contact_type_options,
	  );
	}

	$accounts = sirius_ledger_account_list();
	if ($accounts) {
	  $var = sirius_domain_variable_name('sirius_dispatch_view_accounts');
	  $form[$var] = array(
	    '#type' => 'checkboxes',
	    '#title' => t('Accounts'),
	    '#description' => t('If you wish to show fund rates for certain accounts, select them here.'),
	    '#default_value' => variable_get($var, NULL),
	    '#options' => $accounts,
	    '#checkall' => TRUE,
	  );
	}

  return system_settings_form($form);
}