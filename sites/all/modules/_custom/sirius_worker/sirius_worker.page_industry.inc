<?php

function sirius_node_tab_worker_industry_access($worker_node) {
	return node_access('view', $worker_node);
}

function sirius_node_tab_worker_industry($worker_node) {
	return drupal_get_form('sirius_node_tab_worker_industry_form', $worker_node);
}

function sirius_node_tab_worker_industry_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	$industry_terms = sirius_taxonomy_load('sirius_industry', 'tid', 'full');
	$member_status_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');
	$industry_member_status_map = array();
	foreach ($member_status_terms as $member_status_tid => $member_status_term) {
		$industry_tid = sirius_fieldval($member_status_term, 'field_sirius_industry', 'tid');
		$industry_member_status_map[$industry_tid][] = $member_status_tid;
		$industry_member_status_map[$member_status_tid] = $industry_tid;
	}

	$my_member_status_tids = sirius_fieldvals($worker_node, 'field_sirius_member_status', 'tid');
	$rows = array();
	foreach ($industry_terms as $industry_tid => $industry_term) {
		$row = array();
		$row[] = $industry_term->name;

		$str = '';
		$str2 = '';
		if ($my_member_status_tids) {

			$job_node = new stdClass();
			$job_node->title = "Dummy";
			$job_node->type = 'sirius_dispatch_job';
			$job_node->language = LANGUAGE_NONE;
			$job_node->uid = $user->uid;
			$job_node->status = 1;
			$job_node->promote = 0;
			$job_node->comment = 0;
			$job_node->field_grievance_shop['und'][0]['target_id'] = 0;
			$job_node->field_sirius_dispatch_job_type['und'][0]['tid'] = sirius_get_special_tid('sirius_dispatch_job_type|OPEN');
			$job_node->field_sirius_datetime['und'][0]['value'] = date('Y-m-d H:i:s');
			$job_node->field_sirius_datetime_created['und'][0]['value'] = date('Y-m-d H:i:s');
			$job_node->field_sirius_dispatch_job_status['und'][0]['value'] = 'Pending';

			foreach ($my_member_status_tids as $member_status_tid) {
				if ($industry_tid == $industry_member_status_map[$member_status_tid]) {
					if ($str) { $str .= ' / '; }
					if ($str2) { $str2 .= ' / '; }
					$str .= $member_status_terms[$member_status_tid]->name;

					$job_node->field_sirius_industry['und'][0]['tid'] = $industry_tid;
					$open_member_status_tids = array_keys(sirius_worker_member_status_get_open_statuses($industry_tid, TRUE, sirius_get_special_tid('sirius_dispatch_job_type|OPEN')));
					foreach ($open_member_status_tids as $tid) {
						if ($tid) {
							$job_node->field_sirius_member_status['und'][] = array('tid' => $tid);
						}
					}
					$dispatchable = sirius_dispatch_job_get_dispatchable_workers_explain(
						$job_node, 
						$worker_node->nid,
						array(
							'ignore_skill' => TRUE,
							'ignore_department' => TRUE,
						)
					);

					if ($dispatchable['success']) {
						$str2 = $dispatchable['msg'];
					} else {
						$str2 = 'Dispatch not available: ' . $dispatchable['msg'];
					}
				}
			}
		}
		if (!$str) { continue; }
		if (!$str2) { $str2 = t('Dispatch not available.'); }
		$row[] = $str;
		$row[] = $str2;

		$rows[] = $row;
	}

	$header = array(
		t('Industry'),
		t('Current Member Status'),
		t('Dispatch Position'),
	);


	$html = '<h2>' . t('Current Industries') . '</h2>';
	$html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));

	$member_status_options = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
	if (!user_access('sirius dispatch staff')) {
		$new_member_status_options = array();
		foreach ($member_status_options as $member_status_tid => $member_status_name) {
			$json = json_decode($member_status_terms[$member_status_tid]->field_sirius_json['und'][0]['value'], TRUE);
			if ($json['optin']['optin'] == 'Yes') {
				$new_member_status_options[$member_status_tid] = $member_status_name;
			}
		}
		$member_status_options = $new_member_status_options;
	}

	foreach ($member_status_options as $member_status_tid => $name) {
		$member_status_options[$member_status_tid] = $industry_terms[$industry_member_status_map[$member_status_tid]]->name . ': ' . $name;
	}

	$form['intro'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="sirius_form_intro">' . $html . '</div>',
	);

	if ($member_status_options) {
		$form['member_status_tid'] = array(
			'#type' => 'select',
			'#title' => 'Member Status',
			'#options' => $member_status_options,
			'#required' => TRUE,
		);

		$form['action'] = array(
			'#type' => 'select',
			'#title' => 'Action',
			'#required' => TRUE,
			'#options' => array(
				NULL => t('-- Select --'),
				TRUE => t('Add this industry'),
				FALSE => t('Remove this industry'),
			),
		);

		if (user_access('sirius dispatch staff')) {

			$form['member_status_tid']['#description'] .= t('Warning: Adding a member status will remove any other member statuses in the same industry.');

			$form['reason'] = array(
				'#type' => 'textarea',
				'#title' => t('Reason'),
				'#required' => TRUE,
			);

			for ($delta=0; $delta<3; ++$delta) {
				$form["attach_$delta"] = array(
					'#type' => 'file',
					'#title' => t('Attachment #') . ($delta + 1),
				);
			}

		}

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => 'Submit',
		);
	}

	$form += sirius_log_render_form($worker_node->nid, 'worker:member_status');

	return $form;
}

