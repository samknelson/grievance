<?php

use Twilio\Rest\Client;

function sirius_twilio_recording_search($params = array(), $count = 1000, $page_url = NULL) {
	try {
		$client = sirius_twilio_client_get();

		if (!$params) { $params = array(); }
		$result = array();
		$result['records'] = array();

		if ($page_url) {
			$iterator = $client->recordings->getPage($page_url);
		} else {
			$iterator = $client->recordings->page($params, $count);
		}

		$result['nextPageUrl'] = $iterator->getNextPageUrl();
		$result['previousPageUrl'] = $iterator->getPreviousPageUrl();
		foreach ($iterator as $record) {
			$result['records'][] = $record->toArray();
		}
		$result['success'] = TRUE;
		return $result;
	} catch (Exception $e) {
		return array('success' => FALSE, 'msg' => 'Error: ' . $e->getCode() . ' - ' . $e->getMessage());
	}
}

function sirius_twilio_recording_delete($rec_sid) {
	try {
		$client = sirius_twilio_client_get();
		$resource = $client->recordings($rec_sid);
		if (!$resource) { return array('success' => TRUE, 'msg' => 'Record not found.'); }
		$resource->delete();
		return array('success' => TRUE, 'msg' => 'Recording deleted.');
	} catch (Exception $e) {
		return array('success' => FALSE, 'msg' => 'Error: ' . $e->getCode() . ' - ' . $e->getMessage());
	}
}

function sirius_twilio_call_fetch($call_sid) {
	try {
		$client = sirius_twilio_client_get();

		// Fetch the call
		$call = $client->calls($call_sid)->fetch();
		$result = array('success' => TRUE, 'call' => $call);

		// Fetch the first recording
		$recordings = $client->recordings->read(["call_sid" => $call_sid], 1);
		foreach ($recordings as $record) {
			$result['recording'] = $record;
			break;
		}

	} catch (Exception $e) {
		$result = array('success' => FALSE, 'msg' => "Failed to fetch call $call_sid: " . $e->getCode() . ' - ' . $e->getMessage());
	}

	return $result;
}
