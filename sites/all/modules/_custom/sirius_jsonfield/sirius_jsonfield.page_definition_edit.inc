<?php

function sirius_json_definition_edit_form($form, &$form_state, $nid, $link_json = NULL, $which_form = 'default') {
	drupal_add_css(drupal_get_path('module', 'sirius_jsonfield') . '/css/form_editor.css', 'file');

	global $user;
	$user_full = user_load($user->uid);
	$user_json = sirius_jsonfield_util_user_json_get($user_full);
	$str = $user_json['sirius']['jsonfield']['form-builder']['preferences']['richtext'];
	$use_richtext = ($str == 'Yes');
	if ($use_richtext) {
		$richtext_type = 'text_format';
		$richtext_options = array(
			'#format' => 'sirius_form_editor',
			'#allowed_formats' => array('full_html'),
		);
	  $form['#after_build'][] = '_sirius_json_definition_edit_remove_format';
	} else {
		$richtext_type = 'textarea';
	}

	$form_state['nid'] = $nid;
	$form_state['which_form'] = $which_form;
	$node = node_load($nid);

	$json = sirius_jsonfield_util_node_json_get($node);
	$json_form = sirius_jsonfield_util_form_get($json, $which_form);
	$tree = _sirius_json_definition_edit_render_recursive($json_form);

	$field_name = $_REQUEST['field_name'];
	$json_path = $_REQUEST['json_path'];
	$parts = explode(':', $json_path);
	$ref = &$json_form;
	foreach ($parts as $part) {
		if ($part) { $ref = &$ref[$part]; }
	}
	if ($field_name) { $leaf = $ref[$field_name]; }

	$field_list = array();
	_sirius_json_definition_edit_field_list_recursive($field_list, $json_form);

	$link_field_list = array();
	_sirius_json_definition_edit_field_list_recursive($link_field_list, $link_json);

	$fieldset_list = array();
	_sirius_json_definition_edit_fieldset_list_recursive($fieldset_list, $json_form);

	// Can't move fieldset to child
	if ($leaf['#type'] === 'fieldset') {
		$full_path = $json_path . ':' . $field_name;
		$fieldset_paths = array_keys($fieldset_list);
		foreach ($fieldset_paths as $fieldset_path) {
			if (preg_match("/^$full_path/", $fieldset_path)) { unset($fieldset_list[$fieldset_path]); }
		}
	}


	$form['prefix'] = array('#markup' => '<div class="sirius-form"><table><tr>');

	$form['prefix_col'] = array('#markup' => '<td valign="top" style="vertical-align: top;"><div class="field_definition_wrap">');

	$actions = array();
	if ($leaf) {
		$actions['edit'] = t('Edit');
		$actions['delete'] = t('Delete');
		$actions['move'] = t('Move');
		if ($fieldset_list) {
			$actions['regroup'] = t('Change Group');
		}
	} else {
		$actions['edit'] = t('Create');
	}

	$form['action'] = array(
		'#type' => 'select',
		'#title' => 'Action',
		'#options' => $actions,
		'#required' => TRUE,
		'#default_value' => 'edit',
		'#prefix' => '<div class="container-inline">',
		'#suffix' => '</div>',
	);

	if ($fieldset_list) {
		$form['regroup_target'] = array(
			'#type' => 'select',
			'#title' => 'New Group',
			'#options' => $fieldset_list,
			'#states' => array(
				'visible' => array(
					array(':input[name="action"]' => array('value' => 'regroup')),
				),
			),
			'#prefix' => '<div class="container-inline">',
			'#suffix' => '</div>',
		);
	}

	$form['move_direction'] = array(
		'#type' => 'select',
		'#title' => 'Direction',
		'#options' => array('up' => t('Up'), 'down' => t('Down')),
		'#states' => array(
			'visible' => array(
				array(':input[name="action"]' => array('value' => 'move')),
			),
		),
		'#prefix' => '<div class="container-inline">',
		'#suffix' => '</div>',
	);

	$form['move_amount'] = array(
		'#type' => 'textfield',
		'#title' => 'How far?',
		'#default_value' => 1,
		'#states' => array(
			'visible' => array(
				array(':input[name="action"]' => array('value' => 'move')),
			),
		),
		'#prefix' => '<div class="container-inline">',
		'#suffix' => '</div>',
		'#size' => 5,
	);

	$form['name'] = array(
		'#type' => 'textfield',
		'#title' => 'Field Name',
		'#required' => TRUE,
		'#default_value' => $field_name,
		'#states' => array(
			'visible' => array(
				array(':input[name="action"]' => array('value' => 'edit')),
			),
		),
		'#prefix' => '<div class="container-inline">',
		'#suffix' => '</div>',
	);

	$form['title'] = array(
		'#type' => 'textfield',
		'#title' => 'Title',
		'#default_value' => $leaf['#title'],
		'#states' => array(
			'visible' => array(
				array(':input[name="action"]' => array('value' => 'edit')),
			),
		),
		'#prefix' => '<div class="container-inline">',
		'#suffix' => '</div>',
	);

	$field_type = $leaf['#type'];
	if ($field_type == 'markup') { $field_type = 'item'; }
	$form['type'] = array(
		'#type' => 'select',
		'#title' => 'Type',
		'#options' => array(
			'fieldset' => 'Fieldset',
			'item' => 'Markup',
			'textfield' => 'Text field (one line)',
			'textarea' => 'Text area (multi-line)',
			'date_popup' => 'Date',
			// 'integer' => 'Integer',
			// 'number_decimal' => 'Numeric',
			'select' => 'Select',
			'radios' => 'Radio',
			'checkboxes' => 'Checkboxes'
		),
		'#required' => TRUE,
		'#default_value' => $field_type,
		'#states' => array(
			'visible' => array(
				array(':input[name="action"]' => array('value' => 'edit')),
			),
		),
		'#prefix' => '<div class="container-inline">',
		'#suffix' => '</div>',
	);

	$options_str = '';
	if ($leaf['#options']) {
		foreach ($leaf['#options'] as $key => $value){
			if ($key == NULL) { continue; }
			if ($options_str) { $options_str .= "\n"; }
			$options_str .= "$key|$value";
		}
	}
	$form['options'] = array(
		'#type' => 'textarea',
		'#title' => 'Options',
		'#states' => array(
			'visible' => array(
				array(
					':input[name="action"]' => array('value' => 'edit'),
					':input[name="type"]' => array(array('value' => 'select'), array('value' => 'radios'), array('value' => 'checkboxes')),
				),
			),
		),
		'#default_value' => $options_str,
		'#prefix' => '<div class="container-inline">',
		'#suffix' => '</div>',
	);

	$form['required'] = array(
		'#type' => 'select',
		'#title' => 'Required?',
		'#states' => array(
			'visible' => array(
				array(
					':input[name="action"]' => array('value' => 'edit'),
					':input[name="type"]' => array(
						array('value' => 'textfield'), 
						array('value' => 'textarea'), 
						array('value' => 'date_popup'), 
						array('value' => 'select'), 
						array('value' => 'radios'), 
						array('value' => 'checkboxes')
					),
				),
			),
		),
		'#options' => array(
			FALSE => t('No'),
			TRUE => t('Yes'),
		),
		'#default_value' => $leaf['#required'],
		'#prefix' => '<div class="container-inline">',
		'#suffix' => '</div>',
	);

	$form['default_value'] = array(
		'#type' => 'textfield',
		'#title' => 'Default Value',
		'#states' => array(
			'visible' => array(
				array(
					':input[name="action"]' => array('value' => 'edit'),
					':input[name="type"]' => array(
						array('value' => 'textfield'), 
						array('value' => 'textarea'), 
						array('value' => 'select'), 
						array('value' => 'radios'), 
						array('value' => 'checkboxes')
					),
				),
			),
		),
		'#default_value' => $leaf['#default_value'],
		'#prefix' => '<div class="container-inline">',
		'#suffix' => '</div>',
	);

	$form['size'] = array(
		'#type' => 'textfield',
		'#title' => 'Size',
		'#size' => 5,
		'#states' => array(
			'visible' => array(
				array(
					':input[name="action"]' => array('value' => 'edit'),
					':input[name="type"]' => array(array('value' => 'textfield')),
				),
			),
		),
		'#default_value' => $leaf['#size'],
		'#prefix' => '<div class="container-inline">',
		'#suffix' => '</div>',
	);

	$form['markup'] = array(
		'#type' => $richtext_type,
		'#title' => 'Enter the text as it should be displayed on the form.',
		'#default_value' => $leaf['#markup'],
		'#states' => array(
			'visible' => array(
				array(
					':input[name="action"]' => array('value' => 'edit'),
					':input[name="type"]' => array('value' => 'item')
				),
			),
		),
	);
	if ($use_richtext) { $form['markup'] += $richtext_options; }

	$form['description'] = array(
		'#type' => $richtext_type,
		'#title' => 'Help text, description, or special instructions for this field',
		'#default_value' => $leaf['#description'],
		'#states' => array(
			'visible' => array(
				array(
					':input[name="action"]' => array('value' => 'edit'),
					':input[name="type"]' => array(
						array('value' => 'textfield'), 
						array('value' => 'textarea'), 
						array('value' => 'date_popup'), 
						array('value' => 'select'), 
						array('value' => 'radios'), 
						array('value' => 'checkboxes')
					),
				),
			),
		),
	);
	if ($use_richtext) { $form['description'] += $richtext_options; }

	if ($field_list) {
		$form['conditional'] = array(
			'#type' => 'select',
			'#title' => 'Conditional?',
			'#options' => array('No' => t('No'), 'Yes' => t('Yes')),
			'#default_value' => $leaf['#sirius']['conditional'],
			'#states' => array(
				'visible' => array(
					array(':input[name="action"]' => array('value' => 'edit')),
				),
			),
			'#prefix' => '<div class="container-inline">',
			'#suffix' => '</div>',
		);

		$form['conditional_target'] = array(
			'#type' => 'select',
			'#title' => 'Conditional: Field',
			'#options' => $field_list,
			'#default_value' => $leaf['#sirius']['conditional_target'],
			'#states' => array(
				'visible' => array(
					':input[name="action"]' => array('value' => 'edit'),
					':input[name="conditional"]' => array('value' => 'Yes')
				),
			),
			'#prefix' => '<div class="container-inline">',
			'#suffix' => '</div>',
		);

		$form['conditional_value'] = array(
			'#type' => 'textarea',
			'#title' => 'Conditional: Value',
			'#default_value' => $leaf['#sirius']['conditional_value'],
			'#states' => array(
				'visible' => array(
					':input[name="action"]' => array('value' => 'edit'),
					':input[name="conditional"]' => array('value' => 'Yes')
				),
			),
			'#prefix' => '<div class="container-inline">',
			'#suffix' => '</div>',
			'#description' => 'If there are multiple possible values, enter one per line. Remember to enter the value, not the prompt.',
		);
	}

	if ($link_field_list) {
		$form['link'] = array(
			'#type' => 'select',
			'#title' => 'Linked Field?',
			'#options' => array('No' => t('No'), 'Yes' => t('Yes')),
			'#default_value' => $leaf['#sirius']['link'],
			'#states' => array(
				'visible' => array(
					array(':input[name="action"]' => array('value' => 'edit')),
				),
			),
			'#prefix' => '<div class="container-inline">',
			'#suffix' => '</div>',
		);

		$form['link_target'] = array(
			'#type' => 'select',
			'#title' => 'Link: Target',
			'#options' => $link_field_list,
			'#default_value' => $leaf['#sirius']['link_target'],
			'#states' => array(
				'visible' => array(
					':input[name="action"]' => array('value' => 'edit'),
					':input[name="link"]' => array('value' => 'Yes')
				),
			),
			'#prefix' => '<div class="container-inline">',
			'#suffix' => '</div>',
		);
	}

	$form['style'] = array(
		'#type' => 'select',
		'#title' => 'Style',
		'#states' => array(
			'visible' => array(
				array(
					':input[name="action"]' => array('value' => 'edit'),
					':input[name="type"]' => array(
						array('value' => 'textfield'), 
						array('value' => 'textarea'), 
						array('value' => 'date_popup'), 
						array('value' => 'select'), 
						array('value' => 'radios'), 
						array('value' => 'checkboxes')
					),
				),
			),
		),
		'#options' => array(
			'standard' => t('Standard'),
			'inline_label' => t('Inline Label'),
		),
		'#default_value' => $leaf['#sirius']['style'],
		'#prefix' => '<div class="container-inline">',
		'#suffix' => '</div>',
	);

	$form['xprefix'] = array(
		'#type' => $richtext_type,
		'#title' => 'Prefix',
		'#states' => array(
			'visible' => array(
				array(
					':input[name="action"]' => array('value' => 'edit'),
					':input[name="type"]' => array(
						array('value' => 'textfield'), 
						array('value' => 'textarea'), 
						array('value' => 'date_popup'), 
						array('value' => 'select'), 
						array('value' => 'radios'), 
						array('value' => 'checkboxes')
					),
				),
			),
		),
		'#default_value' => $leaf['#sirius']['prefix'],
	);
	if ($use_richtext) { $form['xprefix'] += $richtext_options; }

	$form['xsuffix'] = array(
		'#type' => $richtext_type,
		'#title' => 'Suffix',
		'#states' => array(
			'visible' => array(
				array(
					':input[name="action"]' => array('value' => 'edit'),
					':input[name="type"]' => array(
						array('value' => 'textfield'), 
						array('value' => 'textarea'), 
						array('value' => 'date_popup'), 
						array('value' => 'select'), 
						array('value' => 'radios'), 
						array('value' => 'checkboxes')
					),
				),
			),
		),
		'#default_value' => $leaf['#sirius']['suffix'],
	);
	if ($use_richtext) { $form['xsuffix'] += $richtext_options; }

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Save',
	);

	$form['suffix_col'] = array('#markup' => '</div></td>');

	$form['tree'] = array(
		'#markup' => '<td width="35%" valign="top" style="vertical-align: top; border-left: 1px solid #333333; padding-left: 10px;">' . 
		'<div class="field_list_wrap">' . 
		'<h2>Field List</h2>' . 
		$tree . 
		'</div>' .
		'</td>',
	);


	$form['suffix'] = array('#markup' => '</tr></table></div>');

	return $form;
}

