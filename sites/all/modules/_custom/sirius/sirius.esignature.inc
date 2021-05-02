<?php

function sirius_esignature_init() {
	drupal_add_js('sites/all/modules/jquery_update/replace/ui/ui/minified/jquery-ui.min.js');
	drupal_add_js('sites/all/libraries/jquery_signature/js/jquery.touch-punch.min.js');
	drupal_add_js('sites/all/libraries/jquery_signature/js/jquery.signature.js');
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/sirius.esignature.js');

	drupal_add_css('sites/all/libraries/jquery_signature/css/jquery.signature.css');
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/sirius.esignature.css');

	$settings_for_js = array(
		'popup_width' => 600,
		'popup_height' => 200,
	);

	// $settings_for_js['fields'] = $fields;
	drupal_add_js(array('sirius_esignature' => $settings_for_js), 'setting');
}

/**
 * Stream an esignature image. 
 * @todo: Access control
 */

function sirius_esignature_stream($nid, $which) {
	$node = node_load($nid);
	$json = sirius_jsonfield_util_node_json_get($node);
	$esignature = sirius_jsonfield_util_esignature_get($json, $which);
	$graphic = $esignature['graphic'];
	$graphic = preg_replace('/^data\:image\/png\;base64/', '', $graphic);
	header("Content-type: image/png");
	print base64_decode($graphic);
	exit();
}






function sirius_esignature_render($esignature, $format, $nid, $which, $options = array()) {
	$defs = sirius_esignature_defs();
	foreach ($defs as $delta => $probe) {
		if ($probe['code'] == $which) {
			$def = $probe;
		}
	}

	$class = "sirius_esignature_render ";
	$class .= "sirius_esignature_render_$format ";
	if (!$esignature) { $class .= "sirius_esignature_render_missing "; }
	$html .= "<div class=\"$class\">";

	if ($format == 'full') {
		if ($esignature['statement']) {
			$html .= '<div class="sirius_esignature_statement">' . $esignature['statement'] . '</div>';
		} else if ($def['title']) {
			$html .= '<div class="sirius_esignature_statement">' . $def['title'] . '</div>';
		}
	}

	$html .= "<div class=\"sirius_esignature_col_name\">";

	$html .= "<div class=\"sirius_esignature_canvas\">";
	if ($esignature) {
		if ($format == 'full') {
			$html .= "<img src=\"/sirius/esignature/stream/$nid/$which\" class=\"sirius_esignature_img\" />";
		} else {
			$html .= "<a href=\"#\" class=\"sirius_esignature_popup_trigger\">";
			$html .= "<img src=\"/sirius/esignature/stream/$nid/$which\" class=\"sirius_esignature_img\" />";
			$html .= "</a>";
			$html .= '<div class="sirius_esignature_render_date">';
			$html .= $esignature['date_str'];
			$html .= '</div>';

			$html .= '<div style="display:none" class="sirius_esignature_popup_overlay"></div>';
			$html .= '<div style="display:none" class="sirius_esignature_popup_wrap">';
			$html .= '<div class="sirius_esignature_popup_contents">';
			$html .= sirius_esignature_render($esignature, 'full', $nid, $which, array('onecol' => TRUE));
			$html .= '</div>';
			$html .= '<div class="sirius_esignature_popup_close"></div>';
			$html .= '</div>';
		}
	}
	$html .= '</div>';

	$html .= '<div class="sirius_esignature_metadata">';
	if ($format == 'full') {
		if ($esignature) {
			if ($options['onecol']) {
				$html .= 'Date: ' . $esignature['date_str'] . '<br />';
				$html .= 'User: ' . $esignature['user_name'] . ' (UID ' . $esignature['user_uid'] . ')' . '<br />';
				$html .= 'IP: ' . $esignature['ip'] . '<br />';
			}
			if ($esignature['name']) { $html .= $esignature['name'] . '<br />'; }
			if ($esignature['org']) { $html .= $esignature['org'] . '<br />'; }
		} else {
			$html .= $def['name'] . '<br />' . $def['org'];
		}
	}
	$html .= '</div>'; 

	$html .= '</div>';

	if ($format == 'full' && !$options['onecol']) {
		$html .= "<div class=\"sirius_esignature_col_date\">";
		$html .= "<div class=\"sirius_esignature_datecanvas\">";
		if ($esignature) {
			$html .= $esignature['date_str'] . '<br />';
			$html .= 'User: ' . $esignature['user_name'] . ' (UID ' . $esignature['user_uid'] . ')' . '<br />';
			$html .= 'IP: ' . $esignature['ip'];
		}
		$html .= "</div>";
		$html .= "Date";
		$html .= "</div>";
	}

	$html .= '</div><br clear="left" />';
	return $html;
}
























