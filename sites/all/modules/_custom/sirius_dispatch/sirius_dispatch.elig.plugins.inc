<?php

function sirius_dispatch_elig_info_all() {
  $infos = &drupal_static(__FUNCTION__);
  if (!isset($infos)) {
    $infos = module_invoke_all('sirius_dispatch_elig_info');
    ksort($infos);
  }
  return $infos;
}

function sirius_dispatch_elig_info($key) {
  $infos = &drupal_static(__FUNCTION__); 
  if (!isset($infos)) { $infos = sirius_dispatch_elig_info_all(); }
  return $infos[$key];
}

function sirius_dispatch_elig_instance($key) {
  $info = sirius_dispatch_elig_info($key);
  return $info['instance'];
}

function sirius_dispatch_elig_plugin_active($job_node, $plugin_key) {
	$job_policy = sirius_dispatch_job_policy($job_node);
	return ($job_policy['plugin'][$plugin_key]['active'] == 'Yes');
}

function sirius_dispatch_elig_plugin_load($plugin_key) {
	$plugin = sirius_dispatch_elig_info($plugin_key);
	if ($plugin['file']) { sirius_require($plugin['module'], $plugin['file']); }
}


/**
 * Implement this class to create a new eligibility plugin
 */

class Sirius_Dispatch_Elig_Plugin {
  // Reports are singletons. Use My_Report::getInstance()->whatever()
  protected static $instance = array();
  protected function __construct() { }
  final private function __clone() { }

  final public static function getInstance() {
    $class = get_called_class();
    if (!isset(static::$instance[$class])) { static::$instance[$class] = new static(); }
    return static::$instance[$class];
  }

  public function info($result = array()) {
    $result['name'] = $this->name;
    $result['instance'] = $this;
    return $result;
  }

  public function setInfo(&$items) {
    $items[$this->type] = $this->info();
  }

  /**
   * Settings callback. 
   * 
   * @return array A renderable FAPI form
   */

  public function settings() { return array(); }

  /**
   * Eligibility check. Must be optimized to run quickly, since it may be called many times for large
   * numbers of workers.
   */
   
  public function elig($context, $workers) { return array('success' => TRUE); }
  public function accept(&$dispatch_node, $notes = "") { return array('success' => TRUE); }
  public function notify(&$dispatch_node, $requested_medium, $notes = "") { return array('success' => TRUE); }
}
