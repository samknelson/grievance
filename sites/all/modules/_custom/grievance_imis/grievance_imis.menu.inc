<?php

/***
 * Implements hook_menu
 */

function grievance_imis_menu() {
  $items = array();

	$items['admin/config/grievance/imis'] = array(
		'title' => 'Grievance Module - IMIS',
		'description' => t('Settings for the IMIS integration'),
		'page callback' => 'drupal_get_form',
		'page arguments' => array('grievance_imis_admin'),
		'access arguments' => array('grievance admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['admin/config/grievance/imis/settings'] = array(
		'title' => 'Settings',
		'description' => t('Settings for the IMIS integration'),
		'page callback' => 'drupal_get_form',
		'page arguments' => array('grievance_imis_admin'),
		'access arguments' => array('grievance admin'),
		'type' => MENU_DEFAULT_LOCAL_TASK,
	);

	$items['admin/config/grievance/imis/test'] = array(
		'title' => 'Test',
		'description' => t('Test the IMIS connection and search'),
		'page callback' => 'drupal_get_form',
		'page arguments' => array('grievance_imis_test_form'),
		'access arguments' => array('grievance admin'),
		'type' => MENU_LOCAL_TASK,
	);

	$items['grievance/imis/lookup/%'] = array(
		'page callback' => 'grievance_imis_lookup_ajax',
		'page arguments' => array(3),
		'access arguments' => array('grievance user'),
		'type' => MENU_CALLBACK,
	);

  return $items;
}