function sirius_esignature_form_prepare(&$form, $field_name, $title, $statement, $options = array()) {
	global $user;
	$user_full = user_load($user->uid);
	$user_json = sirius_jsonfield_util_user_json_get($user_full);

	$suffix = "<div class=\"sirius_esignature\">";
	$suffix .= "<div class=\"sirius_esignature_statement\">";
	$suffix .= $statement;
	$suffix .= '</div>';
	$suffix .= '<h2>Signature</h2>';
	$suffix .= "<div class=\"sirius_esignature_pane_wrap\">";
	$suffix .= "<div class=\"sirius_esignature_pane\" id=\"sirius_esig\">";
	$suffix .= "</div>";
	$suffix .= "</div>";
	$suffix .= "<button id=\"sirius_esig_clear\">Clear</button>";
	$suffix .= "</div>";

	$form[$field_name]['#type'] = 'textarea';
	$form[$field_name]['#suffix'] = $suffix;
	$form[$field_name]['#id'] = "sirius_esig_data";
	$form[$field_name]['#required'] = TRUE;

	if ($title) {
		$form[$field_name]['#title'] = $title;
	}

	$form[$field_name . '_statement'] = array();
	$form[$field_name . '_statement']['#type'] = 'value';
	$form[$field_name . '_statement']['#value'] = $statement;

	if ($options['show_name']) {
		$form[$field_name . '_name'] = array(
			'#title' => t('Name'),
			'#type' => 'textfield',
		);
		if ($user_json['esignature']['settings']['name']) {
			$form[$field_name . '_name']['#default_value'] = $user_json['esignature']['settings']['name'];
		} else {
			$form[$field_name . '_name']['#default_value'] = $def['name'];
			if ($user && $user->uid) {
				$form[$field_name . '_name']['#description'] = 'You can set your default esignature name on the ' . l('account settings page.', 'user/' . $user->uid . '/esig');
			}
		}
	}

	if ($options['show_org']) {
		$form[$field_name . '_org'] = array(
			'#title' => 'Organization',
			'#type' => 'textfield',
		);
		if ($user_json['esignature']['settings']['org']) {
			$form[$field_name . '_org']['#default_value'] = $user_json['esignature']['settings']['org'];
		}  else {
			$form[$field_name . '_org']['#default_value'] = $def['org'];
		}
	}

	$form[$field_name . '_submit'] = array();
	$form[$field_name . '_submit']['#type'] = 'submit';
	$form[$field_name . '_submit']['#value'] = t('Submit');
	$form[$field_name . '_submit']['#id'] = "sirius_esig_submit";
}

function sirius_esignature_form_get_json($form, &$form_state, $field_name) {
	$json = array();
	$json['statement'] = $form_state['values'][$field_name . '_statement'];
	$json['name'] = $form_state['values'][$field_name . '_name'];
	$json['org'] = $form_state['values'][$field_name . '_org'];
	$json['graphic'] = $form_state['values'][$field_name];
	$json['ip'] = sirius_loopback_remote_ip();
	$ts = time();
	$json['date_ts'] = $ts;
	$json['date_str'] = date('c', $ts);
	global $user;
	if ($user) {
		$json['user_uid'] = $user->uid;
		$json['user_name'] = $user->name;
		$json['user_mail'] = $user->mail;
	}
	return $json;
}

function sirius_esignature_fully_signed($json, $which) {
	$json_form = sirius_jsonfield_util_form_get($json, $which);
	$signature_definitions = $json_form['#sirius']['signature_definitions'];
	if (!$signature_definitions) { return TRUE; }
	foreach ($signature_definitions as $signature_definition) {
		$name = $signature_definition['name'];
		$esignature = sirius_jsonfield_util_esignature_get($json, $name);
		if (!$esignature) { return FALSE; }
	}

	return TRUE;
}

