<?php

// Got to get the damn autoload working...
require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'sirius_postal') . '/sirius_postal.client.inc';

/**
 * Router for webhooks
 */

function sirius_postal_lob_webhook() {
	drupal_json_output(Sirius_Postal_Client_Lob::getInstance()->webhook());
}

class Sirius_Postal_Client_Lob extends Sirius_Postal_Client {
	public $type = 'postal_lob';
	public $name = 'Lob Postal Client';
	public $description = 'Postal client connecting to the "Lob" service.';

	private $_auth;

  public function settings_form() { 
	  $form['info'] = array(
	  	'#markup' => '<p>API keys for the Lob service can be obtained <a href="https://dashboard.lob.com/settings/api-keys" target="_blank">here</a>.</p>',
	  );

	  $form['test'] = array(
	  	'#type' => 'fieldset',
	  	'#title' => t('Credentials (Test)'),
	  );

	  $form['test']['public_key'] = array(
	  	'#type' => 'textfield',
	  	'#title' => t('Publishable API Key'),
	  	'#description' => t('Should begin "<strong>test_pub_....</strong>"'),
	  );

	  $form['test']['private_key'] = array(
	  	'#type' => 'textfield',
	  	'#title' => t('Secret API Key'),
	  	'#description' => t('Should begin "<strong>test_....</strong>"'),
	  );

	  $form['live'] = array(
	  	'#type' => 'fieldset',
	  	'#title' => t('Credentials (Live)'),
	  );

	  $form['live']['public_key'] = array(
	  	'#type' => 'textfield',
	  	'#title' => t('Publishable API Key'),
	  	'#description' => t('Should begin "<strong>live_pub_....</strong>"'),
	  );

	  $form['live']['private_key'] = array(
	  	'#type' => 'textfield',
	  	'#title' => t('Secret API Key'),
	  	'#description' => t('Should begin "<strong>live_....</strong>"'),
	  );

	  $form['enabled_types'] = array(
	  	'#title' => 'Enabled Mail Types',
	  	'#type' => 'checkboxes',
	  	'#options' => $this->mail_types(),
	  );

		$letterhead_options = sirius_letterhead_list(array('format' => 'html'));
		if ($letterhead_options) {
			$form['letterhead_nid'] = array(
				'#type' => 'select',
				'#title' => t('Default Letterhead'),
				'#options' => array(NULL => t('-- Select --')) + $letterhead_options,
			);
		}

	  $form['addr'] = array(
	  	'#type' => 'fieldset',
	  	'#title' => t('Default Sender Address'),
	  );

		$keys = $this->addr_keys();
		foreach ($keys as $key => $title) {
			$form['addr'][$key] = array(
				'#type' => 'textfield',
				'#title' => $title,
			);
		}

		return $form;
	}

	public function mail_options_form() {
		$type_options = $this->mail_type_options();
		if (!$type_options) { return sirius_form_error('No mail types are enabled for this mailer.'); }

		$form['mail_type'] = array(
			'#type' => 'select',
			'#title' => t('Mail Type'),
			'#options' => array(NULL => t('-- Select --')) + $type_options,
		);

		$form['color'] = array(
			'#type' => 'select',
			'#title' => t('Color?'),
			'#options' => array(
				'false' => t('No, print black-and-white'),
				'true' => t('Yes, print color'),
			),
		);

		$form['double_sided'] = array(
			'#type' => 'select',
			'#title' => t('Double Sided?'),
			'#options' => array(
				'true' => t('Yes, print double sided'),
				'false' => t('No, print single sided'),
			),
		);

		$letterhead_options = sirius_letterhead_list(array('format' => 'html'));
		if ($letterhead_options) {
			$form['letterhead_nid'] = array(
				'#type' => 'select',
				'#title' => t('Letterhead'),
				'#options' => array(NULL => t('-- Select --')) + $letterhead_options,
			);
		}

		return $form;
	}

	public function mail_types() {
		return array(
			'usps_standard' => 'USPS Standard',
			'usps_first_class' => 'USPS First Class',
			'usps_first_class|certified' => 'Certified',
			'usps_first_class|certified_return_receipt' => 'Certified, Return Receipt',
			'usps_first_class|registered' => 'Registered',
		);
	}

	public function mail_types_enabled() {
		$settings = $this->settings();
		$enabled_types = $settings['enabled_types'];
		if (!$enabled_types) { $enabled_types = array(); }
		return array_filter(array_values($enabled_types));
	}

	public function mail_type_check($type) {
		return in_array($type, $this->mail_types_enabled());
	}

	public function mail_type_default() {
		return 'usps_first_class';
	}

	public function ping($mode = 'test') {
		try {
			$auth = $this->init($mode);
			$apiInstance = new OpenAPI\Client\Api\AddressesApi($auth);
			$lob_result = $apiInstance->list();
			$data = $lob_result->getData();
		} catch (Exception $e) {
			return array('success' => FALSE, 'msg' => $e->getMessage());
		}

		return array('success' => TRUE, 'msg' => 'Lob connection successful.');
	}

