<?php

/**
 * Wrapper for forms, because we can't call drupal_get_form() on a class method
 */

function sirius_worker_tos_worker_form($form, &$form_state, $worker_node) {
	return Sirius_Worker_Tos::getInstance()->worker_form($form, $form_state, $worker_node);
}

function sirius_worker_tos_worker_form_validate($form, &$form_state) {
	return Sirius_Worker_Tos::getInstance()->worker_form_validate($form, $form_state);
}

function sirius_worker_tos_worker_form_submit($form, &$form_state) {
	return Sirius_Worker_Tos::getInstance()->worker_form_submit($form, $form_state);
}

function sirius_worker_tos_form($form, &$form_state, $worker_node) {
	return Sirius_Worker_Tos::getInstance()->form($form, $form_state, $worker_node);
}

function sirius_worker_tos_form_validate($form, &$form_state) {
	return Sirius_Worker_Tos::getInstance()->form_validate($form, $form_state);
}

function sirius_worker_tos_form_submit($form, &$form_state) {
	return Sirius_Worker_Tos::getInstance()->form_submit($form, $form_state);
}

function sirius_worker_tos_cron_reminders($mode = 'test', $force = FALSE) {
	return Sirius_Worker_Tos::getInstance()->cron_reminders($mode, $force);
}

/**
 * Implements hook_sirius_sched_info
 */

function sirius_worker_tos_sirius_sched_info() {
	$info['worker_tos'] = array(
		'title' => 'Worker TOS records',
		'callback_denorm_check' => 'sirius_worker_tos_sched_check',
		'callback_denorm' => 'sirius_worker_tos_sched_write',
	);
	return $info;
}

/**
 * Called on every node save; determines whether we need to queue it for scheduling
 */

function sirius_worker_tos_sched_check($tos_node) {
	return Sirius_Worker_Tos::getInstance()->sched_check($tos_node);
}

function sirius_worker_tos_sched_write($tos_node) {
	return Sirius_Worker_Tos::getInstance()->sched_write($tos_node);
}

class Sirius_Worker_Tos extends Sirius_Singleton {
	public $type = 'tos';
	public $name = 'Call In / Out';
	public $description = '';

	public function sched_check($tos_node) {
		// Just if it's a log entry
	  if ($tos_node->type != 'sirius_log') { return FALSE; }

	  // Must be of the right type
		if ($tos_node->field_sirius_category['und'][0]['value'] != 'tos') { return FALSE; }

		return TRUE;
	}

	public function worker($tos_node) {
		return sirius_log_handler($tos_node, 'sirius_worker');
	}

	public function sched_write($tos_node) {
		$worker_node = $this->worker($tos_node);
		if (!$worker_node) { return; }

		$tos_json = sirius_json_get($tos_node);
		$start_ts = $tos_node->created;
		$end_ts = $tos_json['worker_tos']['cancel']['ts'];
		if (!$end_ts) { $end_ts = NULL; }

		$sched_json = array();
		$sched_json['display']['type'] = 'Call In / Out';
		$sched_json['display']['status'] = $this->status_label($this->status($tos_node));
		$sched_json['display']['title_short'] = 'Call In / Out';
		$sched_json['display']['path'] = "node/" . $tos_node->nid . "/sirius_worker_tos";
		$sched_json['display']['granularity'] = 'min';
		$sched_json['display']['details'] = $tos_node->field_sirius_summary['und'][0]['value'];

		$sched = array(
			'entity_nid' => $worker_node->nid,
			'start_ts' => $start_ts,
			'end_ts' => $end_ts,
			'json' => $sched_json,
		);

		return array($sched);
	}

	public function worker_active_record($worker_nid) {
		$search_params = array();
		$search_params['worker_nid'] = $worker_nid;
		$search_params['status'] = 'active';
		$search_params['limit'] = 1;
		$tos_nodes = $this->search($search_params);
		return reset($tos_nodes);
	}

	public function status_options() {
		return array(
			'active' => 'Absent',
			'cancel' => 'Canceled',
		);
	}

	public function status_label($status) {
		$label = $this->status_options()[$status];
		if (!$label) { $label = $status; }
		return $label;
	}

	public function status($tos_node) {
		return $tos_node->field_sirius_type['und'][0]['value'];
	}