function sirius_node_tab_worker_industry_form_validate($form, &$form_state) {
	$info = field_info_instance('node', 'field_sirius_attachments', 'sirius_log');
	$extensions = $info['settings']['file_extensions'];
	for ($delta = 0; $delta < 3; ++$delta) {
	  $file = file_save_upload("attach_$delta", array(
	    'file_validate_extensions' => array($extensions),
	  ));
	  if ($file) {
	    $name = rand();
	    if ($file = file_move($file, 'private://tmp/' . rand())) {
	      $form_state['storage']["attach_$delta"] = $file;
	    }
	    else {
	      form_set_error("attach_$delta", t("Failed to write the uploaded file to the site's file folder."));
	    }
	  }
	}
}

function sirius_node_tab_worker_industry_form_submit($form, &$form_state) {
	$worker_nid = $form_state['worker_nid'];
	$worker_node = node_load($worker_nid);
	$my_member_status_tids = sirius_fieldvals($worker_node, 'field_sirius_member_status', 'tid');

	$industry_terms = sirius_taxonomy_load('sirius_industry', 'tid', 'full');
	$member_status_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');
	$industry_member_status_map = array();
	foreach ($member_status_terms as $member_status_tid => $member_status_term) {
		$industry_tid = sirius_fieldval($member_status_term, 'field_sirius_industry', 'tid');
		$industry_member_status_map[$industry_tid][] = $member_status_tid;
		$industry_member_status_map[$member_status_tid] = $industry_tid;
	}

	$files = array();
	for ($delta = 0; $delta < 3; ++$delta) {
	  $file = $form_state['storage']["attach_$delta"];
	  if ($file) {
  		$file = (array) $file;
  		$file['display'] = 1;
			unset($form_state['storage']["attach_$delta"]);
  		unset($file['destination']);
  		unset($file['uri']);
  		$files[] = $file;
  	}
  }

	$new_member_status_tid = $form_state['values']['member_status_tid'];
	$new_member_status_term = taxonomy_term_load($new_member_status_tid);
	$new_industry_tid = $industry_member_status_map[$new_member_status_tid];
	$action = $form_state['values']['action'];
	$reason = $form_state['values']['reason'];
	if (!$reason) { $reason = 'Worker opt-in or opt-out.'; }

	if ($action && in_array($new_member_status_tid, $my_member_status_tids)) {
		drupal_set_message(t('The worker already has that member status.'), 'error');
		return;
	}
	if (!$action && !in_array($new_member_status_tid, $my_member_status_tids)) {
		drupal_set_message(t('The worker does not have that member status.'), 'error');
		return;
	}

	if ($action) {
		// Clear any other member statuses from this industry.
		foreach ($my_member_status_tids as $clear_member_status_tid) {
			if ($clear_member_status_tid == $new_member_status_tid) { continue; }
			if ($industry_member_status_map[$clear_member_status_tid] != $new_industry_tid) { continue; }
			$clear_member_status_term = $member_status_terms[$clear_member_status_tid];

			if (!user_access('sirius dispatch staff')) {
				drupal_set_message(t('You already have a member status @status in this industry, and cannot add a new one.', array('@status' => $clear_member_status_term->name)), 'error');
				return;
			}

			/*
			sirius_worker_member_status_set(
				$worker_node, 
				$clear_member_status_term, 
				FALSE, 
				$reason,
				array()
			);

			drupal_set_message(t('Removed member status @status.', array('@status' => $clear_member_status_term->name)), 'error');
			*/
		}
	}

	sirius_worker_member_status_set(
		$worker_node, 
		$new_member_status_term, 
		$action, 
		$reason,
		$files
	);

	if ($form_state['values']['action']) {
		drupal_set_message(t("Success! Worker @worker has added @member_status.", array('@worker' => $worker_node->title, '@member_status' => $new_member_status_term->name)));
	} else {
		drupal_set_message(t("Success! Worker @worker has removed the @member_status.", array('@worker' => $worker_node->title, '@member_status' => $new_member_status_term->name)));
	}
}