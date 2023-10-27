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
	$result->setMsg($obj_or_array['msg']);
	$result->setSuccess($obj_or_array['success']);

	$data = $obj_or_array;
	unset($obj_or_array['msg']);
	unset($obj_or_array['success']);
	$result->setData($data);
	return $result;
}

/**
 * Result object for function calls
 */

class SiriusResult {
	public $success = FALSE;
	public $msg = '';
	public $data = array();

	public function setData($data) { $this->data = $data; }
	public function setMsg($msg) { $this->msg = $msg; }
	public function setSuccess($success) { $this->success = $success; }

	public function getData() { return $this->data; }
	public function getMsg() { return $this->msg; }
	public function getSuccess() { return $this->success; }
}