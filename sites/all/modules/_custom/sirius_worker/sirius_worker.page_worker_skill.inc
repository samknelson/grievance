<?php

function sirius_node_tab_worker_skill_access($worker_node) {
	return node_access('view', $worker_node);
}

function sirius_node_tab_worker_skill($worker_node) {
	return drupal_get_form('sirius_node_tab_worker_skill_form', $worker_node);
}

function sirius_node_tab_worker_skill_form($form, &$form_state, $worker_node) {
	$form_state['worker_node'] = $worker_node;

	drupal_set_title($worker_node->title . ': Skills');

	// Show dispatch staff all the DNCs, and let them pick the employer
	$html = '';

	if ($worker_node->field_sirius_skill['und']) {
		foreach ($worker_node->field_sirius_skill['und'] as $tmp) {
			$skill_tid = $tmp['tid'];
			$skill_term = taxonomy_term_load($skill_tid);
			$html .= '<li>' . $skill_term->name . '</li>';
		}
	}
	if ($html) {
		$html = $worker_node->title . ' currently has the following skills:<ul>' . $html . '</ul>';
	} else {
		$html = $worker_node->title . ' does not currently have any skills registered.';
	}
	$form['intro'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="sirius_form_intro">' . $html . '</div>',
	);


	if (user_access('sirius dispatch staff')) {
		$skill_options = sirius_taxonomy_load('sirius_skill', 'tid', 'name', TRUE);
		$form['skill_tid'] = array(
			'#type' => 'select',
			'#title' => 'Skill',
			'#options' => $skill_options,
			'#required' => TRUE,
		);

		$form['action'] = array(
			'#type' => 'select',
			'#title' => 'Action',
			'#required' => TRUE,
			'#options' => array(
				NULL => t('-- Select --'),
				TRUE => t('Add this skill'),
				FALSE => t('Remove this skill'),
			),
		);

		$form['reason'] = array(
			'#type' => 'textarea',
			'#title' => t('Reason'),
			'#required' => TRUE,
		);

		for ($delta=0; $delta<3; ++$delta) {
			$form["attach_$delta"] = array(
				'#type' => 'file',
				'#title' => t('Attachment #') . ($delta + 1),
			);
		}

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => 'Submit',
		);
	}

	$form += sirius_log_render_form($worker_node->nid, 'worker:skill');

	return $form;
}

function sirius_node_tab_worker_skill_form_validate($form, &$form_state) {
	$info = field_info_instance('node', 'field_sirius_attachments', 'sirius_log');
	$extensions = $info['settings']['file_extensions'];
	for ($delta = 0; $delta < 3; ++$delta) {
	  $file = file_save_upload("attach_$delta", array(
	    'file_validate_extensions' => array($extensions),
	  ));
	  if ($file) {
	    $name = rand();
	    if ($file = file_move($file, 'private://tmp/' . rand())) {
	      $form_state['storage']["attach_$delta"] = $file;
	    }
	    else {
	      form_set_error("attach_$delta", t("Failed to write the uploaded file to the site's file folder."));
	    }
	  }
	}
}

function sirius_node_tab_worker_skill_form_submit($form, &$form_state) {
	$files = array();
	for ($delta = 0; $delta < 3; ++$delta) {
	  $file = $form_state['storage']["attach_$delta"];
	  if ($file) {
  		$file = (array) $file;
  		$file['display'] = 1;
			unset($form_state['storage']["attach_$delta"]);
  		unset($file['destination']);
  		unset($file['uri']);
  		$files[] = $file;
  	}
  }

	$worker_node = $form_state['worker_node'];
	$skill_tid = $form_state['values']['skill_tid'];
	$skill_term = taxonomy_term_load($skill_tid);

	sirius_worker_skill_set(
		$worker_node, 
		$skill_term, 
		$form_state['values']['action'], 
		$form_state['values']['reason'],
		$files
	);

	if ($form_state['values']['action']) {
		drupal_set_message(t("Success! Worker @worker has added the skill @skill.", array('@worker' => $worker_node->title, '@skill' => $skill_term->name)));
	} else {
		drupal_set_message(t("Success! Worker @worker has removed the skill @skill.", array('@worker' => $worker_node->title, '@skill' => $skill_term->name)));
	}
}