<?php

function sirius_node_tab_event_participants_register_access($event_node) {
	if (user_access('sirius event admin')) { return TRUE; }

	if ($event_node->field_sirius_active['und'][0]['value'] == 'No') { return FALSE; }

	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
	$rules = sirius_event_paccess_rule_match($event_node->nid, $json['register']['rules'], TRUE);
	if ($rules) { return TRUE; }

	return FALSE; 
}


function sirius_node_tab_event_participants_register($event_node) {
	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
	$all_rules = $json['register']['rules'];
	if (user_access('sirius event admin')) {
		$rules = $all_rules;
	} else {
		$rules = sirius_event_paccess_rule_match($event_node->nid, $all_rules, TRUE);
	}
	$delta = $_REQUEST['delta'];
	if (!isset($delta)) {
		$widgets = sirius_event_register_widgets();

		$html = '';
		if (!$rules) {
			return t('You do not have access to any registration rules for this event type.');
		}
		foreach ($rules as $delta => $rule) {
			$html .= '<strong>' . $rule['name'] . '</strong><br />';
			foreach ($rule['widget'] as $widget) {
				$html .= l($widgets[$widget], current_path(), array('attributes' => array('class' => array('tool-button')), 'query' => array('delta' => $delta, 'widget' => $widget))); 
			}
			$html .= '<br /><br />';
		}
		return $html;
	} else {
		$rule = $rules[$delta];
		$widget = $_REQUEST['widget'];
		if (!$rule) { return t('No such rule.'); }
		if (!in_array($widget, $rule['widget'])) { return t('No such widget.'); }


		return sirius_node_tab_event_participants_register_rule($event_node, $rule, $widget);
	}
}

function sirius_node_tab_event_participants_register_rule($event_node, $rule, $widget) {
	if ($widget == 'user_select') {
		return drupal_get_form('sirius_event_register_user_select_form', $event_node, $rule);
	} else if ($widget == 'bullpen_signin') {
		return drupal_get_form('sirius_event_register_bullpen_signin_form', $event_node, $rule);
	} else if ($widget == 'worker_byname') {
		return drupal_get_form('sirius_event_register_worker_byname_form', $event_node, $rule);
	} else if ($widget == 'contact_bulk') {
		list ($prole_tid, $pstatus_tid) = explode('|', array_values($rule['register_as'])[0]);
		return drupal_get_form('sirius_node_tab_event_participants_register_bulk_form', $event_node, $prole_tid);
	}

	return t('Widget not found: @widget', array('@widget' => $widget));
}

