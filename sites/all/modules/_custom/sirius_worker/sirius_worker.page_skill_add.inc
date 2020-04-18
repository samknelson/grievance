<?php

function sirius_node_tab_worker_skill_add($worker_node) {
	return drupal_get_form('sirius_node_tab_worker_skill_add_form', $worker_node);
}

function sirius_node_tab_worker_skill_add_form($form, &$form_state, $worker_node) {
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

	if ($worker_node->field_sirius_skill['und']) {
		foreach ($worker_node->field_sirius_skill['und'] as $tmp) {
			unset($skill_options[$tmp['tid']]);
		}
	}

	$form['b1'] = array('#markup' => '<h2>' . t('Add New Skills') . '</h2>');
	$form['b2'] = array('#markup' => '<table>');
	$form['b2x'] = array('#markup' => '<tr><th>Skill</th><th>Expiration Date</th></tr>');
	for ($delta = 1; $delta < 10; ++$delta) {
		$form[$delta]['tr_open'] = array('#markup' => '<tr>');

		$form[$delta]['skill_a'] = array('#markup' => '<td>');
		$form[$delta]['skill'] = array(
			'#type' => 'select',
			'#options' => $skill_options,
	    '#ajax' => array('callback' => 'sirius_node_tab_worker_skill_add_form_ajax'),
		);
		$form[$delta]['skill_b'] = array('#markup' => '</td>');

		$form[$delta]['renew_a'] = array('#markup' => '<td width="50%">');

		$form[$delta]['renew'] = array(
			'#type' => 'textfield',
			'#prefix' => '<div id="renew_' . $delta . '">',
			'#suffix' => '</div>',
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

function sirius_node_tab_worker_skill_add_form_ajax($form, $form_state) {
  $commands = array();
	if ($form_state['triggering_element']) {
		$delta = $form_state['triggering_element']['#name'];
		$delta = preg_replace('/\[.+/', '', $delta);
	  $commands[] = ajax_command_replace("#renew_" . $delta, render($form[$delta]['renew']));
	}

  return array(
    '#type' => 'ajax',
    '#commands' => $commands
  );
}

function sirius_node_tab_worker_skill_add_form_validate($form, &$form_state) {
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

function sirius_node_tab_worker_skill_add_form_submit($form, &$form_state) {
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

  $delta = 1;
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

  drupal_goto('node/' . $worker_node->nid . '/sirius_worker_skill/view');
}
