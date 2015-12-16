<?php

function grievance_admin() {
  $form = array();

  $form['grievance_log_update_type'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_log_type')),
    '#title' => t('Grievance Log Update Type'),
    '#default_value' => variable_get('grievance_log_update_type', ''),
    '#description' => t("Grievance log type to be used for updates."),
  );

  $form['grievance_alerts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Alerts'),
  );

  $form['grievance_alerts']['grievance_alert_days_warning1'] = array(
    '#type' => 'textfield',
    '#title' => t('Days Before Deadline - Warning 1'),
    '#default_value' => variable_get('grievance_alert_days_warning1', 14),
    '#description' => t("Send the FIRST warning this many days before the deadline."),
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

  return system_settings_form($form);
}