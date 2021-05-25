<?php

function sirius_node_tab_event_intake_application_manage_skill_access($application_node, $skill_category) {
	// Must be an application-type log entry
	if ($application_node->field_sirius_category['und'][0]['value'] != 'event:intake_application') { return FALSE; }

	// There must already be a worker
	if (!sirius_log_handler($application_node, 'sirius_worker')) { return FALSE; }

	// There must be an event
	$event_node = sirius_log_handler($application_node, 'sirius_event');
	if (!$event_node) { return FALSE; }
	$event_json = sirius_json_get($event_node);

	// There must be some optional ms
	$skill_options = sirius_node_tab_event_intake_application_manage_skill_options($event_json, $skill_category);
	if (!$skill_options) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_event_intake_application_manage_skill($application_node, $skill_category) {
	return drupal_get_form('sirius_node_tab_event_intake_application_manage_skill_form', $application_node, $skill_category);
}

function sirius_node_tab_event_intake_application_manage_skill_form($form, &$form_state, $application_node, $skill_category) {
	form_load_include($form_state, 'inc', 'sirius_event', 'sirius_event.page_intake_application_manage_skill');

	$form_state['application_nid'] = $application_node->nid;
	$worker_node = sirius_log_handler($application_node, 'sirius_worker');
	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$application_json = sirius_json_get($application_node);

	$skill_options = sirius_node_tab_event_intake_application_manage_skill_options($event_json, $skill_category);

  drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/event.css', 'file');

  $form['#tree'] = TRUE; 
  $form['table_open'] = array('#markup' => '<table class="columns_table">');
  $form['col1_open'] = array('#markup' => '<tr><td style="width: 50%; vertical-align: top;">');

	$skill_terms = sirius_taxonomy_load('sirius_skill', 'tid', 'full');
	$skill_days = array();
	$skill_expiration_defaults = array();
	foreach ($skill_options as $skill_tid => $skill_name) {
		$skill_term = $skill_terms[$skill_tid];
		$days = $skill_term->field_grievance_days['und'][0]['value'];
		if ($days) {
			$skill_days[$skill_tid] = $days;
			$skill_expiration_defaults[$skill_tid] = gmdate('m/d/Y', strtotime("+$days days"));
		}
	}

	if ($worker_node->field_sirius_skill['und']) {
		foreach ($worker_node->field_sirius_skill['und'] as $tmp) {
			unset($skill_options[$tmp['tid']]);
		}
	}

	$form['b1'] = array('#markup' => '<h2>' . t('Add New Skills') . '</h2>');
	$form['b2'] = array('#markup' => '<table class="sirius_data_table">');
	$form['b2x'] = array('#markup' => '<tr><th>Skill</th><th>Expiration Date</th></tr>');
	for ($delta = 1; $delta < 10; ++$delta) {
		$form[$delta]['tr_open'] = array('#markup' => '<tr>');

		$form[$delta]['skill_a'] = array('#markup' => '<td>');
		$form[$delta]['skill'] = array(
			'#type' => 'select',
			'#options' => array(NULL => t('-- Select --')) + $skill_options,
	    '#ajax' => array('callback' => 'sirius_node_tab_worker_skill_add_form_ajax'),
		);
		$form[$delta]['skill_b'] = array('#markup' => '</td>');

		$form[$delta]['renew_a'] = array('#markup' => '<td>');

		$form[$delta]['renew'] = array(
			'#type' => 'textfield',
			'#prefix' => '<div id="renew_' . $delta . '">',
			'#suffix' => '</div>',
			'#size' => 20,
		);
		$form[$delta]['renew_b'] = array('#markup' => '</td>');
		$form[$delta]['tr_close'] = array('#markup' => '</tr>');

		$skill_tid = $form_state['input'][$delta]['skill'];
		if ($skill_tid) {
			if (!$form_state['input'][$delta]['renew']) {
				unset($form_state['input'][$delta]['renew']);
				$days = $skill_days[$skill_tid];
				if ($days) {
					$form[$delta]['renew']['#default_value'] = $skill_expiration_defaults[$skill_tid];
				} else {
					$form[$delta]['renew']['#default_value'] = '(never expires)';
				}
			}
		} else {
			// $form[$delta]['renew']['#type'] = 'hidden';
		}
	}
	$form['b3'] = array('#markup' => '</table>');

	$attachments = $application_json['wizard']['intake']['attachments'];
	if ($attachments) {
		$attachment_options = array();
		foreach ($attachments as $attachment_tid => $attachment) {
			$attachment_term = taxonomy_term_load($attachment_tid);
			$attachment_options[$attachment['fid']] = $attachment_term->name;
		}
		$form['skill_attachments'] = array(
			'#title' => t('Skill Attachments'),
			'#type' => 'checkboxes',
			'#options' => $attachment_options,
			'#description' => t('When adding skills, log the entry with the checked attachments.'),
		);
	}

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

  $worker_html .= '<h2>Skills</h2>';
	if ($worker_node->field_sirius_skill['und']) {
		$rows = array();
		$header = array(t('Skill'), t('Expires'));
		foreach ($worker_node->field_sirius_skill['und'] as $delta => $tmp) {
			$skill_tid = $tmp['tid'];
			$skill_term = $skill_terms[$skill_tid];

			$expires_ts = strtotime($worker_node->field_sirius_skill_expire['und'][$delta]['value']);
			if (!$expires_ts || $expires_ts > time() + 60*60*24*365*50) {
				$expires = sirius_domain_variable_get('sirius_worker_skill_never_expires_text', 'Never Expires');
			} else {
				$expires = t('Expires: @date', array('@date' => date('m/d/Y', $expires_ts)));
				$expires .= ' (' . format_interval($expires_ts - time(), 2) . t(' remaining') . ')';
			}

			$row = array();
			$row[] = $skill_term->name;
			$row[] = $expires;
			$rows[] = $row;
		}
		$worker_html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array('class' => array('sirius_data_table'))));
	} else {
		$worker_html .= t('@name does not currently have any skills registered.', array('@name' => $worker_node->title));
		$worker_html .= '<br />';
	}
	$worker_html .= l($worker_node->title . '\'s Skills Tab', 'node/' . $worker_node->nid . '/sirius_worker_skill', array('attributes' => array('class' => array('tool-button'))));
	$worker_html .= '<br /><br />';

  $worker_html .= '<h2>Member Status / Industries</h2>';
	$worker_html .= sirius_worker_render_industries($worker_node, array('table_attributes' => array('class' => array('sirius_data_table'))));
	$worker_html .= l($worker_node->title . '\'s Industry Tab', 'node/' . $worker_node->nid . '/sirius_worker_industry', array('attributes' => array('class' => array('tool-button'))));
	$worker_html .= '<hr>';

  $form['worker'] = array('#markup' => $worker_html);
  $form['application'] = array('#markup' => '<h2 class="big">Application</h2>' . sirius_event_intake_application_render($application_node));
  $form['col2_close'] = array('#markup' => '</td></tr></table>');

	return $form;
}

