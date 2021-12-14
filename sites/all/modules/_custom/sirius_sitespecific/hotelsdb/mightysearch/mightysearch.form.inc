<?php

function mightysearch_search_form($form, &$form_state) {
	// Include the MultiSelect JS / CSS
	drupal_add_css(drupal_get_path('module', 'multiselect') . '/multiselect.css');
	drupal_add_js(drupal_get_path('module', 'multiselect') . '/multiselect.js');
	drupal_add_css(drupal_get_path('module', 'mightysearch') . '/css/mightysearch.css');
	
	// Get the schema
	$schema = mightysearch_mschema();
	
	// Our field options
	$field_options = array('' => '[Select]');
	foreach ($schema as $table_name => $table) {
		foreach ($table['fields'] as $field_name => $field) {
			if (is_array($field['col'])) {
				foreach ($field['col'] as $col_name => $col_prompt) {
				$field_options["$table_name.$field_name.$col_name"] = 
					$table['table']['prompt'] . ": " . $field['prompt'] . ": " . $col_prompt;
				}
			} else {
				$field_options["$table_name.$field_name"] = 
					$table['table']['prompt'] . ": " . $field['prompt'];
			}
		}
	}
	
	// Re-order our options
	$highlights = array('', 'property.title', 'property.address.locality', 'owner.title', 'operator.title', 'any_actor.title');
	$new_field_options = array();
	foreach ($field_options as $key => $value) {
		if (in_array($key, $highlights)) {
			$new_field_options[$key] = $value;
		}
	}
	$new_field_options['ignore'] = '----------';
	foreach ($field_options as $key => $value) {
		if (!in_array($key, $highlights)) {
			$new_field_options[$key] = $value;
		}
	}
	$field_options = $new_field_options;
	
	// And just because display needs no empty values
	$display_options = array();
	foreach ($field_options as $key => $value) {
		if ($key && $key != 'ignore') {
			$display_options[$key] = $value;
		}
	}

	// Can't display on "Any Actor"
	unset($display_options['any_actor.title']);
	
	// Can't sort on "Any Actor"
	$sort_options = $field_options;
	unset($sort_options['any_actor.title']);
	
	// Flatten includes NID
	$flatten_options = array();
	foreach ($sort_options as $key => $value) {
		$flatten_options[$key] = $value;
		if ($key == '') {
			$flatten_options['property.nid'] = 'Property: NID';
		}
	}

  // Because we have many fields with the same values, we have to set #tree to be able to access them.
 	$form['#tree'] = TRUE;

	/////////////////////////////////////////
	// CLAUSE
	/////////////////////////////////////////
	
	$form['clauses_intro'] = array(
    '#type' => 'markup',
		'#markup' => '<h2>' . t('Find Properties Where') . '</h2>',
  );

	// Wrapper for all clauses
	$form['clauses'] = array(
    '#type' => 'fieldset',
		
    // Set up the wrapper so that AJAX will be able to replace the fieldset.
    '#prefix' => '<div id="clauses-add-wrapper">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['num_clauses'])) {
    $form_state['num_clauses'] = 5;
  }
	for ($i=0; $i<$form_state['num_clauses']; ++$i) {
		$clause = "clause_$i";

		if ($i > 0) {
			$form['clauses'][$clause]['connector'] = array(
				'#type' => 'select',
				'#title' => t('Connector'),
				'#options' => array(
					'and' => 'and',
					'or' => 'or'
				),
				'#attributes' => array('class' => array('clause-connector')),
				'#prefix' => '<div class="clause">',
			);
		} else {
			$form['clauses'][$clause]['connector'] = array(
				'#type' => 'markup',
				'#markup' => '<div class="clause"><div class="connector-spacer">&nbsp;</div>',
			);
		}

		$form['clauses'][$clause]['open'] = array(
      '#type' => 'select',
      '#title' => t('Open'),
			'#options' => array(
				'' => '', 
				1 => '(',
				2 => '((',
				3 => '(((',
				4 => '((((',
				5 => '(((((',
			),
			'#attributes' => array('class' => array('clause-open')),
		);

		$form['clauses'][$clause]['field'] = array(
      '#type' => 'select',
      '#title' => t('Field'),
			'#options' => $field_options,
			'#attributes' => array('class' => array('clause-field')),
		);

		$form['clauses'][$clause]['op'] = array(
      '#type' => 'select',
      '#title' => t('Operator'),
			'#options' => array(
				''			=> '[select]',
				'='			=> '=',
				'!='		=> '!=',
				'<'			=>	'<',
				'<='		=>	'<=',
				'>'			=>	'>',
				'>='		=>	'>=',
				'like'			=>	'Contains',
				'starts_with'			=>	'Starts With',
				'ends_with'			=>	'End With',
				'empty'			=>	'Empty',
				'not_empty'			=>	'Not Empty',
			),
			'#attributes' => array('class' => array('clause-op')),
		);

		$form['clauses'][$clause]['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Value'),
			'#attributes' => array('class' => array('clause-value')),
		);

		$form['clauses'][$clause]['close'] = array(
      '#type' => 'select',
      '#title' => t('Close'),
			'#options' => array(
				'' => '', 
				1 => ')',
				2 => '))',
				3 => ')))',
				4 => '))))',
				5 => ')))))',
			),
			'#attributes' => array('class' => array('clause-close')),
			'#suffix' => '</div><div class="clearfix">&nbsp;</div>',
		);
	}
	
	/*
	$form['clauses']['clause_0']['value']['#default_value'] = 'Open';
	$form['clauses']['clause_0']['op']['#default_value'] = '=';
	$form['clauses']['clause_0']['field']['#default_value'] = 'property.development_status';
	*/
	
  $form['clauses']['add_clause'] = array(
    '#type' => 'submit',
    '#value' => t('Add a Condition'),
    '#submit' => array('mightysearch_search_form_clause_add'),
    '#ajax' => array(
      'callback' => 'mightysearch_search_form_clause_callback',
			'wrapper' => 'clauses-add-wrapper',
    ),
  );
	
	if ($form_state['num_clauses'] > 5) {
		$form['clauses']['remove_clause'] = array(
			'#type' => 'submit',
			'#value' => t('Remove a Condition'),
			'#submit' => array('mightysearch_search_form_clause_remove'),
			'#ajax' => array(
				'callback' => 'mightysearch_search_form_clause_callback',
				'wrapper' => 'clauses-add-wrapper',
			),
		);
	}

	$form['submit_1'] = array(
		'#type' => 'submit', 
		'#value' => t('Search Mightily')
	);

	/////////////////////////////////////////
	// DISPLAY
	/////////////////////////////////////////

	$form['display_intro'] = array(
    '#type' => 'markup',
		'#markup' => '<h2>' . t('Show fields') . '</h2>',
  );
	
	$default = $form_state['input']['display'];
	if (!$default) {
		$default = array(
			'property.title',
			'property.rooms',
			'property.address.locality',
			'property.address.administrative_area',
			'local.title',
			'property.development_status',
			'property.union_status',
			'owner.title',
			'operator.title',
		);
	}

	$multiselect_default = array();
	foreach ($default as $key => $value) {
		$multiselect_default[$key] = array('value' => $value);
	}

	$form['display'] = array(
		'#type' => 'multiselect',
		'#options' => $display_options,
    '#multiple' => TRUE,
    '#size' => 15,
    '#default_value' => $default,
	);
	
	/////////////////////////////////////////
	// SORT
	/////////////////////////////////////////
	
	$form['sort_intro'] = array(
    '#type' => 'markup',
		'#markup' => '<h2>' . t('Sort By') . '</h2>',
  );

	// Wrapper for all clauses
	$form['sort'] = array(
    '#type' => 'fieldset',
		
    // Set up the wrapper so that AJAX will be able to replace the fieldset.
    '#prefix' => '<div id="sort-add-wrapper">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['num_sort'])) {
    $form_state['num_sort'] = 2;
  }
	for ($i=0; $i<$form_state['num_sort']; ++$i) {
		$sort = "sort_$i";

		$form['sort'][$sort]['field'] = array(
      '#type' => 'select',
      '#title' => t('Order By'),
			'#options' => $sort_options,
			'#attributes' => array('class' => array('clause-open')),
			'#prefix' => '<div class="clause">',
		);
		
		$form['sort'][$sort]['direction'] = array(
      '#type' => 'select',
      '#title' => t('Direction'),
			'#options' => array('ASC' => 'Ascending', 'DESC' => 'Descending'),
			'#attributes' => array('class' => array('clause-open')),
			'#suffix' => '</div><div class="clearfix">&nbsp;</div>',
		);
	}
	
  $form['sort']['add_sort'] = array(
    '#type' => 'submit',
    '#value' => t('Add a Sort'),
    '#submit' => array('mightysearch_search_form_sort_add'),
    '#ajax' => array(
      'callback' => 'mightysearch_search_form_sort_callback',
			'wrapper' => 'sort-add-wrapper',
    ),
  );
	
	if ($form_state['num_sort'] > 2) {
		$form['sort']['remove_sort'] = array(
			'#type' => 'submit',
			'#value' => t('Remove a Sort'),
			'#submit' => array('mightysearch_search_form_sort_remove'),
			'#ajax' => array(
				'callback' => 'mightysearch_search_form_sort_callback',
				'wrapper' => 'sort-add-wrapper',
			),
		);
	}

	/////////////////////////////////////////
	// LIMIT
	/////////////////////////////////////////

	$form['limit_intro'] = array(
    '#type' => 'markup',
		'#markup' => '<h2>' . t('Limit') . '</h2>',
  );

	$default = $form_state['input']['limit'];
	if (!$default) {
		$default = 50000;
	}
	$form['limit'] = array(
		'#title' => t('Number of Results to Display'),
		'#type' => 'textfield',
		'#default_value' => $default,
	);

	/*
	$form['offset'] = array(
		'#title' => t('Start at Result #'),
		'#type' => 'textfield',
	);
	*/

	/////////////////////////////////////////
	// FLATTEN
	/////////////////////////////////////////
	
	$form['flatten_intro'] = array(
    '#type' => 'markup',
		'#markup' => '<h2>' . t('Group By') . '</h2>',
  );

	// Wrapper for all clauses
	$form['flatten'] = array(
    '#type' => 'fieldset',
		
    // Set up the wrapper so that AJAX will be able to replace the fieldset.
    '#prefix' => '<div id="flatten-add-wrapper">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['num_flatten'])) {
    $form_state['num_flatten'] = 2;
  }
	for ($i=0; $i<$form_state['num_flatten']; ++$i) {
		$flatten = "flatten_$i";
		
		$default = null;
		if ($i == 0) { $default = 'property.nid'; }

		$form['flatten'][$flatten]['field'] = array(
      '#type' => 'select',
      '#title' => t('Group By'),
			'#options' => $flatten_options,
			'#attributes' => array('class' => array('clause-open')),
			'#prefix' => '<div class="clause">',
			'#suffix' => '</div><div class="clearfix">&nbsp;</div>',
			'#default_value' => $default
		);
	}
	
  $form['flatten']['add_flatten'] = array(
    '#type' => 'submit',
    '#value' => t('Add a Grouping Field'),
    '#submit' => array('mightysearch_search_form_flatten_add'),
    '#ajax' => array(
      'callback' => 'mightysearch_search_form_flatten_callback',
			'wrapper' => 'flatten-add-wrapper',
    ),
  );
	
	if ($form_state['num_flatten'] > 2) {
		$form['flatten']['remove_flatten'] = array(
			'#type' => 'submit',
			'#value' => t('Remove a Grouping Field'),
			'#submit' => array('mightysearch_search_form_flatten_remove'),
			'#ajax' => array(
				'callback' => 'mightysearch_search_form_flatten_callback',
				'wrapper' => 'flatten-add-wrapper',
			),
		);
	}
	

	/////////////////////////////////////////
	// SAVE
	/////////////////////////////////////////
	
	$form['save_as'] = array(
		'#title' => t('Save'),
		'#type' => 'textfield',
		'#description' => t('Enter a name to save this search as.  If there is already a search with that name, it will be overwritten.'),
	);

	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
	$query->entityCondition('bundle', 'saved_search');
	$query->propertyOrderBy('title');
	$result = $query->execute();
	$saved_options = array('' => '[select]');
	if ($result['node']) {
		$tmp = array_keys($result['node']);
		foreach ($tmp as $nid) {
			$node = node_load($nid);
			$saved_options[$nid] = $node->title;
		}
	}
	
	$form['load_from'] = array(
		'#title' => t('Load From'),
		'#type' => 'select',
		'#options' => $saved_options,
		'#description' => t('Discard the current search, and load a new search.'),
	);

	/////////////////////////////////////////
	// OUTPUT
	/////////////////////////////////////////

	$form['output'] = array(
		'#title' => t('Output'),
		'#type' => 'select',
		'#options' => array(
			'html' => 'Web Page',
			'csv' => 'CSV',
			'sql' => 'SQL Query',
		)
	);


	/////////////////////////////////////////
	// SUBMIT
	/////////////////////////////////////////
	
	$form['submit'] = array(
		'#type' => 'submit', 
		'#value' => t('Search Mightily')
	);
	
	/////////////////////////////////////////
	// RESULTS
	/////////////////////////////////////////
	
	if ($form_state['results']) {
		$form['results'] = array(
			'#type' => 'markup',
			'#markup' => $form_state['results']
		);
	}
	
	return $form;
}


