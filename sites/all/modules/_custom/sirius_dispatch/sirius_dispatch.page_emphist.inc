<?php

function sirius_dispatch_node_tab_emphist_form($form, &$form_state, $dispatch_node) {
	$form_state['dispatch_nid'] = $dispatch_node->nid;
	$form['#tree'] = TRUE;

	$form['emphist']['type'] = array(
		'#type' => 'select',
		'#title' => 'Type',
		'#options' => array(
			'Primary' => t('Primary'),
			'Secondary' => t('Secondary'),
			'None' => t('None -- Remove this record'),
		),
		'#default_value' => $dispatch_node->field_sirius_dispatch_type['und'][0]['value'],
	);

	$form['emphist']['start'] = array(
    '#title' => t('Start Date'),
    '#type' => 'date_popup',
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
    '#date_format' => 'Y-m-d',
  );
  $date = $dispatch_node->field_sirius_date_start['und'][0]['value'];
  if ($date) {
		$ts = strtotime($date);
		if ($ts !== NULL) {
			$form['emphist']['start']['#default_value'] = date('Y-m-d', $ts);
		}
	}

	$form['emphist']['end'] = array(
    '#title' => t('End Date'),
    '#type' => 'date_popup',
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
    '#date_format' => 'Y-m-d',
  );
  $date = $dispatch_node->field_sirius_date_end['und'][0]['value'];
  if ($date) {
		$ts = strtotime($date);
		if ($ts !== NULL) {
			$form['emphist']['end']['#default_value'] = date('Y-m-d', $ts);
		}
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_dispatch_node_tab_emphist_form_submit($form, &$form_state) {
	$dispatch_nid = $form_state['dispatch_nid'];

	$result = sirius_dispatch_emphist_save($form_state['values']['emphist'], $dispatch_nid);
	if (!$result['success']) {
		drupal_set_message("Update failed: " . $result['msg'], 'error');
		$form_state['rebuild'] = TRUE;
	} else {
		drupal_set_message("Record updated.");
		$dispatch_node = node_load($dispatch_nid);
		$worker_nid = $dispatch_node->field_sirius_worker['und'][0]['target_id'];
		drupal_goto("node/$worker_nid/sirius_worker_emphist");
	}
}
