<?php

// drush --user=1 php-eval "sirius_require('sirius_teamsters631', 'sirius_teamsters631.oneoff_workercontact_cleanup.inc'); Sirius_Teamsters631_Oneoff_Workercontact_Cleanup::run('test');"

class Sirius_Teamsters631_Oneoff_Workercontact_Cleanup {
	public static function run($mode = 'test') {
		sirius_minilog_echo_active(TRUE);
		sirius_minilog("Cleanup starting, mode = $mode");

		$worker_nids = self::get_worker_nids($mode);

		sirius_minilog("Found " . count($worker_nids) . " affected workers.");
		foreach ($worker_nids as $worker_nid) {
			self::fix($mode, $worker_nid);
		}
	}

	public static function fix($mode, $worker_nid) {	
		sirius_minilog("=== FIXING: $worker_nid (" . sirius_node_title($worker_nid) . ") ===");

		// Find the old contact nid
		$worker_node = node_load($worker_nid);
		$old_contact_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];
		if (!$old_contact_nid) {
			sirius_minilog("Old contact ID not found. Aborting.", 'error');
			exit();
		}
		// How many pointers will we have to update?
		$sql = "select count(*) as c from field_data_field_sirius_contact where field_sirius_contact_target_id = :old_contact_nid";
		$stmt = db_query($sql, array(':old_contact_nid' => $old_contact_nid));
		$hr = $stmt->fetchAssoc();
		sirius_minilog("... Updating " . $hr['c'] . " pointers.");

		if ($mode != 'live') {
			sirius_minilog("... Mode = $mode, skipping.");
			return;
		}

		// Generate a new contact
		sirius_minilog("... Generating a new contact");
		unset($worker_node->field_sirius_contact['und']);
		node_save($worker_node);
		$new_contact_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];
		if (!$new_contact_nid) {
			sirius_minilog("New contact ID not found. Aborting.", 'error');
			exit();
		}
		if (!$old_contact_nid == $new_contact_nid) {
			sirius_minilog("New contact ID = Old contact ID ($old_contact_nid). Aborting.", 'error');
			exit();
		}

		// Update pointers
		sirius_minilog("... Updating contact pointers from $old_contact_nid to $new_contact_nid ");
		$sql = "update field_data_field_sirius_contact ";
		$sql .= "set field_sirius_contact_target_id = :new_contact_nid ";
		$sql .= "where field_sirius_contact_target_id = :old_contact_nid ";
		$stmt = db_query($sql, array(':old_contact_nid' => $old_contact_nid, ':new_contact_nid' => $new_contact_nid));

		$sql = "update field_revision_field_sirius_contact ";
		$sql .= "set field_sirius_contact_target_id = :new_contact_nid ";
		$sql .= "where field_sirius_contact_target_id = :old_contact_nid ";
		$stmt = db_query($sql, array(':old_contact_nid' => $old_contact_nid, ':new_contact_nid' => $new_contact_nid));

		sirius_minilog("... Done.");
	}

	public static function get_worker_nids($mode = 'test') {
		$sql = "select entity_id from field_data_field_sirius_contact ";
		$sql .= "left join node target_node on target_node.nid = field_data_field_sirius_contact.field_sirius_contact_target_id ";
		$sql .= "where entity_type = 'node' ";
		$sql .= "and bundle = 'sirius_worker' ";
		$sql .= "and (target_node.nid is null or target_node.type != 'sirius_contact') ";
		$sql .= "order by entity_id ";
		$stmt = db_query($sql, array());

		$count = 0;
		while ($hr = $stmt->fetchAssoc()) {
			$worker_nid = $hr['entity_id'];
			++$count;
			$worker_nids[] = $worker_nid;
			sirius_minilog("Found worker: $worker_nid");
		}

		return $worker_nids;
	}
}