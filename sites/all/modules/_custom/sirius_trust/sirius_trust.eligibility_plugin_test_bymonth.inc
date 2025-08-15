<?php

class Sirius_Trust_Elgibility_Plugin_Test_Bymonth extends Sirius_Trust_Elgibility_Plugin {
	public $type = 'test_bymonth';
	public $name = 'Test By Month';
	public $description = 'Force eligibility for specific months.';

	public function month_options() { 
		return array(
			1 => t('January'),
			2 => t('February'),
			3 => t('March'),
			4 => t('April'),
			5 => t('May'),
			6 => t('June'),
			7 => t('July'),
			8 => t('August'),
			9 => t('September'),
			10 => t('October'),
			11 => t('November'),
			12 => t('December'),
		);
	}

	public function form($json) {
		$month_options = $this->month_options();
		$form['months'] = array(
			'#title' => 'Months', 
			'#type' => 'checkboxes',
			'#options' => $month_options,
			'#default_value' => sirius_form_array_default_value($json['months']),
			'#checkall' => TRUE,
			'#description' => 'Workers will be eligible for the benefit in the specified months only.',
		);

		return $form;
	}

	public function check($settings, $params) {
		$month_options = $this->month_options();

		$ts = $params['ts'];
		$month = date('n', $ts);

		$ok_months = $settings['months'];
		if (!$ok_months) { $ok_months = array(); }
		$ok_months = array_filter(array_values($ok_months));

		if (in_array($month, $ok_months)) {
			return array(
				'success' => TRUE, 
				'msg' => "The asof month, " . $month_options[$month] . ", is flagged as eligible.",
				'tools' => $tools,
			);
		}

		return array(
			'success' => FALSE, 
			'msg' => "The asof month, " . $month_options[$month] . ", is not flagged as eligible.",
			'tools' => $tools,
		);
	}
}
