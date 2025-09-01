<?php

class Sirius_Trust_Qmsco extends Sirius_Singleton {

	public function tid() {
    $tid = &drupal_static(__FUNCTION__);

    if (!isset($tid)) {
    	$id = 'QMSCO';
      $tid = sirius_find_tid_by_id($id, 'sirius_contact_relationship_types');

	    if (!$tid) {
	    	drupal_set_message("QMSCO relationship type not found. Please make sure that there is a relationship type with the id [$id].", 'error');
	    }
    }

    return $tid;
	}

	public function rp_tid() {
    $tid = &drupal_static(__FUNCTION__);

    if (!isset($tid)) {
    	$id = 'RP';
      $tid = sirius_find_tid_by_id($id, 'sirius_contact_relationship_types');

	    if (!$tid) {
	    	drupal_set_message("QMSCO RP relationship type not found. Please make sure that there is a relationship type with the id [$id].", 'error');
	    }
    }

    return $tid;
	}

	public function rp_relationship_nodes($qmsco_worker_node) {
		$contact_nid = $qmsco_worker_node->field_sirius_contact['und'][0]['target_id'];
		if (!$contact_nid) { return NULL; }

		$rp_tid = $this->rp_tid();
		if (!$rp_tid) { return NULL; }

		$relationship_nodes = sirius_contact_relationships(
			array(
				'contact_alt_nid' => $contact_nid,
				'reltype_tid' => $rp_tid,
				'active' => 'Yes',
			)
		);

		if (!$relationship_nodes) { return array(); }

		return $relationship_nodes;
	}

	public function rp_relationship_node($qmsco_worker_node) {
		return reset($this->rp_relationship_nodes($qmsco_worker_node));
	}

	public function rp_worker_node($qmsco_worker_node) {
		$relationship_node = $this->rp_relationship_node($qmsco_worker_node);
		if (!$relationship_node) { return NULL; }
		return sirius_worker_get_by_contact_nid($relationship_node->field_sirius_contact['und'][0]['target_id']);
	}
}