	public function worker_form($form, &$form_state, $worker_node, $options = array()) {
		if (!$worker_node) { return sirius_form_error("Missing worker node."); }

		drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');

		$form['#tree'] = TRUE;
		$form_state['worker_nid'] = $worker_node->nid;

		$form['intro'] = array('#markup' => sirius_domain_variable_get('sirius_worker_tos_intro'));

		$tos_node = $this->worker_active_record($worker_node->nid);

		if ($tos_node) {
			$status = $this->status($tos_node);
			$form['action'] = array('#type' => 'value', '#value' => 'cancel');

			$html = '<p>' . '<strong><span class="sirius_text_red">' . t('You are currently marked as "Absent".') . '</span></strong></p>';

			$class = 'sirius_callout_basic ';
			if ($status == 'active') { $class .= 'sirius_callout_red '; }
			$class .= "sirius_worker_tos sirius_worker_tos_$status";

			$html .= "<div class=\"$class\">";
			$html .= sirius_fakerender_field('Record ID:', $tos_node->uuid);
			$html .= sirius_fakerender_field('Status:', $this->status_label($status));
			$html .= sirius_fakerender_field('Absent at:', date('l, F j, Y g:i A T', $tos_node->created));
			$tos_json = sirius_json_get($tos_node);
			$ts = $tos_json['worker_tos']['cancel']['ts'];
			if ($ts) {
				$html .= sirius_fakerender_field('Returned at:', date('l, F j, Y g:i A T', $ts));
			}
			$html .= '<p>' . sirius_text_to_html($tos_node->field_sirius_summary['und'][0]['value']) . '</p>';
			$html .= '</div>';

			$form['cancel']['intro'] = array('#markup' => $html);

			$form['cancel']['confirm'] = array(
				'#type' => 'checkbox',
				'#title' => t('Remove me from the "Absent" list.'),
				'#required' => TRUE,
			);

			$form['cancel']['submit'] = array(
				'#type' => 'submit',
				'#value' => t('Remove me from the "Absent" list'),
			);

		} else {
			$form['action'] = array('#type' => 'value', '#value' => 'activate');

			$html = '<p>' . '<strong><span class="sirius_text_green">' . t('You are not on the "Absent" list.') . '</span></strong></p>';
			$form['activate']['intro'] = array('#markup' => $html);

			$form['activate']['comments'] = array(
				'#type' => 'textarea',
				'#title' => t('Comments'),
				'#required' => TRUE,
			);

			$form['activate']['confirm'] = array(
				'#type' => 'checkbox',
				'#title' => t('Place me on the "Absent" list.'),
				'#required' => TRUE,
			);

			$form['activate']['submit'] = array(
				'#type' => 'submit',
				'#value' => t('Mark me absent now'),
			);
		}

		return $form;
	}

	public function worker_form_validate($form, &$form_state) {
	}

	public function worker_form_submit($form, &$form_state) {
		$worker_node = node_load($form_state['worker_nid']);
		$action = $form_state['values']['action'];
		if ($action == 'activate') {
			$result = SiriusResult::create($this->activate($worker_node, $form_state['values']['activate']));
		} else if ($action == 'cancel') {
			$result = SiriusResult::create($this->cancel($worker_node, $form_state['values']['cancel']));
		} else {
			$result = SiriusResult::create(array('success' => FALSE, 'msg' => "Missing or unknown action."));
		}

		$result->drupalSetMessage();
	}

















	public function comments($tos_node) {
		return $tos_node->field_sirius_summary['und'][0]['value'];
	}


