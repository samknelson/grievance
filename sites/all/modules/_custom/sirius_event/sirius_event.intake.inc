<?php

/**
 * Fetch "My" application
 */

function sirius_event_intake_mine($event_nid) {
	return node_load($_SESSION["sirius_event_${event_nid}_intake"]);
}

/**
 * Set "My" application
 */

function sirius_event_intake_mine_set($event_nid, $application_nid) {
	$_SESSION["sirius_event_${event_nid}_intake"] = $application_nid;
}

/**
 * Create "My" new blank application
 */

function sirius_event_intake_mine_create($event_nid, $application_json = NULL) {
	$event_node = node_load($event_nid);
	$application_node = sirius_log('event:intake_application', 'draft', 'New', 'New', NULL, array($event_nid), array(), $application_json);
	$application_node->field_sirius_domain = $event_node->field_sirius_domain;
	sirius_event_intake_set_title($application_node);
	sirius_event_intake_mine_set($event_nid, $application_node->nid);
	return $application_node;
}

function sirius_event_intake_enabled_tid($event_type_tid) { return sirius_event_intake_enabled_type(taxonomy_term_load($event_type_tid)); }

function sirius_event_intake_enabled_type($event_type_term) {
  $json = sirius_jsonfield_util_term_json_get($event_type_term);
  return $json['intake']['enabled'] == 'Yes';
}


function sirius_event_intake_enabled_nid($event_nid) { return sirius_event_intake_enabled(node_load($event_nid)); }

function sirius_event_intake_enabled($event_node) {
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	return $event_json['intake']['enabled'] == 'Yes';
}

