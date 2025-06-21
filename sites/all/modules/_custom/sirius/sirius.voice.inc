<?php

function sirius_voice_send($params) { return sirius_comm_send('voice', $params); }

function sirius_voice_callback($json) {
  drupal_json_output(Sirius_Comm_Voice::getInstance()->webhook($json));
}

class Sirius_Comm_Voice extends Sirius_Comm {
  public $type = 'voice';
  public $name = 'Voice';

  public function send_possible_impl($params) {
    return array('success' => TRUE);
  }

  public function send_impl(&$params) {
    // Allow callers to set the X-Sirius-Id, and don't resent with the same ID. This allows callers to set unique keys.
    $id = $params['id'];
    $phone = $params['to'];
    $possible = sirius_phonenumber_voice_possible($phone);
    if (!$possible) { return array('success' => FALSE, 'msg' => "$phone does not appear to have a valid voice carrier."); }

    $msg = $params['body'];
    $handler_nids = $params['handler_nids'];
    if (is_array($handler_nids)) {
      $handler_nid = $handler_nids[0];
    } else {
      $handler_nid = $handler_nids;
    }

    $context = array();
    $context['outgoing_msg'] = $msg;
    $context['outgoing_msg_machine'] = $params['voice_machine'];
    $context['handler_nid'] = $handler_nid;
    $context['worker_nid'] = $params['worker_nid'];
    $context['callerid_nid'] = $params['callerid_nid'];
    $context['phonetree'] = $params['phonetree'];
    $context['amd'] = $params['amd'];

    $result = sirius_twilio_outgoing_voice($phone, $context);

    return $result; 
  }

  public function webhook_impl($json = array()) {
    // Really need different imlpementations for AWS and SendGrid. This is AWS.
    $external_id = $json['CallSid'];
    if (!$external_id) { return array('success' => FALSE, 'msg' => 'No external ID sent'); }

    $status = $json['CallStatus'];

    $summary = '';
    if ($json['CallDuration']) {
	  	$summary = $json['CallDuration'] . ' seconds';
	  }
    $event_id = NULL;
    $details = $json;

    return array(
      'success' => TRUE, 
      'event_id' => $event_id, 
      'external_id' => $external_id, 
      'status' => $status, 
      'details' => $details,
      'msg' => $summary
    );
  }

  public function event_status_info_all() {
    return array(
			'queued' => array('comm_status' => 'sending'),
			'ringing' => array('comm_status' => 'sending'),
			'in-progress' => array('comm_status' => 'sending'),
			'completed' => array('comm_status' => 'sent'),
			'busy' => array('comm_status' => 'failed'),
			'failed' => array('comm_status' => 'failed'),
			'no-answer' => array('comm_status' => 'failed'),
			'canceled' => array('comm_status' => 'failed'),
    );
  }
}


