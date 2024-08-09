<?php 

function sirius_update_7029_impl() {
	sirius_minilog_echo_active(TRUE);
	sirius_minilog("==== Migrating field_sirius_dispatch_sib_status ====");

	if (!class_exists('Sirius_Sib')) {
		sirius_minilog("Class Sirius_Sib not found. Maybe the module isn't enabled?");
		return;
	}
	$sib = Sirius_Sib::getInstance();

	////////////////////////////////////////////////////
	// Update our legacy data
	////////////////////////////////////////////////////

	$sql = "select * from field_data_field_sirius_dispatch_sib_status ";
	$sql .= "order by entity_id ";
	$stmt = db_query($sql, array());

	$records_to_fix = array();
	while ($hr = $stmt->fetchAssoc()) {
		$records_to_fix[] = $hr;
	}
	if (!$records_to_fix) {
		sirius_minilog("There are no legacy gender entries that haven't been updated. Nothing left to do.");
		return;
	}

	$total = count($records_to_fix);
	sirius_minilog("Found $total legacy gender entries.");

	$cache = array();
	$count = 0;
	foreach ($records_to_fix as $record) {
		$count++;
		$entity_nid = $record['entity_id'];
		$tid = $record['field_sirius_dispatch_sib_status_tid'];

		if (!$cache[$tid]) { $cache[$tid] = sirius_term_title($tid); }
		sirius_minilog("Fixing record $count / $total: Checking worker $nid");
		if (preg_match('/not blocked/i', $cache[$tid])) {
			sirius_minilog("... Status is $cache[$tid]. Skipping.");
			continue;
		}

		$entity_node = node_load($entity_nid);
		$entries = $sib->list($entity_node);
		if ($entries[$tid]['active']) {
			sirius_minilog("... Already assigned. Skipping.");
			continue;
		}

		$sib->assign($entity_node, $tid, 'Migrating legacy status');
		sirius_minilog("... Assigned $cache[$tid] to " . $entity_node->title);
	}
}
