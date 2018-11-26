<?php

function grievance_admin_log_types() {

  $form['intro'] = array(
    '#type' => 'markup',
    '#markup' => t('The settings below indicate which "log types" are used for which tasks.  Since the log types can vary from site to site, log types which have a special meaning -- such as "updated with status change" -- must be selected here.')
  );

  $form['grievance_log_incident_type'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_log_type')),
    '#title' => t('Grievance Log Incident Type'),
    '#default_value' => variable_get('grievance_log_incident_type', ''),
    '#description' => t("Grievance log type to be used for incidents -- i.e. an entry of this type is created the date that the incident occurred."),
  );

  $form['grievance_log_insert_type'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_log_type')),
    '#title' => t('Grievance Log Insert Type'),
    '#default_value' => variable_get('grievance_log_insert_type', ''),
    '#description' => t("Grievance log type to be used when a grievance record is first saved into the database."),
  );

  $form['grievance_log_update_type_with_status_change'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_log_type')),
    '#title' => t('Grievance Log Update Type With Status Change'),
    '#default_value' => variable_get('grievance_log_update_type_with_status_change', ''),
    '#description' => t("Grievance log type to be used for updates where the status changes -- i.e. when the record is saved and the status is different."),
  );

  $form['grievance_log_update_type_without_status_change'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_log_type')),
    '#title' => t('Grievance Log Update Type Without Status Change'),
    '#default_value' => variable_get('grievance_log_update_type_without_status_change', ''),
    '#description' => t("Grievance log type to be used for updates where the status does not change -- i.e. an entry is created each time the grievance is saved."),
  );

  $form['grievance_log_notify_type'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_log_type')),
    '#title' => t('Grievance Log Company Notification Type'),
    '#default_value' => variable_get('grievance_log_notify_type', ''),
    '#description' => t("Grievance log type to be used for notification to the company."),
  );

  $form['grievance_log_member_forward_type'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_log_type')),
    '#title' => t('Grievance Log Member Notification Type'),
    '#default_value' => variable_get('grievance_log_member_forward_type', ''),
    '#description' => t("Grievance log type to be used for notification to the member."),
  );

  $form['grievance_log_assignment_type'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_log_type')),
    '#title' => t('Grievance Log Assignment Type'),
    '#default_value' => variable_get('grievance_log_assignment_type', ''),
    '#description' => t("Grievance log type to be used for assignment to a staff member."),
  );

  $form['grievance_log_first_call_type'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_log_type')),
    '#title' => t('Grievance Log First Call Type'),
    '#default_value' => variable_get('grievance_log_first_call_type', ''),
    '#description' => t("Grievance log type to be used for initial call to grievant.  Used for reporting."),
  );

  $form['grievance_log_phaxio_callback_type'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_log_type')),
    '#title' => t('Phaxio Callback Type'),
    '#default_value' => variable_get('grievance_log_phaxio_callback_type', ''),
    '#description' => t("Grievance log type to be used for Phaxio callback -- i.e. fax confirmation receipts."),
  );

  $form['grievance_log_spi_type'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_log_type')),
    '#title' => t('SPI Type'),
    '#default_value' => variable_get('grievance_log_spi_type', ''),
  );

  $form['grievance_log_term_type'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_log_type')),
    '#title' => t('Term Type'),
    '#default_value' => variable_get('grievance_log_term_type', ''),
  );

  $form['grievance_log_written_warning_type'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_log_type')),
    '#title' => t('Written Warning Type'),
    '#default_value' => variable_get('grievance_log_written_warning_type', ''),
  );

  $form['grievance_log_final_warning_type'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_log_type')),
    '#title' => t('Final Warning Type'),
    '#default_value' => variable_get('grievance_log_final_warning_type', ''),
  );

  $form['grievance_log_suspension_type'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_log_type')),
    '#title' => t('Suspension Type'),
    '#default_value' => variable_get('grievance_log_suspension_type', ''),
  );

  $form['grievance_log_merge_type'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_log_type')),
    '#title' => t('Merge Type'),
    '#default_value' => variable_get('grievance_log_merge_type', ''),
    '#description' => t('Log type to be used when a grievance is merged into another grievance.'),
  );

  $form['grievance_log_timelines_extended_type'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_log_type')),
    '#title' => t('Timelines Extended Type'),
    '#default_value' => variable_get('grievance_log_timelines_extended_type', ''),
    '#description' => t('Log type to be used when a when timelines are extended by mutual agreement.'),
  );

  return system_settings_form($form);
}
