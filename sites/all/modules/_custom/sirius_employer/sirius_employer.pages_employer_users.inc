<?php

function sirius_employer_node_tab_employer_users_access($employer_node) {
	// Admins can access
	if (user_access('sirius employer admin')) { return TRUE; }

	// Employers can access their own page only
	if (user_access('sirius employer user manager')) { 
		if (!$employer_node->nid) { return FALSE; }
		global $user;
		$user_full = user_load($user->uid);
		if ($user_full->field_grievance_shops['und'][0]['target_id'] == $employer_node->nid) { return TRUE; }
	}

	// Oh well.
	return FALSE;
}

function sirius_employer_node_tab_employer_users_add_access($employer_node) {
	return sirius_employer_node_tab_employer_users_access($employer_node);
}

function sirius_employer_node_tab_employer_users($employer_node) {
	if ($_REQUEST['action'] == 'add') {
		return drupal_get_form('sirius_employer_node_tab_employer_users_add_form', $employer_node);
	}

	if ($_REQUEST['action'] == 'edit') {
		return drupal_get_form('sirius_employer_node_tab_employer_users_add_form', $employer_node);
	}

	$roles = sirius_employer_usermanage_roles();
	$html = views_embed_view('sirius_employer_users', 'block', $employer_node->nid, join('+', array_keys($roles)));
	return $html;
}

function sirius_employer_node_tab_employer_users_add($employer_node) {
	return drupal_get_form('sirius_employer_node_tab_employer_users_add_form', $employer_node);
}

function sirius_employer_node_tab_employer_users_add_form($form, &$form_state, $employer_node) {
	$form_state['employer_node'] = $employer_node;

	$new_uid = $_REQUEST['uid'];
	if ($new_uid) {
		$new_user = user_load($new_uid);
		if (!$new_user) {
			drupal_set_message("User not found.", 'error');
			return array();
		}

		if ($new_user->field_grievance_shops['und'][0]['target_id'] != $employer_node->nid) {
			drupal_set_message("The user is not associated with this shop.", 'error');
			return array();
		}

		$roles = sirius_employer_usermanage_roles();
		if (!$roles || !array_intersect(array_keys($roles), array_keys($new_user->roles))) {
			drupal_set_message("The user does not have one of the required roles, and cannot be edited in this form.");
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

	$roles = sirius_employer_usermanage_roles('optional');
	if ($roles) {
		$form['roles'] = array(
			'#type' => 'checkboxes',
			'#title' => 'Roles',
			'#options' => $roles,
		);
		if ($roles && $new_user) {
			$form['roles']['#default_value'] = array_intersect(array_keys($roles), array_keys($new_user->roles));
		}
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

function sirius_employer_node_tab_employer_users_add_form_validate($form, &$form_state) {
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

function sirius_employer_node_tab_employer_users_add_form_submit($form, &$form_state) {
	$employer_node = $form_state['employer_node'];
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
	$new_user->field_grievance_shops['und'][0]['target_id'] = $employer_node->nid;
	$new_user->field_grievance_phone['und'][0]['value'] = $phone;

	$domain_nid = $employer_node->field_sirius_domain['und'][0]['target_id'];
	if ($domain_nid) { $new_user->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }

	$all_roles = user_roles();
	$required_roles = sirius_employer_usermanage_roles('required');
	foreach ($required_roles as $rid => $role) {
		$new_user->roles[$rid] = $role;
	}
	if ($form_state['values']['roles']) {
		foreach ($form_state['values']['roles'] as $rid => $checked) {
			if ($checked) { 
				$new_user->roles[$rid] = $all_roles[$rid];
			} else {
				unset($new_user->roles[$rid]);
			}
		}
	}

	$new_user->status = $form_state['values']['status'];

	$new_user = user_save($new_user, $edit);
	//dpm($new_user);
	// $form_state['rebuild'] = TRUE;
	// return;

	if ($new_uid) {
		drupal_set_message(t('User @name has been updated.', array('@name' => $new_user->name)));
	} else {
		drupal_set_message(t('User @name has been created.', array('@name' => $new_user->name)));
	}
	drupal_goto('node/' . $employer_node->nid . '/employer_users');
}

function sirius_employer_usermanage_admin() {
  $form = array();

  $roles = user_roles(TRUE);
  foreach ($roles as $rid => $role) {
  	$form["sirius_employer_userrmanage_$rid"] = array(
  		'#type' => 'select',
  		'#title' => $role,
  		'#options' => array(
  			'omit' => 'Omit',
  			'required' => 'Required',
  			'optional' => 'Optional',
  		),
  		'#default_value' => variable_get("sirius_employer_userrmanage_$rid", 'omit'),
  	);
  }

  return system_settings_form($form);
}

function sirius_employer_usermanage_roles($which = NULL) {
	$all_roles = user_roles();
	$roles = array();
	foreach ($all_roles as $rid => $role) {
		$avail = variable_get("sirius_employer_userrmanage_$rid", 'omit');
		if ($which && ($avail != $which))  { continue; }
		if (!$which && ($avail == 'omit')) { continue; }
		$roles[$rid] = $role;
	}

	return $roles;
}