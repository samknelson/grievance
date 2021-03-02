<?php

function sirius_ledger_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  // Replacement values for tokens that require additional contextual data.
  if ($type == 'sirius-ledger') {
    foreach ($tokens as $name => $original) {
        $parts = explode(':', $name);

        //
    	// e.g. [sirius-ledger:account:DUES:balance]
    	//

    	if ($parts[0] == 'account') { 
    		$account_id = $parts[1];
    		$account_computation = $parts[2];

    		// Get the ledger participant. 
    		// - For employers (grievance_shop), this is the node itself.
    		// - For contacts, it's the node itself.
    		// - For workers, it's whatever's referenced in the contact field

    		$entity_node = $data['node'];
    		if ($entity_node->type == 'sirius_contact' || $entity_node->type == 'grievance_shop') { 
    			$participant_nid = $entity_node->nid;
    		} else {
    			$participant_nid = $entity_node->field_sirius_contact['und'][0]['target_id'];
    		}

    		if (!$participant_nid) { $replacement = "Unknown participant"; }

    		// Get the account
    		if (!$replacement) {
    			$account_nid = sirius_find_nid_by_id($account_id, 'sirius_ledger_account');
    			if (!$account_nid) { $replacement = "Unknown account: $account_id"; }
    		}

    		if (!$replacement) {
	    		if ($account_computation == 'balance') {
	    			$replacement = number_format(sirius_ledger_balance_fetch($participant_nid, $account_nid), 2);
	    		} else {
	    			$replacement = "Unknown computation: $account_computation"; 
	    		}
	    	}
    	}

        // 
        // e.g. [sirius-ledger:auth:items]
        // 

        if ($parts[0] == 'auth' && $parts[1] == 'items') {
            $auth_node = $data['node'];
            $replacement = sirius_ledger_auth_render_items($auth_node);
        }

	  	$replacements[$original] = $replacement;
    }
  }

  return $replacements;
}
