<?php

function sirius_ledger_node_tab_account_purge_form($form, &$form_state, $account_node) {
	$form_state['account_nid'] = $account_node->nid;

	$form['participant_nid'] = array(
	 	'#type' => 'textfield',
	 	'#size' => 4,
	 	'#title' => 'Participant NID',
	 	'#description' => t('Enter the NID of the ledger participant. Remember that for workers, the participant is the associated contact node. To purge an entire account for ALL entries, enter "-1".'),
	);

	 $form['confirm'] = array(
	 	'#type' => 'checkbox',
	 	'#title' => t('I understand that his will permanently and completely delete entries from the ledger. There will be no way to retrieve or restore any data lost.'),
	 	'#required' => TRUE,
	 );

	 $form['submit'] = array(
	 	'#type' => 'submit',
	 	'#value' => t('Submit'),
	 );

	 return $form;
}

function sirius_ledger_node_tab_account_purge_form_submit($form, &$form_state) {
	$account_node = node_load($form_state['account_nid']);

	$form_state['rebuild'] = TRUE;
	$account_nid = $account_node->nid;
	if (!$account_nid) {
		drupal_set_message("Please select an account.");
		return;
	}

	$participant_nid = $form_state['values']['participant_nid'];
	if (!$participant_nid) {
		drupal_set_message("Please select a participant.");
		return;
	}

	$params = array();
	$params['ledger_account'] = $account_nid;
	if ($participant_nid != -1) { $params['ledger_participant'] = $participant_nid; }
	$count = sirius_ledger_ar_purge($params);

	drupal_set_message("Account purged. $count records deleted.");
}
