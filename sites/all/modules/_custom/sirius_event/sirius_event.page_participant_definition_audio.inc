<?php
/**
 * @todo: I've stupidly merged the concepts of "Audio" and "OMG". This page claims to be a tool to manage audio attachments to a participant definition 
 * node. In fact, it's a page to manage outgoing messages -- which might be an audio attachment, but which might also be simply an SMS, or, eventually, something
 * else entirely like an email. So this is a bit of a mess.
 */

function sirius_node_tab_event_participant_definition_audio_access($definition_node) {
	if ($definition_node->field_sirius_type['und'][0]['value'] != 'node:sirius_event_participant:field_sirius_json') { return FALSE; }
	return TRUE;
}

function sirius_node_tab_event_participant_definition_audio($definition_node) {
	return drupal_get_form('sirius_node_tab_event_participant_definition_audio_form', $definition_node->nid);
}

function sirius_node_tab_event_participant_definition_audio_form($form, &$form_state, $definition_nid) {
	$form_state['definition_nid'] = $definition_nid;
	$definition_node = node_load($definition_nid);
	$json = sirius_jsonfield_util_node_json_get($definition_node);

	$form['#tree'] = TRUE;
	$form['intro'] = array('#markup' => '<div class="sirius_form_intro">' . t('Use this form to manage outgoing messages for phone banks. Audio outgoing messages may be uploaded in wav or mp3 format. (You may want to use a service such as <a href="https://online-voice-recorder.com/" target="_blank">online-voice-recorder.com</a>.) Outgoing messages may also be configured to use a text message.') . '<br /><br /></div>');

	$omg_records = sirius_event_omg_list($definition_node);
	$omg_records[] = array();
	$audio_items = field_get_items('node', $definition_node, 'field_sirius_audio');

	$pstatus_options = sirius_taxonomy_load('sirius_event_participant_status', 'tid', 'name', TRUE);
	$form['omg']['pstatus'] = array(
			'#type' => 'select',
			'#title' => t('Participant Status'),
			'#description' => t('When an outgoing message is played, all the members of the household should have their participant status set to this value.'),
			'#options' => $pstatus_options,
			'#default_value' => $json['omg']['pstatus'],
		);

	foreach ($omg_records as $delta => $omg_record) {

		$form['omg']['records'][$delta] = array(
			'#type' => 'fieldset',
			'#title' => 'Outgoing Message #' . ($delta+1),
		);

		$form['omg']['records'][$delta]['title'] = array(
			'#type' => 'textfield',
			'#title' => t('Title'),
			'#description' => t('Title of the outgoing message, as it will be displayed to phonebank users.'),
			'#default_value' => $omg_record['title'],
		);

		$audio_html_id = "omg_records_${delta}_audio";
		$form['omg']['records'][$delta]['audio'] = array(
			'#type' => 'select',
			'#title' => t('Audio File'),
			'#options' => array(
				'No' => 'No Audio File',
				'Yes' => 'Yes, Play an Audio File',
			),
			'#default_value' => $omg_record['audio'],
			'#id' => $audio_html_id,
		);

		$form['omg']['records'][$delta]['audio_file'] = array(
			'#type' => 'file',
			'#title' => 'Upload',
			'#name' => "files[omg_records_${delta}_audio_file]",
			'#states' => array(
				'visible' => array(
					array(':input[id="' . $audio_html_id . '"]' => array('value' => 'Yes')),
				)
			)
		);

		if ($audio_items[$delta]['fid']) {
			$renderable = field_view_value('node', $definition_node, 'field_sirius_audio', $audio_items[$delta], 'full');
			$form['omg']['records'][$delta]['audio_file']['#title'] = 'Replace';
			$form['omg']['records'][$delta]['audio_file']['#field_prefix'] = render($renderable) . '<br />';
		}

		/*
		$sms_html_id = "omg_records_${delta}_sms";
		$form['omg']['records'][$delta]['sms'] = array(
			'#type' => 'select',
			'#title' => t('SMS (Text Message)'),
			'#options' => array(
				'No' => 'No SMS',
				'Yes' => 'Yes, Send an SMS',
			),
			'#default_value' => $omg_record['sms'],
			'#id' => $sms_html_id,
		);

		$form['omg']['records'][$delta]['sms_body'] = array(
			'#type' => 'textarea',
			'#title' => 'SMS Body',
			'#default_value' => $omg_record['sms_body'],
			'#states' => array(
				'visible' => array(
					array(':input[id="' . $sms_html_id . '"]' => array('value' => 'Yes')),
				)
			),
		);
		*/
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_event_participant_definition_audio_form_validate($form, &$form_state) {
	$definition_nid = $form_state['definition_nid'];
	$definition_node = node_load($definition_nid, NULL, TRUE);
	$info = field_info_instance('node', 'field_sirius_audio', $definition_node->type);
	$extensions = $info['settings']['file_extensions'];

	foreach ($form_state['values']['omg']['records'] as $delta => $omg_record) {
		if ($omg_record['audio'] == 'Yes') {
		  $file = file_save_upload("omg_records_{$delta}_audio_file", array('file_validate_extensions' => array($extensions)));
		  if ($file) {
		    $name = rand();
		    if ($file = file_move($file, 'private://attach_' . rand())) {
		      $form_state['storage']['omg']['records'][$delta]['audio_file'] = $file;
		    }
		    else {
		      form_set_error("omg][records][$delta][audio_file", t("Failed to write the uploaded file to the site's file folder."));
		    }
		  }
		}
  }
}

function sirius_node_tab_event_participant_definition_audio_form_submit($form, &$form_state) {
	$definition_nid = $form_state['definition_nid'];

	// I don't know why I have to reset the node cache. Something in field_view_value changes the values of field_audio. I'm not kidding.
	// I spent an hour trying to figure out what the heck was going on. I still have no idea, but coercing a hard reload here fixes the problem
	$definition_node = node_load($definition_nid, NULL, TRUE);
	$json = sirius_jsonfield_util_node_json_get($definition_node);
	$json['omg']['records'] = $form_state['values']['omg']['records'];

	// Trim any empty records from the end.
	while (count($json['omg']['records']) > 0) {
		$last = $json['omg']['records'][count($json['omg']['records']) - 1];
		if ($last['title'] || $last['audio'] == 'Yes' || $last['sms'] == 'Yes') { break; }
		array_pop($json['omg']['records']);
	}

	$json['omg']['pstatus'] = $form_state['values']['omg']['pstatus'];
	sirius_jsonfield_util_node_json_set($definition_node, $json);

	// We're assuming that the delta for the omg record is the same as the delta for the field_audio field. 
	// This is an ugly-ass kludge
	// Also, suppose records #0 and #2 have audio files, but record #1 doesn't. Since Drupal fields don't allow empty entries, we need to
	// generate a placeholder file for record #1. How ugly is that?
	foreach ($form_state['values']['omg']['records'] as $delta => $omg_record) {
		$file = $form_state['storage']['omg']['records'][$delta]['audio_file'];
		if ($file) { 
			$file = (array) $file;
			$file['display'] = 1;
			$file['description'] = $form_state['values']['omg']['records'][$delta]['title'];
			$file['status'] = 1;
			unset($file['destination']);
			unset($file['uri']);
			$definition_node->field_sirius_audio['und'][$delta] = $file;
		} else {
			if (!$definition_node->field_sirius_audio['und'][$delta]) {
				$file = file_save_data('NULL', 'private://attach_' . rand() . '.mp3');
				$file = (array) $file;
				$file['display'] = 1;
				$file['description'] = $form_state['values']['omg']['records'][$delta]['title'];
				$file['status'] = 1;
				unset($file['destination']);
				unset($file['uri']);
				$definition_node->field_sirius_audio['und'][$delta] = $file;
			}
		}
	}

  node_submit($definition_node);
  node_save($definition_node);
  drupal_set_message("Outgoing messages saved.");
}

function sirius_event_omg_list($definition_node) {
	$json = sirius_jsonfield_util_node_json_get($definition_node);
	$json_omg = $json['omg'];

	// Get the records.
	if ($json_omg) { 
		if (!$json_omg['records']) { return array(); }
		$omg_records = $json_omg['records'];
	} else {
		// Support for legacy data. If we have no OMG JSON, we generate it based on the contents of the audio field
		if (!$definition_node->field_sirius_audio['und']) { return array(); }
		$omg_records = array();
		foreach ($definition_node->field_sirius_audio['und'] as $delta => $audio_file) {
			$omg_records[] = array(
				'audio' => 'Yes',
				'title' => $audio_file['description'],
			);
		}
	}

	return $omg_records;
}

function sirius_event_omg_fetch_from_key($omg_key) {
	list($definition_nid, $delta) = explode('-', $omg_key);
	$definition_node = node_load($definition_nid);
	$omg_records = sirius_event_omg_list($definition_node);
	return $omg_records[$delta];
}