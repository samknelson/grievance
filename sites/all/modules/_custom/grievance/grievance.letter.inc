<?php

function grievance_node_tab_letter_access($grievance_node, $is_loopback = FALSE) {
	// Disable access if the new interface is enabled.
	$enabled_types = sirius_domain_variable_get('sirius_letter_content_types');
	if ($enabled_types['grievance']) { return FALSE; }
	return TRUE;
}

function grievance_node_tab_letter($grievance_node, $is_loopback = FALSE) {
	 $enabled_types = sirius_domain_variable_get('sirius_letter_content_types');
	if ($enabled_types['grievance']) { drupal_goto('node/' . $grievance_node->nid . '/grievance_letter'); }

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
    foreach ($tokens as $name => $original) {

    	//
    	// Add "Delivery" as a siml=ple replacement token
    	//

    	if ($name == 'delivery') {
    		$delivery_tid = $data['other_tokens']['delivery_tid'];
    		$delivery_term = taxonomy_term_load($delivery_tid);
    		$replacement = $delivery_term->description;
    		if (!$replacement) {
    			$replacement = $delivery_term->name;
    		}
				$replacements[$original] = $replacement;
			}

			//
			// Add "render:display_mode" to render the grievance with the specified display mode.
			//

			if (preg_match('/^render:/', $name)) {
				list ($ignore, $display_mode) = explode(':', $name);
				$renderable = node_view($data['node'], $display_mode);
				$replacements[$original] = drupal_render($renderable);
			}

			//
			// Add the clauserfs as a rendered list
			//

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
			
			//
			// Add the clauserefs as a one-line rendered list.
			//

			if ($name == 'clauseref-oneline') {
				// @TODO
				$html = '';
				if ($data['node']->field_grievance_clauseref['und'][0]) {
					foreach ($data['node']->field_grievance_clauseref['und'] as $item) {
						if ($html) { 
							$html .= ', ';
						}
						$clause_node = node_load($item['target_id']);
						$html .= $clause_node->field_grievance_label['und'][0]['value'] . ': ' . $clause_node->title;
					}
				}
				$replacements[$original] = $html;
			}

			//
			// Add the information request as a one-line rendered list.
			//

			if ($name == 'inforeq') {
				// @TODO
				$html = '';
				if ($data['node']->field_grievance_document_types['und'][0]) {
					foreach ($data['node']->field_grievance_document_types['und'] as $item) {
						$term = taxonomy_term_load($item['tid']);
						if ($term->field_grievance_emp_name['und'][0]['value']) {
							$html .= '<li>' . $term->field_grievance_emp_name['und'][0]['value'] . '</li>';
						} else {
							$html .= '<li>' . $term->name . '</li>';
						}
					}
				}
				if ($data['node']->field_grievance_inforeq['und'][0]['value']) {
					$rows = explode("\n", $data['node']->field_grievance_inforeq['und'][0]['value']);
					foreach ($rows as $row) {
						$row = trim($row);
						if (!preg_match('/^\s*$/', $row)) {
							$html .= '<li>' . $row . '</li>';
						}
					}
				}
				if ($html) { $html = '<ul>' . $html . '</ul>'; }
				$replacements[$original] = $html;
			}

			//
			// Add "Arbitration Clause"
			// @todo: This is defunct and should be replaced with article tags from the contract
			//        Still relied on by a couple of letters in use, so we can't get rid of it yet.
			//

			if ($name == 'arbitration_clause') {
				$contract_tplt_nid = $data['node']->field_grievance_contract_tplt['und'][0]['target_id'];
				if ($contract_tplt_nid) { 
					$contract_tplt_node = node_load($contract_tplt_nid);
				}
				if ($contract_tplt_node) {
					$replacements[$original] = $contract_tplt_node->field_grievance_pullclause['und'][0]['value'];
				}
			}

			//
			// Add specially tagged articles from contract templates
			//

			if (preg_match('/^article-tag:/', $name)) {
				list ($ignore, $tag_name, $format) = explode(':', $name);
				if (!$format) { $format = 'full'; }

				$replacement = '';
				// Find the contract template
				$contract_tplt_nid = $data['node']->field_grievance_contract_tplt['und'][0]['target_id'];
				if (!$contract_tplt_nid) {
					$replacement = '(no contract found)';
				}

				// Find the relevant article tag.  Stupidly "Articles" mave the machine name "section", and "Sections" have the machine name "clause".
				if (!$replacement) {
					$section_terms = taxonomy_get_term_by_name($tag_name, 'grievance_contract_section_tags');
					$section_tids = array_keys($section_terms);
					if (!$section_tids) {
						$replacement = '(No article tag found: ' . $tag_name . ')';
					}
				}
	
				// Look up the relevant article
				if (!$replacement) {
				  $query = new EntityFieldQuery();
				  $query->entityCondition('entity_type', 'node');
				  $query->entityCondition('bundle', 'grievance_contract_section');
				  $query->fieldCondition('field_grievance_contract_tplt', 'target_id', $contract_tplt_nid);
				  $query->fieldCondition('field_grievance_cont_sec_tags', 'tid', $section_tids, 'in');
				  $query->fieldOrderBy('field_grievance_weight', 'value');
				  $query->range(0,1);
				  $stmt = $query->execute();
				  if (!$stmt['node']) {
				  	// $replacement = '(No section was tagged with: ' . $tag_name . ')';
				  	$replacement = '';
				  } else {
				  	$section_nodes = node_load_multiple(array_keys($stmt['node']));
				  }
				}

				// Generate our text
				if (!$replacement && $section_nodes) {
					foreach ($section_nodes as $section_node) {
						if ($replacement) { $replacement .= ', '; }

						$replacement .= $section_node->field_grievance_section_number['und'][0]['value'];
						if ($format == 'full') {
							$replacement .= ': ';
							$replacement .= $section_node->title;
						}
					}
				}

		  	$replacements[$original] = $replacement;
			}

			//
			// Add specially tagged clauses from contract templates
			//

			if (preg_match('/^section-tag:/', $name)) {
				list ($ignore, $tag_name, $format) = explode(':', $name);
				if (!$format) { $format = full; }

				$replacement = '';
				// Find the contract template
				$contract_tplt_nid = $data['node']->field_grievance_contract_tplt['und'][0]['target_id'];
				if (!$contract_tplt_nid) {
					$replacement = '(no contract found)';
				}

				// Find the relevant article tag.  Stupidly "Articles" mave the machine name "section", and "Sections" have the machine name "clause".
				if (!$replacement) {
					$clause_terms = taxonomy_get_term_by_name($tag_name, 'grievance_contract_clause_tags');
					$clause_tids = array_keys($clause_terms);
					if (!$clause_tids) {
						$replacement = '(No section tag found: ' . $tag_name . ')';
					}
				}
	
				// Look up the relevant article
				if (!$replacement) {
				  $query = new EntityFieldQuery();
				  $query->entityCondition('entity_type', 'node');
				  $query->entityCondition('bundle', 'grievance_contract_clause');
				  $query->fieldCondition('field_grievance_contract_tplt', 'target_id', $contract_tplt_nid);
				  $query->fieldCondition('field_grievanct_cont_clse_tags', 'tid', $clause_tids, 'in');
				  $query->fieldOrderBy('field_grievance_weight', 'value');
				  $query->range(0,1);
				  $stmt = $query->execute();
				  if (!$stmt['node']) {
				  	// $replacement = '(No section was tagged with: ' . $tag_name . ')';
				  	$replacement = '';
				  } else {
				  	$clause_nodes = node_load_multiple(array_keys($stmt['node']));
				  }
				}

				// Generate our text
				if (!$replacement && $section_nodes) {
					foreach ($clause_nodes as $clause_node) {
						if ($replacement) { $replacement .= ', '; }

						$replacement .= $clause_node->field_grievance_label['und'][0]['value'];
						if ($format == 'full') {
							$replacement .= ': ';
							$replacement .= $clause_node->title;
						}
					}
				}

		  	$replacements[$original] = $replacement;
			}

			//
			// Add log date for status changes
			//

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
