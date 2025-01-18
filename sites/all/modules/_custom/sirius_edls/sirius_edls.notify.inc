<?php

function sirius_edls_notify_active($sheet_node) {
	if (!sirius_domain_component_enabled('edls:notify', $sheet_node->field_sirius_domain['und'][0]['target_id'])) { return FALSE; }

	$sheet_json = sirius_json_get($sheet_node);
	if ($sheet_json['edls']['notify']['enable'] != 'Yes') { return FALSE; }

	return TRUE;
}

function sirius_edls_sheet_notify_enable($sheet_node, $enable) {
	$sheet_json = sirius_json_get($sheet_node);
	$sheet_json['edls']['notify']['enable'] = $enable;
	sirius_json_set($sheet_node, $sheet_json);
	node_save($sheet_node);
	return array('success' => TRUE);
}

function sirius_edls_sheet_notify_cancel($sheet_node) {
	$sheet_nid = $sheet_node->nid;
	if (!$sheet_nid) { return array('success' => FALSE, 'msg' => 'No sheet.'); }

	$sql = "update sirius_edls_notifications ";
	$sql .= "set assignment_id = NULL, notification_status = 'error' ";
	$sql .= "where sheet_nid = :sheet_nid ";


	$stmt = db_query($sql, array(':sheet_nid' => $sheet_nid));
	$count = $stmt->rowCount();
	return array('success' => TRUE, 'count' => $count, 'msg' => "Canceled $count notifications.");
}

function sirius_edls_notify_queue($sheet_node, $new_status) {
	// Only queue if the component is enabled
	if (!sirius_edls_notify_active($sheet_node)) {
		return array('success' => TRUE, 'msg' => 'Notification is not active for this domain, employer, or sheet.');
	}

	// Only queue if we're locking or unlocking the sheet
	$old_status = $sheet_node->field_sirius_edls_sheet_status['und'][0]['value'];
	if ( ($old_status != 'lock') && ($new_status != 'lock') ) {
		return array('success' => TRUE, 'msg' => "Notification queue not necessary: old_status = $old_status and new_status = $new_status.");
	}

	// Fetch our list of notifications and assignments
	$notifications = sirius_edls_notify_fetch_sheet($sheet_node);
	$assignments_by_crew = sirius_edls_assignment_fetch_sheet($sheet_node);
	$assignments = array();
	foreach ($assignments_by_crew as $crew_uuid => $crew_assignments) { 
		foreach ($crew_assignments as $assignment) {
			$assignments[$assignment['assignment_id']] = $assignment;
		}
	}

	// Notify anybody who's no longer on the list
	$count = 0;
	foreach ($notifications as $notification_id => $notification) {
		$assignment_id = $notification['assignment_id'];
		if (!$assignments[$assignment_id]) {
			++$count;
			sirius_edls_notify_queue_item($sheet_node, $old_status, $new_status, NULL, $notification);
		} else {
			$assignments[$assignment_id]['notification_id'] = $notification_id;
		}
	}

	// Notify or update anyody who has changed
	foreach ($assignments as $assignment_id => $assignment) {
		$notification_id = $assignment['notification_id'];
		++$count;
		sirius_edls_notify_queue_item($sheet_node, $old_status, $new_status, $assignment, $notifications[$notification_id]);
	}

	return array('success' => TRUE, 'count' => $count, 'msg' => "Queued $count notifications.");
}

