<?php

function sirius_dashboard_page_item($rid, $uuid) {
	return drupal_get_form('sirius_dashboard_page_item_form', $rid, $uuid);
}

function sirius_dashboard_page_item_form($form, &$form_state, $rid, $uuid) {
  drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/event.css', 'file');

  $form['#tree'] = TRUE;
	$form_state['rid'] = $rid;
	$form_state['uuid'] = $uuid;

	$roles = user_roles(TRUE);
	sirius_backlink_add('sirius/dashboard/items_by_role/' . $rid, array('name' => $roles[$rid]));
	sirius_backlink_add('sirius/dashboard/items_by_role', array('name' => 'All Roles'));
	sirius_backlink_add('sirius/dashboard', array('name' => 'Dashboard'));
	$json = sirius_dashboard_items_by_role($rid);

	$title = $json['item_list'][$uuid]['title'];
	drupal_set_title('Dashboard Item Preferences: ' . $title);

	$plugin_key = $json['item_list'][$uuid]['plugin'];
	if (!$plugin_key) {
		drupal_set_message("No plugin has been assigned to this dashboard item.", 'warning');
		return array();
	}

	$plugin_info = sirius_dashboard_plugin_info($plugin_key);
	$callback = $plugin_info['callbacks']['prefs'];
	if (!$callback) {
		drupal_set_message("There are no preferences for this dashboard item.", 'warning');
		return array();
	}
	sirius_require($plugin_info['module'], $plugin_info['file']);
	$form['preferences'] = $callback($json['item'][$uuid]['preferences']);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_dashboard_page_item_form_submit($form, &$form_state) {
	$rid = $form_state['rid'];
	$uuid = $form_state['uuid'];

	$json = json_decode(sirius_domain_variable_get("sirius_dashboard_items_by_rid_$rid"), TRUE);
	$json['item'][$uuid]['preferences'] = $form_state['values']['preferences'];
	sirius_domain_variable_set("sirius_dashboard_items_by_rid_$rid", json_encode($json));

	drupal_set_message("Items saved.");
}

