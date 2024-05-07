<?php

class Report_Cache_Report_Generator {
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
    $result['generator'] = $this;
    $result['fields'] = $this->fields();
    return $result;
  }

  public function fields() {
    return array();
  }

  public function pks(&$form_state) {
    return array();
  }

  public function batch($pks, $values, &$context) {
    return array();
  }

  public function render_parameters($parameters) {
  	// Legacy supporrt
  	if ($this->info()['render_parameters_callback']) {
  		return $this->info()['render_parameters_callback']($parameters);
  	}
    return '';
  }

  public function setInfo(&$items) {
    $items[$this->type] = $this->info();
  }

  public function parameters($report_id, $parameters = array()) {
  	return report_cache_parameters($this->type, $report_id, $parameters);
  }

  public function most_recent($instance_uuid = NULL) {
  	return report_cache_most_recent($this->type, $instance_uuid);
  }

  public function dashboard_render_never_run($prefs, $options) {
		return array(
			array(
				'count' => 0, 
				'tools' => l('Generate', "sirius/reports/$report_type/form", array('attributes' => array('class' => array('tool-button')))),
				'details' => 'Report has never been run',
			)
		);
	}

  public function dashboard($prefs, $options, $report_id = NULL) {
		$report_id = $this->most_recent($prefs['instance']);
		if (!$report_id) { return $this->dashboard_render_never_run($prefs, $options); }

    if ($prefs['link_only'] != 'Yes') {
      return $this->dashboard_render_link($prefs, $options, $report_id);
    }

    return $this->dashboard_render_full($prefs, $options, $report_id);
  }

  public function dashboard_render_link($prefs, $options, $report_id) {
  	$report_type = $this->type;
		$parameters = $this->parameters($report_id);
		$details = $parameters['meta']['dashboard']['details'];
		$result = array();
		$result['tools'] = l('View', "sirius/reports/$report_type", array('query' => array('report_id' => $report_id, 'action' => 'render', 'callback-name' => 'View'), 'attributes' => array('class' => array('tool-button'))));
		$count = $parameters['meta']['dashboard']['count'];
		$result['count'] = $count;
		$ts = $parameters['meta']['ts']['completed'];
		if (isset($count)) {
			$result['details'] = "<strong>$count records</strong> found " . format_interval(time() - $ts) . ' ago.';
		} else {
			$result['details'] = "Last report run " . format_interval(time() - $ts) . ' ago.';
		}
		if ($details) { $result['details'] .= '<br>' . $details; }
		return array($result);
  }

  public function dashboard_render_full($prefs, $options, $report_id) {
  	return $this->dashboard_render_link($prefs, $options, $report_id);
  }

  public function search($options = array()) {
    $report_ids = array();

    $domain_nid = sirius_domain_nid(); if (!$domain_nid) { $domain_nid = 0; }
    $sql_args = array();

    $limit = $options['limit']; 
    if (!$limit || !is_int($limit)) { $limit = 1000; }

    $sql = "select * from report_cache ";
    $sql .= "where report_type = :report_type ";
    $sql_args[':report_type'] = $this->type . " :: PARAMETERS";
    $sql .= "and report_domain_nid = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
    $sql .= "order by report_id desc ";
    $stmt = sirius_sql_query($sql, $sql_args);
    $report_ids = array();

    $count = 0;
    while ($hr = $stmt->fetchAssoc()) {
      $report_data = unserialize($hr['report_data']);

      // Inefficient! Load all the parameters and check them one at a time. Yuck.
      $skip = FALSE;
      if ($options['parameters']) {
        foreach ($options['parameters'] as $key => $value) {
          if ($report_data['parameters'][$key] != $value) { $skip = TRUE; }
        }
      }
      if ($skip) { continue; }
      $report_ids[$hr['report_id']] = $report_data;
      ++$count; 
      if ($count >= $limit) { break; }
    }
    return $report_ids;
  }

  public function callback_wrapper_render($callback, $report_id, $output, $filter_values) {
  	if (method_exists($this, $callback)) {
  		return $this->{$callback}($report_id, $output, $filter_values);
  	} else {
  		return $callback($this->type, $report_id, $output, $filter_values);
  	}
  }
}

