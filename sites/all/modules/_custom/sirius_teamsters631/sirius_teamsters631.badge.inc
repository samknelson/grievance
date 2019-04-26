<?php

function sirius_teamsters631_badge($worker_node) {
	return drupal_get_form('sirius_teamsters631_badge_form', $worker_node);
}

function sirius_teamsters631_badge_form($form, &$form_state, $worker_node) {

	$form_state['worker_node'] = $worker_node;

	$form['name'] = array(
		'#type' => 'textfield',
		'#title' => t('Name'),
		'#default_value' => $worker_node->title,
	);

	/*
	If a worker is a journeyman (MS) & active member (WS), then the background color should be solid blue.
	If a worker is a journeyman (MS) & hiring hall fee (WS), then the background is yellow
	If a worker is an apprentice (MS) & active member (WS), then the background is red on top/blue on bottom
	If a worker is an apprentice (MS) & non-member, then the background is full red.  (this combination should never happen any more, but there is logic for it if it does).
	If a worker is a skilled extraboard, the background is orange
	If a worker is an unskilled extraboard, the background is brown
	If a worker is specialty craft (a new MS) & an active member (WS), then their background is green on top and blue on bottom.
	If a worker is specialty craft (a new MS) & a non member (WS), then their background is green on top and yellow on bottom.
	*/

	$colors = array(
		'blue' => 'Blue',
		'yellow' => 'Yellow',
		'red' => 'Red',
		'green' => 'Green',
		'orange' => 'Orange',
		'brown' => 'brown'
	);

	$table = array();

	$table[0] = array('bt' => 'white', 'bb' => 'white', 'msg' => 'No Match');
	$table[1] = array('bt' => 'blue', 'bb' => 'blue', 'msg' => 'Journeyman / Active Member');
	$table[2] = array('bt' => 'yellow', 'bb' => 'yellow', 'msg' => 'Journeyman / Hiring Hall Fee');
	$table[3] = array('bt' => 'red', 'bb' => 'blue', 'msg' => 'Apprentice / Active Member');
	$table[4] = array('bt' => 'red', 'bb' => 'red', 'msg' => 'Apprentice / Non-Member');
	$table[5] = array('bt' => 'orange', 'bb' => 'orange', 'msg' => 'Skilled Extraboard');
	$table[6] = array('bt' => 'brown', 'bb' => 'brown', 'msg' => 'Unskilled Extraboard');
	$table[7] = array('bt' => 'green', 'bb' => 'blue', 'msg' => 'Specialty Craft / Active Member');
	$table[8] = array('bt' => 'green', 'bb' => 'yellow', 'msg' => 'Specialty Craft / Non-Member');

	$member_status_tids 		= sirius_fieldvals($worker_node, 'field_sirius_member_status', 'tid');
	$is_ms_journeyman 			= in_array(271, $member_status_tids);
	$is_ms_apprentice 			= in_array(268, $member_status_tids);
	$is_ms_seb 							= in_array(270, $member_status_tids);
	$is_ms_ueb 							= in_array(269, $member_status_tids);
	$is_ms_specialty 				= in_array(673, $member_status_tids); 

	$work_status_tid = sirius_fieldval($worker_node, 'field_sirius_work_status', 'tid');
	$work_status_term = taxonomy_term_load($work_status_tid);
	$is_ws_hhf 							= ($work_status_tid == 303);
	$is_ws_active						= ($work_status_term->field_sirius_member_active['und'][0]['value'] == 'Yes');

	$which = 0;
	if ($is_ms_journeyman && $is_ws_active) 					{ $which = 1; }
	else if ($is_ms_journeyman && $is_ws_hhf) 				{ $which = 2; }
	else if ($is_ms_apprentice && $is_ws_active) 			{ $which = 3; }
	else if ($is_ms_apprentice) 											{ $which = 4; }
	else if ($is_ms_specialty) 												{ $which = 5; }
	else if ($is_ms_ueb) 															{ $which = 6; }
	else if ($is_ms_specialty && $is_ws_active) 			{ $which = 7; }
	else if ($is_ms_specialty) 												{ $which = 8; }

	$rows = array();
	foreach ($table as $rownum => $hr) {
		$row = array();
		if ($which == $rownum) {
			$row[] = '***'; 
		} else {
			$row[] = '';
		}
		$row[] = $hr['msg'];
		$row[] = "<span style=\"display: inline-block; width: 50px; border: 2px solid #000000; background-color: $hr[bt];\">&nbsp;</span>&nbsp;" . $colors[$hr['bt']];
		$row[] = "<span style=\"display: inline-block; width: 50px; border: 2px solid #000000; background-color: $hr[bb];\">&nbsp;</span>&nbsp;" . $colors[$hr['bb']];
		$rows[] = $row;
	}
	
	$header = array(
		'&nbsp;', 
		'Status',
		'Top Color',
		'Bottom Color',
	);

	$default_front = '';

	$skill_expire = sirius_fieldvals($worker_node, 'field_sirius_skill_expire', 'value');
	$skill_tids = sirius_fieldvals($worker_node, 'field_sirius_skill', 'tid');
	$skill_terms = taxonomy_term_load_multiple($skill_tids);
	$default_skill_indicators = '';
	$default_back = '';
	if ($skill_terms) {
		$delta = 0;
		foreach ($skill_terms as $skill_tid => $skill_term) {
			if ($skill_term->field_sirius_name_short['und'][0]['value']) {
				$default_skill_indicators .= $skill_term->field_sirius_name_short['und'][0]['value'] . ' ';
			}
			if ($skill_term->field_sirius_badge['und'][0]['value'] == 'Yes') {
				if ($default_back) { $default_back .= "\n"; }
				$default_back .= $skill_term->name;
				$expire_ts = strtotime($skill_expire[$delta]);
				if ($expire_ts > time() && $expire_ts < time() + 60*60*24*365*25) {
					$default_back .= " - " . date('Y-m-d', $expire_ts);
				}
			}
			++$delta;
		}
	}


	if ($is_ms_journeyman) { $default_front = "Convention\nJourneyman"; }
	else if ($is_ms_apprentice) { $default_front = "Apprentice"; }
	else if ($is_ms_seb) { $default_front = "Skilled Extra\nBoard"; }
	else if ($is_ms_ueb) { $default_front = "Unskilled Extra\nBoard"; }
	else if ($is_ms_specialty) { $default_front = "Specialty Craft"; }

	$form['front'] = array(
		'#type' => 'textarea',
		'#title' => t('Front'),
		'#default_value' => $default_front,
	);

	$form['skill_indicators'] = array(
		'#type' => 'textfield',
		'#title' => t('Skill Indicators'),
		'#default_value' => $default_skill_indicators,
	);

	$form['back'] = array(
		'#type' => 'textarea',
		'#title' => t('Back'),
		'#default_value' => $default_back,
	);

	$form['bt'] = array(
		'#type' => 'select',
		'#options' => $colors,
		'#title' => t('Top Color'),
		'#default_value' => $table[$which]['bt'],
	);
	$form['bb'] = array(
		'#type' => 'select',
		'#options' => $colors,
		'#title' => t('Bottom Color'),
		'#default_value' => $table[$which]['bb'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Create Badge',
	);

	$form['#attributes'] = array('target' => '_blank');

	$colortable_markup = '<h2>Color Table</h2>' . theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
	$form['colortable'] = array('#markup' => $colortable_markup);

	return $form;
}

function sirius_teamsters631_badge_form_submit($form, &$form_state) {
	$worker_node = $form_state['worker_node'];
	$worker_nid = $worker_node->nid;

	global $is_https;
	if ($is_https ||
		(isset($_SERVER['HTTP_X_FORWARDED_PROTO']) && $_SERVER['HTTP_X_FORWARDED_PROTO'] == 'https')) {
		$https = 'https://';
	} else { 
		$url = 'http://';
	}

	$url = $https . $_SERVER['SERVER_NAME'] . "/sirius/teamsters631/badge/$worker_nid/contents";
	$url .= '?front=' . urlencode($form_state['values']['front']);
	$url .= '&back=' . urlencode($form_state['values']['back']);
	$url .= '&bt=' . urlencode($form_state['values']['bt']);
	$url .= '&bb=' . urlencode($form_state['values']['bb']);
	$url .= '&name=' . urlencode($form_state['values']['name']);
	$url .= '&skill_indicators=' . urlencode($form_state['values']['skill_indicators']);
	$html = file_get_contents($url);
	preg_match('/___NOPREFIX___(.+)___NOSUFFIX___/s', $html, $matches);
	$html = $matches[1];

	$html_path = DRUPAL_ROOT . "/sites/default/files/private/badge/$worker_nid.html";
	file_put_contents($html_path, $html);

	$pdf_path = DRUPAL_ROOT . "/sites/default/files/private/badge/$worker_nid.pdf";

	$cmd = '/usr/bin/xvfb-run -a ';
	$cmd .= '--server-args="-screen 0 1024x768x24" ';
	$cmd .= '/usr/bin/wkhtmltopdf ';
	$cmd .= '--print-media-type ';
	$cmd .= '-q ';
	$cmd .= '-L 0mm -R 0mm -T 0mm -B 0mm '; 
	$cmd .= "--page-height 85.6mm ";
	$cmd .= "--page-width 54mm ";
	$cmd .= $html_path;
	$cmd .= ' ';
	$cmd .= $pdf_path;
	exec($cmd);

	header('Content-Type: application/pdf');
	print file_get_contents($pdf_path);
	drupal_exit();
}

function sirius_teamsters631_badge_contents_access() {
	if ($_SERVER['SERVER_ADDR'] == $_SERVER['REMOTE_ADDR'] ||
		  $_SERVER['SERVER_ADDR'] == $_SERVER['HTTP_X_FORWARDED_FOR'] ||
		  $_SERVER['REMOTE_ADDR'] == '18.224.235.106' || 
		  $_SERVER['REMOTE_ADDR'] == '127.0.0.1' || 
		  $_SERVER['SERVER_ADDR'] == variable_get('grievance_loopback_ip', '')) { 
		return TRUE; 
	}

	if (user_access('sirius admin')) { return TRUE; }

	return FALSE;
}

function sirius_teamsters631_badge_headshot($worker_node) {
	// return '<pre>' . print_r($worker_node->field_sirius_headshot['und'][0], 1) . '</pre>';
	$fid = $worker_node->field_sirius_headshot['und'][0]['fid'];
	$uri = $worker_node->field_sirius_headshot['und'][0]['uri'];

	$source = (object) $worker_node->field_sirius_headshot['und'][0];
	$dest = 'temporary://sirius_headshot_for_streaming/' . $source->filename;
	
	file_copy($source, $dest, FILE_EXISTS_REPLACE);
	header("Content-Type: " . $source->filemime);
	print file_get_contents(drupal_realpath($dest));
	exit();
}

function sirius_teamsters631_badge_contents($worker_node) {
	$worker_nid = $worker_node->nid;

	global $is_https;
	if ($is_https ||
		(isset($_SERVER['HTTP_X_FORWARDED_PROTO']) && $_SERVER['HTTP_X_FORWARDED_PROTO'] == 'https')) {
		$https = 'https://';
	} else { 
		$url = 'http://';
	}

	print "___NOPREFIX___";

	// Headshot
	if ($worker_node->field_sirius_headshot['und'][0]['fid']) {
		$url = $https . $_SERVER['SERVER_NAME'] . "/sirius/teamsters631/badge/$worker_nid/headshot";
	} else { 
		$url = 'https://via.placeholder.com/250x220px.png?text=' . urlencode("Image missing");
	}
	$bt = $_REQUEST['bt'];
	$bb = $_REQUEST['bb'];
	$name = $_REQUEST['name'];
	$skill_indicator_str = $_REQUEST['skill_indicators'];
	$tmp = explode(' ', $skill_indicator_str);
	$skill_indicators = array();
	foreach ($tmp as $str) {
		$str = trim($str);
		if ($str) { $skill_indicators[] = $str; }
	}

	print "<div style=\"width: 250px; height: 230px; overflow: hidden; padding: 0; background: linear-gradient(to bottom, $bt 0%, $bt 50%, $bb 50%, $bb 100%);\">";
	print "<div style=\"width: 210px; height: 200px; overflow: hidden; padding: 0; display: block; margin-left: 20px; margin-right: 20px; margin-top: 15px; margin-bottom: 15px;\">";
	print "<img src=\"$url\" alt=\"" . htmlspecialchars($name) . "\" height=\"200\" style=\"height: 210px; display: block; margin-left: auto; margin-right: auto;\">";
	print "</div>";
	print "</div>";

	// Worker name
	print '<div style="font-weight: bold; text-align: center; font-size: 100%; height: 20px; overflow: hidden;">';
	print $name;
	print '</div>';

	// Bottom wrapper
	print '<div style="padding-top: 5px; text-align: right;">';

	// Logo
	print '<img src="https://dispatch.teamsters631.com/sites/default/files/sirius/banner/logo/teamsters-union-631_3.png" style="float: left;" height="120">';

	// Front text
	print '<div style="height: 110px; overflow: hidden; white-space: pre;">';
	print $_REQUEST['front'];
	print '</div>';

	// Worker ID
	print '<div style="">';
	print "ID #" . '<strong>' . $worker_node->field_sirius_id['und'][0]['value'] . '</strong>';
	print '</div>';

	// Close bottom wrapper
	print '</div>';

	// Skill Indicators
	print '<div style="height: 20px; overflow: hidden;">';
	foreach ($skill_indicators as $skill) {
		print '<span style="color: white; background-color: darkblue; padding-left: 2px; padding-right: 2px;">' . $skill . '</span> ';
	}
	print '</div>';

	// Page break
	print '<div style="page-break-before: always;"></div>';

	// Property of
	print '<div style="background-color: #000000; color: #ffffff; font-weight: bold; text-align: center; padding-top: 5px; padding-bottom: 5px; margin-bottom: 10px; padding-left: 0; padding-right: 0; margin-left: 0; margin-right: 0;">';
	print "Property of Teamsters Local Union 631";
	print '</div>';

	// Contact
	print '<div style="text-align: center; background-image: url(\'https://test.dispatch.teamsters631.com/sites/all/modules/_custom/sirius_teamsters631/images/logo_bg.jpg\');  background-repeat: no-repeat;  background-position: top center; background-size: auto 100%; ">';

	// Back text
	print '<div style="height: 210px; overflow: hidden; white-space: pre; font-size: 75%">';
	print $_REQUEST['back'];
	print '</div>';

	print '<div style="margin-bottom: 5px;">';
	print "Dispatch: " . sirius_phone_format(variable_get('sirius_twilio_operator', ''));
	print "<br />";
	print "www.TeamstersLocal631.org";
	print '</div>';

	print '</div>';

	// QR
	print '<div style="text-align: center; margin-top: 0; margin-bottom: 10px;">';
	$url = $https . $_SERVER['SERVER_NAME'] . "/node/" . $worker_node->nid;
	print sirius_qr_url($url);
	print '</div>';

	print "___NOSUFFIX___";
	exit();
}
