<?php

//
// This is both an "Access" and a logic module. 
// 

class Sirius_Smf_Echp extends Sirius_Access {
	public function price($hrs_worked) {
		if ($hrs_worked < 40) { return 750; }
		if ($hrs_worked < 44) { return 540; }
		if ($hrs_worked < 49) { return 515; }
		if ($hrs_worked < 54) { return 490; }
		if ($hrs_worked < 59) { return 465; }
		if ($hrs_worked < 64) { return 440; }
		if ($hrs_worked < 69) { return 415; }
		if ($hrs_worked < 74) { return 390; }
		if ($hrs_worked < 79) { return 365; }
		if ($hrs_worked < 84) { return 340; }
		if ($hrs_worked < 89) { return 315; }
		if ($hrs_worked < 94) { return 290; }
		if ($hrs_worked < 100) { return 265; }
		return 0;
	}

	public function build_context(&$context) {
		// Get the event center hours employer
		$echp_employer_nid = sirius_find_nid_by_id('ECHP', 'grievance_shop');
		$context['echp_employer_node'] = node_load($echp_employer_nid);

		// Some of this is for eligibility plugin. Might as well build a complete context that anybody can use.
		if (!$context['ts']) {
			$context['ts'] = time();
		}
		$context['subscriber_worker_node'] = $context['worker_node'];

		// Get the worker's election
		$context['election_node'] =  sirius_trust_worker_election($context['worker_node']);

		// Get the worker's policy
		$context['policy_node'] = node_load($context['election_node']->field_sirius_trust_policy['und'][0]['target_id']);

		// Get the worker's home employer
		$context['employer_node'] = node_load($context['election_node']->field_grievance_shop['und'][0]['target_id']);

		// Get the work status
		// $context['ws_tid'] = sirius_worker_history_asof($context['worker_node'], 'worker:work_status', time());
		// $context['ws_term'] = taxonomy_term_load($context['ws_tid']);

		// Get some buildup stuff
		$context['buildup_info'] = sirius_trust_eligibility_plugin_info('smf-buildup');
		$context['buildup'] = $context['buildup_info']['instance'];
		$context['hrs_threshold'] = round($context['buildup']->threshold_from_worker_and_policy($context['worker_node'], $context['policy_node']), 2);
		$context['buildup_result'] = $context['buildup']->check(array(), $context);

		// Target month
		$context['target_y'] = $context['buildup_result']['current_break_first_year'];
		$context['target_m'] = $context['buildup_result']['current_break_first_month'];
		$context['hrs_worked'] = round($context['buildup_result']['current_break_first_hrs'], 2);
		$context['hrs_needed'] = round($context['hrs_threshold'] - $context['hrs_worked'], 2);
		$context['price'] = $this->price($context['hrs_worked']);

		// Get the most recent status at the home employer
		$sql = "select * from sirius_hours_cache where worker_nid = :worker_nid and employer_nid = :employer_nid order by year desc, month desc, day desc limit 1";
		$stmt = sirius_sql_query($sql, array(':worker_nid' => $context['worker_node']->nid, ':employer_nid' => $context['employer_node']->nid));
		$hr = $stmt->fetchAssoc();
		$context['last_hours'] = $hr;
		$context['last_hours_type_term'] = taxonomy_term_load($hr['hours_type_tid']);

		return array('success' => TRUE);
	}

	public function access($action, $context = array()) {
		if ($action == 'purchase') {
			// Staff can always get in
			if (user_access('sirius trust admin') && $context['staff_bypass']) {
				return TRUE;
			}

			$d = date('j', $context['ts']);
			if ($d > 15) { return $this->deny('Online purchasing for this month is closed. Please contact the office for assistance.'); }

			if (!$context['election_node']) { return $this->deny("The worker has no election."); }
			if (!$context['policy_node']) 	{ return $this->deny("There is no policy associated with the election."); }
			if (!$context['employer_node']) { return $this->deny('No employer associated with the election.'); }
			if (!$context['echp_employer_node']) { return $this->deny("No event center hours purchase employer (employer with id ECHP)."); }

			if ($context['last_hours_type_term']->field_sirius_id['und'][0]['value'] == 'terminated') {
				return $this->deny('You have been marked as Terminated, which means you are ineligible for the Hours Buy-Up program. If you wish to maintain coverage, you may be eligible to make a COBRA election');
			}

			$result = $this->build_context($context);
			if (!$result['success']) { return $this->deny('Unable to complete context: ' . $result['msg']); }

			// Event center policy?
			if (!preg_match('/event center/i', $context['policy_node']->title)) {
				return $this->deny("The policy, " . $context['policy_node']->title . ", is not an Event Center policy.");
			}

			// Active?
			/*
			if ($context['ws_term']->field_sirius_member_active['und'][0]['value'] != 'Yes') {
				return $this->deny("The current work status, " . $ws_term->name . ', is no active.');
			}
			*/

			if (!$context['buildup_result']['success']) {
				return $this->deny("Buildup is not complete.");
			}

			if (!$context['target_y']) { return $this->deny("There are no break months that need purchasing."); }

			$m = date('n', $context['ts']);
			$y = date('Y', $context['ts']);
			$m -= 3;
			if ($m < 1) { $m += 12; $y--; }
			if ( ($m != ($context['target_m'] + 0)) || ($y != ($context['target_y'] + 0)) ) {
				return $this->deny("Retroactive coverage cannot be purchased online. Please contact an administrator to update prior months. (Hours needed for $context[target_m] / $context[target_y])");
			}

			if (!$context['hrs_needed']) { return $this->deny("No hours are needed."); }

			$purchases_this_year = $this->purchases_this_year($context);
			if (count($purchases_this_year) >= 6) {
				return $this->deny('You have already used the hours purchase service six times this year.');
			}

			return TRUE;
		}
	}

	public function hour_type_tid() {
		return sirius_taxonomy_lookup_by_anything('sirius_hour_type', 'echp');
	}

	public function track_purchase($context) {
		$worker_json = sirius_json_get($context['worker_node']);
		$key = date('Y');
		$value = $context['target_y'] . '-' . $context['target_m'];
		$worker_json['smf']['echp'][$key][$value] = TRUE;
		sirius_json_set($context['worker_node'], $worker_json);
		node_save($context['worker_node']);
	}



	public function purchases_this_year($context) {
		// We used to define "purchases_this_year" as the number of times you used the service in a calendar year.
		// We now define it as the number of times you purchased hours *for* a given calendar year.
		/*
		$key = date('Y');
		$worker_json = sirius_json_get($context['worker_node']);
		$result = $worker_json['smf']['echp'][$key];
		if (!$result) { $result = array(); }
		return $result;
		*/

		// Get the most recent status at the home employer
		$sql = "select * from sirius_hours_cache where worker_nid = :worker_nid and employer_nid = :employer_nid and year = :year and hours_type_tid = :hours_type_tid and hours > 0 ";
		$stmt = sirius_sql_query($sql, 
			array(
				':worker_nid' => $context['worker_node']->nid, 
				':employer_nid' => $context['echp_employer_node']->nid,
				':year' => $context['target_y'],
				':hours_type_tid' => $this->hour_type_tid(),
			)
		);
		$result = array();
		while ($hr = $stmt->fetchAssoc()) { $result[] = $hr; }
		return $result;
	}
}
