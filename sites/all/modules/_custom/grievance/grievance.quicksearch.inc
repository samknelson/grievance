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
	drupal_set_title(t('Grievance Quick-Search'));
	
	$form = drupal_get_form('grievance_quicksearch_page_form', $string);
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
	if (preg_match('/\((\d+)\)/', $value, $matches) && $matches[1]) { 
		drupal_goto('node/' . $matches[1]);
	}
	
	drupal_goto("grievance/admin/quicksearch/" . urlencode($value));
}

function grievance_quicksearch_autocomplete($string) {
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
	
	if ($results) {
		$output = "<ul>";
		$count = 0;
		foreach ($results as $result) {
			if (preg_match('/\((\d+)\)/', $result, $matches) && $matches[1]) { 
				$count++;
				$output .= "<li>";
				$output .= l($result, 'node/' . $matches[1]);
				$output .= '</li>';
			}
		}
		
		$output = "<h2>" . t('Found @count results matching: @grievance', array('@count' => $count, '@grievance' => $grievance)) . '</h2>' . $output;
		
		return $output;
	} else {
		drupal_set_message(t("No grievances found."));
		
		return '<h2>' . t('No grievances found.') . '</h2>';
	}
}

function grievance_quicksearch_results($string, $limit = 20) {
	if (!preg_match('/\d/', $string)) {
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node');
		$query->entityCondition('bundle', array('grievance'), 'in');
		$query->propertyCondition('status', 1);
		$query->propertyCondition('title', '%' . $string . '%', 'like');
		$query->propertyOrderBy('created', 'desc');
		$query->range(0, $limit);
		$result = $query->execute();
		if (isset($result['node'])) {
			$grievance_nids = array_keys($result['node']);
			$grievance_nodes = entity_load('node', $grievance_nids);
			foreach ($grievance_nodes as $grievance_node) {
				$idx = $grievance_node->title . ' (' . $grievance_node->nid . ')';
				$matches[$idx] = check_plain($idx);
			}
		}	
	}

	if (preg_match('/\d/', $string)) {
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node');
		$query->entityCondition('bundle', array('grievance'), 'in');
		$query->propertyCondition('status', 1);
		$query->propertyOrderBy('created', 'desc');
		$query->fieldCondition('field_grievance_id', 'value', '%' . $string . '%', 'like');
		$query->range(0, $limit);
		$result = $query->execute();
		if (isset($result['node'])) {
			$grievance_nids = array_keys($result['node']);
			$grievance_nodes = entity_load('node', $grievance_nids);
			foreach ($grievance_nodes as $grievance_node) {
				$idx = $grievance_node->title . ' (' . $grievance_node->nid . ')';
				$matches[$idx] = check_plain($idx);
			}
		}	
	}
	
	return $matches;
}


