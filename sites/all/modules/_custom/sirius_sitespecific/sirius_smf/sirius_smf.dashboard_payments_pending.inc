<?php

function sirius_smf_dashboard_payments_pending_data($prefs, $options = array()) {
  $id = 'PAYMENTS_PENDING';
  $result = sirius_feed_lookup_by_field('grievance_shop', 'field_data_field_sirius_id', 'field_sirius_id_value', $id);
  if (!$result['success']) {
    return array(
    	array(
	    	'success' => FALSE, 
	    	'details' => "<strong>Please make sure that there is exactly one shop with the ID \"$id\"</strong>",
	    	'count' => 0,
	    	'tools' => array(),
	    )
	  );
  }
  $employer_nid = $result['nid'];


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