	public function form($form, &$form_state, $tos_node, $options = array()) {
		if (!$tos_node) { return sirius_form_error("Missing tos node."); }

		global $user;
		$status = $this->status($tos_node);
		if ($status == 'active') {
			$form['disabled'] = array('#markup' => '<p><strong>' . t('This record is active. Active records cannot be edited until they are canceled by the worker.' . '</strong></p>'));

			if ($user->uid != 1) {
				$form['#disabled'] = TRUE;
			}
		}

		drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');

		$form['#tree'] = TRUE;
		$form_state['tos_nid'] = $tos_node->nid;

		$form['intro'] = array(
			'#markup' => sirius_fakerender_field('Status:', $this->status_label($this->status($tos_node))),
		);

		$form['comments'] = array(
			'#type' => 'textarea',
			'#title' => t('Comments'),
			'#default_value' => $this->comments($tos_node),
		);

		$form['start_date'] = array(
			'#title' => t('Start Date'),
			'#type' => 'date_popup',
			'#date_format' => 'Y-m-d H:i:s',
			'#attributes' => array('autocomplete' => 'off'),
		);
		$start_ts = $this->start_ts($tos_node);
		if ($start_ts) { $form['start_date']['#default_value'] = date('Y-m-d H:i:s', $start_ts); }

		$form['end_date'] = array(
			'#title' => t('End Date'),
			'#type' => 'date_popup',
			'#date_format' => 'Y-m-d H:i:s',
			'#attributes' => array('autocomplete' => 'off'),
		);
		$end_ts = $this->end_ts($tos_node);
		if ($end_ts) { $form['end_date']['#default_value'] = date('Y-m-d H:i:s', $end_ts); }


		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Save'),
		);

