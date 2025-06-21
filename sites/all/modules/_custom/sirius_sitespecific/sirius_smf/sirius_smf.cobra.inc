<?php

class Sirius_Smf_Cobra extends Sirius_Singleton {
	public function rate_from_worker($worker_node) {
		if (!$worker_node) {
			$msg = "No worker.";
			sirius_minilog($msg, 'error'); return array('success' => FALSE, 'msg' => $msg);
		}

		sirius_minilog("Computing COBRA rate for " . $worker_node->title . ' (' . $worker_node->nid . ')');
		$election_node = sirius_trust_worker_election($worker_node);
		if (!$election_node) {
			$msg = "No election found.";
			sirius_minilog($msg, 'error'); return array('success' => FALSE, 'msg' => $msg);
		}

		return $this->rate_from_election($election_node);
	}

	public function rate_from_election($election_node) {
		sirius_minilog("Computing cobra rate for " . $election_node->title . ' (' . $election_node->nid . ')');

		// Get the "Tier" (Single, Single+1, Family)
		$relationship_nids = sirius_fieldvals($election_node, 'field_sirius_contact_relations', 'target_id');
		$relationship_count = count($relationship_nids);
		if ($relationship_count == 0) {
			$tier = 'Single';
		} else if ($relationship_count == 1) {
			$tier = 'Single+1';
		} else {
			$tier = 'Family';
		}
		sirius_minilog("Found $relationship_count relationships; using tier $tier.");

		// Get the benefits
		$benefit_nids = sirius_fieldvals($election_node, 'field_sirius_trust_benefits', 'target_id');
		$medical = $dental = '';
		foreach ($benefit_nids as $benefit_nid) {
			$benefit_node = node_load($benefit_nid);
			$benefit_type_tid = $benefit_node->field_sirius_trust_benefit_type['und'][0]['tid'];
			$benefit_type_term = taxonomy_term_load($benefit_type_tid);

			sirius_minilog("Checking benefit: " . $benefit_node->title);

			// @todo: Really need to standardize benefit types
			if (preg_match('/medical/i', $benefit_type_term->name)) {
				if ($medical) {
					$msg = "Found two medical benefits.";
					sirius_minilog($msg, 'error'); return array('success' => FALSE, 'msg' => $msg);
				}
				$medical = $benefit_node->field_sirius_id['und'][0]['value'];
				if (!$medical) {
					$msg = "Benefit has no value in the 'ID' field. Can't compute the rate.";
					sirius_minilog($msg, 'error'); return array('success' => FALSE, 'msg' => $msg);
				}
				if (!in_array($medical, array('K', 'H', 'M'))) {
					$msg = "Don't know how to compute the COBRA rate for benefit with ID $medical.";
					sirius_minilog($msg, 'error'); return array('success' => FALSE, 'msg' => $msg);
				}
				sirius_minilog("... Found medical benefit: $medical");
			} else if (preg_match('/dental/i', $benefit_type_term->name)) {
				if ($dental) {
					$msg = "Found two dental benefits.";
					sirius_minilog($msg, 'error'); return array('success' => FALSE, 'msg' => $msg);
				}
				$dental = $benefit_node->field_sirius_id['und'][0]['value'];
				if (!$dental) {
					$msg = "Benefit has no value in the 'ID' field. Can't compute the rate.";
					sirius_minilog($msg, 'error'); return array('success' => FALSE, 'msg' => $msg);
				}
				if (!in_array($dental, array('L', 'D'))) {
					$msg = "Don't know how to compute the COBRA rate for benefit with ID $dental.";
					sirius_minilog($msg, 'error'); return array('success' => FALSE, 'msg' => $msg);
				}
				sirius_minilog("... Found dental benefit: $dental");
			} else {
				sirius_minilog("... Benefit is not a medical or dental benefit. Skipping.");
				continue; 
			}
		}

		$rates = array();
		$rates['K']['']['Single'] = 769.15;
		$rates['K']['L']['Single'] = 793.92;
		$rates['K']['D']['Single'] = 847.28;
		$rates['H']['']['Single'] = 537.83;
		$rates['H']['L']['Single'] = 562.60;
		$rates['H']['D']['Single'] = 615.95;
		$rates['M']['']['Single'] = 202.76;
		$rates['M']['L']['Single'] = 227.53;
		$rates['M']['D']['Single'] = 280.88;
		$rates['K']['']['Single+1'] = 1526.15;
		$rates['K']['L']['Single+1'] = 1551.96;
		$rates['K']['D']['Single+1'] = 1605.32;
		$rates['H']['']['Single+1'] = 1130.25;
		$rates['H']['L']['Single+1'] = 1156.07;
		$rates['H']['D']['Single+1'] = 1209.43;
		$rates['M']['']['Single+1'] = 350.15;
		$rates['M']['L']['Single+1'] = 375.97;
		$rates['M']['D']['Single+1'] = 429.32;
		$rates['K']['']['Family'] = 2151.54;
		$rates['K']['L']['Family'] = 2178.65;
		$rates['K']['D']['Family'] = 2232.00;
		$rates['H']['']['Family'] = 1550.31;
		$rates['H']['L']['Family'] = 1577.42;
		$rates['H']['D']['Family'] = 1630.77;
		$rates['M']['']['Family'] = 470.14;
		$rates['M']['L']['Family'] = 497.25;
		$rates['M']['D']['Family'] = 550.60;

		$rate = $rates[$medical][$dental][$tier];
		if (!$rate) { 
			$msg = "Rate not found for medical = $medical, dental = $dental, tier = $tier";
			sirius_minilog($msg, 'error'); return array('success' => FALSE, 'msg' => $msg);
		}

		sirius_minilog("Found rate for medical = $medical, dental = $dental, tier = $tier, rate = \$" . number_format($rate, 2));
		return array('success' => TRUE, 'rate' => $rate);
	}
}

