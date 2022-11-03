<?php

function unite_feed_relationship_info() {
  $items = array();
  $items['unite_feed_relationship'] = array(
    'name' => 'unite_feed_relationship',
    'title' => 'Relationships',
    'description' => 'Import new or existing property / actor relationships.',
    'steps' => array(
      'upload'    => array('label' => 'Upload',     'callback' => 'sirius_feed_wizard_upload'),
      'sheet'     => array('label' => 'Worksheet',  'callback' => 'sirius_feed_wizard_sheet'),
      'map'       => array('label' => 'Mapping',    'callback' => 'sirius_feed_wizard_map'),
      // 'options'   => array('label' => 'Options',    'callback' => 'unite_feed_relationship_options'),
      'verify'    => array('label' => 'Verify',     'callback' => 'sirius_feed_wizard_verify'),
      'process'   => array('label' => 'Process',    'callback' => 'sirius_feed_wizard_process'),
      'review'    => array('label' => 'Review',     'callback' => 'sirius_feed_wizard_review'),
    ),
    'feed' => array(
      'pks' => array(
        'nid' => 'Internal ID (NID)',
        'pat' => 'Propert + Actor + Type',
      ),
      'create_new_records' => 'optional',
      'fields' => array(
        'nid' => array('group' => 'pk', 'title' => 'Internal ID (NID)'),
        'property_nid' => array('group' => 'pk', 'title' => 'Property NID'),
        'actor_title' => array('group' => 'pk', 'title' => 'Actor Name'),
        'pk_relationship_type' => array('group' => 'pk', 'title' => 'Relationship Type'),

        'replace' => array(
          'group' => 'pk', 
          'title' => 'Replace Existing Owner / Operator?',
          'description' => 'If this value is set to "Yes", and if this is an Owner or Operator relationship, and if there is an existing relationship of that type to a different actor, then that relationship whill be changed to "Previous Owner" or "Previous Operator", as appropriate.'
        ),

        'relationship_type' => array('group' => 'core', 'title' => 'New Relationship Type', 'type' => 'list'),
        'notes' => array('group' => 'core', 'type' => 'long'),

        'percent_owned' => array('group' => 'ownership', 'type' => 'integer'),
        'amount_owned' => array('group' => 'ownership', 'type' => 'integer'),
        'owned_raw' => array('group' => 'ownership', 'type' => 'text'),
        'sales_price' => array('group' => 'ownership', 'type' => 'text'),
        'pension' => array('group' => 'ownership', 'type' => 'list'),

        'lg_type' => array('group' => 'lg', 'type' => 'term_reference', 'taxonomy' => 'hdb_lg_loan_type'),
        'lg_status' => array('group' => 'lg', 'type' => 'term_reference', 'taxonomy' => 'hdb_lg_loan_status'),
        'lg_balance' => array('group' => 'lg', 'type' => 'integer'),
        'lg_date_balance' => array('group' => 'lg', 'type' => 'date'),
        'lg_amount' => array('group' => 'lg', 'type' => 'integer'),
        'lg_employees_claimed' => array('group' => 'lg', 'type' => 'integer'),
        'lg_date_issued' => array('group' => 'lg', 'type' => 'date'),
        'lg_date_due' => array('group' => 'lg', 'type' => 'date'),
        'lg_amortization' => array('group' => 'lg', 'type' => 'term_reference', 'taxonomy' => 'hdb_lg_amortization'),
        'lg_master_servicer' => array('group' => 'lg', 'type' => 'text'),
        'lg_special_servicer' => array('group' => 'lg', 'type' => 'text'),
        'lg_multiparty' => array('group' => 'lg', 'type' => 'list'),
        'lg_servicer_commentary' => array('group' => 'lg', 'type' => 'long'),
        'lg_date_servicer_comment' => array('group' => 'lg', 'type' => 'date'),
        'lg_portfolio' => array('group' => 'lg', 'type' => 'text'),
        'lg_id' => array('group' => 'lg', 'type' => 'text'),
        'lg_sources' => array('group' => 'lg', 'type' => 'long'),
        'lg_date_sources' => array('group' => 'lg', 'type' => 'date'),

      ),
      'batch_size' => 10,
      'callback_row_verify' => 'unite_feed_relationship_row_verify',
      'callback_row_process' =>'unite_feed_relationship_row_process',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('unite_feed_relationship'),
  );

  $groups = array(
    'pk' => 'Primary Key',
    'core' => 'Core',
    'ownership' => 'Ownership',
    'lg' => 'Loans and Grants',
  );
  foreach ($items['unite_feed_relationship']['feed']['fields'] as $field_name => $field_info) {
    $group = $field_info['group'];
    $group_name = $groups[$group];

    if (!$field_info['title']) {

      $structure_info = field_info_instance('node', "field_$field_name", 'property_actor');
      if ($structure_info) { $items['unite_feed_relationship']['feed']['fields'][$field_name]['title'] = $structure_info['label']; }
    }

    $items['unite_feed_relationship']['feed']['fields'][$field_name]['title'] = $group_name . ': ' . $items['unite_feed_relationship']['feed']['fields'][$field_name]['title'];
  }

  return $items;
}

function unite_feed_relationship_options($info, $data, $feed_node, $current_step) { return drupal_get_form('unite_feed_relationship_options_form', $info, $data, $feed_node, $current_step); }

function unite_feed_relationship_options_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
  $form_state['info'] = $info;
  $form_state['data'] = $data;
  $form_state['feed_node'] = $feed_node;
  $form_state['current_step'] = $current_step;
  $steps = sirius_wizard_steps($info, $data, $feed_node);
  $last_data = sirius_feed_lastdata($info, $feed_node);

  $form['#tree'] = TRUE;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

function unite_feed_relationship_options_form_submit($form, &$form_state) {
  $info = $form_state['info'];
  $data = $form_state['data'];
  $feed_node = $form_state['feed_node'];
  $current_step = $form_state['current_step'];
  $data['charge_settings'] = $form_state['values']['charge_settings'];

  sirius_wizard_submit($info, $data, $feed_node, $current_step, "Payment selection saved.");
}

function unite_feed_relationship_handle_list($field_name, $value) {
  $structure_info = field_info_field("field_$field_name");
  $allowed_values = list_allowed_values($structure_info);
  $allowed_values_flip = array_flip($allowed_values);

  // Match case insensitive
  foreach ($allowed_values as $a => $b) {
    if (trim(strtolower($a)) == trim(strtolower($value))) { return $a; }
    if (trim(strtolower($b)) == trim(strtolower($value))) { return $a; }

    if (trim(strtolower($value)) == 'yes') {
      if (trim(strtolower($a)) == 'y') { return $a; }
    }

    if (trim(strtolower($value)) == 'no') {
      if (trim(strtolower($a)) == 'n') { return $a; }
    }
  }

  return NULL;
}

function unite_feed_relationship_handle_entity_reference($entity_type, $value) {
  $sql = "select min(nid) as mn, max(nid) as mx from node ";
  $sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_name_short on field_data_field_sirius_name_short.entity_type = 'node' and field_data_field_sirius_name_short.entity_id = node.nid ";
  $sql .= "where nid = :v1 ";
  $sql .= "or title = :v2 ";
  $sql .= "or field_sirius_id_value = :v3 ";
  $sql .= "or field_sirius_name_short_value = :v4 ";
  $stmt = sirius_sql_query($sql, array(':v1' => $value, ':v2' => $value, ':v3' => $value, ':v4' => $value));
  $hr = $stmt->fetchAssoc();
  if (!$hr['mn']) { return NULL; }
  if ($hr['mn'] != $hr['mx']) { return NULL; }
  return $hr['mn'];
}

function unite_feed_relationship_row_verify($feed_node, $info, $data, &$row, $precompute) {
  $pk = $data['pk'];

  static $rowcount;
  $rowcount++;

	// Trim all
	foreach ($row as $key => $value) {
    if (!is_array($value)) { $row[$key] = trim($value); }
	}

  // "Replace" must be "Yes" or "No"
  if ($row['replace']) {
    $replace = strtolower(substr($row['replace'], 0, 1));
    if (!in_array($replace, array('y', 'n'))) {
      return array('success' => FALSE, 'msg' => '"Replace" must contain the value "Yes" or "No".');
    }
    if ($replace == 'y') {
      if (!in_array($row['pk_relationship_type'], array('Owner', 'Operator'))) {
        return array('success' => FALSE, 'msg' => 'Replace is only available if relationship type is "Owner" or "Operator"');
      }
    }
  }

  foreach ($info['feed']['fields'] as $field_name => $field_info) {
    if (!$row[$field_name]) { continue; }

    // Validate integer fields
    if ($field_info['type'] == 'integer') {
      $row[$field_name] = preg_replace('/\,/', '', $row[$field_name]);
      if (!preg_match('/^[0-9]+$/', $row[$field_name])) { return array('success' => FALSE, 'msg' => "$field_name must be an integer."); }
    }

    // Validate list fields
    if ($field_info['type'] == 'list') {
      $value = unite_feed_relationship_handle_list($field_name, $row[$field_name]);
      if (!$value) { return array('success' => FALSE, 'msg' => "$field_name must be one of the allowed values."); }
      $row[$field_name] = $value;
    }

    // Validate term_reference
    if ($field_info['type'] == 'term_reference') {
      $value = sirius_taxonomy_lookup_by_anything($field_info['taxonomy'], $row[$field_name]);
      if (!$value) { return array('success' => FALSE, 'msg' => "$field_name must be one of the values in the taxonomy: $field_info[taxonomy]."); }
      $row[$field_name] = $value;
    }
  }

  $relationship_type_field_info = field_info_field('field_relationship_type');
  $relationship_type_allowed_values= list_allowed_values($relationship_type_field_info);

  if ($precompute) {
    $row['replace'] = $replace; 

    if ($pk == 'nid') {
      $nid = $row['nid'];
      if (!$nid) { return array('success' => FALSE, 'msg' => 'If the primary key is NID, all rows must have a node ID.'); }
      $probe = node_load($nid);
      if (!$probe) { return array('success' => FALSE, 'msg' => 'No node was found with this NID.'); }
      if ($probe->type != 'actor') { return array('success' => FALSE, 'msg' => 'The node for this NID was found, but it is not a actor.'); }

      $msg = "An existing actor will be updated.";
    } else if ($pk == 'pat') {
      $property_nid = $row['property_nid'];
      $actor_title = $row['actor_title'];
      $relationship_type = $row['pk_relationship_type'];

      if (!$property_nid || !$actor_title || !$relationship_type) {
        return array('success' => FALSE, 'msg' => "This record has no value for at least one of the elements of the primary key (property, actor, relationship type.");
      }

      if (!in_array($relationship_type, $relationship_type_allowed_values)) { 
        return array('success' => FALSE, 'msg' => "The relationship type $relationship_type is not one of the known types.");
      }

      $property_node = node_load($property_nid);
      if (!$property_node || ($property_node->type != 'property')) {
        return array('success' => FALSE, 'msg' => "The specified property does not match a valid property record.");
      }

      $result = sirius_feed_lookup_by_field('actor', 'field_data_field_title_canon', 'field_title_canon_value', unite_actor_canonicalize_title($actor_title));
      if (!$result['success']) { return array('success' => FALSE, 'msg' => $result['msg']); }
      $actor_nid = $result['nid'];
      if (!$actor_nid) { return array('success' => FALSE, 'msg' => "Actor not found."); }
      $row['actor_nid'] = $actor_nid;

      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'node');
      $query->entityCondition('bundle', 'property_actor');
      $query->fieldCondition('field_property', 'target_id', $property_nid);
      $query->fieldCondition('field_actor', 'target_id', $actor_nid);
      $query->fieldCondition('field_relationship_type', 'value', $relationship_type);
      $stmt = $query->execute();
      $relationship_nid = NULL;
      if ($stmt['node']) {
        if (count($stmt['node']) > 1) {
          return array('success' => FALSE, 'msg' => 'There is more than one matching relationships.');
        }
        $relationship_nid = reset(array_keys($stmt['node']));
      }

      if ($relationship_nid) {
        // We found exactly one. Use that NID.
        $row['nid'] = $relationship_nid;
        $msg = "An existing relationship will be updated.";
      } else {
        if ($data['create_new_records'] == 'Yes') { 
          // Not found, but we can create new records. Do nothing
          $msg = "A new record will be created.";
        } else {
          // We can't create new records, or we have a different kind of error
          return array('success' => FALSE, 'msg' => 'A matching record was not found, and the feed is not set to create new records.');
        }
      }
    }

    foreach ($info['feed']['fields'] as $field_name => $field_info) {
      if (!$row[$field_name]) { continue; }

      // Validate entity_reference
      if ($field_info['type'] == 'entity_reference') {
        $value = unite_feed_relationship_handle_entity_reference($field_info['entity_type'], $row[$field_name]);
        if (!$value) { return array('success' => FALSE, 'msg' => "No value found (or multiple values found) for $field_name in the entity type: $field_info[entity_type]."); }
        $row[$field_name] = $value;
      }

      // Lookup parent
      if ($field_name == 'parent') {
        if ($row['parent']) {
          $result = sirius_feed_lookup_by_field('actor', 'field_data_field_title_canon', 'field_title_canon_value', unite_actor_canonicalize_title($row['parent']));
          if (!$result['success']) {
            return $result;
          }
          $row['parent_nid'] = $result['nid'];
        }
      }
    }

  }

  if (!$msg) { $msg = "Looks good."; }
  return array('success' => TRUE, 'msg' => $msg);
}

