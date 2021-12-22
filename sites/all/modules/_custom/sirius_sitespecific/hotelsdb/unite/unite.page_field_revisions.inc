<?php

function unite_node_tab_field_revisions($node) {
	drupal_set_title($node->title . ': ' . t('Field Revisions'));
	
	$instance = array();
	$instances['title'] = array('label' => 'Title');
	$instances += field_info_instances('node', $node->type);

	$boring_fields = array(
		'field_address_geofield',
		'field_legacy_id'
	);
	
	$rows = array();
	
	foreach ($instances as $name => $instance) {
		if (in_array($name, $boring_fields)) { continue; }
		
		if ($name == 'title') {
			$sql = "select * from node_revision ";
			$sql .= "where node_revision.nid = :nid ";
			$sql .= "order by node_revision.timestamp";
		} else {
			$sql = "select * from field_revision_{$name}, node_revision ";
			$sql .= "where node_revision.vid = field_revision_{$name}.revision_id ";
			$sql .= "and node_revision.nid = :nid ";
			$sql .= "and entity_type = 'node' ";
			$sql .= "order by field_revision_{$name}.delta, node_revision.timestamp";
		}
		$stmt = db_query($sql, array("nid" => $node->nid));
		
		$first = 1;
		$last = array();
		while ($hr = $stmt->fetchAssoc()) {
			$value = "";

			if ($name == 'title') {
				$value = $hr['title'];
			} else {
				foreach ($hr as $field => $v) {
					if (preg_match("/$name/", $field) && !preg_match('/revision/', $field) && !preg_match('/format/', $field)) {
						if ($value && $v) { $value .= " / "; }
						$value .= $v;
					}
				}
			}

			$value = preg_replace('/\s*00:00:00/', '', $value);

			if ($last_value != $value) {
				$last_value = $value;
				$u = user_load($hr['uid']);
				$row = array();
				if ($first) {
					$row[] = $instance['label'];
					$row[] = $name;
				} else {
					$row[] = '';
					$row[] = '';
				}
				$row[] = $u->name;
				$row[] = date('Y-m-j H:i:s', $hr['timestamp']);
				$row[] = $value;
				$rows[] = $row;
				$first = 0;
			}
		}
	}
	
	$header = array(
		'Field Name', 
		'Internal Field Name', 
		'User',
		'Date',
		'Value'
	);
	return theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
}
