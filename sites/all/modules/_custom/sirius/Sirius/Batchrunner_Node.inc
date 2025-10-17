<?php

class Sirius_Batchrunner_Node extends Sirius_Batchrunner {
	public $type = 'batchrunner_node';
	public $name = 'Batchrunner Node';

	public function record_pk($record) {
		return $record->nid;
	}

	public function record_title($record) {
		return $record->title . ' (' . $this->record_pk($record) . ')';
	}

	public function record_load($pk, &$context = array()) {
		return node_load($pk);
	}

	public function get_next_batch(&$context = array()) {
		$batch_size = $this->batch_size($context);
		$last_pk = $this->last_pk($context);
		$content_type = $this->content_type_impl($context);
		if (!$content_type) { return array(); }

		$sql = "select nid from node where type = :content_type and nid > :last_pk order by nid limit $batch_size";
		$stmt = sirius_sql_query($sql, array(':content_type' => $content_type, ':last_pk' => $last_pk));

		$pks = array();
		while ($hr = $stmt->fetchAssoc()) {
			$pks[] = $hr['nid'];
		}
		return $pks;
	}

	public function grand_total_pending(&$context = array()) {
		$content_type = $this->content_type_impl($context);
		if (!$content_type) { return 0; }

		$last_pk = $this->last_pk($context);
		$sql = "select count(*) as c from node where type = :content_type and nid > :last_pk ";
		$stmt = sirius_sql_query($sql, array(':content_type' => $content_type, ':last_pk' => $last_pk));
		$hr = $stmt->fetchAssoc();
		return $hr['c'];
	}
}