//
// @todo: We have two notions of signature definitions, "the old" and "the new".
//        What a mess
//


function sirius_esignature_defs($entity_node = NULL) {
	$json = json_decode(sirius_domain_variable_get('sirius_esignature_defs'), TRUE);
	if (!$json) { return array(); }

	if ($entity_node) {
		$entity_json = sirius_jsonfield_util_node_json_get($entity_node);
		$entity_codes = $entity_json['esignature']['defs'];
		if (!$entity_codes) { $entity_codes = array(); }
		$entity_codes = array_filter(array_values($entity_codes));
		$entity_defs = array();
		foreach ($json as $code => $def) {
			if (in_array($code, $entity_codes)) { $entity_defs[$code] = $def; }
		}
		$json = $entity_defs;
	}

	return $json;
}

function sirius_esignature_defs_set($defs) {
	foreach ($defs as $delta => $probe) {
		if ($probe['roles']) {
			if (!array_filter(array_values($probe['roles']))) {
				unset($probe['roles']);
			}
		}
		$probe = array_filter(array_values($probe));
		if (!$probe) {
			unset($defs[$delta]);
		}
	}

	// Map by code
	$by_code = array();
	foreach ($defs as $delta => $def) {
		$by_code[$def['code']] = $def;
	}
	sirius_domain_variable_set('sirius_esignature_defs', json_encode($by_code));
}

function sirius_esignature_entity_status($entity_node) {
	$defs = sirius_esignature_defs($entity_node);
	$count = $count_complete = $count_incomplete = 0;
	$json = sirius_jsonfield_util_node_json_get($entity_node);
	foreach ($defs as $def) {
		++$count;
		$sig = sirius_jsonfield_util_esignature_get($json, $def['code']);
		if ($sig) { ++$count_complete; } else { ++$count_incomplete; }
	}
	
	if (!$count) {
		$status = 'unsigned';
		$status_msg = "Unsigned - no signatures required";
	} else if (!$count_complete) {
		$status = 'unsigned';
		$status_msg = "Unsigned - 0 / $count signatures complete";
	} else if ($count_complete < $count) { 
		$status = "partial";
		$status_msg = "Partially signed - $count_complete / $count signatures complete";
	}
	else { 
		$status = "signed";
		$status_msg = "Fully signed - $$count signatures complete";
	}

	return array(
		'count' => $count,
		'count_complete' => $count_complete,
		'count_incomplete' => $count_incomplete,
		'status' => $status,
		'status_msg' => $status_msg,
	);
}

function sirius_user_tab_esignature_access($esig_uid) {
	// Admins can see anybody
	if (user_access('sirius admin')) { return TRUE; }

	// Users can see themselves
	global $user;
	if ($user->uid == $esig_uid) { return TRUE; }

	// Domain admin can see anybody in their domain
	if (user_access('sirius domain admin')) {
		$esig_user = user_load($esig_uid);
		if ($esig_user->field_sirius_domain['und'][0]['target_id'] == sirius_domain_nid()) { return TRUE; }
	}

	// Oh well
	return FALSE;
}

function sirius_user_tab_esignature($esig_uid) {
	return drupal_get_form('sirius_user_tab_esignature_form', $esig_uid);
}

