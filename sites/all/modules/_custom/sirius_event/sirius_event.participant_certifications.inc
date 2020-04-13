<?php

function sirius_event_page_participant_certifications($event_node, $rule) {
	sirius_esignature_init();

	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);
	if (!$event_type_json['class']['certifications']['available']) { return 'No certifications have been configured for this event type.'; }
	$certification_definition_nodes = node_load_multiple(array_values($event_type_json['class']['certifications']['available']));
	if (!$certification_definition_nodes) { return 'No certifications found for this event type.'; }

	// Get the participants
	$sql_args = array();
	$sql_args[':event_nid'] = $event_node->nid;
	$sql = "select node.nid as participant_nid ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_event_prole on field_data_field_sirius_event_prole.entity_type = 'node' and field_data_field_sirius_event_prole.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_event_pstatus on field_data_field_sirius_event_pstatus.entity_type = 'node' and field_data_field_sirius_event_pstatus.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_event_participant' ";
	$sql .= "and field_sirius_event_target_id = :event_nid ";
	$sql .= "and ( ";
	$delta = 0;
	foreach ($rule['participants'] as $tmp) {
		list ($prole_tid, $pstatus_tid) = explode('|', $tmp);
		if ($delta > 0) { $sql .= ' or '; }
		$sql .= " (field_sirius_event_prole_tid = :prole_tid_$delta and field_sirius_event_pstatus_tid = :pstatus_tid_$delta ) ";
		$sql_args[":prole_tid_$delta"] = $prole_tid;
		$sql_args[":pstatus_tid_$delta"] = $pstatus_tid;
		++$delta;
	}
	$sql .= ') ';

	$stmt = db_query($sql, $sql_args);
	$participant_nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$participant_nids[] = $hr['participant_nid'];
	}


	$participant_nodes = node_load_multiple($participant_nids);
	$header = array('Name', 'Badge',);
	foreach ($certification_definition_nodes as $certification_definition_nid => $certification_definition_node) {
		$header[] = $certification_definition_node->title;
		$header[] = t('Tools');
	}

	$rows = array();
	foreach ($participant_nodes as $participant_nid => $participant_node) {
		$contact_node = node_load($participant_node->field_sirius_contact['und'][0]['target_id']);
		$worker_node = sirius_worker_get_by_contact_nid($participant_node->field_sirius_contact['und'][0]['target_id']);
		$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
		$esignature = sirius_jsonfield_util_esignature_get($participant_json, $which);
		$row = array();
		$row[] = $worker_node->title;
		$row[] = $worker_node->field_sirius_id['und'][0]['value'];

		// Load all the certifications for this participant
		$query = new EntityFieldQuery();
	  $query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'sirius_log');
	  $query->fieldCondition('field_sirius_category', 'value', 'worker:certification');
	  $query->fieldCondition('field_sirius_log_handler', 'target_id', $participant_nid);
	  $query->propertyOrderBy('created');
	  $stmt = $query->execute();
	  $certification_nids = array();
	  if ($stmt['node']) { $certification_nids = array_keys($stmt['node']); }
	  $certification_nodes = node_load_multiple($certification_nids);

	  $certification_nodes_by_definition = array();
	  foreach ($certification_nodes as $certification_nid => $certification_node) {
	  	$handler_nodes = node_load_multiple(sirius_fieldvals($certification_node, 'field_sirius_log_handler', 'target_id'));
	  	foreach ($handler_nodes as $handler_nid => $handler_node) {
	  		if ($handler_node->type == 'sirius_json_definition') {
	  			$certification_nodes_by_definition[$handler_node->nid][] = $certification_node;
	  		}
	  	}
	  }

		foreach ($certification_definition_nodes as $certification_definition_nid => $certification_definition_node) {
			$paths = array();
			if ($certification_nodes_by_definition[$certification_definition_nid]) {
				$certification_node = $certification_nodes_by_definition[$certification_definition_nid][0];
				$paths['node/' . $certification_node->nid . '/sirius_log_tc/certification_view'] = 'View';
				$paths['node/' . $certification_node->nid . '/sirius_log_tc/certification_edit'] = 'Edit';
				$paths['node/' . $certification_node->nid . '/sirius_log_tc/certification_signatures'] = 'Signatures';
				$paths['node/' . $certification_node->nid . '/sirius_log_tc/certification_grant'] = 'Grant';
				$title = 'Complete';
				$title .= ' - ';
				$title .= $certification_node->field_sirius_type['und'][0]['value'];
			} else {
				$certification_node = '';
				$title = 'Incomplete';
				$paths['sirius/training/certification/' . $worker_node->nid . '/' . $certification_definition_nid . '/' . $participant_node->nid] = 'Add';
			}

			$tools = '';
			foreach ($paths as $path => $tool_title) {
				if (drupal_valid_path($path)) {
					$tools .= l($tool_title, $path, array('attributes' => array('class' => 'tool-button')));
				}
			}

			$row[] = $title;
			$row[] = $tools;
		}

		$rows[] = $row;
	}
	$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));

	return $html;
}
