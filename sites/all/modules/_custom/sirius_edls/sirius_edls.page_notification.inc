<?php

function sirius_edls_page_notification($notification_uuid) {
  drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');
	drupal_add_css(drupal_get_path('module', 'sirius_edls') . '/css/sheet.css');

	$access = sirius_access_instance('edls');

	if (!$notification_uuid) { return sirius_form_error("No notification UUID."); }
	$form_state['notification_uuid'] = $notification_uuid;

	$notification = sirius_edls_notify_fetch_by_uuid($notification_uuid);
	if (!$notification) { return sirius_form_error("Notification not found."); }

	if (!$access->access('notification_view', array('notification' => $notification))) { 
		return sirius_form_error('Access denied.');
	}

	$assignment = sirius_edls_notify_fetch_assignment($notification);
	$html = sirius_edls_notify_render_status_long($assignment, $notification);
	$html .= sirius_edls_notify_render_delivery($notification);

	return $html;
}
