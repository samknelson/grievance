<?php

class Sirius_Trust_Elgibility_Plugin_Geog extends Sirius_Trust_Elgibility_Plugin {
	public $type = 'geog';
	public $name = 'Geography';
	public $description = 'Eligibility for this benefit is restricted to workers living within a radius of a set of addresses.';

	public function form($json) {
		$address_count = $json['address_count'];
		if (!$address_count) { $address_count = 0; }

		$form['address_count'] = array(
			'#type' => 'textfield',
			'#size' => 4,
			'#title' => t('Number of Addresses'),
		);

		for ($delta = 0; $delta < $address_count; ++$delta) {
			$form['addresses'][$delta] = array(
				'#type' => 'fieldset',
				'#title' => t('Address #') . ($delta+1),
			);

			$form['addresses'][$delta]['address'] = array(
				'#type' => 'textfield',
				'#title' => t('Address'),
			);

			$form['addresses'][$delta]['lat'] = array(
				'#type' => 'textfield',
				'#title' => t('Lat'),
				'#disabled' => TRUE,
			);

			$form['addresses'][$delta]['lon'] = array(
				'#type' => 'textfield',
				'#title' => t('Lon'),
				'#disabled' => TRUE,
			);

			$form['addresses'][$delta]['radius'] = array(
				'#type' => 'textfield',
				'#title' => t('Radius (in miles)'),
				'#size' => 4,
			);
		}

		$form['inout'] = array(
			'#type' => 'select',
			'#title' => 'Inside or Outside?',
			'#description' => 'Select "inside" to require memebers to be within the radius. Select "outside" to requir members to be outside the radius.',
			'#options' => array(
				NULL => t('-- Select --'),
				'in' => t('Inside'),
				'out' => t('Outside'),
			),
		);

		sirius_jsonfield_util_set_defaults($form, $json);

		return $form;
	}

	public function form_submit(&$json) {
		if (!$json['addresses']) { return; }
		foreach ($json['addresses'] as $delta => $ignore) {
			$addr = $json['addresses'][$delta]['address'];
			if (!$addr) { 
				$json['addresses'][$delta]['lat'] = $json['addresses'][$delta]['lon'] = NULL;
			} else {
				$result = sirius_addr_geocode_impl($addr);
				if (!$result['success']) {
					drupal_set_message("Geocode failed for [$addr]: " . $result['msg'], 'warning');
					$json['addresses'][$delta]['lat'] = $json['addresses'][$delta]['lon'] = NULL;
				} else {
					$json['addresses'][$delta]['lat'] = $result['point']->coords[1];
					$json['addresses'][$delta]['lon'] = $result['point']->coords[0];
				}
			}
		}
	}

	public function check($settings, $params) {
		if (!$settings['addresses']) {
			return array('success' => FALSE, 'msg' => 'There are no addresses configured for this plugin.');
		}

		if ($params['addr'] && isset($params['addr']['lat']) && isset($params['addr']['lon'])) {
			$worker_lat = $params['addr']['lat'];
			$worker_lon = $params['addr']['lon'];
		} else {
			$worker_node = $params['subscriber_worker_node'];
			$contact_node = sirius_worker_contact_get($worker_node);
			if (!$contact_node) {
				return array('success' => FALSE, 'msg' => 'This worker does not have an associated contact record.');
			}

			$tools = array(
				l('Contact', "node/" . $worker_node->nid . "/sirius_worker_contact", array('attributes' => array('class' => array('tool-button')))),
			);

			if (!$contact_node->field_sirius_address_geo['und'][0]['geom']) {
				return array('success' => FALSE, 'msg' => 'This worker does not have a geocoded address.', 'tools' => $tools);
			}
			$worker_lat = $contact_node->field_sirius_address_geo['und'][0]['lat'];
			$worker_lon = $contact_node->field_sirius_address_geo['und'][0]['lon'];
		}

		$success = FALSE;
		foreach ($settings['addresses'] as $delta => $record) {
			$addr = $record['address'];
			if (!$record['lat'] && !$record['lon']) { continue; }
			$distance = round(sirius_addr_latlon_distance($worker_lat, $worker_lon, $record['lat'], $record['lon']), 4);
			$radius = $record['radius'];
			if ($distance <= $radius) {
				$success = TRUE;
				$msg = "The worker's address is $distance miles from $addr, which is less than the limit of $radius.";
				break;
			}
		}
		if (!$success) {
			$msg = "The worker's address is not within the required distance of any of the specified addresses.";
		}

		$inout = $settings['inout'];

		if ($inout == 'out') {
			$success = !$success;
		}

		return array(
			'success' => $success, 
			'msg' => $msg,
			'tools' => $tools,
		);
	}
}
