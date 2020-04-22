<?php

function sirius_mightysearch_ajax_valuefield($form, $form_state) {
	$i = $form_state['triggering_element']['#ajax_args']['clause_number'];
  $commands = array();
	$commands[] = ajax_command_replace("#sirius_mightysearch_valuefield_$i", render($form['mightysearch'][$i]['value']));
  return array(
    '#type' => 'ajax',
    '#commands' => $commands
  );
}

function sirius_mightysearch_clause_count($form_state) {
	$count = 0;
	while ($form_state['values']['mightysearch'][$count]['field']) { ++$count; }
	$count += 2;
	if ($count < 5) { $count = 5; }
	return $count;
}

function sirius_mightysearch_form_prepare(&$form, &$form_state, $mightysearch_id, $title, $json_forms, $actions) {
  drupal_add_js(drupal_get_path('module', 'sirius') . '/js/mightysearch.js');
  drupal_add_css(drupal_get_path('module', 'sirius') . '/css/mightysearch.css');
  drupal_add_library('system', 'ui.accordion');
  drupal_add_js('jQuery(document).ready(function () { jQuery("#sirius_accordion").accordion({collapsible: true, autoehight: true, heightStyle: "content"}); });', 'inline');

  $tag_options = sirius_taxonomy_load('sirius_contact_tags', 'tid', 'name');

	$form_state['json_forms'] = $json_forms;
	$form_state['actions'] = $actions;
	$form_state['mightysearch_id'] = $mightysearch_id;

	if (!$form_state['values']) { 
		$form_state['values']['mightysearch'] = $my_saved_mightysearch = $_SESSION['my_saved_mightysearch'][$mightysearch_id];
	}

  $form['#tree'] = TRUE;


  $paths = array();
  $paths[NULL] = t('-- Select --');
  $paths[':sirius_contact:core:tags'] = ':sirius_contact:core:tags';
  foreach ($json_forms as $node_type => $node_type_forms) {
  	foreach ($node_type_forms as $which => $json_form) {
	  	$new_paths = array_keys(sirius_jsonfield_util_list_node_fields($node_type));
	  	foreach ($new_paths as $path) { 
	  		$path = preg_replace("/\:$node_type/", ":$node_type:core", $path);
	  		$paths[$path] = $path;
	  	}

	  	$new_paths = array_values(sirius_jsonfield_util_form_list_paths($json_form, TRUE, $node_type));
	  	foreach ($new_paths as $path) {
	  		$paths[":$node_type:$which$path"] = ":$node_type:$which$path";
	  	}
	  }
  }
  if ($json_forms['sirius_event_participant']) {
  	$paths[':sirius_event_participant:core:status'] = ':sirius_event_participant:core:status';
  	$pstatuses = sirius_event_participant_role_status_tids($json_forms['sirius_event_participant']['prole_tid']);

  	$event_node = node_load($json_forms['sirius_event_participant']['event_nid']);
		$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
		$is_phonebank = sirius_event_phonebank_enabled_type($event_type_term);
		if ($is_phonebank) {
	  	$paths[':sirius_event_participant:phonebank:caller:name'] = ':sirius_event_participant:phonebank:caller:name';
	  	$paths[':sirius_event_participant:phonebank:caller:mail'] = ':sirius_event_participant:phonebank:caller:mail';
	  	$paths[':sirius_event_participant:phonebank:caller:uid'] = ':sirius_event_participant:phonebank:caller:uid';
		}
  }
  foreach ($paths as $key => $label) {
  	$label = preg_replace('/^\:sirius_/', '', $label);
  	$label = preg_replace('/:value/', '', $label);
  	$label = preg_replace('/:field_sirius_/', ':', $label);
  	$paths[$key] = $label;
  }
  $form_state['all_paths'] = $paths;

	$clause_count = sirius_mightysearch_clause_count($form_state);

	$form['#tree'] = TRUE;

  $form['mightysearch']['open'] = array('#markup' => '<div id="mightysearch"><h2>' . $title . '</h2>');
  $form['mightysearch']['accordion_open'] = array('#markup' => '<div id="sirius_accordion">');
  $form['mightysearch']['accordion_open_clauses'] = array('#markup' => '<div class="sirius_accordion_label">' . t('Query') . '</div><div class="sirius_accordion_body">');

	for ($i=0; $i<$clause_count; ++$i) {
		$form['mightysearch'][$i] = array(
			'#type' => 'fieldset',
			'#prefix' => '<div class="sirius_mightysearch_clause">',
			'#suffix' => '</div>',
		);

		$form['mightysearch'][$i]['andor'] = array(
			'#type' => 'select',
			'#options' => array(
				'and' => 'AND',
				'or' => 'OR',
			),
			'#prefix' => '<div class="sirius_mightsearch_clausepart sirius_mightsearch_clausepart_andor">',
			'#suffix' => '</div>',
			'#default_value' => $my_saved_mightysearch[$i]['andor'],
		);
		if ($i == 0) { $form['mightysearch'][$i]['andor']['#options'] = array(NULL => 'WHERE'); }

		$form['mightysearch'][$i]['open'] = array(
			'#type' => 'select',
			'#options' => array(
				NULL => '',
				'(' => '(',
				'((' => '((',
				'(((' => '(((',
				'((((' => '((((',
			),
			'#prefix' => '<div class="sirius_mightsearch_clausepart">',
			'#suffix' => '</div>',
			'#default_value' => $my_saved_mightysearch[$i]['open'],
		);

		$form['mightysearch'][$i]['field'] = array(
			'#type' => 'select',
			'#options' => $paths,
			'#prefix' => '<div class="sirius_mightsearch_clausepart sirius_mightsearch_clausepart_field">',
			'#suffix' => '</div>',
	    '#ajax' => array('callback' => 'sirius_mightysearch_ajax_valuefield'),
	    '#ajax_args' => array('clause_number' => $i),
			'#default_value' => $my_saved_mightysearch[$i]['field'],
		);

		$form['mightysearch'][$i]['criterion'] = array(
			'#type' => 'select',
			'#options' => array(
				'=' => '=',
				'!=' => '!=',
				'like' => 'contains',
				'not like' => 'does not contain',
				'starts' => 'starts with',
				'ends' => 'ends with',
				'empty' => 'empty',
				'not empty' => 'not empty',
				'>=' => '>=',
				'>' => '>',
				'<' => '<',
				'<=' => '<=',
			),
			'#prefix' => '<div class="sirius_mightsearch_clausepart">',
			'#suffix' => '</div>',
			'#default_value' => $my_saved_mightysearch[$i]['criterion'],
	    '#ajax' => array('callback' => 'sirius_mightysearch_ajax_valuefield'),
	    '#ajax_args' => array('clause_number' => $i),
		);

		$full_path = $form_state['values']['mightysearch'][$i]['field'];
		$criterion = $form_state['values']['mightysearch'][$i]['criterion'];
		if ($full_path) {
			if ($criterion == 'empty' || $criterion == 'not empty') {
				$element = array('#type' => 'hidden');
			} else if ($criterion == '=' || $criterion == '!=') {
				list ($node_type, $which_form, $path) = sirius_mightysearch_parse_path($full_path);

				if ($node_type == 'sirius_contact') {
					if ($which_form == 'core') {
						list ($field_name, $column) = explode(':', $path);
						if ($field_name == 'field_sirius_source' && $column == 'value') {
							$element = array('#type' => 'select', '#options' => drupal_map_assoc(sirius_contact_list_sources()));
						} else if ($field_name == 'tags') {
							$element = array('#type' => 'select', '#options' => drupal_map_assoc(array_values($tag_options)));
						} else if ($field_name == 'nid') {
								$element = array('#type' => 'textfield', '#size' => 6);
						} else {
							$element = array('#type' => 'textfield');
						}
					} else {
						$element = sirius_jsonfield_util_get_from_path($json_forms[$node_type][$which_form], $path);
						unset($element['#sirius']);
						unset($element['#title']);
						unset($element['#weight']);
					}
				} else if ($node_type == 'sirius_event_participant') {
					if ($which_form == 'core') {
						list ($field_name, $column) = explode(':', $path);
						if ($field_name == 'status') {
							$element = array('#type' => 'select', '#options' => drupal_map_assoc($pstatuses));
						} else {
							$element = array('#type' => 'textfield');
						}
					} else {
						$element = sirius_jsonfield_util_get_from_path($json_forms[$node_type][$which_form], $path);
						unset($element['#sirius']);
						unset($element['#title']);
						unset($element['#weight']);
					}
				}
			} else {
				$element = array('#type' => 'textfield');
			}
		} else {
			$element = array('#type' => 'textfield');
		}
		$element['#prefix'] = "<div class=\"sirius_mightsearch_clausepart\" id=\"sirius_mightysearch_valuefield_$i\">";
		$element['#suffix'] = '</div>';
		$element['#default_value'] = $my_saved_mightysearch[$i]['value'];

		$form['mightysearch'][$i]['value'] = $element;

		$form['mightysearch'][$i]['close'] = array(
			'#type' => 'select',
			'#options' => array(
				NULL => '',
				')' => ')',
				'))' => '))',
				')))' => ')))',
				'))))' => '))))',
			),
			'#prefix' => '<div class="sirius_mightsearch_clausepart">',
			'#suffix' => '</div>',
			'#default_value' => $my_saved_mightysearch[$i]['close'],
		);

		$form['mightysearch'][$i]['clear'] = array(
			'#markup' => "<div class=\"sirius_mightsearch_clausepart sirius_mightsearch_clausepart_clearlink\"><div><a href=\"\">Clear</a></div></div>"
		);
	}

  $form['mightysearch']['accordion_close_clauses'] = array('#markup' => '</div>');
  $form['mightysearch']['accordion_open_display'] = array('#markup' => '<div class="sirius_accordion_label">' . t('Display') . '</div><div class="sirius_accordion_body">');

  $options = $paths;
  unset($options[NULL]);
  $form['mightysearch']['additional_fields']['display'] = array(
  	'#type' => 'checkboxes',
  	'#options' => $options,
		'#default_value' => $my_saved_mightysearch['additional_fields']['display'],
  );

  $form['mightysearch']['accordion_close_display'] = array('#markup' => '</div>');
  $form['mightysearch']['accordion_close'] = array('#markup' => '</div>');

	$form['mightysearch']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Search'),
	);

  $form['mightysearch']['close'] = array('#markup' => '</div>');

	$form['suffix'] = array('#markup' => '</div>');

	if ($form_state['results']) {
		$count = $form_state['results']['count'];
		$count_total = $form_state['results']['count_total'];
		if ($form_state['results']['rendered']) {
			$results_summary = t('Results');
		} else if ($count_total > $count) {
			$results_summary .= t('@count_total matching contacts found, showing the first @count.', array('@count_total' => number_format($count_total), '@count' => number_format($count)));
		} else if ($count < 1) {
			$results_summary .= t('No contacts match your criteria.');
		} else {
			$results_summary .= t('@count contacts found.', array('@count' => number_format($count)));
		}

		$form['mightysearch_results']['summary'] = array('#markup' => '<hr><h2>' . $results_summary . '</h2>');

		$action_options = array(NULL => t('-- Select --'));
		if ($actions['register']) { $action_options['register'] = $actions['register']['title']; }
		if ($actions['export']) 	{ $action_options['export'] = $actions['export']['title']; }
		if ($actions['tag']) 			{ $action_options['tag_add'] = 'Add Tag'; $action_options['tag_remove'] = 'Remove Tag'; }
		if (user_access('sirius admin')) { $action_options['sql'] = 'Display SQL Query'; }

		$form['mightysearch_results']['action'] = array(
			'#type' => 'select',
			'#title' => t('Action'),
			'#options' => $action_options,
		);

		// @todo: Actions should be callbacks

		if ($action_options['register']) {
			$pstatus_options = $actions['register']['pstatus_options'];
			if (!$pstatus_options) { drupal_set_message("No participant status options for this role. Registration is not possible.", 'error'); }
			else {
				$form['mightysearch_results']['pstatus_tid'] = array(
					'#title' => 'Register With Status',
					'#options' => $pstatus_options,
					'#type' => 'select',
					'#states' => array('visible' => array(array(":input[name=\"mightysearch_results[action]\"]" => array('value' => 'register')))),
				);
			}
		}

		if ($action_options['tag_add'] || $action_options['tag_remove']) {
			$form['mightysearch_results']['tag_tid'] = array(
				'#title' => 'Tag',
				'#options' => $tag_options,
				'#description' => 'Tags can be managed <strong>' . l('here.', 'sirius/taxonomy/sirius_contact_tags', array('attributes' => array('target' => '_blank'))) . '</strong>',
				'#type' => 'select',
					'#states' => array(
						'visible' => array(
							":input[name=\"mightysearch_results[action]\"]" => array(
								array('value' => 'tag_add'), 
								array('value' => 'tag_remove'),
							),
						),
					),
			);
		}

		if ($action_options['register'] || $action_options['tag_add'] || $action_options['tag_remove']) {
			if ($count_total > $count) {
				$form['mightysearch_results']['results_select_all'] = array(
					'#type' => 'checkbox', 
					'#title' => t("Apply to ALL matching contacts, including any not listed below."),
					'#states' => array(
						'visible' => array(
							":input[name=\"mightysearch_results[action]\"]" => array(
								array('value' => 'register'), 
								array('value' => 'tag_add'), 
								array('value' => 'tag_remove'),
							),
						),
					),
				);
			}
		}

		if ($action_options['export']) {
			$form['mightysearch_results']['results_all_fields'] = array(
				'#type' => 'checkbox',
				'#title' => t("Export ALL fields"),
				'#states' => array('visible' => array(array(":input[name=\"mightysearch_results[action]\"]" => array('value' => 'export')))),
			);
		}

		$form['mightysearch_results']['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);

		if ($count > 0) {
			$form['mightysearch_results']['results_select'] = array(
				'#type' => 'tableselect',
				'#header' => $form_state['results']['header'],
				'#options' => $form_state['results']['rows'],
			);
		}

		if ($form_state['results']['rendered']) {
			$form['mightysearch_results']['results_rendered'] = array('#markup' => '<div class="sirius_mightysearch_results_rendered">' . $form_state['results']['rendered'] . '</div>');
		}
	}
}

