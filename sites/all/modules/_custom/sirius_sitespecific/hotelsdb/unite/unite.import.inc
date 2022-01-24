<?php

/**
 * One-off functions. I believe that this entire file is unreferenced and should be removed.
 */

function unite_import_development_status() {
	$stmt = db_query(
		'SELECT nid FROM {node} WHERE type = :type',
		array(':type' => 'property')	
	);
	
	$nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$nids[] = $hr['nid'];
	}
	
	foreach ($nids as $nid) {
		print "Checking: $nid ...\n";
		$node = node_load($nid);
		
		if ($node->field_development_status['und'][0]) {
			print "... DS exists.  Skipping.\n";
			continue;
		}
		
		if (!$node->field_legacy_id['und'][0]['value']) {
			print "... No legacy id.  Skipping.\n";
			continue;
		}
		$legacy_id = $node->field_legacy_id['und'][0]['value'];
		$legacy_id = preg_replace('/property_/', '', $legacy_id);
		
		print "... Found legacy id $legacy_id ...\n";

		db_set_active('sps');
		$sql = "select * from h_Properties where property_id = :legacy_id ";
		$stmt = db_query($sql, array('legacy_id' => $legacy_id));
		$hr = $stmt->fetchAssoc();
		db_set_active('default');

		if (!$hr['development_status']) { 
			print "... no DS.  Skipping.\n";
			continue;
		}
		
		$node->field_development_status['und'][0]['value'] = $hr['development_status'];
		
		node_save($node);
		
		print "... Done\n";
		// break;
	}
}



function unite_import_geo() {
	$stmt = db_query(
		'SELECT nid FROM {node} WHERE type = :type',
		array(':type' => 'property')
	);
	
	$nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$nids[] = $hr['nid'];
	}
	
	foreach ($nids as $nid) {
		print "Checking: $nid ...\n";
		$node = node_load($nid);
		
		if ($node->field_address_geofield['und'][0]) {
			print "... Geofield exists.  Skipping.\n";
			continue;
		}
		
		if (!$node->field_legacy_id['und'][0]['value']) {
			print "... No legacy id.  Skipping.\n";
			continue;
		}
		$legacy_id = $node->field_legacy_id['und'][0]['value'];
		$legacy_id = preg_replace('/property_/', '', $legacy_id);
		
		print "... Found legacy id $legacy_id ...\n";

		db_set_active('sps');
		$sql = "select * from h_Properties where property_id = :legacy_id ";
		$stmt = db_query($sql, array('legacy_id' => $legacy_id));
		$hr = $stmt->fetchAssoc();
		db_set_active('default');

		if (!$hr['geo_latitude'] || !$hr['geo_longitude']) { 
			print "... no latitude.  Skipping.\n";
			continue;
		}
		
		$latitude = $hr['geo_latitude'] / 1000000;
		$longitude = $hr['geo_longitude'] / 1000000;

		print "... Setting: $latitude, $longitude ...\n";

		$node->field_address_geofield['und'][0]['lat'] = $latitude;
		$node->field_address_geofield['und'][0]['lon'] = $longitude;
		
		node_save($node);
		
		print "... Done\n";
		// break;
	}
}

function unite_import_brand() {
	db_set_active('sps');
	$sql = "select property_id, brand_id from h_Properties where brand_id is not null and brand_id > 0 ";
	$stmt = db_query($sql, array());
	$records = array();
	while ($hr = $stmt->fetchAssoc()) {
		$records[] = $hr;
	}
	db_set_active('default');

	foreach ($records as $hr) {
		print_r($hr);
		$legacy_property_id = $hr['property_id'];
		$legacy_brand_id = $hr['brand_id'];
		
		$sql = "select entity_id from field_data_field_legacy_id ";
		$sql .= "where field_legacy_id_value = :legacy_id ";
		$sql .= "and entity_type = 'taxonomy_term' ";
		$sql .= "and bundle = 'brand' ";
		$stmt = db_query($sql, array('legacy_id' => $legacy_brand_id));
		$hr2 = $stmt->fetchAssoc();
		$brand_tid = $hr2['entity_id'];
		if (!$brand_tid) { continue; }
		
		print "Brand TID: $brand_tid\n";

		$sql = "select entity_id from field_data_field_legacy_id ";
		$sql .= "where field_legacy_id_value = :legacy_id ";
		$sql .= "and entity_type = 'node' ";
		$sql .= "and bundle = 'property' ";
		$stmt = db_query($sql, array('legacy_id' => "property_$legacy_property_id"));
		$hr2 = $stmt->fetchAssoc();
		$property_nid = $hr2['entity_id'];
		if (!$property_nid) { die("Boo for $legacy_property_id "); continue; }
		
		print "Property NID: $property_nid\n";
		
		$node = node_load($property_nid);
		$node->field_brand['und'][0]['tid'] = $brand_tid;
		node_save($node);
	}
}

