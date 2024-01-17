<?php

function sirius_edls_page_notification($notification_uuid) {
	return drupal_get_form('sirius_edls_page_notification_form', $notification_uuid);
}

function sirius_edls_page_notification_form($form, &$form_state, $notification_uuid) {
  drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');
	drupal_add_css(drupal_get_path('module', 'sirius_edls') . '/css/sheet.css');

	$form_state['notification_uuid'] = $notification_uuid;

	$notification = sirius_edls_notify_fetch_by_uuid($notification_uuid);
	$access = sirius_access_instance('edls');
	if (!$access->access('notification_view', array('notification' => $notification, 'notification_uuid' => $notification_uuid))) {
		return sirius_form_error('Access denied.');
	}

	$sheet_node = node_load($notification['sheet_nid']);
	if (!$sheet_node) { return sirius_form_error("Sheet not found."); }

	$old_domain = sirius_domain_nid();
	if (!$old_domain) { $old_domain = -1; }
	$new_domain = $sheet_node->field_sirius_domain['und'][0]['target_id'];
	if (!$new_domain) { $new_domain = -1; }
	sirius_domain($new_domain);

	$form['view'] = array('#markup' => sirius_edls_notify_render($notification));

	if ($access->access('notification_accept', array('notification' => $notification, 'notification_uuid' => $notification_uuid))) {
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Accept'),
		);
	}

	sirius_domain($old_domain);

	return $form;
}

function sirius_edls_page_notification_form_submit($form, &$form_state) {
	$notification_uuid = $form_state['notification_uuid'];
	$notification = sirius_edls_notify_fetch_by_uuid($notification_uuid);

	$old_domain = sirius_domain_nid(); 
	if (!$old_domain) { $old_domain = -1; }
	$new_domain = $sheet_node->field_sirius_domain['und'][0]['target_id'];
	if (!$new_domain) { $new_domain = -1; }
	sirius_domain($new_domain);

	$access = sirius_access_instance('edls');
	if (!$access->access('notification_accept', array('notification' => $notification, 'notification_uuid' => $notification_uuid))) {
		drupal_set_message("Access denied.", 'error');
	}

	$result = sirius_edls_notify_accept($notification['notification_id']);
	if ($result['success']) {
		drupal_set_message("Assignment accepted.");
	} else {
		drupal_set_message("Error: " . $result['msg'], 'error');
	}

	sirius_domain($old_domain);
}
