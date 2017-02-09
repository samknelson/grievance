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
				foreach ($data['node']->field_grievance_clauseref['und'] as $item) {
					if ($html) { 
						$html .= '<br>';
					}
					$clause_node = node_load($item['target_id']);
					$html .= $clause_node->field_grievance_label['und'][0]['value'] . ': ' . $clause_node->title;
				}
				$replacements[$original] = $html;
				/*
				if ($node->content['field_grievance_clauseref']['#items']) {
					$html = '';
					foreach ($data['node']->content['field_grievance_clauseref']['#items'] as $item) {
						$clause_node = $item['entity'];
						$title = 
						$node->content['field_grievance_clauseref'][$delta]['#markup'] = $title;
						++$delta;
					}
				}
				*/
			}

    }
  }

  return $replacements;
}


