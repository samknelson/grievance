<?php

/**
 * Plugin callback: Settings
 */

class Sirius_Dispatch_1for1 extends Sirius_Dispatch_Elig_Plugin {
  public $type = '1for1';
  public $name = 'One-for-One';

	public function settings() {
		$form['notify'] = array(
			'#title' => 'Notify Replaced Worker?',
			'#type' => 'select',
			'#options' => array(
				NULL => t('-- Select --'),
				'No' => t('No'),
				'Yes' => t('Yes'),
			),
			'#description' => t('If "Yes", then a notification will be sent to the replaced worker.'),
			'#id' => 'sirius_dispatch_1for1_notify'
		);

		$form['email_subject'] = array(
			'#title' => 'Email Subject',
			'#type' => 'textfield',
		);
		$form['email_body'] = array(
			'#title' => 'Email Body',
			'#type' => 'text_format',
		);
		$form['sms_body'] = array(
			'#title' => 'SMS',
			'#type' => 'textarea',
		);

		return $form;
	}

	/**
	 * Plugin callback: Accept a dispatch
	 *
	 * - Find the next worker with status "ready"
	 * - Replace them
	 * - Notify the replaced worker
	 * If there is no "ready" worker, then return a failure.
	 */

	public function accept(&$dispatch_node, $notes = '') {
		$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
		$job_policy = sirius_dispatch_job_policy($job_node);
		$worker_nid = $dispatch_node->field_sirius_worker['und'][0]['target_id'];
		$data = $this->for1_data($job_node);

		// First try to use the extra slots
		$result = $this->extraslots_replace($job_node, $dispatch_node);
		if ($result['success']) {
			return $result;
		}

		// Replace the next "ready" worker
		$found = FALSE;
		foreach ($data as $candidate => $record) {
			if ($record['status'] == 'ready') {
				$data[$candidate]['status'] = 'replaced';
				$data[$candidate]['replaced_by'] = $worker_nid;
				$data[$candidate]['replaced_ts'] = time();
				$found = TRUE;
				break;
			} else if ($record['status'] == 'ready' && $record['replaced_by'] == $worker_nid) {
				$found = TRUE;
				break;
			}
		}

		// No "ready" worker found. Cannot accept the dispatch.
		if (!$found) {
			return array('success' => FALSE, 'msg' => 'There is no worker available to be replaced.');
		}

		// Notify the worker.
		$settings = $job_policy['plugin']['1for1']['settings'];
		if ($settings['notify'] == 'Yes') {
			$worker_node = node_load($worker_nid);
			$subject = $settings['email_subject'];
			$body = sirius_richtext_handle_input($settings['email_body']);
			$sms = $settings['sms_body'];
			sirius_worker_notify(
				$worker_node, 
				array(
					'subject' => $subject,
					'body' => $body,
					'sms' => $sms,
				)
			);
		}

		// Save our data
		$this->for1_data_set($job_node, $data);

		return array('success' => TRUE);
	}

	/**
	 * List of statuses for one-for-one workers
	 */

	public function for1_statuses() {
		return array(
			'ready' => 'Ready',
			'replaced' => 'Replaced',
		);
	}

	/**
	 * Load the list of workers to be replaced (or already replaced)
	 */

	public function for1_data($job_node) {
		$json = sirius_json_get($job_node);

		$data = $json['dispatch']['1for1']['data'];
		if (!$data) { $data = array(); }

		uksort($data, function($a, $b) use ($data) {
			if ($data[$a]['status'] == 'ready' && $data[$b]['status'] == 'replaced') { return -1; }
			if ($data[$a]['status'] == 'replaced' && $data[$b]['status'] == 'ready') { return 1; }
			return strcmp($data[$a]['name'], $data[$b]['name']);
		});

		return $data;
	}

	public function for1_data_set($job_node, $data) {
		$json = sirius_json_get($job_node);
		$json['dispatch']['1for1']['data'] = $data;
		sirius_json_set($job_node, $json);
		node_save($job_node);
	}

	public function for1_add_by_id($job_node, $worker_id) {
		$worker_node = sirius_worker_get_by_id($worker_id);
		if (!$worker_node) { return array('success' => FALSE, 'msg' => "The worker with ID $worker_id was not found."); }
		return $this->for1_add($job_node, $worker_node);
	}