function sirius_edls_notify_queue_item($sheet_node, $old_status, $new_status, $assignment, $notification) {
	// Insert a notification if:
	// - The new status is "lock"
	// - There is an assignment record
	// - There isn't a notification record

	if ( ($new_status == 'lock') && ($assignment) && (!$notification) ) {
		$json = array();

		$sql = "insert into sirius_edls_notifications (employer_nid, worker_id, assignment_date, sheet_nid, assignment_id, notification_status, notification_uuid, notification_extra) ";
		$sql .= "values (:employer_nid, :worker_id, :assignment_date, :sheet_nid, :assignment_id, :notification_status, :notification_uuid, :notification_extra) ";
		$sql .= "on duplicate key update ";
		$sql .= "  sheet_nid = :sheet_nid, ";
		$sql .= "  assignment_id = :assignment_id, ";
		$sql .= "  notification_status = :notification_status ";

		$stmt = db_query($sql, array(
			':employer_nid' => $assignment['employer_nid'],
			':worker_id' => $assignment['worker_id'],
			':assignment_date' => $assignment['assignment_date'],
			':sheet_nid' => $sheet_node->nid,
			':assignment_id' => $assignment['assignment_id'],
			':notification_status' => 'queue',
			':notification_uuid' => uuid_generate(),
			':notification_extra' => json_encode($json),
		));
	}

	// Cancel a notification if:
	// - The new status is not "lock"
	// - The notification is queued

	else if ( ($new_status != 'lock') && ($notification['notification_status'] == 'queue') ) {
		// $sql = "update sirius_edls_notifications set notification_status = :notification_status where assignment_id = :assignment_id ";
		// $stmt = db_query($sql, array(':assignment_id' => $assignment_id, 'notification_status' => 'cancel'));
	}

	// In all other cases, we need to decide whether the notification has been updated. 

	else if ($new_status == 'lock') {
		if ($assignment['assignment_id'] != $notification['assignment_id']) {
			$sql = "update sirius_edls_notifications set ";
			$sql .= "notification_status = :notification_status, ";
			$sql .= "sheet_nid = :sheet_nid, ";
			$sql .= "assignment_id = :assignment_id ";
			$sql .= "where notification_id = :notification_id ";
			$stmt = db_query($sql, array(
				':assignment_id' => $assignment['assignment_id'], 
				':notification_status' => 'queue',
				':sheet_nid' => $sheet_node->nid,
				':notification_id' => $notification['notification_id'],
			));
		}
	}
}

function sirius_edls_notify_fetch_sheet($sheet_node) {
  $sql = "select * from sirius_edls_notifications ";
  $sql .= "where sheet_nid = :sheet_nid ";
  $sql .= "order by notification_id ";
  $stmt = sirius_sql_query($sql, array(':sheet_nid' => $sheet_node->nid));

  $notifications = array();
  while ($hr = $stmt->fetchAssoc()) {
    $notifications[$hr['notification_id']] = $hr;
  }
  foreach ($notifications as $notification_id => $notification) {
    $notifications[$notification_id]['notification_extra'] = json_decode($notifications[$notification_id]['notification_extra'], TRUE);
	}

  return $notifications;
}

function sirius_edls_notify_fetch_by_uuid($uuid) {
  $sql = "select * from sirius_edls_notifications ";
  $sql .= "where notification_uuid = :uuid ";
  $stmt = sirius_sql_query($sql, array(':uuid' => $uuid));
  $hr = $stmt->fetchAssoc();
  if (!$hr) { return array(); }

  $hr['notification_extra'] = json_decode($hr['notification_extra'], TRUE);
  return $hr;
}

function sirius_edls_notify_fetch_by_id($notification_id) {
  $sql = "select * from sirius_edls_notifications ";
  $sql .= "where notification_id = :notification_id ";
  $stmt = sirius_sql_query($sql, array(':notification_id' => $notification_id));
  $hr = $stmt->fetchAssoc();
  if (!$hr) { return array(); }
  $hr['notification_extra'] = json_decode($hr['notification_extra'], TRUE);
  return $hr;
}

function sirius_edls_notify_fetch_by_assignment_id($assignment_id) {
  $sql = "select * from sirius_edls_notifications ";
  $sql .= "where assignment_id = :assignment_id ";
  $stmt = sirius_sql_query($sql, array(':assignment_id' => $assignment_id));
  $hr = $stmt->fetchAssoc();
  if (!$hr) { return array(); }
  $hr['notification_extra'] = json_decode($hr['notification_extra'], TRUE);
  return $hr;
}

function sirius_edls_notify_cleanup($mode) {
  // Clean up assignments  where the sheet no longer exists
  $sql = "select assignment_id from sirius_edls_notifications ";
  $sql .= "left join node on node.nid = sheet_nid ";
  $sql .= "where nid is null ";
  $sql .= "order by assignment_id limit 5000 ";
  $stmt = db_query($sql, array());

  $count = 0;
  while ($hr = $stmt->fetchAssoc()) {
    ++$count;
    $assignment_id = $hr['assignment_id'];
    if ($mode == 'live') {
      sirius_minilog("Purging notification $assignment_id");
      $sql2 = "delete from sirius_edls_notifications where assignment_id = :assignment_id ";
      $stmt2 = db_query($sql2, array(':assignment_id' => $assignment_id));
    } else {
      sirius_minilog("Purging notification $assignment_id (skipping purge because mode = $mode)");
    }
  }

  return array('success' => TRUE, 'count' => $count);
}

