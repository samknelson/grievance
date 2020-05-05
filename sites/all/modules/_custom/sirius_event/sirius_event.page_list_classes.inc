<?php

function sirius_event_page_list_classes_access() {
	return TRUE; 
}

function sirius_event_page_list_classes() {
	$event_types = sirius_taxonomy_load('sirius_event_type', 'tid', 'full');
	$my_worker = sirius_worker_get();
	if ($my_worker) { $my_ms_tids = sirius_fieldvals($my_worker, 'field_sirius_member_status', 'tid'); }

	$event_type_tids = array();
	foreach ($event_types as $event_type_tid => $event_type_term) {
		if (sirius_event_class_enabled_type($event_type_term)) {
			$event_type_tids[] = $event_type_tid;
		}
	}

	if (!$event_type_tids) {
		return t('There are no event types configured as classes.');
	}

	$query = new EntityFieldQuery;
	$query->entityCondition('entity_type', 'node');
	$query->entityCondition('bundle', 'sirius_event');
	$query->fieldCondition('field_sirius_event_type', 'tid', $event_type_tids, 'in');
	$query->fieldCondition('field_sirius_active', 'value', 'Yes');
	$query->fieldCondition('field_sirius_daterepeat', 'value', gmdate('Y-m-d 00:00:00'), '>=');
	$query->fieldOrderBy('field_sirius_daterepeat', 'value');
	$stmt = $query->execute();
	if (!$stmt['node']) { 
		$html .= t('There are no upcoming @titles.', array('@title' => $event_types[$event_type_tid]->name));
	} else {
		$event_nodes = node_load_multiple(array_keys($stmt['node']));

		foreach ($event_nodes as $event_node) {
			$event_json = sirius_jsonfield_util_node_json_get($event_node);
			$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
			$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);
			$mode = $event_json['class']['listing']['mode'];

			// Can I see?
			$can_view = FALSE;
			if ($mode == 'public') { $can_view = TRUE; }
			if ($mode == 'workers') {
				if (user_access('sirius staff') || user_access('sirius event admin') || user_access('sirius admin')) { $can_view = TRUE; }

				if ($my_worker && $my_ms_tids && $event_json['class']['listing']['ms_tids']) {
					$event_ms_tids = array_filter(array_values($event_json['class']['listing']['ms_tids']));
					if (array_intersect($event_ms_tids, $my_ms_tids)) { $can_view = TRUE; }
				}
			}
			if (!$can_view) { continue; }

			$html .= '<div class="sirius_event_class_listing">';
			$html .= '<strong>' . $event_node->title . '</strong>' . '<br>';
		  $html .= sirius_dispatch_view_render_field($event_node, 'field_sirius_daterepeat');
		  $html .= sirius_dispatch_view_render_field($event_node, 'body');

			// Register
			$tools = '';
			$widgets = sirius_event_register_widgets();
			$all_rules = $event_type_json['register']['rules'];
			if (user_access('sirius event admin')) { $rules = $all_rules; } 
			else { $rules = sirius_event_paccess_rule_match($event_node->nid, $all_rules, TRUE); }
			if ($rules) {
				foreach ($rules as $delta => $rule) {
					foreach ($rule['widget'] as $widget) {
						$widget_settings = $event_type_json['widget_settings']['register'][$rule['delta']][$widget];
						$name = $widget_settings['name'];
						if (!$name) { $name = $rule['name'] . ': ' . $widgets[$widget]; }
						$tools .= l($name, 'node/' . $event_node->nid . '/sirius_event_participants_register', array('attributes' => array('class' => array('tool-button')), 'query' => array('delta' => $delta, 'widget' => $widget))); 
					}
				}
			}
			$html .= $tools;

		  $html .= '</div>';
		}
	}

	if (!$html) {
		return 'There are no available upcoming classes.';
	}

	return $html;
}
