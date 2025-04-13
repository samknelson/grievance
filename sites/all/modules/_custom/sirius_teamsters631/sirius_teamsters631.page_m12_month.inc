<?php

function sirius_teamsters631_page_m12_month($ym, $filter = array()) {
	return drupal_get_form('sirius_teamsters631_page_m12_month_form', $ym, $filter);
}

function sirius_teamsters631_page_m12_month_form($form, &$form_state, $ym, $filter = array()) {
	$m12 = Sirius_Teamsters631_M12::getInstance();
	$status_options = $m12->status_options();
	if (!$filter) { $filter = array(); }

	list ($year, $month) = explode('-', $ym);
	if (!($year > 0) || ($year < 1900) || ($year > 2100) ) { $year = date('Y'); }
	if (!($month > 0) || ($month < 1) || ($month > 12) ) { $month = date('n'); }
	drupal_set_title("12th of the Month: $month/$year");

	$form_state['year'] = $year;
	$form_state['month'] = $month;

	$header = array();
	$header['employer_name'] = t('Employer');
	$header['status'] = t('Status');
	$header['reminder'] = t('Reminder');
	$header['dispatch'] = t('Dispatches');
	$header['received'] = t('Reminder');
	$header['received'] = t('Received');
	$header['layoffs'] = t('Ineligible Workers');
	$header['notify'] = t('Notification');
	$header['checked'] = t('Follow-Up');
	$header['tools'] = $tools;

	$employers = $m12->employers($year, $month);
	$rows = array();
	foreach ($employers as $employer_nid => $employer_name) {
		$m12_node = $m12->fetch($employer_nid, $year, $month, FALSE);
		$m12_json = sirius_json_get($m12_node);
		if (in_array('active', $filter)) {
			if (!$m12_json['teamsters631_m12']['dispatches']['count']) { continue; }
		}

		if (in_array('missing', $filter)) {
			$received = $m12->received_get($m12_node);
			if ($received['ts']) { continue; }
		}

		if (!$m12_node) {
			$status_display = '[empty]';
		} else {
			$status_display = $status_options[$m12->get_status($m12_node)];
		}

		$tools = l('Employer', "node/$employer_nid/grievance_shop_teamsters631_m12", array('attributes' => array('class' => array('tool-button'))));
		if ($m12_node) {
			$tools .= l('View', "node/" . $m12_node->nid . "/sirius_log_teamsters631_m12", array('attributes' => array('class' => array('tool-button'))));
		}

		$row = array();
		$row['employer_name'] = $employer_name;
		$row['status'] = $status_display;
		$row['reminder'] = $m12->reminder_render($m12_node);
		$row['dispatch'] = $m12->dispatches_render($m12_node);
		$row['layoffs'] = $m12->layoff_render($m12_node, 'summary');
		$row['received'] = $m12->received_render($m12_node);
		$row['notify'] = $m12->notify_render($m12_node);
		$row['checked'] = $m12->checked_render($m12_node);
		$row['tools'] = $tools;

		$rows[$employer_nid] = $row;
	}

	$form['employers'] = array(
		'#type' => 'tableselect',
		'#header' => $header,
		'#options' => $rows,
		'#empty' => t('No employers found with an active dispatch in the preceding month.'),
	);

	$form['action'] = array(
		'#type' => 'select',
		'#title' => t('Action'),
		'#options' => array(
			NULL => '-- Select --',
			'count' => 'Count Active Dispatches',
			'received' => 'Mark Received',
			'check' => 'Follow-Up',
			'reminder' => 'Send Reminder',
		),
		'#required' => TRUE,
	);

	$form['asof_date'] = array(
		'#type' => 'textfield',
		'#title' => t('As Of'),
    '#states' => array(
	    'visible' => array(
        array(':input[name="action"]' => array('value' => 'count')),
        array(':input[name="action"]' => array('value' => 'received')),
	    )
    ),
		'#description' => t('(Leave blank to search for current dispatches.)'),
	);

	$form['reminder'] = array(
		'#type' => 'fieldset',
		'#title' => t('Reminder Email'),
    '#states' => array(
	    'visible' => array(
        array(':input[name="action"]' => array('value' => 'reminder')),
	    )
    ),
    '#tree' => TRUE,
	);

	$form['reminder']['subject'] = array(
		'#type' => 'textfield', 
		'#title' => 'Subject',
		'#default_value' => 'Reminder - Please submit your 12th of the month report',
	);

	$body = '<p>' . 'This is a reminder that your 12th of the month report is past due.' . '</p>';

	$form['reminder']['body'] = array(
		'#type' => 'text_format',
		'#rows' => 30,
		'#default_value' => $body,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_teamsters631_page_m12_month_form_submit($form, &$form_state) {
	$m12 = Sirius_Teamsters631_M12::getInstance();

	$year = $form_state['year'];
	$month = $form_state['month'];
	$employer_nids = $form_state['values']['employers'];
	if ($employer_nids) { $employer_nids = array_filter(array_values($employer_nids)); }
	if (!$employer_nids) {
		drupal_set_message("Please select at least one employer.", 'error');
		return;
	}

	$asof_date = $form_state['values']['asof_date'];

	$action = $form_state['values']['action'];
	$count = 0;
	foreach ($employer_nids as $employer_nid) {
		++$count;

		$m12_node = $m12->fetch($employer_nid, $year, $month, TRUE);

		if ($action == 'count') {
			$m12->dispatches_count($m12_node, $asof_date);
		} else if ($action == 'received') {
			$m12->received_set($m12_node, $asof_date);
		} else if ($action == 'check') {
			$m12->checked_set($m12_node);
		} else if ($action == 'reminder') {
			$subject = $form_state['values']['reminder']['subject'];
			$body = sirius_richtext_handle_input($form_state['values']['reminder']['body']);

			$params = array();
		  $params['body'] = $body;
		  $params['subject'] =  $subject;
		  $params['handler_nids'] = array($m12_node->nid);

			$employer_node = node_load($employer_nid);
			$employer_json = sirius_json_get($employer_node);
			$emails = $employer_json['teamsters631_m12']['emails'];

		  if (!$emails) {
		  	drupal_set_message($employer_node->title . ': No emails found.', 'warning');
		  	continue;
		  }

			foreach ($emails as $email) {
		    $params['to'] = $email;
		    $result = sirius_email_send($params);
		    if ($result['success']) {
		    	drupal_set_message($employer_node->title . ": Sent to $email");
		    } else {
		    	drupal_set_message($employer_node->title . ": Send to $employer_name $email failed: " . $result['msg'], 'warning');
		    }
			}

			$m12->reminder_set($m12_node);
		}
	}

	drupal_set_message("Performed action $action on $count employers.");
}
