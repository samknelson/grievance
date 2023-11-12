<?php

class Sirius_Dispatch_Nosecondaryatprimary extends Sirius_Dispatch_Elig_Plugin {
  public $type = 'nosecondaryatprimary';
  public $name = 'No Secondary at Primary';

	public function elig($context, $workers) {
		// The job
		$job_node = $context['job_node'];
		if (!$job_node) { return array('success' => FALSE, 'msg' => 'No job node.'); }

		// The employer
		$employer_nid = $job_node->field_grievance_shop['und'][0]['target_id'];
		if (!$employer_nid) { return array('success' => FALSE, 'msg' => 'No employer.'); }
		$employer_nids = $this->_employer_nids_at_this_company($employer_nid);
		if (!$employer_nids) { return array('success' => FALSE, 'msg' => 'No employers.'); }

		$inelig_workers = array();
		foreach ($workers as $worker_nid => $ignore) {
			// Get the worker's current employer. SQL to avoid node_load()
			$sql = "select field_grievance_shop_target_id as employer_nid ";
			$sql .= "from field_data_field_grievance_shop ";
			$sql .= "where entity_type = 'node' and entity_id = :worker_nid ";
			$stmt = sirius_sql_query($sql, array(':worker_nid' => $worker_nid));
			$hr = $stmt->fetchAssoc();
			if (!$hr['employer_nid']) { continue; }
			if (!in_array($hr['employer_nid'], $employer_nids)) { continue; }

			// The worker's current employer is in the same company as the job's employer
			$inelig_workers[$worker_nid] = array('details' => 'Already employed at this employer or at another employer in the same company.');
		}

		return array('success' => TRUE, 'inelig_workers' => $inelig_workers);
	}

	/**
	 * Plugin callback: Accept a dispatch
	 *
	 * Lay off any secondary dispatches at the same company.
	 */

	public function accept(&$dispatch_node, $notes = '') {
		// Only for "accepted" dispatches
		if ($dispatch_node->field_sirius_dispatch_status['und'][0]['value'] != 'accepted') { return; }

		$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
		if (!$job_node) { return array('success' => TRUE); }

		$employer_nid = $job_node->field_grievance_shop['und'][0]['target_id'];
		if (!$employer_nid) { return array('success' => TRUE); }

		$employer_nids = $this->_employer_nids_at_this_company($employer_nid);
		if (!$employer_nids) { return array('success' => TRUE); }

		$worker_nid = $dispatch_node->field_sirius_worker['und'][0]['target_id'];
		if (!$worker_nid) { return array('success' => TRUE); }

		// Find any secondary dispatches at any of these employers
		$params = array();
		$params['worker_nid'] = $worker_nid;
		$params['status'] = 'accepted_secondary';
		$layoff_dispatch_nodes = sirius_dispatch_search($params);

		// Just to be on the safe side, make sure we don't lay off the dispatch we're about to accept. 
		unset($layoff_dispatch_nodes[$dispatch_node->nid]);

		if (!$layoff_dispatch_nodes) { return array('success' => TRUE); }

		// Layoff any secondary dispatches
		foreach ($layoff_dispatch_nodes as $layoff_dispatch_nid => $layoff_dispatch_node) {

			$result = sirius_dispatch_terminate(
				$layoff_dispatch_node, 
				'Laying off secondary dispatch because a primary dispatch at the same company has been accepted.'
			);

			if (!$result['success']) {
				$result['msg'] = "Failed to layoff secondary dispatch: " . $result['msg']; 
				return $result;
			}
		}

		return array('success' => TRUE);
	}

  private function _employer_nids_at_this_company($employer_nid) {
		$employer_node = node_load($employer_nid);

		// The company
		$company_nid = $employer_node->field_grievance_company['und'][0]['target_id'];
		if (!$company_nid) { return array($employer_nid); }

		// All the employers at that company
		$employer_nids = array();
		$sql = "select nid from node ";
		$sql .= "left join field_data_field_grievance_company on field_data_field_grievance_company.entity_type = 'node' and field_data_field_grievance_company.entity_id = node.nid ";
		$sql .= "where node.type = 'grievance_shop' ";
		$sql .= "and field_grievance_company_target_id = :company_nid ";
		$stmt = sirius_sql_query($sql, array(':company_nid' => $company_nid));
		while ($hr = $stmt->fetchAssoc()) {
			$employer_nids[] = $hr['nid'];
		}

		return $employer_nids;
  }
}