<?php

function sirius_node_tab_event_phonebank_lock_access($event_node) {
	return sirius_node_tab_event_phonebank_settings_access($event_node);
}

function sirius_node_tab_event_phonebank_lock($event_node) {
	return drupal_get_form('sirius_node_tab_event_phonebank_lock_form', $event_node);
}

function sirius_node_tab_event_phonebank_lock_form($form, &$form_state, $event_node) {
	$form_state['event_nid'] = $event_node->nid;

	$sql = "select sirius_lock.uid, sirius_lock.id, sirius_lock.ts, users.name, users.mail ";
	$sql .= "from sirius_lock ";
	$sql .= "left join users on users.uid = sirius_lock.uid ";
	$sql .= "where sirius_lock.type = :lock_type ";
	$sql .= "order by ts ";
	$stmt = db_query($sql, array(':lock_type' => sirius_event_phonebank_lock_type($event_node)));
	$locks = array();
	$options = array();
	while ($hr = $stmt->fetchAssoc()) {
		$row = array();
		$row[] = $hr['name'] . ' [' . $hr['mail'] . ']';
		$row[] = $hr['id'];
		$row[] = date('Y-m-d h:i:s a');
		$options[$hr['uid']] = $row;
	}

	if (!$options) {
		$form['intro'] = array('#markup' => '<div class="sirius_form_intro">' . t('No participants are locked.') . '</div>');
		return $form;
	}

	$header = array('User', 'Phone', 'Time');
	$form['locks'] = array(
		'#type' => 'tableselect',
		'#title' => 'Current Locks',
		'#header' => $header,
		'#options' => $options,
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_event_phonebank_lock_form_submit($form, &$form_state) {
	$event_nid = $form_state['event_nid'];
	$event_node = node_load($event_nid);
	$uids = array_values($form_state['values']['locks']);
	$lock_type = sirius_event_phonebank_lock_type($event_node);
	if (!$uids) {
		drupal_set_message("Please select at least one lock to clear.", 'warning');
	}

	$count = 0;
	foreach ($uids as $uid) {
		++$count;
		sirius_lock_release_by_uid($lock_type, $uid);
	}
	drupal_set_message(t('@count locks cleared.', array('@count' => $count)));
}