<?php

function sirius_ledger_node_tab_ea_access($ea_node) {
	if ($ea_node->field_sirius_category['und'][0]['value'] != 'ledger:ea') { return FALSE; }
	return Sirius_Ledger_Access::getInstance()->access('ea_view', array('ea_node' => $ea_node));
}

function sirius_ledger_node_tab_ea($ea_node) {
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');

	$ea = Sirius_Ledger_EA::getInstance();
	$access = Sirius_Ledger_Access::getInstance();
	$participant_node = $ea->participant_node($ea_node);
	$account_node = $ea->account_node($ea_node);
	$account_json = sirius_json_get($account_node);
	$entity_node = $ea->entity_node($ea_node);

	if ($access->access('ea_view', array('ea_node' => $ea_node))) { 
		$balance = $ea->balance($ea_node);
		$currency = sirius_ledger_currency_from_account_nid($account_nid);
		$balance_render = $currency->render_amt($balance);

		$html .= '<div class="sirius_callout_basic">';
		$html .= '<h2>' . t('Outstanding Balance') . '</h2>';
		$html .= sirius_fakerender_field('Balance Due:', $balance_render);
		$html .= '<br />';

		$tools = array();
		$tools['node/' . $ea_node->nid . '/sirius_ea_invoices'] = 'Invoices';
		$tools['node/' . $ea_node->nid . '/sirius_ea_ar'] = 'All Transactions';
		foreach ($tools as $path => $title) {
			if (!drupal_valid_path($path)) { continue; }
			$html .= l($title, $path, array('attributes' => array('class' => array('tool-button'))));
		}
		$html .= '</div>';


		$params = array();
		$params['account_nid'] = $account_node->nid;
		$params['payer_nid'] = $participant_node->nid;
		$params['limit'] = 5;
		$payments = sirius_ledger_payments_search($params);

		$html .= '<div class="sirius_callout_basic">';
		$html .= '<h2>' . t('Recent Payments and Adjustments') . '</h2>';
		$options = array();
		$options['hide_account'] = TRUE;
		$html .= sirius_ledger_payments_render($payments, $options);

		$tools = array();
		$tools['node/' . $ea_node->nid . '/sirius_ea_payments'] = 'All Payments and Adjustments';
		$tools['node/' . $ea_node->nid . '/sirius_ea_stripe_payment'] = 'Make a Payment';
		$tools['node/' . $ea_node->nid . '/sirius_ea_payment_create'] = 'Enter an Offline Payment or Adjustment';
		foreach ($tools as $path => $title) {
			if (!drupal_valid_path($path)) { continue; }
			$html .= l($title, $path, array('attributes' => array('class' => array('tool-button'))));
		}

		$html .= '</div>';
	}

	if ($access->access('ea_view_employer_workers', array('ea_node' => $ea_node))) {
		//
	}

	if ($account_json['settings']['description']) {
		$html .= '<div class="sirius_callout_basic">';
		$html .= $account_json['settings']['description'];
		$html .= '</div>';
	}

	return $html;
}