function mightysearch_search_form_clause_add($form, &$form_state) {
  $form_state['num_clauses']++;
  $form_state['rebuild'] = TRUE;
}

function mightysearch_search_form_clause_remove($form, &$form_state) {
	if ($form_state['num_clauses'] > 5) { 
	  $form_state['num_clauses']--;
	}
  $form_state['rebuild'] = TRUE;
}


function mightysearch_search_form_clause_callback($form, $form_state) {
  return $form['clauses'];
}

function mightysearch_search_form_sort_add($form, &$form_state) {
  $form_state['num_sort']++;
  $form_state['rebuild'] = TRUE;
}

function mightysearch_search_form_sort_remove($form, &$form_state) {
	if ($form_state['num_sort'] > 2) { 
	  $form_state['num_sort']--;
	}
  $form_state['rebuild'] = TRUE;
}


function mightysearch_search_form_sort_callback($form, $form_state) {
  return $form['sort'];
}


function mightysearch_search_form_flatten_add($form, &$form_state) {
  $form_state['num_flatten']++;
  $form_state['rebuild'] = TRUE;
}

function mightysearch_search_form_flatten_remove($form, &$form_state) {
	if ($form_state['num_flatten'] > 2) { 
	  $form_state['num_flatten']--;
	}
  $form_state['rebuild'] = TRUE;
}


function mightysearch_search_form_flatten_callback($form, $form_state) {
  return $form['flatten'];
}
