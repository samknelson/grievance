<?php

function sirius_teamsters631_oneoff_russallen() {
	$content_types = array(
		'sirius_log',
		'sirius_dispatch_job',
		'sirius_dispatch',
		'sirius_event'
	);
	
	$fields = array();

	$all_fields = field_info_fields();
	foreach ($all_fields as $field_name => $info) {
		if ($field_name == 'body') { continue; }
		foreach ($content_types as $content_type) {
			if ($info['bundles']['node'] && in_array($content_type, $info['bundles']['node'])) {
				$fields[$content_type][] = $field_name;
			}
		}
	}

	$database_info = array(
	  'host' => 'restored-dispatch-mysql-from-2021-05-22.csergrsuiexu.us-east-2.rds.amazonaws.com', 
	  'database' => 'dispatch_prod',
	  'username' => 'dispatch_prod', 
	  'password' => 'REMOVED_FOR_SECURITY', 
	  'driver' => 'mysql',
	);
	Database::addConnectionInfo('russallen', 'default', $database_info);

	print "========= NEW ======\n";
	db_set_active('russallen');

	$sql = "select * from node where uid = 72771";
	$stmt = sirius_sql_query($sql, array());
	while ($hr = $stmt->fetchAssoc()) {
		$data['node'][] = $hr;
	}


	foreach ($data['node'] as $node_data) {
		$type = $node_data['type'];
		$nid = $node_data['nid'];

		$table_name = "node_revision";
		$sql = "select * from $table_name where nid = :nid";
		$stmt = sirius_sql_query($sql, array(':nid' => $nid));
		$hr = $stmt->fetchAssoc();
		if ($hr) {
			$data[$table_name][] = $hr;
		}

		foreach ($fields[$type] as $field_name) {
			$table_name = "field_data_$field_name";
			$sql = "select * from $table_name where entity_type = 'node' and entity_id = :nid";
			$stmt = sirius_sql_query($sql, array(':nid' => $nid));
			$hr = $stmt->fetchAssoc();
			if ($hr) {
				$data[$table_name][] = $hr;
			}

			$table_name = "field_revision_$field_name";
			$sql = "select * from $table_name where entity_type = 'node' and entity_id = :nid";
			$stmt = sirius_sql_query($sql, array(':nid' => $nid));
			$hr = $stmt->fetchAssoc();
			if ($hr) {
				$data[$table_name][] = $hr;
			}
		}
	}
	print_r($data);

	print "========= OLD ======\n";
	db_set_active();

	foreach ($data as $table_name => $records) {
		foreach ($records as $record) {
			if ($table_name == 'node' || $table_name == 'node_revision') { $nid = $record['nid']; } else { $nid = $record['entity_id']; }
			print "Inserting: $table_name, $nid\n";

			$sql_args = array();
			foreach ($record as $key => $value) { $sql_args[":$key"] = $value; }

			$sql = "insert ignore into $table_name (";
			$sql .= join(', ', array_keys($record));
			$sql .= ") values (";
			$sql .= join(', ', array_keys($sql_args));
			$sql .= ")";
			try {
				$stmt = sirius_sql_query($sql, $sql_args);
			} catch (exception $e) {
				print "... Failed\n";
			}
		}
	}
}