function sirius_node_tab_event_intake_application_manage_skill_form_submit($form, &$form_state) {
	$application_node = node_load($form_state['application_nid']);
	$worker_node = sirius_log_handler($application_node, 'sirius_worker');
	$event_node = sirius_log_handler($application_node, 'sirius_event');

  $reason = 'Intake from ' . $event_node->title;

  $delta = 1;
	$files = array();
	if ($form_state['values']['skill_attachments']) {
		$fids = array_filter(array_values($form_state['values']['skill_attachments']));
		foreach ($fids as $fid) {
			$file = (array) file_load($fid);
			$file['display'] = TRUE;
			$files[] = $file;
		}
	}

  while ($form_state['values'][$delta]['skill']) {
  	$tid = $form_state['values'][$delta]['skill'];
  	$expires = $form_state['values'][$delta]['renew'];
  	if (sirius_strtotime($expires) < 1) { $expires = NULL; }
		$result = sirius_worker_skill_set($worker_node, array($tid), $expires, 'add', $reason, $files);
		if ($result['success']) {
			drupal_set_message($result['msg']);
		} else {
			drupal_set_message($result['msg'], 'warning');
		}
  	++$delta;
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

function sirius_node_tab_event_intake_application_manage_skill_options($event_json, $skill_category) {
	$options = array();
	if ($event_json['intake']['skills'][$skill_category]) {
		$all_options = sirius_taxonomy_load('sirius_skill', 'tid', 'name');
		$values = array_filter(array_values($event_json['intake']['skills'][$skill_category]));
		foreach ($values as $tid) {
			$options[$tid] = $all_options[$tid];
		}
	}
	return $options;
}