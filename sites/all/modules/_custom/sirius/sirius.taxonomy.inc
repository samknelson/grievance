<?php

function sirius_taxonomy_checkboxes(&$form, &$form_state, $field_name, $vocabulary_name) {
  $vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary_name);
  $terms = sirius_domain_taxonomy_get_tree($vocabulary->vid, 0, NULL, TRUE, sirius_domain_nid());

  foreach ($terms as $term) {
    $tid = $term->tid;
    $name = $term->name;
    $depth = $term->depth;
    $parent = $term->parents[0];

    $field_prefix = '';
    for ($i=0; $i<$depth; ++$i) {
      $field_prefix .= '&nbsp;--';
    }
    $states = NULL;
    if ($parent) {
      $states = array(
        'visible' => array(
          array(':input[name="' . $field_name . '_checkbox_' . $parent . '"]' => array('checked' => TRUE)),
        )
      );
    }

    $form["${field_name}_checkbox_{$tid}"] = array(
      '#type' => 'checkbox',
      '#value' => $tid,
      '#title' => $name,
      '#field_prefix' => $field_prefix,
      '#states' => $states,
    );
  }
}

/**
 * A stupid utility function to create dropdowns and mappings from taxonomies.
 * Key can be:
 * - 'tid' to create a mapping by tid
 * - 'name' to create a mapping by name
 * Value can be:
 * - 'full' to return the fully loaded term
 * - 'name' to return the term name
 * - field_whatever to return the value of "field_whatever"
 */

function sirius_taxonomy_load($vocabulary_name, $key_type, $value_type, $include_null = FALSE) {
  $domain_nid = sirius_domain_nid();
  $vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary_name);
  if ($value_type == 'name') { $load_entities = FALSE; } else { $load_entities = TRUE; }
  $terms = sirius_domain_taxonomy_get_tree($vocabulary->vid, 0, NULL, $load_entities, sirius_domain_nid());

  $map = array();
  if ($include_null) {
    $map[NULL] = t('-- Select --');
  }
  
  foreach ($terms as &$term) {
    if ($value_type == 'full') {
      $value = $term;
    } else if ($value_type == 'name') {
      $value = $term->name;
    } else {
      $value = $term->{$value_type}['und'][0]['value'];
    }

    if ($key_type == 'tid') {
      $key = $term->tid;
    } else if ($key_type == 'field_sirius_id') {
      $key = $term->field_sirius_id['und'][0]['value'];
    } else if ($key_type == 'field_sirius_id2') {
      $key = $term->field_sirius_id2['und'][0]['value'];
    } else {
      $key = $term->name;
    }

    $map[$key] = $value;
  }
  return $map;
}

function sirius_taxonomy_lookup_by_anything($vocabulary_name, $value) {
  static $preload;

  if (!$preload[$vocabulary_name]) {
    $tmp = sirius_taxonomy_load($vocabulary_name, 'tid', 'full');
    foreach ($tmp as $tid => $term) {
      $keys = array();
      $keys[] = trim(strtolower($term->name));
      $keys[] = trim(strtolower($term->field_sirius_name_short['und'][0]['value']));
      $keys[] = trim(strtolower($term->field_sirius_id['und'][0]['value']));
      $keys[] = trim(strtolower($term->field_sirius_id2['und'][0]['value']));
      $keys[] = trim(strtolower($term->field_sirius_id3['und'][0]['value']));
      $keys[] = $term->tid;
      foreach ($keys as $key) {
        if ($key) {
          $preload[$vocabulary_name][$key] = $tid;
        }
      }
    }
  }

  return $preload[$vocabulary_name][trim(strtolower($value))];
}

/**
 * Implements hook_taxonomy_term_update
 *
 * Log taxonomy term changes.
 */

function sirius_taxonomy_term_update($term) {
  sirius_log(
    'taxonomy_term', 
    'update', 
    'The taxonomy term ' . $term->name . ' has been updated.', 
    '', 
    '',  
    NULL, 
    NULL,
    $term
  );
}

/**
 * Implements hook_taxonomy_term_insert
 *
 * Log taxonomy term changes.
 */

function sirius_taxonomy_term_insert($term) {
  sirius_log(
    'taxonomy_term', 
    'insert', 
    'The taxonomy term ' . $term->name . ' has been inserted.', 
    '', 
    '',  
    NULL, 
    NULL,
    $term
  );
}

