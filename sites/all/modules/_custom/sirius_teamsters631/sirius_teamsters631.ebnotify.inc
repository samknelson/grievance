<?php

/**
 * Send notifications to specific email addresses when the dispatch roles associated with certain member statuses are changed.
 */

function sirius_teamsters631_ebnotify_admin() {
  $form = array();

  $form['sirius_teamsters631_ebnotify'] = array(
    '#type' => 'textarea',
    '#title' => t('Extraboard Notifications'),
    '#default_value' => variable_get('sirius_teamsters631_ebnotify', ''),
    '#description' => t('Enter the email addresses (one per line) that should receive notifications when the skilled or unskilled extraboard lists are opened or closed to employers.'),
  );

  return system_settings_form($form);
}

function sirius_teamsters631_taxonomy_term_update_ebnotify($term) {
	// Get our list of notifications
	$recipients = preg_split('/\s+/', variable_get('sirius_teamsters631_ebnotify', ''));
	if (!$recipients) { return; }

	// Only interested in changes to the field sirius_dispatch_roles
	$new_val = $old_val = array();
	if ($term->field_sirius_dispatch_roles['und']) {
		foreach ($term->field_sirius_dispatch_roles['und'] as $tmp) {
			$new_val[$tmp['value']] = TRUE;
		}
	}

	if ($term->original->field_sirius_dispatch_roles['und']) {
		foreach ($term->original->field_sirius_dispatch_roles['und'] as $tmp) {
			$old_val[$tmp['value']] = TRUE;
		}
	}

	if ($new_val == $old_val) { return; }

	global $base_url;
	$subject = "Sirius Dispatch - List opened / close notification - " . $term->name;

	$body = "An administrator has opened or closed the <strong>" . $term->name . "</strong> list.<br /><br />";
	$body .= "The PREVIOUS set of roles with permission to dispatch from this list was:";
	$body .= '<ul>';
	foreach ($old_val as $rid => $ignore) {
		$role = user_role_load($rid);
		$body .= '<li>' . $role->name . '</li>';
	}
	$body .= '</ul>';
	$body .= "The NEW set of roles is:";
	$body .= '<ul>';
	foreach ($new_val as $rid => $ignore) {
		$role = user_role_load($rid);
		$body .= '<li>' . $role->name . '</li>';
	}
	$body .= '</ul>';

	global $user;
	foreach ($recipients as $email) {
		if ($email == $user->mail) { continue; }

		sirius_email_send(
			array(
				'to' => $email,
				'subject' => $subject,
				'body' => $body,
			)
		);
		drupal_set_message("Sent a notification to $email: $subject");
	}
}