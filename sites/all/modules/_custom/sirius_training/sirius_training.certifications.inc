<?php

function sirius_training_certifications_fetch_by_worker($worker_nid, $type = NULL) {
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->fieldCondition('field_sirius_log_handler', 'target_id', $worker_nid);
  $query->fieldCondition('field_sirius_category', 'value', 'worker:certification');
  if ($type) { $query->fieldCondition('field_sirius_type', 'value', $type); }
  $domain_nid = sirius_domain_nid();
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  $query->propertyOrderBy('created', 'desc');
  $stmt = $query->execute();
  if (!$stmt['node']) { return array(); }
  return node_load_multiple(array_keys($stmt['node']));
}

function sirius_training_certifications_fetch_by_participant($participant_nid, $type = NULL) {
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->fieldCondition('field_sirius_category', 'value', 'worker:certification');
  $query->fieldCondition('field_sirius_log_handler', 'target_id', $participant_nid);
  if ($type) { $query->fieldCondition('field_sirius_type', 'value', $type); }
  $domain_nid = sirius_domain_nid();
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  $stmt = $query->execute();
  $query->propertyOrderBy('created');
  if (!$stmt['node']) { return array(); }
  return node_load_multiple(array_keys($stmt['node']));
}

function sirius_training_certifications_enabled() {
	return sirius_domain_variable_get('sirius_training_certifications_enabled', '') == 'Yes';
}

function sirius_training_certification_definitions() {
  $type = 'sirius_training:certification';
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_json_definition');
  $query->fieldCondition('field_sirius_type', 'value', $type);
  $domain_nid = sirius_domain_nid();
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  $query->propertyOrderBy('title');
  $stmt = $query->execute();
  if (!$stmt['node']) { return array(); }
  return node_load_multiple(array_keys($stmt['node']));
}

function sirius_training_certifications_render($certification_node, $options = array()) {
	sirius_esignature_init();
	drupal_add_css('sites/all/modules/_custom/sirius_training/css/certification.css');
	$handler_nodes = node_load_multiple(sirius_fieldvals($certification_node, 'field_sirius_log_handler', 'target_id'));
	foreach ($handler_nodes as $handler_nid => $handler_node) {
		if ($handler_node->type == 'sirius_worker') {
			$worker_nid = $handler_nid;
			$worker_node = $handler_node;
		}
	}

	$author_user = user_load($certification_node->uid);
	if ($author_user->field_sirius_worker['und'][0]['target_id']) {
		$author_worker_node = node_load($author_user->field_sirius_worker['und'][0]['target_id']);
		$author_name = $author_worker_node->title;
	} else {
		$author_name = $author_user->name;
	}

	$status_options = sirius_training_certification_status_options();
	$status = $certification_node->field_sirius_type['und'][0]['value'];
	$status_name = $status_options[$status];
	$html .= "<div class=\"sirius_training_certification sirius_training_certification_{$status}\">";

	foreach ($handler_nodes as $handler_nid => $handler_node) {
		if ($handler_node->type == 'sirius_json_definition') {
			$html .= '<h2 class="sirius_training_certification_title">' . $handler_node->title . '</h2>';
		}
	}

	$html .= sirius_fakerender_field('Status:', $status_name);
	if (!$options['hide_worker']) {
		$html .= sirius_fakerender_field('Worker:', $worker_node->title . ' / ' . l($worker_node->field_sirius_id['und'][0]['value'], "node/$worker_nid"));
	}
	$html .= sirius_fakerender_field('Submitted On:', date('l, F d g:i A', $certification_node->created));
	$html .= sirius_fakerender_field('Submitted By:', $author_name);

	$json = sirius_jsonfield_util_node_json_get($certification_node);
	$form = sirius_jsonfield_util_form_get($json, 'certification');
	$results = sirius_jsonfield_util_results_get($json, 'certification');
	$html .= sirius_jsonfield_util_render_all_values($form, $results);

	$signature_definitions = $form['#sirius']['signature_definitions'];
	if ($signature_definitions) {
		$html .= '<hr>' . '<h2>' . t('Signatures') . '</h2>';
		foreach ($signature_definitions as $signature_definition) {
			$name = $signature_definition['name'];
			$esignature = sirius_jsonfield_util_esignature_get($json, $name);
			$html .= sirius_fakerender_field($name, sirius_esignature_render($esignature, '', $certification_node->nid, $name));
		}
	}

	if (!$options['hide_edit_link']) {
		$path = 'node/' . $certification_node->nid . '/sirius_log_tc/certification_edit';
		if (drupal_valid_path($path))
		$html .= l('Edit', $path, array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => current_path())));
	}
	$html .= '</div>';

	return $html;
}