	/**
	 * "Add" a worker to the job, marking them as "ready for replacement".
	 */

	public function for1_add($job_node, $worker_node) {
		// Must be currently employed
		// @todo: Should this include secondary dispatches?
		if ($worker_node->field_grievance_shop['und'][0]['target_id'] != $job_node->field_grievance_shop['und'][0]['target_id']) {
			return array('success' => FALSE, 'msg' => 'Worker ' . $worker_node->title . ' is not currently employed by the employer who created this job.');
		}

		$data = $this->for1_data($job_node);
		if ($data[$worker_node->nid]) {
			return array('succss' => FALSE, 'msg' => 'Worker ' . $worker_node->title . ' is already listed for replacement.');
		}

		$data[$worker_node->nid] = array(
			'added_ts' => time(),
			'status' => 'ready',
		);

		$this->for1_data_set($job_node, $data);

		return array('success' => TRUE, 'msg' => 'Worker ' . $worker_node->title . ' has been added.');
	}

	public function for1_remove($job_node, $worker_node, $force = FALSE) {
		$data = $this->for1_data($job_node);
		if (!$data[$worker_node->nid]) {
			return array('succss' => FALSE, 'msg' => 'Worker ' . $worker_node->title . ' has not been added.');
		}

		if (!$force) {
			if ($data[$worker_node->nid]['status'] != 'ready') {
				return array('succss' => FALSE, 'msg' => 'Worker ' . $worker_node->title . ' does not have the status "ready". They have probably already been replaced.');
			}
		}

		unset($data[$worker_node->nid]);
		$this->for1_data_set($job_node, $data);
		return array('success' => TRUE, 'msg' => 'Worker ' . $worker_node->title . ' has been removed.');
	}













	public function extraslots_set_total($job_node, $total) {
		if ($total === '' || $total === null) { $total = 0; }
	
		if (!is_numeric($total) || ((int)$total != $total) || ($total < 0) || ($total > 10) ) {
			return array('success' => FALSE, 'msg' => 'Please enter an integer between 0 and 10.');
		}

		$job_json = sirius_json_get($job_node);
		$job_json['1for1']['extraslots']['total'] = $total;
		sirius_json_set($job_node, $job_json);
		node_save($job_node);

		return array('success' => TRUE, 'msg' => "Extra slots set to $total.");
	}

	public function extraslots_get_total($job_node) {
		$job_json = sirius_json_get($job_node);
		return $job_json['1for1']['extraslots']['total'];
	}

	public function extraslots_available($job_node) {
		$total = $this->extraslots_get_total($job_node);
		if (!$total) { return 0; }

		$dispatch_nids = $this->extraslots_dispatch_nids($job_node);
		foreach ($dispatch_nids as $dispatch_nid) {
			/*
			$dispatch_node = node_load($dispatch_nid);
			$status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
			if (!in_array($status, array('accepted', 'accepted_secondary'))) { continue; }
			*/
			
			--$total;
		}

		return $total;
	}

	public function extraslots_dispatch_nids($job_node) {
		$job_json = sirius_json_get($job_node);
		$dispatch_nids = $job_json['1for1']['extraslots']['dispatches'];
		if (!$dispatch_nids) { $dispatch_nids = array(); }
		return $dispatch_nids;
	}

	public function extraslots_replace($job_node, $dispatch_node) {
		$available = $this->extraslots_available($job_node);
		if ($available <= 0) { return array('success' => FALSE, 'msg' => 'No extra slots are available.'); }

		$job_json = sirius_json_get($job_node);
		$job_json['1for1']['extraslots']['dispatches'][$dispatch_node->nid] = $dispatch_node->nid;
		sirius_json_set($job_node, $job_json);
		node_save($job_node);

		$log_category = '1for1';
		$log_type = 'replace';
		$log_msg = 'Replaced extra slot';
		$handlers = array();
		$handlers[] = $dispatch_node->nid;
		$handlers[] = $job_node->nid;
		$handlers[] = $job_node->field_grievance_shop['und'][0]['target_id'];
		$log_node = sirius_log($log_category, $log_type, $log_msg, '', '', $handlers);

		return array('success' => TRUE, 'msg' => 'Replaced extra slot');
	}
}