function sirius_edls_notify_deliver_batch($mode = 'test', $force = FALSE) {

	/*
	if (!$force) {
		// Don't send before 5:00 PM
		// @todo: This should be configurable, not hardcoded.
		// @todo: We should be using the domain-specific timezone. But we're not, because this isn't a multi-domain function, because it comes 
		//        from Teamsters 631, where things happen in the root domain :(

		$tz_orig = date_default_timezone_get();
		$tz = variable_get('date_default_timezone', @date_default_timezone_get());
		if ($tz != $tz_orig) { 	date_default_timezone_set($tz); }
		$h = date('H');
		$t = date('H:i');
		date_default_timezone_set($tz_orig);
		if ($h < 17) { return array('success' => TRUE, 'msg' => "Cannot run before 17:00 (currently $t)"); }
	}
	*/

	// $tomorrow = date('Y-m-d', strtotime('+1 day'));

	$sql_args = array();
	$sql = "select notification_id from sirius_edls_notifications ";
	$sql .= "where notification_status = 'queue' ";
	// $sql .= "and assignment_date = :tomorrow ";
	// $sql_args[':tomorrow'] = $tomorrow;
	$sql .= "order by notification_id ";
	$sql .= "limit 100 ";

	$stmt = db_query($sql, $sql_args);
	$count = $count_success = $count_error = 0;
	$notification_ids = array();
	while ($hr = $stmt->fetchAssoc()) { $notification_ids[] = $hr['notification_id']; }
	if (!$notification_ids) { return array('success' => TRUE, 'count' => 0, 'msg' => 'There are no queued notifications.'); }

	foreach ($notification_ids as $notification_id) {
		$result = sirius_edls_notify_deliver($mode, $notification_id);
		++$count;
		if ($result['success']) { ++$count_success; } else { ++$count_error; }
	}

	return array('success' => TRUE, 'count' => $count, 'count_success' => $count_success, 'count_error' => $count_error);
}

function sirius_edls_notify_fetch_assignment($notification) {
	$assignment = sirius_edls_assignment_fetch_by_assignment_id($notification['assignment_id']);

	if (!$assignment) { 
		// Create a "shadow" assignment for notifciations to assignments that have been deleted.
		$assignment = $notification;
		$assignment['worker_id'] = $notification['worker_id'];
	}
	return $assignment;
}