function sirius_teamsters631_oneoff_notification_update() {
	// Get the last job created by this job at this employer with this type.
	$sql = "select entity_id from field_data_field_sirius_dispatch_medium ";
	$sql .= "where entity_type = 'node' ";
	$sql .= "and bundle = 'sirius_worker' ";
	$sql .= "and delta = 2 "; 
	$sql .= "and field_sirius_dispatch_medium_value = 'email' ";
	$sql .= "order by entity_id ";

	$worker_nids = array();
	$stmt = db_query($sql, array());
	while ($hr = $stmt->fetchAssoc()) {
		$worker_nids[] = $hr['entity_id'];
	}

	foreach ($worker_nids as $worker_nid) {
		print "Scanning: $worker_nid\n";
		$sql = "select * from field_data_field_sirius_type, field_data_field_sirius_category, field_data_field_sirius_log_handler ";
		$sql .= "where field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.bundle = 'sirius_log' ";
		$sql .= "and field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.bundle = 'sirius_log' ";
		$sql .= "and field_data_field_sirius_log_handler.entity_type = 'node' and field_data_field_sirius_log_handler.bundle = 'sirius_log' ";
		$sql .= "and field_data_field_sirius_type.entity_id = field_data_field_sirius_category.entity_id ";
		$sql .= "and field_data_field_sirius_category.entity_id = field_data_field_sirius_log_handler.entity_id ";
		$sql .= "and field_sirius_log_handler_target_id = :worker_nid ";
		$sql .= "and field_sirius_category_value = 'worker:dispatch' ";
		$sql .= "and field_sirius_type_value = 'update_preferences' ";
		$sql .= "limit 1 ";
		$stmt = db_query($sql, array(':worker_nid' => $worker_nid));
		$hr = $stmt->fetchAssoc();
		if ($hr) {
			print "... Already set preferences; skipping\n";
			continue; 
		}

		$sql = "select * from field_data_field_sirius_phone ";
		$sql .= "where entity_type = 'node' ";
		$sql .= "and entity_id = :worker_nid ";
		$stmt = db_query($sql, array(':worker_nid' => $worker_nid));
		$hr = $stmt->fetchAssoc();
		if (!$hr) {
			print "... No phone; skipping.\n";
			continue; 
		}

		$tables = array('field_data_field_sirius_dispatch_medium', 'field_revision_field_sirius_dispatch_medium');
		foreach ($tables as $table) {
			$sql = "update $table set field_sirius_dispatch_medium_value = 'sms' where entity_type = 'node' and entity_id = :worker_nid and delta = 2 ";
			$stmt = db_query($sql, array(':worker_nid' => $worker_nid));
		}
		print "... Updated.\n";
	}
}

function sirius_teamsters631_oneoff_10hrosha() {
	$sql = "select * from field_data_field_sirius_skill, field_data_field_sirius_skill_expire ";
	$sql .= "where field_data_field_sirius_skill.entity_type = 'node' ";
	$sql .= "and field_data_field_sirius_skill.bundle = 'sirius_worker' ";
	$sql .= "and field_data_field_sirius_skill_expire.entity_type = 'node' ";
	$sql .= "and field_data_field_sirius_skill_expire.bundle = 'sirius_worker' ";
	$sql .= "and field_data_field_sirius_skill_expire.entity_id = field_data_field_sirius_skill.entity_id ";
	$sql .= "and field_data_field_sirius_skill_expire.delta = field_data_field_sirius_skill.delta ";
	$sql .= "and field_sirius_skill_tid = 594 ";
	$sql .= "order by field_sirius_skill_expire_value ";

	$stmt = db_query($sql, array());
	while ($hr = $stmt->fetchAssoc()) {
		$worker_nid = $hr['entity_id'];
		$workers[$worker_nid] = array('delta' => $hr['delta'], 'expires' => $hr['field_sirius_skill_expire_value']);

		print "$worker_nid: $hr[field_sirius_skill_expire_value]\n";
	}
	print_r($workers);

	return;
	foreach ($worker_nids as $worker_nid) {
		$worker_node = node_load($worker_nid);
		// Grant 745, expire 5 years after [something]
		// Set 594 to expire [never]
	}
	print_r($worker_nids);
}









