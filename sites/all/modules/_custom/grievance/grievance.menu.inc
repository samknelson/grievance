<?php
/**
 * Tabs: Which node type has which tabs. 
 *
 * Much of the functionality is via local tasks in nodes.  For instance, nodes of type "member" have a local tab called "housing" which 
 * shows housing-related functions. 
 */
 
function grievance_xnode_tabs() {
	$node_tabs = array();

	$node_tabs['grievance']['letter'] = array(
		'title' => 'Letters',
		'description' => '',
		'callback' => 'grievance_node_tab_letter',
		'permissions' => array('grievance user'),
	);

	$node_tabs['grievance']['statuslog'] = array(
		'title' => 'Log',
		'description' => '',
		'callback' => 'grievance_node_tab_log',
		'permissions' => array('grievance statuslog'),
	);

	$node_tabs['grievance']['timeline'] = array(
		'title' => 'Timeline',
		'description' => '',
		'callback' => 'grievance_node_tab_timeline',
		'permissions' => array('grievance statuslog'),
	);

	$node_tabs['grievance']['status_correct'] = array(
		'title' => 'Status Correct',
		'description' => '',
		'callback' => 'grievance_node_tab_status_correct',
		'permissions' => array('grievance admin'),
	);

	$node_tabs['grievance']['attachments'] = array(
		'title' => 'Attachments',
		'description' => '',
		'callback' => 'grievance_node_tab_attachments',
		'permissions' => array('grievance attachment'),
	);

	$node_tabs['grievance']['settlements'] = array(
		'title' => 'Settlements',
		'description' => '',
		'callback' => 'grievance_node_tab_settlements',
		'permissions' => array('grievance settlement'),
	);

	$node_tabs['grievance']['prior'] = array(
		'title' => 'Prior',
		'description' => '',
		'callback' => 'grievance_node_tab_prior',
		'permissions' => array('grievance view'),
	);

	// These will be obsolete any minute, as soon as Local 100 moves over to letters.
	/*
	$node_tabs['grievance']['gform'] = array(
		'title' => 'Forms',
		'description' => '',
		'callback' => 'grievance_node_tab_gform',
		'permissions' => array('grievance form'),
	);

	$node_tabs['grievance']['gform/gform'] = array(
		'title' => 'Grievance Form',
		'description' => '',
		'callback' => 'grievance_node_tab_gform',
		'permissions' => array('grievance form'),
	);

	$node_tabs['grievance']['gform/arbitrationform'] = array(
		'title' => 'Arbitration Prep Form',
		'description' => '',
		'callback' => 'grievance_node_tab_arbitrationform',
		'permissions' => array('grievance form'),
	);
	*/

	$node_tabs['grievance']['bundle'] = array(
		'title' => 'Bundle',
		'description' => '',
		'callback' => 'grievance_node_tab_bundle',
		'permissions' => array('grievance bundle'),
	);

	$node_tabs['grievance']['bundle/download'] = array(
		'title' => 'Bundle',
		'description' => '',
		'callback' => 'grievance_node_tab_bundle_download',
		'permissions' => array('grievance bundle'),
	);

	$node_tabs['grievance']['grievance_merge'] = array(
		'title' => 'Merge',
		'description' => '',
		'callback' => 'grievance_node_tab_merge',
		'permissions' => array('grievance admin'),
	);

	$node_tabs['grievance_shop']['contacts'] = array(
		'title' => 'Contacts',
		'description' => '',
		'callback' => 'grievance_shop_node_tab_contacts',
		'permissions' => array(),
	);

	$node_tabs['grievance_company']['contacts'] = array(
		'title' => 'Contacts',
		'description' => '',
		'callback' => 'grievance_company_node_tab_contacts',
		'permissions' => array('grievance contact'),
	);

	$node_tabs['grievance_contract_template']['rules'] = array(
		'title' => 'Rules',
		'description' => '',
		'callback' => 'grievance_contract_template_node_tab_rules',
		'permissions' => array('grievance contract'),
	);

	$node_tabs['grievance_contract_template']['sections'] = array(
		'title' => 'Articles',
		'description' => '',
		'callback' => 'grievance_contract_template_node_tab_sections',
		'permissions' => array('grievance contract view'),
	);

	$node_tabs['grievance_contract_template']['sections/list'] = array(
		'title' => 'List',
		'description' => '',
		'callback' => 'grievance_contract_template_node_tab_sections_list',
		'permissions' => array('grievance contract view'),
		'weight' => 1,
	);

	$node_tabs['grievance_contract_template']['sections/bulk'] = array(
		'title' => 'Bulk Edit',
		'description' => '',
		'callback' => 'grievance_contract_template_node_tab_sections_bulk',
		'permissions' => array('grievance contract'),
		'weight' => 2,
	);



	$node_tabs['grievance_contract_template']['clauses'] = array(
		'title' => 'Sections',
		'description' => '',
		'callback' => 'grievance_contract_template_node_tab_clauses',
		'permissions' => array('grievance contract view'),
	);

	$node_tabs['grievance_contract_template']['clauses/list'] = array(
		'title' => 'Sections',
		'description' => '',
		'callback' => 'grievance_contract_template_node_tab_clauses_list',
		'permissions' => array('grievance contract view'),
		'weight' => 1,
	);

	$node_tabs['grievance_contract_template']['clauses/full'] = array(
		'title' => 'Full Text',
		'description' => '',
		'callback' => 'grievance_contract_template_node_tab_full',
		'permissions' => array('grievance contract view'),
		'weight' => 2,
	);

	$node_tabs['grievance_contract_template']['clauses/pdf'] = array(
		'title' => 'PDF',
		'description' => '',
		'callback' => 'grievance_contract_template_node_tab_pdf',
		'permissions' => array('grievance contract view'),
		'weight' => 3,
	);

	$node_tabs['grievance_contract_template']['clauses/picker'] = array(
		'title' => 'Clause Picker',
		'description' => '',
		'callback' => 'grievance_contract_template_node_tab_picker',
		'permissions' => array('grievance contract view'),
		'weight' => 4,
	);

	$node_tabs['grievance_contract_template']['clauses/bulk-rename'] = array(
		'title' => 'Bulk Rename',
		'description' => '',
		'callback' => 'grievance_contract_template_node_tab_clause_bulk_rename',
		'permissions' => array('grievance contract edit'),
		'weight' => 5,
	);




	$node_tabs['grievance_contract_section']['clauses'] = array(
		'title' => 'Sections',
		'description' => '',
		'callback' => 'grievance_contract_section_node_tab_clauses',
		'permissions' => array('grievance contract view'),
	);

	$node_tabs['grievance_contract_section']['clauses/list'] = array(
		'title' => 'List',
		'description' => '',
		'callback' => 'grievance_contract_section_node_tab_clauses_list',
		'permissions' => array('grievance contract view'),
		'weight' => 1,
	);

	$node_tabs['grievance_contract_section']['clauses/bulk'] = array(
		'title' => 'Bulk Edit',
		'description' => '',
		'callback' => 'grievance_contract_section_node_tab_clauses_bulk',
		'permissions' => array('grievance contract'),
		'weight' => 2,
	);

	$node_tabs['grievance_contract_template']['holidays'] = array(
		'title' => 'Holidays',
		'description' => '',
		'callback' => 'grievance_contract_template_node_tab_holidays',
		'permissions' => array('grievance contract'),
	);
	
	$node_tabs['grievance_contract_template']['replicate'] = array(
		'title' => 'Duplicate',
		'description' => '',
		'callback' => 'grievance_contract_template_node_tab_replicate',
		'permissions' => array('grievance contract'),
	);

	$node_tabs['grievance_contract_template']['replicate/all'] = array(
		'title' => 'Duplicate All',
		'description' => '',
		'callback' => 'grievance_contract_template_node_tab_replicate_all',
		'permissions' => array('grievance contract'),
	);

	$node_tabs['grievance_contract_template']['replicate/rules'] = array(
		'title' => 'Duplicate Timeline Only',
		'description' => '',
		'callback' => 'grievance_contract_template_node_tab_replicate_rules',
		'permissions' => array('grievance contract'),
	);

	return $node_tabs;
}

