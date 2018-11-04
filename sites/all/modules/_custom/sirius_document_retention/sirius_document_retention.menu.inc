<?php

/***
 * Implements hook_menu
 */

function sirius_document_retention_menu() {
  $items = array();

	$items['sirius/document-retention'] = array(
		'title' => t('Document Retenion Policies'),
		'page callback' => 'sirius_document_retention_page_landing',
		'access arguments' => array('sirius document retention admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['sirius/document-retention/configure'] = array(
		'title' => t('Configure Document Retenion Policies'),
		'page callback' => 'sirius_document_retention_page_configure',
		'access arguments' => array('sirius document retention admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['sirius/document-retention/configure/%'] = array(
		'title' => t('Configure Document Retenion Policies'),
		'page callback' => 'sirius_document_retention_page_configure_policy',
		'page arguments' => array(3),
		'access arguments' => array('sirius document retention admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['sirius/document-retention/execute/%'] = array(
		'title' => t('Execute Document Retenion Policies'),
		'page callback' => 'sirius_document_retention_page_execute',
		'page arguments' => array(3),
		'access arguments' => array('sirius document retention admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	return $items;
}

