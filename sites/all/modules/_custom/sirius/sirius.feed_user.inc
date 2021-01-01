<?php

function sirius_feed_user_info() {
  $items = array();
  $items['feed_user'] = array(
    'name' => 'feed_user',
    'title' => 'Users',
    'description' => 'Create or update users. Users are uniquely identified by an email address, and have a user name and password that allow them to sign in to the system.',
    'steps' => array(
      'upload'    => array('label' => 'Upload',     'callback' => 'sirius_feed_wizard_upload'),
      'map'       => array('label' => 'Mapping',    'callback' => 'sirius_feed_wizard_map'),
      'options'   => array('label' => 'Options',    'callback' => 'sirius_feed_user_options'),
      'verify'    => array('label' => 'Verify',     'callback' => 'sirius_feed_wizard_verify'),
      'process'   => array('label' => 'Process',    'callback' => 'sirius_feed_wizard_process'),
      'review'    => array('label' => 'Review',     'callback' => 'sirius_feed_wizard_review'),
    ),
    'feed' => array(
      'pks' => array(
        'mail' => 'Email',
      ),
      'batch_size' => 50,
      'callback_fields' => 'sirius_feed_user_fields',
      'callback_row_verify' => 'sirius_feed_user_row_verify',
      'callback_row_process' =>'sirius_feed_user_row_process',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('feed_user'),
  );
  return $items;
} 

function sirius_feed_user_fields($info, $data, $feed_node, $current_step) {
  // Core fields
  $fields = array(
    'mail' => array('title' => 'Email', 'required' => TRUE),
    'name' => array('title' => 'User Name'),
    'phone' => array('title' => 'Phone'),
    'pass' => array('title' => 'Password'),
  );

  return $fields;
}

function sirius_feed_user_options($info, $data, $feed_node, $current_step) { return drupal_get_form('sirius_feed_user_options_form', $info, $data, $feed_node, $current_step); }

function sirius_feed_user_options_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
  $form_state['info'] = $info;
  $form_state['info'] = $info;
  $form_state['data'] = $data;
  $form_state['feed_node'] = $feed_node;
  $form_state['current_step'] = $current_step;
  $steps = sirius_wizard_steps($info, $data, $feed_node);
  $last_data = sirius_feed_lastdata($info, $feed_node);

  $form['#tree'] = TRUE;

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
    $form['user_options']['roles'] = array(
      '#type' => 'checkboxes',
      '#title' => 'Roles',
      '#options' => $roles_avail,
      '#default_value' => $roles_default,
    );
  }


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

function sirius_feed_user_options_form_submit($form, &$form_state) {
  $info = $form_state['info'];
  $data = $form_state['data'];
  $feed_node = $form_state['feed_node'];
  $current_step = $form_state['current_step'];
  $data['user_options'] = $form_state['values']['user_options'];

  sirius_wizard_submit($info, $data, $feed_node, $current_step, "Options saved.");
}

function sirius_feed_user_row_verify($feed_node, $info, $data, &$row, $precompute) {
  $pk = $data['pk'];
  if (!$pk) { $pk = 'mail'; }

  // Trim all
  foreach ($row as $key => $value) { if (is_string($value)) { $row[$key] = trim($value); } }

  if ($row['phone'] && !sirius_phone_validate($row['phone'])) { return array('success' => FALSE, 'msg' => 'Invalid Phone'); } else { $row['phone'] = sirius_phone_format($row['phone']); }
  if ($row['mail'] && !sirius_email_validate($row['mail'])) { return array('success' => FALSE, 'msg' => 'Invalid Email'); }
  if (!$row['name']) { return array('success' => FALSE, 'msg' => 'Missing user name'); }

  if ($pk == 'mail' && !$row['mail']) { return array('success' => FALSE, 'msg' => 'Missing primary key (email)'); }
  $domain_nid = $feed_node->field_sirius_domain['und'][0]['target_id'];

  // Precompute
  if ($precompute) {
    $probe = user_load_by_mail($row['mail']);
    if ($probe) {
      if ($probe->field_sirius_domain['und'][0]['target_id'] != $domain_nid) { return array('success' => FALSE, 'msg' => 'A user with that email address already exists in another domain.'); }
      $uid = $probe->uid;
    }
    $row['uid'] = $probe->uid; 

    $probe = user_load_by_name($row['name']);
    if ($probe) {
      if ($probe->field_sirius_domain['und'][0]['target_id'] != $domain_nid) { return array('success' => FALSE, 'msg' => 'A user with that name already exists in another domain.'); }
      if ($probe && ($probe->uid != $uid)) { return array('success' => FALSE, 'msg' => 'A user with that name already exists, but with a different email address.'); }
    }

  } 

  return array('success' => TRUE, 'msg' => 'Looks good.');
}



function sirius_feed_user_row_process($feed_node, $info, $data, $row) {
  // return array('success' => FALSE, 'msg' => 'Disabled');
  $result = sirius_feed_user_row_verify($feed_node, $info, $data, $row, TRUE);
  if (!$result['success']) { return $result; }

  if ($row['uid']) {
    $old_user = user_load($row['uid']);
  } else {
    $old_user = NULL;
  }

  // New user is an array, not an object, for some stupid reason.
  $domain_node = sirius_domain();
  $new_user = array();
  $new_user['status'] = 1;
  $new_user['language'] = 'en';
  $new_user['init'] = $row['mail'];
  $new_user['timezone'] = $domain_node->field_sirius_tz['und'][0]['value'];
  if ($domain_node) {
    $new_user['field_sirius_domain']['und'][0]['target_id'] = $domain_node->nid;
  }

  // Basic information
  if ($row['name']) { $new_user['name'] = $row['name']; }
  if ($row['mail']) { $new_user['mail'] = $row['mail']; }
  if ($row['pass']) { $new_user['pass'] = $row['pass']; }
  if ($row['phone']) { $new_user['field_grievance_phone']['und'][0]['value'] = $row['phone']; }

  // Add any required roles
  $roles = user_roles();
  foreach ($roles as $rid => $role) {
    $avail = variable_get("sirius_domain_userrmanage_$rid", 'omit');
    if ($avail == 'required') { 
      $new_user['roles'][$rid] = $role;
    }
  }

  // Add any optional roles that were checked
  if ($data['user_options']['roles']) {
    foreach (array_filter(array_values($data['user_options']['roles'])) as $rid) {
      $new_user['roles'][$rid] = $roles[$rid];
    }
  }

  try {
    user_save($old_user, $new_user);
  } catch (exception $e) {
    return array('success' => FALSE, 'msg' => 'Failed to save user: ' . $e->getMessage());
  }

  if ($old_user->uid) {
    return array('success' => TRUE, 'msg' => 'Existing user updated.');
  } else {
    return array('success' => TRUE, 'msg' => 'New user saved.');
  }
}