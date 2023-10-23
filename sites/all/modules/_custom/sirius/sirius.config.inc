<?php

function sirius_config_links() {
 	$links = &drupal_static(__FUNCTION__);

 	if (!isset($links)) {
		$links['domain'] = array('title' => 'Domains');
		$links['domain']['links']['sirius/domains'] 									= array('title' => 'Manage Domains', 'description' => 'Create or manage domains ("sites") and subdomains.');
		$links['domain']['links']['sirius/domain/config'] 						= array('title' => 'Domain Settings', 'description' => 'Global domain settings -- user manager, stats configuration, etc.');
		
		$links['core'] = array('title' => 'Core', 'weight' => -1);
		$links['core']['links']['sirius/taxonomy'] 										= array('title' => 'Drop-Down Lists', 'description' => 'View and edit all drop-down lists (aka "Taxonomies").');
		$links['core']['links']['sirius/admin/do_not_disturb'] 				= array('title' => 'Do Not Disturb', 'description' => 'Set do-not-disturb blackout times, e.g. "do not call after 9:00 PM".');
		$links['core']['links']['sirius/admin/banner'] 								= array('title' => 'Site Banner', 'description' => 'Configure the banner and footer to be shown on all web pages.');
		$links['core']['links']['admin/config/grievances/fieldoverrides']							= array('title' => 'Field Overrides', 'description' => 'Change the names of fields.');
		$links['core']['links']['admin/config/grievance/quicksearch'] = array('title' => 'Quickseach', 'description' => 'Settings for the banner "quicksearch" tool.');
		$links['core']['links']['sirius/document-retention'] 				= array('title' => 'Document Retention', 'description' => 'Manage document retention policies.');
		$links['core']['links']['sirius/auditlog/config'] 					= array('title' => 'Audit Log', 'description' => 'Configure audit logging.');
		$links['core']['links']['grievance/admin/holidays'] 				= array('title' => 'Holidays', 'description' => 'Global holidays settings.');
		$links['core']['links']['grievance/admin/basic-pages'] 			= array('title' => 'Basic Pages', 'description' => 'Manage basic pages.');
		$links['core']['links']['sirius/pdf/settings'] 							= array('title' => 'PDF Settings', 'description' => 'Global settings for PDF generation for the site.');
		$links['core']['links']['sirius/admin/letterhead'] 					= array('title' => 'Letterhead', 'description' => 'Manage letterheads and watermarks for the site.');
		$links['core']['links']['sirius/contact/definition'] 				= array('title' => 'Contact Definition', 'description' => 'Set additional domain-specific fields for contacts.');
		$links['core']['links']['sirius/contact/resync/all']				= array('title' => 'Contact Resync', 'description' => 'A data integrity utility to re-connect all worker and user accounts with contacts.');
		$links['core']['links']['sirius/config/letter']							= array('title' => 'Letters', 'description' => 'Configure the Letters system and manage letter templates.');
		$links['core']['links']['sirius/disclaimer']						= array('title' => 'Disclaimers', 'description' => 'Configure dislcaimers -- text which can be attached to events or event types, and which must be accepted by the user.');
		$links['core']['links']['sirius/config/feed']								= array('title' => 'Feeds', 'description' => 'Configure the feeds system.');
		$links['core']['links']['sirius/mailattach']								= array('title' => 'Mailattach', 'description' => 'Configure the attach-by-email system.');
		$links['core']['links']['sirius/customfield/definition/list']								= array('title' => 'Custom Field Sets', 'description' => 'Define custom field sets which can be attached to events or other entity types.');
		$links['core']['links']['sirius/dashboard/items_by_role']								= array('title' => 'Dashboard', 'description' => 'Configure dashboard items.');
		$links['core']['links']['sirius/log/quicklog/config']								= array('title' => 'Quicklog', 'description' => 'Settings for the Quicklog tools.');
		$links['core']['links']['sirius/phonetree/config'] 			= array('title' => 'Phonetree', 'description' => 'Phonetree settings for when a user calls in to the system.');
		$links['core']['links']['sirius/lang/config'] 					= array('title' => 'Language Configuration', 'description' => 'Configure available language list for the domain.');

		$links['worker'] = array('title' => 'Worker');
		$links['worker']['links']['sirius/worker/config/user'] = array('title' => 'Worker / User Sync', 'description' => 'Configure how users are created from workers.');
		$links['worker']['links']['admin/config/grievance/workersync'] = array('title' => 'Worker Database', 'description' => 'Configure the connection between the grievance module and the worker database.');
		$links['worker']['links']['sirius/skill/settings'] = array('title' => 'Skill Settings', 'description' => 'Configure settings for skill management and display.');

		$links['employer'] = array('title' => 'Employer');
		$links['employer']['links']['sirius/employer/config/usermanage'] = array('title' => 'Employer User Manager', 'description' => 'Configure tools for allowing employers to manage their own users.');

		$links['grievance'] = array('title' => 'Grievance');
		$links['grievance']['links']['admin/config/grievance/core'] 	= array('title' => 'Grievance Settings', 'description' => 'Configure the grievance module.');
		$links['grievance']['links']['grievance/admin/statuses/sequences'] = array('title' => 'Status Sequences', 'description' => 'Configure the sequences of statuses that constrain timelines.');
		$links['grievance']['links']['admin/config/grievance/intake-log'] = array('title' => 'Intake Log', 'description' => 'Allow grievance intake to request specific log dates, such as the date of a discipline or termiantion.');
		$links['grievance']['links']['grievances/irsets'] = array('title' => 'Information Request Sets', 'description' => 'Group multiple attachment types into an easily accessible "information request set".');

		$links['dispatch'] = array('title' => 'Dispatch');
		$links['dispatch']['links']['sirius/dispatch/config/run'] 	= array('title' => 'Running Job Settings', 'description' => 'Global settings for running jobs.');
		$links['dispatch']['links']['sirius/dispatch/threestrikes'] = array('title' => '"Three Strikes"', 'description' => 'Configure or test the "three strikes" rule, blocking users who decline dispatches on multiple successive days.');
		$links['dispatch']['links']['sirius/dispatch/config/view'] 	= array('title' => 'Dispatch Display', 'description' => 'Configure the header, footer, and style for the dispatch page.');
		$links['dispatch']['links']['sirius/dispatch/config/vlri'] 	= array('title' => 'VLRI', 'description' => 'VLRI - Voluntary layoff with a request to convert to an involuntary configuration.');
		$links['dispatch']['links']['sirius/dispatch/job/stub'] 		= array('title' => 'Stub Jobs', 'description' => 'Create or manage "stub" jobs, for generic dispatches or for importing legacy data.');

		$links['event'] = array('title' => 'Events');
		$links['event']['links']['sirius/event/interestlist/settings'] 	= array('title' => 'Interest List Settings', 'description' => 'Configure settings for "interest lists" for event types.');
		$links['event']['links']['sirius/event/catalog/list'] 	= array('title' => 'Catalogs', 'description' => 'Configure course catalogs.');

		$links['bulk'] = array('title' => 'Bulk Messenger');
		$links['bulk']['links']['sirius/bulk/config'] 					= array('title' => 'Bulk messenger settings', 'description' => 'Bulk messenger batch size, delay, etc.');

		$links['hour'] = array('title' => 'Hours');
		$links['hour']['links']['sirius/hour/config'] 					= array('title' => 'Hour settings', 'description' => 'General settings for hour tracking.');

		$links['training'] = array('title' => 'Training Center');
		$links['training']['links']['sirius/config/training'] 					= array('title' => 'Training Center Settings', 'description' => 'Settings for the training center (evaluations, etc.).');

		$links['users'] = array('title' => 'Users');
		$links['users']['links']['admin/people'] = array('title' => 'Users', 'description' => 'Sitewide user management');
		$links['users']['links']['admin/config/system/flood-control'] = array('title' => 'Flood Control', 'description' => 'Settings for blocking multiple failed login attempts from the same IP address.');
		$links['users']['links']['admin/config/system/flood-control']['sub_paths']['sirius/flood'] = array('title' => 'Flood Unblock');
		$links['users']['links']['admin/config/people/tfa-trusted-ip'] = array('title' => 'TFA Trusted IPs', 'description' => 'Set the list of trusted IP addresses for two-factor authentication.');

		$links['systems'] = array('title' => 'Systems');
		$links['systems']['links']['sirius/dev/config'] 					= array('title' => 'Development Settings', 'description' => 'Development settings, e.g. staging site, production site, etc.');
		$links['systems']['links']['sirius/config/server-paths'] 					= array('title' => 'Server Paths', 'description' => 'Locations of services on the server: wkhtmltopdf, pdftk, etc.');
		$links['systems']['links']['sirius/admin/caller-id-full-list'] 	= array('title' => 'Cross-Domain Caller ID List', 'description' => 'View a list of all caller IDs across all domains.');
		$links['systems']['links']['sirius/config/specialrid'] 			= array('title' => 'Special Roles', 'description' => 'Certain roles have special meaning for the system; select them here.');
		$links['systems']['links']['sirius/config/specialtid'] 			= array('title' => 'Special Terms', 'description' => 'Certain taxonomy terms have special meaning for the system; select them here.');
		$links['systems']['links']['sirius/config/email'] 					= array('title' => 'Email', 'description' => 'Oubound email settings');
		$links['systems']['links']['sirius/config/phone'] 					= array('title' => 'Phone', 'description' => 'Phone settings, including caller IDs, SMS and Twilio');
		$links['systems']['links']['sirius/office/operator/list'] 	= array('title' => 'Operators', 'description' => 'List of operators available to the phone tree.');
		$links['systems']['links']['sirius/twilio/network-test'] 		= array('title' => 'Twilio Network Test', 'description' => 'Use the Twilio network test tool to verify if the client computer has sufficient connectivity to run a VoIP call.');
		$links['systems']['links']['sirius/phonenumber'] 						= array('title' => 'Phone Number Cache', 'description' => 'Search and review the cache of phone numbers and deliverability metrics (i.e. SMS-enabled).');
		$links['systems']['links']['sirius/map/admin'] 							= array('title' => 'Map Settings', 'description' => 'Sirius map settings.');
		$links['systems']['links']['sirius/map/admin']['sub_paths']['admin/config/content/geocoder'] = array('title' => 'Geocode Service');
		$links['systems']['links']['sirius/map/admin']['sub_paths']['admin/config/content/geofield_map'] = array('title' => 'Geofield Service');
		$links['systems']['links']['sirius/map/admin']['sub_paths']['admin/config/system/leaflet_more_maps'] = array('title' => 'Leaflet "More Maps" Service');
		$links['systems']['links']['sirius/sns/config'] 						= array('title' => 'SNS', 'description' => 'Settings for Amazon Simple Notification Service (SNS)');
		$links['systems']['links']['sirius/signature/admin'] 				= array('title' => 'Signatures', 'description' => 'User signature settings (image)');
		$links['systems']['links']['sirius/esignature/admin'] 			= array('title' => 'Signatures', 'description' => 'eSignature settings (signature lines, etc.)');
		$links['systems']['links']['admin/config/grievance/phaxio'] = array('title' => 'Phaxio', 'description' => 'Configure the Phaxio fax service.');
		$links['systems']['links']['admin/config/services/shorten'] = array('title' => 'Shorten', 'description' => 'Configure the link shortening services such as blt.ly.');
		$links['systems']['links']['admin/config'] 									= array('title' => 'Drupal Configuration', 'description' => 'Enter the system-wide Drupal configuration screens.');

		drupal_alter('sirius_config_links', $links);

		$taxonomy_settings_all = sirius_taxonomy_settings('all');
		foreach ($taxonomy_settings_all as $vocabulary_machine_name => $taxonomy_settings) {
			$category = $taxonomy_settings['config_category'];
			if (!$category) { continue; }

			$path = "sirius/taxonomy/$vocabulary_machine_name";
			$name = sirius_taxonomy_friendly_name($vocabulary_machine_name);
			$links[$category]['links'][$path] = array('title' => "Drop-down List: " . $name, 'description' => $vocabulary->description);
		}

	  uksort($links, function($a, $b) use ($links) {
	  	if ($links[$a]['weight'] < $links[$b]['weight']) { return 1; }
	  	if ($links[$a]['weight'] > $links[$b]['weight']) { return -1; }
	  	return strcasecmp($links[$a]['title'], $links[$b]['title']);
	  });

	}

	foreach ($links as $category => $record) {
		if (!$record['title']) { $record['title'] = $category; }
	}

	return $links;
}