		return $form;
	}

	public function form_validate($form, &$form_state) {
		$start_ts = strtotime($form_state['values']['start_date']);
		$end_ts = strtotime($form_state['values']['end_date']);
		if ($start_ts && $end_ts && ($end_ts < $start_ts)) {
			form_set_error('end_date', 'End date must be after start date.');
		}

		if ($end_ts && $end_ts > time()) {
			form_set_error('end_date', 'End time must be in the past.');
		}
	}

	public function form_submit($form, &$form_state) {
		$tos_node = node_load($form_state['tos_nid']);

		$tos_node->field_sirius_summary['und'][0]['value'] = $form_state['values']['comments'];
		$tos_json = sirius_json_get($tos_node);
		if ($form_state['values']['start_date']) {
			$tos_node->created = strtotime($form_state['values']['start_date']);
		}
		if ($form_state['values']['end_date']) {
			$tos_json['worker_tos']['cancel']['ts'] = strtotime($form_state['values']['end_date']);
		}
		sirius_json_set($tos_node, $tos_json);
		node_save($tos_node);
		
		drupal_set_message("Record saved.");
	}
























	public function employer_notify_emails($worker_node) {
		// Get the employer email addresses
		$search_params = array();
		$search_params['worker_nid'] = $worker_node->nid;
		$search_params['statuses'] = array('accepted', 'accepted_secondary');
		$dispatch_nodes = sirius_dispatch_search($search_params);

		$notify_emails = array();
		foreach ($dispatch_nodes as $dispatch_nid => $dispatch_node) {
			$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
			$notify_emails += sirius_fieldvals($job_node, 'field_sirius_emails');
		}
		$notify_emails = array_filter(array_unique($notify_emails));
		return $notify_emails;
	}


	public function activate($worker_node, $params) {
		if (!$worker_node) { return array('success' => FALSE, 'msg' => 'No worker node.'); }

	  if (!sirius_flood_is_allowed('sirius_worker_tos_activate', $worker_node->nid)) {
	  	return array('success' => FALSE, 'msg' => "There have been too many attempts to mark this worker absent. Please try again later.");
	  }

		$tos_node = $this->worker_active_record($worker_node->nid);
		if ($tos_node) { return array('success' => FALSE, 'msg' => 'There is already an active "absent" record for this worker.'); }

		$tos_node = sirius_log('tos', 'active', "\"Call Out\" activated for " . $worker_node->nid, $params['comments'], '', array($worker_node->nid));

		sirius_flood_register_event('sirius_worker_tos_activate', $worker_node->nid);

		$notify_emails = $this->employer_notify_emails($worker_node);
		$params = array();
		$params['subject'] = "Call out activated: " . $worker_node->title;
		$params['body'] = $worker_node->title . " has activated a call out.";
		foreach ($notify_emails as $notify_email) {
			$params['to'] = $notify_email;
      $result = sirius_email_send($params);
		}

		return array('success' => TRUE, 'msg' => '"Absent" status has been turned on.', 'tos_node' => $tos_node);
	}

	public function cancel($worker_node, $params) {
		if (!$worker_node) { return array('success' => FALSE, 'msg' => 'No worker node.'); }
		$tos_node = $this->worker_active_record($worker_node->nid);
		if (!$tos_node) { return array('success' => FALSE, 'msg' => 'There is no active "absent" record'); }

		$tos_node->field_sirius_type['und'][0]['value'] = 'cancel';
		$tos_json = sirius_json_get($tos_node);
		$tos_json['worker_tos']['cancel']['ts'] = time();
		sirius_json_set($tos_node, $tos_json);
		node_save($tos_node);

		$notify_emails = $this->employer_notify_emails($worker_node);
		$params = array();
		$params['subject'] = "Call out canceled: " . $worker_node->title;
		$params['body'] = $worker_node->title . " has canceled an call out.";
		foreach ($notify_emails as $notify_email) {
			$params['to'] = $notify_email;
      $result = sirius_email_send($params);
		}

		return array('success' => TRUE, 'msg' => '"Absent" status has been canceled.', 'tos_node' => $tos_node);
	}

	public function search($params) {
		$domain_nid = sirius_domain_nid();
		$sql_args = array();
		$sql = "select distinct node.nid from node ";
		$sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";

		$sql .= "left join field_data_field_sirius_log_handler dispatch_handler on dispatch_handler.entity_type = 'node' and dispatch_handler.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_log_handler worker_handler on worker_handler.entity_type = 'node' and worker_handler.entity_id = node.nid ";

		$sql .= "left join node worker_node on worker_node.type = 'sirius_worker' and worker_node.nid = worker_handler.field_sirius_log_handler_target_id ";

		$sql .= "left join node dispatch_node on dispatch_node.type = 'sirius_dispatch' and dispatch_node.nid = dispatch_handler.field_sirius_log_handler_target_id ";
		$sql .= "left join field_data_field_sirius_dispatch_job on field_data_field_sirius_dispatch_job.entity_type = 'node' and field_data_field_sirius_dispatch_job.entity_id = dispatch_node.nid ";

		$sql .= "left join node job_node on job_node.type = 'sirius_dispatch_job' and job_node.nid = field_sirius_dispatch_job_target_id ";
		$sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = job_node.nid ";

		$sql .= "where node.type = 'sirius_log' ";
		$sql .= "and field_sirius_category_value = 'tos' ";

		if ($domain_nid) {
			$sql .= "and field_sirius_domain_target_id = :domain_nid ";
			$sql_args[':domain_nid'] = $domain_nid;
		} else {
			$sql .= "and field_sirius_domain_target_id IS NULL ";
		}

		if ($params['worker_nid']) {
			$sql .= "and worker_node.nid = :worker_nid ";
			$sql_args[':worker_nid'] = $params['worker_nid']; 
		}

		if ($params['status']) {
			$sql .= "and field_sirius_type_value = :type ";
			$sql_args[':type'] = $params['status']; 
		}

		if ($params['statuses']) {
			$sql .= "and field_sirius_type_value in (:statuses) ";
			$sql_args[':statuses'] = $params['statuses']; 
		}

		if ($params['start_ts']) {
			$sql .= "and node.created >= :start_ts ";
			$sql_args[':start_ts'] = $params['start_ts'];
		}
		if ($params['end_ts']) {
			$sql .= "and node.created <= :end_ts ";
			$sql_args[':end_ts'] = $params['end_ts'];
		}

		$sql .= "order by node.created desc ";
		$sql .= "limit 100";
		$stmt = sirius_sql_query($sql, $sql_args);

		$tos_nids = array();
		while ($hr = $stmt->fetchAssoc()) {
			$tos_nids[] = $hr['nid'];
		}

		if ($params['nids_only']) { return $tos_nids; }

		return node_load_multiple($tos_nids);
	}

	public function start_ts($tos_node) {
		return $tos_node->created;
	}

	public function end_ts($tos_node) {
		$tos_json = sirius_json_get($tos_node);
		return $tos_json['worker_tos']['cancel']['ts'];
	}

	public function render_list($tos_nodes, $options = array()) {
		if (!$tos_nodes) { return '<p>' . t('No records found.') . '<p>'; }

		foreach ($tos_nodes as $tos_node) {
			if (!$options['hide_worker']) {
				$worker_node = $this->worker($tos_node);
			}
			$start_ts = $this->start_ts($tos_node);
			if ($start_ts) { 
				$start_ts_render = date('Y-m-d h:i a', $start_ts);
			} else {
				$start_ts_render = '';
			}
			$end_ts = $this->end_ts($tos_node);
			if ($end_ts) { 
				$end_ts_render = date('Y-m-d h:i a', $end_ts);
			} else {
				$end_ts_render = '';
			}

			if ($start_ts && $end_ts) {
				$duration_render = format_interval($end_ts - $start_ts);
			} else {
				$duration_render = format_interval(time() - $start_ts);
			}

			$tools = array();
			if (!$options['hide_worker']) {
				$tools[] = l('Worker', 'node/' . $worker_node->nid . '/sirius_worker_schedule/tos-archive', array('attributes' => array('class' => array('tool-button'))));
			}
			$tools[] = l('Record', 'node/' . $tos_node->nid . '/sirius_worker_tos', array('attributes' => array('class' => array('tool-button'))));

			$row = array();
			if (!$options['hide_worker']) { $row[] = $worker_node->title; }
			$row[] = $this->status_label($this->status($tos_node));
			$row[] = $start_ts_render;
			$row[] = $end_ts_render;
			$row[] = $duration_render;
			$row[] = join('', $tools);
			$rows[] = $row;
		}

		$header = array();
		if (!$options['hide_worker']) { $header[] = t('Worker'); }
		$header[] = t('Status');
		$header[] = t('Start');
		$header[] = t('End');
		$header[] = t('Duration');
		$header[] = t('Tools');

		return sirius_table_render($header, $rows);
	}

	public function cron_reminders($mode, $force) {
		// Make sure reminders are enabled
		if (sirius_domain_variable_get('sirius_worker_tos_reminder_active') != 'Yes') {
			sirius_minilog("Reminders are not enabled for this domain.");
			return array('success' => TRUE);
		}

		// Check do not disturb
		if (!$force) {
			$result = sirius_do_not_disturb_check();
			if (!$result['success']) {
				sirius_minilog("Cannot send messages now: " . $result['msg'], 'warning');
				return array('success' => TRUE);
			}
		}

		$search_params = array();
		$search_params['status'] = 'active';
		$tos_nodes = $this->search($search_params);
		if (!$tos_nodes) {
			sirius_minilog("There are no active TOS records.");
			return array('success' => TRUE);
		}

		sirius_minilog("Found " . count($tos_nodes) . " active TOS records."); 

		$reminders = array(
			1 => array(
				'start' => 24*60*60,
				'end' => 24*60*60*2 - 60*60
			),
			2 => array(
				'start' => 24*60*60*2,
				'end' => 24*60*60*3 - 60*60
			),
			3 => array(
				'start' => 24*60*60*3,
				'end' => 24*60*60*4 - 60*60
			),
			4 => array(
				'start' => 24*60*60*6,
				'end' => 24*60*60*10
			),
		);

		global $base_url;
		foreach ($tos_nodes as $tos_nid => $tos_node) {
			$delta = time() - $tos_node->created;

			foreach ($reminders as $which => $reminder) {
				if ($delta < $reminder['start']) { continue; }
				if ($delta > $reminder['end']) { continue; }

				$worker_node = $this->worker($tos_node);
				if (!$worker_node) { continue; }

				$url = $base_url;
				$url .= "/node/" . $worker_node->nid . "/sirius_worker_schedule/tos";

				$subject = sirius_domain_variable_get('sirius_worker_tos_reminder_subject');
				$body = sirius_domain_variable_get('sirius_worker_tos_reminder_body');
				$body .= "<br /><br />";
				$body .= "Absent for: " . format_interval($delta);
				$body .= "<br />";
				$body .= "<a href=\"$url\">Manage</a>";

				$sms = sirius_domain_variable_get('sirius_worker_tos_reminder_sms');
				$sms .= "\n\n";
				$sms .= "Absent for: " . format_interval($delta);
				$sms .= "\n";
				$sms .= $url;

				$unique_id = "reminder/$tos_nid/$which";

				sirius_minilog("Sending reminder #$which to " . $worker_node->title);

				if ($mode == 'live') {
					$result = sirius_worker_notify(
						$worker_node, 
						array(
							'subject' => $subject,
							'body' => $body,
							'sms' => $sms,
							'X-Sirius-Id' => $unique_id,
						)
					);
					if (!$result['success']) {
						sirius_minilog("... Delivery failed: " . $result['msg']);
					}
				} else {
					sirius_minilog("(Skipping delivery because mode = $mode.)");
				}
			}
		}

		return array('success' => TRUE);
	}
}