function sirius_edls_notify_deliver($mode, $notification_id) {
	$notification = sirius_edls_notify_fetch_by_id($notification_id);

	if (!$notification) { return array('success' => FALSE, 'msg' => 'Notification not found.'); }

	$assignment = sirius_edls_notify_fetch_assignment($notification);

	$sheet_node = node_load($notification['sheet_nid']);
	if (!$sheet_node) { return array('success' => FALSE, 'msg' => 'Sheet node not found.'); }

	$old_domain = sirius_domain_nid();
	if (!$old_domain) { $old_domain = -1; }
	$new_domain = $sheet_node->field_sirius_domain['und'][0]['target_id'];
	if (!$new_domain) { $new_domain = -1; }
	sirius_domain($new_domain);

	$notification_status = '';
	$worker_id = $notification['worker_id'];

	if (!$worker_id) {
		$notification_status = 'error';
		$msg = 'No worker ID.';
	} else if (preg_match('/^I/', $worker_id)) {

		// 
		// Handle Insource workers ($worker_node)
		// Notify via sirius_worker_notify(), which pulls from their preferences
		//

		$real_id = preg_replace('/^I/', '', $worker_id);
		$worker_node = sirius_worker_get_by_id($real_id);
		if (!$worker_node) {
			$notification_status = 'error';
			$msg = "Worker record not found: $real_id";
		} else {
			$date = date('l, F j', strtotime($sheet_node->field_sirius_date_start['und'][0]['value']));
			global $base_url;
			$url = $base_url . '/edls-notify/' . $notification['notification_uuid'];
			$subject = $worker_node->title . ": Crew assignment for $date";
			$body = "Your crew assignment for $date has been posted or updated. Please follow the link below and accept:<br><br><a href=\"$url\">$url</a>";
			$sms = $worker_node->title . ": your crew assignment for $date has been posted or updated. Please follow the link below and accept:\n\n$url";

			if ($mode == 'live') {

				$notify_params = array(
					'subject' => $subject,
					'body' => $body,
					'sms' => $sms,
				);

				$employer_node = node_load($sheet_node->field_grievance_shop['und'][0]['target_id']);
				$context = array('notify_params' => &$notify_params, 'employer_node' => $employer_node);
				drupal_alter('sirius_edls_notify_pre_send', $context);

				$result = sirius_worker_notify($worker_node, $notify_params);
				$comm_node = $result['comm'];

				$context = array('notify_params' => &$notify_params, 'notify_result' => &$result, 'employer_node' => $employer_node);
				drupal_alter('sirius_edls_notify_post_send', $context);
				
				if ($result['success']) {
					$notification_status = 'sent';
					$msg = "Notification sent at " . date('r');
				} else {
					$notification_status = 'error';
					$msg = $result['msg'];
				}
			}
		}
	} else {
		//
		// Handle outsource workers ($worker_record)
		// Notify via sirius_sms_send()
		//

		$worker_record = sirius_edls_outsource_get_by_id($sheet_node->field_grievance_shop['und'][0]['target_id'], $worker_id);
		if (!$worker_record) {
			$notification_status = 'error';
			$msg = "Worker $worker_id was not found";
		} else if (!$worker_record['worker_phone']) {
			$notification_status = 'error';
			$msg = "Worker $worker_id does not have a phone number.";
		} else {
			$date = date('l, F j', strtotime($sheet_node->field_sirius_date_start['und'][0]['value']));
			global $base_url;
			$url = $base_url . '/edls-notify/' . $notification['notification_uuid'];
			$subject = $worker_record['worker_name'] . ": Crew assignment for $date";
			$body = "Your crew assignment for $date has been posted or updated. Please follow the link below and accept:<br><br><a href=\"$url\">$url</a>";
			$sms = $worker_record['worker_name'] . ": your crew assignment for $date has been posted or updated. Please follow the link below and accept:\n\n$url";

			if ($mode == 'live') {

				$notify_params = array(
					'subject' => $subject,
					'body' => $body,
					'sms' => $sms,
					'to' => $worker_record['worker_phone'],
				);

				$employer_node = node_load($sheet_node->field_grievance_shop['und'][0]['target_id']);
				$context = array('notify_params' => &$notify_params, 'employer_node' => $employer_node);
				drupal_alter('sirius_edls_notify_pre_send', $context);

				$result = sirius_sms_send($notify_params);
				$comm_node = $result['comm'];

				$context = array('notify_params' => &$notify_params, 'notify_result' => &$result, 'employer_node' => $employer_node);
				drupal_alter('sirius_edls_notify_post_send', $context);
				
				if ($result['success']) {
					$notification_status = 'sent';
					$msg = "Notifiation sent at " . date('r');
				} else {
					$notification_status = 'error';
					$msg = $result['msg'];
				}
			}
		}
	}

	if ($mode != 'live') {
		sirius_minilog("... Skipping update because mode is $mode.");
	} else {

		if ($notification_status == 'sent') {
			sirius_edls_log_instance()->assignment_log('notification_sent', $assignment);
		} else {
			sirius_edls_log_instance()->assignment_log('notification_error', $assignment);
		}

		$notification['notification_extra']['delivery'] = array();
		$notification['notification_extra']['delivery']['msg'] = $msg;
		$notification['notification_extra']['delivery']['ts'] = time();
		$notification['notification_extra']['delivery']['comm_nid'] = $comm_nid;

		$comm_nid = NULL;
		if ($comm_node) { $comm_nid = $comm_node->nid; }

		$sql = "update sirius_edls_notifications set ";
		$sql .= "comm_nid = :comm_nid, ";
		$sql .= "notification_status = :notification_status, ";
		$sql .= "notification_extra = :notification_extra ";
		$sql .= "where notification_id = :notification_id";
		$stmt = db_query($sql, array(
			':notification_id' => $notification_id, 
			':notification_status' => $notification_status,
			':comm_nid' => $comm_nid,
			':notification_extra' => json_encode($notification['notification_extra']),
		));
	}

	sirius_minilog("... Notification $notification_id ($notification[notification_uuid]) set to \"$notification_status\". Msg = $msg.");
	sirius_domain($old_domain);
	return array('success' => TRUE, 'msg' => $msg);
}

