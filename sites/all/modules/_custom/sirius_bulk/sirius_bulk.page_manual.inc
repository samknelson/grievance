<?php

function sirius_bulk_page_manual() {
	$html = '<div class="sirius_bulk_manual_instructions">';
	$html .= 'The messages below have recipients that require manual confirmation.';
	$html .= '</div>';

	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_bulk');
	$domain_nid = sirius_domain_nid();
  if ($domain_nid) {
  	$query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid);
  }
	$query->fieldCondition('field_sirius_bulk_status', 'value', 'pending');
  $stmt = $query->execute();
  $bulk_nids = array();
  if ($stmt['node']) { $bulk_nids = array_keys($stmt['node']); }

  $rows = array();
  foreach ($bulk_nids as $bulk_nid) {
  	$summary = sirius_bulk_queue_summary($bulk_nid);
  	$count = $summary['add-manual'];
  	if ($count > 0) {
  		$bulk_node = node_load($bulk_nid);
  		$row = array();
  		$row[] = $bulk_node->title;
  		$row[] = number_format($count, 0);
  		$row[] = l('Manual Confirmation', "node/$bulk_nid/sirius_bulk_manual", array('attributes' => array('class' => array('tool-button'))));
  		$rows[] = $row;
  	}
  }
  if ($rows) {
  	$header = array('Message', 'Recipients Pending Confirmation', 'Tools');
  	$html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
  } else {
  	$html .= '<div><em>' . t('There are no bulk messages with recipients needing manual confirmation.') . '</em></div>';
  }

  return $html;
}

function sirius_bulk_node_tab_manual($bulk_node) {
	return drupal_get_form('sirius_bulk_node_tab_manual_form', $bulk_node);
}

function sirius_bulk_node_tab_manual_form($form, &$form_state, $bulk_node) {
	drupal_add_css(drupal_get_path('module', 'sirius_bulk') . '/css/bulk.css');

	$form_state['bulk_nid'] = $bulk_node->nid;

	$form['#tree'] = TRUE;

	$form['summary'] = array('#markup' => sirius_bulk_queue_summary_render($bulk_node));

	$medium = $bulk_node->field_sirius_bulk_medium['und'][0]['value'];
	$instructions = '<div class="sirius_bulk_manual_instructions">';
	$instructions .= sirius_fakerender_field('Subject:', $bulk_node->title);
	if (in_array($medium, array('email', 'choice'))) 	{ $instructions .= sirius_fakerender_field('Body:', $bulk_node->body['und'][0]['value']); }
	if (in_array($medium, array('sms', 'choice'))) 		{ $instructions .= sirius_fakerender_field('SMS:', $bulk_node->field_sirius_sms['und'][0]['value']); }
	if (in_array($medium, array('voice', 'choice'))) 	{ $instructions .= sirius_fakerender_field('SMS:', $bulk_node->field_sirius_voice['und'][0]['value']); }
	$instructions .= "Click each of the recipients below to deliver the message.";
	$instructions .= '</div>';
	$form['instructions'] = array('#markup' => $instructions);

	$batch = sirius_bulk_manual_get_my_batch($bulk_node->nid);
	if (!$batch) {
		$form['noneleft'] = array('#markup' => '<br /><br /><em>No more recipients need confirmation.</em>');
		return $form;
	}
	$form['confirmed'] = array(
		'#type' => 'fieldset',
		'#title' => 'Recipients',
		'#prefix' => '<div class="sirius_bulk_manual_recipients">',
		'#suffix' => '</div>',
	);
	foreach ($batch as $log_nid => $log_node) {
		$form['confirmed'][$log_nid] = array(
			'#type' => 'checkbox',
			'#title' => $log_node->field_sirius_message['und'][0]['value'] . '<br />[#' . $log_node->nid . ']',
			'#prefix' => '<div class="sirius_bulk_manual_recipient">',
			'#suffix' => '</div>',
		);
	}

	$form['submit'] = array('#type' => 'submit', '#value' => t('Send to the selected recipients'));
	return $form;
}

function sirius_bulk_node_tab_manual_form_submit($form, &$form_state) {
	global $user;
	$count = $skipped = 0;

	$options = $form_state['values']['confirmed'];
	if ($options) {
		foreach ($options as $log_nid => $confirmed) {
			if (!$confirmed) {
				++$skipped;
				continue;
			}
			++$count;

			$log_node = node_load($log_nid);
			if ($log_node->field_sirius_type['und'][0]['value'] == 'add-manual') {
				$log_node->field_sirius_type['und'][0]['value'] = 'add';
				sirius_jsonfield_log($log_node, 'confirmed', 'Delivery confirmed at ' . date('c') . ' by ' . $user->name . ' [' . $user->mail . '] ');
			}
			node_save($log_node);
		}
	}

	drupal_set_message(t("Confirmed @count recipients, skipped @skipped.", array('@count' => $count, '@skipped' => $skipped)));
}

function sirius_bulk_manual_get_my_batch($bulk_nid) {
	global $user;
	$sql_args = array();
	$sql = "select nid, field_sirius_json_value, json_unquote(json_extract(field_sirius_json_value, '\$.bulk.lock.ts')) as ts ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_log_handler on field_data_field_sirius_log_handler.entity_type = 'node' and field_data_field_sirius_log_handler.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_log' ";
	$sql .= "and field_sirius_log_handler_target_id = :bulk_nid ";
	$sql_args[':bulk_nid'] = $bulk_nid;
	$sql .= "and field_sirius_category_value = 'bulk:queue' ";
	$sql .= "and field_sirius_type_value = 'add-manual' ";
	$sql .= "and (json_unquote(json_extract(field_sirius_json_value, '\$.bulk.lock.ts')) is null ";
	$sql .= "     or cast(json_unquote(json_extract(field_sirius_json_value, '\$.bulk.lock.ts')) as signed) < :ts ";
	$sql .= "     or cast(json_unquote(json_extract(field_sirius_json_value, '\$.bulk.lock.uid')) as signed) = :uid ";
	$sql .= ") ";
	$sql_args[':ts'] = time() - 120;
	$sql_args[':uid'] = $user->uid;
	$sql .= "order by nid limit 24 ";
  $stmt = sirius_sql_query($sql, $sql_args);

  $batch = array();
  while ($hr = $stmt->fetchAssoc()) {
  	$log_node = node_load($hr['nid']);
  	$json = sirius_jsonfield_util_node_json_get($log_node);
  	$json['bulk']['lock'] = array('ts' => time(), 'uid' => $user->uid);
  	sirius_jsonfield_util_node_json_set($log_node, $json);
  	node_save($log_node);
  	$batch[$hr['nid']] = $log_node;
  }

  return $batch;
}