<?php

//
// Grievance Quick-Search
//

function grievance_quicksearch_form($form, &$form_state) {
  $form = array();
 
  $form['grievance'] = array(
    '#title' => t('Grievance'),
    '#type' => 'textfield',
    '#autocomplete_path' => 'grievance/ajax/quicksearch',
		'#size' => 25
   );
 
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'GO',
  );
 
  return $form;
}

function grievance_quicksearch_page($string) {
	drupal_set_title(t('Quick Search'));
	
	$form = drupal_get_form('grievance_quicksearch_page_form', urldecode($string));
	return $form;
}

function grievance_quicksearch_page_form($form, &$form_state, $string) {
  $form = array();
 
  $form['grievance'] = array(
    '#title' => t('Grievance Quick-Search'),
    '#type' => 'textfield',
		'#default_value' => $string
   );
 
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search Now')
  );
	
	if (empty($form_state['results'])) {
		$form_state['results'] = grievance_quicksearch_results_html($string, 1000);
	}
	$form['results'] = array(
		'#markup' => '<div class="results">' . $form_state['results'] . '</div>'
	);
 
  return $form;
}

function grievance_quicksearch_page_form_submit($form, &$form_state) {
	$grievance = $form_state['values']['grievance'];

	$form_state['results'] = grievance_quicksearch_results_html($grievance, 1000);
  $form_state['rebuild'] = TRUE;
}

function grievance_quicksearch_form_submit($form, &$form_state) {
	$value = $form_state['values']['grievance'];
	if (!$value) { return; }
	
	$matches = array();
	if (preg_match('/\s+\((\d+)\)/', $value, $matches) && $matches[1]) { 
		drupal_goto('node/' . $matches[1]);
	}
	
	drupal_goto("grievance/admin/quicksearch/" . urlencode($value));
}

function grievance_quicksearch_autocomplete($string, $s2 = NULL, $s3 = NULL, $s4 = NULL) {
	// If the string has a slash, it's not passed by the autocomplete function.
	// See https://www.drupal.org/project/drupal/issues/93854
	// So we use this monstrosity.
	if (isset($s2)) { $string .= "/$s2"; }
	if (isset($s3)) { $string .= "/$s3"; }
	if (isset($s4)) { $string .= "/$s4"; }

  $matches = array();
	if (strlen($string) < 3) { 
		drupal_json_output($matches);
		return;
	}
	$matches = grievance_quicksearch_results($string);
	if (!$matches) { $matches = array(); }
  drupal_json_output($matches);
}

function grievance_quicksearch_results_html($grievance, $limit = 20) {
	$results = grievance_quicksearch_results($grievance, $limit);

	$header = array();
	$header[] = t('Type');
	$header[] = t('Title');
	$header[] = t('ID');
	$header[] = t('Tools');

	if (!$results) { return '<h2>' . t('No records found.') . '</h2>'; }

	$rows = array();
	$count = 0;
	foreach ($results as $idx => $result) {
		if (preg_match('/\s+\((\d+)\)/', $idx, $matches) && $matches[1]) { $nid = $matches[1]; }
		$node = node_load($nid);
		if (!$node) { continue; }

		$id = $node->field_grievance_id['und'][0]['value'];
		if (!$id) { $id = $node->field_sirius_id['und'][0]['value']; }

		$row = array();
		$row[] = sirius_content_type_name($node->type);
		$row[] = $node->title;
		$row[] = $id;
		$row[] = l('View', "node/$nid", array('attributes' => array('class' => array('tool-button'))));
		$rows[] = $row;

		++$count;
	}
	
	$html = "<h2>" . t('Found @count results matching: @grievance', array('@count' => $count, '@grievance' => $grievance)) . '</h2>';
	$html .= sirius_table_render($header, $rows);
	return $html;
}