function sirius_json_definition_edit_form_submit($form, &$form_state) {
	$nid = $form_state['nid'];
	$which_form = $form_state['which_form'];
	$node = node_load($nid);
	$json = sirius_jsonfield_util_node_json_get($node);
	$json_form = sirius_jsonfield_util_form_get($json, $which_form);

	$json_path = $_REQUEST['json_path'];
	$old_field_name =  sirius_json_definition_canonical_field_name($_REQUEST['field_name']);
	$field_name = sirius_json_definition_canonical_field_name($form_state['values']['name']);

	// Find the ref for our current fieldset
	$parts = explode(':', $json_path);
	$ref = &$json_form;
	foreach ($parts as $part) {
		if ($part) {
			$ref = &$ref[$part];
		}
	}

	// Deal with changing field name
	if ($old_field_name != $field_name) { 
		$ref[$field_name] = $ref[$old_field_name];
		unset($ref[$old_field_name]);
	}

	sirius_jsonfield_util_form_sort_by_weight($ref);

	//
	// What do we want to do today?
	//

	$action = $form_state['values']['action'];

	if ($action == 'delete') {

		// Delete this field
		unset($ref[$field_name]);

	} else if ($action == 'move') {

		// Move this field
		$form_state['rebuild'] = TRUE;
		$direction = $form_state['values']['move_direction'];
		$amount = $form_state['values']['move_amount'];
		$weight = 0;
		$weights = array();
		foreach ($ref as $key => $value) {
			if (preg_match('/^#/', $key)) { $weights[$key] = -1; } else { $weights[$key] = ++$weight; }
		}
		$new_weight = $weights[$field_name];
		if ($direction == 'up') { $new_weight -= ($amount + 0.5); } else { $new_weight += ($amount + 0.5); }
		if ($new_weight < 0) { $new_weight = 0;}
		$weights[$field_name] = $new_weight;

		foreach ($ref as $key => $value) {
			if (is_array($ref[$key])) {
				$ref[$key]['#weight'] = $weight = 10 * $weights[$key];
			}
		}

	} else if ($action == 'regroup') {

		// Chagne group
		$new_path = $form_state['values']['regroup_target'];

		// Find the ref for our current fieldset
		$parts = explode(':', $new_path);
		$new_ref = &$json_form;
		foreach ($parts as $part) {
			if ($part) {
				$new_ref = &$new_ref[$part];
			}
		}

		$new_ref[$field_name] = $ref[$field_name];
		unset($ref[$field_name]);

	} else {

		// Action = Edit. Create our new "leaf"
		$leaf = array();

		if ($form_state['values']['style']) { $leaf['#sirius']['style'] = $form_state['values']['style']; }
		if ($form_state['values']['xprefix']) { $leaf['#sirius']['prefix'] = _sirius_json_definition_edit_handle_richtext($form_state['values']['xprefix']); }
		if ($form_state['values']['xsuffix']) { $leaf['#sirius']['suffix'] = _sirius_json_definition_edit_handle_richtext($form_state['values']['xsuffix']); }

		// Set basic attributes
		foreach (array('title', 'type', 'description', 'size', 'required') as $key) {
			$leaf['#' . $key] = _sirius_json_definition_edit_handle_richtext($form_state['values'][$key]);
		}
		if ($form_state['values']['type'] == 'item') { 
			$leaf['#markup'] = _sirius_json_definition_edit_handle_richtext($form_state['values']['markup']);
		}

		if ($form_state['values']['default_value']) {
			$leaf['#default_value'] = $form_state['values']['default_value'];
		}
		if ($leaf['#sirius']['style'] == 'inline_label') {
			if ($form_state['values']['type'] == 'checkboxes') {
				$leaf['#prefix'] = '<div class="checkboxes-inline">';
				$leaf['#suffix'] = '</div>';
			} else {
				$leaf['#prefix'] = '<div class="container-inline">';
				$leaf['#suffix'] = '</div>';
			}
		} else {
			$leaf['#prefix'] = '';
			$leaf['#suffix'] = '';
		}

		$post_render = array();
		if ($leaf['#sirius']['prefix']) { $post_render[] = 'sirius_jsonfield_render_prefix'; }
		if ($leaf['#sirius']['suffix']) { $post_render[] = 'sirius_jsonfield_render_suffix'; }
		$leaf['#post_render'] = $post_render; 

		// Set conditional targets.
		if ($form_state['values']['conditional'] == 'Yes') {
			$id = $form_state['values']['conditional_target'];
			$value = $form_state['values']['conditional_value'];
			$leaf['#sirius']['conditional'] = 'Yes';
			$leaf['#sirius']['conditional_target'] = $id;
			$leaf['#sirius']['conditional_value'] = $value;

			$conditional_options = array_filter(explode("\n", $value));
			$conditional_value = array();
			foreach($conditional_options as $conditional_option) {
				$conditional_value[] = array('value' => trim($conditional_option));
			}

			$leaf['#states'] = array(
				'visible' => array(
					array(":input[id='$id']" => $conditional_value),
					'or',
					array("#$id :input" => $conditional_value),
				),
			);

		} else {
			$leaf['#sirius']['conditional'] = 'No';
			unset($leaf['#sirius']['conditional_target']);
			$leaf['#states'] = NULL;
		}

		// Set link targets.
		if ($form_state['values']['link'] == 'Yes') {
			$id = $form_state['values']['link_target'];
			$leaf['#sirius']['link'] = 'Yes';
			$leaf['#sirius']['link_target'] = $id;
		} else {
			$leaf['#sirius']['link'] = 'No';
			unset($leaf['#sirius']['link_target']);
		}

		if ($form_state['values']['options']) {
			$options = array();
			if ($leaf['#type'] == 'select') {
				$options[NULL] = '-- Select --';
			}
			$tmp = preg_split('/[\n\r]+/', $form_state['values']['options']);
			foreach ($tmp as $str) {
				list ($key, $value) = explode('|', $str);
				if (!$key) { continue; }
				if (!$value) { $value = $key; }
				$options[$key] = $value;
			}
			$leaf['#options'] = $options;
		}

		if (!in_array($leaf['#type'], array('select', 'radios', 'checkboxes'))) { unset($leaf['#options']); }

		// Generating the HTML ID is surprisingly ugly. Needs to include the full path, and IDs don't like colons or periods.
		$leaf['#id'] = preg_replace('/:/', '__', 'sirius_json_id' . $json_path . ':' . $field_name);

		// For new leafs, put at the bottom
		if (!$leaf['#weight'] && !$_REQUEST['field_name']) {
			$max_weight = 0;
			foreach ($ref as $other_field_name => $other_leaf) {
				if (isset($other_leaf['#weight']) && $other_leaf['#weight'] > $max_weight) { $max_weight = $other_leaf['#weight']; }
			}
			if ($max_weight) {
				$leaf['#weight'] = $max_weight + 10;
			}
		}

		if (!$ref[$field_name]) { $ref[$field_name] = array(); }
		unset($ref[$field_name]['#markup']);
		unset($ref[$field_name]['#options']);
		foreach ($leaf as $key => $value) { $ref[$field_name][$key] = $value; }
	}

	sirius_jsonfield_util_form_set($json, $which_form, $json_form);
	sirius_jsonfield_util_node_json_set($node, $json);
	node_save($node);
}

