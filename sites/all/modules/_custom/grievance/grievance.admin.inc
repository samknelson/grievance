<?php

function grievance_admin() {
  $form = array();

  $form['grievance_status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Status Types'),
  );

  $form['grievance_status']['grievance_log_incident_status'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_status')),
    '#title' => t('Grievance Incident Status'),
    '#default_value' => variable_get('grievance_log_incident_status', ''),
    '#description' => t("Grievance log status to be used for incidents -- i.e. an entry with this status is created the date that the incident occurred."),
  );

  $form['grievance_status']['grievance_incident_required'] = array(
    '#type' => 'select',
    '#options' => array('No' => 'No', 'Yes' => 'Yes'),
    '#title' => t('Incident Date Required?'),
    '#default_value' => variable_get('grievance_incident_required', ''),
    '#description' => t("Select Yes to make the incident date required for all new grievances."),
  );

  $form['grievance_status']['grievance_log_filed_status'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_status')),
    '#title' => t('Grievance Filed Status'),
    '#default_value' => variable_get('grievance_log_filed_status', ''),
    '#description' => t("Grievance status for filed, used for generating reports"),
  );

  $form['grievance_status']['grievance_log_filed_status_2'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_status')),
    '#title' => t('Grievance Filed Status 2'),
    '#default_value' => variable_get('grievance_log_filed_status_2', ''),
  );

  $form['grievance_status']['step_1'] = array(
    '#type' => 'fieldset',
    '#title' => t('Step 1 Status'),
    '#description' => t('On "My Grievances", there is an option to display the last Step 1 status for a grievance (i.e. to show what status a grievance passed through).  Select the possible statuses here.'),
  );

  $form['grievance_status']['step_1']['grievance_log_step_1_status_display'] = array(
    '#type' => 'select',
    '#title' => t('Display last Step 1 Status?'),
    '#options' => array('No' => t('No'), 'Yes' => t('Yes')),
    '#default_value' => variable_get('grievance_log_step_1_status_display', ''),
  );

  // Here's where I wish system_settings_form supported multiple values.  @todo this is something of a hack.
  for ($i=0; $i<10; ++$i) {
    $form['grievance_status']['step_1'] ['grievance_log_step_1_status_' . $i] = array(
      '#type' => 'select',
      '#options' => taxonomy_allowed_values(field_info_field('field_grievance_status')),
      '#title' => t('Grievance Step 1 Status #' . $i),
      '#default_value' => variable_get('grievance_log_step_1_status_' . $i, ''),
      '#description' => t("All Step 1 statuses, used for generating reports"),
    );
  }

  $form['grievance_status']['grievance_incident_default_days_ago'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Incident Days Ago'),
    '#size' => 4,
    '#default_value' => variable_get('grievance_incident_default_days_ago', 15),
    '#description' => t('When a new grievance is created, a "timeline starts" log entry is created. If the date of this incident is not entered, the log entry should be this many days before the current date.'),
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

  $form['grievance_alerts']['grievance_alert_disable'] = array(
    '#type' => 'select',
    '#title' => t('Disable Alerts?'),
    '#default_value' => variable_get('grievance_alert_disable', NULL),
    '#options' => array('No' => 'No', 'Yes' => 'Yes'),
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

  $form['grievance_managers']['grievance_rep_assignee'] = array(
    '#default_value' => variable_get('grievance_rep_assignee', ''),
    '#title' => t('Default Assignee'),
    '#description' => t('This user will be set as the default assignee for newly created grievances'),
    '#type' => 'entityreference',
    '#era_entity_type' => 'user',
  );

  $form['grievance_managers']['grievance_assignee_notes'] = array(
    '#default_value' => variable_get('grievance_assignee_notes', ''),
    '#title' => t('Default Assignee Notes'),
    '#description' => t('Default assignment notes (e.g. "Intake complete ready for review.").'),
    '#type' => 'textfield',
  );

  $form['grievance_loopback_ip'] = array(
    '#default_value' => variable_get('grievance_loopback_ip', ''),
    '#title' => t('Loopback IP'),
    '#description' => t('The server needs to make "loopback" calls to itself, which sometimes come in as an IP different from the server\'s public IP.  Enter a privileged IP address which is permitted to make unrestricted requests to the loopback functions.'),
    '#type' => 'textfield',
  );

  $form['grievance_hidefields'] = array(
    '#default_value' => variable_get('grievance_hidefields', ''),
    '#title' => t('Hide Fields'),
    '#description' => t('The grievance system comes with many fields, not all of which will be appropriate for a given deployment.  Enter the machine names (one per line) of fields which should be hidden.'),
    '#type' => 'textarea',
  );

  $form['grievance_classaction_hidefields'] = array(
    '#default_value' => variable_get('grievance_classaction_hidefields', grievance_admin_default_classaction_hidefields()),
    '#title' => t('Class Action Hide Fields'),
    '#description' => t('Certain fields can be hidden for class action grievances. Enter the fields to hide, one per line.'),
    '#type' => 'textarea',
  );

  $form['sirius_email_default_from_address'] = array(
    '#default_value' => variable_get('sirius_email_default_from_address', ''),
    '#title' => t('From Email'),
    '#description' => t('The "From" email for the system; must be configured so that the server is a legitimate sender from this email.'),
    '#type' => 'textfield',
  );

  $form['grievances_mine'] = array(
    '#type' => 'fieldset',
    '#title' => t('My Grievances'),
  );

  $form['grievances_mine']['grievance_mine_view_name'] = array(
    '#type' => 'textfield',
    '#title' => t('My Grievances View Name'),
    '#default_value' => variable_get('grievance_mine_view_name', 'grievances_mine'),
    '#description' => t("Enter the system name for the Drupal View to be used for the 'My Grievances' page."),
  );

  $form['grievances_mine']['grievance_mine_view_display'] = array(
    '#type' => 'textfield',
    '#title' => t('My Grievances View Display'),
    '#default_value' => variable_get('grievance_mine_view_display', 'page_1'),
    '#description' => t("Enter the system name for the Drupal Display to be used for the 'My Grievances' page."),
  );

  $form['grievances_mine']['grievances_mine_processed_title'] = array(
    '#type' => 'textfield',
    '#title' => t('My Grievances "Processed" title'),
    '#default_value' => variable_get('grievances_mine_processed_title', 'Processed'),
    '#description' => t("Enter the label to be used for 'Processed' grievances."),
  );
  $form['grievances_mine']['grievances_mine_first_call_title'] = array(
    '#type' => 'textfield',
    '#title' => t('My Grievances "First Call" title'),
    '#default_value' => variable_get('grievances_mine_first_call_title', 'First&nbsp;Call'),
    '#description' => t("Enter the label to be used for 'First Call'."),
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

  $form['grievance_id']['grievance_id_suffix'] = array(
    '#type' => 'textfield',
    '#title' => t('Suffix'),
    '#default_value' => variable_get('grievance_id_suffix', ''),
    '#description' => t('Enter a suffix to be appended to each grievance ID.'),
  );

  return system_settings_form($form);
}


function grievance_admin_default_classaction_hidefields() {
  return join("\n", array(
    'field_grievance_phone',
    'field_grievance_email',
    'field_grievance_address',
    'field_grievance_address_2',
    'field_grievance_city',
    'field_grievance_state',
    'field_grievance_zip',
    'field_grievance_hire_date',
    'field_grievance_gender',
    'field_grievance_work_status',
    'field_grievance_shift',
    'field_grievance_daysoff',
    'field_grievance_ein',
    'field_grievance_corrected',
    'field_grievance_supervisor_name',
    'field_grievance_supervisor_title',
    'field_grievance_supervisor_unit',
  ));
}