function unite_import_zip_to_msa() {
	$nids = db_query(
		'SELECT nid FROM {node} WHERE type = :type',
		array(':type' => 'zip_to_msa')
	);
	foreach ($nids as $nid) {
		print "Deleting " . $nid->nid . "\n";
		node_delete($nid->nid);
	}
	
	// Fetch from the old
	db_set_active('sps');
	// $last_wsh = 0;
	$sql = "select * from MSAZipLookup order by zip ";
	$stmt = db_query($sql, array("last_wsh" => $last_wsh));
	$records = array();
	while ($hr = $stmt->fetchAssoc()) {
		$records[] = $hr;
	}
	db_set_active('default');

	foreach ($records as $hr) {
		print_r($hr);
		
		$sql = "select entity_id from field_data_field_legacy_id ";
		$sql .= "where field_legacy_id_value = :legacy_id ";
		$sql .= "and entity_type = 'taxonomy_term' ";
		$sql .= "and bundle = 'msa' ";
		$stmt = db_query($sql, array('legacy_id' => $hr['msa_id']));
		$hr2 = $stmt->fetchAssoc();
		$entity_id = $hr2['entity_id'];
		if (!$entity_id) { die("No entity ID."); }
		
		print "$entity_id\n";
		
		$node = new stdClass();
		$node->type = "zip_to_msa";
		$node->language = LANGUAGE_NONE;
		$node->uid = 1;
		node_object_prepare($node);
		$node->field_zip["und"][0]["value"] = $hr['zip'];
		$node->field_msa['und'][0]['tid'] = $entity_id;
		$node = node_submit($node);
		node_save($node);
	}
}

