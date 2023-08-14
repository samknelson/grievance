<?php

/**
 * Singleton base class
 */

class Sirius_Singleton {
  // Reports are singletons. Use My_Report::getInstance()->whatever()
  protected static $instance = array();
  protected function __construct() { }
  final private function __clone() { }

  final public static function getInstance() {
    $class = get_called_class();
    if (!isset(static::$instance[$class])) { static::$instance[$class] = new static(); }
    return static::$instance[$class];
  }

	public $type = 'singleton';
	public $name = 'Singleton Base Class';
	public $description = '';

	public function get_name() { return $this->name; }
	public function get_type() { return $this->type; }
	public function get_description() { return $this->description; }

  public function info($result = array()) {
    $result['type'] = $this->get_type();
    $result['name'] = $this->get_name();
    $result['description'] = $this->get_description();
    $result['instance'] = $this;
    return $result;
  }

  public function setInfo(&$items) {
    $items[$this->get_type()] = $this->info();
  }
}

