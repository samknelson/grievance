<?php

function sirius_node_tab_entity_log_view_form($form, &$form_state, $node, $log_bucket_id, $view_id) {
	$form_state['node'] = $node;
	$form_state['log_bucket_id'] = $log_bucket_id;
  $form['#tree'] = TRUE;

	$log_bucket = sirius_log_bucket($log_bucket_id);
  $form['quicknav'] = array('#markup' => sirius_node_tab_entity_log_quicknav($log_bucket));

  $form['quicknav'] = array('#markup' => sirius_node_tab_entity_log_quicknav($log_bucket));

  $form['filters'] = array(
  	'#type' => 'fieldset',
  	'#title' => 'filters',
  	'#collapsible' => TRUE,
  	'#collapsed' => TRUE,
  );

  if ($log_bucket['categories']) {
  	if (count($log_bucket['categories']) > 1) {
		  $form['filters']['category'] = array(
		  	'#type' => 'select',
		  	'#title' => t('Category'),
		  	'#options' => array(NULL => t('-- All Categories --')) + array_combine($log_bucket['categories'], $log_bucket['categories']),
		  );
		}
  } else {
	  $form['filters']['category'] = array(
	  	'#type' => 'textfield',
	  	'#title' => t('Category'),
	  	'#size' => 10,
	  );
	}

  $form['filters']['type'] = array(
  	'#type' => 'textfield',
  	'#title' => t('Type'),
  	'#size' => 10,
  );

  $form['filters']['message'] = array(
  	'#type' => 'textfield',
  	'#title' => t('Message'),
  	'#description' => t('Substring ok'),
  );

  $form['filters']['date_start'] = array(
  	'#type' => 'date_popup',
  	'#title' => t('Start Date'),
  );

  $form['filters']['date_end'] = array(
  	'#type' => 'date_popup',
  	'#title' => t('Start Date'),
  );

  $form['filters']['display'] = array(
  	'#type' => 'select',
  	'#options' => array(
  		'screen' => t('Show on Screen'),
  		'csv' => t('Export to Spreadsheet (CSV)')
  	),
  	'#title' => t('Display'),
  );

	$form['filters']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

  if ($log_bucket['categories']) {
  	$filter_notes = 'This view is restricted to the categories <strong>' . join(', ', $log_bucket['categories']) . '</strong>. ';
  }
  if ($log_bucket['categories_exclude']) {
  	$filter_notes = 'This view excludes the categories <strong>' . join(', ', $log_bucket['categories_exclude']) . '</strong>. ';
  }
  if ($filter_notes) {
  	$form['filters']['filter_notes'] = array('#markup' => '<p><em>' . $filter_notes . '</em></p>');
  }


	$nids = array($node->nid);
	if ($node->type == 'sirius_worker') {
		if ($node->field_sirius_contact['und'][0]['target_id']) { $nids[] = $node->field_sirius_contact['und'][0]['target_id']; }
	}

	$filters = $form_state['values']['filters'];
	$filters['view_id'] = $view_id;

	$form['results'] = array(
		'#markup' => '<hr>' . sirius_log_entity_render($nids, $log_bucket, $filters)
	);

	return $form;
}

function sirius_node_tab_entity_log_view_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	// drupal_set_message(t('Comment added.'));
}

