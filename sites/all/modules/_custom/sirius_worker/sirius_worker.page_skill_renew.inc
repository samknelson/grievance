<?php

function sirius_node_tab_worker_skill_renew($worker_node) {
	return drupal_get_form('sirius_node_tab_worker_skill_renew_form', $worker_node);
}

function sirius_node_tab_worker_skill_renew_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	$form['#tree'] = TRUE;

	$skill_terms = sirius_taxonomy_load('sirius_skill', 'tid', 'full');
	$skill_options = sirius_taxonomy_load('sirius_skill', 'tid', 'name', TRUE);
	$skill_days = array();
	$skill_expiration_defaults = array();
	foreach ($skill_terms as $skill_tid => $skill_term) {
		$days = $skill_term->field_grievance_days['und'][0]['value'];
		if ($days) {
			$skill_days[$skill_tid] = $days;
			$skill_expiration_defaults[$skill_tid] = gmdate('m/d/Y', strtotime("+$days days"));
		}
	}

	$rows = array();
	$header = array(t('Skill'), t('Expires'));

	$empty = TRUE;
	if ($worker_node->field_sirius_skill['und']) {
		$form['a1'] = array('#markup' => '<h2>' . t('Renew Current Skills') . '</h2>');
		$form['a2'] = array('#markup' => '<table>');
		foreach ($worker_node->field_sirius_skill['und'] as $delta => $tmp) {
			$skill_tid = $tmp['tid'];
			$skill_term = $skill_terms[$skill_tid];
	    $days = $skill_days[$skill_tid];
	    if (!$days) { continue; }
	    $empty = FALSE; 

			$form[$delta]['tr_open'] = array('#markup' => '<tr>');

			$form[$delta]['name'] = array('#markup' => '<td width="40%">' . $skill_term->name . '</td>');

			$form[$delta]['checkbox_a'] = array('#markup' => '<td width="10%">');
			$form[$delta]['checkbox'] = array(
				'#type' => 'checkbox',
				'#title' => 'Renew',
				'#id' => 'edit_' . $delta,
			);
			$form[$delta]['checkbox_b'] = array('#markup' => '</td>');
			$form[$delta]['skill'] = array(
				'#type' => 'value',
				'#value' => $skill_tid,
			);

			$form[$delta]['renew_a'] = array('#markup' => '<td width="10%">' . t('Renew until:') . '</td><td width="40%">');
			if ($days) {
				$form[$delta]['renew'] = array(
					'#type' => 'textfield',
					'#default_value' => $skill_expiration_defaults[$skill_tid],
					'#states' => array(
		        'visible' => array(
		          ':input[id="edit_' . $delta . '"]' => array('checked' => TRUE),
		        )
					),
				);
			}
			$form[$delta]['renew_b'] = array('#markup' => '</td>');

			$form[$delta]['tr_close'] = array('#markup' => '</tr>');
		}
		$form['a3'] = array('#markup' => '</table>');
	}

	if ($empty) {
		return array('empty' => array('#markup' => t('This worker does not have any expiring skills.')));
	}

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

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}


function sirius_node_tab_worker_skill_renew_form_validate($form, &$form_state) {
	$info = field_info_instance('node', 'field_sirius_attachments', 'sirius_log');
	$extensions = $info['settings']['file_extensions'];
	for ($delta = 0; $delta < 3; ++$delta) {
	  $file = file_save_upload("attach_$delta", array(
	    'file_validate_extensions' => array($extensions),
	  ));
	  if ($file) {
	    $name = rand();
	    if ($file = file_move($file, 'private://attach_' . rand())) {
	      $form_state['storage']["attach_$delta"] = $file;
	    }
	    else {
	      form_set_error("attach_$delta", t("Failed to write the uploaded file to the site's file folder."));
	    }
	  }
	}
}

function sirius_node_tab_worker_skill_renew_form_submit($form, &$form_state) {
	$worker_node = node_load($form_state['worker_nid']);

	$files = array();
	for ($delta = 0; $delta < 3; ++$delta) {
	  $file = $form_state['storage']["attach_$delta"];
	  if ($file) {
  		$file = (array) $file;
  		$file['display'] = 1;
			unset($form_state['storage']["attach_$delta"]);
  		unset($file['destination']);
  		unset($file['uri']);
  		$file['status'] = FILE_STATUS_PERMANENT;
  		$files[] = $file;
  	}
  }

  $reason = $form_state['values']['reason'];

  for ($delta = 0; $delta < 1000; ++$delta) {
  	if (!$form_state['values'][$delta]) { continue; }
  	if (!$form_state['values'][$delta]['checkbox']) { continue; }
  	$tid = $form_state['values'][$delta]['skill'];
  	$expires = $form_state['values'][$delta]['renew'];
  	if (sirius_strtotime($expires) < 1) { $expires = NULL; }
		$result = sirius_worker_skill_set($worker_node, array($tid), $expires, 'add', $reason, $files);
		if ($result['success']) {
			drupal_set_message($result['msg']);
		} else {
			drupal_set_message($result['msg'], 'warning');
		}
  }

	drupal_goto('node/' . $worker_node->nid . '/sirius_worker_skill/view');
}