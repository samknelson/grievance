<?php

function sirius_bulk_node_tab_body_voice_access($bulk_node) {
	$medium = $bulk_node->field_sirius_bulk_medium['und'][0]['value'];
	return in_array($medium, array('voice', 'choice'));
}

function sirius_bulk_node_tab_body_voice($bulk_node) {
	return drupal_get_form('sirius_bulk_node_tab_body_voice_form', $bulk_node);
}


function sirius_bulk_node_tab_body_voice_form($form, &$form_state, $bulk_node) {
	$form_state['bulk_nid'] = $bulk_node->nid;
	$json = sirius_json_get($bulk_node);

	$form['#tree'] = TRUE;

	$intro_html = '';
	$intro_html .= t('Use the field below to select the caller ID that will be used for the outgoing voice call.');
	$callerid_nodes = sirius_callerid_list();
	$callerid_options = array(NULL => t('-- Select --'));
	foreach ($callerid_nodes as $callerid_nid => $callerid_node) {
		if ($callerid_node->field_sirius_active['und'][0]['value'] == 'Yes') {
			$callerid_options[$callerid_nid] = $callerid_node->title . ' (' . $callerid_node->field_sirius_phone['und'][0]['value'] . ')';
		}
	}

	$form["callerid_nid"] = array(
		'#type' => 'select',
		'#options' => $callerid_options,
		'#title' => t('Caller ID'),
		'#default_value' => $bulk_node->field_sirius_callerids['und'][0]['target_id'],
	);

	$form['voice']['type'] = array(
		'#type' => 'select',
		'#title' => 'Message Type',
		'#options' => array(
			NULL => t('-- Select --'),
			'file' => t('Audio File'),
			'text' => t('Text-to-Speech'),
			'flow' => t('Twilio Flow'),
		),
		'#default_value' => $json['voice']['type'],
		'#id' => 'voice_type',
	);

	$form['voice']['flow']['sid'] = array(
		'#type' => 'textfield',
		'#title' => 'Twilio Flow SID',
		'#default_value' => $json['voice']['flow']['sid'],
		'#states' => array('visible' => array(":input[id=\"voice_type\"]" => array('value' => 'flow'))),
		'#description' => t('A valid Twilio Flow SID starting with "FW", e.g. FW...')
	);


	$form['voice']['text']['human'] = array(
		'#type' => 'textarea',
		'#title' => 'Text-to-Speech if a human answers',
		'#default_value' => $json['voice']['text']['human'],
		'#states' => array('visible' => array(":input[id=\"voice_type\"]" => array('value' => 'text'))),
	);

	$form['voice']['text']['machine'] = array(
		'#type' => 'textarea',
		'#title' => 'Text-to-Speech if an answering machine answers',
		'#default_value' => $json['voice']['text']['machine'],
		'#states' => array('visible' => array(":input[id=\"voice_type\"]" => array('value' => 'text'))),
	);


	$form['voice']['file']['human'] = array(
		'#type' => 'fieldset',
		'#title' => 'Audio to play if a human answers',
		'#states' => array('visible' => array(":input[id=\"voice_type\"]" => array('value' => 'file'))),
	);
	if ($bulk_node->field_sirius_audio_public['und'][0]) {
		$renderable = field_view_value('node', $bulk_node, 'field_sirius_audio_public', $bulk_node->field_sirius_audio_public['und'][0], 'full');
		$prefix = 'Current file:' . render($renderable);
		$title = t('Voice Body (Replace With)');
	} else {
		$prefix = '';
		$title = t("Voice Body (Upload)");
	}
	$form['voice']['file']['human']['file'] = array(
		'#type' => 'file',
		'#title' => $title,
		'#prefix' => $prefix,
		'#name' => 'files[audio_human]',
	);

	$form['voice']['file']['machine'] = array(
		'#type' => 'fieldset',
		'#title' => 'Audio to play if an answering machine answers',
		'#states' => array('visible' => array(":input[id=\"voice_type\"]" => array('value' => 'file'))),
	);
	if ($bulk_node->field_sirius_audio_public['und'][1]) {
		$renderable = field_view_value('node', $bulk_node, 'field_sirius_audio_public', $bulk_node->field_sirius_audio_public['und'][1], 'full');
		$prefix = 'Current file:' . render($renderable);
		$title = t('Voice Body (Replace With)');
	} else {
		$prefix = '';
		$title = t("Voice Body (Upload)");
	}
	$form['voice']['file']['machine']['file'] = array(
		'#type' => 'file',
		'#title' => $title,
		'#prefix' => $prefix,
		'#name' => 'files[audio_machine]',
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
		'#required' => TRUE,
	);

	return $form;
}

function sirius_bulk_node_tab_body_voice_form_validate($form, &$form_state) {
	$definition_nid = $form_state['definition_nid'];
	$definition_node = node_load($definition_nid);
	$info = field_info_instance('node', 'field_sirius_audio_public', 'sirius_bulk');
	$extensions = $info['settings']['file_extensions'];

	foreach (array('audio_human', 'audio_machine') as $key) {
	  $file = file_save_upload($key, array(
	    'file_validate_extensions' => array($extensions),
	  ));
	  if ($file) {
	    $name = rand();
	    if ($file = file_move($file, 'public://attach_' . rand())) {
	      $form_state['storage'][$key] = $file;
	    }
	    else {
	      form_set_error("file", t("Failed to write the uploaded file to the site's file folder."));
	    }
	  }
	}
}

function sirius_bulk_node_tab_body_voice_form_submit($form, &$form_state) {
	$bulk_node = node_load($form_state['bulk_nid']);
	$json = sirius_json_get($bulk_node);
	$json['voice'] = $form_state['values']['voice'];
	sirius_json_set($bulk_node, $json);

	if ($form_state['values']['callerid_nid']) {
		$bulk_node->field_sirius_callerids['und'][0]['target_id'] = $form_state['values']['callerid_nid'];
	} else {
		unset($bulk_node->field_sirius_callerids['und']);
	}

	foreach (array('audio_human', 'audio_machine') as $delta => $key) {
	  $file = $form_state['storage'][$key];
	  if ($file) {
			$file = (array) $file;
			$file['display'] = 1;
			$file['description'] = $form_state['values']['description'];
			unset($form_state['storage']["attach_$delta"]);
			unset($file['destination']);
			unset($file['uri']);
			$bulk_node->field_sirius_audio_public['und'][$delta] = $file;
	  }
	}

  node_submit($bulk_node);
	node_save($bulk_node);
	drupal_set_message("Voice message saved.");

}