function sirius_json_definition_canonical_field_name($name) {
	$name = strtolower($name);
	$name = preg_replace('/[^a-zA-Z0-9_]/', '_', $name);
	$name = preg_replace('/_+/', '_', $name);
	$name = preg_replace('/_+$/', '', $name);
	$name = preg_replace('/^_+/', '', $name);

	return $name;
}

function _sirius_json_definition_edit_render_recursive($obj, $path = '') {
	if (!$obj) {
    return l('[New field]', current_path(), array('query' => array('json_path' => $path), 'attributes' => array('class' => array('new_field')))) . '<br />';
	}

  if (!is_array($obj)) { 
  	return l($obj, current_path(), array('query' => array('json_path' => $path)));
  }

	sirius_jsonfield_util_form_sort_by_weight($obj);

	$type = $obj['#type'];
	if (!$type || $type == 'fieldset') {
	  $html .= '<ol>';
	  foreach ($obj as $key => $value) {
	  	if (!is_array($value)) { continue; }
	  	if (preg_match('/^#/', $key)) { continue; }
	    $html .= '<li>';
	    $html .= l($key, current_path(), array('query' => array('json_path' => $path, 'field_name' => $key)));
	    if ($path == $_REQUEST['json_path'] && $key == $_REQUEST['field_name']) { $html .= '&nbsp;&larr;'; }
	    $html .= _sirius_json_definition_edit_render_recursive($value, $path . ':' . $key);
	    $html .= '</li>';
	  }

    $html .= '<li>';
    $html .= l('[New field]', current_path(), array('query' => array('json_path' => $path), 'attributes' => array('class' => array('new_field'))));
	    if ($path == $_REQUEST['json_path'] && !$_REQUEST['field_name']) { $html .= '&nbsp;&larr;'; }
    $html .= '</li>';

	  $html .= '</ol>';
	} else {
		// $html = l($obj['title'], current_path(), array('query' => array('json_path' => $path)));
	}

  return $html;
}

