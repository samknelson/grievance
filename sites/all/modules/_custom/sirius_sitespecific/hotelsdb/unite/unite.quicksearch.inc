<?php

//
// Quick-Search
//

function unite_quicksearch_form($form, &$form_state) {
  $form = array();

  $form['string'] = array(
    '#title' => t('Property / Actor'),
    '#type' => 'textfield',
    '#autocomplete_path' => 'unite/ajax/quicksearch',
		'#size' => 25
   );
 
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'GO',
  );
 
  return $form;
}

function unite_quicksearch_page($string) {
	drupal_set_title(t('Quick-Search'));
	
	$form = drupal_get_form('unite_quicksearch_page_form', $string);
	return $form;
}

function unite_quicksearch_page_form($form, &$form_state, $string) {
  $form = array();

	$string = urldecode($string);

  $form['string'] = array(
    '#title' => t('Property / Actor Quick-Search'),
    '#type' => 'textfield',
		'#default_value' => $string
   );
 
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search Now')
  );
	
	if (empty($form_state['results'])) {
		$form_state['results'] = '<br><strong>';
		
		$form_state['results'] .= l(
			'Advanced Property Search &rarr;', 
			'properties',
			array(
				'html' => TRUE,
				'query' => array(
					'relationship-a' => 'Owner',
					'relationship-b' => 'Operator',
					'title_2' => $string
				)
			)
		);
		$form_state['results'] .= '&nbsp;|&nbsp;';
		
		$form_state['results'] .= l(
			'Advanced Actor Search &rarr;', 
			'actors',
			array(
				'html' => TRUE,
				'query' => array(
					'title' => $string
				)
			)
		);
		
		$form_state['results'] .= '</strong>';
		
		$form_state['results'] .= unite_quicksearch_results_html($string, 1000);
	}
	$form['results'] = array(
		'#markup' => '<div class="results">' . $form_state['results'] . '</div>'
	);
 
  return $form;
}

function unite_quicksearch_page_form_submit($form, &$form_state) {
	$string = $form_state['values']['string'];

	$form_state['results'] = unite_quicksearch_results_html($string, 1000);
  $form_state['rebuild'] = TRUE;
}

function unite_quicksearch_form_submit($form, &$form_state) {
	$value = $form_state['values']['string'];
	if (!$value) { return; }
	
	$matches = array();
	if (preg_match('/\((\d+)\)/', $value, $matches) && $matches[1]) { 
		drupal_goto('node/' . $matches[1]);
	}
	if (preg_match('/(.+)\[brand\]/', $value, $matches) && $matches[1]) { 
		drupal_goto('properties', array('query' => array('field_brand_tid' => $matches[1])));
	}
	
	drupal_goto("unite/admin/quicksearch/" . urlencode($value));
}

function unite_quicksearch_autocomplete($string) {
  $matches = array();
	if (strlen($string) < 3) { 
		drupal_json_output($matches);
		return;
	}
	$matches = unite_quicksearch_results($string);
  drupal_json_output($matches);
}

function unite_quicksearch_results_html($string, $limit = 20) {
	$results = unite_quicksearch_results($string, $limit);
	
	if ($results) {
		$output = "<ul>";
		$count = 0;
		foreach ($results as $result) {
			if (preg_match('/\((\d+)\)/', $result, $matches) && $matches[1]) { 
				$count++;
				$nid = $matches[1];
				$output .= "<li>";
				if ($count < 100) { 
					$node = node_load($nid);
					$output .= $node->type . ': ';
				}
				$output .= l($result, 'node/' . $nid);
				$output .= '</li>';
			} else if (preg_match('/(.+)\[brand\]/', $result, $matches) && $matches[1]) { 
				$count++;
				$tid = $matches[1];
				$output .= "<li>";
				$output .= t('Brand') . ': ';
				$output .= l($result, 'properties', array('query' => array('field_brand_tid' => $matches[1])));
				$output .= '</li>';
			}
		}
		
		$output = "<h2>" . t('Found @count results matching: @string', array('@count' => $count, '@string' => $string)) . '</h2>' . $output;
		
		return $output;
	} else {
		drupal_set_message(t("No matches found."));
		
		return '<h2>' . t('No matches found.') . '</h2>';
	}
}

function unite_quicksearch_results($string, $limit = 20) {
	$matches = array();
	if (!preg_match('/\d/', $string)) {
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node');
		$query->entityCondition('bundle', array('property', 'actor'), 'in');
		$query->propertyCondition('status', 1);
		$query->propertyCondition('title', '%' . $string . '%', 'like');
		$query->range(0, $limit);
		$result = $query->execute();
		if (isset($result['node'])) {
			$nids = array_keys($result['node']);
			$nodes = entity_load('node', $nids);
			foreach ($nodes as $node) {
				$idx = $node->title . ' (' . $node->nid . ')';
				$matches[$idx] = check_plain($idx);
			}
		}	
	}

	if (!preg_match('/\d/', $string)) {
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'taxonomy_term');
		$query->entityCondition('bundle', 'brand');
		$query->propertyCondition('name', '%' . $string . '%', 'like');
		$query->range(0, $limit);
		$result = $query->execute();
		if (isset($result['taxonomy_term'])) {
			$tids = array_keys($result['taxonomy_term']);
			$terms = entity_load('taxonomy_term', $tids);
			foreach ($terms as $term) {
				$idx = $term->name. ' [brand]';
				$matches[$idx] = check_plain($idx);
			}
		}
	}

	if (preg_match('/\d\d\d\d\d/', $string)) {
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node');
		$query->entityCondition('bundle', array('property', 'actor'), 'in');
		$query->propertyCondition('status', 1);
		$query->fieldCondition('field_address', 'postal_code', $string);
		$query->range(0, $limit);
		$result = $query->execute();
		if (isset($result['node'])) {
			$nids = array_keys($result['node']);
			$nodes = entity_load('node', $nids);
			foreach ($nodes as $node) {
				$idx = $node->title . ' (' . $node->nid . ')';
				$matches[$idx] = check_plain($idx);
			}
		}	
	}
	
	return $matches;
}

