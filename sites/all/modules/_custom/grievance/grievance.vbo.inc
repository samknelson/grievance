<?php

/**
 * Implements hook_action_info
 */

function grievance_action_info() {
  return array(
    'grievance_vbo_status_correct' => array(
      'type' => 'node',
      'label' => t('Grievance Status Correct'),
      'configurable' => TRUE,
      'vbo_configurable' => FALSE,
      'triggers' => array(),
    ),
    'grievance_vbo_log' => array(
      'type' => 'node',
      'label' => t('Grievance Log'),
      'configurable' => TRUE,
      'vbo_configurable' => FALSE,
      'triggers' => array(),
    ),
  );
}



function grievance_vbo_log(&$grievance_node, $context) {
  if ($grievance_node->type != 'grievance') { return; }

  $log_type_term = taxonomy_term_load($context['grievance_log_type']);

  global $user;
  $log_node = new stdClass();
  $log_node->title = $log_type_term->name;
  $log_node->type = 'grievance_log';
  node_object_prepare($log_node);
  $log_node->language = LANGUAGE_NONE;
  $log_node->uid = $user->uid; 
  $log_node->status = 1;
  $log_node->promote = 0;
  $log_node->comment = 0;
  $log_node->field_grievance['und'][0]['target_id'] = $grievance_node->nid;
  $log_node->field_grievance_log_type['und'][0]['tid'] = $context['grievance_log_type'];
  if ($context['grievance_log_tags']) {
    $log_node->field_grievance_log_tags['und'] = array();
    foreach ($context['grievance_log_tags'] as $tid => $value) {
      if ($value) { $log_node->field_grievance_log_tags['und'][] = array('tid' => $tid); }
    }
  }

  $log_node->field_grievance_status_date['und'][0]['value'] = date('Y-m-d H:i:s', strtotime($context['grievance_status_date']));
  $log_node->body['und'][0]['value'] = $context['body'];
  if ($context['grievance_days']) {
  	$log_node->field_grievance_days['und'][0]['value'] = $context['grievance_days'];
  }

  $log_node = node_submit($log_node); // Prepare node for saving
  node_save($log_node);
  drupal_set_message(t('Log entry added: ' . $grievance_node->title));
}

function grievance_vbo_log_form(&$context, &$form_state) {
  $form = array();

  $form['grievance_log_type'] = array(
    '#type'           => 'select',
    '#title'          => 'Type',
    '#options'        => grievance_log_types_i_can_edit(),
    '#required'       => TRUE,
    '#default_value'  => sirius_get_special_tid('grievance_log_types|modal_default'),
  );

  $timelines_extended_tid = sirius_get_special_tid('grievance_log_types|timelines_extended');
  if ($timelines_extended_tid) {
	  $form['grievance_days'] = array(
	    '#type'           => 'textfield',
	    '#title'          => t('Number of Days'),
	    '#size'						=> 4,
			'#states' => array(
	      'visible' => array(
	        ':input[name="grievance_log_type"]' => array('value' => $timelines_extended_tid),
	      ),
	    ),
	  );
	}

  if (taxonomy_allowed_values(field_info_field('field_grievance_log_tags'))) {
    $form['grievance_log_tags'] = array(
      '#type'           => 'checkboxes',
      '#title'          => 'Tags',
      '#options'        => taxonomy_allowed_values(field_info_field('field_grievance_log_tags')),
    );
  }

  $form['grievance_status_date'] = array(
    '#type'           => 'date_popup',
    '#title'          => 'Date',
    '#required'       => TRUE,
    '#date_format'    => 'm/d/Y H:i',
    '#date_label_position'  => 'none',
    '#default_value'  =>  date('Y-m-d H:i'),
  );

  $form['body'] = array(
    '#type'           => 'textarea',
    '#title'          => 'Body',
  );

  return $form;
}

function grievance_vbo_log_submit(&$form, &$form_state) {
  return array(
    'grievance_log_type' => $form_state['values']['grievance_log_type'],
    'grievance_days' => $form_state['values']['grievance_days'],
    'grievance_log_tags' => $form_state['values']['grievance_log_tags'],
    'grievance_status_date' => $form_state['values']['grievance_status_date'],
    'body' => $form_state['values']['body'],
  );
}