/***
 * Implements hook_menu
 */
function grievance_menu() {
  $items = array();

  //
	// Node Tabs
  // 

	$node_tabs = grievance_xnode_tabs();
	foreach ($node_tabs as $node_type => $node_record) {
		foreach ($node_record as $tab_name => $tab_record) {
			$items['node/%node/' . $tab_name] = array(
				'title' => $tab_record['title'],
				'description' => $tab_record['description'],
				'page callback' => 'grievance_node_tab_content',
				'page arguments' => array(1, $tab_name),
				'access callback' => 'grievance_node_tab_access',
				'access arguments' => array(1, $tab_name),
				'type' => MENU_LOCAL_TASK,
			);
			if ($tab_record['weight']) {
				$items['node/%node/' . $tab_name]['weight'] = $tab_record['weight'];
			}
		}
	}

  //
	// Home
  // 
  
	$items['grievances'] = array(
		'title' => t('My Grievances'),
		'page callback' => 'grievance_home',
		'access arguments' => array('grievance user'),
		'type' => MENU_NORMAL_ITEM,
	);

  //
	// My Grievances
  // 
  
	$items['grievances/mine'] = array(
		'title' => t('My Grievances'),
		'page callback' => 'grievance_mine',
		'access arguments' => array('grievance user'),
		'type' => MENU_LOCAL_TASK,
		'weight' => 1,
	);

	$items['grievances/mine/upcoming'] = array(
		'title' => t('My Upcoming Grievances'),
		'page callback' => 'grievance_mine',
		'access arguments' => array('grievance user'),
		'type' => MENU_DEFAULT_LOCAL_TASK,
		'weight' => 0,
	);

	$items['grievances/mine/expanded'] = array(
		'title' => t('Tickler'),
		'page callback' => 'grievance_mine_expanded',
		'page arguments' => array(),
		'access arguments' => array('grievance mine expanded'),
		'type' => MENU_LOCAL_TASK,
		'weight' => 1,
	);

	$items['grievances/mine/mine-all'] = array(
		'title' => t('All My Grievances'),
		'page callback' => 'grievance_mine_all',
		'access arguments' => array('grievance user'),
		'type' => MENU_LOCAL_TASK,
		'weight' => 2,
	);

	$items['grievances/mine/reminder-30'] = array(
		'title' => t('30-Day'),
		'page callback' => 'grievance_mine_reminder_30',
		'page arguments' => array(),
		'access arguments' => array('grievance user'),
		'type' => MENU_LOCAL_TASK,
		'weight' => 3,
	);


	$items['grievances/mine/reminder-60'] = array(
		'title' => t('60-Day'),
		'page callback' => 'grievance_mine_reminder_60',
		'page arguments' => array(),
		'access arguments' => array('grievance user'),
		'type' => MENU_LOCAL_TASK,
		'weight' => 4,
	);


	$items['grievances/mine/reminder-90'] = array(
		'title' => t('90-Day'),
		'page callback' => 'grievance_mine_reminder_90',
		'page arguments' => array(),
		'access arguments' => array('grievance user'),
		'type' => MENU_LOCAL_TASK,
		'weight' => 5,
	);

	$items['grievances/mine/reminder-no-first-call'] = array(
		'title' => t('No First Call'),
		'page callback' => 'grievance_mine_reminder_no_first_call',
		'page arguments' => array(),
		'access arguments' => array('grievance mine no first call'),
		'type' => MENU_LOCAL_TASK,
		'weight' => 6,
	);

	$items['grievances/mine/assigned-today'] = array(
		'title' => t('Assigned Today'),
		'page callback' => 'grievance_mine_assigned_today',
		'page arguments' => array(),
		'access arguments' => array('grievance form rep assignee'),
		'type' => MENU_LOCAL_TASK,
		'weight' => 6,
	);
	

  //
	// All Grievances
  // 
  

	$items['grievances/all'] = array(
		'title' => t('All Grievances'),
		'page callback' => 'grievance_all',
		'page arguments' => array(),
		'access arguments' => array('grievance view'),
		'type' => MENU_LOCAL_TASK,
		'weight' => 2,
	);

	$items['node/%node/loopback'] = array(
		'page callback' => 'grievance_forward_loopback',
		'page arguments' => array(1),
		'access callback' => 'grievance_forward_loopback_access',
		'access arguments' => array(1),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['node/%node/loopback-gform'] = array(
		'page callback' => 'grievance_gform_loopback',
		'page arguments' => array(1),
		'access callback' => 'grievance_forward_loopback_access',
		'access arguments' => array(1),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['node/%node/loopback-letter'] = array(
		'page callback' => 'grievance_letter_loopback',
		'page arguments' => array(1),
		'access callback' => 'grievance_forward_loopback_access',
		'access arguments' => array(1),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['node/%node/loopback-arbitrationform'] = array(
		'page callback' => 'grievance_arbitrationform_loopback',
		'page arguments' => array(1),
		'access callback' => 'grievance_forward_loopback_access',
		'access arguments' => array(1),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['admin/config/grievance'] = array(
		'title' => t('Grievance Module'),
		'description' => t('Configuration for the Grievance suite of modules') ,
		'page callback' => 'system_admin_menu_block_page',
		'access arguments' => array('grievance admin'),
		'file' => 'system.admin.inc',
		'file path' => drupal_get_path('module', 'system'),
		'position' => 'right',
	);

	$items['admin/config/grievance/core'] = array(
		'title' => 'Grievance Module - Core',
		'description' => t('Settings for the Grievance Module core'),
		'page callback' => 'drupal_get_form',
		'page arguments' => array('grievance_admin'),
		'access arguments' => array('grievance admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['admin/config/grievance/core/core'] = array(
		'title' => 'Core',
		'description' => t('Settings for the Grievance Module core'),
		'page callback' => 'drupal_get_form',
		'page arguments' => array('grievance_admin'),
		'access arguments' => array('grievance admin'),
		'type' => MENU_DEFAULT_LOCAL_TASK,
	);

	$items['admin/config/grievance/gform'] = array(
		'title' => 'Grievance Module - Form',
		'description' => t('Settings for the Grievance Form'),
		'page callback' => 'drupal_get_form',
		'page arguments' => array('grievance_gform_admin'),
		'access arguments' => array('grievance admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['admin/config/grievance/arbitrationform'] = array(
		'title' => 'Grievance Module - Arbitration Prep Form',
		'description' => t('Settings for the Grievance Arbitration Prep Form'),
		'page callback' => 'drupal_get_form',
		'page arguments' => array('grievance_arbitrationform_admin'),
		'access arguments' => array('grievance admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['admin/config/grievance/quicksearch'] = array(
		'title' => 'Grievance Module - QuickSearch',
		'description' => t('Settings for the quicksearch'),
		'page callback' => 'drupal_get_form',
		'page arguments' => array('grievance_quicksearch_admin'),
		'access arguments' => array('grievance admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['admin/config/grievance/contacts'] = array(
		'title' => 'Grievance Module - Contacts',
		'description' => t('Contact-related settings'),
		'page callback' => 'drupal_get_form',
		'page arguments' => array('grievance_contacts_admin'),
		'access arguments' => array('grievance admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['admin/config/grievance/workersync'] = array(
		'title' => 'Grievance Module - Worker Sync',
		'page callback' => 'drupal_get_form',
		'page arguments' => array('grievance_workersync_admin'),
		'access arguments' => array('grievance admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['admin/config/grievance/workersync/settings'] = array(
		'title' => 'Settings',
		'page callback' => 'drupal_get_form',
		'page arguments' => array('grievance_workersync_admin'),
		'access arguments' => array('grievance admin'),
		'type' => MENU_DEFAULT_LOCAL_TASK,
		'weight' => 1,
	);

	$items['admin/config/grievance/workersync/test'] = array(
		'title' => 'Test',
		'page callback' => 'drupal_get_form',
		'page arguments' => array('grievance_workersync_test_form'),
		'access arguments' => array('grievance admin'),
		'type' => MENU_LOCAL_TASK,
		'weight' => 2,
	);

	$items['admin/config/grievance/workersync/sync'] = array(
		'title' => 'Sync',
		'page callback' => 'drupal_get_form',
		'page arguments' => array('grievance_workersync_sync_form'),
		'access arguments' => array('grievance admin'),
		'type' => MENU_LOCAL_TASK,
		'weight' => 3,
	);

	$items['grievance/workersync/lookup/%'] = array(
		'page callback' => 'grievance_workersync_lookup_ajax',
		'page arguments' => array(3),
		'access arguments' => array('grievance worker lookup'),
		'type' => MENU_CALLBACK,
	);

	$items['grievance/report/bytype'] = array(
		'title' => 'Report: Count by Type',
		'description' => '',
		'page callback' => 'grievance_report_bytype',
		'page arguments' => array(),
		'access arguments' => array('grievance report bytype'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['grievance/report/shop-monthly'] = array(
		'title' => 'Report: Count by Type',
		'description' => '',
		'page callback' => 'grievance_report_shop_monthly',
		'page arguments' => array(),
		'access arguments' => array('grievance report'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['grievance/report/status-summary'] = array(
		'title' => 'Report: Status Summary',
		'description' => '',
		'page callback' => 'grievance_report_status_summary',
		'page arguments' => array(),
		'access arguments' => array('grievance report'),
		'type' => MENU_NORMAL_ITEM,
	);

	/*
	$items['grievance/report/open'] = array(
		'title' => 'Report: Open Grievances by Specialist',
		'description' => '',
		'page callback' => 'grievance_report_open_by_specialist',
		'page arguments' => array(),
		'access arguments' => array('grievance view'),
		'type' => MENU_NORMAL_ITEM,
		'file' => 'grievance.report_open_by_specialist.inc',
	);
	*/

	$items['grievance/report/average-time'] = array(
		'title' => 'Report: Average Time Between Statuses',
		'description' => '',
		'page callback' => 'grievance_report_average_time',
		'page arguments' => array(),
		'access arguments' => array('grievance report'),
		'type' => MENU_NORMAL_ITEM,
	);

	// Legacy support
	$items['grievance/report/settlement/summary'] = array(
		'title' => 'Report: Settlement Summary',
		'description' => '',
		'page callback' => 'grievance_report_settlement_summary_redirect',
		'page arguments' => array(),
		'access arguments' => array('grievance report'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['grievance/report/spi-term'] = array(
		'title' => 'Report: SPI and Term',
		'description' => '',
		'page callback' => 'grievance_report_spi_term',
		'page arguments' => array(),
		'access arguments' => array('grievance report'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['grievance/report/year-summary'] = array(
		'title' => 'Report: Yearly Summary',
		'description' => '',
		'page callback' => 'grievance_report_year_summary',
		'page arguments' => array(),
		'access arguments' => array('grievance report'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['grievance/report/aged-summary'] = array(
		'title' => 'Report: Aged Summary',
		'description' => '',
		'page callback' => 'grievance_report_aged_summary',
		'page arguments' => array(),
		'access arguments' => array('grievance report'),
		'type' => MENU_NORMAL_ITEM,
	);

	// Legacy support
	$items['grievance/report/open-summary-log'] = array(
		'title' => 'Report: Open Grievances: Summary with Log',
		'description' => '',
		'page callback' => 'grievance_report_open_summary_with_log_redirect',
		'page arguments' => array(),
		'access arguments' => array('grievance report open summary with log'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['admin/config/grievances/fieldoverrides/apply'] = array(
		'title' => 'Apply Field Overrides',
		'description' => '',
		'page callback' => 'grievance_field_overrides_apply',
		'page arguments' => array(),
		'access arguments' => array('grievance admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	$items['grievance/admin/shops/bulk-reassign'] = array(
		'title' => 'Bulk Re-Assign Grievances',
		'description' => '',
		'page callback' => 'grievance_shop_bulk_reassign',
		'page arguments' => array(),
		'access arguments' => array('grievance admin'),
		'type' => MENU_NORMAL_ITEM,
	);

	// QuickSearch
	
	$items['grievance/ajax/quicksearch'] = array(
		'page callback' => 'grievance_quicksearch_autocomplete',
		'page arguments' => array(),
		'access arguments' => array('grievance quicksearch'),
		'type' => MENU_CALLBACK,
	);

	$items['grievance/admin/quicksearch/%'] = array(
		'page callback' => 'grievance_quicksearch_page',
		'page arguments' => array(3),
		'access arguments' => array('grievance quicksearch'),
		'type' => MENU_CALLBACK,
	);


  $items['grievance/modal/%node/log-add'] = array(
    'title'            => 'Add Log Entry',
    'page callback'    => 'grievance_log_modal_add',
    'page arguments'   => array(2),
    'access arguments' => array('grievance statuslog'),
    'type'             => MENU_LOCAL_ACTION,
    'context'          => MENU_CONTEXT_INLINE,
    'modal'            => TRUE,
  );

	$items['grievance/autocomplete/grievance-job-class-tid/%'] = array(
		'page callback' => 'grievance_autocompete_job_class_tid',
		'page arguments' => array(3),
		'access arguments' => array('grievance user'),
		'type' => MENU_CALLBACK,
	);

	// Custom search home -- should be denied to all

	$items['grievance/report/custom'] = array(
		'page callback' => 'grievance_menu_deny',
		'page arguments' => array(),
		'access callback' => 'grievance_menu_deny_access',
		'access arguments' => array(),
		'type' => MENU_CALLBACK,
	);

	$items['user/%/grievance_preferences'] = array(
		'title' => 'Grievance Preferences',
		'page callback' => 'grievance_page_preferences',
		'page arguments' => array(1),
		'access callback' => 'grievance_page_preferences_access',
		'access arguments' => array(1),
		'type' => MENU_LOCAL_TASK,
	);

  return $items;
}

function grievance_menu_deny() { return 'Access denied.'; }
function grievance_menu_deny_access() { return FALSE; }

function grievance_node_tab_content($node, $in_tab_name) {
	$node_tabs = grievance_xnode_tabs();
	foreach ($node_tabs as $node_type => $node_record) {
		if ($node->type == $node_type) {
			foreach ($node_record as $tab_name => $tab_record) {
				if ($tab_name == $in_tab_name) {
					$callback = $tab_record['callback'];
					return $callback($node);
				}
			}
		}
	}
	
	drupal_set_message("Bad tab: $node->type, $in_tab_name");
	
	return "";
}

function grievance_node_tab_access($node, $in_tab_name) {
	// Specific logic re. edit
	global $user;
	if ($in_tab_name == 'notification' && $node->type == 'grievance') {
		if (grievance_node_access_grievance($node, 'update', $user) == NODE_ACCESS_DENY) {
			return false;
		}
	}

	// Check generic permissions
	$node_tabs = grievance_xnode_tabs();
	foreach ($node_tabs as $node_type => $node_record) {
		if ($node->type == $node_type) {
			foreach ($node_record as $tab_name => $tab_record) {
				if ($tab_name == $in_tab_name) {
					foreach ($tab_record['permissions'] as $permission) {
						if (!user_access($permission)) { return false; }
					}

          // Check for a custom access permission
          $access_function_name = $tab_record['callback'] . '_access';
          if (function_exists($access_function_name)) {
            if (!$access_function_name($node)) {
              return false;
            }
          }
					
					return true;
				}
			}
		}
	}
	
	return false;
}

function grievance_node_tab_get_addlink($content_type, $query, $prompt = NULL) {
	if (!$prompt) { $prompt = t('Add a new entry'); }

	$query['destination'] = current_path();

	global $user;
	$grievance_nid = $query['grievance'];
	if ($grievance_nid) {
		$grievance_node = node_load($grievance_nid);
		if (grievance_node_access_grievance($grievance_node, 'update', $user) == NODE_ACCESS_DENY) {
			return '';
		}
	}
	
	$path = 'node/add/' . $content_type;
	$item = menu_get_item($path);
	if (!$item['access']) { return ''; }
	
	$result .= l(
		$prompt, 
		$path,
		array('query' => $query, 'attributes' => array('class' => array('tool-button')))
	);
	
	return $result;
}

/**
 * Implments hook_modal_paths
 */

function grievance_modal_paths() {
  $paths = array();

  $paths['grievance/modal/%node/log-add'] = array(
    'style' => 'grievance-modal-style',
  );

  return $paths;
}

function grievance_modal_styles() {
  $styles = array();

  $styles['grievance-modal-style'] = array(
    'modalSize' => array(
      'type' => 'fixed',
      'width' => 500,
      'height' => 350,
    ),
  );

  return $styles;
}

function grievance_report_settlement_summary_redirect() {
  drupal_goto('sirius/reports/grievance-report-settlement-summary/form');
}
