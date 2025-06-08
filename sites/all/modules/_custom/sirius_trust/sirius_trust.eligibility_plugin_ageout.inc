<?php

class Sirius_Trust_Elgibility_Plugin_Ageout extends Sirius_Trust_Elgibility_Plugin {
	public $type = 'ageout';
	public $name = 'Age Out';
	public $description = 'Deny eligibility to selected relationship types if they affected dependent has reached a specified age.';

	public function form($json) {
		$form['reltypes'] = array(
			'#type' => 'checkboxes',
			'#title' => t('Relationship Types'),
			'#options' => array('[self]' => t('[Self]')) + sirius_taxonomy_load('sirius_contact_relationship_types', 'tid', 'name'),
		);

		$form['min_age'] = array(
			'#type' => 'textfield',
			'#size' => 4,
			'#title' => t('Minimum Age (in years)'),
		);

		$form['max_age'] = array(
			'#type' => 'textfield',
			'#size' => 4,
			'#title' => t('Maximum Age (in years)'),
		);

		$form['max_age_warning'] = array(
			'#type' => 'textfield',
			'#size' => 4,
			'#title' => t('Maximum Age Warning (in years)'),
			'#description' => 'Generate a warning if the age is greater than this value.',
		);

		sirius_jsonfield_util_set_defaults($form, $json);

		return $form;
	}

	public function check($settings, $params) {
		if (!$settings['reltypes']) { 
			return array('success' => TRUE, 'msg' => 'This plugin is not configured.');
		}
		$reltypes = array_filter(array_values($settings['reltypes']));
		if (!$reltypes) {
			return array('success' => TRUE, 'msg' => 'No relationship types configured.');
		}

		$min_age = $settings['min_age'];
		if (!$min_age) { $min_age = 0; }
		if (!is_numeric($min_age)) { return array('success' => FALSE, 'msg' => 'Please enter a numeric value for min_age.'); }

		$max_age = $settings['max_age'];
		if (!$max_age) { $max_age = 0; }
		if (!is_numeric($max_age)) { return array('success' => FALSE, 'msg' => 'Please enter a numeric value for max_age.'); }

		$max_age_warning = $settings['max_age_warning'];
		if (!$max_age_warning) { $max_age_warning = 0; }
		if (!is_numeric($max_age_warning)) { return array('success' => FALSE, 'msg' => 'Please enter a numeric value for max_age_warning.'); }

		if (!$min_age && !$max_age) {
			return array('success' => TRUE, 'msg' => 'There is no minimum or maximum age set.');
		}

		if (!$params['relationship_node']) {
			if (!in_array('[self]', $reltypes)) {
				return array('success' => TRUE, 'msg' => 'Age out requirement does not apply to [Self].');
			} else {
				$worker_node = $params['subscriber_worker_node'];
			}
		}

		if ($params['relationship_node']) {
			$reltype_tid = $params['relationship_node']->field_sirius_contact_reltype['und'][0]['tid'];
			if (!$reltype_tid) { return array('success' => FALSE, 'msg' => 'The relationship does not have a type.'); }
			if (!in_array($reltype_tid, $reltypes)) {	return array('success' => TRUE, 'msg' => 'Age out requirement does not apply to ' . sirius_term_title($reltype_tid)); }

			$contact_nid = $params['relationship_node']->field_sirius_contact_alt['und'][0]['target_id'];
			if (!$contact_nid) {
				return array('succes' => FALSE, 'msg' => 'The relationship does not point to a contact.');
			}
			$worker_node = sirius_worker_get_by_contact_nid($contact_nid);
			if (!$worker_node) { 
				return array('success' => FALSE, 'msg' => "There is no worker associated with contact nid $contact_nid");
			}
		}

		$dob = $worker_node->field_sirius_dob['und'][0]['value'];
		if (!$dob) {
			return array('success' => FALSE, 'msg' => "There is no birth date set for " . $worker_node->title);
		}
		$from = new DateTime(date('Y-m-1', strtotime($dob))); // Beginning of the birth month
		$to = new DateTime(date('Y-m-t', $params['ts'])); // End of the current month
		$diff = $from->diff($to);
		// drupal_set_message("Difference is " . $diff->y . " years " . $diff->m . " months " . $diff->d . " days ");

		$age_in_months = (12*$diff->y) + $diff->m + 1;
		$age = round(($age_in_months / 12), 2);

		if ($age < $min_age) {
			return array('success' => FALSE, 'msg' => $worker_node->title . " has an age of $age years, which is less than the minimum age of $min_age.");
		}

		if ($age > $max_age) {
			return array('success' => FALSE, 'msg' => $worker_node->title . " has an age of $age years, which is greater than the maximum age of $max_age.");
		}

		if ($age > $max_age_warning) {
			return array('success' => TRUE, 'msg' => $worker_node->title . " has an age of $age years, which is less than the maximum age of $max_age, but greater than the warning age of $max_age_warning.", 'warning' => TRUE);
		}

		return array('success' => TRUE, 'msg' => $worker_node->title . " has an age of $age years, which is between $min_age and $max_age.");
	}
}
