<?php

function sirius_dispatch_notify_employer_dispatch($dispatch_node) {
	$worker_nid = $dispatch_node->field_sirius_worker['und'][0]['target_id'];
	$worker_node = node_load($worker_nid);
	if (!$worker_node) { return; }

	$job_nid = $dispatch_node->field_sirius_dispatch_job['und'][0]['target_id'];
	$job_node = node_load($job_nid);
	if (!$job_node) { return; }

	$employer_nid = $job_node->field_grievance_shop['und'][0]['target_id'];
	$employer_node = node_load($employer_nid);

	if ($job_node->field_sirius_notify['und'][0]['value'] != 'Yes') { return; }
	$emails = sirius_fieldvals($job_node, 'field_sirius_emails');
	if (!$emails) { return; }
	if (!$job_node->field_sirius_dispatch_job_nfcns['und'][0]) { return; }

	$notifications = array();
	foreach ($job_node->field_sirius_dispatch_job_nfcns['und'] as $tmp) {
		$notifications[$tmp['value']] = TRUE; 
	}

	$status_new = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
	$status_old = $dispatch_node->original->field_sirius_dispatch_status['und'][0]['value'];
	if (!$status_new || !$status_old) { return; }
	if ($status_new == $status_old) { return; }

	$found = FALSE; 
	if ($status_new == 'accepted' && $notifications['dispatch_accept']) { $found = TRUE; }
	if ($status_new == 'declined' && $notifications['dispatch_decline']) { $found = TRUE; }
	if ($status_new == 'expired' && $notifications['dispatch_expire']) { $found = TRUE; }
	if (($status_new == 'resigned' || $status_new == 'vlri') && $notifications['dispatch_resign']) { $found = TRUE; }
	if (!$found) { return; }

	global $base_url;
	$subject = "Dispatch update - Call #";
	if ($job_node->field_sirius_id['und'][0]['value']) {
		$subject .= $job_node->field_sirius_id['und'][0]['value'];
	} else {
		$subject .= $job_node->nid;
	}
	$subject .= ' / Worker ' . $worker_node->title . ' / ' . $status_new;
	$body = "This is a status update notification.<br />";
	$body .= "Job: <a href=\"$base_url/node/" . $job_node->nid . "\">" . $job_node->field_sirius_id['und'][0]['value'] . "</a><br />";
	$body .= "Dispatch: <a href=\"$base_url/node/" . $dispatch_node->nid . "\">" . $dispatch_node->nid . "</a><br />";
	$body .= "Worker: <a href=\"$base_url/node/" . $worker_node->nid . "\">" . $worker_node->title . "</a><br />";
	$body .= "Old status: <strong>$status_old</strong><br />";
	$body .= "New status: <strong>$status_new</strong><br />";

	$body .= '<br /><strong>' . 'Other dispatches of ' . $worker_node->title . ' to ' . $employer_node->title . '</strong><br>';

	$params = array();
	$params['employer_nid'] = $employer_nid;
	$params['worker_nid'] = $worker_nid;
	$prior_nodes = sirius_dispatch_search($params);

	if ($prior_nodes) {
		$rows = array();
		foreach ($prior_nodes as $prior_nid => $prior_node) {
			if ($prior_node->nid == $dispatch_node->nid) { continue; }
			$prior_job = node_load($prior_node->field_sirius_dispatch_job['und'][0]['target_id']);
			$type_name = sirius_term_title($prior_job->field_sirius_dispatch_job_type['und'][0]['tid']);

			$ts = sirius_fieldval($prior_job, 'field_sirius_datetime', 'ts');

			$row = array();
			$row[] = date('m/d/Y', $ts);
			$row[] = $type_name;
			$row[] = "<a href=\"$base_url/node/$prior_nid/sirius_dispatch_worker_view\">View</a>";
			$rows[] = $row;
		}

		$header = array();
		$header[] = t('Date');
		$header[] = t('Job Type');
		$header[] = t('Tools');

		$body .= sirius_table_render($header, $rows);
	} else {
		$body .= '<p>' . t('No other dispatches found.') . '</p>';
	}

	foreach ($emails as $email) {
		sirius_email_send(
			array(
				'to' => $email,
				'subject' => $subject,
				'body' => $body,
				'handler_nids' => array($dispatch_node->nid),
			)
		);
	}
}

function sirius_dispatch_notify_employer_job($job_node) {
	if (!$job_node) { return; }
	if ($job_node->field_sirius_notify['und'][0]['value'] != 'Yes') { return; }
	$emails = sirius_fieldvals($job_node, 'field_sirius_emails');
	if (!$emails) { return; }
	if (!$job_node->field_sirius_dispatch_job_nfcns['und'][0]) { return; }

	$notifications = array();
	foreach ($job_node->field_sirius_dispatch_job_nfcns['und'] as $tmp) {
		$notifications[$tmp['value']] = TRUE; 
	}

	$status_new = $job_node->field_sirius_dispatch_job_status['und'][0]['value'];
	$status_old = $job_node->original->field_sirius_dispatch_job_status['und'][0]['value'];
	if (!$status_new || !$status_old) { return; }
	if ($status_new == $status_old) { return; }

	$found = FALSE; 
	if ($status_new == 'Unfilled' && $notifications['job_approve']) { $found = TRUE; }
	if ($status_new == 'Filled' && $notifications['job_fill']) { $found = TRUE; }
	if (!$found) { return; }

	global $base_url;
	$subject = "Job update - Call #";
	if ($job_node->field_sirius_id['und'][0]['value']) {
		$subject .= $job_node->field_sirius_id['und'][0]['value'];
	} else {
		$subject .= $job_node->nid;
	}
	$subject .= ' / ' . $status_new;
	$body = "This is a status update notification.<br />";
	$body .= "Job: <a href=\"$base_url/node/" . $job_node->nid . "\">" . $job_node->field_sirius_id['und'][0]['value'] . "</a><br />";
	$body .= "Old status: <strong>$status_old</strong><br />";
	$body .= "New status: <strong>$status_new</strong><br />";

	foreach ($emails as $email) {
		sirius_email_send(
			array(
				'to' => $email,
				'subject' => $subject,
				'body' => $body,
				'handler_nids' => array($job_node->nid),
			)
		);
	}
}