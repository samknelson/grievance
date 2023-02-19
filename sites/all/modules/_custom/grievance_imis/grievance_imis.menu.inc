<?php

/***
 * Implements hook_menu
 */

function grievance_imis_menu() {
  $items = array();

	$items['user/%/grievance_imis_user_settings'] = array(
		'title' => 'SePuede',
		'page callback' => 'grievance_imis_user_settings',
		'page arguments' => array(1),
		'access callback' => 'grievance_imis_user_settings_access',
		'access arguments' => array(1),
		'type' => MENU_LOCAL_TASK,
	);

  return $items;
}