function _sirius_json_definition_edit_field_list_recursive(&$result, $obj, $path = '') {
	if (!$obj) { return; }
  if (!is_array($obj)) { return; }
	sirius_jsonfield_util_form_sort_by_weight($obj);
	$type = $obj['#type'];
	$id = $obj['#id'];
	if ($id) { $result[$id] = $path; }
	if ($type && ($type != 'fieldset')) { return; }

  foreach ($obj as $key => $value) {
  	if (!is_array($value)) { continue; }
  	if (preg_match('/^#/', $key)) { continue; }
    _sirius_json_definition_edit_field_list_recursive($result, $value, $path . ':' . $key);
  }
}

function _sirius_json_definition_edit_fieldset_list_recursive(&$result, $obj, $path = '') {
	if (!$obj) { return; }
  if (!is_array($obj)) { return; }
	sirius_jsonfield_util_form_sort_by_weight($obj);
	$type = $obj['#type'];
	if ($type && ($type != 'fieldset')) { return; }
	if ($path) { $result[$path] = $path; }

  foreach ($obj as $key => $value) {
  	if (!is_array($value)) { continue; }
  	if (preg_match('/^#/', $key)) { continue; }
    _sirius_json_definition_edit_fieldset_list_recursive($result, $value, $path . ':' . $key);
  }
}

