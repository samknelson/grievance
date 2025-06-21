<?php

function sirius_dashboard_plugin_bookmarks_prefs($json) {
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

function sirius_dashboard_plugin_bookmarks_data($prefs, $options = array()) {
	$count = sirius_bookmark_count();
	$recent = sirius_bookmark_recent();

	if (!$count) { return array(); }

	$link = l('All Bookmarks', 'sirius/bookmarked', array('attributes' => array('class' => array('tool-button'))));
	$details = array();
	foreach ($recent as $record) {
		$nid = $record['nid'];
		$title = $record['title'];
		$details[] = l($title, "node/$nid", array('attributes' => array('class' => array('tool-button'))));
	}
	if ($details) { $details = 'Recent: ' . join(' ', $details); } else { $details = ''; }

	$row = array(
		'count' => $count,
		'tools' => $link,
		'details' => $details,
	);

	return array($row);
}

