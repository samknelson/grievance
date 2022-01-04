<?php

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
	$domain_nid = sirius_domain_nid();

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
		'#default_value' => $json['intake']['enabled'],
	);


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

	$type = 'node:sirius_event_participant:field_sirius_json';
	$survey_options = array(NULL => t('-- Select --'));
	$avail = views_get_view_result('sirius_json_definitions', 'entityreference_1', $domain_nid, $type);
	if ($avail) {
		foreach ($avail as $hr) {
			$survey_options[$hr->nid] = $hr->node_title;
		}
	}

  $ajax_params = $form_state['triggering_element']['#sirius_ajax'];
  $survey_count = $form_state['survey_count'];
  $survey_count += $ajax_params['qty'];
  if ($survey_count < 1) { $survey_count = 1; }
  if ($survey_count < count($json['intake']['survey']['list'])) { $survey_count = count($json['intake']['survey']['list']); }
  $form_state['survey_count'] = $survey_count;

  $form['intake']['survey'] = array(
  	'#type' => 'fieldset',
  	'#title' => t('Surveys'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
		'#description' => 'Select the survey form(s) that the applicant must complete. Each form will be presented as a separate step in the intake wizard.' . l('Manage Surveys', 'sirius/event/participant_definitions', array('attributes' => array('class' => array('tool-button')))) . '<br /><br />',
  );

	$form['intake']['survey']['list']['prefix'] = array('#markup' => '<div id="survey_list">');

  for ($i=0; $i<$survey_count; ++$i) {
  	$title = "Survey #" . ($i+1);
  	if ($json['intake']['survey']['list'][$i]['title']) { $title .= ': ' . $json['intake']['survey']['list'][$i]['title']; }

  	$form['intake']['survey']['list'][$i] = array(
  		'#type' => 'fieldset',
  		'#title' => $title,
  	);

  	$survey_nid = $json['intake']['survey']['list'][$i]['survey_nid'];
		if ($survey_nid) {
			if ($survey_nid) {
				$html = '<div class="sirius_form_intro">';
				$survey_node = node_load($survey_nid);
				$html .= 'See details for <strong>' . $survey_node->title . '</strong>: ';
				$paths = array(
					"node/$survey_nid/sirius_event_participant_definition_form/preview" => 'Preview', 
					"node/$survey_nid/sirius_event_participant_definition_form" => 'Edit Fields', 
				);
				foreach ($paths as $path => $title) {
					if (!drupal_valid_path($path)) { continue; }
					$html .= l($title, $path, array('attributes' => array('class' => array('tool-button'))));
				}
				$html .= '</div><br />';
				$form['intake']['survey']['list'][$i]['survey_details'] = array('#markup' => $html);
			}
		}

  	$form['intake']['survey']['list'][$i]['title'] = array(
  		'#title' => t('Title'),
  		'#type' => 'textfield',
  		'#default_value' => $json['intake']['survey']['list'][$i]['title'],
  	);

  	$form['intake']['survey']['list'][$i]['survey_nid'] = array(
  		'#title' => t('Survey'),
  		'#type' => 'select',
  		'#options' => $survey_options,
  		'#default_value' => $json['intake']['survey']['list'][$i]['survey_nid'],
  	);
  }

	$form['intake']['survey']['list']['suffix'] = array('#markup' => '</div>');

  $form['intake']['survey']['add_one'] = array(
    '#type' => 'button',
    '#value' => t('Add one'),
    '#ajax' => array(
      'callback' => 'sirius_ajax_generic_replace',
    ),
    '#sirius_ajax' => array(
      'qty' => 1,
      'wrapper' => '#survey_list',
      'element' => 'intake:survey:list',
    ),
  );

  $form['intake']['ms'] = array(
  	'#type' => 'fieldset',
  	'#title' => t('Member Status / Industries'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
		'#description' => t('Select the member status / industry and apprenticeship level that newly created workers will have '),
  );

  $ms_level_options = sirius_ledger_ms_level_options();
	$form['intake']['ms']['required'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Required'),
		'#options' => $ms_level_options,
		'#description' => t('All newly created workers will have these member statuses'),
	);
	if ($json['intake']['ms']['required']) { $form['intake']['ms']['required']['#default_value'] = $json['intake']['ms']['required']; }

	$form['intake']['ms']['optional'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Optional'),
		'#options' => $ms_level_options,
		'#description' => t('Newly created workers may optionally be assigned these member status.'),
	);
	if ($json['intake']['ms']['optional']) { $form['intake']['ms']['optional']['#default_value'] = $json['intake']['ms']['optional']; }


  $form['intake']['skills'] = array(
  	'#type' => 'fieldset',
  	'#title' => t('Skills'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
  );

	$skill_categories = array('verified' => 'Verified', 'unverified' => 'Unverified');
	$skill_options = sirius_taxonomy_load('sirius_skill', 'tid', 'name');
	foreach ($skill_categories as $key => $category_name) {
		$count = 0;
		if ($json['intake']['skills'][$key]) { $count = count($json['intake']['skills'][$key]); }
		$count += 5;
		$form['intake']['skills'][$key] = array(
			'#type' => 'fieldset',
			'#title' => $category_name,
			'#collapsible' => TRUE,
			'#collapsed' => TRUE,
		);
		for ($i=0; $i<$count; ++$i) {
			$form['intake']['skills'][$key][$i] = array(
				'#title' => $category_name . ' Skill #' . ($i+1),
				'#type' => 'select',
				'#options' => array(NULL => t('-- Select --')) + $skill_options,
				'#default_value' => $json['intake']['skills'][$key][$i],
			);
		}
	}

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
		'#default_value' => $json['intake']['anonymous']['enabled'],
	);

	if ($entity_type == 'node') {
		$form['intake']['anonymous']['access_code'] = array(
			'#type' => 'textfield',
			'#title' => t('Access Code'),
			'#description' => t('Enter the access code to allow anonymous intake. Leave blank to generate automatically.'),
			'#default_value' => $json['intake']['anonymous']['access_code'],
		);
	}

	$form['intake']['anonymous']['title'] = array(
		'#type' => 'textfield',
		'#title' => t('Title'),
		'#description' => t('What page title should be shown to anonymous users?'),
		'#default_value' => $json['intake']['anonymous']['title'],
	);

	$form['intake']['anonymous']['require_ssn'] = array(
		'#type' => 'select',
		'#title' => t('Require SSN?'),
		'#description' => t('Should anonymous users be required to submit a valid, unique SSN? Default is "Yes".'),
		'#default_value' => $json['intake']['anonymous']['require_ssn'],
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
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

  $form['intake']['worker'] = array(
  	'#type' => 'fieldset',
  	'#title' => t('Workers'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
		'#description' => t('Settings for the Worker record which is created as part of the intake process.'),
  );


	$id2_info = field_info_instance('node', 'field_sirius_id2', 'sirius_worker');
	$id2_label = $id2_info['label'];

  $form['intake']['worker']['id2_required'] = array(
  	'#type' => 'select',
  	'#title' => $id2_label . ' Required',
  	'#description' => "Should the field <strong>$id2_label</strong> be required for all newly created workers?",
  	'#options' => array(
  		NULL => t('-- Select --'),
  		'No' => t('No'),
  		'Yes' => t('Yes'),
  	),
  	'#default_value' => $json['intake']['worker']['id2_required'],
  );

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

  $notifications = array(
  	'accept' => 'Accepted',
  	'reject' => 'Rejected',
  	'submit' => 'Submitted',
  );

  foreach ($notifications as $key => $name) {
		$form['intake']['notifications'][$key] = array(
			'#type' => 'fieldset',
			'#title' => 'Notifications - ' . $name,
			'#collapsible' => TRUE,
			'#collapsed' => TRUE,
		);

	  $form['intake']['notifications'][$key]['email_subject'] = array(
	    '#type' => 'textfield',
	    '#title' => 'Email Subject',
	    '#default_value' => $json['intake']['notifications'][$key]['email_subject'],
	  );

	  $form['intake']['notifications'][$key]['email_body'] = array(
	    '#type' => 'text_format',
	    '#title' => 'Email Body',
	    '#default_value' => $json['intake']['notifications'][$key]['email_body'],
	    '#format' => 'sirius_form_editor',
	    '#allowed_formats' => array('sirius_form_editor'),
	  );

	  $form['intake']['notifications'][$key]['sms'] = array(
	    '#type' => 'textarea',
	    '#title' => 'SMS',
	    '#default_value' => $json['intake']['notifications'][$key]['sms'],
	  );
	}

	$form['intake']['assignments'] = array(
		'#type' => 'fieldset',
		'#title' => 'Assignments',
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	$form['intake']['assignments']['options'] = array(
		'#title' => 'Assignment Options',
		'#type' => 'textarea',
		'#description' => 'Use this field to list the entities that the intake application may be assigned to. For example, an application might be assigned to "Union Hall", "Training Center", or "Accounting". The first entry will be the default, and all newly created applications will be assigned to this entity.',
		'#default_value' => $json['intake']['assignments']['options'],
	);

	$form['intake']['admin_prole_tids'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Admin Roles'),
		'#options' => $prole_options,
		'#description' => t('These roles have permission to use the intake tools.'),
	);
	if ($json['intake']['admin_prole_tids']) { $form['intake']['admin_prole_tids']['#default_value'] = $json['intake']['admin_prole_tids']; }

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
	$json['intake']['notifications']['submit']['email_body'] = sirius_richtext_handle_input($form_state['values']['intake']['notifications']['submit']['email_body']);
	$json['intake']['anonymous']['access_code'] = sirius_event_intake_access_code_anonicalize($form_state['values']['intake']['anonymous']['access_code']);
	if (!$json['intake']['anonymous']['access_code']) {
		$json['intake']['anonymous']['access_code'] = sirius_aat_generate(8, 'letter');
	}
	if ($json['intake']['survey']['list']) {
		foreach ($json['intake']['survey']['list'] as $delta => $item) {
			if (!$item['survey_nid']) {
				unset($json['intake']['survey']['list'][$delta]);
			}
		}
	}

	$skill_categories = array('verified' => 'Verified', 'unverified' => 'Unverified');
	foreach ($skill_categories as $key => $category_name) {
		foreach ($json['intake']['skills'][$key] as $delta => $value) {
			if (!$value) { unset($json['intake']['skills'][$key][$delta]); }
		}
		$json['intake']['skills'][$key] = array_values($json['intake']['skills'][$key]);
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

