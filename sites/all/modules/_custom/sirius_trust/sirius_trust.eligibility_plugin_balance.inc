<?php

class Sirius_Trust_Elgibility_Plugin_Balance extends Sirius_Trust_Elgibility_Plugin {
	public $type = 'balance';
	public $name = 'Balance';
	public $description = 'Members lose eligiblity if their balance in a fund exceeds a certain amount.';

	public function form($json) {
		if (!sirius_ledger_account_list()) {
			return sirius_form_error("There are no ledger accounts.");
		}

		$form['account'] = array(
			'#type' => 'select',
			'#title' => t('Account'),
			'#options' => array(NULL => t('-- Select --')) + sirius_ledger_account_list(),
		);

		$form['warning_balance'] = array(
			'#type' => 'textfield',
			'#size' => 4,
			'#title' => t('Warning Balance'),
			'#description' => t('Generate a warning when the balance exceeds this amount.'),
		);

		$form['max_balance'] = array(
			'#type' => 'textfield',
			'#size' => 4,
			'#title' => t('Maximum Outstanding Balance'),
			'#description' => t('Remove eligibility when the balance exceeds this amount.'),
		);

		sirius_jsonfield_util_set_defaults($form, $json);

		return $form;
	}

	public function check($settings, $params) {
		$account_nid = $settings['account'];
		if (!$account_nid) {
			return array('success' => TRUE, 'msg' => 'No account specified.');
		}
		$currency = sirius_ledger_currency_from_account_nid($account_nid);

		$max_balance = $settings['max_balance'];
		if (!$max_balance) { $max_balance = 0; }
		if (!is_numeric($max_balance)) { return array('success' => FALSE, 'msg' => 'Please enter a numeric value for max balance.'); }
		$max_balance_render = $currency->render_amt($max_balance);

		$warning_balance = $settings['warning_balance'];
		if (!$warning_balance) { $warning_balance = 0; }
		if (!is_numeric($warning_balance)) { return array('success' => FALSE, 'msg' => 'Please enter a numeric value for warning balance.'); }
		$warning_balance_render = $currency->render_amt($warning_balance);

		$ea = Sirius_Ledger_EA::getInstance();
		$ea_node = $ea->fetch($params['subscriber_worker_node']->nid, $account_nid, FALSE);
		if ($ea_node) {
			$balance = $ea->balance($ea_node);
		} else {
			$balance = '0';
		}
		$balance_render = $currency->render_amt($balance);

		if ($balance > $max_balance) {
			return array('success' => FALSE, 'msg' => "The member's balance, $balance_render, is greater than $max_balance_render");
		}

		if ($warning_balance && ($balance > $warning_balance)) {
			return array(
				'success' => TRUE, 
				'msg' => "The member's balance, $balance_render, is greater than the warning threshold of $warning_balance_render but less than the maximum balance of $max_balance_render.",
				'warning' => TRUE,
			);
		}

		return array('success' => TRUE, 'msg' => "The member's balance, $balance_render, is less than $max_balance_render");
	}
}
