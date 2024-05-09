<?php

function sirius_do_not_disturb_tz() {
	$domain_nid = sirius_domain_nid();
	if ($domain_nid) {
		$sql = "select field_sirius_tz_value from field_data_field_sirius_tz ";
		$sql .= "where entity_type = 'node' and entity_id = :domain_nid ";
		$sql_args = array(':domain_nid' => $domain_nid);
		$stmt = sirius_sql_query($sql, $sql_args);
		$hr = $stmt->fetchAssoc();
		$tz = $hr['field_sirius_tz_value'];
	}

	if (!$tz) {
		$tz = variable_get('date_default_timezone');
	}

	return $tz;
}

function sirius_page_do_not_disurb_config() { return drupal_get_form('sirius_page_do_not_disurb_config_form'); }

function sirius_page_do_not_disurb_config_form($form, &$form_state) {
	$form['current_time'] = array('#markup' => sirius_tz_indicator(sirius_do_not_disturb_tz()));

	$json = sirius_do_not_disturb_json();

  $days = array(
  	1 => 'Monday',
  	2 => 'Tuesday',
  	3 => 'Wednesday',
  	4 => 'Thursday',
  	5 => 'Friday',
  	6 => 'Saturday',
  	7 => 'Sunday',
  );

  $hours = array();
  for ($i=0; $i<=24; ++$i) { $hours[$i] = $i; }

  $minutes = array();
  for ($i=0; $i<=59; ++$i) { $minutes[$i] = str_pad($i, 2, '0', STR_PAD_LEFT); }

	if ($json['schedules']) { 
		$count = count($json['schedules']) + 2;
	} else {
		$count = 2;
	}
	for ($i=0; $i<$count; ++$i) {
		$title = "Secheudle #" . ($i+1);
		if ($json['schedules'][$i]['title']) { $title = $json['schedules'][$i]['title']; }
		if ($json['schedules'][$i]['id']) { $title .= ' (' . $json['schedules'][$i]['id'] . ')'; }

		$form['do_not_disturb']['schedules'][$i] = array(
			'#type' => 'fieldset',
			'#title' => $title,
			'#collapsible' => TRUE,
			'#collapsed' => TRUE,
		);

		$form['do_not_disturb']['schedules'][$i]['title'] = array(
			'#type' => 'textfield',
			'#title' => 'Title',
			'#default_value' => $json['schedules'][$i]['title'],
			'#description' => t('Enter a title to identify this schedule, i.e. "Standard Phone Bank".'),
		);
		$form['do_not_disturb']['schedules'][$i]['id'] = array(
			'#type' => 'textfield',
			'#title' => 'ID',
			'#default_value' => $json['schedules'][$i]['id'],
			'#description' => t('Enter an alphanumeric ID for this schedule, i.e. "standard_phone_bank".'),
		);
		if ($i == 0) {
			$form['do_not_disturb']['schedules'][$i]['id']['#default_value'] = 'default';
			$form['do_not_disturb']['schedules'][$i]['id']['#disabled'] = TRUE;
		}

  	$form['do_not_disturb']['schedules'][$i]['days']['table_open'] = array('#markup' => '<table><tr><th>Day</th><th>Can Send on This Day</th><th>Start</th><th>Stop</th></tr>');

	  foreach ($days as $day => $day_label) {
	  	$form['do_not_disturb']['schedules'][$i]['days'][$day]['tr_open'] = array('#markup' => '<tr>');

	  	$form['do_not_disturb']['schedules'][$i]['days'][$day]['enable'] = array(
	  		'#type' => 'select',
	  		'#options' => array('Yes' => 'Yes', 'No' => 'No'),
	  		'#default_value' => variable_get($var, 'Yes'),
	  		'#prefix' => "<td>$day_label</td><td>",
	  		'#suffix' => '</td>',
	  		'#default_value' => $json['schedules'][$i]['days'][$day]['enable'],
	  	);

	  	$form['do_not_disturb']['schedules'][$i]['days'][$day]['start_hour'] = array(
	  		'#type' => 'select',
	  		'#options' => $hours,
	  		'#default_value' => variable_get($var, 9),
	  		'#prefix' => '<td><div style="float: left;">',
	  		'#suffix' => '</div>',
	  		'#default_value' => $json['schedules'][$i]['days'][$day]['start_hour'],
	  	);

	  	$form['do_not_disturb']['schedules'][$i]['days'][$day]['start_minute'] = array(
	  		'#type' => 'select',
	  		'#options' => $minutes,
	  		'#default_value' => variable_get($var, 0),
	  		'#prefix' => '<div style="float: left;">',
	  		'#suffix' => '</div></td>',
	  		'#default_value' => $json['schedules'][$i]['days'][$day]['start_minute'],
	  	);

	  	$form['do_not_disturb']['schedules'][$i]['days'][$day]['stop_hour'] = array(
	  		'#type' => 'select',
	  		'#options' => $hours,
	  		'#default_value' => variable_get($var, 17),
	  		'#prefix' => '<td><div style="float: left;">',
	  		'#suffix' => '</div>',
	  		'#default_value' => $json['schedules'][$i]['days'][$day]['stop_hour'],
	  	);

	  	$form['do_not_disturb']['schedules'][$i]['days'][$day]['stop_minute'] = array(
	  		'#type' => 'select',
	  		'#options' => $minutes,
	  		'#default_value' => variable_get($var, 0),
	  		'#suffix' => '</td>',
	  		'#default_value' => $json['schedules'][$i]['days'][$day]['stop_minute'],
	  	);
	  	
	  	$form['do_not_disturb']['schedules'][$i]['days'][$day]['tr_close'] = array('#markup' => '</tr>');
	  }
  	$form['do_not_disturb']['schedules'][$i]['days']['table_close'] = array('#markup' => '</table>');
  }

	$form['#tree'] = TRUE;

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_page_do_not_disurb_config_form_submit($form, &$form_state) {
	// Need to clear any empty schedules, otherwise we'll add two each time.
	$values = $form_state['values']['do_not_disturb'];
	foreach ($values['schedules'] as $delta => $record) {
		if (!$record['title'] && !$record['id']) { unset($values['schedules'][$delta]); }
	}
	$values['schedules'] = array_values($values['schedules']);

	sirius_domain_variable_set('sirius_do_not_disturb', json_encode($values));
	drupal_set_message("Settings saved.");
}


function sirius_page_do_not_disurb_test() { return drupal_get_form('sirius_page_do_not_disurb_test_form'); }

function sirius_page_do_not_disurb_test_form($form, &$form_state) {
	$form['current_time'] = array('#markup' => sirius_tz_indicator(sirius_do_not_disturb_tz()));

	$options = sirius_do_not_disturb_list();
	$form['id'] = array(
		'#type' => 'select',
		'#options' => $options,
		'#title' => t('Which Setting?'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_page_do_not_disurb_test_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$result = sirius_do_not_disturb_check($form_state['values']['id']);
	if ($result['success']) {
		drupal_set_message('Sending is possible now.');
	} else {
		drupal_set_message("Sending is NOT possible now: " . $result['msg'], 'warning');
	}
}

function sirius_do_not_disturb_list() {
	$json = sirius_do_not_disturb_json();

	if (!$json['schedules']) { return array(); }
	$options = array();
	foreach ($json['schedules'] as $delta => $record) {
		$id = $record['id'];
		if (!$id) { continue; }
		$title = $record['title'];
		if (!$title) { $title = $id; }
		$options[$id] = $title;
	}
	return $options;
}

function sirius_do_not_disturb_check($id = 'default', $ts = NULL) {
	if (!$ts) { $ts = time(); }

	$json = sirius_do_not_disturb_json();

	if (!$json['schedules']) { return array('success' => FALSE, 'msg' => 'No Do Not Disturb schedules have been configured.'); }

	$found = FALSE;
	foreach ($json['schedules'] as $delta => $schedule) {
		if ($id == $schedule['id']) { $found = TRUE; break; }
	}
	if (!$found) { return array('success' => FALSE, 'msg' => 'No Do Not Disturb schedules with id [' . $id . '] was not found.'); }

	$orig_tz = date_default_timezone_get();
	$tz = sirius_do_not_disturb_tz();
	date_default_timezone_set($tz);
	$current_time = date("H:i");
	$day = date('N');
	$hour = date('G');
	$minute = (int) date('i');
	date_default_timezone_set($orig_tz);

	$enabled 			= $schedule['days'][$day]['enable'];
	$start_hour 	= $schedule['days'][$day]['start_hour'];
	$start_minute = $schedule['days'][$day]['start_minute'];
	$start_minute_display = str_pad($start_minute, 2, '0', STR_PAD_LEFT);
	$stop_hour 		= $schedule['days'][$day]['stop_hour'];
	$stop_minute 	= $schedule['days'][$day]['stop_minute'];
	$stop_minute_display = str_pad($stop_minute, 2, '0', STR_PAD_LEFT);

	if ($enabled != 'Yes') {
		return array('success' => FALSE, 'msg' => 'Notifications cannot be sent today.');
	}

	if ($start_hour > $hour || ($start_hour == $hour && $start_minute > $minute)) {
		return array('success' => FALSE, 'msg' => "Notifications cannot be sent until $start_hour:$start_minute_display. (Using timezone $tz where it is now $current_time)");
	}

	if ($hour > $stop_hour || ($hour == $stop_hour && $minute > $stop_minute)) {
		return array('success' => FALSE, 'msg' => "Notifications cannot be sent after $stop_hour:$stop_minute_display. (Using timezone $tz where it It is now $current_time)");
	}

	return array('success' => TRUE);
}

function sirius_do_not_disurb_render($id = 'default') {
	$json = sirius_do_not_disturb_json();

	if (!$json['schedules']) { return '<div class="sirius_do_not_disturb_render">No Do Not Disturb schedules have been configured.</div>'; }

	$found = FALSE;
	foreach ($json['schedules'] as $delta => $schedule) {
		if ($id == $schedule['id']) { $found = TRUE; break; }
	}
	if (!$found) { return '<div class="sirius_do_not_disturb_render">No Do Not Disturb schedules with id [' . $id . '] was not found.</div>'; }

  $days = array(
  	1 => 'Monday',
  	2 => 'Tuesday',
  	3 => 'Wednesday',
  	4 => 'Thursday',
  	5 => 'Friday',
  	6 => 'Saturday',
  	7 => 'Sunday',
  );

	$html = '<div class="sirius_do_not_disturb_render">';
	$html .= '<h2>';
	$html .= $schedule['title'];
	$html .= ' (' . $schedule['id'] . ')';
	$html .= '</h2>';
	$html .= sirius_tz_indicator(sirius_do_not_disturb_tz());
	$html .= '<table>';
	$html .= '<tr>';
	$html .= '<th>Day</th>';
	$html .= '<th>Can Send?</th>';
	$html .= '<th>Opens</th>';
	$html .= '<th>Closes</th>';
	$html .= '</tr>';
	foreach ($days as $day => $day_name) {
		$html .= '<tr>';
		$html .= '<td>' . $day_name . '</td>';
		$html .= '<td>' . $schedule['days'][$day]['enable'] . '</td>';
		$html .= '<td>' . $schedule['days'][$day]['start_hour'] . ':' . str_pad($schedule['days'][$day]['start_minute'], 2, '0', STR_PAD_LEFT);
		$html .= '<td>' . $schedule['days'][$day]['stop_hour'] . ':' . str_pad($schedule['days'][$day]['stop_minute'], 2, '0', STR_PAD_LEFT);
		$html .= '</tr>';
	}
	$html .= '</table>';
	if (drupal_valid_path('sirius/admin/do_not_disturb')) {
		$html .= l("Manage Do Not Disturb Settings", 'sirius/admin/do_not_disturb', array('attributes' => array('class' => array('tool-button'))));
	}
	$html .= '</div>';

	return $html;
}

function sirius_do_not_disturb_json() {
	$json = json_decode(sirius_domain_variable_get('sirius_do_not_disturb', ''), TRUE);
	if (!$json) { $json = sirius_do_not_disturb_default(); }
	return $json;
}

function sirius_do_not_disturb_default() {
	return json_decode('{"schedules":[{"title":"default","id":"default","days":{"1":{"enable":"Yes","start_hour":"9","start_minute":"0","stop_hour":"20","stop_minute":"0"},"2":{"enable":"Yes","start_hour":"9","start_minute":"0","stop_hour":"20","stop_minute":"0"},"3":{"enable":"Yes","start_hour":"9","start_minute":"0","stop_hour":"20","stop_minute":"0"},"4":{"enable":"Yes","start_hour":"9","start_minute":"0","stop_hour":"20","stop_minute":"0"},"5":{"enable":"Yes","start_hour":"9","start_minute":"0","stop_hour":"20","stop_minute":"0"},"6":{"enable":"Yes","start_hour":"9","start_minute":"0","stop_hour":"20","stop_minute":"0"},"7":{"enable":"Yes","start_hour":"9","start_minute":"0","stop_hour":"20","stop_minute":"0"}}},{"title":"Send Any Time (NO RESTRICTIONS)","id":"send_always","days":{"1":{"enable":"Yes","start_hour":"0","start_minute":"0","stop_hour":"24","stop_minute":"59"},"2":{"enable":"Yes","start_hour":"0","start_minute":"0","stop_hour":"24","stop_minute":"59"},"3":{"enable":"Yes","start_hour":"0","start_minute":"0","stop_hour":"24","stop_minute":"59"},"4":{"enable":"Yes","start_hour":"0","start_minute":"0","stop_hour":"24","stop_minute":"59"},"5":{"enable":"Yes","start_hour":"0","start_minute":"0","stop_hour":"24","stop_minute":"59"},"6":{"enable":"Yes","start_hour":"0","start_minute":"0","stop_hour":"24","stop_minute":"59"},"7":{"enable":"Yes","start_hour":"0","start_minute":"0","stop_hour":"24","stop_minute":"59"}}},{"title":"Send Never","id":"send_never","days":{"1":{"enable":"No","start_hour":"0","start_minute":"0","stop_hour":"0","stop_minute":"0"},"2":{"enable":"No","start_hour":"0","start_minute":"0","stop_hour":"0","stop_minute":"0"},"3":{"enable":"No","start_hour":"0","start_minute":"0","stop_hour":"0","stop_minute":"0"},"4":{"enable":"No","start_hour":"0","start_minute":"0","stop_hour":"0","stop_minute":"0"},"5":{"enable":"No","start_hour":"0","start_minute":"0","stop_hour":"0","stop_minute":"0"},"6":{"enable":"No","start_hour":"0","start_minute":"0","stop_hour":"0","stop_minute":"0"},"7":{"enable":"No","start_hour":"0","start_minute":"0","stop_hour":"0","stop_minute":"0"}}},{"title":"Business Hours","id":"business","days":{"1":{"enable":"Yes","start_hour":"9","start_minute":"0","stop_hour":"17","stop_minute":"0"},"2":{"enable":"Yes","start_hour":"9","start_minute":"0","stop_hour":"17","stop_minute":"0"},"3":{"enable":"Yes","start_hour":"9","start_minute":"0","stop_hour":"17","stop_minute":"0"},"4":{"enable":"Yes","start_hour":"9","start_minute":"0","stop_hour":"17","stop_minute":"0"},"5":{"enable":"Yes","start_hour":"9","start_minute":"0","stop_hour":"17","stop_minute":"0"},"6":{"enable":"No","start_hour":"0","start_minute":"0","stop_hour":"0","stop_minute":"0"},"7":{"enable":"Yes","start_hour":"0","start_minute":"0","stop_hour":"0","stop_minute":"0"}}}]}', TRUE);
}
