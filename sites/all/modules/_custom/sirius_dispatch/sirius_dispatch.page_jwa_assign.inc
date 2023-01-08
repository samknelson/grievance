<?php

function sirius_dispatch_page_jwa_assign() {
	return drupal_get_form('sirius_dispatch_page_jwa_assign_form');
}

function sirius_dispatch_page_jwa_assign_form($form, &$form_state) {
	sirius_require('sirius_dispatch', 'sirius_dispatch.jwa.inc');

	$prefs = sirius_dispatch_jwa_prefs();
	if (!$prefs) { drupal_set_message(t('Please set your Job / Worker Assign Preferences.'), 'warning'); }

	$form['#tree'] = TRUE;

	// Get our lookups
	$employers_lookup = sirius_employer_list();
  $all_levels = sirius_training_levels_fetch_all();
  $ms_lookup = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
  $ms_level_lookup = sirius_ledger_ms_level_options();
	$skill_lookup = sirius_taxonomy_load('sirius_skill', 'tid', 'name');

	// Fetch the jobs, roster, and workers
	$job_nodes = sirius_dispatch_jwa_jobs($prefs);
	$roster = sirius_dispatch_jwa_roster($prefs, array_keys($job_nodes));
	$workers = sirius_dispatch_jwa_workers($prefs);

	if (!$job_nodes) { drupal_set_message(t('There are no jobs matching the criteria set in your preferences.'), 'warning'); }
	if (!$workers) { drupal_set_message(t('There are no workers matching the criteria set in your preferences.'), 'warning'); }

	// Build our output
  drupal_add_library('system', 'ui.accordion');
  drupal_add_js('jQuery(document).ready(function () { jQuery("#sirius_accordion").accordion({active: false, collapsible: true, autoehight: true, heightStyle: "content"}); });', 'inline');
	drupal_add_library('system', 'ui.tooltip');
	drupal_add_js(drupal_get_path('module', 'sirius_dispatch') . '/js/sirius_dispatch.jwa.js', 'file');

	$form['accordion_open'] = array('#markup' => '<div id="sirius_accordion">');

  //
  // JOBS
  //

	$html = '';
	foreach ($job_nodes as $job_nid => $job_node) {
		$count = $job_node->field_sirius_count['und'][0]['value']; if (!$count) { $count = 0; }
		$notified = $job_node->field_sirius_count_notified['und'][0]['value']; if (!$notified) { $notified = 0; }
		$yes = $job_node->field_sirius_count_yes['und'][0]['value']; if (!$yes) { $yes = 0; }
		$no = $job_node->field_sirius_count_no['und'][0]['value']; if (!$no) { $no = 0; }
		$tools = l(t('Manage'), "node/$job_nid/sirius_dispatch_job_manage", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));

		$title = '<strong>' . $employers_lookup[$job_node->field_grievance_shop['und'][0]['target_id']] . '</strong><br />';
		$title .= sirius_date_date($job_node, 'field_sirius_datetime') . ', ';
		$title .= $job_node->field_sirius_dispatch_job_status['und'][0]['value'];


		$facility_title = sirius_node_title($job_node->field_sirius_dispatch_facility['und'][0]['target_id']);
		$hall_title = sirius_node_title($job_node->field_sirius_dispatch_hall['und'][0]['target_id']);
		$notes_title = $job_node->field_sirius_address_notes['und'][0]['value'];

		$skill_titles = array();
		if ($job_node->field_sirius_skill['und']) {
			foreach ($job_node->field_sirius_skill['und'] as $delta => $record) {
				$skill_titles[] = $skill_lookup[$record['tid']];
			}
		}
		if ($facility_title || $hall_title || $notes_title || $skill_titles) {
			if ($facility_title) { $title .= " / $facility_title"; }
			if ($hall_title) { $title .= " $hall_title"; }
			if ($notes_title) { $title .= " $notes_title"; }
			if ($skill_titles) { $title .= ' / ' . join(' / ', $skill_titles); }
		}

		$title .= '<br />';
		$title .= "Need: <strong>$count</strong> / Notified: <strong>$notified</strong> / Yes: <strong>$yes</strong> / No: $no";

		$html .= '<div class="sirius_accordion_label">' . $title . '</div><div class="sirius_accordion_body">';

		$html .= l(t("Select"), "<front>", array('attributes' => array('target' => '_blank', 'data-job-nid' => $job_nid, 'class' => array('tool-button', 'sirius_job_nid_set'))));
		$html .= ' ';
		$html .= l(t("Manage"), "node/$job_nid/sirius_dispatch_job_manage", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));

		if ($roster[$job_nid]) {
			$header = array(t('Worker'), t('Status'), t('Tools'));
			$rows = array();
			foreach ($roster[$job_nid] as $hr) {
				$tools = l(t('Dispatch'), "node/$hr[dispatch_nid]/sirius_dispatch_manage", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
				$tools .= l(t('Worker'), "node/$hr[worker_nid]", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));

				$row = array();
				$row[] = $hr['worker_name'] . '<br />' . $hr['sirius_id'];
				$row[] = $hr['dispatch_status'];
				$row[] = $tools;
				$rows[] = $row;
			}
			$html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
		} else {
			$html .= '<br /><br /><em>' . t('No workers dispatched.') . '</em>';
		}
		$html .= '</div>';
	}
	if ($html) { $form['job_html'] = array('#markup' => $html); }

	$form['accordion_close'] = array('#markup' => '</div>');

	if ($job_nodes) {
	  $form['submit'] = array(
	  	'#type' => 'submit',
	  	'#value' => t('Dispatch the selected workers to the selected job'),
	  );
	}

	//
  // WORKERS
  //

  $header = array(
  	t('Worker'),
  	t('ID'),
  	t('Seniority'),
  	t('Member Status'),
  	t('Dispatch Status'),
  	t('EBA'),
  	t('Employer'),
  	t('Hours'),
  	t('Worker'),
  );

  $rows = array();
  $today_date = date('Y-m-d 00:00:00', strtotime('yesterday'));

	foreach ($workers as $worker_nid => $hr) {
  	$employer_name = $employers_lookup[$hr['employer_nid']];
  	if (!$employer_name) { $employer_name = 'AVAILABLE'; }
 		$tools = l(t('Worker'), "node/$worker_nid", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
 		// $tools .= l(t('Hours'), "node/$worker_nid/sirius_worker_hours", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
		$tools .= l(t('Quick Log'), "sirius/modal/$worker_nid/quicklog", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));

 		if ($hr['dispatches']) {
 			foreach ($hr['dispatches'] as $dispatch) {
		 		$tools .= ' ';
		 		$tools .= l($dispatch['dispatch_status'], "node/$dispatch[dispatch_nid]/sirius_dispatch_manage", array('attributes' => array('target' => '_blank', 'class' => array('tool-button')))) . ' ';
 			}
 		}

 		$hours = sirius_dispatch_jwa_hours_render($prefs, $hr);

		if ($hr['eba_name'] == 'Yes') {	
			$ebas = explode('|', $hr['eba_dates']);
			$ebas_render = array();
			foreach ($ebas as $delta => $eba) {
				if ($eba >= $today_date) {
					$ebas_render[] = date('D,\&\n\b\s\p\;M\&\n\b\s\p\;j', strtotime($eba));
				}
			}
			$eba_render = join('<br />', $ebas_render);
		} else {
			$eba_render = '';
		}

  	$dispatch_status_render = $hr['dispatch_status_name'];
  	$schedule = sirius_worker_schedule($worker_nid);
  	if ($schedule) { $dispatch_status_render .= sirius_worker_schedule_render($schedule, 'tiny'); }

   	$row = array();
		$row[] = $hr['worker_name'];
		$row[] = $hr['sirius_id'];
		$row[] = $hr['availdate'];
		$row[] = $hr['ms_name'];
		$row[] = $dispatch_status_render;
		$row[] = $eba_render;
  	$row[] = $employer_name;
  	$row[] = $hours;
		$row[] = $tools;

		$rows[$worker_nid] = $row;
  }

  $form['worker_nid'] = array(
  	'#type' => 'tableselect',
  	'#header' => $header,
  	'#options' => $rows,
  	'#suffix' => '<p>' . t('Found <strong>@count</strong> matching workers.', array('@count' => count($rows))) . '</p>',
  );

  $form['job_nid'] = array('#type' => 'hidden', '#attributes' => array('id' => 'sirius_job_nid'));

	return $form;
}

