<?php

function sirius_ledger_page_policies() {
  $domain_nid = sirius_domain_nid();
  $type = 'sirius_ledger:policy';

  $html = '';
  $policies = sirius_ledger_policy_list();

  $trust_active = function_exists('sirius_trust_policy_benefits_offered');

  if ($policies) {
		$rows = array();
	  foreach ($policies as $policy_nid => $policy_node) {
	  	$policy_json = sirius_json_get($policy_node);
	  	$title = sirius_tooltip_render($policy_json['ledger']['policy']['description'], $policy_node->title);

	  	$benefit_names = array();
	  	if ($trust_active) {
				$benefits = sirius_trust_policy_benefits_offered($policy_node);
				foreach ($benefits as $benefit_nid => $benefit_node) { $benefit_names[] = $benefit_node->title; }
			}				

	  	$paths = array();
	  	$paths['View'] = "node/$policy_nid";
			$paths['Benefits'] = "node/$policy_nid/sirius_trust_policy_benefits";
			if ($trust_active) {
				$paths['Charge Plugins'] = "node/$policy_nid/sirius_ledger_policy_charge_plugins";
				$paths['Elections'] = "node/$policy_nid/sirius_trust_policy_election";
			}

			$tools = array();
			foreach ($paths as $label => $path) {
				if (drupal_valid_path($path)) {
			  	$tools[] = l($label, $path, array('attributes' => array('class' => array('tool-button'))));
				}
			}

	  	$row = array();
	  	$row[] = $title;
	  	if ($trust_active) {
	  		$row[] = join(' / ', $benefit_names);
	  	}
	  	$row[] = join('', $tools);
	  	$rows[] = $row;
	  }

	  $header = array();
	  $header[] = t('Title');
	  if ($trust_active) {
	  	$header[] = t('Benefits');
	  }
	  $header[] = t('Tools');

	  $html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
	} else {
		$html .= '<p>' . t('No policies have been defined.') . '</p>';
	}

  $html .= l('Add a Policy', 'node/add/sirius-json-definition', array('attributes' => array('class' => array('tool-button')), 'query' => array('sirius_type' => $type)));
  return $html;
}

