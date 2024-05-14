<?php

function sirius_disclaimer_admin($form, &$form_state) {
  $name = sirius_domain_variable_name('sirius_disclaimer_notify_subject');
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t('Notify Subject'),
    '#default_value' => variable_get($name, 'The following disclaimer(s) have been accepted.'),
    '#description' => t('Subject of the email notification that is sent when somebody accepts a disclaimer.')
  );

  $name = sirius_domain_variable_name('sirius_disclaimer_notify_intro');
  $form[$name] = array(
    '#type' => 'textarea',
    '#title' => t('Notify Intro'),
    '#default_value' => variable_get($name, '<em>The following disclaimers have been accepted.</em><br /><br />'),
    '#description' => t('Body of the email notification that is sent when somebody accepts a disclaimer. May contain HTML. It is followed by the text of the disclaimer.')
  );

  return system_settings_form($form);
}

function sirius_disclaimer_page_list() {
	drupal_set_title('Disclaimers');
	$domain_nid = sirius_domain_nid();
	$type = 'sirius:disclaimer';
	$html = views_embed_view('sirius_json_definitions', 'block_3', $domain_nid, $type);
	$html .= l('Add a Disclaimer', current_path() . '/add', array('query' => array('destination' => current_path()), 'attributes' => array('class' => array('tool-button'), 'query')));
	return $html;
}

function sirius_node_tab_json_definition_disclaimer_access($disclaimer_node) {
	return ($disclaimer_node->field_sirius_type['und'][0]['value'] == 'sirius:disclaimer');
}

function sirius_node_tab_json_definition_disclaimer($disclaimer_node) {
	return drupal_get_form('sirius_node_tab_json_definition_disclaimer_form', $disclaimer_node);
}

function sirius_disclaimer_accept_text_default() {
	return 'By checking this box I accept the terms of the above disclaimer.';
}

function sirius_disclaimer_add() {
	return drupal_get_form('sirius_node_tab_json_definition_disclaimer_form');
}

function sirius_node_tab_json_definition_disclaimer_ajax_accept_text($form, $form_state) {
  $commands = array();
  $commands[] = ajax_command_replace('#sirius_node_tab_json_definition_disclaimer_ajax_accept_text', render($form['disclaimer']['accept_text']));
  return array(
    '#type' => 'ajax',
    '#commands' => $commands
  );
}