function sirius_log_entity_render($nids, $log_bucket, $filters) {
	// Use view filters as an override
	$view_id = $filters['view_id'];
	if ($view_id) {
		if ($log_bucket['views'][$view_id]['filters']) {
			foreach ($log_bucket['views'][$view_id]['filters'] as $key => $value) {
				$filters[$key] = $value;
			}
		}
	}

	$display = $filters['display'];
	if ($display == 'csv') { $limit = 10000000; } else { $limit = 1000; }

	$sql = "select node.nid, ";
	$sql .= "node.created, ";
	$sql .= "field_sirius_category_value, ";
	$sql .= "field_sirius_type_value, ";
	$sql .= "concat(field_sirius_category_value, '|', field_sirius_type_value) as category_type, ";
	$sql .= "field_sirius_message_value, ";
	$sql .= "users.uid as user_uid, ";
	$sql .= "users.name as user_name, ";
	$sql .= "users.mail as user_mail, ";
	$sql .= "group_concat(file_managed.fid separator '|') as file_fids, ";
	$sql .= "group_concat(file_managed.uri separator '|') as file_uris, ";
	$sql .= "group_concat(file_managed.filesize separator '|') as file_sizes, ";
	$sql .= "group_concat(file_managed.filename separator '|') as file_names, ";
	$sql .= "group_concat(file_managed.filemime separator '|') as file_mimes ";
	$sql .= " from node ";
	$sql .= "left join field_data_field_sirius_log_handler on field_data_field_sirius_log_handler.entity_type = 'node' and field_data_field_sirius_log_handler.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_attachments on field_data_field_sirius_attachments.entity_type = 'node' and field_data_field_sirius_attachments.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_message on field_data_field_sirius_message.entity_type = 'node' and field_data_field_sirius_message.entity_id = node.nid ";
	$sql .= "left join file_managed on file_managed.fid = field_sirius_attachments_fid ";
	$sql .= "left join users on users.uid = node.uid ";
	$sql .= "where type = 'sirius_log' ";
	$sql .= "and field_sirius_log_handler_target_id in (:nids) ";
	$sql_args[':nids'] = $nids;
	if ($log_bucket['categories']) {
		$sql .= "and field_sirius_category_value in (:categories) ";
		$sql_args[':categories'] = $log_bucket['categories'];
	}
	if ($log_bucket['categories_exclude']) {
		$sql .= "and field_sirius_category_value not in (:categories_exclude) ";
		$sql_args[':categories_exclude'] = $log_bucket['categories_exclude'];
	}
	if ($filters['category']) {
		$sql .= "and field_sirius_category_value = :filter_category ";
		$sql_args[':filter_category'] = $filters['category'];
	}
	if ($filters['type']) {
		$sql .= "and field_sirius_type_value = :filter_type ";
		$sql_args[':filter_type'] = $filters['type'];
	}
	if ($filters['message']) {
		$sql .= "and field_sirius_message_value like :filter_message ";
		$sql_args[':filter_message'] = '%' . $filters['message'] . '%';
	}
	if ($filters['date_start']) {
		$ts = strtotime($filters['date_start']);
		if ($ts > 1) {
			$sql .= "and node.created >= :ts_start ";
			$sql_args[':ts_start'] = $ts;
		}
	}
	if ($filters['date_end']) {
		$ts = strtotime($filters['date_end']);
		if ($ts > 1) {
			$sql .= "and node.created <= :ts_end ";
			$sql_args[':ts_end'] = $ts;
		}
	}

	if ($filters['category_types']) {
		$sql .= "and concat(field_sirius_category_value, '|', field_sirius_type_value) in (:category_types) ";
		$sql_args[':category_types'] = $filters['category_types'];
	}
	$sql .= "group by node.nid ";
	$sql .= "order by created desc, node.nid desc ";
	$sql .= "limit $limit ";

	$stmt = sirius_sql_query($sql, $sql_args);

	$header = array();
	if ($display == 'csv') {
		$header[] = t('Date');
		$header[] = t('User');
		$header[] = t('Category');
		$header[] = t('Type');
		$header[] = t('Message');
	} else {
		$header[] = t('Date<br />User');
		$header[] = t('Category<br />Type');
		$header[] = t('Message');
		$header[] = t('Attachments');
		$header[] = t('Tools');
	}

	if ($display == 'csv') {
		$filename = "log_" . $log_bucket_id . '_' . join('_', $nids) . '.csv';
		drupal_add_http_header('Content-Type', 'text/csv');
		drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
		$fp = fopen('php://output', 'w');
		fputcsv($fp, $header);
	}

	$rows = array();
	while ($hr = $stmt->fetchAssoc()) {
		$attachments_render = '';
		if ($hr['file_uris']) {
			$file_uris = explode('|', $hr['file_uris']);
			$file_names = explode('|', $hr['file_names']);
			$file_mimes = explode('|', $hr['file_mimes']);
			foreach ($file_uris as $delta => $file_uri) {
				$file = new stdClass();
				$file->filemime = $file_mimes[$delta];
				if ($attachments_render) { $attachments_render .= '<br />'; }
				$attachments_render .= '<span style="white-space: nowrap;">';
				$attachments_render .= theme_file_icon(array('file' => $file, 'alt' => $file_names[$delta]));
				$attachments_render .= l($file_names[$delta], file_create_url($file_uri));
				$attachments_render .= '</span>';
			}
		}

		$row = array();

		if ($display == 'csv') {
			$row[] = date('n/j/Y g:ia', $hr['created']);
			$row[] = $hr['user_name'];
			$row[] = $hr['field_sirius_category_value'];
			$row[] = $hr['field_sirius_type_value'];
		} else {
			$row[] = date('n/j/Y\<\b\r\ \/\>g:ia', $hr['created']) . '<br />' . $hr['user_name'];
			$row[] = $hr['field_sirius_category_value'] . '<br />' . $hr['field_sirius_type_value'];
		}
		$row[] = $hr['field_sirius_message_value'];
		if ($display != 'csv') {
			$row[] = $attachments_render;
			$row[] = l('View', 'node/' . $hr['nid'], array('attributes' => array('class' => array('tool-button'))));
		}

		if ($display == 'csv') {
			fputcsv($fp, $row);
		} else {
			$rows[] = $row;
		}
	}

	if ($display == 'csv') { drupal_exit(); }

	if (!$rows) { return '<p><em>' . t('There are no matching log entries.') . '</em></p>'; }

	$html = '';

	$count = count($rows);
	$html .= "<p>";
	$html .= "<strong>Found $count log entries.</strong> ";
	if ($count >= $limit-1) {
		$html .= "(Limiting output to $limit entries. To see more records, filter by date or export to CSV.) ";
	}
	$html .= '</p>';

	$html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));

	return $html;
}

