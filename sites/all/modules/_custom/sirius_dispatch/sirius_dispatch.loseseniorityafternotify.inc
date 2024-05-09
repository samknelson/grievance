<?php

/**
 * Plugin callback: Settings
 */

class Sirius_Dispatch_Loseseniorityafternotify extends Sirius_Dispatch_Elig_Plugin {
  public $type = 'loseseniorityafternotify';
  public $name = 'Lose Seniority After Notify';

	/**
	 * Plugin callback: Notify
	 *
	 * Reset the seniority
	 */

	public function notify(&$dispatch_node, $requested_medium, $notes = '') {
		$worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);
		$worker_node->field_sirius_dispatch_availdate['und'][0]['value'] = gmdate('Y-m-d H:i:s');
		node_save($worker_node);
		return array('success' => TRUE);
	}
}