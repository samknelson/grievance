<?php

function sirius_dispatch_plugin_events_prefs($json) {
	$event_types = sirius_taxonomy_load('sirius_event_type', 'tid', 'name');
	$form['event_types'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Event Types'),
		'#options' => $event_types
	);
	if ($json['event_types']) { $form['event_types']['#default_value'] = $json['event_types']; }

	$form['class'] = array(
		'#type' => 'select',
		'#title' => t('Show All Classes?'),
		'#options' => array(
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#default_value' => $json['class'],
	);

	$form['days_before'] = array(
		'#type' => 'textfield',
		'#title' => t('Days Before'),
		'#default_value' => $json['days_before'],
		'#description' => t('Show classes that start this many days before today'),
		'#size' => 4,
	);

	$form['days_after'] = array(
		'#type' => 'textfield',
		'#title' => t('Days After'),
		'#default_value' => $json['days_after'],
		'#description' => t('Show classes that start this many days after today'),
		'#size' => 4,
	);

	$form['link_path'] = array(
		'#type' => 'textfield',
		'#title' => t('Link Path'),
		'#default_value' => $json['link_path'],
		'#description' => t('Enter the system path that this dashboard item should link to. The pathshould not begin with a slash (/). For example, "sirius/config".'),
	);

	$form['link_text'] = array(
		'#type' => 'textfield',
		'#title' => t('Link Text'),
		'#default_value' => $json['link_text'],
		'#description' => t('Enter the text that should appear in the link button.'),
	);

	return $form;
}

function sirius_dispatch_plugin_events_data($prefs, $options = array()) {
	$event_types = array();
	if ($prefs['event_types']) { $event_types = array_filter(array_values($prefs['event_types'])); }

	if ($prefs['class'] == 'Yes') {
		$all_event_types = sirius_taxonomy_load('sirius_event_type', 'tid', 'full');
		foreach ($all_event_types as $tid => $term) {
			$event_type_json = sirius_json_get($term);
			if ($event_type_json['class']['enabled'] == 'Yes') {
				if (!in_array($tid, $event_types)) {
					$event_types[] = $tid;
				}
			}
		}
	}

	$days_before = $prefs['days_before'];
	$days_after = $prefs['days_after'];
	if (!$days_before) { $days_before = 0; }
	if (!$days_after) { $days_after = 1; }
	$cutoff_before = gmdate('Y-m-d 00:00:00', strtotime("-$days_before days"));
	$cutoff_after = gmdate('Y-m-d 24:59:59', strtotime("+$days_after days"));

	$domain_nid = sirius_domain_nid();
	$query = new EntityFieldQuery;
	$query->entityCondition('entity_type', 'node');
	$query->entityCondition('bundle', 'sirius_event');
	$query->fieldCondition('field_sirius_daterepeat', 'value', $cutoff_before, '>=');
	$query->fieldCondition('field_sirius_daterepeat', 'value', $cutoff_after, '<=');
	if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
	if ($event_types) { $query->fieldCondition('field_sirius_event_type', 'tid', $event_types, 'in'); }
	$query->fieldOrderBy('field_sirius_daterepeat', 'value', 'asc');
	$stmt = $query->execute();
	if (!$stmt['node']) { return array(); }

	$tools = '';
	if ($prefs['link_path']) {
		$tools = l($prefs['link_text'], $prefs['link_path'], array('attributes' => array('class' => array('tool-button'))));
	}
	$first_event_nid = array_keys($stmt['node'])[0];
	$first_event_node = node_load($first_event_nid);
	$row = array(
		'count' => count($stmt['node']),
		'tools' => $tools,
		'details' => 'E.g. ' . $first_event_node->title,
	);

	return array($row);
}

