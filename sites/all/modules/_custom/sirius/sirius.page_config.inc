<?php

function sirius_page_config() {
	$links = array();

	$links['domain'] = array('title' => 'Domains');
	$links['domain']['links']['sirius/domains'] 									= array('title' => 'Domains', 'description' => 'Create or manage domains ("sites") and subdomains.');
	$links['domain']['links']['sirius/domain/config/usermanage'] 	= array('title' => 'Domain User Manager', 'description' => 'Settings for the domain-specific user manager.');
	
	$links['core'] = array('title' => 'Core');
	$links['core']['links']['sirius/taxonomy'] 									= array('title' => 'Drop-Down Lists', 'description' => 'View and edit all drop-down lists (aka "Taxonomies").');
	$links['core']['links']['grievance/admin/letter-templates'] = array('title' => 'Letters', 'description' => 'Set up letter templates, such as a grievance notification or a graduation announcement for a worker..');
	$links['core']['links']['sirius/admin/banner'] 							= array('title' => 'Site Banner', 'description' => 'Configure the banner to be shown at the top of all web pages.');
	$links['core']['links']['sirius/field_overrides']						= array('title' => 'Field Overrides', 'description' => 'Change the names of fields.');
	$links['core']['links']['admin/config/grievance/quicksearch'] = array('title' => 'Quickseach', 'description' => 'Settings for the banner "quicksearch" tool.');
	$links['core']['links']['sirius/document-retention'] 				= array('title' => 'Document Retention', 'description' => 'Manage document retention policies.');
	$links['core']['links']['sirius/auditlog/config'] 					= array('title' => 'Audit Log', 'description' => 'Configure audit logging.');
	$links['core']['links']['grievance/admin/holidays'] 				= array('title' => 'Holidays', 'description' => 'Global holidays settings.');
	$links['core']['links']['sirius/admin/help'] 								= array('title' => 'Help Text', 'description' => 'Put explanatory text on any page on the site.');
	$links['core']['links']['grievance/admin/basic-pages'] 			= array('title' => 'Basic Pages', 'description' => 'Manage basic pages.');
	$links['core']['links']['sirius/admin/letterhead'] 					= array('title' => 'Letterhead', 'description' => 'Manage letterheads and watermarks for the site.');
	$links['core']['links']['sirius/contact/definition'] 				= array('title' => 'Contact Definition', 'description' => 'Set additional domain-specific fields for contacts.');
	$links['core']['links']['sirius/contact/resync/all']				= array('title' => 'Contact Resync', 'description' => 'A data integrity utility to re-connect all worker and user accounts with contacts.');

	$links['worker'] = array('title' => 'Worker');
	$links['worker']['links']['admin/config/grievance/workersync'] = array('title' => 'Worker Database', 'description' => 'Configure the connection between the grievance module and the worker database.');

	$links['employer'] = array('title' => 'Employer');
	$links['employer']['links']['sirius/employer/config/usermanage'] = array('title' => 'Employer User Manager', 'description' => 'Configure tools for allowing employers to manage their own users.');


	$links['grievance'] = array('title' => 'Grievance');
	$links['grievance']['links']['admin/config/grievance/core'] 	= array('title' => 'Grievance Settings', 'description' => 'Configure the grievance module.');
	$links['grievance']['links']['admin/config/grievance/workersync'] = array('title' => 'Worker Database', 'description' => 'Configure the connection between the grievance module and the worker database.');
	$links['grievance']['links']['grievance/admin/statuses/sequences'] = array('title' => 'Status Sequences', 'description' => 'Configure the sequences of statuses that constrain timelines.');
	$links['grievance']['links']['admin/config/grievance/intake-log'] = array('title' => 'Intake Log', 'description' => 'Allow grievance intake to request specific log dates, such as the date of a discipline or termiantion.');
	$links['grievance']['links']['grievances/irsets'] = array('title' => 'Information Request Sets', 'description' => 'Group multiple attachment types into an easily accessible "information request set".');

	$links['dispatch'] = array('title' => 'Dispatch');
	$links['dispatch']['links']['sirius/dispatch/config/run'] 	= array('title' => 'Running Job Settings', 'description' => 'Global settings for running jobs.');
	$links['dispatch']['links']['sirius/dispatch/threestrikes'] = array('title' => '"Three Strikes"', 'description' => 'Configure or test the "three strikes" rule, blocking users who decline dispatches on multiple successive days.');
	$links['dispatch']['links']['sirius/dispatch/config/view'] 	= array('title' => 'Dispatch Display', 'description' => 'Configure the header, footer, and style for the dispatch page.');
	$links['dispatch']['links']['sirius/dispatch/config/vlri'] 	= array('title' => 'VLRI', 'description' => 'VLRI - Voluntary layoff with a request to convert to an involuntary configuration.');

	$links['bulk'] = array('title' => 'Bulk Messenger');
	$links['bulk']['links']['sirius/config/bulk'] 					= array('title' => 'Bulk messenger settings', 'description' => 'Bulk messenger batch size, delay, etc.');

	$links['training'] = array('title' => 'Training Center');
	$links['training']['links']['sirius/config/training'] 					= array('title' => 'Training Center Settings', 'description' => 'Settings for the training center (evaluations, etc.).');

	$links['users'] = array('title' => 'Users');
	$links['users']['links']['admin/people/permissions'] = array('title' => 'Permissions', 'description' => 'Define which roles have which permissions.');
	$links['users']['links']['admin/people/permissions/roles'] = array('title' => 'Roles', 'description' => 'Define the system-wide user roles.');
	$links['users']['links']['admin/config/system/flood-control'] = array('title' => 'Flood Control', 'description' => 'Settings for blocking multiple failed login attempts from the same IP address.');
	$links['users']['links']['admin/config/system/flood-unblock'] = array('title' => 'Flood Unblock', 'description' => 'Clear the flood block for a legitimate user who is trying to sign in.');
	$links['users']['links']['admin/config/people/tfa-trusted-ip'] = array('title' => 'TFA Trusted IPs', 'description' => 'Set the list of trusted IP addresses for two-factor authentication.');

	$links['systems'] = array('title' => 'Systems');
	$links['systems']['links']['sirius/callerid'] 							= array('title' => 'Caller ID', 'description' => 'Create or manage Caller ID entries.');
	$links['systems']['links']['sirius/config/specialrid'] 			= array('title' => 'Special Roles', 'description' => 'Certain roles have special meaning for the system; select them here.');
	$links['systems']['links']['sirius/config/specialtid'] 			= array('title' => 'Special Terms', 'description' => 'Certain taxonomy terms have special meaning for the system; select them here.');
	$links['systems']['links']['sirius/config/email'] 					= array('title' => 'Email', 'description' => 'Oubound email settings');
	$links['systems']['links']['sirius/phonenumber'] 						= array('title' => 'Phone Number Cache', 'description' => 'Search and review the cache of phone numbers and deliverability metrics (i.e. SMS-enabled).');
	$links['systems']['links']['sirius/config/phone'] 					= array('title' => 'Phone', 'description' => 'Phone settings, including SMS and Twilio');
	$links['systems']['links']['sirius/sns/config'] 						= array('title' => 'SNS', 'description' => 'Settings for Amazon Simple Notification Service (SNS)');
	$links['systems']['links']['admin/config/grievance/phaxio'] = array('title' => 'Phaxio', 'description' => 'Configure the Phaxio fax service.');
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
			if (drupal_valid_path($path)) {
				$html .= '<tr>';
				$html .= '<td>' . $link['title'] . '</td>';
				$html .= '<td>' . $link['description'] . '</td>';
				$html .= '<td>' . l('Manage', $path, array('attributes' => array('class' => array('tool-button')))) . '</td>';
				$html .= '</tr>';
			}
		}
	}

	$html .= '</table>';
	return $html;
}