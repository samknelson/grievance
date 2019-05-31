<?php

function sirius_node_tab_worker_skill_access($worker_node) {
	return node_access('view', $worker_node);
}

function sirius_node_tab_worker_skill($worker_node) {
	return drupal_get_form('sirius_node_tab_worker_skill_form', $worker_node);
}

function sirius_node_tab_worker_skill_form($form, &$form_state, $worker_node) {
	$form_state['worker_node'] = $worker_node;

	// Show dispatch staff all the DNCs, and let them pick the employer
	$html = '';

	if (!user_access('sirius dispatch staff')) {
		if ($worker_node->field_sirius_skill['und']) {
			foreach ($worker_node->field_sirius_skill['und'] as $delta => $tmp) {
				$skill_tid = $tmp['tid'];
				$skill_term = taxonomy_term_load($skill_tid);

				$html .= '<li>' . $skill_term->name;

				$expires_ts = strtotime($worker_node->field_sirius_skill_expire['und'][$delta]['value']);
				if (!$expires_ts || $expires_ts > time() + 60*60*24*365*50) {
					$html .= ' (expires: ' . date('m/d/Y', $expires_ts) . ')';
				}

				$html .= '</li>';
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
	}


	if (user_access('sirius dispatch staff')) {
	  $vocabulary = taxonomy_vocabulary_machine_name_load('sirius_skill');
	  $terms_nohash = taxonomy_get_tree($vocabulary->vid, 0, NULL, TRUE);
	  $count = 0;
	  foreach ($terms_nohash as $term) { $terms[$term->tid] = $term; }

	  $form['col1'] = array(
	  	'#type' => 'fieldset',
	  	'#prefix' => '<div style="width: 30%; float: left; padding-right: 3%;">',
	  	'#suffix' => '</div>',
	  );
	  $form['col2'] = array(
	  	'#type' => 'fieldset',
	  	'#prefix' => '<div style="width: 30%; float: left; padding-right: 3%;">',
	  	'#suffix' => '</div>',
	  );
	  $form['col3'] = array(
	  	'#type' => 'fieldset',
	  	'#prefix' => '<div style="width: 30%; float: left;">',
	  	'#suffix' => '</div>',
	  );

	  $root_count = 0;
	  foreach ($terms as $tid => $term) { 
	  	if (!$term->depth) { ++$root_count; }
	  }
	  $count = 0;
	  foreach ($terms as $tid => $term) {
	  	if (!$term->depth) { ++$count; }
	  	if ($count <= $root_count / 3 + 1) { $terms[$tid]->col = 'col1'; } 
	  	else if ($count <= 2 * ($root_count / 3) + 1) { $terms[$tid]->col = 'col2'; } 
	  	else { $terms[$tid]->col = 'col3'; }
	  }

	  foreach ($terms as $term) {
	    $tid = $term->tid;
	    $name = $term->name;
	    $depth = $term->depth;
	    $parent = $term->parents[0];
	    $days = $term->field_grievance_days['und'][0]['value'];
	    $expiration = '';

	    $field_name = "skill_checkbox_$tid";

	    $field_prefix = '';
	    for ($i=0; $i<$depth; ++$i) {
	      $field_prefix .= '&nbsp;--';
	    }

	    $states = NULL;
	    if ($parent) {
	    	$parent_field_name = "skill_checkbox_$parent";
	      $states = array(
	        'visible' => array(
	          array(':input[name="' . $parent_field_name . '"]' => array('checked' => TRUE)),
	        )
	      );
	    }

	    $form[$term->col][$field_name] = array(
	      '#type' => 'checkbox',
	      '#title' => $name,
	      '#field_prefix' => $field_prefix,
	      '#states' => $states,
	    );

	    if ($days) {
	    	$form[$term->col]["skill_expires_$tid"] = array(
	    		'#type' => 'textfield',
	    		'#default_value' => gmdate('m/d/Y', strtotime("+$days days")),
	    		'#size' => 20,
	    		'#states' => array(
		        'visible' => array(
		          array(':input[name="' . $field_name . '"]' => array('checked' => TRUE)),
		        )
		      ),
	    	);
	    }

	  }

		if ($worker_node->field_sirius_skill['und']) {
			foreach ($worker_node->field_sirius_skill['und'] as $delta => $tmp) {
				$tid = $tmp['tid'];
		    $field_name = "skill_checkbox_$tid";
				$form[$terms[$tid]->col]["skill_checkbox_$tid"]['#default_value'] = TRUE;
				$expires_ts = strtotime($worker_node->field_sirius_skill_expire['und'][$delta]['value']);
				if (!$expires_ts || $expires_ts < time() + 60*60*24*365*50) {
					$form[$terms[$tid]->col]["skill_expires_$tid"]['#default_value'] = gmdate('m/d/Y', $expires_ts);
				}
			}
		}

		$form['reason'] = array(
			'#type' => 'textarea',
			'#title' => t('Reason'),
			'#required' => TRUE,
			'#prefix' => '<div style="clear: left;">',
			'#suffix' => '</div>',
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
	    if ($file = file_move($file, 'private://attach_' . rand())) {
	      $form_state['storage']["attach_$delta"] = $file;
	    }
	    else {
	      form_set_error("attach_$delta", t("Failed to write the uploaded file to the site's file folder."));
	    }
	  }
	}
}

function sirius_node_tab_worker_skill_form_submit($form, &$form_state) {
	$worker_node = $form_state['worker_node'];

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

  $new_vals = array();
  foreach ($form_state['values'] as $name => $value) {
  	if ($value != 1) { continue; }
  	if (!preg_match('/^skill_checkbox_(\d+)$/', $name, $matches)) { continue; }
  	$tid = $matches[1];
  	$expires = $form_state['values']["skill_expires_$tid"];
  	$new_vals[$tid] = $expires;
  }

  $result = sirius_worker_skill_set_all($worker_node, $new_vals, $reason, $files);
	drupal_set_message($result['msg']);
}