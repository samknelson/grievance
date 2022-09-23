<?php
//
// @TODO: Throughout the functions below we're recalculating the "aliased field" and "aliased table" and "aliased column"
// again and again, usually in slightly different ways and with slightly different meanings.  Standardize the language
// and move into a function!
//


function mightysearch_maketree(&$clauses) {
	$tree = array();
	$tree['connector'] = 'and';
	$tree['clauses'] = array();
	$tree['children'] = array();
	
	while ($clauses) {
		$clause = array_shift($clauses);
		if (!$clause) { 
			continue;
		} else if ($clause == 'and') {
			$tree['connector'] = 'and';
		} else if ($clause == 'or') {
			$tree['connector'] = 'or';
		} else if ($clause == 'PUSH') {
			$tree['children'][] = mightysearch_maketree($clauses);
		} else if ($clause == 'POP') {
			return $tree;
		} else {
			$tree['clauses'][] = $clause;
		}
	}
	
	return $tree;
}

function mightysearch_makecondition_anyactor($op, $value) {
	$condition = db_or();
	
	for ($i=1; $i<=7; ++$i) {
		$aliased_field = 'any_actor_' . $i . '_node.title';
		if ($op == 'like') {
			$condition->condition($aliased_field, '%' . $value . '%', $op);
		} else if ($op == 'starts_with') {
			$condition->condition($aliased_field, $value . '%', 'like');
		} else if ($op == 'ends_with') {
			$condition->condition($aliased_field, '%' . $value, 'like');
		} else if ($op == 'empty') {
			$a = db_or();
			$a->isNull($aliased_field);
			$a->condition($aliased_field, '', '=');
			$condition->condition($a);
		} else if ($op == 'not_empty') {
			$a = db_and();
			$a->isNotNull($aliased_field);
			$a->condition($aliased_field, '', '!=');
			$condition->condition($a);
		}	else {
			$condition->condition($aliased_field, $value, $op);
		}
	}
	
	return $condition;
}

function mightysearch_makecondition($tree, &$query, &$already_joined, $schema) {
	if ($tree['connector'] == 'or') {
		$condition = db_or(); 
	} else {
		$condition = db_and();
	}
	
	foreach ($tree['children'] as $subtree) {
		$condition->condition(mightysearch_makecondition($subtree, $query, $already_joined, $schema));
	}
	
	foreach ($tree['clauses'] as $clause) {
		$open = $clause['open'];
		$close = $clause['close'];
		$connector = $clause['connector'];
		$field = $clause['field'];
		$value = $clause['value'];
		$op = $clause['op'];
	
		list ($table, $field, $col) = explode('.', $field);
		if (!$col) { $col = 'value'; }
		
		mightysearch_join($query, $already_joined, $schema, $table, $field);
		
		if ($table == 'any_actor') {
			$sub_condition = mightysearch_makecondition_anyactor($op, $value);
			$condition->condition($sub_condition);
		} else {
			if ($table == 'any_actor') {
				$aliased_field = 'any_actor_1_node.title';
			} else if ($schema[$table]['fields'][$field]['type'] == 'tid') {
				$aliased_field = "${table}_${field}_term.name";
			} else if ($field == 'title') {
				$aliased_field = "${table}_node.title";
			} else if ($field == 'name') {
				$aliased_field = "${table}_term.name";
			} else if ($field == 'nid') {
				$aliased_field = "${table}_node.nid";
			} else {
				$aliased_field = "${table}_${field}.field_${field}_${col}";
			}
			
			if ($op == 'like') {
				$condition->condition($aliased_field, '%' . $value . '%', $op);
			} else if ($op == 'starts_with') {
				$condition->condition($aliased_field, $value . '%', 'like');
			} else if ($op == 'ends_with') {
				$condition->condition($aliased_field, '%' . $value, 'like');
			} else if ($op == 'empty') {
				$a = db_or();
				$a->isNull($aliased_field);
				$a->condition($aliased_field, '', '=');
				$condition->condition($a);
			} else if ($op == 'not_empty') {
				$a = db_and();
				$a->isNotNull($aliased_field);
				$a->condition($aliased_field, '', '!=');
				$condition->condition($a);
			}	else {
				$condition->condition($aliased_field, $value, $op);
			}
		}
	}

	return $condition;
}