function 	sirius_edls_notify_accept($notification_id) {
	$notification = sirius_edls_notify_fetch_by_id($notification_id);
	if (!$notification) { return array('success' => FALSE, 'msg' => 'Notification not found.'); }

	$assignment = sirius_edls_notify_fetch_assignment($notification);
	sirius_edls_log_instance()->assignment_log('notification_accept', $assignment);

	global $user;
	$notification['notification_extra']['accepted'] = array(
		'ts' => time(),
		'ip' => sirius_loopback_remote_ip(),
		'uid' => $user->uid,
		'name' => $user->name,
		'assignment' => $assignment,
	);

	$sql = "update sirius_edls_notifications set notification_status = 'accepted', notification_extra = :json where notification_id = :notification_id";
	$stmt = db_query($sql, array(
		':notification_id' => $notification_id,
		':json' => json_encode($notification['notification_extra']),
	));
	return array('success' => TRUE, 'msg' => t('Assignment Accepted'));
}

function 	sirius_edls_notify_decline($notification_id) {
	$notification = sirius_edls_notify_fetch_by_id($notification_id);
	if (!$notification) { return array('success' => FALSE, 'msg' => 'Notification not found.'); }

	$assignment = sirius_edls_notify_fetch_assignment($notification);
	sirius_edls_log_instance()->assignment_log('notification_decline', $assignment);

	global $user;
	$notification['notification_extra']['declined'] = array(
		'ts' => time(),
		'ip' => sirius_loopback_remote_ip(),
		'uid' => $user->uid,
		'name' => $user->name,
		'assignment' => $assignment,
	);

	$sql = "update sirius_edls_notifications set notification_status = 'declined', notification_extra = :json where notification_id = :notification_id";
	$stmt = db_query($sql, array(
		':notification_id' => $notification_id,
		':json' => json_encode($notification['notification_extra']),
	));
	return array('success' => TRUE, 'msg' => t('Assignment declined'));
}

function sirius_edls_notify_render_status($assignment, $notification, $options = array()) {
	$notification_status = $notification['notification_status'];

	if ($notification_status == 'accepted') { $icon = '<i class="fas fa-thumbs-up"></i>'; }
	if ($notification_status == 'declined') { $icon = '<i class="fas fa-thumbs-down"></i>'; }
	if ($notification_status == 'queue') { $icon = '<i class="far fa-envelope"></i>'; }
	if ($notification_status == 'sent') { $icon = '<i class="far fa-envelope"></i>'; }
	if ($notification_status == 'error') { $icon = '<i class="fas fa-exclamation-triangle"></i>'; }

	if (!$icon) { return ''; }

	$html = "<span class=\"sirius_edls_notify_status sirius_edls_notify_status_$notification_status\">";

	if ($options['can_edit_notifications']) {
		$html .= l($icon, "edls-notify/$notification[notification_uuid]", array('html' => TRUE));
	} else {
		$html .= $icon;
	}

	$html .= '</span>';

	return $html;
}

function sirius_edls_notify_render_status_long($assignment, $notification) {
	$notification_status = $notification['notification_status'];
	if ($notification_status == 'accepted') { $msg = 'Assignment is ACCEPTED'; }
	if ($notification_status == 'declined') { $msg = 'Assignment is DECLINED'; }
	if ($notification_status == 'queue') { $msg = 'Assignment is BEING DELIVERED'; }
	if ($notification_status == 'sent') { $msg = 'Assignment is AWAITING CONFIRMATION'; }
	if ($notification_status == 'error') { $msg = 'Assignment is UNDELIVERABLE'; }

	if ($notification_status == 'accepted') {
		$msg .= '<div class="sirius_edls_notify_status_long_details">';
		$msg .= "(Accepted " . date('c', $notification['notification_extra']['accepted']['ts']) . ')';
		$msg .= '</div>';
	}

	$html = "<div class=\"sirius_edls_notify_status_long sirius_edls_notify_status_$notification_status\">";
	$html .= $msg;
	$html .= "</div>";

	return $html;
}

