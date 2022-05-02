<?php

function sirius_revision_page_config() {
  return drupal_get_form('sirius_revision_page_config_form');
}

function sirius_revision_page_config_form() {
  $form = array();

  $form['sirius_revision_content_types'] = array(
    '#title' => t('Content Types'),
    '#type' => 'checkboxes',
    '#default_value' => variable_get('sirius_revision_content_types', array()),
    '#options' => node_type_get_names(),
    '#description' => t('Check the content types that should have revisions generated when the content changes.'),
    '#checkall' => TRUE,
  );

  return system_settings_form($form);
}
