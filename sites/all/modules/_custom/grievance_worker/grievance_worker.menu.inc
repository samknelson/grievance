<?php

/***
 * Implements hook_menu
 */

function grievance_worker_menu() {
  $items = array();

	$items['admin/config/grievance/worker/test'] = array(
		'title' => 'Grievance Module - Worker - Test',
		'description' => t('Test the Worker connection and search'),
		'page callback' => 'drupal_get_form',
		'page arguments' => array('grievance_worker_test_form'),
		'access arguments' => array('grievance admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['admin/config/grievance/worker'] = array(
		'title' => 'Grievance Module - Worker',
		'description' => t('Settings for the worker integration'),
		'page callback' => 'drupal_get_form',
		'page arguments' => array('grievance_worker_admin'),
		'access arguments' => array('grievance admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['grievance/worker/lookup/%'] = array(
		'page callback' => 'grievance_worker_lookup_ajax',
		'page arguments' => array(3),
		'access arguments' => array('grievance user'),
		'type' => MENU_CALLBACK,
	);

  return $items;
}
