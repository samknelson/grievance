<?php

class Sirius_Ledger_Charge_Plugin_Monthly extends Sirius_Ledger_Charge_Plugin {
	public $type = 'monthly';
	public $name = 'Monthly';
	public $description = 'Generate fixed monthly charges at all participating employers.';

	public function trigger_enabled($trigger_key) { return ($trigger_key == 'monthly'); }

	public function weekdays() {
		return array(
			1 => 'Monday',
			2 => 'Tuesday',
			3 => 'Wednesday',
			4 => 'Thursday',
			5 => 'Friday',
			6 => 'Saturday',
			7 => 'Sunday',
		);
	}

	public function weeknumber($ts) {
		// We just want a function that returns a consistent value no matter what the timezone. And doesn't get
		// fussed about leap-seconds or crap like that. So we jump to noon GMT.

		static $start_ts;
		if (!isset($start_ts)) { $start_ts = strtotime('12:00 PM GMT, January 1, 1980'); }
		$ts = strtotime("12:00 PM GMT", $ts);
		// drupal_set_message("Here with " . date('Y-m-d H:i:s', $ts) . " and " . date('Y-m-d H:i:s', $start_ts));

		return floor(($ts-$start_ts) / (24 * 60 * 60 * 7));
	}

	public function entity_settings_form($form, &$form_state, $entity_node, $plugin_uuid) {
		$form = parent::entity_settings_form($form, $form_state, $entity_node, $plugin_uuid);

		$ws_options = sirius_taxonomy_load('sirius_work_status', 'tid', 'name');
		$form['settings']['ws'] = array(
			'#type' => 'checkboxes',
			'#title' => t('Work Statuses'),
			'#options' => $ws_options,
		);

		$form['settings']['schedule'] = array(
			'#type' => 'select',
			'#title' => t('Schedule'),
			'#options' => array(
				NULL => t('-- Select --'),
				'monthly' => 'Once a month',
				'semimonthly' => 'Twice a month',
				'weekly' => 'Once a week',
				'biweekly' => 'Every other week',
			),
			'#id' => 'settings_schedule',
			'#required' => TRUE,
		);

		$form['settings']['monthly'] = array(
			'#type' => 'fieldset',
			'#title' => 'Monthly Settings',
			'#states' => array('visible' => array(":input[id=\"settings_schedule\"]" => array('value' => 'monthly'))),
		);

		$form['settings']['monthly']['charge_day'] = array(
			'#type' => 'textfield',
			'#title' => t('Once a Month Settings'),
			'#size' => 2,
			'#description' => t('On which day of the month should the charge be entered?')
		);

		$form['settings']['semimonthly'] = array(
			'#type' => 'fieldset',
			'#title' => 'Twice a Month Settings',
			'#states' => array('visible' => array(":input[id=\"settings_schedule\"]" => array('value' => 'semimonthly'))),
		);

		$form['settings']['semimonthly']['charge_day_1'] = array(
			'#type' => 'textfield',
			'#title' => t('First Monthly Charge Day'),
			'#size' => 2,
		);

		$form['settings']['weekly'] = array(
			'#type' => 'fieldset',
			'#title' => 'Weekly',
			'#states' => array('visible' => array(":input[id=\"settings_schedule\"]" => array('value' => 'weekly'))),
		);

		$form['settings']['weekly']['charge_weekday'] = array(
			'#type' => 'select',
			'#title' => t('Charge Day'),
			'#options' => array(NULL => t('-- Select --')) + $this->weekdays(),
		);

		$form['settings']['biweekly'] = array(
			'#type' => 'fieldset',
			'#title' => 'Every Other Week',
			'#states' => array('visible' => array(":input[id=\"settings_schedule\"]" => array('value' => 'biweekly'))),
		);

		$form['settings']['biweekly']['charge_weekday'] = array(
			'#type' => 'select',
			'#title' => t('Charge Day'),
			'#options' => array(NULL => t('-- Select --')) + $this->weekdays(),
		);

		sirius_jsonfield_util_set_defaults($form['settings'], $this->entity_settings($entity_node, $plugin_uuid));

		return $form;
	}

