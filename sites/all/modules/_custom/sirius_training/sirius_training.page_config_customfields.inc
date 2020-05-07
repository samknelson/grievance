<?php

function sirius_training_config_customfields_edit() {
  $domain_nid = sirius_domain_nid();
  $definition_nid = sirius_contact_definition_get_nid($domain_nid, TRUE);
  return drupal_get_form('sirius_json_definition_edit_form', $definition_nid, NULL, 'tc_form');
}

function sirius_training_config_customfields_preview() {
  $domain_nid = sirius_domain_nid();
  $definition_nid = sirius_contact_definition_get_nid($domain_nid, TRUE);
  return drupal_get_form('sirius_json_definition_preview_form', $definition_nid, 'tc_form');
}

function sirius_training_config_customfields_exim() {
  $domain_nid = sirius_domain_nid();
  $definition_nid = sirius_contact_definition_get_nid($domain_nid, TRUE);
  return drupal_get_form('sirius_json_definition_exim_form', $definition_nid, 'tc_form');
}
