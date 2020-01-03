<?php

function sirius_node_tab_dispatch_job_fore_access($job_node) {
	return sirius_node_tab_dispatch_job_manage_access($job_node);
}

function sirius_node_tab_dispatch_job_fore($job_node) {
	return drupal_get_form('sirius_node_tab_dispatch_job_fore_form', $job_node);
}

function sirius_node_tab_dispatch_job_fore_form($form, &$form_state, $job_node) {
	$form_state['job_nid'] = $job_node->nid;

	$fore = sirius_dispatch_fore_list($job_node);

	if ($fore) {
		$html = 'Current forepersons: <strong>';
		$html .= join(', ', array_values($fore));
		$html .= '</strong>';
		$form['intro'] = array('#markup' => '<div class="sirius_form_intro">' . $html . '</div>');
		$form['remove'] = array(
			'#type' => 'checkboxes',
			'#title' => t('Remove forepersons'),
			'#options' => $fore,
			'#description' => t('Check any foreperson to remove them'),
		);
	}

  $form['worker'] = array(
    '#title' => t('Add a foreperson'),
    '#type' => 'textfield',
    '#autocomplete_path' => 'sirius/ajax/fore-quicksearch',
		'#description' => t('Enter or scan the worker\'s dispatch ID, SSN, or name.'),
	);
 
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_dispatch_job_fore_form_submit($form, &$form_state) {
	$job_node = node_load($form_state['job_nid']);

	if ($form_state['values']['remove']) {
		$to_remove = array_values($form_state['values']['remove']);
		foreach ($to_remove as $worker_nid) {
			if (!$worker_nid) { continue; }
			$worker_node = node_load($worker_nid);
		  $result = sirius_dispatch_fore_remove($job_node, $worker_node);
		  if ($result['success']) { drupal_set_message($result['msg']); } else { drupal_set_message($result['msg'], 'error'); }
		}
	}

  $worker_str = $form_state['values']['worker'];
  if ($worker_str) { 
	  $matches = array();
	  if (preg_match('/\s+\((\d+)\)/', $worker_str, $matches) && $matches[1]) { 
	    $worker_nid = $matches[1];
	  }
	  if (!$worker_nid) {
	    drupal_set_message("Please make sure that you have selected a matching worker.", 'warning');
	    return;
	  }
	  $worker_node = node_load($worker_nid);
	  $result = sirius_dispatch_fore_add($job_node, $worker_node);
	  if ($result['success']) { drupal_set_message($result['msg']); } else { drupal_set_message($result['msg'], 'error'); }
	}
}