/**
 * Implements hook_taxonomy_term_delete
 *
 * Log taxonomy term changes.
 */

function sirius_taxonomy_term_delete($term) {
  sirius_log(
    'taxonomy_term', 
    'delete', 
    'The taxonomy term ' . $term->name . ' has been deleted.', 
    '', 
    '',  
    NULL, 
    NULL,
    $term
  );
}


function sirius_taxonomy_settings($vocabulary_machine_name) {
  static $settings;
  if (!$settings) {
    $settings['grievance_alert_types']['view'] = 'sirius_taxonomy';
    $settings['grievance_alert_types']['display'] = 'block_2';
    $settings['grievance_alert_types']['multidomain'] = 'No';
    $settings['grievance_assignee_notes']['view'] = 'sirius_taxonomy';
    $settings['grievance_assignee_notes']['display'] = 'block';
    $settings['grievance_assignee_notes']['multidomain'] = 'No';
    $settings['grievance_broughtby']['view'] = 'sirius_taxonomy';
    $settings['grievance_broughtby']['display'] = 'block';
    $settings['grievance_broughtby']['multidomain'] = 'No';
    $settings['grievance_category']['view'] = 'sirius_taxonomy';
    $settings['grievance_category']['display'] = 'block';
    $settings['grievance_category']['multidomain'] = 'No';
    $settings['grievance_contact_types']['view'] = 'sirius_taxonomy';
    $settings['grievance_contact_types']['display'] = 'block';
    $settings['grievance_contact_types']['multidomain'] = 'Yes';
    $settings['grievance_contract_clause_tags']['view'] = 'sirius_taxonomy';
    $settings['grievance_contract_clause_tags']['display'] = 'block_3';
    $settings['grievance_contract_clause_tags']['multidomain'] = 'No';
    $settings['grievance_contract_section_tags']['view'] = 'sirius_taxonomy';
    $settings['grievance_contract_section_tags']['display'] = 'block_3';
    $settings['grievance_contract_section_tags']['multidomain'] = 'No';
    $settings['grievance_contract_template_tags']['view'] = 'sirius_taxonomy';
    $settings['grievance_contract_template_tags']['display'] = 'block_3';
    $settings['grievance_contract_template_tags']['multidomain'] = 'No';
    $settings['grievance_delivery']['view'] = 'sirius_taxonomy';
    $settings['grievance_delivery']['display'] = 'block';
    $settings['grievance_delivery']['multidomain'] = 'No';
    $settings['grievance_department']['view'] = 'sirius_taxonomy';
    $settings['grievance_department']['display'] = 'block_4';
    $settings['grievance_department']['multidomain'] = 'No';
    $settings['grievance_document_types']['view'] = 'sirius_taxonomy';
    $settings['grievance_document_types']['display'] = 'block_5';
    $settings['grievance_document_types']['multidomain'] = 'No';
    $settings['grievance_job_classification']['view'] = 'sirius_taxonomy';
    $settings['grievance_job_classification']['display'] = 'block_4';
    $settings['grievance_job_classification']['multidomain'] = 'No';
    $settings['grievance_log_tags']['view'] = 'sirius_taxonomy';
    $settings['grievance_log_tags']['display'] = 'block_3';
    $settings['grievance_log_tags']['multidomain'] = 'No';
    $settings['grievance_log_types']['view'] = 'sirius_taxonomy';
    $settings['grievance_log_types']['display'] = 'block_6';
    $settings['grievance_log_types']['multidomain'] = 'Yes';
    $settings['grievance_outcome']['view'] = 'sirius_taxonomy';
    $settings['grievance_outcome']['display'] = 'block_3';
    $settings['grievance_outcome']['multidomain'] = 'No';
    $settings['grievance_remedies']['view'] = 'sirius_taxonomy';
    $settings['grievance_remedies']['display'] = 'block';
    $settings['grievance_remedies']['multidomain'] = 'No';
    $settings['grievance_resproc']['view'] = 'sirius_taxonomy';
    $settings['grievance_resproc']['display'] = 'block';
    $settings['grievance_resproc']['multidomain'] = 'No';
    $settings['grievance_settlement_tags']['view'] = 'sirius_taxonomy';
    $settings['grievance_settlement_tags']['display'] = 'block_3';
    $settings['grievance_settlement_tags']['multidomain'] = 'No';
    $settings['grievance_shift']['view'] = 'sirius_taxonomy';
    $settings['grievance_shift']['display'] = 'block';
    $settings['grievance_shift']['multidomain'] = 'No';
    $settings['grievance_status']['view'] = 'grievance_statuses';
    $settings['grievance_status']['display'] = 'block_1';
    $settings['grievance_status']['multidomain'] = 'Yes';
    $settings['grievance_tags']['view'] = 'sirius_taxonomy';
    $settings['grievance_tags']['display'] = 'block_3';
    $settings['grievance_tags']['multidomain'] = 'No';
    $settings['grievance_types']['view'] = 'sirius_taxonomy';
    $settings['grievance_types']['display'] = 'block';
    $settings['grievance_types']['multidomain'] = 'No';
    $settings['grievance_work_status']['view'] = 'sirius_taxonomy';
    $settings['grievance_work_status']['display'] = 'block';
    $settings['grievance_work_status']['multidomain'] = 'No';
    $settings['sirius_contact_tags']['view'] = 'sirius_taxonomy';
    $settings['sirius_contact_tags']['display'] = 'block';
    $settings['sirius_contact_tags']['multidomain'] = 'Yes';
    $settings['sirius_dispatch_job_tags']['view'] = 'sirius_taxonomy';
    $settings['sirius_dispatch_job_tags']['display'] = 'block_3';
    $settings['sirius_dispatch_job_tags']['multidomain'] = 'No';
    $settings['sirius_dispatch_job_type']['view'] = 'sirius_taxonomy';
    $settings['sirius_dispatch_job_type']['display'] = 'block';
    $settings['sirius_dispatch_job_type']['multidomain'] = 'No';
    $settings['sirius_dispatch_sib']['view'] = '';
    $settings['sirius_dispatch_sib']['display'] = '';
    $settings['sirius_dispatch_sib']['multidomain'] = '';
    $settings['sirius_event_participant_role']['view'] = 'sirius_taxonomy';
    $settings['sirius_event_participant_role']['display'] = 'block';
    $settings['sirius_event_participant_role']['multidomain'] = 'Yes';
    $settings['sirius_event_participant_status']['view'] = 'sirius_taxonomy';
    $settings['sirius_event_participant_status']['display'] = 'block_9';
    $settings['sirius_event_participant_status']['multidomain'] = 'Yes';
    $settings['sirius_event_type']['view'] = 'sirius_taxonomy';
    $settings['sirius_event_type']['display'] = 'block';
    $settings['sirius_event_type']['multidomain'] = 'Yes';
    $settings['sirius_gender']['view'] = 'sirius_taxonomy';
    $settings['sirius_gender']['display'] = 'block';
    $settings['sirius_gender']['multidomain'] = 'Yes';
    $settings['sirius_hour_type']['view'] = 'sirius_taxonomy';
    $settings['sirius_hour_type']['display'] = 'block';
    $settings['sirius_hour_type']['multidomain'] = 'Yes';
    $settings['sirius_industry']['view'] = 'sirius_taxonomy';
    $settings['sirius_industry']['display'] = 'block_7';
    $settings['sirius_industry']['multidomain'] = 'No';
    $settings['sirius_member_status']['view'] = 'sirius_member_status';
    $settings['sirius_member_status']['display'] = 'page';
    $settings['sirius_member_status']['multidomain'] = 'Yes';
    $settings['sirius_skill']['view'] = 'sirius_taxonomy';
    $settings['sirius_skill']['display'] = 'block_1';
    $settings['sirius_skill']['multidomain'] = 'No';
    $settings['sirius_worker_dispatch_status']['view'] = 'sirius_taxonomy';
    $settings['sirius_worker_dispatch_status']['display'] = 'block';
    $settings['sirius_worker_dispatch_status']['multidomain'] = 'No';
    $settings['sirius_work_status']['view'] = 'sirius_taxonomy';
    $settings['sirius_work_status']['display'] = 'block_8';
    $settings['sirius_work_status']['multidomain'] = 'No';
  }

  if ($vocabulary_machine_name == 'all') { return $settings; }
  return $settings[$vocabulary_machine_name];
}