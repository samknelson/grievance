<?php

function sirius_taxonomy_options($vocabulary_name, $options = array()) {
  $indent = $options['indent'];
  if (!isset($indent)) { $indent = ' -- '; }

  $vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary_name);
  $terms = sirius_domain_taxonomy_get_tree($vocabulary->vid, 0, NULL, TRUE, sirius_domain_nid());
  $options = array();
  foreach ($terms as $term) {
    $tid = $term->tid;
    $name = $term->name;
    $depth = $term->depth;
    $parent = $term->parents[0];

    $field_prefix = '';
    for ($i=0; $i<$depth; ++$i) { $field_prefix .= $indent; }
    $options[$tid] = $field_prefix . $name;
  }

  return $options;
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
  if ($domain_nid < 0) { $domain_nid = NULL; }
  $vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary_name);
  if ($value_type == 'name') { $load_entities = FALSE; } else { $load_entities = TRUE; }
  $terms = sirius_domain_taxonomy_get_tree($vocabulary->vid, 0, NULL, $load_entities, $domain_nid);

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

function sirius_taxonomy_lookup_by_anything($vocabulary_name, $value, $employer_nid = NULL) {
  static $preload;

  if (!$preload[$vocabulary_name]['global']) {
    // drupal_set_message("Preloading $vocabulary_name");
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
          $preload[$vocabulary_name]['global'][$key] = $tid;
        }
      }
    }
  }

  if ($employer_nid) {
    if (!isset($preload[$vocabulary_name][$employer_nid])) {
      // drupal_set_message("Preloading $vocabulary_name / $employer_nid");
      $employer_node = node_load($employer_nid); 
      $preload[$vocabulary_name][$employer_nid] = array();
      $json = sirius_jsonfield_util_node_json_get($employer_node);
      if ($json['taxonomy_mappings'][$vocabulary_name]['mappings']) {
        foreach ($json['taxonomy_mappings'][$vocabulary_name]['mappings'] as $record) {
          if ($record['from'] && $record['to']) {
            $preload[$vocabulary_name][$employer_nid][trim(strtolower($record['from']))] = $record['to'];
          }
        }
      }
    }
  }

  $value = trim(strtolower($value));
  $tid = NULL;
  if ($employer_nid) { $tid = $preload[$vocabulary_name][$employer_nid][$value]; }
  if (!$tid) { $tid = $preload[$vocabulary_name]['global'][$value]; }

  return $tid;
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
    $settings['grievance_department']['multidomain'] = 'Yes';
    $settings['grievance_department']['employer_map'] = 'Yes';
    $settings['grievance_document_types']['view'] = 'sirius_taxonomy';
    $settings['grievance_document_types']['display'] = 'block_5';
    $settings['grievance_document_types']['multidomain'] = 'Yes';
    $settings['grievance_job_classification']['view'] = 'sirius_taxonomy';
    $settings['grievance_job_classification']['display'] = 'block_4';
    $settings['grievance_job_classification']['multidomain'] = 'Yes';
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
    $settings['grievance_resproc']['multidomain'] = 'Yes';
    $settings['grievance_settlement_tags']['view'] = 'sirius_taxonomy';
    $settings['grievance_settlement_tags']['display'] = 'block_3';
    $settings['grievance_settlement_tags']['multidomain'] = 'Yes';
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
    $settings['grievance_work_status']['multidomain'] = 'Yes';
    $settings['sirius_contact_tags']['view'] = 'sirius_taxonomy';
    $settings['sirius_contact_tags']['display'] = 'block_18';
    $settings['sirius_contact_tags']['multidomain'] = 'Yes';
    $settings['sirius_contact_relationship_types']['view'] = 'sirius_taxonomy';
    $settings['sirius_contact_relationship_types']['display'] = 'block_12';
    $settings['sirius_contact_relationship_types']['multidomain'] = 'Yes';
    $settings['sirius_dispatch_job_tags']['view'] = 'sirius_taxonomy';
    $settings['sirius_dispatch_job_tags']['display'] = 'block_3';
    $settings['sirius_dispatch_job_tags']['multidomain'] = 'No';
    $settings['sirius_dispatch_job_type']['view'] = 'sirius_taxonomy';
    $settings['sirius_dispatch_job_type']['display'] = 'block';
    $settings['sirius_dispatch_job_type']['multidomain'] = 'Yes';
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
    $settings['sirius_event_type']['display'] = 'block_13';
    $settings['sirius_event_type']['multidomain'] = 'Yes';
    $settings['sirius_gender']['view'] = 'sirius_taxonomy';
    $settings['sirius_gender']['display'] = 'block_16';
    $settings['sirius_gender']['multidomain'] = 'Yes';
    $settings['sirius_hour_type']['view'] = 'sirius_taxonomy';
    $settings['sirius_hour_type']['display'] = 'block';
    $settings['sirius_hour_type']['multidomain'] = 'Yes';
    $settings['sirius_hour_type']['employer_map'] = 'Yes';
    $settings['sirius_industry']['view'] = 'sirius_taxonomy';
    $settings['sirius_industry']['display'] = 'block_7';
    $settings['sirius_industry']['multidomain'] = 'Yes';
    $settings['sirius_member_status']['view'] = 'sirius_member_status';
    $settings['sirius_member_status']['display'] = 'page';
    $settings['sirius_member_status']['multidomain'] = 'Yes';
    $settings['sirius_skill']['view'] = 'sirius_taxonomy';
    $settings['sirius_skill']['display'] = 'block_1';
    $settings['sirius_skill']['multidomain'] = 'Yes';
    $settings['sirius_worker_dispatch_status']['view'] = 'sirius_taxonomy';
    $settings['sirius_worker_dispatch_status']['display'] = 'block';
    $settings['sirius_worker_dispatch_status']['multidomain'] = 'Yes';
    $settings['sirius_work_status']['view'] = 'sirius_taxonomy';
    $settings['sirius_work_status']['display'] = 'block_8';
    $settings['sirius_work_status']['multidomain'] = 'Yes';
    $settings['sirius_ledger_type']['view'] = 'sirius_taxonomy';
    $settings['sirius_ledger_type']['display'] = 'block';
    $settings['sirius_ledger_type']['multidomain'] = 'Yes';
    $settings['sirius_payment_type']['view'] = 'sirius_taxonomy';
    $settings['sirius_payment_type']['display'] = 'block_10';
    $settings['sirius_payment_type']['multidomain'] = 'Yes';
    $settings['sirius_worker_tor_type']['view'] = 'sirius_taxonomy';
    $settings['sirius_worker_tor_type']['display'] = 'block_11';
    $settings['sirius_worker_tor_type']['multidomain'] = 'Yes';
    $settings['sirius_trust_benefit_type']['view'] = 'sirius_taxonomy';
    $settings['sirius_trust_benefit_type']['display'] = 'block';
    $settings['sirius_trust_benefit_type']['multidomain'] = 'Yes';
    $settings['sirius_trust_election_type']['view'] = 'sirius_taxonomy';
    $settings['sirius_trust_election_type']['display'] = 'block_15';
    $settings['sirius_trust_election_type']['multidomain'] = 'Yes';
    $settings['sirius_trust_election_status']['view'] = 'sirius_taxonomy';
    $settings['sirius_trust_election_status']['display'] = 'block_14';
    $settings['sirius_trust_election_status']['multidomain'] = 'Yes';
    $settings['sirius_trust_provider_level']['view'] = 'sirius_taxonomy';
    $settings['sirius_trust_provider_level']['display'] = 'block';
    $settings['sirius_trust_provider_level']['multidomain'] = 'Yes';
    $settings['sirius_trust_provider_level']['component'] = 'trust';
    $settings['sirius_trust_service_type']['view'] = 'sirius_taxonomy';
    $settings['sirius_trust_service_type']['display'] = 'block_17';
    $settings['sirius_trust_service_type']['multidomain'] = 'Yes';
    $settings['sirius_trust_service_type']['component'] = 'trust';
  }

  if ($vocabulary_machine_name == 'all') { return $settings; }
  return $settings[$vocabulary_machine_name];
}

