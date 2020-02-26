<?php

function sirius_page_config() {


//

	$links = array();

	$links['core'] = array('title' => 'Core');
	$links['core']['links']['sirius/taxonomy'] 									= array('title' => 'Drop-Down Lists', 'description' => 'View and edit all drop-down lists (aka "Taxonomies").');
	$links['core']['links']['sirius/admin/banner'] 							= array('title' => 'Site Banner', 'description' => 'Configure the banner to be shown at the top of all web pages.');
	$links['core']['links']['sirius/phonenumber'] 							= array('title' => 'Phone Number Cache', 'description' => 'Search and review the cache of phone numbers and deliverability metrics (i.e. SMS-enabled).');
	$links['core']['links']['sirius/field_overrides']						= array('title' => 'Field Overrides', 'description' => 'Change the names of fields.');
	$links['core']['links']['admin/config/grievance/quicksearch'] = array('title' => 'Quickseach', 'description' => 'Settings for the banner "quicksearch" tool.');

	$links['dispatch'] = array('title' => 'Dispatch');
	$links['dispatch']['links']['sirius/dispatch/config/run'] 	= array('title' => 'Running Job Settings', 'description' => 'Global settings for running jobs.');
	$links['dispatch']['links']['sirius/dispatch/threestrikes'] = array('title' => '"Three Strikes"', 'description' => 'Configure or test the "three strikes" rule, blocking users who decline dispatches on multiple successive days.');
	$links['dispatch']['links']['sirius/dispatch/config/view'] 	= array('title' => 'Dispatch Display', 'description' => 'Configure the header, footer, and style for the dispatch page.');

	$links['dispatch'] = array('title' => 'Bulk Messenger');
	$links['dispatch']['links']['sirius/config/bulk'] 					= array('title' => 'Bulk messenger settings', 'description' => 'Bulk messenger batch size, delay, etc.');

	$links['users'] = array('title' => 'Users');
	$links['users']['links']['admin/people/permissions'] = array('title' => 'Permissions', 'description' => 'Define which roles have which permissions.');
	$links['users']['links']['admin/people/permissions/roles'] = array('title' => 'Roles', 'description' => 'Define the system-wide user roles.');
	$links['users']['links']['admin/config/system/flood-control'] = array('title' => 'Flood Control', 'description' => 'Settings for blocking multiple failed login attempts from the same IP address.');
	$links['users']['links']['admin/config/system/flood-unblock'] = array('title' => 'Flood Unblock', 'description' => 'Clear the flood block for a legitimate user who is trying to sign in.');


	$links['systems'] = array('title' => 'Systems');
	$links['systems']['links']['sirius/config/specialrid'] 			= array('title' => 'Special Roles', 'description' => 'Certain roles have special meaning for the system; select them here.');
	$links['systems']['links']['sirius/config/specialtid'] 			= array('title' => 'Special Terms', 'description' => 'Certain taxonomy terms have special meaning for the system; select them here.');
	$links['systems']['links']['sirius/config/email'] 					= array('title' => 'Email', 'description' => 'Oubound email settings');
	$links['systems']['links']['sirius/config/sms'] 						= array('title' => 'SMS', 'description' => 'SMS (text message) settings');
	$links['systems']['links']['sirius/admin/twilio'] 					= array('title' => 'Twilio', 'description' => 'Settings for the Twilio phone service');
	$links['systems']['links']['admin/config'] 									= array('title' => 'Drupal Configuration', 'description' => 'Enter the system-wide Drupal configuration screens.');





	$html .= '<table>';

	$html .= '<tr>';
	$html .= '<th>' . t('Feature') . '</th>';
	$html .= '<th>' . t('Description') . '</th>';
	$html .= '<th>' . t('Tools') . '</th>';
	$html .= '</tr>';

	foreach ($links as $category_name => $category) {
		$html .= '<tr>';
		$html .= '<td colspan="3"><h2>' . $category['title'] . '</h2></td>';
		$html .= '</tr>';

		foreach ($category['links'] as $path => $link) {
			$html .= '<tr>';
			$html .= '<td>' . $link['title'] . '</td>';
			$html .= '<td>' . $link['description'] . '</td>';
			$html .= '<td>' . l('Manage', $path, array('attributes' => array('class' => array('tool-button')))) . '</td>';
			$html .= '</tr>';
		}
	}

	$html .= '</table>';
	return $html;
}