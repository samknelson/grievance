<?php

// drush --user=1 php-eval "sirius_require('sirius_smf', 'sirius_smf.oneoff_progeny_election.inc'); Sirius_Smf_Batchrunner_Oneoff_Progeny_Election::getInstance()->cl('test', 2);"

sirius_require('sirius', 'Sirius/Batchrunner.inc');
sirius_require('sirius', 'Sirius/Batchrunner_Node.inc');

class Sirius_Smf_Batchrunner_Oneoff_Progeny_Election extends Sirius_Batchrunner_Node {
	public $type = 'smf_oneoff_progeny_election';
	public $name = 'Sirius_Smf_Oneoff_Progeny_Election';

	public function content_type_impl(&$context = array()) {
		return 'sirius_trust_worker_election';
	}

	public function process_record_impl($election_node, &$context = array()) {
		$lookups = &drupal_static(__FUNCTION__);

		if (!isset($lookups['progeny'])) {
			$lookups['progeny'] = sirius_find_nid_by_id('PROGYNY', 'sirius_trust_benefit');
		}
		if (!$lookups['progeny']) {
			sirius_minilog("Didn't find benefit NID for progenyn.", 'error');
			return;
		}

		if ($election_node->field_sirius_active['und'][0]['value'] != 'Yes') {
			sirius_minilog("Election isn't active.");
			return array('success' => TRUE);
		}

		if (in_array($lookups['progeny'], sirius_fieldvals($election_node, 'field_sirius_trust_benefits', 'target_id'))) {
			sirius_minilog("Worker already has progeny.");
			return array('success' => TRUE);
		}

		$election_node->field_sirius_trust_benefits['und'][] = array('target_id' => $lookups['progeny']);
		if ($this->mode($context) == 'live') {
			node_save($election_node);
			sirius_minilog("Election updated.");
		} else {
			sirius_minilog("(Skipping save because mode = " . $this->mode($context) . ')');
		}
		return array("success" => TRUE, "changed" => TRUE);
	}











	public function get_next_batch(&$context = array()) {
		$lookups = &drupal_static(__FUNCTION__);
		if (!isset($lookups['mlk'])) {
			$lookups['mlk'] = sirius_find_nid_by_id('M', 'sirius_trust_benefit');
		}
		if (!$lookups['mlk']) {
			sirius_minilog("Didn't find benefit NID for MLK (M).", 'error');
			return;
		}


		$batch_size = $this->batch_size($context);
		$last_pk = $this->last_pk($context);
		$content_type = $this->content_type_impl($context);
		if (!$content_type) { return array(); }

		$sql = "select distinct nid from node ";
		$sql .= "left join field_data_field_sirius_active on field_data_field_sirius_active.entity_type = 'node' and field_data_field_sirius_active.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_trust_benefits on field_data_field_sirius_trust_benefits.entity_type = 'node' and field_data_field_sirius_trust_benefits.entity_id = node.nid ";
		$sql .= "where type = :content_type ";
		$sql .= "and nid > :last_pk ";
		$sql .= "and field_sirius_active_value = 'Yes' ";
		$sql .= "and field_sirius_trust_benefits_target_id = :mlk_nid ";
		$sql .= "order by nid limit $batch_size";
		$stmt = db_query($sql, array(':content_type' => $content_type, ':last_pk' => $last_pk, ':mlk_nid' => $lookups['mlk']));

		$pks = array();
		while ($hr = $stmt->fetchAssoc()) {
			$pks[] = $hr['nid'];
		}
		return $pks;
	}

	public function grand_total_pending(&$context = array()) {
		$lookups = &drupal_static(__FUNCTION__);
		if (!isset($lookups['mlk'])) {
			$lookups['mlk'] = sirius_find_nid_by_id('M', 'sirius_trust_benefit');
		}
		if (!$lookups['mlk']) {
			sirius_minilog("Didn't find benefit NID for MLK (M).", 'error');
			return;
		}

		$content_type = $this->content_type_impl($context);
		if (!$content_type) { return 0; }

		$last_pk = $this->last_pk($context);
		$sql = "select count(distinct nid) as c from node ";
		$sql .= "left join field_data_field_sirius_active on field_data_field_sirius_active.entity_type = 'node' and field_data_field_sirius_active.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_trust_benefits on field_data_field_sirius_trust_benefits.entity_type = 'node' and field_data_field_sirius_trust_benefits.entity_id = node.nid ";
		$sql .= "where type = :content_type ";
		$sql .= "and nid > :last_pk ";
		$sql .= "and field_sirius_active_value = 'Yes' ";
		$sql .= "and field_sirius_trust_benefits_target_id = :mlk_nid ";
		$stmt = db_query($sql, array(':content_type' => $content_type, ':last_pk' => $last_pk, ':mlk_nid' => $lookups['mlk']));
		$hr = $stmt->fetchAssoc();
		return $hr['c'];
	}




}