	public function send_impl(&$params) {
		$to = $params['to'];
		
		$auth = $this->init($params['mode']);
		$settings = $this->settings();
		$options = $params['options'];
		try {

			$to_addr = new OpenAPI\Client\Model\AddressEditable($to);

			$mail_type_raw = $options['mail_type'];
			if (!$mail_type_raw) { $mail_type_raw = $this->mail_type_default(); }
			if (!$this->mail_type_check($mail_type_raw)) { return array('success' => FALSE, 'msg' => "Mail type not enabled: $mail_type"); }
			list ($mail_type_real, $extra_service) = explode('|', $mail_type_raw);

			$color = $options['color'];
			if (!$color) { $color = 'false'; }

			$from_arr = $params['from'];
			if (!$from_arr) { $from_arr = $this->get_from_default(); }
			$from = new OpenAPI\Client\Model\AddressEditable($from_arr);
			$params['from'] = $from;

			$letterhead_nid = $options['letterhead_nid'];
			if (!$letterhead_nid) { $letterhead_nid = $settings['letterhead_nid']; }
			if ($letterhead_nid) {
				$letterhead_node = node_load($letterhead_nid);
				$body = $letterhead_node->body['und'][0]['value'];
			} else {
				$body = $this->default_template();
			}

			$tokens = array();
			$tokens['sirius']['letterhead']['body'] = $params['body'];
			$tokens['sirius']['letterhead']['title'] = '';
			$body = token_replace($body, $tokens, array('clear' => TRUE));

			// return array('success' => FALSE, 'msg' => 'Dev');

			$apiInstance = new OpenAPI\Client\Api\LettersApi($auth);
			$letter_editable = new OpenAPI\Client\Model\LetterEditable(
			  array(
			    "description"     => "Letter " . $params['id'],
			    "from"     				=> $from,
			    "file"     				=> $body,
			    'address_placement' => 'insert_blank_page',
			    "color"    				=> $color,
			    "to"     					=> $to_addr,
			    "merge_variables" => NULL,
			    "cards"     			=> NULL,
			    "use_type" 				=> 'operational',
			    "qr_code"   			=> NULL,
			    "fsc" 						=> FALSE,
			    "mail_type"				=> $mail_type_real,
			    'extra_service' 	=> $extra_service,
			  )
			);

	    $lob_result = $apiInstance->create($letter_editable);
		} catch (Exception $e) {
			return array('success' => FALSE, 'msg' => $e->getMessage());
		}

		return array('success' => TRUE, 'message' => (array) $lob_result, 'external_id' => $lob_result['id']);
	}


	public function postal_lob_retrieve($ltr_id) {
		$auth = $this->init($params['mode']);
		try {
			$apiInstance = new OpenAPI\Client\Api\LettersApi($auth);
			$lob_result = $apiInstance->get($ltr_id);
		} catch (Exception $e) {
			return array('success' => FALSE, 'msg' => $e->getMessage());
		}
		return array('success' => TRUE, 'data' => (array) $lob_result);
	}

	public function init($mode = 'test') {
		require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'sirius_postal_lob') . '/lob-php/vendor/autoload.php';

		if (!$this->_auth) {
			$settings = $this->settings();
			$auth = OpenAPI\Client\Configuration::getDefaultConfiguration();

			if ($mode == 'live') {
				$private = $settings['live']['private_key'];
				$public = $settings['live']['public_key'];
			} else {
				$private = $settings['test']['private_key'];
				$public = $settings['test']['public_key'];
			}

			$auth->setUsername($public);
			$auth->setPassword($private);
			$auth->setApiKey('basic', $private . ':' . $public);
			$this->_auth = $auth;
		}

