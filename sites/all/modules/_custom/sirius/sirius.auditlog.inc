<?php

/**
 * @todo: I'm hardcoding everything for now, but long term it should have lots of fun configuration and callback and hook_alters.
 *
 * At the moment, this is only a roughed-out framework with just one implementation, the "seniority list" item. It's possible
 * that we're going to want additional regular audit logs created, but we'll cross that bridge when we come to it.
 */

/**
 * Eventually, a function with callbacks to allow modules to define their own audit log functions.
 * For the moment, just a hardcoded entry.
 */

function sirius_auditlog_info() {
	$info = array(
		'dispatch_seniority' => array(
			'title' => 'Nightly Seniority List',
			'schedule' => '1 15 * * *', /* Unused */
			'callback' => 'sirius_dispatch_auditlog_viewfetch',
			'callback_args' => array('sirius_dispatch_auditlog_seniority', 'views_data_export_1'),
		),
	);

	return $info;
}

function sirius_auditlog($force = FALSE) {
	sirius_become_superuser(TRUE);

	$all_info = sirius_auditlog_info();
	foreach ($all_info as $type => $info) {
		if (variable_get("sirius_auditlog_enable_$type", '') != 'Yes') { continue; }

		// @todo: Implement a more versatile schedule process.
		$today = date('Y-m-d');
		$last_run = variable_get('sirius_auditlog_lastrun_' . $type, '');
		if (!$force && ($last_run == $today)) { continue; }
		variable_set('sirius_auditlog_lastrun_' . $type, $today);

		$callback = $info['callback'];
		$args = $info['callback_args'];
		$title = $info['title'];

		// @todo: How to pass all the args?  I'm just hardcoding two at the moment.
		$data = $callback($args[0], $args[1]);

		// @todo: Data might be stored as a file, but do we also want to be able to drop it into JSON or something?
		// @todo: is the file always CSV?
		$file = file_save_data($data, 'private://' . 'tmp_auditlog_' . $type . '_' . time() . '.csv', FILE_EXISTS_RENAME);
		$file->display = 1;

		sirius_log(
			'auditlog', 
			$type, 
			$title, 
			'Auditlog generated at ' . date('c'),
			'', 
			'', 
			array((array) $file),
			array(),
			NULL
		);
	}

	sirius_become_superuser(FALSE);
}

function sirius_dispatch_auditlog_viewfetch($view_name, $display_id) {
	return views_embed_view($view_name, $display_id);
}

function sirius_auditlog_config_form($form, &$form_state) {
	$all_info = sirius_auditlog_info();
	foreach ($all_info as $type => $info) {
		$form["sirius_auditlog_enable_$type"] = array(
			'#type' => 'select',
			'#options' => array(
				NULL => '-- Select --',
				'No' => 'No',
				'Yes' => 'Yes',
			),
			'#default_value' => variable_get("sirius_auditlog_enable_$type", ''),
			'#title' => "Enable auditlog for $info[title] ($type)?",
		);
	}

	return system_settings_form($form);
}