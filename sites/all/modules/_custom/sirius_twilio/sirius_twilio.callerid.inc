<?php

function sirius_twilio_callerid_verify($phone, $name_display) {
	$phone = sirius_phone_format_e164($phone);

	if (!$phone || !$name_display) {
		return array('success' => FALSE, 'msg' => t('Please enter a phone number and a display name.'));
	}

	try {
		$client = sirius_twilio_client_get();
		$validation_request = $client->validationRequests->create($phone, array("friendlyName" => $name_display));

		$validation_code = $validation_request->validationCode;
		$call_sid = $validation_request->callSid;
		$friendly_name = $validation_request->friendlyName;
		$phone_number = $validation_request->phoneNumber;

		return array('success' => TRUE, 'validation_code' => $validation_code);
	} catch (Exception $e) {
		$msg =  $e->getCode() . ' - ' . $e->getMessage() . ": " . $msg;
		sirius_log('twilio:callerid', 'error', $msg, $msg, $phone, array($callerid_node->nid));
		return array('success' => FALSE, 'msg' => "Twilio API failed: $msg");
	}
}

function sirius_twilio_callerid_fetch_by_phone($phone) {
	$phone = sirius_phone_format_e164($phone);

	try {
		$client = sirius_twilio_client_get();
		$callerid_request = $client->outgoingCallerIds->read(array("phoneNumber" => $phone), 20);
		foreach ($callerid_request as $record) {
			$callerid_sid = $record->sid;
			return sirius_twilio_callerid_fetch_by_sid($callerid_sid);
		}
		return array('success' => FALSE, 'msg' => t('Phone number @phone not found.', array('@phone' => $phone)));
	} catch (Exception $e) {
		$msg =  $e->getCode() . ' - ' . $e->getMessage() . ": " . $msg;
		sirius_log('twilio:callerid', 'error', $msg, $msg, $phone, array($callerid_node->nid));
		return array('success' => FALSE, 'msg' => "Twilio API failed: $msg");
	}
}

function sirius_twilio_callerid_fetch_by_sid($sid) {
	try {
		$client = sirius_twilio_client_get();
		$callerid = $client->outgoingCallerIds($sid)->fetch();
		$created = $callerid->dateCreated->getTimestamp();
		$updated = $callerid->dateUpdated->getTimestamp();
		$name_display = $callerid->friendlyName;
		return array('success' => TRUE, 'created' => $created, 'updated' => $updated, 'sid' => $sid, 'name_display' => $name_display);
	} catch (Exception $e) {
		$msg =  $e->getCode() . ' - ' . $e->getMessage() . ": " . $msg;
		sirius_log('twilio:callerid', 'error', $msg, $msg, $phone, array($callerid_node->nid));
		return array('success' => FALSE, 'msg' => "Twilio API failed: $msg");
	}
}


function sirius_twilio_callerid_update($phone, $name_display) {
	$result = sirius_twilio_callerid_fetch_by_phone($phone);
	if (!$result['success']) { return $result; }
	$sid = $result['sid'];

	try {
		$client = sirius_twilio_client_get();
		$client->outgoingCallerIds($sid)->update(array("friendlyName" => $name_display));
		return array('success' => TRUE);
	} catch (Exception $e) {
		$msg =  $e->getCode() . ' - ' . $e->getMessage() . ": " . $msg;
		sirius_log('twilio:callerid', 'error', $msg, $msg, $phone, array($callerid_node->nid));
		return array('success' => FALSE, 'msg' => "Twilio API failed: $msg");
	}
}

function sirius_twilio_callerid_delete($phone) {
	$result = sirius_twilio_callerid_fetch_by_phone($phone);
	if (!$result['success']) { return $result; }
	$sid = $result['sid'];

	try {
		$client = sirius_twilio_client_get();
		$client->outgoingCallerIds($sid)->delete();
		return array('success' => TRUE);
	} catch (Exception $e) {
		$msg =  $e->getCode() . ' - ' . $e->getMessage() . ": " . $msg;
		sirius_log('twilio:callerid', 'error', $msg, $msg, $phone, array($callerid_node->nid));
		return array('success' => FALSE, 'msg' => "Twilio API failed: $msg");
	}
}