function mightysearch_search_form_submit($form, &$form_state) {
	drupal_set_title(t('Advanced Property Search'));
	
	$schema = mightysearch_mschema();
	
	$output = $form_state['values']['output'];

	/////////////////////////////////////////
	// Save
	/////////////////////////////////////////
	
	if ($form_state['values']['save_as']) {
		//
		// Save
		//
		 
		$title = $form_state['values']['save_as'];
		drupal_set_message("Saved as: " . $title);

		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node');
		$query->entityCondition('bundle', 'sirius_json_definition');
		$query->propertyCondition('title', $title);
		$query->fieldCondition('field_sirius_type', 'value', 'mightysearch:saved_search');
		$result = $query->execute();
		if ($result['node']) { 
			$tmp = array_keys($result['node']);
			$nid = array_shift($tmp);
			$node = node_load($nid);
		} else {
			$node = new stdClass();
			$node->type = "sirius_json_definition";
			$node->field_sirius_type['und'][0]['value'] = 'mightysearch:saved_search';
			$node->language = LANGUAGE_NONE;
			node_object_prepare($node);
		}
		
		$node->title = $title;

		$json = sirius_json_get($node);
		$json['search']['values'] = $form_state['values'];
		sirius_json_set($node, $json);
		$node = node_submit($node);
		node_save($node);
	}
	

	/////////////////////////////////////////
	// Start the Query
	/////////////////////////////////////////
	
	$query=db_select('node', 'property_node');
	$query->condition('property_node.type', 'property');
	$query->condition('property_node.status', 1);
	$query->fields('property_node', array('nid'));
	
	$already_joined = array();

	/////////////////////////////////////////
	// CLAUSES
	/////////////////////////////////////////
	
	$clauses = array();
	foreach ($form_state['values']['clauses'] as $name => $clause) {
		if (!preg_match('/clause_(\d+)/', $name)) { continue; }
		$field = $clause['field'];
		if (!$field) { continue; }
		
		$clauses[] = $clause['connector'];
		
		for ($i=0; $i<$clause['open']; ++$i) {
			$clauses[] = 'PUSH';
		}
		
		$clauses[] = $clause;
		
		for ($i=0; $i<$clause['close']; ++$i) {
			$clauses[] = 'POP';
		}
	}
	
	if (!$clauses) {
		$clauses[] = array(
			'field' => 'property.nid',
			'op' => '>', 
			'value' => 1
		);
	}

	// Make the tree out of the clauses
	$tree = mightysearch_maketree($clauses);
	
	// Build a query condition out of the tree
	$condition = mightysearch_makecondition($tree, $query, $already_joined, $schema);
	
	// Pass to SQL
	$query->condition($condition);
	
	/////////////////////////////////////////
	// DISPLAY
	/////////////////////////////////////////

	$fields = $form_state['values']['display'];
	$header = array('nid');
	foreach ($fields as $field) {
		list ($table, $field, $col) = explode('.', $field);
		if (!$table || !$field) { continue; }

		$prompt = $schema[$table]['table']['prompt'];
		$prompt .= ':';
		if ($output == 'html') {
			$prompt .= '<br>';
		} else {
			$prompt .= ' ';
		}
		if ($col) {
			$prompt .= $schema[$table]['fields'][$field]['col'][$col];
		} else {
			$prompt .= $schema[$table]['fields'][$field]['prompt'];
		}

		$header[] = $prompt;
		
		if (!$col) { $col = 'value'; }
		
		mightysearch_join($query, $already_joined, $schema, $table, $field);
		
		if ($schema[$table]['fields'][$field]['type'] == 'tid') {
			$aliased_table = "${table}_${field}_term";
			$aliased_field = "name";
		} else if ($field == 'title') {
			$aliased_table = "${table}_node";
			$aliased_field = "title";
		} else if ($field == 'name') {
			$aliased_table = "${table}_term";
			$aliased_field = "name";
		} else if ($field == 'nid') {
			$aliased_table = "${table}_node";
			$aliased_field = "nid";
		} else {
			$aliased_table = "${table}_${field}";
			$aliased_field = "field_${field}_${col}";
		}
		
		$query->addField($aliased_table, $aliased_field, "${aliased_table}___${aliased_field}");
	}
	
	/////////////////////////////////////////
	// SORT
	/////////////////////////////////////////
	
	foreach ($form_state['values']['sort'] as $name => $sort) {
		if (!preg_match('/sort_(\d+)/', $name)) { continue; } 
		
		$direction = $sort['direction'];
		$field = $sort['field'];
		list ($table, $field, $col) = explode('.', $field);
		if (!$table || !$field || !$direction) { continue; }
		if (!$col) { $col = 'value'; }

		mightysearch_join($query, $already_joined, $schema, $table, $field);

		if ($schema[$table]['fields'][$field]['type'] == 'tid') {
			$aliased_table = "${table}_${field}_term";
			$aliased_field = "name";
		} else if ($field == 'title') {
			$aliased_table = "${table}_node";
			$aliased_field = "title";
		} else if ($field == 'nid') {
			$aliased_table = "${table}_node";
			$aliased_field = "nid";
		} else {
			$aliased_table = "${table}_${field}";
			$aliased_field = "field_${field}_${col}";
		}
		
		$query->orderBy("${aliased_table}.${aliased_field}", $direction);
	}

	/////////////////////////////////////////
	// LIMIT
	/////////////////////////////////////////

	$limit = $form_state['values']['limit'];
	if (!$limit) { $limit = 10000; }
	$offset = $form_state['values']['offset'];
	if (!$limit) { $offset = 0; }

	$query->range($offset, $limit);

	$rows = array();
	
	if ($output != 'sql') {
		$stmt = $query->execute();
		$rows = array();
		while ($hr = $stmt->fetchAssoc()) {
			$rows[] = $hr;
		}
	}
	
	/////////////////////////////////////////
	// CLEAN
	/////////////////////////////////////////
	
	$fields = $form_state['values']['display'];
	if ($fields) {
		foreach ($rows as &$hr) {
			foreach ($fields as $field) {
				list ($table, $field, $col) = explode('.', $field);
				if (!$table || !$field) { continue; }
				if (!$col) { $col = 'value'; }
				
				$type = $schema[$table]['fields'][$field]['type'];
				
				if ($type == 'tid') {
					$aliased_table = "${table}_${field}_term";
					$aliased_field = "name";
				} else if ($field == 'title') {
					$aliased_table = "${table}_node";
					$aliased_field = "title";
				} else if ($field == 'nid') {
					$aliased_table = "${table}_node";
					$aliased_field = "nid";
				} else {
					$aliased_table = "${table}_${field}";
					$aliased_field = "field_${field}_${col}";
				}
				
				$f = "${aliased_table}___${aliased_field}";
				
				if ($type == 'date') {
					if ($hr[$f]) {
						$hr[$f] = date('Y-m-d', strtotime($hr[$f]));
					}
				}
			}
		}
	}
	
	/////////////////////////////////////////
	// FLATTEN
	/////////////////////////////////////////

	// @todo I'm not sure why I'm not doing a GroupBy in SQL. But for some reason I decided long ago to do this in code.
	
	$flatten = array();
	foreach ($form_state['values']['flatten'] as $key => $value) {
		if (!preg_match('/^flatten_\d+$/', $key)) { continue; }
		if (!$value['field']) { continue; }
		$flatten[] = $value['field'];
	}
	
	if ($flatten) {
	
		$goodrows = array();
		foreach ($rows as &$hr) {
			$flatten_key = "";
			
			$skip_this_row = 0;
			foreach ($flatten as $orig_field) {
				
				$field = $orig_field;
				
				list ($table, $field, $col) = explode('.', $field);
				if (!$table || !$field) { continue; }
				if (!$col) { $col = 'value'; }
		
				if ($table == 'property' && $field == 'nid') {
					$aliased_table = 'property';
					$aliased_field = 'nid';
					$alias = 'nid';
				} else if ($schema[$table]['fields'][$field]['type'] == 'tid') {
					$aliased_table = "${table}_${field}_term";
					$aliased_field = "name";
					$alias = "${aliased_table}___${aliased_field}";
				} else if ($field == 'title') {
					$aliased_table = "${table}_node";
					$aliased_field = "title";
					$alias = "${aliased_table}___${aliased_field}";
				} else if ($field == 'nid') {
					$aliased_table = "${table}_node";
					$aliased_field = "nid";
					$alias = "${aliased_table}___${aliased_field}";
				} else {
					$aliased_table = "${table}_${field}";
					$aliased_field = "field_${field}_${col}";
					$alias = "${aliased_table}___${aliased_field}";
				}
				
				if (!$hr[$alias]) {
					$skip_this_row = 1;
				}
				
				$flatten_key .= $hr[$alias] . " -- ";
			}
			if ($skip_this_row) { continue; }
			
			if (!$goodrows[$flatten_key]) { $goodrows[$flatten_key] = array(); }
			
			foreach ($hr as $key => $value) {
				if (!$goodrows[$flatten_key][$key]) {
					$goodrows[$flatten_key][$key] = array();
				}
				if ($value) {
					$goodrows[$flatten_key][$key][] = $value;
				}
			}
		}
		
		$greatrows = array();
		
		foreach ($goodrows as $flatten_key => $hr) {
			$greatrow = array();
			foreach ($hr as $key => $values) {
				$greatrow[$key] = join(",", array_unique($values));
			}
			$greatrows[] = $greatrow;
		}
		$rows = $greatrows;
	}
	
	/////////////////////////////////////////
	// LINK
	/////////////////////////////////////////

	if ($output == 'html') {
		foreach ($rows as &$hr) {
			$nids = $hr['nid'];
			$nids = explode(',', $nids);
			foreach ($nids as &$nid) {
				$nid = l($nid, 'node/' . $nid);
			}
			$hr['nid'] = join(',', $nids);

			$nids = $hr['local_node___nid'];
			$nids = explode(',', $nids);
			foreach ($nids as &$nid) {
				$nid = l($nid, 'node/' . $nid);
			}
			$hr['local_node___nid'] = join(',', $nids);
		}
	}

	/////////////////////////////////////////
	// RESULTS
	/////////////////////////////////////////
	
	if ($output == 'html') {
		$form_state['rebuild'] = 1;
		$count = count($rows);
		$form_state['results'] = '<h2>' . t('Found @count results', array('@count' => $count)) . '</h2>';
		$form_state['results'] .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
	}
	
	if ($output == 'sql') {
		$form_state['rebuild'] = 1;
		require_once(DRUPAL_ROOT . '/sites/all/libraries/sql-formatter/lib/SqlFormatter.php');
		$form_state['results'] = '<h2>Raw Query</h2>';
		$form_state['results'] .= SqlFormatter::format($query->__toString());
	}

	if ($output == 'json') {
		$results = array();
		foreach ($rows as $row) {
			$result_row = array();

			$idx = 0;
			foreach ($row as $val) {
				$result_row[$header[$idx]] = $val;
				++$idx;
			}

			$results[] = $result_row;
		}
		$form_state['results'] = $results;
	}
	
	if ($output == 'csv') {
		$form_state['rebuild'] = 1;

		// Log all downloads
		$count = count($rows);
		if ($count > 100) {
			$json = array();
			$json['search']['values'] = $form_state['values'];
			sirius_log(
			  'hotelsdb', 
			  'search:export', 
			  "Search results exported, $count results.", 
			  "Search results exported, $count results.", 
			  $count, 
			  array(), 
			  array(),
			  $json
			);
		}
		
		drupal_add_http_header('Content-Type', 'text/csv');
		drupal_add_http_header('Content-Disposition', 'attachment;filename=property_search.csv');
		$fp = fopen('php://output', 'w');
		
		fputcsv($fp, $header);
		foreach ($rows as $row) {
			fputcsv($fp, array_values($row));
		}
		fclose($fp);
		drupal_exit();
	}
	
}