function unite_import_airport() {
	$airport['8296'] = 1;
	$airport['13926'] = 1;
	$airport['13949'] = 1;
	$airport['13951'] = 1;
	$airport['14020'] = 3;
	$airport['13888'] = 5;
	$airport['2583'] = 10;
	$airport['2780'] = 10;
	$airport['2829'] = 10;
	$airport['2918'] = 10;
	$airport['2994'] = 10;
	$airport['3031'] = 10;
	$airport['3041'] = 10;
	$airport['3065'] = 10;
	$airport['3107'] = 10;
	$airport['3841'] = 10;
	$airport['3854'] = 10;
	$airport['3856'] = 10;
	$airport['3859'] = 10;
	$airport['3900'] = 10;
	$airport['3903'] = 10;
	$airport['3919'] = 10;
	$airport['4011'] = 10;
	$airport['4030'] = 10;
	$airport['4111'] = 10;
	$airport['4120'] = 10;
	$airport['4127'] = 10;
	$airport['4128'] = 10;
	$airport['14587'] = 10;
	$airport['42904'] = 10;
	$airport['2821'] = 11;
	$airport['3108'] = 11;
	$airport['3218'] = 11;
	$airport['4585'] = 11;
	$airport['4587'] = 11;
	$airport['1527'] = 13;
	$airport['1534'] = 13;
	$airport['1666'] = 14;
	$airport['2630'] = 15;
	$airport['4184'] = 15;
	$airport['43050'] = 15;
	$airport['828'] = 16;
	$airport['1529'] = 16;
	$airport['1543'] = 16;
	$airport['1547'] = 16;
	$airport['1548'] = 16;
	$airport['1550'] = 16;
	$airport['1561'] = 16;
	$airport['1574'] = 16;
	$airport['1575'] = 16;
	$airport['1597'] = 16;
	$airport['1599'] = 16;
	$airport['1604'] = 16;
	$airport['1616'] = 16;
	$airport['4575'] = 16;
	$airport['14285'] = 16;
	$airport['20104'] = 16;
	$airport['100254'] = 16;
	$airport['43016'] = 16;
	$airport['3298'] = 17;
	$airport['3299'] = 17;
	$airport['3300'] = 17;
	$airport['3302'] = 17;
	$airport['3303'] = 17;
	$airport['3305'] = 17;
	$airport['3306'] = 17;
	$airport['3307'] = 17;
	$airport['3308'] = 17;
	$airport['3309'] = 17;
	$airport['3310'] = 17;
	$airport['3311'] = 17;
	$airport['3312'] = 17;
	$airport['3313'] = 17;
	$airport['3314'] = 17;
	$airport['3315'] = 17;
	$airport['3316'] = 17;
	$airport['3317'] = 17;
	$airport['3318'] = 17;
	$airport['3319'] = 17;
	$airport['3325'] = 17;
	$airport['3326'] = 17;
	$airport['4239'] = 17;
	$airport['4240'] = 17;
	$airport['4241'] = 17;
	$airport['4242'] = 17;
	$airport['4243'] = 17;
	$airport['4244'] = 17;
	$airport['4245'] = 17;
	$airport['4246'] = 17;
	$airport['4247'] = 17;
	$airport['4248'] = 17;
	$airport['4249'] = 17;
	$airport['4250'] = 17;
	$airport['4251'] = 17;
	$airport['4253'] = 17;
	$airport['4254'] = 17;
	$airport['11047'] = 17;
	$airport['11406'] = 17;
	$airport['11407'] = 17;
	$airport['40264'] = 17;
	$airport['40311'] = 17;
	$airport['40312'] = 17;
	$airport['40313'] = 17;
	$airport['40315'] = 17;
	$airport['40316'] = 17;
	$airport['40317'] = 17;
	$airport['2389'] = 18;
	$airport['2401'] = 18;
	$airport['2667'] = 18;
	$airport['2676'] = 18;
	$airport['2680'] = 18;
	$airport['2686'] = 18;
	$airport['14511'] = 18;
	$airport['14513'] = 18;
	$airport['14525'] = 18;
	$airport['14529'] = 18;
	$airport['14564'] = 18;
	$airport['14565'] = 18;
	$airport['14566'] = 18;
	$airport['14567'] = 18;
	$airport['14575'] = 18;
	$airport['43597'] = 18;
	$airport['43603'] = 18;
	$airport['1545'] = 19;
	$airport['2589'] = 20;
	$airport['2591'] = 20;
	$airport['2598'] = 20;
	$airport['2604'] = 20;
	$airport['2612'] = 20;
	$airport['2614'] = 20;
	$airport['2624'] = 20;
	$airport['2633'] = 20;
	$airport['2635'] = 20;
	$airport['2643'] = 20;
	$airport['2644'] = 20;
	$airport['3365'] = 22;
	$airport['3366'] = 22;
	$airport['3368'] = 22;
	$airport['3370'] = 22;
	$airport['3372'] = 22;
	$airport['3374'] = 22;
	$airport['3772'] = 22;
	$airport['4770'] = 22;
	$airport['10397'] = 22;
	$airport['41166'] = 23;
	$airport['43001'] = 24;
	$airport['41524'] = 24;
	$airport['3330'] = 24;
	$airport['3332'] = 24;
	$airport['3335'] = 24;
	$airport['3336'] = 24;
	$airport['3337'] = 24;
	$airport['3339'] = 24;
	$airport['3341'] = 24;
	$airport['3342'] = 24;
	$airport['3345'] = 24;
	$airport['3351'] = 24;
	$airport['3354'] = 24;
	$airport['3358'] = 24;
	$airport['3359'] = 24;
	$airport['3362'] = 24;
	$airport['3363'] = 24;
	$airport['3364'] = 24;
	$airport['4187'] = 24;
	$airport['4349'] = 24;
	$airport['4350'] = 24;
	$airport['43027'] = 24;
	$airport['4565'] = 24;
	$airport['4641'] = 24;
	$airport['4736'] = 24;
	$airport['4793'] = 24;
	$airport['4840'] = 24;
	$airport['9718'] = 24;
	$airport['41183'] = 24;
	$airport['20141'] = 24;
	$airport['20181'] = 24;
	$airport['20188'] = 24;
	$airport['20273'] = 24;
	$airport['40085'] = 24;
	$airport['40088'] = 24;
	$airport['40091'] = 24;
	$airport['40092'] = 24;
	$airport['40093'] = 24;
	$airport['40094'] = 24;
	$airport['40095'] = 24;
	$airport['40097'] = 24;
	$airport['40098'] = 24;
	$airport['40099'] = 24;
	$airport['40101'] = 24;
	$airport['40102'] = 24;
	$airport['40105'] = 24;
	$airport['40108'] = 24;
	$airport['40112'] = 24;
	$airport['40113'] = 24;
	$airport['40114'] = 24;
	$airport['40115'] = 24;
	$airport['40116'] = 24;
	$airport['40120'] = 24;
	$airport['40121'] = 24;
	$airport['40122'] = 24;
	$airport['40124'] = 24;
	$airport['40125'] = 24;
	$airport['40127'] = 24;
	$airport['40128'] = 24;
	$airport['40130'] = 24;
	$airport['40134'] = 24;
	$airport['40135'] = 24;
	$airport['40137'] = 24;
	$airport['40140'] = 24;
	$airport['40141'] = 24;
	$airport['40142'] = 24;
	$airport['40203'] = 24;
	$airport['40204'] = 24;
	$airport['40211'] = 24;
	$airport['40416'] = 24;
	$airport['40418'] = 24;
	$airport['40460'] = 24;
	$airport['40466'] = 24;
	$airport['40468'] = 24;
	$airport['40478'] = 24;
	$airport['40480'] = 24;
	$airport['40536'] = 24;
	$airport['40546'] = 24;
	$airport['40558'] = 24;
	$airport['40559'] = 24;
	$airport['40564'] = 24;
	$airport['40566'] = 24;
	$airport['40567'] = 24;
	$airport['40569'] = 24;
	$airport['40572'] = 24;
	$airport['40575'] = 24;
	$airport['41260'] = 24;
	$airport['100017'] = 24;
	$airport['42972'] = 24;
	$airport['831'] = 25;
	$airport['882'] = 25;
	$airport['892'] = 25;
	$airport['893'] = 25;
	$airport['894'] = 25;
	$airport['895'] = 25;
	$airport['896'] = 25;
	$airport['897'] = 25;
	$airport['899'] = 25;
	$airport['900'] = 25;
	$airport['2374'] = 25;
	$airport['2379'] = 25;
	$airport['2380'] = 25;
	$airport['2383'] = 25;
	$airport['2384'] = 25;
	$airport['2385'] = 25;
	$airport['2386'] = 25;
	$airport['2387'] = 25;
	$airport['2388'] = 25;
	$airport['2391'] = 25;
	$airport['2394'] = 25;
	$airport['2397'] = 25;
	$airport['2398'] = 25;
	$airport['2400'] = 25;
	$airport['2402'] = 25;
	$airport['14508'] = 25;
	$airport['14510'] = 25;
	$airport['14515'] = 25;
	$airport['14522'] = 25;
	$airport['14526'] = 25;
	$airport['14528'] = 25;
	$airport['14530'] = 25;
	$airport['14533'] = 25;
	$airport['14534'] = 25;
	$airport['14536'] = 25;
	$airport['14537'] = 25;
	$airport['14539'] = 25;
	$airport['14540'] = 25;
	$airport['14542'] = 25;
	$airport['14544'] = 25;
	$airport['14546'] = 25;
	$airport['14548'] = 25;
	$airport['14549'] = 25;
	$airport['14553'] = 25;
	$airport['14554'] = 25;
	$airport['14555'] = 25;
	$airport['14556'] = 25;
	$airport['14557'] = 25;
	$airport['14558'] = 25;
	$airport['14561'] = 25;
	$airport['14563'] = 25;
	$airport['14568'] = 25;
	$airport['14569'] = 25;
	$airport['14570'] = 25;
	$airport['14571'] = 25;
	$airport['14576'] = 25;
	$airport['14579'] = 25;
	$airport['14584'] = 25;
	$airport['14585'] = 25;
	$airport['14586'] = 25;
	$airport['40390'] = 25;
	$airport['43112'] = 25;
	$airport['43113'] = 25;
	$airport['43116'] = 25;
	$airport['1664'] = 26;
	$airport['1671'] = 26;
	$airport['1686'] = 26;
	$airport['3304'] = 26;
	$airport['3320'] = 26;
	$airport['3321'] = 26;
	$airport['3322'] = 26;
	$airport['3323'] = 26;
	$airport['3324'] = 26;
	$airport['4252'] = 26;
	$airport['43253'] = 29;
	$airport['2786'] = 29;
	$airport['3055'] = 29;
	$airport['20109'] = 29;
	$airport['2253'] = 33;
	$airport['2260'] = 33;
	$airport['10748'] = 33;
	$airport['14602'] = 33;
	$airport['14606'] = 33;
	$airport['2406'] = 35;
	$airport['2442'] = 35;
	$airport['2491'] = 35;
	$airport['14067'] = 40;
	$airport['2415'] = 41;
	$airport['2433'] = 41;
	$airport['2444'] = 41;
	$airport['2468'] = 41;
	$airport['2469'] = 41;
	$airport['2497'] = 41;
	$airport['2514'] = 41;
	$airport['8677'] = 41;
	$airport['10450'] = 41;
	$airport['10940'] = 41;
	$airport['42704'] = 41;
	$airport['42803'] = 41;
	$airport['41471'] = 41;
	$airport['42325'] = 41;
	$airport['42710'] = 41;
	$airport['2981'] = 42;
	$airport['3100'] = 42;
	$airport['3780'] = 42;
	$airport['3821'] = 42;
	$airport['3866'] = 42;
	$airport['3890'] = 42;
	$airport['4084'] = 42;
	$airport['42477'] = 42;
	$airport['110065'] = 42;
	$airport['110084'] = 44;
	$airport['2806'] = 46;
	$airport['3214'] = 46;
	$airport['3237'] = 46;
	$airport['4219'] = 46;
	$airport['14033'] = 46;
	$airport['14034'] = 46;
	$airport['14035'] = 46;
	$airport['14036'] = 46;
	$airport['2414'] = 47;
	$airport['2199'] = 48;
	$airport['2211'] = 48;
	$airport['2220'] = 48;
	$airport['3193'] = 48;
	$airport['3826'] = 48;
	$airport['3884'] = 48;
	$airport['3920'] = 48;
	$airport['4126'] = 48;
	$airport['13898'] = 48;
	$airport['14019'] = 48;
	$airport['102109'] = 48;
	$airport['1253'] = 50;
	$airport['1225'] = 51;
	$airport['1235'] = 51;
	$airport['1254'] = 51;
	$airport['1256'] = 51;
	$airport['13950'] = 51;
	$airport['1213'] = 52;
	$airport['971'] = 60;
	$airport['4272'] = 60;
	$airport['2519'] = 61;
	$airport['2522'] = 61;
	$airport['2531'] = 61;
	$airport['2535'] = 61;
	$airport['2538'] = 61;
	$airport['2539'] = 61;
	$airport['2542'] = 61;
	$airport['2546'] = 61;
	$airport['2548'] = 61;
	$airport['2550'] = 61;
	$airport['2556'] = 61;
	$airport['2559'] = 61;
	$airport['2562'] = 61;
	$airport['2563'] = 61;
	$airport['2564'] = 61;
	$airport['2566'] = 61;
	$airport['4203'] = 61;
	$airport['4273'] = 61;
	$airport['4274'] = 61;
	$airport['4275'] = 61;
	$airport['4280'] = 61;
	$airport['4281'] = 61;
	$airport['4282'] = 61;
	$airport['4283'] = 61;
	$airport['4284'] = 61;
	$airport['4286'] = 61;
	$airport['4313'] = 61;
	$airport['4314'] = 61;
	$airport['4315'] = 61;
	$airport['4317'] = 61;
	$airport['4318'] = 61;
	$airport['20055'] = 61;
	$airport['42825'] = 61;
	$airport['42824'] = 61;
	$airport['41627'] = 61;
	$airport['42297'] = 61;
	$airport['4649'] = 67;
	$airport['9224'] = 67;
	$airport['14183'] = 67;
	$airport['20074'] = 67;
	$airport['40354'] = 67;
	$airport['40361'] = 67;
	$airport['42821'] = 67;
	$airport['41464'] = 67;
	$airport['2842'] = 69;
	$airport['9115'] = 69;
	$airport['13889'] = 69;
	$airport['13902'] = 73;
	$airport['2231'] = 76;
	$airport['2235'] = 76;
	$airport['3892'] = 76;
	$airport['4012'] = 76;
	$airport['793'] = 78;
	$airport['795'] = 78;
	$airport['1855'] = 78;
	$airport['3756'] = 78;
	$airport['14222'] = 78;
	$airport['14596'] = 78;
	$airport['20013'] = 78;
	$airport['20064'] = 78;
	$airport['40439'] = 78;
	$airport['40518'] = 78;
	$airport['40528'] = 78;
	$airport['100618'] = 78;
	$airport['100619'] = 78;
	$airport['100622'] = 78;
	$airport['41782'] = 78;
	$airport['41956'] = 78;
	$airport['41962'] = 78;
	$airport['41966'] = 78;
	$airport['41969'] = 78;
	$airport['41972'] = 78;
	$airport['41973'] = 78;
	$airport['41974'] = 78;
	$airport['41977'] = 78;
	$airport['1347'] = 79;
	$airport['1348'] = 79;
	$airport['1349'] = 79;
	$airport['1350'] = 79;
	$airport['1351'] = 79;
	$airport['1352'] = 79;
	$airport['1354'] = 79;
	$airport['1355'] = 79;
	$airport['1356'] = 79;
	$airport['1357'] = 79;
	$airport['1358'] = 79;
	$airport['1360'] = 79;
	$airport['1361'] = 79;
	$airport['1362'] = 79;
	$airport['1364'] = 79;
	$airport['1365'] = 79;
	$airport['1366'] = 79;
	$airport['1367'] = 79;
	$airport['1368'] = 79;
	$airport['1369'] = 79;
	$airport['1370'] = 79;
	$airport['1371'] = 79;
	$airport['1372'] = 79;
	$airport['1373'] = 79;
	$airport['1374'] = 79;
	$airport['1376'] = 79;
	$airport['1380'] = 79;
	$airport['1383'] = 79;
	$airport['1384'] = 79;
	$airport['1385'] = 79;
	$airport['1386'] = 79;
	$airport['1388'] = 79;
	$airport['1389'] = 79;
	$airport['1390'] = 79;
	$airport['1391'] = 79;
	$airport['1392'] = 79;
	$airport['1394'] = 79;
	$airport['1396'] = 79;
	$airport['1397'] = 79;
	$airport['1398'] = 79;
	$airport['1399'] = 79;
	$airport['1400'] = 79;
	$airport['1401'] = 79;
	$airport['1402'] = 79;
	$airport['1403'] = 79;
	$airport['1404'] = 79;
	$airport['1405'] = 79;
	$airport['2913'] = 79;
	$airport['3183'] = 79;
	$airport['43179'] = 79;
	$airport['4599'] = 79;
	$airport['4691'] = 79;
	$airport['4715'] = 79;
	$airport['8697'] = 79;
	$airport['8824'] = 79;
	$airport['9044'] = 79;
	$airport['9724'] = 79;
	$airport['10368'] = 79;
	$airport['10370'] = 79;
	$airport['14266'] = 79;
	$airport['14270'] = 79;
	$airport['14271'] = 79;
	$airport['14272'] = 79;
	$airport['14273'] = 79;
	$airport['14274'] = 79;
	$airport['1375'] = 82;
	$airport['1719'] = 83;
	$airport['1722'] = 83;
	$airport['1725'] = 83;
	$airport['1730'] = 83;
	$airport['1743'] = 83;
	$airport['4783'] = 83;
	$airport['9004'] = 83;
	$airport['14213'] = 83;
	$airport['1640'] = 91;
	$airport['1641'] = 91;
	$airport['1645'] = 91;
	$airport['1653'] = 91;
	$airport['1656'] = 91;
	$airport['1660'] = 91;
	$airport['3729'] = 91;
	$airport['100662'] = 91;
	$airport['42843'] = 91;
	$airport['42847'] = 91;
	$airport['42845'] = 91;
	$airport['42849'] = 91;
	$airport['14642'] = 92;
	$airport['2006'] = 93;
	$airport['2009'] = 93;
	$airport['2109'] = 95;
	$airport['2111'] = 95;
	$airport['2113'] = 95;
	$airport['2116'] = 95;
	$airport['2118'] = 95;
	$airport['2124'] = 95;
	$airport['40407'] = 95;
	$airport['40408'] = 95;
	$airport['40409'] = 95;
	$airport['3187'] = 99;
	$airport['2014'] = 106;
	$airport['2028'] = 106;
	$airport['2029'] = 106;
	$airport['2068'] = 106;
	$airport['2069'] = 106;
	$airport['2092'] = 106;
	$airport['2099'] = 106;
	$airport['9466'] = 106;
	$airport['13984'] = 106;
	$airport['42119'] = 106;
	$airport['2287'] = 108;
	$airport['42301'] = 108;
	$airport['42910'] = 108;
	$airport['42979'] = 108;
	$airport['42980'] = 108;
	$airport['2571'] = 110;
	$airport['4378'] = 110;
	$airport['9270'] = 111;
	$airport['9271'] = 111;
	$airport['13992'] = 111;
	$airport['13993'] = 111;
	$airport['20158'] = 112;
	$airport['1285'] = 113;
	$airport['1332'] = 113;
	$airport['3438'] = 113;
	$airport['4571'] = 113;
	$airport['4572'] = 113;
	$airport['4573'] = 113;
	$airport['10299'] = 113;
	$airport['14365'] = 113;
	$airport['14458'] = 113;
	$airport['43833'] = 113;
	$airport['1283'] = 114;
	$airport['1287'] = 114;
	$airport['1315'] = 114;
	$airport['1346'] = 114;
	$airport['3435'] = 114;
	$airport['4574'] = 114;
	$airport['14369'] = 114;
	$airport['14447'] = 114;
	$airport['14600'] = 114;
	$airport['3120'] = 116;
	$airport['2186'] = 117;
	$airport['2191'] = 117;
	$airport['2192'] = 117;
	$airport['2194'] = 117;
	$airport['11451'] = 117;
	$airport['3054'] = 120;
	$airport['13976'] = 120;
	$airport['14001'] = 120;
	$airport['41235'] = 120;
	$airport['13982'] = 121;
	$airport['13983'] = 121;
	$airport['20078'] = 125;
	$airport['2309'] = 132;
	$airport['2316'] = 132;
	$airport['2322'] = 132;
	$airport['2333'] = 132;
	$airport['2347'] = 132;
	$airport['2353'] = 132;
	$airport['2354'] = 132;
	$airport['2359'] = 132;
	$airport['2363'] = 132;
	$airport['3482'] = 132;
	$airport['40337'] = 132;
	$airport['42615'] = 132;
	$airport['1995'] = 133;
	$airport['4410'] = 133;
	$airport['43790'] = 133;
	$airport['2573'] = 135;
	$airport['2577'] = 135;
	$airport['2581'] = 135;
	$airport['3765'] = 135;
	$airport['4697'] = 135;
	$airport['9672'] = 135;
	$airport['10154'] = 135;
	$airport['14194'] = 135;
	$airport['14293'] = 135;
	$airport['40834'] = 135;
	$airport['40840'] = 135;
	$airport['40971'] = 135;
	$airport['40982'] = 135;
	$airport['9919'] = 146;
	$airport['14149'] = 146;
	$airport['14152'] = 146;
	$airport['14167'] = 146;
	$airport['2785'] = 153;
	$airport['2977'] = 153;
	$airport['2978'] = 153;
	$airport['3017'] = 153;
	$airport['3461'] = 153;
	$airport['3851'] = 153;
	$airport['4046'] = 153;
	$airport['4054'] = 153;
	$airport['3895'] = 156;
	$airport['3000'] = 157;
	$airport['3022'] = 157;
	$airport['3941'] = 157;
	$airport['9974'] = 162;
	$airport['14629'] = 162;
	$airport['2919'] = 164;
	$airport['2805'] = 166;
	$airport['2807'] = 166;
	$airport['3093'] = 166;
	$airport['3148'] = 166;
	$airport['3202'] = 166;
	$airport['3291'] = 166;
	$airport['3465'] = 166;
	$airport['4218'] = 166;
	$airport['8931'] = 166;
	$airport['14231'] = 166;
	$airport['14232'] = 166;
	$airport['14233'] = 166;
	$airport['14234'] = 166;
	$airport['14255'] = 166;
	$airport['14256'] = 166;
	$airport['14258'] = 166;
	$airport['14259'] = 166;
	$airport['14263'] = 166;
	$airport['3171'] = 167;
	$airport['3227'] = 167;
	$airport['4589'] = 167;
	$airport['4590'] = 167;
	$airport['4591'] = 167;
	$airport['4593'] = 167;
	$airport['4594'] = 167;
	$airport['4595'] = 167;
	$airport['4596'] = 167;
	$airport['4597'] = 167;
	$airport['4598'] = 167;
	$airport['4445'] = 168;
	$airport['14301'] = 168;
	$airport['14302'] = 168;
	$airport['931'] = 169;
	$airport['2835'] = 169;
	$airport['3007'] = 169;
	$airport['3008'] = 169;
	$airport['3027'] = 169;
	$airport['3084'] = 169;
	$airport['3163'] = 169;
	$airport['3212'] = 169;
	$airport['3247'] = 169;
	$airport['3248'] = 169;
	$airport['3264'] = 169;
	$airport['3267'] = 169;
	$airport['3268'] = 169;
	$airport['3277'] = 169;
	$airport['4817'] = 169;
	$airport['8699'] = 169;
	$airport['14257'] = 169;
	$airport['14260'] = 169;
	$airport['14262'] = 169;
	$airport['14264'] = 169;
	$airport['43553'] = 169;
	$airport['947'] = 170;
	$airport['1387'] = 170;
	$airport['2759'] = 170;
	$airport['2781'] = 170;
	$airport['2782'] = 170;
	$airport['2795'] = 170;
	$airport['2822'] = 170;
	$airport['2897'] = 170;
	$airport['2899'] = 170;
	$airport['2926'] = 170;
	$airport['3005'] = 170;
	$airport['3014'] = 170;
	$airport['3043'] = 170;
	$airport['3106'] = 170;
	$airport['3119'] = 170;
	$airport['3201'] = 170;
	$airport['4423'] = 170;
	$airport['4683'] = 170;
	$airport['8702'] = 170;
	$airport['8703'] = 170;
	$airport['41635'] = 170;
	$airport['1411'] = 173;
	$airport['1414'] = 173;
	$airport['1433'] = 173;
	$airport['1441'] = 173;
	$airport['1443'] = 173;
	$airport['1452'] = 173;
	$airport['3469'] = 173;
	$airport['4205'] = 173;
	$airport['4207'] = 173;
	$airport['4210'] = 173;
	$airport['4216'] = 173;
	$airport['42486'] = 182;
	$airport['43751'] = 182;
	$airport['13092'] = 182;
	$airport['13093'] = 182;
	$airport['13094'] = 182;
	$airport['13121'] = 182;
	$airport['13122'] = 182;
	$airport['13124'] = 182;
	$airport['13131'] = 182;
	$airport['13145'] = 182;
	$airport['13146'] = 182;
	$airport['13191'] = 182;
	$airport['13195'] = 182;
	$airport['13206'] = 182;
	$airport['13212'] = 182;
	$airport['13296'] = 182;
	$airport['13299'] = 182;
	$airport['13359'] = 182;
	$airport['13368'] = 182;
	$airport['13382'] = 182;
	$airport['13440'] = 182;
	$airport['13441'] = 182;
	$airport['13445'] = 182;
	$airport['13447'] = 182;
	$airport['13450'] = 182;
	$airport['13509'] = 182;
	$airport['13512'] = 182;
	$airport['13523'] = 182;
	$airport['13525'] = 182;
	$airport['13761'] = 182;
	$airport['13779'] = 182;
	$airport['13825'] = 182;
	$airport['13826'] = 182;
	$airport['13832'] = 182;
	$airport['13845'] = 182;
	$airport['13847'] = 182;
	$airport['13850'] = 182;
	$airport['13851'] = 182;
	$airport['13853'] = 182;
	$airport['14658'] = 182;
	$airport['20157'] = 182;
	$airport['20194'] = 182;
	$airport['20199'] = 182;
	$airport['20200'] = 182;
	$airport['20201'] = 182;
	$airport['20202'] = 182;
	$airport['20203'] = 182;
	$airport['20206'] = 182;
	$airport['20208'] = 182;
	$airport['20213'] = 182;
	$airport['20215'] = 182;
	$airport['20219'] = 182;
	$airport['20220'] = 182;
	$airport['20222'] = 182;
	$airport['20225'] = 182;
	$airport['20226'] = 182;
	$airport['20227'] = 182;
	$airport['20229'] = 182;
	$airport['20230'] = 182;
	$airport['20231'] = 182;
	$airport['20267'] = 182;
	$airport['43757'] = 182;
	$airport['43755'] = 182;
	$airport['41058'] = 182;
	$airport['41093'] = 182;
	$airport['42449'] = 182;
	$airport['43756'] = 182;
	$airport['42242'] = 182;
	$airport['42684'] = 182;
	$airport['43758'] = 182;
	$airport['43766'] = 182;
	$airport['43773'] = 182;
	$airport['43775'] = 182;
	$airport['43776'] = 182;
	$airport['43777'] = 182;
	$airport['12789'] = 184;
	$airport['12898'] = 184;
	$airport['12971'] = 184;
	$airport['12975'] = 184;
	$airport['13307'] = 184;
	$airport['13373'] = 184;
	$airport['13387'] = 184;
	$airport['13401'] = 184;
	$airport['13424'] = 184;
	$airport['13481'] = 184;
	$airport['13496'] = 184;
	$airport['13582'] = 184;
	$airport['13583'] = 184;
	$airport['13689'] = 184;
	$airport['13719'] = 184;
	$airport['13724'] = 184;
	$airport['13725'] = 184;
	$airport['13796'] = 184;
	$airport['42568'] = 184;
	$airport['42966'] = 184;
	$airport['12773'] = 186;
	$airport['42675'] = 186;
	$airport['42685'] = 190;
	$airport['41577'] = 200;
	$airport['42446'] = 203;
	$airport['43765'] = 203;
	$airport['40201'] = 210;
	$airport['20093'] = 211;
	$airport['42674'] = 211;
	
	foreach ($airport as $property_legacy_id => $airport_legacy_id) {
		$sql = "select entity_id from field_data_field_legacy_id ";
		$sql .= "where entity_type = 'node' and bundle = 'property' and field_legacy_id_value = :legacy_id";
		$stmt = db_query($sql, array("legacy_id" => "property_$property_legacy_id"));
		$hr = $stmt->fetchAssoc();
		$property_nid = $hr['entity_id'];
		if (!$property_nid) { print "Unable to find property $property_legacy_id\n"; continue; }

		$sql = "select entity_id from field_data_field_legacy_id ";
		$sql .= "where entity_type = 'taxonomy_term' and bundle = 'airport' and field_legacy_id_value = :legacy_id";
		$stmt = db_query($sql, array("legacy_id" => $airport_legacy_id));
		$hr = $stmt->fetchAssoc();
		$airport_tid = $hr['entity_id'];
		if (!$airport_tid) { print "Unable to find airport $airport_legacy_id\n"; continue; }

		print "Found $property_nid, $airport_tid\n";
		
		$node = node_load($property_nid);
		$node->field_airport['und'][0]['tid'] = $airport_tid;
		node_save($node);
	}
}



function unite_geocode_batch() {
	$last_nid = variable_get('unite_geocode_last_nid', 0);
	
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
	$query->entityCondition('bundle', 'property');
	$query->propertyCondition('nid', $last_nid, '>');
	$query->fieldCondition('field_address', 'postal_code', '%', 'like');
	$query->range(0, 100);
	$result = $query->execute();
	if (!$result['node']) { 
		print "No more nodes.";
		exit();
	}
	
	$nids = array_keys($result['node']);
	foreach ($nids as $nid) {
		$node = node_load($nid);
		variable_set('unite_geocode_last_nid', $nid);
		print "Examining $nid " . $node->title . " ... \n";
		
		if ($node->field_address_geofield['und'][0]['lat']) { 
			print "... Has a latitude.  Skipping.\n";
			continue;
		}
		
		node_save($node);
		print "... Saved.\n";
	}
}