<?php

function grievance_node_tab_letter($grievance_node, $is_loopback = FALSE) {
  drupal_set_title($grievance_node->title . ': ' . t('Letter'));
  return drupal_get_form('grievance_letter_form', $grievance_node);
}

// @todo pass in $account, instead of assuming current user
function grievance_letter_permission_check($letter_node) {
	global $user;

	// Admin can send any letter
	if (user_access('grievance admin')) { return TRUE; }

	// No permissions set, return false
	if (!isset($letter_node->field_grievance_roles['und'])) { return FALSE; }

	// Does the user have nay of the specified roles?
	foreach ($letter_node->field_grievance_roles['und'] as $tmp) {
		if (user_has_role($tmp['value'], $account)) {
			return TRUE;
		}
	}

	// Apparently not.
	return FALSE;
}

/**
 * Implements hook_tokens
 */

function grievance_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  // Replacement values for tokens that require additional contextual data.
  if ($type == 'grievance-letter') {
    $record = $data['grievance-letter'];
    foreach ($tokens as $name => $original) {
    	// Add "Delivery" as a siml=ple replacement token
    	if ($name == 'delivery') {
    		$delivery_tid = $record['delivery_tid'];
    		$delivery_term = taxonomy_term_load($delivery_tid);
    		$replacement = $delivery_term->description;
    		if (!$replacement) {
    			$replacement = $delivery_term->name;
    		}
				$replacements[$original] = $replacement;
			}

			// Add "render:display_mode" to render the grievance with the specified display mode.
			if (preg_match('/^render:/', $name)) {
				list ($ignore, $display_mode) = explode(':', $name);
				$renderable = node_view($data['node'], $display_mode);
				$replacements[$original] = drupal_render($renderable);
			}

			// Add the clauserfs as a rendered list
			if ($name == 'clauseref') {
				// @TODO
				$html = '';
				if ($data['node']->field_grievance_clauseref['und'][0]) {
					foreach ($data['node']->field_grievance_clauseref['und'] as $item) {
						if ($html) { 
							$html .= '<br>';
						}
						$clause_node = node_load($item['target_id']);
						$html .= $clause_node->field_grievance_label['und'][0]['value'] . ': ' . $clause_node->title;
					}
				}
				$replacements[$original] = $html;
			}

			// Add "Arbitration Clause"
			if ($name == 'arbitration_clause') {
				$contract_tplt_nid = $data['node']->field_grievance_contract_tplt['und'][0]['target_id'];
				if ($contract_tplt_nid) { 
					$contract_tplt_node = node_load($contract_tplt_nid);
				}
				if ($contract_tplt_node) {
					$replacements[$original] = $contract_tplt_node->field_grievance_pullclause['und'][0]['value'];
				}
			}

			// Add log date for status changes
			if (preg_match('/^log\:/', $name)) {
				list ($ignore, $status_name, $log_field) = explode(':', $name);
				$status_terms = taxonomy_get_term_by_name($status_name, 'grievance_status');
				if ($status_terms) {
					$status_tids = array_keys($status_terms);
					$status_tid = $status_tids[0];

				  $query = new EntityFieldQuery();
				  $query->entityCondition('entity_type', 'node');
				  $query->entityCondition('bundle', 'grievance_log');
				  $query->fieldCondition('field_grievance', 'target_id', $data['node']->nid);
				  $query->fieldCondition('field_grievance_status', 'tid', $status_tid);
				  $query->fieldOrderBy('field_grievance_status_date', 'value');
				  $query->range(0,1);
				  $stmt = $query->execute();
				  if ($stmt['node']) {
				  	$log_nids = array_keys($stmt['node']);
				  	$log_nid = $log_nids[0];
				  	$log_node = node_load($log_nid);
				  	$replacements[$original] = date('m/d/Y', strtotime($log_node->field_grievance_status_date['und'][0]['value']));
				  }
				}
			}
    }
  }

  return $replacements;
}