function sirius_user_tab_esignature_form($form, &$form_state, $esig_uid) {
	$esig_user = user_load($esig_uid);
	$form_state['esig_uid'] = $esig_uid;
	$json = sirius_jsonfield_util_user_json_get($esig_user);

	$form['#tree'] = TRUE;

	$form['esignature']['settings']['name'] = array(
		'#type' => 'textfield',
		'#title' => 'Name',
		'#description' => t('Enter your name as it should appear on a signature. For example, "Marvin the Martian, Alien Overlord".'),
		'#default_value' => $json['esignature']['settings']['name'],
	);

	$form['esignature']['settings']['org'] = array(
		'#type' => 'textfield',
		'#title' => 'Organization',
		'#description' => t('Enter your organization as it should appear on a signature. For example, "United Invasion Corporation Ltd.".'),
		'#default_value' => $json['esignature']['settings']['org'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_user_tab_esignature_form_submit($form, &$form_state) {
	$esig_user = user_load($form_state['esig_uid']);
	$json = sirius_jsonfield_util_user_json_get($esig_user);
	$json['esignature']['settings'] = $form_state['values']['esignature']['settings'];
	sirius_jsonfield_util_user_json_set($esig_user, $json);
	user_save($esig_user);
	drupal_set_message("Settings saved.");
}

function sirius_esignature_admin($form, &$form_state) {
	$form['#tree'] = TRUE;

	$roles = user_roles();
	$roles_avail = array();
	foreach ($roles as $rid => $role) {
		$avail = variable_get("sirius_domain_userrmanage_$rid", 'omit');
		if ($avail == 'optional' || $avail == 'required') { $roles_avail[$rid] = $role; }
	}

	$items = array_values(sirius_esignature_defs());
	$ajax_params = $form_state['triggering_element']['#sirius_ajax'];
  $item_count = $form_state['item_count'];
  $item_count += $ajax_params['qty'];
  if ($item_count < 1) { $item_count = 1; }
  if ($item_count < count($items) + 1) { $item_count = count($items) + 1; }
  $form_state['item_count'] = $item_count;

  $form['items']['prefix'] = array('#markup' => '<div id="items">');
  $form['items']['table_open'] = array('#markup' => '<table><tr><th>Title</th><th>Code</th><th>Name</th><th>Organization</th><th>Role</th></tr>');
	for ($i=0; $i<$item_count; ++$i) {
		$form['items'][$i]['tr_open'] = array('#markup' => '<tr>');

    $form['items'][$i]['title'] = array(
      '#type' => 'textfield',
      '#prefix' => '<td>',
      '#suffix' => '</td>',
      '#default_value' => $items[$i]['title'],
      '#size' => 30,
    );

    $form['items'][$i]['code'] = array(
      '#type' => 'textfield',
      '#prefix' => '<td>',
      '#suffix' => '</td>',
      '#size' => 10,
      '#default_value' => $items[$i]['code'],
    );

    $form['items'][$i]['name'] = array(
      '#type' => 'textfield',
      '#prefix' => '<td>',
      '#suffix' => '</td>',
      '#default_value' => $items[$i]['name'],
      '#size' => 30,
    );

    $form['items'][$i]['org'] = array(
      '#type' => 'textfield',
      '#prefix' => '<td>',
      '#suffix' => '</td>',
      '#default_value' => $items[$i]['org'],
      '#size' => 30,
    );

    $form['items'][$i]['roles'] = array(
      '#type' => 'checkboxes',
      '#prefix' => '<td>',
      '#suffix' => '</td>',
      '#options' => $roles_avail,
    );
    if ($items[$i]['roles']) { $form['items'][$i]['roles']['#default_value'] = $items[$i]['roles']; }

		$form['items'][$i]['tr_close'] = array('#markup' => '</tr>');
	}
	$form['items']['table_close'] = array('#markup' => '</table>');
  $form['items']['suffix'] = array('#markup' => '</div>');


  $form['add_one'] = array(
    '#type' => 'button',
    '#value' => t('Add a Row'),
    '#ajax' => array(
      'callback' => 'sirius_ajax_generic_replace',
    ),
    '#sirius_ajax' => array(
      'qty' => 1,
      'wrapper' => '#items',
      'element' => 'items',
    ),
    '#suffix' => '<br clear="left">',
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}


function sirius_esignature_admin_submit($form, &$form_state) {
	$entity_node = node_load($form_state['entity_nid']);
	$items = $form_state['values']['items'];
	foreach ($items as $delta => $item) {
		if (!$items[$delta]['code']) { $items[$delta]['code'] = $items[$delta]['title']; }
		$items[$delta]['code'] = strtolower($items[$delta]['code']);
		$items[$delta]['code'] = preg_replace('/[^a-z_]/', '_', $items[$delta]['code']);
		$items[$delta]['code'] = preg_replace('/_+/', '_', $items[$delta]['code']);
	}
	sirius_esignature_defs_set($items);
	drupal_set_message("Settings saved.");
}






























