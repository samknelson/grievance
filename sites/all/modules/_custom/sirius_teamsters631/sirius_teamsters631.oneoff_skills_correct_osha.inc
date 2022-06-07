<?php

function sirius_teamsters631_oneoff_skills_correct_osha() {
	sirius_require('sirius_teamsters631', 'sirius_teamsters631.oneoff_skills_correct.inc');
	return drupal_get_form('sirius_teamsters631_oneoff_skills_correct_osha_form');
}

function sirius_teamsters631_oneoff_skills_correct_osha_form($form, &$form_state) {
	$status_options = sirius_teamsters631_oneoff_skills_correct_status_options();

	$form['#tree'] = TRUE;

	$form['filters'] = array(
		'#type' => 'fieldset',
		'#title' => 'filters',
		'#collapsible' => TRUE,
	);

	$form['filters']['status'] = array(
		'#title' => 'Status',
		'#type' => 'select',
		'#options' => array(NULL => t('-- All Statuses --')) + $status_options,
	);

	$form['filters']['user_name'] = array(
		'#title' => 'User Name',
		'#type' => 'textfield',
		'#description' => t('Enter all or part of the user name of the person who created the log entry'),
	);

	$form['filters']['start_date'] = array(
		'#title' => 'Start Date',
		'#type' => 'textfield',
		'#description' => t('Search for log entries after this date'),
	);

	$form['filters']['end_date'] = array(
		'#title' => 'End Date',
		'#type' => 'textfield',
		'#description' => t('Search for log entries before this date'),
	);

	$form['filters']['worker_title'] = array(
		'#title' => 'Worker Name',
		'#type' => 'textfield',
		'#description' => t('Enter all or part of the worker\'s name'),
	);

	$form['filters']['limit'] = array(
		'#title' => 'Only Show This Many Records',
		'#type' => 'textfield',
		'#size' => 4,
	);

	$form['filters']['output'] = array(
		'#title' => 'Output',
		'#type' => 'select',
		'#options' => array(
			'screen' => 'Screen',
			'csv' => 'CSV Export',
		),
	);

	$form['filters']['submit_filter'] = array(
		'#type' => 'submit',
		'#value' => t('Search'),
	);
	if ($form_state['input']) {
		$form['filters']['#collapsed'] = TRUE;
	}


	if ($form_state['input']) {
		$sql_args = array();
		$sql = "select node.nid as log_nid, ";
		$sql .= "node.created as log_created, ";
		$sql .= "node.uid as uid, ";
		$sql .= "users.name as user_name, ";
		$sql .= "users.mail as user_mail, ";
		$sql .= "worker_node.nid as worker_nid, ";
		$sql .= "worker_node.title as worker_title, ";
		$sql .= "field_sirius_message_value as message, ";
		$sql .= "teamsters631_oneoff_data_correct.status as status, ";
		$sql .= "field_sirius_attachments_fid as attachment_fid ";
		$sql .= "from node ";
		$sql .= "left join teamsters631_oneoff_data_correct on teamsters631_oneoff_data_correct.nid = node.nid ";
		$sql .= "left join users on users.uid = node.uid ";
		$sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_message on field_data_field_sirius_message.entity_type = 'node' and field_data_field_sirius_message.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_log_handler on field_data_field_sirius_log_handler.entity_type = 'node' and field_data_field_sirius_log_handler.entity_id = node.nid ";
		$sql .= "left join node worker_node on worker_node.nid = field_sirius_log_handler_target_id ";
		$sql .= "left join field_data_field_sirius_attachments on field_data_field_sirius_attachments.entity_type = 'node' and field_data_field_sirius_attachments.entity_id = node.nid and field_data_field_sirius_attachments.delta = 0 ";
		$sql .= "where node.type = 'sirius_log' ";
		$sql .= "and field_sirius_category_value = 'worker:skill' ";
		$sql .= "and field_sirius_message_value like '%osha%' ";
		$sql .= "and ( ";
		$sql .= "  field_sirius_message_value like '%2017%' or ";
		$sql .= "  field_sirius_message_value like '%2018%' or ";
		$sql .= "  field_sirius_message_value like '%2019%' or ";
		$sql .= "  field_sirius_message_value like '%2020%' or ";
		$sql .= "  field_sirius_message_value like '%2021%' or ";
		$sql .= "  field_sirius_message_value like '%2022%' or ";
		$sql .= "  field_sirius_message_value like '%2023%' ";
		$sql .= ") ";

		if ($form_state['values']['filters']['user_name']) {
			$sql .= "and users.name like :user_name ";
			$sql_args['user_name'] = '%' . $form_state['values']['filters']['user_name'] . '%';
		}
		if ($form_state['values']['filters']['status']) {
			$sql .= "and teamsters631_oneoff_data_correct.status = :status ";
			$sql_args['status'] = $form_state['values']['filters']['status'];
		}
		if ($form_state['values']['filters']['start_date']) {
			$sql .= "and node.created >= :start_ts ";
			$sql_args['start_ts'] = strtotime($form_state['values']['filters']['start_date']);
		}
		if ($form_state['values']['filters']['end_date']) {
			$sql .= "and node.created <= :end_ts ";
			$sql_args['end_ts'] = strtotime($form_state['values']['filters']['end_date']) + 24*60*60;
		}
		if ($form_state['values']['filters']['worker_title']) {
			$sql .= "and worker_node.title like :worker_title ";
			$sql_args['worker_title'] = '%' . $form_state['values']['filters']['worker_title'] . '%';
		}
		$sql .= "order by worker_node.nid desc ";
		if ($form_state['values']['filters']['limit']) {
			$sql .= "limit " . (int) $form_state['values']['filters']['limit'];
		}
		$stmt = sirius_sql_query($sql, $sql_args);


		$is_csv = ($form_state['values']['filters']['output'] == 'csv');

		$header = array();
		$header[] = 'Status';
		$header[] = 'Worker';
		$header[] = 'Date';
		$header[] = 'User';
		$header[] = 'Log Entry';
		if (!$is_csv) { 
			$header[] = 'attachments';
			$header[] = 'Tools';
		}

		if ($is_csv) {
			$fp = fopen('php://output', 'w');
			$filename = 'data_correct.csv';
			drupal_add_http_header('Content-Type', 'text/csv');
			drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
			fputcsv($fp, $header);
		}

		$count = 0;
		$counts = array();
		$rows = array();
		while ($hr = $stmt->fetchAssoc()) {
			$tools = '';
			$tools .= l('Remove', 'node/' . $hr['worker_nid'] . '/sirius_worker_skill/remove', array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
			$tools .= l('Renew', 'node/' . $hr['worker_nid'] . '/sirius_worker_skill/renew', array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
			$tools .= l('Log', 'node/' . $hr['worker_nid'] . '/sirius_worker_skill/log', array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
			// $tools .= l('Original Log Entry', 'node/' . $hr['log_nid'], array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));

			if (!$is_csv) {
				$parts = explode('] [', $hr['message']);
				foreach ($parts as $delta => $part) {
					$parts[$delta] = preg_replace('/[\[\]]/', '', $part);
				}
				$msg = join('<br />', $parts);
			}

			$status = $status_options[$hr['status']];

			$attachments = '';
			if ($hr['attachment_fid']) { 
				$log_node = node_load($hr['log_nid']);
				$renderable = field_view_field('node', $log_node, 'field_sirius_attachments', 'full');
				// unset($renderable['#title']);
				$renderable['#label_display'] = 'hidden';
				// drupal_set_message("<pre>" . print_r($renderable, 1) . '</pre>');
				$attachments = render($renderable);
			}

			$row = array();
			$row[] = $status;
			$row[] = $hr['worker_title'];
			$row[] = date('Y-m-d h:i a', $hr['log_created']);
			$row[] = $hr['user_name'];
			$row[] = $msg;

			if (!$is_csv) { 
				$row[] = $attachments;
				$row[] = $tools;
			}

			if ($is_csv) { 
				fputcsv($fp, $row); 
			} else {
				$rows[$hr['log_nid']] = $row;
				++$count;
				++$counts[$hr['worker_nid']];
			}
		}
		if ($is_csv) { 
			fclose($fp); 
			drupal_exit();
		}

		drupal_set_message("Found $count log entries covering " . count($counts) . " different workers.");

		if ($rows) {
		  $form['log_nids'] = array(
		    '#type' => 'tableselect',
		    '#title' => 'Log Entries',
		    '#header' => $header,
		    '#options' => $rows,
		  );
			// $result = theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
		} else {
			$form['log_nids'] = array('#markup' => '<div><em>No log entries found</em></div>');
		}

		$form['status'] = array(
			'#type' => 'select',
			'#title' => t('Status'),
			'#options' => array(NULL => t('-- Select --')) + $status_options,
		);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Set Status'),
			'#name' => 'set_status',
		);
	}

	return $form;
}

function sirius_teamsters631_oneoff_skills_correct_osha_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$status = $form_state['values']['status'];
	if ($status && ($form_state['triggering_element']['#name'] == 'set_status')) {
		$log_nids = array_filter(array_values($form_state['values']['log_nids']));
		if ($log_nids) {
			$count = 0;
			foreach ($log_nids as $log_nid) {
				++$count;
				$sql = "insert into teamsters631_oneoff_data_correct (nid, status) VALUES (:nid, :status) ";
				$sql .= "on duplicate key update status = :status2";
				$stmt = sirius_sql_query($sql, array(':nid' => $log_nid, ':status' => $status, ':status2' => $status));
			}
			drupal_set_message("Set the status for $count records.");
		}
	}
}


/**
CREATE TABLE teamsters631_oneoff_data_correct (
  nid integer primary key,
  status varchar(255),
  index(status)
);
 */