function unite_feed_relationship_row_process($feed_node, $info, $data, $row) {
  // return array('success' => FALSE, 'msg' => 'Disabled');
	$result = unite_feed_relationship_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }

  // Replace any existing entries
  if ($row['replace'] == 'y') {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'property_actor');
    $query->fieldCondition('field_property', 'target_id', $row['property_nid']);
    $query->fieldCondition('field_actor', 'target_id', $row['actor_nid'], '!=');
    $query->fieldCondition('field_relationship_type', 'value', $row['pk_relationship_type']);
    $query->range(0,100);
    $stmt = $query->execute();
    if ($stmt['node']) {
      $replace_relationship_nids = array_keys($stmt['node']);
      $replace_relationship_nodes = node_load_multiple($replace_relationship_nids);
      foreach ($replace_relationship_nodes as $replace_relationship_nid => $replace_relationship_node) {
        $replace_relationship_node->field_relationship_type['und'][0]['value'] = 'Previous ' . $replace_relationship_node->field_relationship_type['und'][0]['value'];
        node_save($replace_relationship_node);
      }
    }
  }

  if ($row['nid']) {
    $is_new = FALSE;
    $relationship_node = node_load($row['nid']);
  } else {
    $is_new = TRUE;
    global $user;
    $relationship_node = new stdClass();
    $relationship_node->type = 'property_actor';
    $relationship_node->language = LANGUAGE_NONE;
    $relationship_node->uid = $user->uid;
    $relationship_node->status = 1;
    $relationship_node->promote = 0;
    $relationship_node->comment = 0;

    $relationship_node->field_property['und'][0]['target_id'] = $row['property_nid'];
    $relationship_node->field_actor['und'][0]['target_id'] = $row['actor_nid'];
    $relationship_node->field_relationship_type['und'][0]['value'] = $row['pk_relationship_type'];
  }

  foreach ($info['feed']['fields'] as $field_name => $field_info) {
    if ($row[$field_name]) {
      if (in_array($field_info['type'], array('integer', 'list', 'text', 'long', 'date'))) {
        $relationship_node->{"field_$field_name"}['und'][0]['value'] = $row[$field_name];
      }
      if ($field_info['type'] == 'term_reference') {
        $relationship_node->{"field_$field_name"}['und'][0]['tid'] = $row[$field_name];
      }
      if ($field_info['type'] == 'entity_reference') {
        $relationship_node->{"field_$field_name"}['und'][0]['target_id'] = $row[$field_name];
      }
    }
  }

  node_save($relationship_node);

  if ($is_new) {
    return array('success' => TRUE, 'msg' => 'Created a new relationshiop.');
  } else {
    return array('success' => TRUE, 'msg' => 'Updated an existing relationship.');
  }
}
