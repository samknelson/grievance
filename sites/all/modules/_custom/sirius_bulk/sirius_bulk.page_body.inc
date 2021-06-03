<?php

function sirius_bulk_node_tab_body($bulk_node) {
	return drupal_get_form('sirius_bulk_node_tab_body_form', $bulk_node);
}

function sirius_bulk_node_tab_body_form($form, &$form_state, $bulk_node) {
	$form_state['bulk_nid'] = $bulk_node->nid;
	$media = sirius_bulk_media();
	$medium = $bulk_node->field_sirius_bulk_medium['und'][0]['value'];

	if (in_array($medium, array('voice', 'choice'))) {
		$form['intro'] = array('#markup' => '<div class="sirius_form_intro">' . t('The voice message may be entered either as <ul><li>A text string. In this case, the system will use text-to-speech to synthesize the voice call</li><li>An uploaded recording in WAV or MP3 format. You may want to use a service such as <a href="https://online-voice-recorder.com/" target="_blank">online-voice-recorder.com</a></li></ul>') . '</div>');

		$form['voice'] = array(
			'#title' => t('Voice Body (Text to Speech)'),
			'#type' => 'textarea',
			'#default_value' => $bulk_node->field_sirius_voice['und'][0]['value'],
		);


		if ($bulk_node->field_sirius_audio_public['und'][0]) {
			$renderable = field_view_value('node', $bulk_node, 'field_sirius_audio_public', $bulk_node->field_sirius_audio_public['und'][0], 'full');
			$prefix = 'Current file:' . render($renderable);
			$title = t('Voice Body (Replace With)');
		} else {
			$title = t("Voice Body (Upload)");
		}

		$max = 1;
		if (isset($bulk_node->field_sirius_callerids['und'])) {
			$max = count($bulk_node->field_sirius_callerids['und']) + 2;
		} else {
			$max = 1;
		}

		$form['file'] = array(
			'#type' => 'file',
			'#title' => $title,
			'#prefix' => $prefix,
		);


		$intro_html = '';
		$intro_html .= t('Use the field below to select the caller ID that will be used for the outgoing voice call.');
		if (drupal_valid_path('sirius/config/phone/callerid')) {
			$intro_html .= '<br />' . l('Manage Caller IDs', 'sirius/config/phone/callerid', array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
		}
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
			'#prefix' => $intro_html,
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
		'#required' => TRUE,
	);

	return $form;
}

function sirius_bulk_node_tab_body_form_validate($form, &$form_state) {
	$definition_nid = $form_state['definition_nid'];
	$definition_node = node_load($definition_nid);
	$info = field_info_instance('node', 'field_sirius_audio_public', 'sirius_bulk');
	$extensions = $info['settings']['file_extensions'];
  $file = file_save_upload("file", array(
    'file_validate_extensions' => array($extensions),
  ));
  if ($file) {
    $name = rand();
    if ($file = file_move($file, 'public://attach_' . rand())) {
      $form_state['storage']["file"] = $file;
    }
    else {
      form_set_error("file", t("Failed to write the uploaded file to the site's file folder."));
    }
  }
}


function sirius_bulk_node_tab_body_form_submit($form, &$form_state) {
	$bulk_node = node_load($form_state['bulk_nid']);

	$bulk_node->title = $form_state['values']['title'];
	$bulk_node->body['und'][0]['value'] = $form_state['values']['body']['value'];
	$bulk_node->body['und'][0]['text_format'] = $form_state['values']['body']['text_format'];
	$bulk_node->field_sirius_sms['und'][0]['value'] = $form_state['values']['sms'];
	$bulk_node->field_sirius_voice['und'][0]['value'] = $form_state['values']['voice'];
	if ($form_state['values']['callerid_nid']) {
		$bulk_node->field_sirius_callerids['und'][0]['target_id'] = $form_state['values']['callerid_nid'];
	} else {
		unset($bulk_node->field_sirius_callerids['und']);
	}

  $file = $form_state['storage']["file"];
  if ($file) {
		$file = (array) $file;
		$file['display'] = 1;
		$file['description'] = $form_state['values']['description'];
		unset($form_state['storage']["attach_$delta"]);
		unset($file['destination']);
		unset($file['uri']);
		$bulk_node->field_sirius_audio_public['und'][0] = $file;
  }
  node_submit($bulk_node);
	node_save($bulk_node);

	drupal_set_message("Message body saved.");
}