	public function entity_settings_form_validate($form, &$form_state) {
		$schedule = $form_state['values']['settings']['schedule'];

		if ($schedule == 'monthly') {
			$day_input = $form_state['values']['settings']['monthly']['charge_day'];
			$day = (int) $day_input;
			if (!$day || ($day != $day_input) || ($day < 1) || ($day > 28)) {
				form_set_error('settings][monthly][charge_day', 'Please enter an integer between 1 and 28.');
			}
		}

		if ($schedule == 'semimonthly') {
			$day_input_1 = $form_state['values']['settings']['semimonthly']['charge_day_1'];
			$day_1 = (int) $day_input_1;

			$day_input_2 = $form_state['values']['settings']['semimonthly']['charge_day_2'];
			$day_2 = (int) $day_input_2;

			if (!$day_1 || ($day_1 != $day_input_1) || ($day_1 < 1) || ($day_1 > 28)) {
				form_set_error('settings][semimonthly][charge_day_1', 'Please enter an integer between 1 and 28.');
			}

			if (!$day_2 || ($day_2 != $day_input_2) || ($day_2 < 1) || ($day_2 > 28) || ($day_2 <= $day_1)) {
				form_set_error('settings][semimonthly][charge_day_2', 'Please enter an integer between 1 and 28 that is greater than the first charge day.');
			}
		}

		if ($schedule == 'weekly') {
			$day = $form_state['values']['settings']['weekly']['charge_weekday'];
			if (!$day) { form_set_error('settings][weekly][charge_weekday', 'Please select a weekday.'); }
		}

		if ($schedule == 'biweekly') {
			$day = $form_state['values']['settings']['biweekly']['charge_weekday'];
			if (!$day) { form_set_error('settings][biweekly][charge_weekday', 'Please select a weekday.'); }
		}

		return parent::entity_settings_form_validate($form, $form_state);
	}