/**
 * Sometimes we want to attach things (like a log entry) to a taxonomy term. 
 * But this is hard, because things like log entries can only reference one type of entity, i.e. a node.
 * So we maintain an as-needed proxy for each taxonomy term. This is just an inaccessible system-level node which references the term in question.
 */

function sirius_taxonomy_proxy($tid) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_term_proxy');
  $query->fieldCondition('field_sirius_term_proxy', 'target_id', $tid);
  $query->propertyOrderBy('created', 'desc');
  $query->range(0,1);
  $stmt = $query->execute();
  if ($stmt['node']) { return node_load(array_keys($stmt['node'])[0]); }

  $term = taxonomy_term_load($tid);
  if (!$term) { return NULL; }

  global $user;
  $node = new stdClass();
  $node->type = 'sirius_term_proxy';
  $node->title = 'Proxy: ' . $term->name . ' (' . $tid . ')';
  $node->no_recurse = TRUE;
  $node->language = LANGUAGE_NONE;
  $node->uid = $user->uid;
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 0;
  $node->field_sirius_term_proxy['und'][0]['target_id'] = $tid;
  $node = node_submit($node);
  node_object_prepare($node);
  node_save($node);
  return $node;
}

function sirius_node_access_term_proxy($node, $op, $account) { return FALSE; }