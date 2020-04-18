<?php

/**
 * @todo: This is not transactional! It is theoretically possible to give two people the same lock.
 */

function sirius_lock_release_by_ts() {
	$ts = time() - 60*60*4;
	$sql = "delete from sirius_lock where ts < :ts";
	$stmt = db_query($sql, array(':ts' => $ts));
}

function sirius_lock_check($type, $id) {
	$sql = "select uid, ts from sirius_lock where type = :type and id = :id";
	$stmt = db_query($sql, array(':type' => $type, ':id' => $id));
	$hr = $stmt->fetchAssoc();
	return $hr;
}

function sirius_lock_refresh($type, $id) {
	$ts = time();
	$sql = "update sirius_lock set ts = :ts where type = :type and id = :id";
	$stmt = db_query($sql, array(':ts' => $ts, ':type' => $type, ':id' => $id));
}

function sirius_lock_create($type, $id, $uid) {
	$ts = time();
	$sql = "insert into sirius_lock (id, type, uid, ts) values (:id, :type, :uid, :ts)";
	$stmt = db_query($sql, array(':id' => $id, ':type' => $type, ':uid' => $uid, ':ts' => $ts));
}

function sirius_lock_release_by_id($type, $id) {
	$sql = "delete from sirius_lock where type = :type and id = :id";
	$stmt = db_query($sql, array(':type' => $type, ':id' => $id));
}

function sirius_lock_release_by_uid($type, $uid = NULL) {
	if (!$uid) {
		global $user;
		$uid = $user->uid;
	}
	$sql = "delete from sirius_lock where type = :type and uid = :uid";
	$stmt = db_query($sql, array(':type' => $type, ':uid' => $uid));
}

function sirius_lock_obtain($type, $id, $uid = NULL) {
	if (!$uid) {
		global $user;
		$uid = $user->uid;
	}

	sirius_lock_release_by_ts();
	$check = sirius_lock_check($type, $id);
	if (!$check) {
		sirius_lock_release_by_uid($type, $uid);
		sirius_lock_create($type, $id, $uid);
		return array('success' => TRUE, 'msg' => 'Lock created');
	} else if ($check['uid'] == $uid) {
		sirius_lock_refresh($type, $id);
		return array('success' => TRUE, 'msg' => 'Lock updated');
	} else {
		return array('success' => FALSE, 'msg' => 'There is an existing lock');
	}
}
