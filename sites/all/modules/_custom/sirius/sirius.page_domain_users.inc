<?php

function sirius_page_domain_users() {
	$domain_node = sirius_domain();
	if (!$domain_node) { return 'No domain .'; }
	drupal_goto('node/' . $domain_node->nid . '/sirius_domain_users');
}


function sirius_node_tab_sirius_domain_users_access($domain_node) {
	if (user_access('sirius admin')) { return TRUE; }
	if (!user_access('sirius domain admin')) { return FALSE; }
	return sirius_domain_access($domain_node->nid);
}

function sirius_node_tab_sirius_domain_users($domain_node) {
	if ($_REQUEST['action'] == 'add') {
		return drupal_get_form('sirius_node_tab_domain_users_add_form', $domain_node);
	}

	if ($_REQUEST['action'] == 'edit') {
		return drupal_get_form('sirius_node_tab_domain_users_add_form', $domain_node);
	}

	$html = views_embed_view('sirius_domain_users', 'block', $domain_node->nid);
	return $html;
}

function sirius_node_tab_domain_users_add($domain_node) {
	return drupal_get_form('sirius_node_tab_domain_users_add_form', $domain_node);
}

function sirius_node_tab_domain_users_add_form($form, &$form_state, $domain_node) {
	$form_state['domain_node'] = $domain_node;

	$new_uid = $_REQUEST['uid'];
	if ($new_uid) {
		$new_user = user_load($new_uid);
		if (!$new_user) {
			drupal_set_message("User not found.", 'error');
			return array();
		}

		if ($new_user->field_sirius_domain['und'][0]['target_id'] != $domain_node->nid) {
			drupal_set_message("The user is not associated with this shop.", 'error');
			return array();
		}

		$form['new_uid'] = array(
			'#type' => 'value',
			'#value' => $new_uid,
		);
	}

	$form['name'] = array(
		'#type' => 'textfield', 
		'#title' => 'User Name',
		'#description' => 'Spaces are allowed; punctuation is not allowed except for periods, hyphens, apostrophes, and underscores.',
		'#required' => TRUE,
		'#default_value' => $new_user->name,
	);

	$form['mail'] = array(
		'#type' => 'textfield', 
		'#title' => 'Email Address',
		'#required' => TRUE,
		'#default_value' => $new_user->mail,
	);

	$form['phone'] = array(
		'#type' => 'textfield', 
		'#title' => 'Mobile Phone',
		'#required' => TRUE,
		'#default_value' => $new_user->field_grievance_phone['und'][0]['value'],
	);

	$form['password'] = array(
		'#type' => 'password', 
		'#title' => 'Password',
		'#required' => ($new_uid ? FALSE : TRUE),
	);

	$form['passwordv'] = array(
		'#type' => 'password', 
		'#title' => 'Password',
		'#required' => TRUE,
		'#required' => ($new_uid ? FALSE : TRUE),
	);

	$form['status'] = array(
		'#type' => 'select', 
		'#title' => 'Status',
		'#options' => array(
			1 => 'Active',
			0 => 'Blocked',
		),
		'#required' => TRUE,
		'#default_value' => $new_user->status,
	);

	$roles = user_roles();
	$roles_avail = array();
	$roles_default = array();
	foreach ($roles as $rid => $role) {
		$avail = variable_get("sirius_domain_userrmanage_$rid", 'omit');
		if ($avail == 'optional') {
			$roles_avail[$rid] = $role;
			if ($new_user->roles[$rid]) {
				$roles_default[] = $rid;
			}
		}
	}
	if ($roles_avail) {
		$form['roles'] = array(
			'#type' => 'checkboxes',
			'#title' => 'Roles',
			'#options' => $roles_avail,
			'#default_value' => $roles_default,
		);
	}

	if ($new_uid) {
		$form['submit'] = array(
			'#type' => 'submit', 
			'#value' => 'Edit This User',
		);
	} else {
		$form['submit'] = array(
			'#type' => 'submit', 
			'#value' => 'Add This User',
		);
	}

	return $form;
}