		return $this->_auth;
	}

	public function webhook_impl($params = array()) {
		// Parse the input
		$input = file_get_contents('php://input');
		if (!$input) { return array('success' => FALSE, 'msg' => 'No data was POSTed to webhook.'); }

		$json = json_decode($input, TRUE);
		if (!$json) { return array('success' => FALSE, 'msg' => 'Posted data did not decode as json.'); }

		$event_id = $json['id'];
		if (!$event_id) { return array('success' => FALSE, 'msg' => 'Posted json did not contain an event ID.'); }

		$external_id = $json['reference_id'];
		if (!$external_id) { return array('success' => FALSE, 'msg' => 'Posted json did not contain a reference id.'); }

		$status = $json['event_type']['id'];
		$details = $json;
		return array('success' => TRUE, 'event_id' => $event_id, 'external_id' => $external_id, 'status' => $status, 'details' => $details);
	}

	public function get_external_url($comm_node) {
		$external_id = $this->get_external_id($comm_node);
		if (!$external_id) { return ''; }
		return 'https://dashboard.lob.com/letters/' . $external_id;
	}

	public function render_body($comm_node) {
		$body = $this->get_body($comm_node);
		$json = sirius_json_get($comm_node);
		if (!$body) { return ''; }
		$html .= '<div class="sirius_comm_section sirius_comm_to">';
		$html .= '<h2>' . t('Message Body') . '</h2>';
		$html .= sirius_text_to_html($body);

		// Should do this: 
		// $pdf_url = $json['result']['message']['*container']['url']
		// but the "*container" contains some weird characters, maybe space-padded. So:
		if ($json['result']['message']) {
			$container_json = $json['result']['message'][array_keys($json['result']['message'])[0]];
		}
		if ($container_json) {
			$pdf_url = $container_json['url'];
		}
		if ($pdf_url) {
			parse_str(parse_url($pdf_url, PHP_URL_QUERY), $query_parts);
			if ($query_parts['expires'] && ($query_parts['expires'] > time())) {
				$html .= '<p>';
				$html .= l('View PDF', $pdf_url, array('attributes' => array('target' => 'blank', 'class' => array('tool-button'))));
				$html .= '</p>';
			}
		}
		$html .= '</div>';
		return $html;
	}

	public function default_template() {
		return <<<EHTML
<html>
<head>
<meta charset="UTF-8">
<link href="https://fonts.googleapis.com/css?family=Lora:400,700" rel="stylesheet">
<title>[sirius:letterhead:body]</title>
<style>

  *, *:before, *:after {
    -webkit-box-sizing: border-box;
    -moz-box-sizing: border-box;
    box-sizing: border-box;
  }

  body {
    width: 8.5in;
    height: 11in;
    margin: 0;
    padding: 0;
    font-family: 'Lora';
  }

  .page {
    page-break-after: always;
  }

  .page-content {
    position: relative;
    width: 7in;
    height: 10.625in;
    left: 0.75in;
    top: 0.1875in;
  }

  #logo {
    position: absolute;
    left: -10px;
    width: 80px;
    top: .35in;
  }

  .wrapper {
    position: absolute;
  }
</style>
</head>
<body>
  <div class="page">
    <div class="page-content">
      <div class="wrapper">
        [sirius:letterhead:body]
      </div>
    </div>
  </div>
</body>
</html>
EHTML;
	}

	public function event_status_info_all() {
		return array(
			'address.created' => array(),
			'address.deleted' => array(),
			'bank_account.created' => array(),
			'bank_account.deleted' => array(),
			'bank_account.verified' => array(),
			'check.created' => array(),
			'check.deleted' => array('comm_status' => 'failed'),
			'check.delivered' => array('comm_status' => 'sent'),
			'check.failed' => array('comm_status' => 'failed'),
			'check.in_local_area' => array(),
			'check.in_transit' => array(),
			'check.international_exit' => array(),
			'check.processed_for_delivery' => array(),
			'check.re-routed' => array(),
			'check.rejected' => array('comm_status' => 'failed'),
			'check.rendered_pdf' => array(),
			'check.rendered_thumbnails' => array(),
			'check.returned_to_sender' => array(),
			'letter.certified.delivered' => array('comm_status' => 'sent'),
			'letter.certified.in_local_area' => array(),
			'letter.certified.in_transit' => array(),
			'letter.certified.issue' => array(),
			'letter.certified.mailed' => array(),
			'letter.certified.pickup_available' => array(),
			'letter.certified.processed_for_delivery' => array(),
			'letter.certified.re-routed' => array(),
			'letter.certified.returned_to_sender' => array('comm_status' => 'failed'),
			'letter.created' => array(),
			'letter.deleted' => array('comm_status' => 'failed'),
			'letter.delivered' => array('comm_status' => 'sent'),
			'letter.failed' => array('comm_status' => 'failed'),
			'letter.in_local_area' => array(),
			'letter.in_transit' => array(),
			'letter.international_exit' => array(),
			'letter.processed_for_delivery' => array(),
			'letter.re-routed' => array(),
			'letter.rejected' => array('comm_status' => 'failed'),
			'letter.rendered_pdf' => array(),
			'letter.rendered_thumbnails' => array(),
			'letter.returned_to_sender' => array('comm_status' => 'failed'),
			'letter.viewed' => array('comm_status' => 'sent'),
			'postcard.created' => array(),
			'postcard.deleted' => array('comm_status' => 'failed'),
			'postcard.delivered' => array('comm_status' => 'sent'),
			'postcard.failed' => array('comm_status' => 'failed'),
			'postcard.in_local_area' => array(),
			'postcard.in_transit' => array(),
			'postcard.international_exit' => array(),
			'postcard.processed_for_delivery' => array(),
			'postcard.re-routed' => array(),
			'postcard.rejected' => array('comm_status' => 'failed'),
			'postcard.rendered_pdf' => array(),
			'postcard.rendered_thumbnails' => array(),
			'postcard.returned_to_sender' => array('comm_status' => 'failed'),
			'postcard.viewed' => array('comm_status' => 'sent'),
		);
	}
}

