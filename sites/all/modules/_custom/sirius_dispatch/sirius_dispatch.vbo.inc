<?php

/**
 * Implements hook_action_info
 */

function sirius_dispatch_action_info() {
  return array(
    'sirius_dispatch_vbo_terminate' => array(
      'type' => 'node',
      'label' => t('Sirius Dispatch Layoff'),
      'configurable' => FALSE,
      'vbo_configurable' => FALSE,
      'triggers' => array(),
    ),
    'sirius_dispatch_vbo_accept' => array(
      'type' => 'node',
      'label' => t('Sirius Dispatch Accept'),
      'configurable' => FALSE,
      'vbo_configurable' => FALSE,
      'triggers' => array(),
    ),
    'sirius_dispatch_vbo_decline' => array(
      'type' => 'node',
      'label' => t('Sirius Dispatch Decline'),
      'configurable' => FALSE,
      'vbo_configurable' => FALSE,
      'triggers' => array(),
    ),
    'sirius_dispatch_vbo_rollover' => array(
      'type' => 'node',
      'label' => t('Sirius Dispatch Rollover'),
      'configurable' => TRUE,
      'vbo_configurable' => FALSE,
      'triggers' => array(),
    ),
    'sirius_dispatch_vbo_vlri_resolve' => array(
      'type' => 'node',
      'label' => t('Sirius Dispatch VLRI'),
      'configurable' => TRUE,
      'vbo_configurable' => FALSE,
      'triggers' => array(),
    ),
  );
}

function sirius_dispatch_vbo_vlri_resolve(&$node, $context) {
  drupal_set_message("About to resolve: " . $node->title . " with action " . $context['vlri_resolve']);
  $result = sirius_dispatch_vlri_resolve($node, '', $context['vlri_resolve']);
}

function sirius_dispatch_vbo_vlri_resolve_form(&$context, &$form_state) {
  $form['vlri_resolve'] = array(
    '#title' => t('Resolve:'),
    '#type' => 'select',
    '#options' => array(NULL => t('-- Select --'), 'accept' => t('Accept'), 'reject' => t('Reject')),
    '#required' => TRUE,
  );

  return $form;
}

function sirius_dispatch_vbo_vlri_resolve_submit(&$form, &$form_state) {
  return array(
    'vlri_resolve' => $form_state['values']['vlri_resolve'],
  );
}

function sirius_dispatch_vbo_terminate(&$node, $context) {
  sirius_dispatch_terminate($node);
}

function sirius_dispatch_vbo_accept(&$node, $context) {
  $result = sirius_dispatch_accept($node);
  if (!$result['success']) { drupal_set_message($result['msg'], 'warning'); }
}

function sirius_dispatch_vbo_decline(&$node, $context) {
  $result = sirius_dispatch_decline($node);
  if (!$result['success']) { drupal_set_message($result['msg'], 'warning'); }
}

function sirius_dispatch_vbo_rollover(&$node, $context) {
  $rollover_employer_nid = $context['rollover_employer_nid'];
  $rollover_type = $context['rollover_type'];
  $rollover_end_date = $context['rollover_end_date'];
  sirius_dispatch_rollover($node, $rollover_employer_nid, $rollover_type, $rollover_end_date);
}

function sirius_dispatch_vbo_rollover_form(&$context, &$form_state) {
  // Find the employer node, from the view argument or from the current user.
  $employer_node = node_load($context->view['args'][0]);
  if (!$employer_node) { $employer_node = sirius_employer_get(); }

  // Find the industry TID
  $industry_tid = NULL;
  if ($employer_node) { $industry_tid = $employer_node->field_sirius_industry['und'][0]['tid']; }

  // Find the possible recipients
  $rollover_options = array(NULL => t('-- Select --'));
  $rollover_options += sirius_employer_list(TRUE, $industry_tid);

  $form['rollover_employer_nid'] = array(
    '#type' => 'select',
    '#title' => 'Rollover To:',
    '#options' => $rollover_options,
    '#required' => TRUE, 
  );

  $form['rollover_type'] = array(
    '#type' => 'select',
    '#title' => 'Rollover Type:',
    '#options' => array('permanent' => 'Permanent', 'temporary' => 'Temporary'),
    '#required' => TRUE, 
    '#description' => t('Permanent rollovers will result in a layoff, whether or not the worker accepts the rollover. Temporary rollovers will not affect the worker\' current dispatch.'),
  );

  $form['rollover_end_date'] = array(
    '#type' => 'date_popup',
    '#title' => 'End Date:',
    '#states' => array(
      'visible' => array(
        array(':input[name="rollover_type"]' => array('value' => 'temporary')),
      )
    ),
    '#timepicker' => 'timepicker',
    '#timepicker_options' => array(
      'rows' => 6,
      'minutes' => array(
        'starts' => 0,
        'ends' => 56,
        'interval' => 5,
      ),
    ),
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
  );

  return $form;
}

function sirius_dispatch_vbo_rollover_submit(&$form, &$form_state) {
  return array(
    'rollover_employer_nid' => $form_state['values']['rollover_employer_nid'],
    'rollover_type' => $form_state['values']['rollover_type'],
    'rollover_end_date' => $form_state['values']['rollover_end_date']
  );
}