function sirius_node_tab_domain_users_add_form_validate($form, &$form_state) {
	if ($form_state['values']['password'] != $form_state['values']['passwordv']) {
		form_set_error('password', 'The passwords do not match.');
	}
	$new_uid = $form_state['values']['new_uid'];

	$mail = sirius_email_format($form_state['values']['mail']);
	$name = strtolower(trim($form_state['values']['name']));
	$phone = sirius_phone_format($form_state['values']['phone']);

	if (!sirius_email_validate($mail)) {
		form_set_error('mail', 'Please enter a valid email address.');
	}

	if (!sirius_phone_validate($phone)) {
		form_set_error('phone', 'Please enter a valid phone number.');
	}

	$sql = "select * from users where name = :name";
	$stmt = db_query($sql, array(':name' => $name));
	$hr = $stmt->fetchAssoc();
	if ($hr && ($hr['uid'] != $new_uid)) {
		form_set_error('name', 'There is already a user with that name.');
	}

	$sql = "select * from users where mail = :mail";
	$stmt = db_query($sql, array(':mail' => $mail));
	$hr = $stmt->fetchAssoc();
	if ($hr &&  ($hr['uid'] != $new_uid)) {
		form_set_error('mail', 'There is already a user with that email address.');
	}
}

function sirius_node_tab_domain_users_add_form_submit($form, &$form_state) {
	$domain_node = $form_state['domain_node'];
	$mail = sirius_email_format($form_state['values']['mail']);
	$name = strtolower(trim($form_state['values']['name']));
	$phone = sirius_phone_format($form_state['values']['phone']);
	$password = $form_state['values']['password'];

	$new_uid = $form_state['values']['new_uid'];
	if ($new_uid) {
		$new_user = user_load($new_uid);
	} else {
	 	$new_user = new stdClass();
		$new_user->is_new = TRUE;
	}

	$edit = array();
	if ($password) {
		$edit['pass'] = $password;
	}

	$new_user->mail = $mail;
	$new_user->name = $name;
	$new_user->field_sirius_domain['und'][0]['target_id'] = $domain_node->nid;
	$new_user->field_grievance_phone['und'][0]['value'] = $phone;
	$roles = user_roles();
	foreach ($roles as $rid => $role) {
		$avail = variable_get("sirius_domain_userrmanage_$rid", 'omit');
		if ($avail == 'required') { 
			$new_user->roles[$rid] = $role;
		}

		if ($avail == 'optional') {
			if ($form_state['values']['roles'][$rid]) { 
				$new_user->roles[$rid] = $role;
			} else {
				unset($new_user->roles[$rid]);
			}
		}
	}
	$new_user->status = $form_state['values']['status'];
	if ($domain_node->field_sirius_tz['und'][0]['value']) { $new_user->timezone = $domain_node->field_sirius_tz['und'][0]['value']; }

	$new_user = user_save($new_user, $edit);
	//dpm($new_user);
	//$form_state['rebuild'] = TRUE;
	//return;

	if ($new_uid) {
		drupal_set_message(t('User @name has been updated.', array('@name' => $new_user->name)));
	} else {
		drupal_set_message(t('User @name has been created.', array('@name' => $new_user->name)));
	}
	drupal_goto('node/' . $domain_node->nid . '/sirius_domain_users');
}

function sirius_domain_usermanage_admin() {
  $form = array();

  $roles = user_roles(TRUE);
  foreach ($roles as $rid => $role) {
  	$form["sirius_domain_userrmanage_$rid"] = array(
  		'#type' => 'select',
  		'#title' => $role,
  		'#options' => array(
  			'omit' => 'Omit',
  			'required' => 'Required',
  			'optional' => 'Optional',
  		),
  		'#default_value' => variable_get("sirius_domain_userrmanage_$rid", 'omit'),
  	);
  }

  return system_settings_form($form);
}

/**
 * Modify the user view to show only the roles that we have access to within the domain user manager
 */

function sirius_domain_users_view_form_alter(&$form, &$form_state) {
	$roles = user_roles();
	foreach ($roles as $rid => $role) {
		$avail = variable_get("sirius_domain_userrmanage_$rid", 'omit');
		if ($avail == 'omit') { 
			unset($form['rid']['#options'][$rid]);
		}
	}
}