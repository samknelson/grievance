<?php

function grievance_node_tab_status_correct($grievance_node) {
  return drupal_get_form('grievance_node_tab_status_correct_form', $grievance_node);
  // return 'Hello, world.';
}
function grievance_node_tab_status_correct_form($form, &$form_state, $grievance_node) {
  $form_state['grievance_node'] = $grievance_node;

  $context = array();
  $form = grievance_vbo_status_correct_form($context, $form_state);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

function grievance_node_tab_status_correct_form_submit($form, &$form_state) {
  $grievance_node = $form_state['grievance_node'];
  $context = grievance_vbo_status_correct_submit($form, $form_state);
  grievance_vbo_status_correct($grievance_node, $context);
}

function grievance_vbo_status_correct(&$grievance_node, $context) {
  if ($grievance_node->type != 'grievance') { return; }

  $from_status_tid = $context['from_status_tid'];
  $to_status_tid = $context['to_status_tid'];

  // First find all log entries
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_node->nid);
  $query->fieldCondition('field_grievance_status', 'tid', $from_status_tid);
  $stmt = $query->execute();
  $log_nids = array();
  if ($stmt['node']) { $log_nids = array_keys($stmt['node']); }

  // Update each log entry
  $count = 0;
  foreach ($log_nids as $log_nid) {
    $log_node = node_load($log_nid);
    $log_node->grievance_no_recurse = TRUE;
    $log_node->field_grievance_status['und'][0]['tid'] = $to_status_tid;
    node_save($log_node);
    ++$count;
  }

  // Now we're moving on to the grievance. Don't call our hooks to update create log entries
  $grievance_node->grievance_no_recurse = TRUE;

  // Set the grievance status
  $grievance_node->field_grievance_status['und'][0]['tid'] = $to_status_tid;

  // Set the alert TID and date
  if ($grievance_node->field_grievance_contract_tplt['und'][0]['target_id']) {
    list($next_alert_tid, $next_alert_ts) = grievance_contract_next_alert($grievance_node);
    if ($grievance_node->field_grievance_alert_tid['und'][0]['tid'] != $next_alert_tid) {
      if ($next_alert_tid) {
        $grievance_node->field_grievance_alert_tid['und'][0]['tid'] = $next_alert_tid;
      } else {
        unset($grievance_node->field_grievance_alert_tid['und'][0]);
      }
    }

    if (isset($grievance_node->field_grievance_alert_date['und'][0]['value'])) { 
      $tmp = $grievance_node->field_grievance_alert_date['und'][0]['value'];
    } else {
      $tmp = NULL;
    }
    if (strtotime($tmp) != $next_alert_ts) {
      if ($next_alert_ts) {
        $grievance_node->field_grievance_alert_date['und'][0]['value'] = date('Y-m-d', $next_alert_ts);
      } else {
        $grievance_node->field_grievance_alert_date['und'][0]['value'] = NULL;
      }
    }
  }

  node_save($grievance_node);
  drupal_set_message("Updated: " . $grievance_node->title . " and $count log entries.");

}

function grievance_vbo_status_correct_form(&$context, &$form_state) {
  $form['intro'] = array('#markup' => '<div class="sirius-form-intro">' . t('Use the form below when an incorrect status has been applied. It will update the status of the grievance and all log entries, without creating any new log entries. Please be cautious, this action cannot be undone.') . '</div>');
  $options = sirius_taxonomy_load('grievance_status', 'tid', 'name', TRUE);
  $form['from_status_tid'] = array(
    '#type' => 'select',
    '#title' => t('Change all entries FROM this status'),
    '#options' => $options,
    '#required' => TRUE, 
  );

  $form['to_status_tid'] = array(
    '#type' => 'select',
    '#title' => t('Change all entries TO this status'),
    '#options' => $options,
    '#required' => TRUE, 
  );

  return $form;
}

function grievance_vbo_status_correct_submit(&$form, &$form_state) {
  return array(
    'from_status_tid' => $form_state['values']['from_status_tid'],
    'to_status_tid' => $form_state['values']['to_status_tid'],
  );
}