function grievance_quicksearch_results($string, $limit = 20) {
	$string = trim($string);

	$types = grievance_quicksearch_types();
	if (!$types) { return array();}


	$domain_nid = sirius_domain_nid();

	// Search the title
	if (!preg_match('!\d!', $string)) {
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node');
		$query->entityCondition('bundle', array_keys($types), 'in');
		$query->propertyCondition('status', 1);
		$query->propertyCondition('title', '%' . $string . '%', 'like');
		if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
		// $query->propertyOrderBy('created', 'desc');
		$query->range(0, $limit);
		$result = $query->execute();
		if (isset($result['node'])) {
			foreach (array_keys($result['node']) as $nid) {
				list ($idx, $label) = grievance_quicksearch_render_idx($nid);
				$matches[$idx] = $label;
			}
		}	
	}

	if (preg_match('!\d!', $string)) {
		$id = $string;

		if (preg_match('/\//', $id)) {
			$id = preg_replace('/.*\//', '', $id);
			$id = trim($id);
		}
		$tmp = $types;
		unset($tmp['sirius_worker']);

		// Search the grievance ID
		if ($tmp) {
			$query = new EntityFieldQuery();
			$query->entityCondition('entity_type', 'node');
			$query->entityCondition('bundle', array_keys($tmp), 'in');
			$query->propertyCondition('status', 1);
			// $query->propertyOrderBy('created', 'desc');
			$query->fieldCondition('field_grievance_id', 'value', $id);
			if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
			$query->range(0, $limit);
			$result = $query->execute();
			if (isset($result['node'])) {
				foreach (array_keys($result['node']) as $nid) {
					list ($idx, $label) = grievance_quicksearch_render_idx($nid);
					$matches[$idx] = $label;
				}
			}
		}

		// Search the sirius ID
		$tmp = $types;
		unset($tmp['grievance']);
		if ($tmp) {
			$query = new EntityFieldQuery();
			$query->entityCondition('entity_type', 'node');
			$query->entityCondition('bundle', array_keys($tmp), 'in');
			$query->propertyCondition('status', 1);
			// $query->propertyOrderBy('created', 'desc');
			$query->fieldCondition('field_sirius_id', 'value', $id, 'like');
			if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
			$query->range(0, $limit);
			$result = $query->execute();
			if (isset($result['node'])) {
				foreach (array_keys($result['node']) as $nid) {
					list ($idx, $label) = grievance_quicksearch_render_idx($nid);
					$matches[$idx] = $label;
				}
			}
		}

		// Search the sirius ID 2
		$tmp = $types;
		unset($tmp['grievance']);
		if ($tmp) {
			$query = new EntityFieldQuery();
			$query->entityCondition('entity_type', 'node');
			$query->entityCondition('bundle', array_keys($tmp), 'in');
			$query->propertyCondition('status', 1);
			// $query->propertyOrderBy('created', 'desc');
			$query->fieldCondition('field_sirius_id2', 'value', $id);
			if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
			$query->range(0, $limit);
			$result = $query->execute();
			if (isset($result['node'])) {
				foreach (array_keys($result['node']) as $nid) {
					list ($idx, $label) = grievance_quicksearch_render_idx($nid);
					$matches[$idx] = $label;
				}
			}
		}
	}

	if ($types['sirius_worker']) {
		// Search by worker's SSN
		$ssn = sirius_ssn_format($string);
		if ($ssn) {
			$query = new EntityFieldQuery();
			$query->entityCondition('entity_type', 'node');
			$query->entityCondition('bundle', 'sirius_worker');
			$query->propertyCondition('status', 1);
			// $query->propertyOrderBy('created', 'desc');
			$query->fieldCondition('field_sirius_ssn', 'value', $ssn);
			if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
			$query->range(0, $limit);
			$result = $query->execute();
			if (isset($result['node'])) {
				foreach (array_keys($result['node']) as $nid) {
					list ($idx, $label) = grievance_quicksearch_render_idx($nid);
					$matches[$idx] = $label;
				}
			}
		}

		// Search by the worker's phone
		$phone = sirius_phone_format($string);
		if (sirius_phone_validate($phone)) {
			$query = new EntityFieldQuery();
			$query->entityCondition('entity_type', 'node');
			$query->entityCondition('bundle', 'sirius_worker');
			$query->propertyCondition('status', 1);
			// $query->propertyOrderBy('created', 'desc');
			$query->fieldCondition('field_sirius_phone', 'value', $phone);
			if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
			$query->range(0, $limit);
			$result = $query->execute();
			if (isset($result['node'])) {
				foreach (array_keys($result['node']) as $nid) {
					list ($idx, $label) = grievance_quicksearch_render_idx($nid);
					$matches[$idx] = $label;
				}
			}
		}
	}

	return $matches;
}

function grievance_quicksearch_types() {
	$types = array();
	if (variable_get('grievance_quicksearch_include_grievances', 'Yes') == 'Yes') { $types['grievance'] = 'GRV'; }
	if (variable_get('grievance_quicksearch_include_workers', 'No') == 'Yes') { $types['sirius_worker'] = 'WRK'; }
	return $types;
}

function grievance_quicksearch_render_idx($nid) {
	$sql = "select title, type from node where nid = :nid";
	$stmt = db_query($sql, array(':nid' => $nid));
	$hr = $stmt->fetchAssoc();
	$title = $hr['title'];
	$type = $hr['type'];

	$types = grievance_quicksearch_types();

	$label = '<span data-nid="' . $nid . '">' .
		check_plain($title);
	if (count($types) > 1) {
		$label .= ' (' . $types[$type] . ')';
	}
	$label .= '</span>';
	$idx = check_plain($title) . ' (' . $nid . ')';
	return array($idx, $label);
}

function grievance_quicksearch_admin() {
  $form = array();

  $form['grievance_quicksearch_include_grievances'] = array(
  	'#type' => 'select',
  	'#options' => array('Yes' => 'Yes', 'No' => 'No'),
  	'#title' => t('Include grievances in quicksearch results?'),
  	'#default_value' => variable_get('grievance_quicksearch_include_grievances', ''),
	);

  $form['grievance_quicksearch_include_workers'] = array(
  	'#type' => 'select',
  	'#options' => array('Yes' => 'Yes', 'No' => 'No'),
  	'#title' => t('Include workers in quicksearch results?'),
  	'#default_value' => variable_get('grievance_quicksearch_include_workers', ''),
	);

  return system_settings_form($form);
}