function sirius_teamsters631_oneoff_skill_log_link($reset = NULL, $limit = 100) {
	if ($reset) { variable_set('sirius_teamsters631_oneoff_skill_log_link_last_nid', NULL); }
	$last_nid = variable_get('sirius_teamsters631_oneoff_skill_log_link_last_nid', NULL);

  $vocabulary = taxonomy_vocabulary_machine_name_load('sirius_skill');
  $terms = taxonomy_get_tree($vocabulary->vid);

	$lookups = array();
	foreach ($terms as $term) {
		$skill_name = $term->name;
		$lookups[trim($skill_name)] = $term->tid;
	}

	$lookups['CDL End T'] = $lookups['CDL End T (Doubles/Triples)'];
	$lookups['CDL End N'] = $lookups['CDL End N  (Tanker)'];
	$lookups['CDL End X'] = $lookups['CDL End X (Haz-Tank)'];
	$lookups['CDL End P'] = $lookups['CDL End P (Passenger)'];
	$lookups['CDL End H'] = $lookups['CDL End H (Haz)'];
	$lookups['Boot Truck'] = $lookups['Boot Truck (Spreader)'];
	$lookups['First Aid/CPR/AED'] = $lookups['First Aid/CPR/AED (AHA or ARC)'];
	$lookups['Painter/Body Man'] = $lookups['Painter, Body Man'];
	$lookups['semi flatbed / van / reefer'] = $lookups['Semi Flatbed/Van/Reefer'];
	$lookups['water truck, 2500-4000 gallon'] = $lookups['Water Truck, Over 2500 Gallons'];
	$lookups['water truck, under 2500 gallon'] = $lookups['Water Truck, Under 2500 Gallons'];
	$lookups['40hr hazwoper'] = $lookups['40 HR Hazwoper'];
	$lookups['8 hr hazwopper/refresher'] = $lookups['8 HR Hazwoper Refresher'];
	$lookups['8hr hazwoper/refresher'] = $lookups['8 HR Hazwoper Refresher'];
	$lookups['carpet cleaner wareh'] = $lookups['Carpet Cleaner Warehouse'];
	$lookups['cdl end p (passenger'] = $lookups['CDL End P (Passenger)'];
	$lookups['cdl permit only'] = $lookups['CDL'];
	$lookups['class c cdl'] = $lookups['Class C (under 65)'];
	$lookups['compactor/sheeps foot'] = $lookups['Compactor, Sheeps Foot'];
	$lookups['environmetal technician'] = $lookups['Environmental Technician'];
	$lookups['fireman, certified'] = $lookups['Firefighter Certified'];
	$lookups['first aid / cpr training'] = $lookups['First Aid/CPR/AED (AHA or ARC)'];
	$lookups['forklift, certified,construction'] = $lookups['Forklift Certified, Construction'];
	$lookups['fuel distribution sys mechanic'] = $lookups['Fuel Distribution System Mechanic'];
	$lookups['gate attendants / scale operator'] = $lookups['Gate Attendants, Scale Operator'];
	$lookups['gate attendants/scale operator'] = $lookups['Gate Attendants, Scale Operator'];
	$lookups['hazmat 16 hr emergency response'] = $lookups['16 HR Emergency Response'];
	$lookups['heavy duty greaser/oiler'] = $lookups['Heavy Duty Greaser, Oiler'];
	$lookups['housekeeper/janitorial'] = $lookups['Housekeeper, Janitorial'];
	$lookups['i & d'] = $lookups['I&D'];
	$lookups['landoll tiltbeds'] = $lookups['Tiltbed Trailer'];
	$lookups['maintenance/electrician'] = $lookups['Electrician Maintenance'];
	$lookups['off road belly dumps'] = $lookups['Off Road Only Belly Dumps'];
	$lookups['off road water truck'] = $lookups['Off Road Only Water Truck'];
	$lookups['osha for convention'] = $lookups['OSHA for Convention'];
	$lookups['plumber/maintenance'] = $lookups['Plumber, Maintenance'];
	$lookups['scraper/grader'] = $lookups['Scraper, Grader'];
	$lookups['transfer trailer-garbage'] = $lookups['Transfer Trailer, Garbage'];
	$lookups['versalift forklift'] = $lookups['Forklift, Versalift'];
	$lookups['yardma'] = $lookups['Yardman, Laborer'];
	$lookups['yardman/    laborer'] = $lookups['Yardman, Laborer'];
	$lookups['10 hr osha, general'] = $lookups['10 HR OSHA, General Industry'];
	$lookups['10hr osha, general'] = $lookups['10 HR OSHA, General Industry'];
	$lookups['10hr osha, construction'] = $lookups['10 HR OSHA, Construction'];
	$lookups['osha for convention'] = $lookups['OSHA for Convention'];

	foreach ($lookups as $skill_name => $tid) {
		$lookups[strtolower($skill_name)] = $tid;
	}
	ksort($lookups);

	$found = $unfound = array();

	$sql = "select nid, created, field_sirius_message_value from node ";
	$sql .= "left join field_data_field_sirius_message on field_data_field_sirius_message.entity_type = 'node' and field_data_field_sirius_message.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_log' ";
	$sql .= "and field_sirius_category_value = 'worker:skill' ";
	if ($last_nid) { $sql .= "and nid < $last_nid "; }
	$sql .= "order by nid desc limit $limit ";

	$sql_args = array();
	$stmt = sirius_sql_query($sql, $sql_args);
	while ($hr = $stmt->fetchAssoc()) {
		$nid = $hr['nid'];
		$msg = $hr['field_sirius_message_value'];
		if (!$msg) { continue; }
		$date = date('Y-m-d H:i:s', $hr['created']);
		print "==== Log node $nid / $date / $msg \n"; 

		$msg = preg_replace('/Add or renew/', '', $msg);
		$msg = preg_replace('/^Remove /', '', $msg);
		$msg = preg_replace('/\([^\)]+\)/', '', $msg);
		$msg = preg_replace('/Skipping add for skills, because they expired on \d+\/\d+\/\d+/', '', $msg);
		$msg = preg_replace('/\[MASQ FROM [^\]]+\]/', '', $msg);
		$msg = preg_replace('/^\s*Remove /', '', $msg);
		$msg = preg_replace('/^\:\s*/', '', $msg);

		$msgs = explode(';', $msg);
		foreach ($msgs as $msg2) {
			$skill_names = explode('] [', $msg2);
			foreach ($skill_names as $skill_name) {
				$skill_name = trim($skill_name);
				$skill_name = preg_replace('/\]/', '', $skill_name);
				$skill_name = preg_replace('/\[/', '', $skill_name);
				$skill_name = preg_replace('/Add or renew/', '', $skill_name);
				$skill_name = preg_replace('/^Remove /', '', $skill_name);
				$skill_name = preg_replace('/\([^\)]+\)/', '', $skill_name);
				$skill_name = preg_replace('/Skipping add for skills, because they expired on \d+\/\d+\/\d+/', '', $skill_name);
				$skill_name = preg_replace('/\[MASQ FROM [^\]]+\]/', '', $skill_name);
				$skill_name = preg_replace('/^\s*Remove /', '', $skill_name);
				$skill_name = preg_replace('/^\:\s*/', '', $skill_name);
				$skill_name = strtolower($skill_name);
				$skill_name = preg_replace('/^renewed */', '', $skill_name);
				$skill_name = preg_replace('/^added */', '', $skill_name);
				$skill_name = preg_replace('/^added */', '', $skill_name);
				$skill_name = preg_replace('/until \d+\-\d+\-\d+/', '', $skill_name);
				$skill_name = preg_replace('/skipping add for skills, because they expired on \d+/', '', $skill_name);
				$skill_name = trim($skill_name);


				if (!$skill_name) { continue; }

				$tid = $lookups[$skill_name];
				if ($tid) { ++$found[$skill_name]; } else { ++$unfound[$skill_name]; }
				if (!$tid) { 
					print "$skill_name: Not found\n";
					continue;
				}

				$proxy_node = sirius_taxonomy_proxy($tid);
				$proxy_nid = $proxy_node->nid;
				if (!$proxy_nid) { continue; }

				$log_node = node_load($nid);
				$handlers = array_keys(sirius_log_handlers($log_node, 'sirius_term_proxy'));
				if (in_array($proxy_nid, $handlers)) { 
					print "$skill_name: Skipping $proxy_nid for $nid because it's already there\n";
					continue;
				}
				print "$skill_name: Adding $proxy_nid to $nid\n";
				$log_node->field_sirius_log_handler['und'][] = array('target_id' => $proxy_nid);
				node_save($log_node);
			}
		}

		variable_set('sirius_teamsters631_oneoff_skill_log_link_last_nid', $nid);
	}

	print "--------- Found -----------\n";
	ksort($found);
	print_r($found);
	print "--------- Un-found -----------\n";
	foreach ($unfound as $key => $count) { if ($count < 10) { unset($unfound[$key]); }}
	ksort($unfound);
	print_r($unfound);
}
