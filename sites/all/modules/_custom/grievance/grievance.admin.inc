<?php

function grievance_admin() {
  $form = array();

  $form['grievance_log'] = array(
    '#type' => 'fieldset',
    '#title' => t('Log Types'),
  );

  $form['grievance_log']['grievance_log_update_type'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_log_type')),
    '#title' => t('Grievance Log Update Type'),
    '#default_value' => variable_get('grievance_log_update_type', ''),
    '#description' => t("Grievance log type to be used for updates."),
  );

  $form['grievance_log']['grievance_log_notify_type'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_log_type')),
    '#title' => t('Grievance Log Company Notification Type'),
    '#default_value' => variable_get('grievance_log_notify_type', ''),
    '#description' => t("Grievance log type to be used for notification to the company."),
  );

  $form['grievance_log']['grievance_log_member_forward_type'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_log_type')),
    '#title' => t('Grievance Log Member Notification Type'),
    '#default_value' => variable_get('grievance_log_member_forward_type', ''),
    '#description' => t("Grievance log type to be used for notification to the member."),
  );

  $form['member_forward'] = array(
    '#type' => 'fieldset',
    '#title' => t('Member Notification'),
  );

  $form['member_forward']['member_forward_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Member Notification Subject'),
    '#default_value' => variable_get('member_notify_subject', ''),
    '#description' => t('Enter the default subject of the e-mail notification to the member.'),
  );

  $form['member_forward']['member_forward_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Member Notification Body'),
    '#default_value' => variable_get('member_notify_body', ''),
    '#description' => t('Enter the default body of the e-mail notification to the member.'),
  );


  $form['grievance_alerts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Alerts'),
  );

  $form['grievance_alerts']['grievance_alert_recipients'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Alert Recipients'),
    '#default_value' => variable_get('grievance_alert_recipients', NULL),
    '#options' => array(
      'field_grievance_rep_filed' => t('The user who filed the grievance'),
      'field_grievance_rep_organizer' => t('The organizer'),
      'field_grievance_rep_lead' => t('The lead'),
      'field_grievance_rep_manager' => t('The grievance admin'),
      'field_grievance_rep_watching' => t('Watchers')
    ),
  );

  $form['grievance_alerts']['grievance_alert_days_warning1'] = array(
    '#type' => 'textfield',
    '#title' => t('Days Before Deadline - Warning 1'),
    '#default_value' => variable_get('grievance_alert_days_warning1', 14),
    '#description' => t("Send the FIRST warning this many days before the deadline.  (Enter -1 to omit this or any warning.)"),
  );

  $form['grievance_alerts']['grievance_alert_days_warning2'] = array(
    '#type' => 'textfield',
    '#title' => t('Days Before Deadline - Warning 2'),
    '#default_value' => variable_get('grievance_alert_days_warning2', 7),
  );

  $form['grievance_alerts']['grievance_alert_days_warning3'] = array(
    '#type' => 'textfield',
    '#title' => t('Days Before Deadline - Warning 3'),
    '#default_value' => variable_get('grievance_alert_days_warning3', 3),
  );

  $form['grievance_alerts']['grievance_alert_days_warning4'] = array(
    '#type' => 'textfield',
    '#title' => t('Days Before Deadline - Warning 4'),
    '#default_value' => variable_get('grievance_alert_days_warning4', 2),
  );

  $form['grievance_alerts']['grievance_alert_days_warning5'] = array(
    '#type' => 'textfield',
    '#title' => t('Days Before Deadline - Warning 5'),
    '#default_value' => variable_get('grievance_alert_days_warning5', 1),
    '#description' => t("Send the LAST warning this many days before the deadline."),
  );

  $form['grievance_managers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Managers'),
    '#description' => t('Set managers and observers for the system.'),
  );

  $form['grievance_managers']['grievance_rep_manager'] = array(
		'#default_value' => variable_get('grievance_rep_manager', ''),
    '#title' => t('Default Manager'),
    '#description' => t('This user will be set as the default manager for all newly created grievances.'),
    '#type' => 'entityreference',
    '#era_entity_type' => 'user',
  );

  $form['grievance_managers']['grievance_rep_watching'] = array(
		'#default_value' => variable_get('grievance_rep_watching', ''),
    '#title' => t('Default Observer'),
    '#description' => t('This user will be set as an observer for all newly created grievances.'),
    '#type' => 'entityreference',
    '#era_entity_type' => 'user',
  );

  $form['grievance_contacts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Contacts'),
  );

  $form['grievance_contacts']['grievance_contacts_hide_nostatusmatch'] = array(
    '#type' => 'select',
    '#title' => t('Hide Non-Matching Status?'),
    '#options' => array('No' => t('No'), 'Yes' => t('Yes')),
    '#default_value' => variable_get('grievance_contacts_hide_nostatusmatch', ''),
    '#description' => t('If "Yes" is selected, non-matching status contacts will be hidden by default (for example, Step 1 contacts will be hidden on Step 2 grievances.)'),
  );

  $form['grievance_messaging'] = array(
    '#type' => 'fieldset',
    '#title' => t('Messaging'),
    '#description' => t('Set text messaging for the footer, the welcome page, etc.'),
  );

  $form['grievance_messaging']['grievance_messaging_footer'] = array(
    '#type' => 'textarea',
    '#title' => t('Footer'),
    '#default_value' => variable_get('grievance_messaging_footer', ''),
    '#description' => t('Enter the footer copy for the site.  May contain HTML.'),
  );

  $form['grievance_hidefields'] = array(
    '#default_value' => variable_get('grievance_hidefields', ''),
    '#title' => t('Hide Fields'),
    '#description' => t('The grievance system comes with many fields, not all of which will be appropriate for a given deployment.  Enter the machine names (one per line) of fields which should be hidden.'),
    '#type' => 'textarea',
  );

  $algorithms = grievaince_id_algorithms();
  $msg = t('Grievance IDs are unique strings used to identify a grievance.  They typically appear on the grievance form sent to the company, and can be used to find a grievance with the Quicksearch box.<br><br>Grievance IDs can be generated using various algorithms.  The available algorithms are:');
  $msg .= '<dl>';
  foreach ($algorithms as $key => $value) {
    $msg .= '<dt>';
    $msg .= $value['name'];
    $msg .= '</dt>';

    $msg .= '<dd>';
    $msg .= $value['description'];
    $msg .= '</dd>';
  }
  $msg .= '</dl>';
  $form['grievance_id'] = array(
    '#type' => 'fieldset',
    '#title' => t('Grievance IDs'),
    '#description' => $msg,
  );
  $options = array();
  foreach ($algorithms as $key => $value) {
    $options[$key] = $value['name'];
  }
  $form['grievance_id']['grievance_id_algorithm'] = array(
    '#type' => 'radios',
    '#title' => t('Algorithm'),
    '#options' => $options,
    '#default_value' => variable_get('grievance_id_algorithm', ''),
  );

  $form['grievance_id']['grievance_id_manual'] = array(
    '#type' => 'select',
    '#title' => t('Allow manual override?'),
    '#options' => array('No' => t('No'), 'Yes' => t('Yes')),
    '#default_value' => variable_get('grievance_id_manual', ''),
    '#description' => t('Select "Yes" to allow grievance admins to manually set or change the grievance ID.'),
  );
 
  return system_settings_form($form);
}