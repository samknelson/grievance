<?php

function sirius_sms_enabled() { return TRUE; }
function sirius_sms_send($params) { return sirius_comm_send('sms', $params); }

function sirius_sms_callback($json) {
  drupal_json_output(Sirius_Comm_Sms::getInstance()->webhook($json));
}

function sirius_sms_segment_count($html_id) {
  $settings = array();
  $settings['html_id'] = $html_id;
  if (sirius_domain_variable_get('sirius_twilio_sms_unsubscribe', TRUE)) {
    $settings['suffix'] = sirius_domain_variable_get('sirius_twilio_sms_unsubscribe_msg', "\nReply STOP to unsubscribe.\n");
  } else {
    $settings['suffix'] = '';
  }

  drupal_add_js(
    array('sirius_sms_segment_count' => $settings),
    array('type' => 'setting')
  );
  drupal_add_js('sites/all/modules/_custom/sirius/js/sms_segment_count.js');
  drupal_add_css('sites/all/modules/_custom/sirius/css/sms_segment_count.css');

  $html = '<div class="sirius_sms_segment_count" id="' . $html_id . '_count">';
  $html .= t('SMS Segments:') . '<span class="stat messages"></span>';
  $html .= ' / ';
  $html .= t('Remaining in segment:') . '<span class="stat remaining"></span>';
  $html .= ' / ';
  $html .= t('Total length:') . '<span class="stat length"></span>';
  $html .= ' / ';
  // $html .= t('Per segment:') . '<span class="stat per_message"></span>';
  // $html .= ' / ';
  $html .= t('Encoding:') . '<span class="stat encoding"></span>';
  $html .= '</div>';

  return $html;
}

function sirius_sms_admin() {
  $name = sirius_domain_variable_name('sirius_sms_global_disable');
  $form[$name] = array(
    '#type' => 'select',
    '#title' => t('Global Disable'),
    '#options' => array(
      'No' => t('No'),
      'Yes' => t('Yes'),
    ),
    '#description' => t('Select "Yes" to globally disable SMS on this server.'),
    '#default_value' => variable_get($name, ''),
  );

  $name = sirius_domain_variable_name('sirius_sms_global_disable_whitelist');
  $form[$name] = array(
    '#type' => 'textarea',
    '#title' => t('Whitelist'),
    '#description' => t('When global disable is on, use the whitelist to indicate which phone numbers <strong>can</strong> receive SMS. Phone numbers must be in the format (xxx) xxx-xxxx.'),
    '#default_value' => variable_get($name, ''),
    '#states' => array(
      'visible' => array(
        array(':input[name="sirius_sms_global_disable"]' => array('value' => 'Yes')),
      )
    ),
  );

  return system_settings_form($form);
}

class Sirius_Comm_Sms extends Sirius_Comm {
  public $type = 'sms';
  public $name = 'SMS';

  public function send_possible_impl($params) {
    return array('success' => TRUE);
  }

  public function get_from_default() {
  	return sirius_domain_variable_get('sirius_twilio_phone', '');
  }

	public function get_external_url($comm_node) {
		$external_id = $this->get_external_id($comm_node);
		if (!$external_id) { return ''; }
		return "https://console.twilio.com/us1/monitor/logs/sms/$external_id";
	}

  public function send_impl(&$params) {
    $msg = $params['body'];

    $id = $params['id'];

    $from = $params['from'];
    if (!$from) { $from = $this->get_from_default(); }
    if (!$from) { return array('success' => FALSE, 'msg' => 'No "From" phone number set in the message or configuration.'); }

    $phone = $params['to'];
    $handler_nids = $params['handler_nids'];

    if (is_array($handler_nids)) {
      $handler_nid = $handler_nids[0];
    } else {
      $handler_nid = $handler_nids;
    }

    $possible = sirius_phonenumber_sms_possible($phone, $from);
    if (!$possible) { 
      return array(
        'success' => FALSE, 
        'msg' => "SMS messaging to $phone is not possible because the number is flagged as non-deliverable. Possible causes include a user STOP message, an administrative block, or a detected landline."
      );
    }

    $phonenumber_node = sirius_phonenumber_lookup($phone);
    if (!$phonenumber_node->nid) { node_save($phonenumber_node); }
    $handler_nids[] = $phonenumber_node->nid;

    $result = sirius_twilio_outgoing_sms($phone, $msg, $handler_nid, $id, $from, $params['sms_attach']);

    $phonenumber_json = sirius_json_get($phonenumber_node);
    $phonenumber_json['last_sms'] = array(
      'ts' => time(),
      'params' => $params,
      'result' => $result,
    );
    sirius_json_set($phonenumber_node, $phonenumber_json);
    node_save($phonenumber_node);

    return $result;
  }

  public function webhook_impl($json = array()) {
    // Really need different imlpementations for AWS and SendGrid. This is AWS.
    $external_id = $json['SmsSid'];
    if (!$external_id) { return array('success' => FALSE, 'msg' => 'No external ID sent'); }

    $status = $json['SmsStatus'];

	  $summary = '';
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
			'accepted' => array('comm_status' => 'sending'),
			'scheduled' => array('comm_status' => 'sending'),
			'canceled' => array('comm_status' => 'failed'),
			'sending' => array('comm_status' => 'sending'),
			'sent' => array('comm_status' => 'sending'),
			'delivered' => array('comm_status' => 'sent'),
			'read' => array('comm_status' => 'sent'),
			'undelivered' => array('comm_status' => 'failed'),
			'failed' => array('comm_status' => 'failed'),
    );
  }
}

