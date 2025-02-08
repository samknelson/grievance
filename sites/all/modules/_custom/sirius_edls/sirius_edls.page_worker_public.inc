<?php

function sirius_edls_page_worker_public($notification_uuid) {
	return drupal_get_form('sirius_edls_page_worker_public_form', $notification_uuid);
}

function sirius_edls_page_worker_public_form($form, &$form_state, $notification_uuid) {
  drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');
	drupal_add_css(drupal_get_path('module', 'sirius_edls') . '/css/sheet.css');

	$form['#tree'] = TRUE;

	// Access to this page is a little special. We must be passed in a notification UUID. 
	// Based on that, we look up the worker, the sheet, and the employer, and provide all UPCOMING assignments for that worker.
	$access = sirius_access_instance('edls');
	if (!$notification_uuid) { return sirius_form_error("No notification UUID."); }
	$notification = sirius_edls_notify_fetch_by_uuid($notification_uuid);
	if (!$notification) { return sirius_form_error("Notification not found."); }
	if (!$access->access('notification_view', array('notification' => $notification, 'notification_uuid' => $notification_uuid))) { 
		return sirius_form_error('Access denied: ' . $access->error_msg());
	}
	$sheet_node = node_load($notification['sheet_nid']);
	if (!$sheet_node) { return sirius_form_error("Sheet not found."); }

	$search_params = array();
	$search_params['grievance_shop'] = $sheet_node->field_grievance_shop['und'][0]['target_id'];
	$search_params['date_min'] = date('Y-m-d');
	$search_params['worker_id'] = $notification['worker_id'];
	$search_params['statuses'] = array('lock', 'reserved');
	$all_assignments = sirius_edls_assignment_search($search_params);

	$old_domain = sirius_domain_nid();
	if (!$old_domain) { $old_domain = -1; }
	$new_domain = $sheet_node->field_sirius_domain['und'][0]['target_id'];
	if (!$new_domain) { $new_domain = -1; }
	sirius_domain($new_domain);

	if (!$all_assignments) {
		$form['view'] = array('#markup' => '<p>' . t("No upcoming asignments found.") . '</p>');
		return $form;
	}

	$can_submit = FALSE;
	foreach ($all_assignments as $assignment_id => $assignment) {
		$form['assignments'][$assignment_id] = array(
			'#type' => 'fieldset',
			'#title' => '<h2>Assignment for: ' . date('l, F j, Y', strtotime($assignment['assignment_date'])) . '</h2>',
		);

		$form['assignments'][$assignment_id]['view'] = array('#markup' => sirius_edls_assignment_render($assignment));

		$notification = sirius_edls_notify_fetch_by_assignment_id($assignment_id);

		if ($notification) {
			$form['assignments'][$assignment_id]['notification'] = array('#markup' => sirius_edls_notify_render_status_long($assignment, $notification));


			if ($access->access('notification_view_delivery')) {
				$form['assignments'][$assignment_id]['notification_delivery'] = array('#markup' => l('Delivery Details', 'sirius/edls/notification/' . $notification['notification_uuid'], array('attributes' => array('class' => array('tool-button')))));
			}


			$actions = array();
			if ($access->access('notification_accept', array('notification' => $notification, 'notification_uuid' => $notification['notification_uuid']))) {
				$actions['accept'] = t('Accept');
			}
			if ($access->access('notification_decline', array('notification' => $notification, 'notification_uuid' => $notification['notification_uuid']))) {
				$actions['decline'] = t('Decline');
			}

			if ($actions) {
				$can_submit = TRUE;
				$form['assignments'][$assignment_id]['notification_uuid'] = array('#type' => 'value', '#value' => $notification['notification_uuid']);

				$form['assignments'][$assignment_id]['action'] = array(
					'#type' => 'select',
					'#options' => array(NULL => t('-- Select --')) + $actions,
					'#title' => 'Action',
				);
			}

		}
	}

	if ($can_submit) {
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);
	}

	sirius_domain($old_domain);

	return $form;
}

function sirius_edls_page_worker_public_form_submit($form, &$form_state) {
	if (!$form_state['values']['assignments']) { return; }

	foreach ($form_state['values']['assignments'] as $assignment_id => $tmp) {
		$notification_uuid = $tmp['notification_uuid'];
		$action = $tmp['action'];
		if (!$action) { continue; }
		$notification = sirius_edls_notify_fetch_by_uuid($notification_uuid);

		$old_domain = sirius_domain_nid(); 
		if (!$old_domain) { $old_domain = -1; }
		$new_domain = $sheet_node->field_sirius_domain['und'][0]['target_id'];
		if (!$new_domain) { $new_domain = -1; }
		sirius_domain($new_domain);

		$access = sirius_access_instance('edls');
		if ($action == 'accept') { 
			if (!$access->access('notification_accept', array('notification' => $notification, 'notification_uuid' => $notification_uuid))) {
				drupal_set_message("Access denied: " . $access->error_msg(), 'error');
				continue;
			}
			$result = sirius_edls_notify_accept($notification['notification_id']);
		} else if ($action == 'decline') {
			if (!$access->access('notification_decline', array('notification' => $notification, 'notification_uuid' => $notification_uuid))) {
				drupal_set_message("Access denied: " . $access->error_msg(), 'error');
				continue;
			}
			$result = sirius_edls_notify_decline($notification['notification_id']);
		}

		if ($result['success']) {
			drupal_set_message($result['msg']);
		} else {
			drupal_set_message("Error: " . $result['msg'], 'error');
		}

		sirius_domain($old_domain);
	}
}
