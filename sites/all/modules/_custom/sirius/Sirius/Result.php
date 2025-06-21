<?php

/**
 * Most function return e.g. array('success' => FALSE, 'msg' => 'Whatever') 
 * 
 * We wish we could convert every function so that it returned a SiriusResult object. But unfortuantely that's 
 * going to be a pretty major undertaking.
 * 
 * In the mean time, everybody can safely use this wrapper function, which accepts either an array or an object.
 */

function sirius_result_create($obj_or_array) {
	if (is_object($obj_or_array)) {
		return $obj_or_array;
	}

	$result = new SiriusResult();
	$result->data = $obj_or_array;
	return $result;
}

/**
 * Result object for function calls
 */

class SiriusResult {
	public static function create($obj_or_array) {
		if (is_object($obj_or_array)) {
			return $obj_or_array;
		}

		$result = new SiriusResult();
		$result->data = $obj_or_array;
		return $result;
	}
	
	public $data = array();

	public function msg() { return $this->data['msg']; }
	public function success() { return $this->data['success']; }

	public function get($name) { return $this->data[$name]; }
	public function set($name, $value) { $this->data[$name] = $value; }

	public function data() { return $this->data; }

	public function drupalSetMessage() {
		if ($this->success()) {
			if ($this->msg()) {
				drupal_set_message("Success: " . $this->msg());
			} else {
				drupal_set_message("Success.");
			}
		} else {
			if ($this->msg()) {
				drupal_set_message("Error: " . $this->msg(), 'error');
			} else {
				drupal_set_message("Error.", 'error');
			}
		}
	}
}