/**
 * @todo: Switch to sirius_richtext_handle_input(); 
 */

function _sirius_json_definition_edit_handle_richtext($value) {
	if (is_array($value)) { 
		return check_markup($value['value'], $value['format']);
	} else { 
		return $value;
	}
}

function _sirius_json_definition_edit_remove_format(&$form) {
	$fields = array('xprefix', 'xsuffix', 'markup', 'description');
	foreach ($fields as $field) {
		if ($form[$field]['format']) {
			$form[$field]['format']['#access'] = FALSE;
		}
	}
  return $form;
}	

/**
 * Callback to insert the prefix. 
 * 
 * WTF, you ask? Why not just use #prefix?
 * 
 * Unfortunately, '#prefix' and '#suffix' are rendered without respect for #states. In other words, if you have conditional field,
 * #prefix and #suffix are rendered no matter what. There does not appear to be a Drupal-friendly way around this, and hence
 * the atrocity that you see before you.
 */

function sirius_jsonfield_render_prefix($markup, $element) {
	if ($element['#sirius']['prefix']) { 
		$markup = preg_replace('/\>/', '>' . '<div class="prefix_wrap"><div class="prefix">' . $element['#sirius']['prefix'] . '</div></div>', $markup, 1);
	}
	return $markup;
}

function sirius_jsonfield_render_suffix($markup, $element) {
	if ($element['#sirius']['suffix']) {
		$markup = preg_replace('/\<\/div\>$/', '<div class="suffix_wrap"><div class="suffix">' . $element['#sirius']['suffix'] . '</div></div></div>', $markup);
	}
	return $markup;
}

