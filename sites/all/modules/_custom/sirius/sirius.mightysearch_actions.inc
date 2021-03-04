<?php

function sirius_mgihtysearch_action_info_all() {
	// @todo will become a hook

	$info['register'] = array(
		'title' => 'Register for Event',
		'selectall' => TRUE,
		'batch' => TRUE,
	);
	$info['export'] = array(
		'title' => 'Export to Spreadsheet',
	);
	$info['tag_add']  = array(
		'title' => 'Add a Tag',
		'selectall' => TRUE,
		'batch' => TRUE,
	);
	$info['tag_remove']  = array(
		'title' => 'Remove a Tag',
		'selectall' => TRUE,
		'batch' => TRUE,
	);
	$info['bulk_recipient_add']  = array(
		'title' => 'Register for Bulk Message',
		'selectall' => TRUE,
		'batch' => TRUE,
	);
	$info['bulk_recipient_remove']  = array(
		'title' => 'Remove Registration for Bulk Message',
		'selectall' => TRUE,
		'batch' => TRUE,
	);
	$info['delete']  = array(
		'title' => 'Delete the Contact',
		'selectall' => TRUE,
		'batch' => TRUE,
	);
	$info['delete_participant']  = array(
		'title' => 'Delete the Participant',
		'selectall' => TRUE,
		'batch' => TRUE,
	);
	$info['change_pstatus']  = array(
		'title' => 'Change Participant Status',
		'selectall' => TRUE,
		'batch' => TRUE,
	);
	$info['report']  = array(
		'title' => 'Report',
	);
	$info['geocode']  = array(
		'title' => 'Geocode',
		'selectall' => TRUE,
		'batch' => TRUE,
	);
	$info['sql']  = array(
		'title' => 'Display SQL Query',
		'permission' => 'sirius admin',
		'universal' => TRUE,
	);

	return $info;
}