function sirius_dispatch_page_jwa_assign_form_validate($form, &$form_state) {
	// Job is required
	$job_nid = $form_state['values']['job_nid'];
	if (!$job_nid) { form_set_error('job_nid', 'Please select a job.'); }
	$worker_nids = array_filter(array_values($form_state['values']['worker_nid']));
	if (!$worker_nids) { form_set_error('worker_nid', 'Please select at least one worker.'); }
}

function sirius_dispatch_page_jwa_assign_form_submit($form, &$form_state) {
	$job_nid = $form_state['values']['job_nid'];
	$job_node = node_load($job_nid);
	$worker_nids = array_filter(array_values($form_state['values']['worker_nid']));

	$success = 0;
	$failure = 0;
	$count = 0;
	foreach ($worker_nids as $worker_nid) {
		++$count;
		$sql = "select title from node where nid = :worker_nid";
		$stmt = db_query($sql, array(':worker_nid' => $worker_nid));
		$hr = $stmt->fetchAssoc();
		$worker_name = $hr['title'];

		$dispatch_node = sirius_dispatch_get($worker_nid, $job_nid);
		if (!$dispatch_node) {
			$result = sirius_dispatch_actions_create($job_node, $worker_nid, 'Yes');
			if (!$result['success']) { 
				drupal_set_message(t('Failed to create dispatch for @worker_name to @job_name: @msg', array('@worker_name' => $worker_name, '@job_name' => $job_node->title, '@msg' => $result['msg'])), 'warning');
				++$failure;
				continue;
			}
			$dispatch_node = $result['dispatch_node'];
		}

		$result = sirius_dispatch_notify($dispatch_node);
		if (!$result['success']) { 
			drupal_set_message(t('Notification failed for @worker_name to @job_name: @msg', array('@worker_name' => $worker_name, '@job_name' => $job_node->title, '@msg' => $result['msg'])), 'warning');
			$result = sirius_dispatch_job_get_dispatchable_workers_explain($job_node, $worker_nid, array('cbn' => 'Yes', 'ignore_redispatch' => TRUE));
			if (!$result['success']) {
				drupal_set_message($result['msg'], 'warning');
			}
			++$failure;
			continue;
		}

		++$success;
		drupal_set_message(t('Notified @worker_name to @job_name', array('@worker_name' => $worker_name, '@job_name' => $job_node->title, '@msg' => $result['msg'])));
	}

	drupal_set_message(t('Assigned @count workers, with @success notifications and @failure failures.', array('@count' => $count, '@success' => $success, '@failure' => $failure)));
}