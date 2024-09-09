<?php

function sirius_node_tab_worker_password_access($worker_node) {
	// "user sync" must be enabled
	if (!sirius_worker_usersync_enabled()) { return FALSE; }

  // Admins can edit
  if (user_access('sirius worker admin')) { return TRUE; }

  // Workers can edit
  $my_worker_node = sirius_worker_get($account->uid);
  if ($my_worker_node && $node && $my_worker_node->nid == $node->nid) { return TRUE; }

  return FALSE;
}

function sirius_node_tab_worker_password_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;
  module_load_include('inc', 'field_ui', 'field_ui.admin');

  $form['#tree'] = TRUE;

	$form['aat'] = array(
		'#title' => t('Password'),
		'#type' => 'password',
		'#description' => t('Enter a password for your account.'),
		'#required' => TRUE,
		'#size' => 6,
	);

	if (sirius_domain_component_enabled('phonetree')) {
		$suffix = '';
		if (preg_match('/[^0-9]/', $worker_node->field_sirius_aat['und'][0]['value'])) {
			$form['aat']['#description'] .= ' (Keypad: <strong>' . sirius_phone_keypad($worker_node->field_sirius_aat['und'][0]['value']) . '</strong>)';
		}

		$form['aat_required'] = array(
			'#title' => t('Password required for phone access?'),
			'#type' => 'select',
			'#options' => array(
				'No' => t('No'),
				'Yes' => t('Yes'),
			),
			'#default_value' => $worker_node->field_sirius_aat_required['und'][0]['value'],
			'#description' => t('"No" means that you will be able to use the phone system by entering only your worker ID. "Yes" means that you will also have to enter your password (above).'),
			'#required' => TRUE,
		);
	}

	
  $form['submit'] = array(
  	'#type' => 'submit',
  	'#value' => t('Set Password'),
  );

  return $form;
}

function sirius_node_tab_worker_password_form_validate($form, &$form_state) {
	$aat = $form_state['values']['aat'];
	if (strlen($aat) < 6) {
		form_set_error('aat', t('Please enter a password that is at least six characters long.'));
	}
}

function sirius_node_tab_worker_password_form_submit($form, &$form_state) {
	$worker_node = node_load($form_state['worker_nid']);
	global $user;

	// AAT Required
	$val = $form_state['values']['aat_required'];
	$oldval = $worker_node->field_sirius_aat_required['und'][0]['value'];
	if ($val != $oldval) {
		$worker_node->field_sirius_aat_required['und'][0]['value'] = $val;
		$msg = t('Changed (password required for phone access) from @oldval to @val', array('@oldval' => ($oldval?$oldval:"[NULL]"), '@val' => ($val?$val:"[NULL]")));
		sirius_log('worker:contact', 'update', $msg, '', '', array($worker_node->nid), array());
		drupal_set_message($msg);
	}

	// AAT
	$val = $form_state['values']['aat'];
	$worker_node->field_sirius_aat['und'][0]['value'] = $val;
	$msg = t('Changed password.');
	sirius_log('worker:contact', 'update', $msg, '', '', array($worker_node->nid), array());

	node_save($worker_node);
	drupal_set_message("Password updated.");
}
