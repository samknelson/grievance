<?php

function sirius_node_tab_event_participant_definition_audio($definition_node) {
	return drupal_get_form('sirius_node_tab_event_participant_definition_audio_form', $definition_node->nid);
}

function sirius_node_tab_event_participant_definition_audio_form($form, &$form_state, $definition_nid) {
	$form_state['definition_nid'] = $definition_nid;
	$definition_node = node_load($definition_nid);

	$form['intro'] = array('#markup' => '<div class="sirius_form_intro">' . t('Use the form below to upload a file in wav or mp3 format. For phone banks, this will be available as the outgoing message. You may want to use a service such as <a href="https://online-voice-recorder.com/" target="_blank">online-voice-recorder.com</a>.') . '<br /><br /></div>');

	if ($definition_node->field_sirius_audio['und'][0]) {
		foreach ($definition_node->field_sirius_audio['und'] as $delta => $item) {
			$renderable = field_view_value('node', $definition_node, 'field_sirius_audio', $item, 'full');

			$form["item_$delta"] = array(
				'#type' => 'select',
				'#options' => array(
					NULL => t('Keep'), 
					'delete' => t('Delete')
				), 
				'#prefix' => '<H2>Audio File #' . ($delta+1) . ': ' . $item['description'] . '</h2>' . render($renderable),
			);
		}
	} else {
		$title = "Upload";
	}

	$max = 2;
	if (isset($definition_node->field_sirius_callerids['und'])) {
		$max = count($definition_node->field_sirius_callerids['und']) + 2;
	} else {
		$max = 2;
	}

	$form['file'] = array(
		'#type' => 'file',
		'#title' => t('File'),
		'#prefix' => '<H2>Upload New File</h2>',
	);

	$form['description'] = array(
		'#type' => 'textfield',
		'#title' => t('Title'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_event_participant_definition_audio_form_validate($form, &$form_state) {
	$definition_nid = $form_state['definition_nid'];
	$definition_node = node_load($definition_nid);
	$info = field_info_instance('node', 'field_sirius_audio', $definition_node->type);
	$extensions = $info['settings']['file_extensions'];
  $file = file_save_upload("file", array(
    'file_validate_extensions' => array($extensions),
  ));
  if ($file) {
    $name = rand();
    if ($file = file_move($file, 'private://attach_' . rand())) {
      $form_state['storage']["file"] = $file;
    }
    else {
      form_set_error("file", t("Failed to write the uploaded file to the site's file folder."));
    }
  }

  //if (!$file) {
  //	form_set_error("file", t("Please upload a file."));
  //}
}

function sirius_node_tab_event_participant_definition_audio_form_submit($form, &$form_state) {
	$definition_nid = $form_state['definition_nid'];
	$definition_node = node_load($definition_nid);
	$info = $form_state['info'];
	$data = $form_state['data'];

	// Save the data for this step
	$new_files = array();
	if ($definition_node->field_sirius_audio['und']) {
		foreach ($definition_node->field_sirius_audio['und'] as $delta => $item) {
			if ($form_state['values']["item_$delta"] != 'delete') {
				$new_files[] = $item;
			}
		}
		$definition_node->field_sirius_audio['und'] = $new_files;
	}

  $file = $form_state['storage']["file"];
  if ($file) {
		$file = (array) $file;
		$file['display'] = 1;
		$file['description'] = $form_state['values']['description'];
		unset($form_state['storage']["attach_$delta"]);
		unset($file['destination']);
		unset($file['uri']);
		$definition_node->field_sirius_audio['und'][] = $file;
  }
  node_submit($definition_node);
  node_save($definition_node);
}