function sirius_edls_notify_render($notification) {
	$assignment = sirius_edls_notify_fetch_assignment($notification);
	$sheet_node = node_load($assignment['sheet_nid']);
	if (!$sheet_node) { return sirius_page_error('Sheet not found.'); }
	$employer_node = node_load($sheet_node->field_grievance_shop['und'][0]['target_id']);
	$sheet_vid = $sheet_node->vid;
	$sheet_nid = $sheet_node->nid;
	$status_options = sirius_edls_sheet_status_options();
	$sheet_json = sirius_json_get($sheet_node);

	$date = date('l, F j', strtotime($sheet_node->field_sirius_date_start['und'][0]['value']));
	drupal_set_title("Crew Assignment for $assignment[worker_name]: $date");

	$crews = sirius_edls_crews($sheet_node, 'uuid');
	$crew = $crews[$assignment['crew_uuid']];

	$html .= '<div class="sirius_callout_fullwidth">';
	if (!$crew) {
		$html .= '<p>' . t('You have been removed from all assignments for employer @employer on <strong>@date</strong>.', array('@employer' => $employer_node->title, '@date' => $date)) . '</p>';
	} else {
		$extra = sirius_edls_assignment_render_extra($assignment, array('text_only' => TRUE));
		if ($extra) {
			$html .= '<h2>' . t('Special Instructions (role or alternate start time)') . '</h2>';
			$html .= $extra;
			$html .= '<hr>';
		}

		$html .= '<h2>' . t('Job Information') . '</h2>';
		$html .= sirius_fakerender_field('Employer:', $employer_node->title);
		$html .= sirius_fakerender_field('Supervisor:', sirius_edls_supervisor_render($sheet_node));
		$html .= sirius_fakerender_field('Date:', date('l, F j, Y', strtotime($sheet_node->field_sirius_date_start['und'][0]['value'])));
		$html .= sirius_fakerender_field('Event:', sirius_node_title($sheet_node->field_sirius_dispatch_job_group['und'][0]['target_id']));
		$html .= sirius_fakerender_field('Event Status:', sirius_text_to_html($sheet_json['edls']['sheet']['show_status']));
		$html .= sirius_fakerender_field('Department:', sirius_term_title($sheet_node->field_grievance_department_tid['und'][0]['tid']));
		$html .= sirius_fakerender_field('Job #:', sirius_text_to_html($sheet_node->field_sirius_job_number['und'][0]['value']));
		$html .= sirius_fakerender_field('Facility:', sirius_node_title($sheet_node->field_sirius_dispatch_facility['und'][0]['target_id']));
		$html .= sirius_fakerender_field('Hall:', sirius_node_title($sheet_node->field_sirius_dispatch_hall['und'][0]['target_id']));

		$html .= '<hr>';
		$html .= '<h2>' . t('Crew') . '</h2>';
		$html .= sirius_fakerender_field('Crew:', $crew['name']);
		$html .= sirius_fakerender_field('Task:', $crew['task']);
		$html .= sirius_fakerender_field('Start Time:', $crew['start_time'] . ' - ' . $crew['end_time']);
		$html .= sirius_fakerender_field('Checkin Location:', $crew['checkin_location']);
	}

	$html .= '</div>';

	$html .= sirius_edls_notify_render_status_long($assignment, $notification);

	$access = sirius_access_instance('edls');
	if ($access->access('notification_view_delivery')) {
		$html .= '<div class="sirius_callout_fullwidth">';
		$html .= '<h2>' . t('Delivery') . '</h2>';

		$ts = $notification['notification_extra']['delivery']['ts'];
		if ($ts) { $html .= sirius_fakerender_field('Sent at:', date('m/d/Y H:i', $ts)); }

		$html .= sirius_fakerender_field('Message:', $notification['notification_extra']['delivery']['msg']);

		if ($notification['comm_nid']) {
			$comm_node = node_load($notification['comm_nid']);
			if ($comm_node) {
				$medium = $comm_node->field_sirius_category['und'][0]['value'];
				$instance = sirius_comm_instance($medium);
				if ($instance) {
					$html .= $instance->render($comm_node);
				}
			}
		}
		$html .= '</div>';
	}

	return $html;
}

/**
 * Implements hook_sirius_comm_update_handlers
 * 
 * Most errors will fail immediately. We're not bothering to highlight subsequent delivery failures at the level of the notification; 
 * it will just keep showing as "sent", and you can click in to the details to see why it wasn't delivered.
 */

function sirius_edls_sirius_comm_update_handlers($comm_node) {
	// watchdog('whatever', 'Updating handlers for ' . $comm_node->nid . ' (' . $comm_node->field_sirius_type['und'][0]['value']);
}