function sirius_event_intake_settings_form($form, &$form_state, $entity_type, $entity) {
  drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/event.css', 'file');
	$form_state['entity_type'] = $entity_type;
	if ($entity_type == 'node') {
		$form_state['entity_id'] = $entity->nid;
		$json = sirius_jsonfield_util_node_json_get($entity);
		$event_type_term = taxonomy_term_load($entity->field_sirius_event_type['und'][0]['tid']);
	} else if ($entity_type == 'taxonomy_term') {
		$form_state['entity_id'] = $entity->tid;
		$json = sirius_jsonfield_util_term_json_get($entity);
		$event_type_term = $entity;
	}
	$intake_json = $json['intake'];

	$prole_tids = sirius_fieldvals($event_type_term, 'field_sirius_event_proles', 'tid');
	$prole_terms = taxonomy_term_load_multiple($prole_tids);
	$prole_options = array();
	foreach ($prole_terms as $term) {
		$prole_options[$term->tid] = $term->name;
	}
	$prole_tids = array_keys($prole_terms);

	$paccess_options = sirius_event_paccess_options($event_type_term, array('exclude_non_participants' => TRUE));

	$form['#tree'] = TRUE;

	$form['intake']['enabled'] = array(
		'#type' => 'select',
		'#title' => t('Intake Enabled?'),
		'#options' => array('No' => t('No'), 'Yes' => t('Yes')),
		'#description' => t('Does this event type support intake?'),
		'#default_value' => $intake_json['enabled'],
	);

	$domain_nid = sirius_domain_nid();
	$type = 'node:sirius_event_participant:field_sirius_json';
	$survey_options = array(NULL => t('-- Select --'));
	$avail = views_get_view_result('sirius_json_definitions', 'entityreference_1', $domain_nid, $type);
	if ($avail) {
		foreach ($avail as $hr) {
			$survey_options[$hr->nid] = $hr->node_title;
		}
	}

	$surveys = array('default');

	$survey_nid = $intake_json['survey'];
	$form['intake']['survey'] = array(
		'#type' => 'select',
		'#title' => 'Survey',
		'#options' => $survey_options,
		'#default_value' => $survey_nid,
		'#prefix' => $html,
	);
	if ($survey_nid) {
		$html = '<div class="sirius_form_intro">';
		if ($survey_nid) {
			$survey_node = node_load($survey_nid);
			$html = 'See details for <strong>' . $survey_node->title . '</strong>: ';
			$paths = array(
				"node/$survey_nid/sirius_event_participant_definition_form/preview" => 'Preview', 
				"node/$survey_nid/sirius_event_participant_definition_form" => 'Edit Fields', 
			);
			foreach ($paths as $path => $title) {
				if (!drupal_valid_path($path)) { continue; }
				$html .= l($title, $path, array('attributes' => array('class' => array('tool-button'))));
			}
		}
		$html .= '</div><br />';
		$form['intake']['survey_details'] = array('#markup' => $html);
	}

  $form['intake']['register']['paccess'] = array(
  	'#type' => 'select',
  	'#title' => 'Register new participants as',
  	'#options' => array(NULL => t('-- Select --')) + $paccess_options,
  	'#description' => 'When a new worker is created through the intake process, they will also be registered for the event with the selected role and status.',
  	'#default_value' => $json['intake']['register']['paccess'],
  );

  $ws_options = sirius_taxonomy_load('sirius_work_status', 'tid', 'name', TRUE);
	$form['intake']['work_status'] = array(
		'#type' => 'select',
		'#title' => t('Work Status'),
		'#options' => $ws_options,
		'#description' => t('What work status should newly created workers have?'),
		'#default_value' => $json['intake']['work_status'],
	);

  $ms_level_options = array(NULL => t('-- Select --')) + sirius_ledger_ms_level_options();
	$form['intake']['member_status_level'] = array(
		'#type' => 'select',
		'#title' => t('Member Status'),
		'#options' => $ms_level_options,
		'#description' => t('What member status (and level) should newly created workers have?'),
		'#default_value' => $json['intake']['member_status_level'],
	);

	$form['intake']['anonymous'] = array(
		'#type' => 'fieldset',
		'#collapsible' => FALSE,
		'#title' => t('Anonymous Intake'),
		'#description' => t('Anonymous intake allows non-user to create new accounts and register for the event.'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	$form['intake']['anonymous']['enabled'] = array(
		'#type' => 'select',
		'#title' => t('Intake Enabled?'),
		'#options' => array('No' => t('No'), 'Yes' => t('Yes')),
		'#description' => t('Does this event type support intake?'),
		'#default_value' => $intake_json['anonymous']['enabled'],
	);

	if ($entity_type == 'node') {
		$form['intake']['anonymous']['access_code'] = array(
			'#type' => 'textfield',
			'#title' => t('Access Code'),
			'#description' => t('Enter the access code to allow anonymous intake. Leave blank to generate automatically.'),
			'#default_value' => $intake_json['anonymous']['access_code'],
		);
	}

	$form['intake']['anonymous']['title'] = array(
		'#type' => 'textfield',
		'#title' => t('Title'),
		'#description' => t('What page title should be shown to anonymous users?'),
		'#default_value' => $intake_json['anonymous']['title'],
	);

  $form['intake']['anonymous']['intro'] = array(
    '#type' => 'text_format',
    '#title' => 'Introductory Text',
    '#default_value' => $json['intake']['anonymous']['intro'],
    '#format' => 'sirius_form_editor',
    '#allowed_formats' => array('sirius_form_editor'),
    '#description' => t('This is the text that will be displayed at the first step of the intake process.')
  );

  $form['intake']['anonymous']['submit'] = array(
    '#type' => 'text_format',
    '#title' => 'Submit Text',
    '#default_value' => $json['intake']['anonymous']['submit'],
    '#format' => 'sirius_form_editor',
    '#allowed_formats' => array('sirius_form_editor'),
    '#description' => t('This is the text that will appear before the intake application is submitted. It should describe the review process and what the next steps for the applicant will be.')
  );

	$defs = sirius_esignature_defs();
	if (!$defs) {
		$form['intake']['anonymous']['esignature']['def'] = array('#markup' => '<p>' . t('There are no signatures configured. Go to the ') . l('configuration page', 'sirius/esignature/admin') . t(' to create them.') . '</p>');
	}	else {
		$options = array(NULL => t('-- Select --'));
		foreach ($defs as $def) {
			$options[$def['code']] = $def['title'];
		}
		$form['intake']['anonymous']['esignature']['def'] = array(
			'#title' => 'Signature',
			'#type' => 'select',
			'#options' => $options,
			'#default_value' => $json['intake']['anonymous']['esignature']['def'],
	 	);
	 	if (drupal_valid_path('sirius/esignature/admin')) {
	 		$form['intake']['anonymous']['esignature']['def']['#title'] .= ' ' . l('(Manage)', 'sirius/esignature/admin');
	 	}
		$form['intake']['anonymous']['esignature']['statement'] = array(
			'#title' => 'Statment',
			'#type' => 'textarea',
			'#default_value' => $json['intake']['anonymous']['esignature']['statement'],
			'#description' => t('E.g. "By signing below, I assert that all the information that I have provided in this application is true and accurate."'),
	 	);
	}

  $attachment_types = sirius_materials_document_types('sirius_worker');
  $attachment_options = array();
  foreach ($attachment_types as $tid => $term) { $attachment_options[$tid] = $term->name; }

  $form['intake']['attachments'] = array(
  	'#type' => 'fieldset',
  	'#title' => t('Attachments'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
		'#description' => 'Any "grievance document type" that is associated with workers may be specified as an attachment for the intake wizard. The available document types can be managed ' . 
  		l('here.', 'sirius/taxonomy/grievance_document_types'),
  );

  $form['intake']['attachments']['required'] = array(
  	'#type' => 'checkboxes',
  	'#title' => 'Required Attachments',
  	'#options' => $attachment_options,
  	'#description' => 'These attachments are required of all intakes, and must be uploaded for the intake to be complete.',
  );
  if ($json['intake']['attachments']['required']) { $form['intake']['attachments']['required']['#default_value'] = $json['intake']['attachments']['required']; }

  $form['intake']['attachments']['optional'] = array(
  	'#type' => 'checkboxes',
  	'#title' => 'Optional Attachments',
  	'#options' => $attachment_options,
  	'#description' => 'These attachments are optional, and the applicant can decide whether or not to include them.',
  );
  if ($json['intake']['attachments']['optional']) { $form['intake']['attachments']['optional']['#default_value'] = $json['intake']['attachments']['optional']; }

	$form['intake']['notifications']['accept'] = array(
		'#type' => 'fieldset',
		'#title' => t('Notifications - Accepted'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

  $form['intake']['notifications']['accept']['email_subject'] = array(
    '#type' => 'textfield',
    '#title' => 'Email Subject',
    '#default_value' => $json['intake']['notifications']['accept']['email_subject'],
  );

  $form['intake']['notifications']['accept']['email_body'] = array(
    '#type' => 'text_format',
    '#title' => 'Email Body',
    '#default_value' => $json['intake']['notifications']['accept']['email_body'],
    '#format' => 'sirius_form_editor',
    '#allowed_formats' => array('sirius_form_editor'),
  );

  $form['intake']['notifications']['accept']['sms'] = array(
    '#type' => 'textarea',
    '#title' => 'SMS',
    '#default_value' => $json['intake']['notifications']['accept']['sms'],
  );

	$form['intake']['notifications']['reject'] = array(
		'#type' => 'fieldset',
		'#title' => t('Notifications - Rejected'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

  $form['intake']['notifications']['reject']['email_subject'] = array(
    '#type' => 'textfield',
    '#title' => 'Email Subject',
    '#default_value' => $json['intake']['notifications']['reject']['email_subject'],
  );

  $form['intake']['notifications']['reject']['email_body'] = array(
    '#type' => 'text_format',
    '#title' => 'Email Body',
    '#default_value' => $json['intake']['notifications']['reject']['email_body'],
    '#format' => 'sirius_form_editor',
    '#allowed_formats' => array('sirius_form_editor'),
  );

  $form['intake']['notifications']['reject']['sms'] = array(
    '#type' => 'textarea',
    '#title' => 'SMS',
    '#default_value' => $json['intake']['notifications']['reject']['sms'],
  );

	$form['intake']['admin_prole_tids'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Admin Roles'),
		'#options' => $prole_options,
		'#description' => t('These roles have permission to use the intake tools.'),
	);
	if ($intake_json['admin_prole_tids']) { $form['intake']['admin_prole_tids']['#default_value'] = $intake_json['admin_prole_tids']; }

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_intake_settings_form_submit($form, &$form_state) {
	$entity_type = $form_state['entity_type'];
	$entity_id = $form_state['entity_id']; 
	if ($entity_type == 'node') { $entity = node_load($entity_id); }
	else if ($entity_type == 'taxonomy_term') { $entity = taxonomy_term_load($entity_id); }

	$json = sirius_jsonfield_util_entity_json_get($entity);
	$json['intake'] = $form_state['values']['intake'];
	$json['intake']['anonymous']['intro'] = sirius_richtext_handle_input($form_state['values']['intake']['anonymous']['intro']);
	$json['intake']['anonymous']['submit'] = sirius_richtext_handle_input($form_state['values']['intake']['anonymous']['submit']);
	$json['intake']['notifications']['accept']['email_body'] = sirius_richtext_handle_input($form_state['values']['intake']['notifications']['accept']['email_body']);
	$json['intake']['notifications']['reject']['email_body'] = sirius_richtext_handle_input($form_state['values']['intake']['notifications']['reject']['email_body']);
	$json['intake']['anonymous']['access_code'] = sirius_event_intake_access_code_anonicalize($form_state['values']['intake']['anonymous']['access_code']);
	if (!$json['intake']['anonymous']['access_code']) {
		$json['intake']['anonymous']['access_code'] = sirius_aat_generate(8, 'letter');
	}

	sirius_jsonfield_util_entity_json_set($entity, $json);

	if ($entity_type == 'node') {
		node_save($entity);
	} 
	else if ($entity_type == 'taxonomy_term') {
		taxonomy_term_save($entity);
	}

	drupal_set_message("Settings saved.");
}

function sirius_event_intake_access_code_anonicalize($code) {
	$code = preg_replace('/[^a-zA-Z0-9_ ]/', '', $code);
	$code = strtoupper($code);
	$code = trim($code);
	return $code;
}

function sirius_event_intake_fetch_by_access_code($access_code) {
	$sql = "select nid ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_event' ";
	$sql .= "and json_unquote(json_extract(field_sirius_json_value, '\$.intake.anonymous.access_code')) = :access_code ";
	$sql .= "order by nid desc ";
	$stmt = sirius_sql_query($sql, array(':access_code' => $access_code));
	$matches = array();
	while ($hr = $stmt->fetchAssoc()) {
		if (!$fetchall) { return node_load($hr['nid']); }
		$matches[] = $hr['nid'];
	}
	return $matches;
}

function sirius_event_intake_set_title(&$application_node) {
	$title = "Application - ";
	$application_json = sirius_jsonfield_util_term_json_get($application_node);
	if ($application_json['wizard']['intake']['identity']['name_first']) {
		$title .= $application_json['wizard']['intake']['identity']['name_first'] . ' ' . $application_json['wizard']['intake']['identity']['name_last'];
	} else {
		$title .= '(no name)'; 
	}
	$title .= ' - ';

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	if (!$event_node) {
		$title .= '(no event)'; 
	} else {
		$event_json = sirius_jsonfield_util_node_json_get($event_node);
		$title .= $event_json['intake']['anonymous']['title'];
		if (!$title) { $title .= $event_node->title; }
	}

	if ($title != $application_node->title) {
		$application_node->title = substr($title, 0, 255);
		node_save($application_node);
	}
}

function sirius_event_intake_application_status_options() {
	return array(
		'draft' => 'Draft',
		'submitted' => 'Submitted',
		'rejected' => 'Rejected',
		'accepted' => 'Accepted',
	);
}

function sirius_event_intake_active($event_node) {
  $result = NULL;

  $json = sirius_jsonfield_util_node_json_get($event_node);

  if ($json['intake_status']['active'] == 'Yes') { 
    $result = array('success' => TRUE);
  } else if ($json['intake_status']['active'] == 'No') { 
    $result = array('success' => FALSE, 'msg' => "The intake form has been closed by an administrator.");
  }
  else if ($json['intake_status']['active'] == 'Auto') { 
    $open_grace = $json['intake_status']['active_grace_open'];
    if ($open_grace > 0) { $open_grace *= 60; } else { $open_grace = 0; }

    $close_grace = $json['intake_status']['active_grace_close'];
    if ($close_grace) { $close_grace *= 60; } else { $close_grace = 0; }

    if ($event_node->field_sirius_daterepeat['und']) {
      $ts_now = time();
      foreach ($event_node->field_sirius_daterepeat['und'] as $tmp) {
        $start_ts = strtotime($tmp['value'] . ' ' . $tmp['timezone_db']);
        $end_ts = strtotime($tmp['value2'] . ' ' . $tmp['timezone_db']);
        if ($start_ts <= 1 || $end_ts <= 1) { continue; }
        $start_ts -= $open_grace;
        $end_ts += $close_grace;
        if ($ts_now >= $start_ts && $ts_now <= $end_ts) {
          $result = array('success' => TRUE);
        }
      }
    }
    if (!$result['success']) {
      $result = array('success' => FALSE, 'msg' => "The intake form is currently closed. " . sirius_event_intake_opens_in($event_node));
    }
  } else { 
    $result = array('success' => FALSE);
  }

  return $result;
}

function sirius_event_intake_opens_in($event_node) {
  if (!$event_node->field_sirius_daterepeat['und']) { return ''; }
  $ts_now = time();
  $opens_in = 999999999;
  foreach ($event_node->field_sirius_daterepeat['und'] as $tmp) {
    $start_ts = strtotime($tmp['value'] . ' ' . $tmp['timezone_db']);
    $end_ts = strtotime($tmp['value2'] . ' ' . $tmp['timezone_db']);
    if ($start_ts <= 1 || $end_ts <= 1) { continue; }
    if ($start_ts >= $ts_now) {
      $range = $start_ts - $ts_now;
      if ($range && $range < $opens_in) { $opens_in = $range; }
    }
  }
  if ($opens_in > 1 && $opens_in < 999999999) {
    return t('Opening in: @interval', array('@interval' => format_interval($opens_in)));
  }
}

function sirius_event_intake_closes_in($event_node) {
  if (!$event_node->field_sirius_daterepeat['und']) { return ''; }
  $ts_now = time();
  $closes_in = 0;
  foreach ($event_node->field_sirius_daterepeat['und'] as $tmp) {
    $start_ts = strtotime($tmp['value'] . ' ' . $tmp['timezone_db']);
    $end_ts = strtotime($tmp['value2'] . ' ' . $tmp['timezone_db']);
    if ($start_ts <= 1 || $end_ts <= 1) { continue; }
    if ($start_ts <= $ts_now && $end_ts >= $ts_now) {
      $range = $end_ts - $ts_now;
      if ($range && $range > $closes_in) { $closes_in = $range; }
    }
  }
  if ($closes_in > 1) {
    return t('Closes in: @interval', array('@interval' => format_interval($closes_in)));
  }
}
