<?php

function sirius_node_tab_event_intake_application_manage_ms_access($application_node) {
	// Must be an application-type log entry
	if ($application_node->field_sirius_category['und'][0]['value'] != 'event:intake_application') { return FALSE; }

	// There must already be a worker
	if (!sirius_log_handler($application_node, 'sirius_worker')) { return FALSE; }

	// There must be an event
	$event_node = sirius_log_handler($application_node, 'sirius_event');
	if (!$event_node) { return FALSE; }
	$event_json = sirius_json_get($event_node);

	// There must be some optional ms
	$ms_level_options = sirius_node_tab_event_intake_application_manage_ms_options($event_json);
	if (!$ms_level_options) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_event_intake_application_manage_ms($application_node) {
	return drupal_get_form('sirius_node_tab_event_intake_application_manage_ms_form', $application_node);
}

function sirius_node_tab_event_intake_application_manage_ms_form($form, &$form_state, $application_node) {
	$form_state['application_nid'] = $application_node->nid;
	$worker_node = sirius_log_handler($application_node, 'sirius_worker');
	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);

	$ms_level_options = sirius_node_tab_event_intake_application_manage_ms_options($event_json);

  drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/event.css', 'file');

  $form['#tree'] = TRUE; 
  $form['table_open'] = array('#markup' => '<table class="columns_table">');
  $form['col1_open'] = array('#markup' => '<tr><td style="width: 50%; vertical-align: top;">');

  $form['ms_levels'] = array(
  	'#type' => 'checkboxes',
  	'#options' => $ms_level_options,
  	'#title' => t('Add These Member Status / Industries'),
  );

 	$assignment_options = sirius_event_intake_application_assignment_options($event_node);
	if ($assignment_options) { 
		$assignment = sirius_event_intake_application_assignment_get($application_node);
	  $form['assign'] = array(
	  	'#type' => 'select',
	  	'#title' => t('Assign To'),
	  	'#options' => array(NULL => t('-- Select --')) + $assignment_options,
	  	'#default_value' => $assignment['name'],
	  );
	}

	if ($application_node->field_sirius_type['und'][0]['value'] != 'complete') { 
	  $form['complete'] = array(
	  	'#type' => 'select',
	  	'#title' => t('Mark application as "Complete"'),
	  	'#options' => array('No' => t('No'), 'Yes' => t('Yes')),
	  	'#default_value' => 'No',
	  	'#description' => t('Marking an application as "complete" will clear any assignments, and remove it from the queue of entries needing to be reviewed.'),
	  );
	}


  $form['submit'] = array(
  	'#type' => 'submit',
  	'#value' => t('Submit'),
  );

  $form['col1_close'] = array('#markup' => '</td>');
  $form['col2_open'] = array('#markup' => '<td style="width: 50%; vertical-align: top;">');

  $worker_html = '<h2 class="big">Worker</h2>';
  $worker_html .= '<h2>Member Status / Industries</h2>';
	$worker_html .= sirius_worker_render_industries($worker_node, array('table_attributes' => array('class' => array('sirius_data_table'))));
	$worker_html .= l($worker_node->title . '\'s Industry Tab', 'node/' . $worker_node->nid . '/sirius_worker_industry', array('attributes' => array('class' => array('tool-button'))));
	$worker_html .= '<br /><br /><h2>Training Center Level(s)</h2>';
	$worker_html .= sirius_training_render_levels($worker_node);
	$worker_html .= l($worker_node->title . '\'s Training Center Tab', 'node/' . $worker_node->nid . '/sirius_worker_tc/level', array('attributes' => array('class' => array('tool-button'))));
	$worker_html .= '<hr>';

  $form['worker'] = array('#markup' => $worker_html);
  $form['application'] = array('#markup' => '<h2 class="big">Application</h2>' . sirius_event_intake_application_render($application_node));
  $form['col2_close'] = array('#markup' => '</td></tr></table>');

	return $form;
}

function sirius_node_tab_event_intake_application_manage_ms_form_submit($form, &$form_state) {
	$application_node = node_load($form_state['application_nid']);
	$worker_node = sirius_log_handler($application_node, 'sirius_worker');

	$ms_tid_levels = array_filter(array_values($form_state['values']['ms_levels']));
	if ($ms_tid_levels) {
		foreach ($ms_tid_levels as $ms_tid_level) {
		  list ($ms_tid, $level_code) = explode('|', $ms_tid_level);
	  	$ms_term = taxonomy_term_load($ms_tid);
		  if ($ms_tid) {
		  	$result = sirius_worker_member_status_set($worker_node, $ms_term, TRUE, 'intake');
		  	if ($result['success']) {
		  		drupal_set_message($ms_term->name . ': ' . $result['msg']);
		  	} else {
		  		drupal_set_message("Unable to add member status " . $ms_term->name . ': ' . $result['msg'], 'warning');
		  	}
		  }

		  if ($level_code) {
		  	$level = sirius_training_levels_fetch_record($ms_tid, $level_code);
		  	$result = sirius_training_level_graduate($worker_node, $ms_term, $level, array());
		  	if ($result['success']) {
		  		drupal_set_message("Set level to: " . $level['title']);
		  	} else {
		  		drupal_set_message("Unable to set level to " . $level['title'] . ': ' . $result['msg'], 'warning');
		  	}
		  }
		}
	}

	if (isset($form_state['values']['assign'])) {
		$result = sirius_event_intake_application_assignment_set($application_node, $form_state['values']['assign']);
		if ($result['success']) {
			drupal_set_message($result['msg']); 
		} else {
			drupal_set_message($result['msg'], 'warning');
		}
	}

	if ($form_state['values']['complete'] == 'Yes') {
		$result = sirius_event_intake_application_complete($application_node);
		if ($result['success']) {
			drupal_set_message($result['msg']); 
		} else {
			drupal_set_message($result['msg'], 'warning');
		}
	}	
}

function sirius_node_tab_event_intake_application_manage_ms_options($event_json) {
	$all_ms_level_options = sirius_ledger_ms_level_options();
	$options = array();
	if ($event_json['intake']['ms']['optional']) {
		$values = array_filter(array_values($event_json['intake']['ms']['optional']));
		foreach ($values as $ms_tid_level) {
			$options[$ms_tid_level] = $all_ms_level_options[$ms_tid_level];
		}
	}
	return $options;
}