	public function charge(&$context, $mode = 'test') {
		$day = date('j', $context['ts']);
		$weekday = date('N', $context['ts']);
		$week = date('W', $context['ts']);
		$weekdays = $this->weekdays();
		$week = $this->weeknumber($context['ts']);

		// Legacy support
		$schedule = $context['policy_settings']['schedule'];

		if (!$schedule) { return array('success' => FALSE, 'msg' => "Configuration error, no schedule set."); }

		if ($schedule == 'monthly') {
			$charge_day = $context['policy_settings']['monthly']['charge_day'];
			sirius_minilog("Executing monthly charge, day = $day, chage day = $charge_day");

			if (!$charge_day) { return array('success' => FALSE, 'msg' => "Configuration error, no charge day set."); }

			if ($charge_day > $day) {
				sirius_minilog("Charges can't be made until day $charge_day of each month. It is only day $day.");
				return array('success' => TRUE);
			}

			$ledger_key = $context['worker_node']->nid . '|' . date('Y-m', $context['ts']);
		}

		if ($schedule == 'semimonthly') {
			$charge_day_1 = $context['policy_settings']['semimonthly']['charge_day_1'];
			$charge_day_2 = $context['policy_settings']['semimonthly']['charge_day_2'];
			sirius_minilog("Executing semimonthly charge, day = $day, first chage day = $charge_day_1, second charge day = $charge_day_2");

			if (!$charge_day_1 || !$charge_day_2) { return array('success' => FALSE, 'msg' => "Configuration error, charge days not set."); }

			if ($charge_day_1 > $day) {
				sirius_minilog("Charges can't be made until day $charge_day_1 of each month.");
				return array('success' => TRUE);
			} else if ( ($charge_day_1 <= $day) && ($charge_day_2 > $day) ) {
				$ledger_key = $context['worker_node']->nid . '|' . date('Y-m', $context['ts']) . '|CHARGE1';
				sirius_minilog("Executing first monthly charge (key = $ledger_key)");
			} else {
				$ledger_key = $context['worker_node']->nid . '|' . date('Y-m', $context['ts']) . '|CHARGE2';
				sirius_minilog("Executing second monthly charge (key = $ledger_key)");
			}
		}


		if ($schedule == 'weekly') {
			$charge_weekday = $context['policy_settings']['weekly']['charge_weekday'];
			sirius_minilog("Executing weekly charge, day = " . $weekdays[$weekday] . ", chage day = " . $weekdays[$charge_weekday]);

			if (!$charge_weekday) { return array('success' => FALSE, 'msg' => "Configuration error, charge weekday not set."); }

			if ($charge_weekday > $weekday) {
				sirius_minilog("Charges can't be made until " . $weekdays[$charge_weekday] . " of each week.");
				return array('success' => TRUE);
			}

			$ledger_key = $context['worker_node']->nid . '|' . date('Y-W', $context['ts']);
			sirius_minilog("Executing weekly charge (key = $ledger_key)");
		}

		if ($schedule == 'biweekly') {
			$charge_weekday = $context['policy_settings']['biweekly']['charge_weekday'];
			sirius_minilog("Executing biweekly charge, day = " . $weekdays[$weekday] . ", chage day = " . $weekdays[$charge_weekday]);

			if (!$charge_weekday) { return array('success' => FALSE, 'msg' => "Configuration error, charge weekday not set."); }

			if ($charge_weekday > $weekday) {
				sirius_minilog("Charges can't be made until " . $weekdays[$charge_weekday] . " of each week.");
				return array('success' => TRUE);
			}

			if (! ($week % 2)) {
				sirius_minilog("Charges are only made on odd-number weeks; we are currently in week #$week.");
				return array('success' => TRUE);
			}

			$ledger_key = $context['worker_node']->nid . '|' . $week;
			sirius_minilog("Executing weekly charge (key = $ledger_key)");
		}

		$history_item = $this->util_history_lookup($context['employer_settings']['rates']['history'], $context['ts']);
		if (!$history_item) {
			sirius_minilog("No rate record found; no charges generated.");
			return array('success' => TRUE); 
		}

		$amt = $history_item['rate'];
		sirius_minilog("Found rate \$" . number_format($amt, 2) . " effective $history_item[date]");

		$ws_restriction = $context['policy_settings']['ws'];
		if (!$ws_restriction) { $ws_restriction = array(); }
		$ws_restriction = array_filter(array_values($ws_restriction));
		if ($ws_restriction) {
			$ws_asof = sirius_worker_history_asof($context['worker_node'], 'worker:work_status', $context['ts']);
			if (!in_array($ws_asof, $ws_restriction)) {
				sirius_minilog("The worker had the work status " . sirius_term_title($ws_asof) . ", which is not one of the specified statuses. Setting rate = 0."); 
				$amt = 0;
			}
		}

		if (!$ledger_key) {
			return array('success' => FALSE, 'msg' => "System error, no ledger key.");
		}

	  $entry = array();
	  $entry['ledger_reference'] = $context['policy_node']->nid;
	  $entry['ledger_key'] = $ledger_key;
	  $entry['ledger_memo'] = "Monthly: $amt";
	  $entry['ledger_amount'] = $amt;

	  $context['entry'] = $entry;
		return parent::charge($context, $mode);
	}

	public function employer_settings_enabled() { return TRUE; }

	public function employer_settings_form($form, &$form_state, $employer_node, $policy_node, $plugin_uuid) {
		$form_state['employer_nid'] = $employer_node->nid;

		$form['#tree'] = TRUE;

		$employer_settings = $this->employer_settings($employer_node, $plugin_uuid);

		$form['settings']['rates'] = array(
			'#type' => 'fieldset',
			'#title' => 'Rate History',
		);

		$form['settings']['rates']['history'] = $this->util_history_form($employer_settings['rates']['history'], array());

		sirius_jsonfield_util_set_defaults($form['settings'], $employer_settings);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);

		return $form;
	}

	public function employer_settings_form_submit($form, &$form_state) {
		$this->util_history_cleanup_form($form_state['values']['settings']['rates']['history']);
		return parent::employer_settings_form_submit($form, $form_state);
	}
}
