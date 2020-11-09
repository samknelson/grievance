<?php

function sirius_page_config() {
	$links = array();

	$links['domain'] = array('title' => 'Domains');
	$links['domain']['links']['sirius/domains'] 									= array('title' => 'Manage Domains', 'description' => 'Create or manage domains ("sites") and subdomains.');
	$links['domain']['links']['sirius/domain/config'] 						= array('title' => 'Domain Settings', 'description' => 'Global domain settings -- user manager, stats configuration, etc.');
	
	$links['core'] = array('title' => 'Core');
	$links['core']['links']['sirius/taxonomy'] 										= array('title' => 'Drop-Down Lists', 'description' => 'View and edit all drop-down lists (aka "Taxonomies").');
	$links['core']['links']['sirius/admin/do_not_disturb'] 				= array('title' => 'Do Not Disturb', 'description' => 'Set do-not-disturb blackout times, e.g. "do not call after 9:00 PM".');
	$links['core']['links']['sirius/admin/banner'] 								= array('title' => 'Site Banner', 'description' => 'Configure the banner and footer to be shown on all web pages.');
	$links['core']['links']['sirius/field_overrides']							= array('title' => 'Field Overrides', 'description' => 'Change the names of fields.');
	$links['core']['links']['admin/config/grievance/quicksearch'] = array('title' => 'Quickseach', 'description' => 'Settings for the banner "quicksearch" tool.');
	$links['core']['links']['sirius/document-retention'] 				= array('title' => 'Document Retention', 'description' => 'Manage document retention policies.');
	$links['core']['links']['sirius/auditlog/config'] 					= array('title' => 'Audit Log', 'description' => 'Configure audit logging.');
	$links['core']['links']['grievance/admin/holidays'] 				= array('title' => 'Holidays', 'description' => 'Global holidays settings.');
	$links['core']['links']['sirius/admin/help'] 								= array('title' => 'Help Text', 'description' => 'Put explanatory text on any page on the site.');
	$links['core']['links']['grievance/admin/basic-pages'] 			= array('title' => 'Basic Pages', 'description' => 'Manage basic pages.');
	$links['core']['links']['sirius/admin/letterhead'] 					= array('title' => 'Letterhead', 'description' => 'Manage letterheads and watermarks for the site.');
	$links['core']['links']['sirius/contact/definition'] 				= array('title' => 'Contact Definition', 'description' => 'Set additional domain-specific fields for contacts.');
	$links['core']['links']['sirius/contact/resync/all']				= array('title' => 'Contact Resync', 'description' => 'A data integrity utility to re-connect all worker and user accounts with contacts.');
	$links['core']['links']['sirius/config/letter']							= array('title' => 'Letters', 'description' => 'Configure the Letters system and manage letter templates.');
	$links['core']['links']['sirius/config/feed']							= array('title' => 'Letters', 'description' => 'Configure the feeds system.');

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
	$links['dispatch']['links']['sirius/dispatch/job/stub'] 		= array('title' => 'Stub Jobs', 'description' => 'Create or manage "stub" jobs, for generic dispatches or for importing legacy data.');

	$links['bulk'] = array('title' => 'Bulk Messenger');
	$links['bulk']['links']['sirius/bulk/config'] 					= array('title' => 'Bulk messenger settings', 'description' => 'Bulk messenger batch size, delay, etc.');

	$links['hour'] = array('title' => 'Hours');
	$links['hour']['links']['sirius/hour/config'] 					= array('title' => 'Hour settings', 'description' => 'General settings for hour tracking.');

	$links['training'] = array('title' => 'Training Center');
	$links['training']['links']['sirius/config/training'] 					= array('title' => 'Training Center Settings', 'description' => 'Settings for the training center (evaluations, etc.).');

	$links['ledger'] = array('title' => 'A/R Accounting and Ledger');
	$links['ledger']['links']['sirius/config/ledger'] 					= array('title' => 'A/R Settings', 'description' => 'Settings for the A/R accounting system: ledger, dues, payments, charges, etc.');

	$links['users'] = array('title' => 'Users');
	$links['users']['links']['admin/people'] = array('title' => 'Users', 'description' => 'Sitewide user management');
	$links['users']['links']['admin/config/system/flood-control'] = array('title' => 'Flood Control', 'description' => 'Settings for blocking multiple failed login attempts from the same IP address.');
	$links['users']['links']['admin/config/system/flood-unblock'] = array('title' => 'Flood Unblock', 'description' => 'Clear the flood block for a legitimate user who is trying to sign in.');
	$links['users']['links']['admin/config/people/tfa-trusted-ip'] = array('title' => 'TFA Trusted IPs', 'description' => 'Set the list of trusted IP addresses for two-factor authentication.');

	$links['systems'] = array('title' => 'Systems');
	$links['systems']['links']['sirius/config/server-paths'] 					= array('title' => 'Server Paths', 'description' => 'Locations of services on the server: wkhtmltopdf, pdftk, etc.');
	$links['systems']['links']['sirius/config/phone/callerid'] 				= array('title' => 'Caller ID', 'description' => 'Create or manage Caller ID entries.');
	$links['systems']['links']['sirius/twilio/network-test'] 					= array('title' => 'Twilio Network Test', 'description' => 'Use the Twilio network test tool to verify if the client computer has sufficient connectivity to run a VoIP call.');
	$links['systems']['links']['sirius/admin/caller-id-full-list'] 	= array('title' => 'Cross-Domain Caller ID List', 'description' => 'View a list of all caller IDs across all domains.');
	$links['systems']['links']['sirius/config/specialrid'] 			= array('title' => 'Special Roles', 'description' => 'Certain roles have special meaning for the system; select them here.');
	$links['systems']['links']['sirius/config/specialtid'] 			= array('title' => 'Special Terms', 'description' => 'Certain taxonomy terms have special meaning for the system; select them here.');
	$links['systems']['links']['sirius/config/email'] 					= array('title' => 'Email', 'description' => 'Oubound email settings');
	$links['systems']['links']['sirius/phonenumber'] 						= array('title' => 'Phone Number Cache', 'description' => 'Search and review the cache of phone numbers and deliverability metrics (i.e. SMS-enabled).');
	$links['systems']['links']['sirius/config/phone'] 					= array('title' => 'Phone', 'description' => 'Phone settings, including SMS and Twilio');
	$links['systems']['links']['sirius/sns/config'] 						= array('title' => 'SNS', 'description' => 'Settings for Amazon Simple Notification Service (SNS)');
	$links['systems']['links']['admin/config/grievance/phaxio'] = array('title' => 'Phaxio', 'description' => 'Configure the Phaxio fax service.');
	$links['systems']['links']['admin/config/services/shorten'] = array('title' => 'Shorten', 'description' => 'Configure the link shortening services such as blt.ly.');
	$links['systems']['links']['admin/config'] 									= array('title' => 'Drupal Configuration', 'description' => 'Enter the system-wide Drupal configuration screens.');
	$html .= '<table>';

	$html .= '<tr>';
	$html .= '<th>' . t('Feature') . '</th>';
	$html .= '<th>' . t('Description') . '</th>';
	$html .= '<th colspan="2">' . t('Tools') . '</th>';
	$html .= '</tr>';

	foreach ($links as $category_name => $category) {
		$html .= '<tr>';
		$html .= '<td colspan="4"><h2>' . $category['title'] . '</h2></td>';
		$html .= '</tr>';

		foreach ($category['links'] as $path => $link) {
			if (drupal_valid_path($path)) {
				// Get the local tasks for this item. 
				// Copied from menu_local_tasks(), which really ought to not require a raw db query.
				$sql = "select path, title from menu_router ";
				$sql .= "where tab_root = :path ";
				$sql .= "and type = " . MENU_LOCAL_TASK . ' ';
				$sql .= "order by weight, title ";
				$stmt = sirius_sql_query($sql, array(':path' => $path));
				$sub_paths = array();
				while ($hr = $stmt->fetchAssoc()) { 
					if (drupal_valid_path($hr['path'])) {
						$sub_paths[$hr['path']] = $hr['title'];
					}
				}

				$html .= '<tr>';
				$html .= '<td>' . $link['title'] . '</td>';
				$html .= '<td>' . $link['description'] . '</td>';
				$html .= '<td>' . l('Manage', $path, array('attributes' => array('class' => array('tool-button')))) . '</td>';
				$html .= '<td>';
				foreach ($sub_paths as $sub_path => $sub_title) {
					$html .= l($sub_title, $sub_path, array('attributes' => array('class' => array('tool-button'))));
				}
				$html .= '</td>';
				$html .= '</tr>';

			}
		}
	}

	$html .= '</table>';
	return $html;
}