function sirius_mightysearch_parse_path($path) {
	$parts = explode(':', $path);
	array_shift($parts);
	$node_type = array_shift($parts);
	$which_form = array_shift($parts);
	$path = join(':', $parts); 
	return array($node_type, $which_form, $path);
}

function sirius_mightysearch_add_clause(&$sql_parts, &$sql_args, $values) {
	$full_path = $values['field'];
	if (!$full_path) { return NULL; }
	$value = strtolower(trim($values['value']));
	$criterion = $values['criterion'];

	$sql_parts['headers'][$full_path] = TRUE;

	if ($criterion) {
		$sql_parts['i']++;
		$i = $sql_parts['i'];
		if ($i == 1) { $sql_parts['where'] .= 'and ('; }
		else 				 { $sql_parts['where'] .= $values['andor'] . ' '; }
	}

	list ($node_type, $which_form, $path) = sirius_mightysearch_parse_path($full_path);

	if ($node_type == 'sirius_contact') {
		//
		// Contact fields
		//

		if ($which_form == 'core') {
			list ($field_name, $column) = explode(':', $path);

			if ($field_name == 'title') {
				if ($criterion) { $where_name = "title "; }
			} else if ($field_name == 'nid') {
				if ($criterion) { $where_name = "nid "; }
			} else if ($field_name == 'tags') {
				if (!$sql_parts['joins_nodupe']['tags']) {
					$sql_parts['joins_nodupe']['tags'] = TRUE;
					$sql_parts['joins'] .= "left join field_data_field_sirius_contact_tags on field_data_field_sirius_contact_tags.entity_type = 'node' and field_data_field_sirius_contact_tags.entity_id = node.nid ";
					$sql_parts['joins'] .= "left join taxonomy_term_data tag_term on tag_term.tid = field_data_field_sirius_contact_tags.field_sirius_contact_tags_tid ";
				}
				if (!$sql_parts['fields_nodupe']['tags']) {
					$sql_parts['fields_nodupe']['tags'] = TRUE;
					$sql_parts['select_fields'] .= ", group_concat(distinct tag_term.name order by tag_term.name separator ' / ') as tags ";
				}
				if ($criterion) { $where_name .= "tag_term.name"; }

			} else {
				if ($criterion) { $where_name = "${field_name}_$column "; }
				if (!$sql_parts['joins_nodupe'][$field_name]) {
					$sql_parts['joins_nodupe'][$field_name] = TRUE;
					$sql_parts['joins'] .= "left join field_data_$field_name on field_data_$field_name.entity_type = 'node' and field_data_$field_name.entity_id = node.nid ";
				}
				if (!$sql_parts['fields_nodupe'][$field_name]["${field_name}_$column"]) {
					$sql_parts['fields_nodupe'][$field_name]["${field_name}_$column"] = TRUE;
					$sql_parts['select_fields'] .= ", ${field_name}_$column ";
				}
			}
		} else {
			$path_for_query = '$.' . $which_form . '.results.' . preg_replace('/:/', '.', $path);
			if ($criterion) { $where_name = "lower(json_unquote(json_extract(field_data_field_sirius_json.field_sirius_json_value, '$path_for_query'))) "; }
		}
	} else if ($node_type == 'sirius_event_participant') {
		//
		// Participant fields
		//
		
		if ($which_form == 'core') {
			list ($field_name, $column) = explode(':', $path);

			if ($field_name == 'status') {
				if (!$sql_parts['joins_nodupe'][$field_name]) {
					$sql_parts['joins_nodupe'][$field_name] = TRUE;
					$sql_parts['joins'] .= "left join field_data_field_sirius_event_pstatus on field_data_field_sirius_event_pstatus.entity_type = 'node' and field_data_field_sirius_event_pstatus.entity_id = field_data_field_sirius_contact.entity_id ";
					$sql_parts['joins'] .= "left join taxonomy_term_data pstatus_term on pstatus_term.tid = field_data_field_sirius_event_pstatus.field_sirius_event_pstatus_tid ";
				}
				if (!$sql_parts['fields_nodupe']['pstatus']) {
					$sql_parts['fields_nodupe']['pstatus'] = TRUE;
					$sql_parts['select_fields'] .= ", group_concat(distinct pstatus_term.name order by pstatus_term.name separator ' / ') as pstatus ";
				}
				if ($criterion) { $where_name = 'pstatus_term.name'; }
			}

		} else if ($which_form == 'phonebank') {
			$path_for_query = '$.' . $which_form . '.' . preg_replace('/:/', '.', $path);
			if ($criterion) { $where_name = "lower(json_unquote(json_extract(participant_json.field_sirius_json_value, '$path_for_query'))) "; }
		} else {
			$path_for_query = '$.' . $which_form . '.results.' . preg_replace('/:/', '.', $path);
			if ($criterion) { $where_name = "lower(json_unquote(json_extract(participant_json.field_sirius_json_value, '$path_for_query'))) "; }
		}
	}

	if ($criterion) {
		if ($criterion == 'like' || $criterion == 'not like') {
			$value = '%' . $value . '%';
		} else if ($criterion == 'starts') {
			$criterion = 'like';
			$value = $value . '%';
		} else if ($criterion == 'ends') {
			$criterion = 'like';
			$value = '%' . $value;
		} else if ($criterion == 'empty') {
		}

		$sql_parts['where'] .= $values['open'];

		if ($criterion == 'empty') {
			$sql_parts['where'] .= "($where_name IS NULL OR $where_name = '') ";
		} else if ($criterion == 'not empty') {
			$sql_parts['where'] .= "($where_name IS NOT NULL OR $where_name != '') ";
		} else {
			$sql_parts['where'] .= $where_name . ' ';
			$sql_parts['where'] .= $criterion . ' ';
			$sql_parts['where'] .= ":value_$i ";
			$sql_args[":value_$i"] = $value;
		}
		$sql_parts['where'] .= $values['close'];
	}

	return TRUE;
}

