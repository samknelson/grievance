<?php

function sirius_node_tab_worker_industry_access($worker_node) {
	return node_access('view', $worker_node);
}

function sirius_node_tab_worker_industry($worker_node) {
	return drupal_get_form('sirius_node_tab_worker_industry_form', $worker_node);
}

function sirius_node_tab_worker_industry_form($form, &$form_state, $worker_node) {
	$form_state['worker_node'] = $worker_node;

	drupal_set_title($worker_node->title . ': Industries');

	$html = '';

	$industry_terms = sirius_taxonomy_load('sirius_industry', 'tid', 'full');
	$member_status_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');
	$member_status_options = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');

	foreach ($member_status_options as $member_status_tid => $name) {
		$member_status_term = $member_status_terms[$member_status_tid];
		$industry_tid = $member_status_term->field_sirius_industry['und'][0]['tid'];
		$industry_term = $industry_terms[$industry_tid];
		$member_status_options[$member_status_tid] = $name . ' (' . $industry_term->name . ')';
	}

	if ($worker_node->field_sirius_member_status['und']) {
		foreach ($worker_node->field_sirius_member_status['und'] as $tmp) {
			$member_status_tid = $tmp['tid'];
			$member_status_term = $member_status_terms[$member_status_tid];
			$industry_tid = $member_status_term->field_sirius_industry['und'][0]['tid'];
			$industry_term = $industry_terms[$industry_tid];
			$html .= '<li>';
			$html .= $industry_term->name . ': ' . $member_status_term->name;

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
			$job_node->field_sirius_industry['und'][0]['tid'] = $industry_tid;
			$member_status_tids = array_keys(sirius_worker_member_status_get_open_statuses($industry_tid, TRUE, sirius_get_special_tid('sirius_dispatch_job_type|OPEN')));
			foreach ($member_status_tids as $tid) {
				if ($tid) {
					$job_node->field_sirius_member_status['und'][] = array('tid' => $tid);
				}
			}
			$dispatchable = $workers = sirius_dispatch_job_get_dispatchable_workers_explain(
				$job_node, 
				$worker_node->nid,
				array(
					'ignore_skill' => TRUE,
					'ignore_department' => TRUE,
				)
			);

			$html .= '<br />';
			if ($dispatchable['success']) {
				$html .= 'Dispatch position: ' . $dispatchable['msg'];
			} else {
				$html .= 'Dispatch not available: ' . $dispatchable['msg'];
			}
			$html .= '</li>';
		}
	}
	if ($html) {
		$html = $worker_node->title . ' currently has the following industries:<ul>' . $html . '</ul>';
	} else {
		$html = $worker_node->title . ' does not currently have any industries registered.';
	}
	$form['intro'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="sirius_form_intro">' . $html . '</div>',
	);


	if (user_access('sirius dispatch staff')) {
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

	$worker_node = $form_state['worker_node'];
	$member_status_tid = $form_state['values']['member_status_tid'];
	$member_status_term = taxonomy_term_load($member_status_tid);

	sirius_worker_member_status_set(
		$worker_node, 
		$member_status_term, 
		$form_state['values']['action'], 
		$form_state['values']['reason'],
		$files
	);

	if ($form_state['values']['action']) {
		drupal_set_message(t("Success! Worker @worker has added @member_status.", array('@worker' => $worker_node->title, '@member_status' => $member_status_term->name)));
	} else {
		drupal_set_message(t("Success! Worker @worker has removed the @member_status.", array('@worker' => $worker_node->title, '@member_status' => $member_status_term->name)));
	}
}