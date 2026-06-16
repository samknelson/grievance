<?php

function sirius_smf_dashboard_member_overview_data($prefs, $options = array()) {
	// Need a worker
	$worker_node = sirius_worker_get();
	if (!$worker_node) { return array(array('details' => 'There is no worker associated with the current user.')); }

	// Some lookups
	sirius_require('sirius_smf', 'sirius_smf.eligibility_buildup.inc');
	$worker_nid = $worker_node->nid;
	$employers = sirius_employer_list();
	$medical_type_tid = sirius_taxonomy_lookup_by_anything('sirius_trust_benefit_type', 'MEDICAL');
	$dental_type_tid = sirius_taxonomy_lookup_by_anything('sirius_trust_benefit_type', 'DENTAL');
	$add_type_tid = sirius_taxonomy_lookup_by_anything('sirius_trust_benefit_type', 'LIFE');

	// Account info
	$ee_account_nid = sirius_find_nid_by_id('EE', 'sirius_ledger_account');
	if (!$ee_account_nid) {
		return array(array('details' => 'Make sure that there is an account with the ID "EE".'));
	}
	$ea = Sirius_Ledger_EA::getInstance();
	$ea_node = $ea->fetch($worker_node->nid, $ee_account_nid, FALSE);
	if ($ea_node) {
		$invoice = Sirius_Ledger_Invoices::getInstance();
		$currency = sirius_ledger_currency_from_account_nid($ee_account_nid);
	}

	if ($invoice) {
		$participant_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];
		$interval = $invoice->interval_from_ts($col1_ts);
		$balances = $invoice->invoice_load_balances($participant_nid, $ee_account_nid, $interval);
	} else {
		$balances = array();
	}

	// Get the election
	$election_node = sirius_trust_worker_election($worker_node);

	// Get the threshold
	$threshold = 100;
	if ($election_node) {
		$policy_nid = $election_node->field_sirius_trust_policy['und'][0]['target_id'];
		$policy_node = node_load($policy_nid);
		$threshold = Sirius_Smf_Eligibility_Plugin_Buildup::getInstance()->threshold_from_worker_and_policy($worker_node, $policy_node);
	}

	$dates = array();

	// I fucking hate dates and timezones.
	// We want date[0] to be four months ago.
	$y = date('Y');
	$m = date('n');
	for ($i=0; $i<3; ++$i) {
		--$m;
		if ($m < 0) { $m += 12; --$y; }
	}

	$dates[0] = array(
		'y' => $y,
		'm' => $m,
		'ts' => strtotime("$y-$m-15"),
	);

	$dates[0]['ts'] = strtotime($dates[0]['y'] . '-' . $dates[0]['m'] . '-15');
	$dates[0]['render'] = date('M Y', $dates[0]['ts']);

	for ($i=1; $i<12; ++$i) {
		$dates[$i] = $dates[0];
		$dates[$i]['m'] += $i;
		if ($dates[$i]['m'] < 1) {
			$dates[$i]['m'] += 12;
			$dates[$i]['y']--;
		}
		$dates[$i]['ts'] = strtotime($dates[$i]['y'] . '-' . $dates[$i]['m'] . '-15');
		$dates[$i]['render'] = date('M Y', $dates[$i]['ts']);
	}

	// 
	// Get the work hours
	//

	$hours = array();
	for ($i = 0; $i<4; ++$i) {
		$sql = "select sum(hours) as c from sirius_hours_cache ";
		$sql .= "where worker_nid = :worker_nid ";
		$sql .= "and year = :y ";
		$sql .= "and month = :m ";
		$stmt = sirius_sql_query(
			$sql, array(
				':worker_nid' => $worker_node->nid,
				':y' => $dates[$i]['y'],
				':m' => $dates[$i]['m'],
			)
		);
		$hr = $stmt->fetchAssoc();
		$hours[$i] = $hr['c'];
		if (!$hours[$i]) { $hours[$i] = 0; }
		$hours[$i] = round($hours[$i], 2);
	}

	// Covered?
	$search_params = array();
	$search_params['worker_nid'] = $worker_node->nid;
	$search_params['benefit_types'] = array($medical_type_tid, $dental_type_tid);
	$search_params['ts'] = time();
	$search_params['nids_only'] = TRUE;
	$wb_nids = sirius_trust_worker_benefits_search($search_params);

	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');
	$check = '<span class="sirius_text_green" style="font-size: 125%; font-weight: bold;"><i class="fas fa-check"></i></span>';
	$times = '<span class="sirius_text_red" style="font-size: 125%; font-weight: bold;"><i class="fas fa-times"></i></span>';


	// ////////////////////
	// Render
	// ////////////////////

	//
	// Row 0
	//
	
	$html = "<table>";
	$html .= "<tr>";
	$html .= "<td colspan=\"5\">";
	$html .= "<div style=\"text-align: center; font-weight: bold;\">Am I Covered Now?</div>";
	$html .= "</td>";
	$html .= "</tr>";

	//
	// Row 1
	//

	$html .= "<tr>";
	$html .= "<td>";
	$html .= "<strong>Work Hours</strong>";
	$html .= "<br />";
	$html .= $dates[0]['render'];
	$html .= "</td>";
	$html .= "<td>";
	$html .= "<strong>Reported: " . number_format($hours[0], 2) . " hrs</strong>";
	$html .= '<br />';
	$html .= "<strong>Required: " . number_format($threshold, 2) . " hrs</strong>";
	$html .= "</td>";
	$html .= "<td>";
	$html .= "<strong>Balance</strong>";
	$html .= "<br>";
	$html .= $currency->render_amt($balances['balance_out']);
	$html .= "</td>";
	$html .= "<td>";
	$html .= "<strong>Current Coverage</strong><br />";
	$html .= $dates[3]['render'];
	$html .= "</td>";
	$html .= "<td>";
	if ($wb_nids) {
		$html .= $check;
	} else {
		$html .= $times;
	}
	$html .= "</td>";
	$html .= "</tr>";

	//
	// Row 2
	//
	
	$html .= "<tr>";
	$html .= "<td colspan=\"5\">";
	$html .= "<div style=\"text-align: center; font-weight: bold;\">Have I Met My Hours for Future Coverage?</div>";
	$html .= "</td>";
	$html .= "</tr>";

	//
	// Row 3
	//
	
	$html .= "<tr>";
	$html .= "<th>Work Month</th>";
	$html .= "<th>Reported Hours</th>";
	$html .= "<th>&nbsp;</th>";
	$html .= "<th>Coverage Month</th>";
	$html .= "<th>Hours Threshold Met</th>";
	$html .= "</tr>";

	//
	// Row 4
	//
	
	$html .= "<tr>";
	$html .= "<td>";
	$html .= $dates[1]['render'];
	$html .= "</td>";
	$html .= "<td>";
	$html .= number_format($hours[1], 2) . " hrs";
	$html .= "</td>";
	$html .= "<td>";
	$html .= '<i class="fas fa-arrow-right"></i>';
	$html .= "</td>";
	$html .= "<td>";
	$html .= $dates[4]['render'];
	$html .= "</td>";
	$html .= "<td>";
	if ($hours[1] >= $threshold) {
		$html .= $check;
	} else {
		$html .= $times;
	}
	$html .= "</td>";
	$html .= "</tr>";

	//
	// Row 5
	//
	
	$html .= "<tr>";
	$html .= "<td>";
	$html .= $dates[2]['render'];
	$html .= "</td>";
	$html .= "<td>";
	$html .= number_format($hours[2], 2) . " hrs";
	$html .= "</td>";
	$html .= "<td>";
	$html .= '<i class="fas fa-arrow-right"></i>';
	$html .= "</td>";
	$html .= "<td>";
	$html .= $dates[5]['render'];
	$html .= "</td>";
	$html .= "<td>";
	if ($hours[2] >= $threshold) {
		$html .= $check;
	} else if (date('j') < 20) {
		$html .= 'pending';
	} else {
		$html .= $times;
	}
	$html .= "</td>";
	$html .= "</tr>";

	$html .= "</table>";

  return array(
  	array(
    	'success' => TRUE, 
    	'details' => $html,
    	'tools' => array(),
    )
  );



  $id = 'EMPLOYER_CONTRIBUTIONS';
  $result = sirius_feed_lookup_by_field('sirius_ledger_account', 'field_data_field_sirius_id', 'field_sirius_id_value', $id);
  if (!$result['success']) {
    return array(
    	array(
	    	'success' => FALSE, 
	    	'details' => "<strong>Please make sure that there is exactly one account with the ID \"$id\"</strong>",
	    	'count' => 0,
	    	'tools' => array(),
	    )
	  );
  }
  $account_nid = $result['nid'];

  $ea = Sirius_Ledger_EA::getInstance();
  $ea_node = $ea->fetch($employer_nid, $account_nid);
  if (!$ea_node) {
    return array(
    	array(
	    	'success' => FALSE, 
	    	'details' => "<strong>There is no \"EA\" record connecting the employer and the account.</strong>",
	    	'count' => 0,
	    	'tools' => array(),
	    )
	  );
  }
  $ea_nid = $ea_node->nid;

	$payment_nids = sirius_ledger_payments_search(array('payer_nid' => $employer_nid, 'limit' => '10000', 'nids_only' => TRUE));
	$count = count($payment_nids);

	$details = "There are <strong>$count</strong> payments assigned to the \"PAYMENTS_PENDING\" employer.";

	$tools = l(
		'Payments',
		"node/$ea_nid/sirius_ea_payments",
		array('attributes' => array('class' => array('tool-button')))
	);

	$row = array(
		'details' => $details,
		'count' => $count,
		'tools' => $tools,
	);

	return array($row);
}
