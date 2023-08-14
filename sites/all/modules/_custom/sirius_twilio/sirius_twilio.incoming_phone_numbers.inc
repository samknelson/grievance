<?php

function sirius_twilio_incoming_phone_numbers_list($search_params = array()) {
	try {
		$client = sirius_twilio_client_get();
		$ipn_request = $client->incomingPhoneNumbers->read($search_params);
		$result = array();

		$count = 0;
		foreach ($ipn_request as $ipn_record) {
			$result[] = $ipn_record->toArray();
			++$count;
		}
		// drupal_set_message("Found $count records");
		return array('success' => TRUE, 'records' => $result);
	} catch (Exception $e) {
		$msg =  $e->getCode() . ' - ' . $e->getMessage() . ": " . $msg;
		return array('success' => FALSE, 'msg' => "Twilio API failed: $msg");
	}
}
