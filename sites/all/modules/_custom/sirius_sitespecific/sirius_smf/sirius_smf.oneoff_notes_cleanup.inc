<?php

// while true ; do drush --user=1 php-eval "sirius_require('sirius_smf', 'sirius_smf.oneoff_notes_cleanup.inc'); sirius_smf_oneoff_notes_cleanup();" ; sleep 10; done

function sirius_smf_oneoff_notes_cleanup() {
	$sql = "select count(*) as c from field_data_field_sirius_message ";
	$sql .= "where field_sirius_message_value = 'Imported notes' ";
	$sql .= "and entity_type = 'node' ";
	$sql .= "and bundle = 'sirius_log' ";
	$stmt = db_query($sql, array());
	$hr = $stmt->fetchAssoc();
	$grand_total = $hr['c'];

	$sql = "select * from field_data_field_sirius_message ";
	$sql .= "where field_sirius_message_value = 'Imported notes' ";
	$sql .= "and entity_type = 'node' ";
	$sql .= "and bundle = 'sirius_log' ";
	$sql .= "order by entity_id limit 1000 ";

	$stmt = db_query($sql, array());
	$log_nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$log_nids[] = $hr['entity_id'];
	}

	$count = 0;
	foreach ($log_nids as $log_nid) {
		++$count;
		--$grand_total;

		$log_node = node_load($log_nid);
		$log_node->field_sirius_message['und'][0]['value'] = substr($log_node->field_sirius_summary['und'][0]['value'], 0, 255);
		node_save($log_node);
		print "$count / $grand_total: " . preg_replace("/[^a-zA-Z0-9_ ]/", ' ', substr($log_node->field_sirius_summary['und'][0]['value'], 0, 40)) . "\n";
	}
}