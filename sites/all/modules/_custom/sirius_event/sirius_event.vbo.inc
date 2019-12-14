<?php

/**
 * Implements hook_action_info
 */

function sirius_event_action_info() {
  return array(
    'sirius_event_participant_status' => array(
      'type' => 'node',
      'label' => t('Sirius Event Participant Status'),
      'configurable' => TRUE,
      'vbo_configurable' => FALSE,
      'triggers' => array(),
    ),
  );
}

function sirius_event_participant_status(&$participant_node, $context) {
  $participant_status_tid = $context['participant_status_tid'];
  if ($participant_node->field_sirius_event_pstatus['und'][0]['tid'] != $participant_status_tid) {
    $participant_node->field_sirius_event_pstatus['und'][0]['tid'] = $participant_status_tid;
    node_save($participant_node);
  } 
}

function sirius_event_participant_status_form(&$context, &$form_state) {
  // @todo This is kludgey! We're assuming that we're being VBO'd from a view, and the arguments are, in order, the event ID and the role TID.
  // There really ought to be a better way to pass these through.
  
  $event_nid = $context['view']->args[0];
  $participant_role_tid = $context['view']->args[1];
  $status_tids = sirius_event_participant_role_status_tids($participant_role_tid);
  $statuses = sirius_event_participant_role_status_tids($participant_role_tid);

  $form['participant_status_tid'] = array(
    '#title' => 'Status',
    '#type' => 'select',
    '#options' => $statuses,
    '#required' => TRUE,
  );

  return $form;
}

function sirius_event_participant_status_submit(&$form, &$form_state) {
  return array(
    'participant_status_tid' => $form_state['values']['participant_status_tid'],
  );
}
