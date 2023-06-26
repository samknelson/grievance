<?php

class Sirius_Sched_Dispatch_Noovertime extends Sirius_Dispatch_Elig_Plugin {
  public $type = 'sched-noovertime';
  public $name = 'Avoid Overtime';

	public function settings() {
		$form['day_limit'] = array(
			'#title' => 'Day Limit',
			'#type' => 'textfield',
			'#size' => 4,
			'#description' => t('Maximum number of hours worked in a day'),
		);

		return $form;
	}

	public function elig($context, $workers) {
		$plugin_key = 'sched_noovertime';
		drupal_set_message("Overtime scan...");

		$plugin_settings = $context['job_policy']['plugin'][$plugin_key]['settings'];
		return array('success' => TRUE);

		$job_node = $context['job_node'];
		if (!$job_node) { return array('success' => FALSE, 'msg' => 'No job node.'); }

		$start_ts = sirius_fieldval($job_node, 'field_sirius_datetime', 'ts');
		if (!$start_ts) { return array('success' => FALSE, 'msg' => 'No start time.'); }
		$grace_sec = $plugin_settings['grace'] * 60;

		$end_ts = sirius_fieldval($job_node, 'field_sirius_datetime_completed', 'ts');
		if ($end_ts <= $start_ts) { $end_ts = $start_ts; }

		$start_ts -= $grace_sec;
		$end_ts += $grace_sec;

		$filters = array();
		$filters['interval'] = array('start_ts' => $start_ts, 'end_ts' => $end_ts);

		$inelig_workers = array();
		foreach ($workers as $worker_nid => $ignore) {
			// drupal_set_message("Checking for schedule conflict: $worker_nid");
			$filters['entity_nid'] = $worker_nid;
			$scheds = sirius_sched_db_search($filters);

			// Filter out entries which reference the current job
			// (i.e. if I'm scheduled to work for this job, that doesn't mean I have a scheduling conflict for this job.)
			$scheds = array_filter($scheds, function($sched) use ($job_node) {
				if ($sched['json']['job_nid'] == $job_node->nid) { return FALSE; }
				return TRUE;
			});
			if ($scheds) {
				// Found a conflicting schedule entry. Oh well.
				$sched = reset($scheds);
				$inelig_workers[$worker_nid] = array('details' => 'Scheduling conflict: ' . 
					$sched['json']['display']['type'] . ' / ' . 
					$sched['json']['display']['status'] . ' / ' . 
					$sched['json']['display']['details']
				);
			}
		}

		return array('success' => TRUE, 'inelig_workers' => $inelig_workers);
	}
}