<?php

/***
 * Implements hook_menu
 */

function grievance_timss_menu() {
  $items = array();

	$items['admin/config/grievance/timss/test'] = array(
		'title' => 'Grievance Module - TIMSS - Test',
		'description' => t('Test the TIMSS connection and search'),
		'page callback' => 'drupal_get_form',
		'page arguments' => array('grievance_timss_test_form'),
		'access arguments' => array('grievance admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['admin/config/grievance/timss'] = array(
		'title' => 'Grievance Module - TIMSS',
		'description' => t('Settings for the TIMSS integration'),
		'page callback' => 'drupal_get_form',
		'page arguments' => array('grievance_timss_admin'),
		'access arguments' => array('grievance admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['grievance/timss/lookup/%'] = array(
		'page callback' => 'grievance_timss_lookup_ajax',
		'page arguments' => array(3),
		'access arguments' => array('grievance user'),
		'type' => MENU_CALLBACK,
	);

  return $items;
}