function sirius_training_certification_status_options() {
	return array(
		'draft' => t('Draft'),
		'signed' => t('Signed'),
		'granted' => t('Granted'),
		'revoked' => t('Revoked'),
		'discarded' => t('Discarded'),
	);
}


function sirius_training_certification_grant($certification_node, $values = array()) {
	$certification_json = sirius_jsonfield_util_node_json_get($certification_node);
	$certification_form = sirius_jsonfield_util_form_get($certification_json, 'certification');
	$handler_nodes = node_load_multiple(sirius_fieldvals($certification_node, 'field_sirius_log_handler', 'target_id'));
	foreach ($handler_nodes as $handler_nid => $handler_node) {
		if ($handler_node->type == 'sirius_worker') 									{ $worker_node = $handler_node;	} 
		else if ($handler_node->type == 'sirius_event_participant') 	{ $participant_node = $handler_node; }
		else if ($handler_node->type == 'sirius_json_definition') 		{ $definition_node = $handler_node; }
	}
	if (!$worker_node) { return array('success' => FALSE, 'msg' => 'The certification node is not associated with a worker.'); }
	if (!$definition_node) { return array('success' => FALSE, 'msg' => 'The certification node is not associated with a definition or form.'); }
	$definition_json = sirius_jsonfield_util_node_json_get($definition_node);

	$title = $worker_node->title . ' / ' . $definition_node->title;

	// Set the status to "Granted"
	$certification_status = $values['certification_status'];
	if (!$certification_status) { $certification_status = 'granted'; }
	if ($certification_node->field_sirius_type['und'][0]['value'] == $certification_status) {
		sirius_minilog("$title: Certification already had status $certification_status.");
	} else {
		$certification_node->field_sirius_type['und'][0]['value'] = $certification_status;
		node_save($certification_node);
		sirius_minilog("$title: Set certification status to \"$certification_status\".");
	}

	// Pass the worker
	if ($participant_node) {
		$event_node = node_load($participant_node->field_sirius_event['und'][0]['target_id']);
		$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
		if (!$event_type_term) {
			sirius_minilog("$title: Unable to find the event type for the participant.");
		} else {
			$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);
			$pstatus_tid = $values['participant_status'];
			if (!$pstatus_tid) { $pstatus_tid = $event_type_json['class']['certifications']['pstatus_passed']; }
			if (!$pstatus_tid) { 
				sirius_minilog("$title: Don't know which status to set the participant. Please check the configuration for the event type " . $event_type_term->name);
			} else {
				$pstatus_term = taxonomy_term_load($pstatus_tid);
				if ($participant_node->field_sirius_event_pstatus['und'][0]['tid'] == $pstatus_tid) {
					sirius_minilog("$title: Participant already had status " . $pstatus_term->name);
				} else {
					$participant_node->field_sirius_event_pstatus['und'][0]['tid'] = $pstatus_tid;
					node_save($participant_node);
					sirius_minilog("$title: Set participant status to \"" . $pstatus_term->name . "\"");
				}
			}
		}
	}

	// Grant the skills
	if (!$definition_json['certification']['skills']) {
		sirius_minilog("$title: The certification is not associated with any skills.");
	} else {
		$skill_tids = array_keys($definition_json['certification']['skills']);
		$skill_terms = taxonomy_term_load_multiple($skill_tids);
		foreach ($skill_terms as $skill_tid => $skill_term) {
			if ($values['skills'][$skill_tid] == 'skip') {
				sirius_minilog("$title: Skipping skill " . $skill_term->name); 
			} else {
				$days = $skill_term->field_grievance_days['und'][0]['value'];
				if ($days) { 
					$expires = gmdate('m/d/Y', strtotime("+$days days"));
					$expires_display = $expires;
				} else { 
					$expires = NULL;
					$expires_display = '(never)';
				}
				$reason = "Granted certification: " . $definition_node->title;
				$result = sirius_worker_skill_set($worker_node, array($skill_tid), $expires, 'add', $reason, array());
				if ($result['success']) {
					sirius_minilog("$title: " . $result['msg']); 
				} else {
					sirius_minilog("$title: " . $result['msg'], 'warning'); 
				}
			}
		}
	}


	return array('success' => TRUE, 'msg' => "$title: Granted");
}