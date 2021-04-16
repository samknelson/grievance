<?php

/************************************************************************************************
 * Landing page
 ************************************************************************************************/

/**
 * You have access to the feed landing page if you have access to at least one feed.
 * @todo: This is pretty expensive. We should probably just create a permission of some kind.
 */

function sirius_feed_page_landing_access() {
	$all_info = sirius_wizard_info_all();
	foreach ($all_info as $key => $info) {
		if (!$info['feed']) { continue; }
		$path = "sirius/feeds/$key";
		if (drupal_valid_path($path)) { return TRUE; }
	}

	return FALSE;
}

function sirius_feed_page_landing() {
	$all_info = sirius_wizard_info_all();
	$html = '<h2>Available Feeds</h2>';
	$found = FALSE;

	$rows = array();

	foreach ($all_info as $key => $info) {
		if (!$info['feed']) { continue; }
		$path = "sirius/feeds/$key";
		if (drupal_valid_path($path)) {

			$paths = array(
				"sirius/feeds/$key" => t('List'),
				"sirius/feeds/launch/$key" => t('Create'),
			);

			$last_feed_node = sirius_feed_latest_entry($key);
			if ($last_feed_node) {
				$last_status = date('M j, Y', $last_feed_node->created);
				$last_status .= '<br />'; 
				$last_status .= $last_feed_node->field_sirius_feed_status['und'][0]['value'];
				$paths["node/" . $last_feed_node->nid . "/sirius_feed_wizard"] = t('Latest');
			} else {
				$last_status = NULL;
			}

			$tools = '';
			foreach ($paths as $path => $tool_name) {
				$tools .= l($tool_name, $path, array('attributes' => array('class' => array('tool-button'))));
			}

			$row = array();
			$row[] = $info['title'];
			$row[] = $last_status;
			$row[] = $info['description'];
			$row[] = $tools;
			$rows[] = $row;
		}
	}

	$header = array(
		t('Name'),
		t('Latest Feed'),
		t('Description'),
		t('Tools'),
	);

	return theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
}