function sirius_node_tab_json_definition_disclaimer_form($form, &$form_state, $disclaimer_node = NULL) {
	if (!$disclaimer_node) { $disclaimer_node = new stdClass(); }
	$form_state['disclaimer_nid'] = $disclaimer_node->nid;
	$json = sirius_jsonfield_util_node_json_get($disclaimer_node);
	$disclaimer_json = $json['disclaimer'];
	if (!$disclaimer_json['title']) { $disclaimer_json['title'] = $disclaimer_node->title; }
	$form['#tree'] = TRUE;
	$form['intro'] = array('#markup' => '<div class="sirius_form_intro">' . l('Back to disclaimer list', 'sirius/disclaimer/list', array('attributes' => array('class' => array('tool-button')))) . '</div>');
	$form['disclaimer']['title'] = array(
		'#type' => 'textfield',
		'#title' => t('Title'),
		'#default_value' => $disclaimer_json['title'],
	);

	$form['disclaimer']['active'] = array(
		'#type' => 'select',
		'#title' => 'Active?',
		'#default_value' => $disclaimer_json['active'],
		'#options' => array(
			'Yes' => t('Yes'),
			'No' => t('No'),
		),
	);

	$form['disclaimer']['body'] = array(
		'#type' => 'text_format',
		'#title' => 'Disclaimer Body',
		'#default_value' => $disclaimer_json['body'],
		'#format' => 'sirius_form_editor',
		'#allowed_formats' => array('sirius_form_editor'),
	);
	if ($disclaimer_node->nid) {
		$form['disclaimer']['body']['#description'] = t('Warning: changing the body of a disclaimer will cause all previous acceptances of that disclaimer to be marked as "invalid".');
	}

	$accept_lines = $form_state['accept_lines'];
	if (!$accept_lines && is_array($disclaimer_json['accept_text'])) { $accept_lines = count(array_filter($disclaimer_json['accept_text'])); }
  if (!$accept_lines) { $accept_lines = 1; }
  if ($form_state['clicked_button']['#value'] == 'Add one') { $accept_lines++; }
  if ($form_state['clicked_button']['#value'] == 'Add ten') { $accept_lines += 10; }
  $form_state['accept_lines'] = $accept_lines;

	$form['disclaimer']['accept_text'] = array(
		'#type' => 'fieldset',
		'#prefix' => '<div id="sirius_node_tab_json_definition_disclaimer_ajax_accept_text">',
		'#suffix' => '</div>',
	);
  for ($i=0; $i<$accept_lines; ++$i) {
		$form['disclaimer']['accept_text'][$i] = array(
			'#type' => 'textfield',
			'#title' => t('Accept Line #@i', array('@i' => ($i+1))),
			'#default_value' => $disclaimer_json['accept_text'][$i],
		);
	}
	$form['disclaimer']['accept_text'][0]['#disclaimer'] = 'For example, ' . sirius_disclaimer_accept_text_default();

  $form['disclaimer']['add_one'] = array(
    '#type' => 'button',
    '#value' => t('Add one'),
    '#ajax' => array('callback' => 'sirius_node_tab_json_definition_disclaimer_ajax_accept_text'),
    '#suffix' => '<br clear="both" />'
  );

	$form['disclaimer']['notify'] = array(
		'#type' => 'select',
		'#title' => t('Notify When Accepted'),
		'#default_value' => $disclaimer_json['notify'],
		'#options' => array(
			'Yes' => t('Yes'),
			'No' => t('No'),
		),
		'#required' => TRUE,
		'#description' => t('When this disclaimer is accepted, a notification should be sent to the person who accepted it.'),
	);

	$form['disclaimer']['esig']['enabled'] = array(
		'#type' => 'select',
		'#title' => t('Require eSignature?'),
		'#default_value' => $disclaimer_json['esig'],
		'#options' => array(
			'Yes' => t('Yes'),
			'No' => t('No'),
		),
		'#required' => TRUE,
		'#description' => t('Require an eSignature?'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_json_definition_disclaimer_form_submit($form, &$form_state) {
	$disclaimer_node = node_load($form_state['disclaimer_nid']);

	if (!$disclaimer_node->nid) {
		global $user;
		$domain_nid = sirius_domain_nid();
	  $disclaimer_node = new stdClass();
	  $disclaimer_node->type = 'sirius_json_definition';
	  $disclaimer_node->no_recurse = TRUE;
	  $disclaimer_node->language = LANGUAGE_NONE;
	  $disclaimer_node->uid = $user->uid;
	  $disclaimer_node->status = 1;
	  $disclaimer_node->promote = 0;
	  $disclaimer_node->comment = 0;
	  if ($domain_nid) { $disclaimer_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
	  $disclaimer_node->field_sirius_feed_status['und'][0]['value'] = 'draft';
	  $disclaimer_node->field_sirius_type['und'][0]['value'] = 'sirius:disclaimer';
	  $disclaimer_node = node_submit($disclaimer_node);
	  node_object_prepare($disclaimer_node);
	}

	$json = sirius_json_get($disclaimer_node);
	$json['disclaimer'] = $form_state['values']['disclaimer'];
	$json['disclaimer']['body'] = sirius_richtext_handle_input($form_state['values']['disclaimer']['body']);
	sirius_json_set($disclaimer_node, $json);
	$disclaimer_node->title = $form_state['values']['disclaimer']['title'];
	node_save($disclaimer_node);

	// This is hacky, but ... the "disclaimer" json should know its own NID. So we might resave it.s
	if ($json['disclaimer']['nid'] != $disclaimer_node->nid) {
		$json['disclaimer']['nid'] = $disclaimer_node->nid;
		sirius_json_set($disclaimer_node, $json);
		node_save($disclaimer_node);
	}
	drupal_set_message("Settings saved.");
}

function sirius_disclaimer_esig_which($disclaimer_json) {
	return 'esig_disclaimer_' . $disclaimer_json['nid'];
}

function sirius_disclaimer_list() {
	$domain_nid = sirius_domain_nid();

	$sql_args = array();
	$sql = "select nid, title from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_json_definition' ";
	$sql .= "and field_sirius_type_value = 'sirius:disclaimer' ";
	if ($domain_nid) { 
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	}
	$sql .= "and json_extract(field_sirius_json_value, '\$.disclaimer.active') = 'Yes' ";
	$sql .= "order by title ";

	$stmt = sirius_sql_query($sql, $sql_args);
	$disclaimer_options = array();
	while ($hr = $stmt->fetchAssoc()) {
		$disclaimer_options[$hr['nid']] = $hr['title'];
	}

	return $disclaimer_options;
}

function sirius_disclaimer_accept($host_node, $disclaimer_nodes, $options = array()) {
	global $user;
	$json = sirius_jsonfield_util_node_json_get($host_node);

	$notify_html = '';
	$log = array();
	foreach ($disclaimer_nodes as $disclaimer_node) {
		$disclaimer_nid = $disclaimer_node->nid;
		if ($json['disclaimer']['disclaimer_nid']) {
			$json['disclaimer']['disclaimer_nid'] = array();
			$json['disclaimer'][$disclaimer_nid]['accepted'] = TRUE;
		}

		$acceptance = array();
		if ($options['acceptance']) { $acceptance = $options['acceptance']; }
		$acceptance['ts'] = time();
		$acceptance['uid'] = $user->uid;
		$acceptance['user_name'] = $user->name;
		$acceptance['title'] = $disclaimer_node->title;
		$acceptance['nid'] = $disclaimer_nid;

		$disclaimer_json = sirius_jsonfield_util_node_json_get($disclaimer_node);
		$disclaimer_body = $disclaimer_json['disclaimer']['body'];
		$acceptance['sig'] = drupal_hash_base64($disclaimer_body);

		if ( ($disclaimer_json['disclaimer']['notify'] == 'Yes') && !$json['disclaimer'][$disclaimer_nid]['acceptances'] && !$options['skip_notify']) {
			$notify_html .= '<hr>' . sirius_disclaimer_render($disclaimer_json['disclaimer']);
		}
		$json['disclaimer'][$disclaimer_nid]['acceptances'][] = $acceptance;
	}

 	sirius_jsonfield_util_node_json_set($host_node, $json);
	node_save($host_node);

	if ($notify_html) {
		$notify_html = sirius_domain_variable_get('sirius_disclaimer_notify_intro', 'The following disclaimer(s) have been accepted.') . $notify_html;
    $params = array();
    $params['body'] = $notify_html;
    $params['subject'] =  sirius_domain_variable_get('sirius_disclaimer_notify_subject', 'The following disclaimer(s) have been accepted.');
    $params['to'] = $user->mail;
    sirius_email_send($params);
	}
}

function sirius_disclaimer_sig_check($sig, $disclaimer_json) {
	return $sig == drupal_hash_base64($disclaimer_json['body']);
}

function sirius_disclaimer_render($disclaimer_json, $options = array()) {
	drupal_add_css('sites/all/modules/_custom/sirius/css/disclaimer.css');

	$html = '';
	if (!$options['hide_wrap']) { $html .= '<div class="sirius_disclaimer">'; }
	$title = $disclaimer_json['title'];
	if (!$title) { $title = 'Disclaimer'; }
	$html .= '<h2>' . $title . '</h2>';
	$html .= $disclaimer_json['body'];
	if (!$options['hide_wrap']) { $html .= '</div>'; }
	return $html;
}

function sirius_disclaimer_render_form($disclaimer_json, $acceptance_json = array()) {
	$form['disclaimer_markup'] = array('#markup' => sirius_disclaimer_render($disclaimer_json, array('hide_wrap' => TRUE)));
	if (is_array($disclaimer_json['accept_text'])) {
		foreach (array_filter($disclaimer_json['accept_text']) as $delta => $accept_text) {
			$form['disclaimer_accept'][$delta] = array(
				'#type' => 'checkbox',
				'#title' => $accept_text,
				'#required' => TRUE,
		    '#return_value' => TRUE,
		    '#default_value' => FALSE,
		 	);
		 	if ($acceptance_json) {
		 		$form['disclaimer_accept'][$delta]['#disabled'] = TRUE;
		 		$form['disclaimer_accept'][$delta]['#value'] = TRUE;
		 	}
		}
	}
 	$class = 'sirius_disclaimer ';
 	if ($acceptance_json) {
		$form['disclaimer_accept']['#suffix'] = '<strong>Accepted date:</strong> ' . date('l, F j, g:i A', $acceptance_json['ts']);
 		$form['disclaimer_accept']['#suffix'] .= '<br /><strong>Accepted by:</strong> ' . $acceptance_json['user_name'];
 		$form['disclaimer_accept']['#suffix'] .= '<br /><strong>Hash:</strong> ' . $acceptance_json['sig'];
		if (sirius_disclaimer_sig_check($acceptance_json['sig'], $disclaimer_json)) {
 			$form['disclaimer_accept']['#suffix'] .= " / verified";
 		} else {
 			$form['disclaimer_accept']['#suffix'] .= " / <strong>verification failed: hash does not match text</strong>";
 		}

 		$class .= 'sirius_disclaimer_accepted ';
 	}
	$form['#prefix'] = "<div class=\"$class\">";
	$form['#suffix'] = '</div>';

	if ($disclaimer_json['esig']['enabled'] == 'Yes') {
		sirius_esignature_init();

		$esig_which = sirius_disclaimer_esig_which($disclaimer_json);

		if (!$acceptance_json) {
			sirius_esignature_form_prepare(
				$form, 
				$esig_which, 
				$disclaimer_json['title'], 
				$disclaimer_json['body'], 
				array(
					'hide_title' => TRUE,
					'hide_statement' => TRUE,
					'hide_submit' => TRUE,
				)
			);
		} else {
			if ($acceptance_json['esig_nid']) {
				$esig_node = node_load($acceptance_json['esig_nid']);
				$esig_json = sirius_json_get($esig_node);
				$esig_which = sirius_disclaimer_esig_which($disclaimer_json);
				$esignature = sirius_jsonfield_util_esignature_get($esig_json, $esig_which);
				if ($esignature) { 
					$html = '<div class="sirius_disclaimer_esig_signature">';
					$html .= sirius_esignature_render($esignature, '', $esig_node->nid, $esig_which);
					$html .= '</div>';
					$form['esig'] = array('#markup' => $html);
				}
			}
		}
	}

	return $form;
}
