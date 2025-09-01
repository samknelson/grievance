<?php

class Sirius_Trust_Elgibility_Plugin_Cardcheck extends Sirius_Trust_Elgibility_Plugin {
	public $type = 'cardcheck';
	public $name = 'Cardcheck';
	public $description = 'Members are required to sign a cardcheck.';

	public function form($json) {
		if (!sirius_domain_component_enabled('cardcheck')) {
			return sirius_form_error("The cardcheck component is not enabled.");
		}

		$cardcheck = Sirius_Cardcheck::getInstance();
		$definitions = $cardcheck->definition_list();
		if (!$definitions) {
			return sirius_form_error("No cardcheck definitions have been created.");
		}

		$options = array();
		foreach ($definitions as $definition_nid => $definition_node) {
			$options[$definition_nid] = $definition_node->title;
		}

		for ($i=0; $i<3; ++$i) {
			$form['entries'][$i] = array(
				'#type' => 'fieldset',
				'#title' => 'Cardcheck #' . ($i+1),
			);

			$form['entries'][$i]['definition_nid'] = array(
				'#type' => 'select',
				'#title' => t('Cardcheck'),
				'#options' => array(NULL => t('-- Select --')) + $options,
			);

			/*
			$form['entries'][$i]['waivable'] = array(
				'#type' => 'select',
				'#title' => t('Can be Waived?'),
				'#options' => array(
					NULL => t('-- Select --'),
					'No' => t('No'), 
					'Yes' => t('Yes'),
				),
			);
			*/
		}

		sirius_jsonfield_util_set_defaults($form, $json);

		return $form;
	}

	public function check($settings, $params) {
		$entries = $settings['entries'];
		if (!$entries) {
			if (!$definition_nid) { return array('success' => TRUE, 'msg' => 'No cardcheck specified.'); }
		}

		$worker_node = $params['subscriber_worker_node'];
		if (!$worker_node) { return array('success' => FALSE, 'msg' => 'Worker node not found.'); }

		$tools = array();
		$tools[] = l('Cardchecks', 'node/' . $worker_node->nid . '/sirius_worker_cardchecks', array('attributes' => array('class' => array('tool-button'))));

		$msgs = array();
		foreach ($entries as $delta => $entry) {

			$definition_nid = $entry['definition_nid'];
			if (!$definition_nid) { continue; }

			$definition_node = node_load($definition_nid);

			/*
			if ($entry['waivable'] == 'Yes') {
				$msgs[] = "Waivable cardcheck: " . $definition_node->title;
				continue;
			}
			*/

			$cardcheck = Sirius_Cardcheck::getInstance();
			$search_params = array();
			$search_params['definition_nid'] = $definition_nid;
			$search_params['worker_nid'] = $worker_node->nid;
			$search_params['status'] = 'signed';
			$cardcheck_nodes = $cardcheck->search($search_params);

			if (!$cardcheck_nodes) {
				return array(
					'success' => FALSE,
					'msg' => "There is no signed cardcheck for: " . $definition_node->title,
					'tools' => $tools
				);
			}

			$msgs[] = "Found a signed cardcheck for: " . $definition_node->title;
		}

		return array(
			'success' => TRUE, 
			'msg' => join('<br />', $msgs),
			'tools' => $tools,
		);
	}
}
