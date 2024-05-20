<?php

// Got to get the damn autoload working...
require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'sirius_service') . '/sirius_service.client.inc';

function sirius_postal_client_instance($type = NULL) {
	if (!$type) { $type = sirius_postal_client_type(); }

	if ($type == 'postal_lob') { return Sirius_Postal_Client_Lob::getInstance(); }

	return NULL;
}

function sirius_postal_client_type() {
	return sirius_domain_variable_get('sirius_postal_client_type', 'postal_default');
}

function sirius_postal_client_settings() {
	$settings = json_decode(sirius_domain_variable_get('sirius_postal_client_settings'), TRUE);
	return $settings;
}


class Sirius_Postal_Client extends Sirius_Comm {
	public function medium() {
		return 'postal';
	}

	public function ping() {
		return array('success' => FALSE, 'msg' => 'Root postal ping called. This is just a stub.');
	}

	public function send_possible_impl($params) {
		return array('success' => TRUE);
	}

	public function send_impl(&$params) {
		return array('success' => FALSE, 'msg' => 'Stub Sirius_Postal_Client::send_impl called.');
	}

	public function settings() {
		return sirius_postal_client_settings();
	}

	public function mail_types() {
		return array();
	}

	public function mail_types_enabled() {
		return array();
	}

	public function mail_type_options() {
		$all_types = $this->mail_types();
		$enabled_types = $this->mail_types_enabled();
		$options = array();
		foreach ($enabled_types as $key) {
			$options[$key] = $all_types[$key];
		}
		return $options;
	}

	public function mail_options_form() {
		return array();
	}

	public function addr_keys() {
		return array(
			'name' => t('Name'),
			'company' => t('Company'),
			'address_line1' => t('Line 1'),
			'address_line2' => t('Line 2'),
			'address_city' => t('City'),
			'address_state' => t('state'),
			'address_zip' => t('Zip'),
			'phone' => t('Phone'),
			'email' => t('Email'),
		);
	}

	public function addr_form() {
		foreach ($this->addr_keys() as $key => $title) {
			$form[$key] = array(
				'#type' => 'textfield',
				'#title' => $title,
			);
			if ($key == 'address_zip') {
				$form[$key]['#size'] = 9;
			}

			if ($key == 'address_state') {
				$form[$key]['#size'] = 2;
			}
		}
		return $form;
	}

	public function get_from_default() {
    $keys = $this->addr_keys();
    $settings = $this->settings();
    $from = array();
    foreach ($keys as $key => $title) {
      if ($settings['addr'][$key]) {
        $from[$key] = $settings['addr'][$key];
      }
    }
    return $from;
	}

	public function render_addr($addr, $mode) {
		if (!$addr) { return ''; }
    $keys = $this->addr_keys();
    $settings = $this->settings();

    $addr_in_order = array();
    $csz = '';
    foreach ($keys as $key => $prompt) {
    	$val = $addr[$key];
    	if (!$val) { continue; }

    	if (in_array($key, array('address_city', 'address_state', 'address_zip'))) {
    		if ($key == 'address_state' && $csz) { $csz .= ', '; }
    		if ($key == 'address_zip' && $csz) { $csz .= ' '; }
    		$csz .= $val;
    	} else {
	  		if ($csz) {
	  			$addr_in_order['csz'] = $csz; 
	  			$csz = '';
	  		}
      	$addr_in_order[$key] = $addr[$key];
    	}
    }
		if ($csz) {
			$addr_in_order['csz'] = $csz; 
			$csz = '';
		}

    if ($mode == 'oneline_txt') {
      $render = join(', ', array_filter($addr_in_order)); 
    } else {
      $render = join('<br />', array_filter($addr_in_order)); 
    }

    return $render;
	}

	public function render_option_label($key, $value) {
		if ($key == 'letterhead_nid') { return 'Letterhead'; }
		else { return ucfirst($key); }
	}

	public function render_option_value($key, $value) {
		if ($key == 'letterhead_nid') {
			$title = sirius_node_title($value);
			return $title . ' [' . $value . ']';
		} else if ($key == 'mail_type') {
			if (!$value) { $value = $this->mail_type_default(); }
			$types = $this->mail_types();
			return $types[$value];
		} else {
			return $value;
		}
	}
}
