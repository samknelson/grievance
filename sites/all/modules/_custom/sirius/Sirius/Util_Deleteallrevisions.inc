<?php

sirius_require('sirius', 'Sirius/Batchrunner.inc');
sirius_require('sirius', 'Sirius/Batchrunner_Node.inc');

class Sirius_Util_Deleteallrevisions extends Sirius_Batchrunner {
	public $type = 'util_deleteallrevisions';
	public $name = 'Util - Delete All Revisions';

	public function batch_size_default(&$context = array()) { return 10000; }

	/**
	 * We don't use last_pk
	 */

	public function last_pk(&$context = array()) {
		return 0; 
	}

	public function last_pk_set($pk, &$context = array()) {
		return 0;
	}

	/**
	 * We never load the record. The record is the pk is the vid.
	 */

	public function record_pk($vid) {
		return $vid;
	}

	public function record_title($vid) {
		return $vid;
	}

	public function get_next_batch(&$context = array()) {
		$batch_size = $this->batch_size($context);

		$sql = "SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;";
		$stmt = db_query($sql, array());

		$sql = "select node_revision.vid ";
		$sql .= "from node, node_revision ";
		$sql .= "where node.nid = node_revision.nid ";
		$sql .= "and node.vid != node_revision.vid ";
		$sql .= "order by node_revision.vid limit $batch_size ";
		$stmt = db_query($sql, array());

		$vids = array();
		while ($hr = $stmt->fetchAssoc()) {
			$vids[] = $hr['vid'];
		}

		$sql = "SET TRANSACTION ISOLATION LEVEL REPEATABLE READ ;";
		$stmt = db_query($sql, array());

		return $vids;
	}

	public function grand_total_pending(&$context = array()) {
		$sql = "select count(*) as c from node";
		$stmt = sirius_sql_query($sql, array());
		$hr = $stmt->fetchAssoc();
		$node_count = $hr['c'];

		$sql = "select count(*) as c from node_revision";
		$stmt = sirius_sql_query($sql, array());
		$hr = $stmt->fetchAssoc();
		$revision_count = $hr['c'];

		return $revision_count - $node_count;
	}

	public function record_load($vid, &$context = array()) {
		return $vid;
	}

	public function process_record_impl($vid, &$context = array()) {
		$mode = $this->mode($context);
		if ($mode != 'live') {
			sirius_minilog("(Skipping delete because mode = $mode)");
			return;
		}
		return node_revision_delete($vid);
	}
}
