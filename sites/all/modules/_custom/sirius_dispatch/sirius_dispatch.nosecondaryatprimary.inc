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
		$employer_node = node_load($employer_nid);

		// The company
		$company_nid = $employer_node->field_grievance_company['und'][0]['target_id'];

		// All the employers at that company
		$employer_nids = array();
		if ($company_nid) {
			$sql = "select nid from node ";
			$sql .= "left join field_data_field_grievance_company on field_data_field_grievance_company.entity_type = 'node' and field_data_field_grievance_company.entity_id = node.nid ";
			$sql .= "where node.type = 'grievance_shop' ";
			$sql .= "and field_grievance_company_target_id = :company_nid ";
			$stmt = sirius_sql_query($sql, array(':company_nid' => $company_nid));
			while ($hr = $stmt->fetchAssoc()) {
				$employer_nids[] = $hr['nid'];
			}
		} else {
			$employer_nids = array($employer_nid);
		}

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
}