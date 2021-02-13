<?php

function sirius_quickhash_set($key, $json, $type = NULL) {
	global $user;

	$sql_args[':key'] = $key;
	$sql_args[':json'] = json_encode($json);
	$sql_args[':type'] = $type;
	$sql_args[':domain'] = sirius_domain_nid();
	$sql_args[':ts'] = time();
	$sql_args[':uid'] = $user->uid;

	$sql = "insert into sirius_quickhash ";
	$sql .= "(hash_key, hash_json, hash_type, hash_domain, hash_uid, hash_ts) ";
	$sql .= "values ";
	$sql .= "(:key, :json, :type, :domain, :uid, :ts) ";
	$sql .= "on duplicate key update ";
	$sql .= "hash_json = :json, hash_type = :type, hash_domain = :domain, hash_uid = :uid, hash_ts = :ts";
	$stmt = sirius_sql_query($sql, $sql_args);
}

