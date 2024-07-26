<?php

function sirius_event_bullpen_enabled_nid($event_nid) { return sirius_event_bullpen_enabled(node_load($event_nid)); }

function sirius_event_bullpen_enabled($event_node) { return sirius_event_bullpen_enabled_tid($event_node->field_sirius_event_type['und'][0]['tid']); }

function sirius_event_bullpen_enabled_tid($event_type_tid) { return sirius_event_bullpen_enabled_type(taxonomy_term_load($event_type_tid)); }

function sirius_event_bullpen_enabled_type($event_type_term) {
  $json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
  return $json['bullpen']['enabled'] == 'Yes';
}

function sirius_event_bullpen_ts($event_node) {
	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);

	$ts = sirius_fieldval($event_node, 'field_sirius_daterepeat', 'ts');
	$event_ts = $ts;
	$signin_opens_self = $ts - $json['bullpen']['signin_opens_self'];
	$signin_opens_others = $ts - $json['bullpen']['signin_opens_others'];

	$grace = $json['bullpen']['signin_grace'];
	if (!$grace) { variable_get('sirius_dispatch_bullpen_signin_grace', 60*15); }
	$signin_closes = $ts + $grace;

	return array(
		'event_ts' => $event_ts,
		'job_ts' => $ts,
		'signin_opens_self' => $signin_opens_self,
		'signin_opens_others' => $signin_opens_others,
		'signin_closes' => $signin_closes,
	);
}

function sirius_event_bullpen_job($event_nid, $include_nonsponsor = FALSE) {
	$sql = "select entity_id from field_data_field_sirius_events ";
	$sql .= "where entity_type = 'node' ";
	$sql .= "and bundle = 'sirius_dispatch_job' ";
	$sql .= "and field_sirius_events_target_id = :event_nid ";
	$sql .= "order by entity_id ";
	if (!$include_nonsponsor) {
		$sql .= "limit 1 ";
	}
	$stmt = db_query($sql, array(':event_nid' => $event_nid));

	if (!$include_nonsponsor) {
		$hr = $stmt->fetchAssoc();
		$job_nid = $hr['entity_id'];
		return node_load($job_nid);
	} 

	$jobs = array();
	while ($hr = $stmt->fetchAssoc()) {
		$jobs[$hr['entity_id']] = node_load($hr['entity_id']);
	}
	return $jobs;
}










class Sirius_Event_Bullpen_Access extends Sirius_Access {
	public function access($action, $context = array()) {
		$actions = array(
			'view',

			'self_view',
			'self_register',
			'self_signin',
			'self_signout',

			'register_view',
			'register_worker',
		);

		// All actions require an event
		if (!$context['event_node']) { return $this->deny("No event node."); }

		// If we're looking at an event, it must be configured as a bullpen
		if (!sirius_event_bullpen_enabled($context['event_node'])) {
			return $this->deny($context['event_node']->title . " is not configured as a bullpen.");
		}

	  // Admins can do anything
	  if (user_access('sirius admin') || user_access('sirius event admin')) { return TRUE; }

	  // Anybody can view the self signin page.
	  if ($action == 'self_view') { return TRUE; }

	  // Fill in the context assuming current worker
	  if (!$context['worker_node']) { $context['worker_node'] = sirius_worker_get(); }
	  if (!$context['contact_node']) { $context['contact_node'] = node_load($context['worker_node']->field_sirius_contact['und'][0]['target_id']); }
		if (!$context['job_node']) { $context['job_node'] = sirius_event_bullpen_job($context['event_node']->nid); }

	  // Some actions require a worker and a contact
		if (in_array($action, array('self_register', 'self_signin', 'self_signout', 'register_worker'))) {
		  if (!$context['worker_node']) { return $this->deny('There is no worker associated with your user.'); }
		  if (!$context['contact_node']) { return $this->deny('There is no contact associated with your user.'); }
		  if (!$context['job_node']) { return $this->deny('There is no job associated with this event.'); }

		  if ($action == 'self_signout') { return TRUE; }
		}

		// Is the event open for registration?
		if (in_array($action, array('register_open', 'register_worker'))) {
		  $ts = sirius_event_bullpen_ts($context['event_node']);
		  $ts_now = time();

		  if ($ts['signin_opens_others'] > $ts_now) {
		    return $this->deny("Sign-in opens at " . date('g:i A, l, F j', $ts['signin_opens_others']));
		  } 
		  if ($ts['signin_closes'] < $ts_now) {
		  	return $this->deny("Sign-in closed at " . date('g:i A, l, F j', $ts['signin_closes']));
		  }

		  if ($action == 'register_open') { return TRUE; }
		}


		// Registering workers must be eligible
		if (in_array($action, array('self_register', 'self_signin', 'register_worker'))) {
			// Check for any sign-in blocks
			if (class_exists('Sirius_Sib')) {
				$sib = Sirius_Sib::getInstance();
				if (in_array($action, array('self_register', 'self_signin'))) {
					$sib_action = 'event_bullpen_register_self';
				} else {
					$sib_action = 'event_bullpen_register';
				}

				if (!$sib->access($sib_action, array('entity_node' => $context['worker_node']))) {
					return $this->deny($sib->error_msg());
				}
			}

		  // Get the event type
		  $event_type_term = taxonomy_term_load($context['event_node']->field_sirius_event_type['und'][0]['tid']);
		  $event_type_json = sirius_json_get($event_type_term);

		  // Self-signins must be employed by the host employer
			if (in_array($action, array('self_register', 'self_signin'))) {
			  if ($event_type_json['bullpen']['self_signin_must_be_employed'] != 'no') {
			    $current_employer_nid = $context['worker_node']->field_grievance_shop['und'][0]['target_id'];
			    if ($current_employer_nid != $context['job_node']->field_grievance_shop['und'][0]['target_id']) {
			      return $this->deny("You can only sign yourself into bullpens offered by your current employer.");
			    }
			  }
			}

			// The worker must be eligible for the job
		  $result = sirius_dispatch_job_get_dispatchable_workers_explain(
		  	$context['job_node'], 
		  	$context['worker_node']->nid, 
		  	array(
		  		'ignore_skill' => TRUE, 
		  		'ignore_department' => TRUE, 
		  		'ignore_dispatch_status' => TRUE, 
		  		'ignore_event' => TRUE
		  	)
		  );
		  if (!$result['success']) { return $this->deny($result['msg']); }

		  // Can't be signed in to another bullpen on the same day.
		  $ts = sirius_event_bullpen_ts($context['event_node']);
		  $date = gmdate('Y-m-d', $ts['event_ts']);
			$probe = $this->fetch_current_signins($event_type_term, $context['worker_node'], $date);
			foreach ($probe as $probe_participant_nid => $probe_participant_node) {
				if ($probe_participant_node->field_sirius_event['und'][0]['target_id'] == $context['event_node']->nid) { continue; }

				$msg = "Currently signed in to a different " . $event_type_term->name . " event on the same day. Please sign out to continue. ";
				$msg .= l('Go to that event.', 'node/' . $probe_participant_node->field_sirius_event['und'][0]['target_id'] . '/sirius_event_info');
				return $this->deny($msg);
			}

		  // Must be EBA on the correct date
		  /*
		  if ($worker_node->field_sirius_dispatch_eba['und'][0]['value'] != 'Yes') {
		    return array('success' => FALSE, 'msg' => "You can only sign yourself into this bullpen if you have indicated that you are employed-but-available for this date. Please check your " . l('dispatch preferences.', 'sirius/worker/me/sirius_worker_dispatch_preferences'));
		  }
		  $found = FALSE;
		  $eba_dates = sirius_fieldvals($worker_node, 'field_sirius_dispatch_eba_dates');
		  $job_date = date('Y-m-d', strtotime(sirius_fieldval($job_node, 'field_sirius_datetime')));
		  foreach ($eba_dates as $eba_date) {
		    $eba_date = date('Y-m-d', strtotime($eba_date));
		    if ($eba_date == $job_date) {
		      $found = TRUE;
		      break;
		    }
		  }
		  if (!$found) {
		    // return array('success' => FALSE, 'msg' => "You can only sign yourself into this bullpen if you have indicated that you are employed-but-available for this date. Please check your " . l('dispatch preferences.', 'sirius/worker/me/sirius_worker_dispatch_preferences'));
		  }
		  */

			return TRUE;
		}

	  return $this->deny("Unknown action: $action");
	}
	public function fetch_current_signins($event_type_term, $worker_node, $date) {
		$sql_args = array();

	  $contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);