function sirius_mightysearch_form_submit($form, &$form_state) {
	$limit = 1000;

	$json_forms = $form_state['json_forms'];
	$actions = $form_state['actions'];
	$mightysearch_id = $form_state['mightysearch_id'];
	$all_paths = $form_state['all_paths'];

	$_SESSION['my_saved_mightysearch'] = array($mightysearch_id => $form_state['values']['mightysearch']);

	$clause_count = sirius_mightysearch_clause_count($form_state); 
	$domain_nid = sirius_domain_nid();

	$sql_parts = array();
	$sql_args = array();
	$sql_parts['headers'] = array();
	$sql_parts['select_fields'] = "select nid, title, field_data_field_sirius_json.field_sirius_json_value ";
	$sql_parts['select_counts'] = "select count(nid) as c "; 
	$sql_parts['select_nids'] = "select nid "; 

	$sql_parts['limit'] = "group by nid limit $limit ";

	$sql_parts['joins'] = "from node ";
	if ($domain_nid) {
		$sql_parts['joins'] .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	}
	$sql_parts['joins'] .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	if ($json_forms['sirius_event_participant']) {
		$sql_parts['joins'] .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_sirius_contact_target_id = node.nid and field_data_field_sirius_contact.bundle = 'sirius_event_participant' ";
		$sql_parts['joins'] .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = field_data_field_sirius_contact.entity_id ";
		$sql_parts['joins'] .= "left join field_data_field_sirius_json participant_json on participant_json.entity_type = 'node' and participant_json.entity_id = field_data_field_sirius_contact.entity_id ";
		$sql_parts['select_fields'] .= ", participant_json.field_sirius_json_value as participant_json_value ";

		$sql_parts['joins'] .= "left join field_data_field_sirius_event_prole on field_data_field_sirius_event_prole.entity_type = 'node' and field_data_field_sirius_event_prole.entity_id = field_data_field_sirius_contact.entity_id ";
	}

	$sql_parts['where'] = "where type = 'sirius_contact' ";
	if ($domain_nid) {
		$sql_parts['where'] .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	}
	if ($json_forms['sirius_event_participant']) {
		$sql_parts['where'] .= "and field_sirius_event_target_id = :event_nid ";
		$sql_args[':event_nid'] = $json_forms['sirius_event_participant']['event_nid'];

		$sql_parts['where'] .= "and field_sirius_event_prole_tid = :prole_tid ";
		$sql_args[':prole_tid'] = $json_forms['sirius_event_participant']['prole_tid'];
	}

	$found = FALSE;
	for ($i=0; $i<$clause_count; ++$i) {
		$values = $form_state['values']['mightysearch'][$i];
		if (sirius_mightysearch_add_clause($sql_parts, $sql_args, $values)) { $found = TRUE; }
	}
	if ($found) { $sql_parts['where'] .= ') '; }

	foreach (array_values($form_state['values']['mightysearch']['additional_fields']['display']) as $full_path) {
		if ($path) { sirius_mightysearch_add_clause($sql_parts, $sql_args, array('field' => $full_path)); }
	}
	


	$form_state['rebuild'] = TRUE;

	$action = '';
	if ($form_state['triggering_element']['#value'] == 'Submit') {
		$action = $form_state['values']['mightysearch_results']['action'];
	}
	if (!$action) { $action = 'search'; }

	unset($form_state['results']);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Search
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if ($action == 'search') {
		$sql = $sql_parts['select_fields'] . $sql_parts['joins'] . $sql_parts['where'] . $sql_parts['limit']; 
		$stmt = sirius_sql_query($sql, $sql_args);

		$rows = array();
		$count = 0;
		while ($hr = $stmt->fetchAssoc()) {
			$row = array();
			$row[] = $hr['nid'];
			$row[] = $hr['title'];
			foreach (array_keys($sql_parts['headers']) as $full_path) {
				$row[] = sirius_mightysearch_render_result($json_forms, $hr, $full_path);
			}

			$tools = '';
			$tools .= l('View', 'node/' . $hr['nid'], array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
			$tools .= l('Custom Fields', 'node/' . $hr['nid'] . '/sirius_contact_json', array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
			$row[] = $tools;
			$rows[$hr['nid']] = $row;
			++$count;
		}

		$header = array('Internal ID', 'Name');
		foreach (array_keys($sql_parts['headers']) as $full_path) { $header[] = $all_paths[$full_path]; }
		$header[] = t('Tools');

		$count_total = $count;
		if ($count >= $limit) {
			$sql = $sql_parts['select_counts'] . $sql_parts['joins'] . $sql_parts['where']; 
			$stmt = sirius_sql_query($sql, $sql_args);
			$hr = $stmt->fetchAssoc();
			$count_total = $hr['c'];
		}

		$form_state['results'] = array(
			'count' => $count,
			'count_total' => $count_total,
			'results_summary' => $results_summary,
			'rows' => $rows,
			'header' => $header,
		);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Batch
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if ($action == 'register' || $action == 'tag_add' || $action == 'tag_remove') {
		$contact_nids = array();
		if ($form_state['values']['mightysearch_results']['results_select_all']) {
			$sql = $sql_parts['select_nids'] . $sql_parts['joins'] . $sql_parts['where']; 
			$stmt = sirius_sql_query($sql, $sql_args);
			while ($hr = $stmt->fetchAssoc()) { $contact_nids[] = $hr['nid']; }
		} else {
			$contact_nids = array_filter(array_values($form_state['values']['mightysearch_results']['results_select']));
		}
		if (!$contact_nids) {
			drupal_set_message("No contacts selected", 'error');
			return;
		}
		$chunks = array_chunk($contact_nids, 5, TRUE);		

		$operations = array();
		$current_path = current_path();
		foreach ($chunks as $chunk) {
			$actions[$action]['name'] = $action;
			$operations[] = array('sirius_mightysearch_batch', array($chunk, array('values' => $form_state['values'], 'action' => $actions[$action], 'destination' => $current_path, 'query' => $_GET)));
		}
	  $batch = array(
	    'operations' => $operations,
			'finished' => 'sirius_mightysearch_batch_finished',
	  );
	  batch_set($batch);
		return;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Export
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if ($action == 'export') {
		if ($form_state['values']['mightysearch_results']['results_all_fields']) {
			foreach ($all_paths as $full_path => $label) {
				sirius_mightysearch_add_clause($sql_parts, $sql_args, array('field' => $full_path));
			}
		}
		$sql = $sql_parts['select_fields'] . $sql_parts['joins'] . $sql_parts['where']; 
		$stmt = sirius_sql_query($sql, $sql_args);

		$header = array('Internal ID', 'Name');
		foreach (array_keys($sql_parts['headers']) as $full_paths) { $header[] = $all_paths[$full_path]; }

		$fp = fopen('php://output', 'w');
		$filename = 'contacts.csv';
		drupal_add_http_header('Content-Type', 'text/csv');
		drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
		fputcsv($fp, $header);

		while ($hr = $stmt->fetchAssoc()) {
			$row = array();
			$row[] = $hr['nid'];
			$row[] = $hr['title'];
			foreach (array_keys($sql_parts['headers']) as $full_path) {
				$row[] = sirius_mightysearch_render_result($json_forms, $hr, $full_path);
			}
			fputcsv($fp, $row);
		}
		fclose($fp);
		exit();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SQL
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if ($action == 'sql') {
		$sql = $sql_parts['select_fields'] . $sql_parts['joins'] . $sql_parts['where'] . $sql_parts['limit']; 
		$form_state['results']['rendered'] = sirius_sql_pretty($sql, $sql_args);
	}
}

function sirius_mightysearch_batch($chunk, $params, &$context) {
  if (!$context['results']['params']) { $context['results']['params'] = $params; }
  if (!$context['results']['success']) { $context['results']['success'] = 0; }
  if (!$context['results']['error']) { $context['results']['error'] = 0; }

  $action = $params['action'];
  $values = $params['values'];
  $action_name = $action['name'];

	foreach ($chunk as $contact_nid) {

		// @todo: actions should be callbacks

		if ($action_name == 'register') {
			$prole_tid = $action['prole_tid'];
			$pstatus_tid = $values['mightysearch_results']['pstatus_tid'];
			$event_nid = $action['event_nid'];
			$result = sirius_event_participant_register($event_nid, $contact_nid, $prole_tid, $pstatus_tid);
		}

		if ($action_name == 'tag_add' || $action_name == 'tag_remove') {
			$tag_tid = $values['mightysearch_results']['tag_tid'];
			$contact_node = node_load($contact_nid);
			if ($action_name == 'tag_add') 	{ $result = sirius_contact_tag_add($contact_node, $tag_tid); }
 			else 														{ $result = sirius_contact_tag_remove($contact_node, $tag_tid); }
		}

		if ($result['success']) {
			$context['results']['success']++;
			$context['results']['successes'][$result['msg']]++;
		} else {
			$context['results']['error']++;
			$context['results']['errors'][$result['msg']]++;
		}
	}
}

function sirius_mightysearch_batch_finished($success, $results, $operations) {
	$params = $results['params'];
	drupal_set_message(t("Process complete: @success success, @error errors.", array('@success' => $results['success'], '@error' => $results['error'])));
	if ($results['errors']) {
		foreach ($results['errors'] as $msg => $count) {
			drupal_set_message(t("Failed with message [@msg] @count time(s).", array('@msg' => $msg, '@count' => $count)), 'warning');
		}
	}
	if ($results['successes']) {
		foreach ($results['successes'] as $msg => $count) {
			drupal_set_message(t("Succeeded with message [@msg] @count time(s).", array('@msg' => $msg, '@count' => $count)));
		}
	}
	drupal_goto($params['destination'], array('query' => $params['query']));
}

function sirius_mightysearch_render_result($json_forms, $hr, $full_path ) {
	list ($node_type, $which_form, $path) = sirius_mightysearch_parse_path($full_path);
	if ($node_type == 'sirius_event_participant') {
		$json = json_decode($hr['participant_json_value'], TRUE);
	} else {
		$json = json_decode($hr['field_sirius_json_value'], TRUE);
	}

	if ($node_type == 'sirius_contact') {
		if ($which_form == 'core') {
			list ($field_name, $column) = explode(':', $path);
			if ($field_name == 'title') {
				return $hr['title'];
			} else if ($field_name == 'nid') {
				return $hr['nid'];
			} else if ($field_name == 'tags') {
				return $hr['tags'];
			} else {
				return $hr["${field_name}_$column"];
			}
		} else {
			$json_form = $json_forms[$node_type][$which_form];
			$json_results = sirius_jsonfield_util_results_get($json, $which_form);
			return sirius_jsonfield_util_render_values($json_form, $json_results, array($path), array('raw' => TRUE));
		}
	} else if ($node_type == 'sirius_event_participant') {
		if ($which_form == 'core') {
			list ($field_name, $column) = explode(':', $path);
			if ($field_name == 'role') {
				return $hr['prole'];
			} else if ($field_name == 'status') {
				return $hr['pstatus'];
			} 
		} else if ($which_form == 'phonebank') {
			list ($field_name, $column) = explode(':', $path);
			return $json[$which_form][$field_name][$column];
		} else {
			$json_form = $json_forms[$node_type][$which_form];
			$json_results = sirius_jsonfield_util_results_get($json, $which_form);
			return sirius_jsonfield_util_render_values($json_form, $json_results, array($path), array('raw' => TRUE));
		}
	}
}
