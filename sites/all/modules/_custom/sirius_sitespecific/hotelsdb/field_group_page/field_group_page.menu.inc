<?php
/***
 * Implements hook_menu
 */
function field_group_page_menu() {
  $items = array();
	
	if (variable_get('field_group_page_groups', '')) {
		$group_ids = array_values(variable_get('field_group_page_groups', ''));
		foreach ($group_ids as $group_id) {
			if (!$group_id) { continue; }
			$sql = "select * from field_group where id = :group_id";
			$stmt = db_query($sql, array(':group_id' => $group_id));
			$hr = $stmt->fetchAssoc();
			if (!$hr) { continue; }
			$hr['data_unserialized'] = unserialize($hr['data']);
			
			$items['node/%node/' . $hr['group_name']] = array(
				'title' => $hr['data_unserialized']['label'],
				'page callback' => 'node_page_edit',
				'page arguments' => array(1),
				'access callback' => 'field_group_page_access',
				'access arguments' => array('update', 1, $hr['group_name']),
				'weight' => 0,
				'type' => MENU_LOCAL_TASK,
				'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
				'file' => 'node.pages.inc',
				'file path' => drupal_get_path('module', 'node'),
			);
		}
	}
	
  $items['admin/config/content/field-group-page'] = array(
    'title' => 'Field Group Page',
    'description' => 'Settings for the field-group-page module.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('field_group_page_admin'),
    'access arguments' => array('administer site configuration'),
  );
	
	return $items;
}

function field_group_page_access($op, $node, $group_name) {
	// Make sure that this group exists in this entity type
	$sql = "select id from field_group where group_name = :group_name and entity_type = 'node' and bundle = :bundle";
	$stmt = db_query($sql, array(
		':group_name' => $group_name,
		':bundle' => $node->type
	));
	$hr = $stmt->fetchAssoc();
	if (!$hr) { return FALSE; }
	
	return node_access($op, $node);
}