	  $event_type_json = sirius_json_get($event_type_term);
		list ($checkin_prole_tid, $checkin_pstatus_tid) = explode('|', $event_type_json['bullpen']['pre_dispatch']);

		$sql = "select node.nid as participant_nid, DATE(field_sirius_daterepeat_value) as a, CURDATE() as b ";
		$sql .= "from node ";
		$sql .= "left join field_data_field_sirius_event_prole on field_data_field_sirius_event_prole.entity_type = 'node' and field_data_field_sirius_event_prole.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_event_pstatus on field_data_field_sirius_event_pstatus.entity_type = 'node' and field_data_field_sirius_event_pstatus.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = node.nid ";
		$sql .= "left join node event_node on field_sirius_event_target_id = event_node.nid ";
		$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = event_node.nid ";
		$sql .= "left join field_data_field_sirius_event_type on field_data_field_sirius_event_type.entity_type = 'node' and field_data_field_sirius_event_type.entity_id = event_node.nid ";
		$sql .= "left join field_data_field_sirius_daterepeat on field_data_field_sirius_daterepeat.entity_type = 'node' and field_data_field_sirius_daterepeat.entity_id = event_node.nid and field_data_field_sirius_daterepeat.delta = 0 ";
		$sql .= "where node.type = 'sirius_event_participant' ";

		$sql .= "and DATE(field_sirius_daterepeat_value) = :date ";
		$sql_args[':date'] = $date;

		$sql .= "and field_sirius_event_type_tid = :event_type_tid ";
		$sql_args[':event_type_tid'] = $event_type_term->tid;

		$sql .= "and field_sirius_event_prole_tid = :checkin_prole_tid ";
		$sql_args[':checkin_prole_tid'] = $checkin_prole_tid;

		$sql .= "and field_sirius_event_pstatus_tid = :checkin_pstatus_tid ";
		$sql_args[':checkin_pstatus_tid'] = $checkin_pstatus_tid;

		$sql .= "and field_sirius_contact_target_id = :contact_nid ";
		$sql_args[':contact_nid'] = $contact_node->nid;

		$domain_nid = sirius_domain_nid();
		if ($domain_nid) {
			$sql .= "and field_sirius_domain_target_id = :domain_nid ";
			$sql_args[':domain_nid'] = $domain_nid;
		} else {
			$sql .= "and field_sirius_domain_target_id is null ";
		}

		$sql .= "limit 100 ";

		$stmt = sirius_sql_query($sql, $sql_args);
		$participant_nids = array();
		while ($hr = $stmt->fetchAssoc()) {
			$participant_nids[] = $hr['participant_nid'];
		}
		return node_load_multiple($participant_nids);
	}
}