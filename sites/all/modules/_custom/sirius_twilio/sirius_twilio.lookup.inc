<?php

function sirius_twilio_lookup($phone) {
	$phone = sirius_phone_format_e164($phone);
	try {
		$client = sirius_twilio_client_get();
		$phone_obj = $client->lookups->v1->phoneNumbers($phone);
		if (!$phone_obj) { return array('success' => FALSE, 'msg' => 'Lookup service returned no results.'); }
		$details = $phone_obj->fetch(array("type" => array("carrier")));
		return array('success' => TRUE, 'msg' => 'Lookup succesful', 'carrier' => $details->carrier);
	} catch (Exception $e) {
		$msg =  $e->getCode() . ' - ' . $e->getMessage();
		return array('success' => FALSE, 'msg' => "Lookup failure: $msg");
	}
}

