<?php


function sirius_node_access_domain($node, $op, $account) {
  // Admins can do anything
  if (user_access('sirius admin', $account)) { return NODE_ACCESS_ALLOW; }

  // Domain admins can access
  if (user_access('sirius domain admin') && sirius_domain_access($node->nid)) { return NODE_ACCESS_ALLOW; }
  if (user_access('sirius domain admin') && $op == 'create') { return NODE_ACCESS_ALLOW; }

  // Nobody else can see do anyhing.
  return NODE_ACCESS_DENY;
}

function sirius_domain_node_validate($node, $form, &$form_state) {
  if ($node->nid && ($node->field_sirius_domain['und'][0]['target_id'] == $node->nid)) {
    form_set_error('field_sirius_domain', t('Parent cannot be self.'));
  }
}

/**
 * Fetch the root domain for the current user
 */

function sirius_domain_root_nid() {
  $cache =& drupal_static(__FUNCTION__, array());

	global $user;
	if (!$user) { return NULL; }
	if (!$user->uid) { return NULL; }

  $uid = $user->uid;
  if (!array_key_exists($uid, $cache)) {
    // $sql for speed
    $sql = "select field_sirius_domain_target_id ";
    $sql .= "from field_data_field_sirius_domain ";
    $sql .= "where entity_type = 'user' and entity_id = :uid ";
    $stmt = sirius_sql_query($sql, array(':uid' => $uid));
    $hr = $stmt->fetchAssoc();
    $cache[$uid] = $hr['field_sirius_domain_target_id'];
  }

  return $cache[$uid];

	// $user_full = user_load($user->uid);
	// return node_load($user_full->field_sirius_domain['und'][0]['target_id']);
}

function sirius_domain_nid() {
  $domain_nid = $_SESSION['domain_nid'];
  if (!$domain_nid) { $domain_nid = sirius_domain_root_nid(); }
  if ($domain_nid < 0) { return NULL; }
  return $domain_nid;
}

function sirius_domain_root() {
  $domain_nid = sirius_domain_root_nid();
  if (!$domain_nid) { return NULL; }
  return node_load($domain_nid);
}

/**
 * Fetch or set the current domain for the current user.
 */

function sirius_domain($new_domain_nid = NULL) {
	// Set if necessary
	if ($new_domain_nid) { $_SESSION['domain_nid'] = $new_domain_nid; }

  $domain_nid = sirius_domain_nid();
  if (!$domain_nid) { return NULL; }
  return node_load($domain_nid);
}

function sirius_domain_component_info_all() {
  $infos = &drupal_static(__FUNCTION__);
  if (!isset($infos)) {
    $infos = module_invoke_all('sirius_domain_component_info');
    ksort($infos);
  }
  return $infos;
}

/**
 * Implements hook_sirius_domain_component_info
 */

function sirius_sirius_domain_component_info() {
  return array(
    'worker' => array(
      'title' => 'Worker',
      'description' => 'Use the worker database',
    ),
    'worker:schedule' => array(
      'title' => 'Worker - Schedule',
      'description' => 'Use scheduling tools for workers such as time off requests',
    ),
    'worker:skill' => array(
      'title' => 'Worker - Skill',
      'description' => 'Manage skills for workers',
    ),
    'employer' => array(
      'title' => 'Employer',
      'description' => 'Use the employer database',
    ),
    'activist:phonebank:prescan' => array(
      'title' => 'Activist - Phonebank - Prescan',
      'description' => 'For a given phonebank, enable the "prescan" function, allowing phonebank admins to remove invalid phone numbers.',
    ),
    'grievance' => array(
      'title' => 'Grievance',
      'description' => 'Grievance and contract enforcement functionality',
    ),
    'grievance:alert:assignment' => array(
      'title' => 'Grievance - Alert - Assignment',
      'description' => 'Notify users when a new grievance has been assigned to them',
    ),
    'mapping' => array(
      'title' => 'Mapping',
      'description' => 'Enable geocoding and map generation.'
    ),
    'mapping:checkin' => array(
      'title' => 'Mapping - Checkin',
      'description' => 'Enable users to "check in" with their current location, so that domain admins can follow their movements on maps.',
    ),
    'bulk' => array(
      'title' => 'Bulk',
      'description' => 'Use the bulk message system',
    ),
    'public' => array(
      'title' => 'Public',
      'description' => 'Permission to create public pages',
    ),
    'training' => array(
      'title' => 'Training Center',
      'description' => 'Use training center functionality, including levels and certifications',
    ),
    'contact:relationships' => array(
      'title' => 'Contact - Relationships',
      'description' => 'Enable relationships between contacts (or workers)',
    ),
    'service' => array(
      'title' => 'Services',
      'description' => 'Enable functionality to allow Sirius to be a service for remote function calls',
    ),
    'service:client' => array(
      'title' => 'Services - Client',
      'description' => 'Allow this instance to server as a client',
    ),
    'service:server' => array(
      'title' => 'Services - Server',
      'description' => 'Allow this instance to server as a server',
    ),
    'ledger:stripe' => array(
      'title' => 'Ledger - Stripe',
      'description' => 'Enable this domain to connect to the Stripe payment processor',
    ),
    'phonetree' => array(
      'title' => 'Phonetree',
      'description' => 'Enable phonetree processing',
    ),
  );
}

function sirius_domain_component_enable($key, $domain_node) {
  $json = sirius_json_get($domain_node);
  $json['component'][$key]['enabled'] = 'Yes';
  sirius_json_set($domain_node, $json);
  node_save($domain_node);
}


function sirius_domain_component_enabled($key, $domain_nid_or_node = NULL) {
  if (!$domain_nid_or_node) {
    $domain_nid = sirius_domain_nid(); 
  } else if (is_numeric($domain_nid_or_node)) {
    $domain_nid = $domain_nid_or_node;
  } else if (is_object($domain_nid_or_node)) {
    $domain_nid = $domain_nid_or_node->nid;
    $domain_node = $domain_nid_or_node;
  }

  $cache = &drupal_static(__FUNCTION__);
  if (!isset($cache)) { $cache = array(); }

  if (!isset($cache[$domain_nid][$key])) {
    if (!$domain_node) { $domain_node = node_load($domain_nid); }
    $json = sirius_json_get($domain_node);

    if ($domain_nid < 1) {
      // Legacy support: In the root domain everything is enabled unless explititly removed
      $variable_name = 'sirius_domain_root_component_enabled_' . $key;
      $cache[$domain_nid][$key] = (variable_get($variable_name, 'Yes') != 'No');
    } else {
      // Normal domains: Must explicitly enable all components
      $cache[$domain_nid][$key] = ($json['component'][$key]['enabled'] == 'Yes');
    }
  }

  return $cache[$domain_nid][$key];
}

function sirius_domain_active($domain_nid) {
  $sql = "select field_sirius_active_value, field_sirius_domain_target_id ";
  $sql .= " from field_data_field_sirius_active ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = field_data_field_sirius_active.entity_id ";
  $sql .= "where field_data_field_sirius_active.entity_type = 'node' ";
  $sql .= "and field_data_field_sirius_active.entity_id = :domain_nid ";
  $stmt = db_query($sql, array(':domain_nid' => $domain_nid));
  $hr = $stmt->fetchAssoc();

  // If this domain is inactive, return FALSE
  if ($hr['field_sirius_active_value'] == 'No') { return FALSE; }

  // If we have a parent, return the value for the parent
  if ($hr['field_sirius_domain_target_id']) { return sirius_domain_active($hr['field_sirius_domain_target_id']); }

  // Neither this domain nor its parents are inactive, so it must be active.
  return TRUE;
}

function sirius_domain_user_login(&$edit, $account) {
  $domain_nid = $account->field_sirius_domain['und'][0]['target_id'];
  if ($domain_nid) {
    if (!sirius_domain_active($domain_nid)) {
      global $user;
      module_invoke_all('user_logout', $user);
      session_destroy();
      $user = drupal_anonymous_user();
      drupal_set_message("Login prohibited; your domain or one of its parents is disabled.", 'warning');
    }
  }
}

function sirius_domain_uids($domain_nid, $depth = 0) {
  if ($depth > 100) { return array(); }
  // Users in this domain.
  $sql = "select entity_id from field_data_field_sirius_domain ";
  $sql .= "where entity_type = 'user' ";
  $sql .= "and field_sirius_domain_target_id = :domain_nid ";
  $stmt = db_query($sql, array(':domain_nid' => $domain_nid));
  $uids = array();
  while ($hr = $stmt->fetchAssoc()) {
    $uids[] = $hr['entity_id'];
  }

  // Users in all children
  $sql = "select entity_id from field_data_field_sirius_domain ";
  $sql .= "where entity_type = 'node' ";
  $sql .= "and bundle = 'sirius_domain' ";
  $sql .= "and field_sirius_domain_target_id = :domain_nid ";
  $stmt = db_query($sql, array(':domain_nid' => $domain_nid));
  $child_domain_nids = array();
  while ($hr = $stmt->fetchAssoc()) {
    $child_domain_nids[] = $hr['entity_id'];
  }
  foreach ($child_domain_nids as $child_domain_nid) {
    $uids = array_merge($uids, sirius_domain_uids($child_domain_nid, $depth+1));
  }

  if (!$uids) { $uids = array(); }
  return $uids;
}

/**
 * If a domain is set to "inactive", force logout all users in the domain (including child domains).
 */

function sirius_domain_node_update($domain_node) {
  if ($domain_node->field_sirius_active['und'][0]['value'] == 'No') {
    $uids = sirius_domain_uids($domain_node->nid);
    foreach ($uids as $uid) {
      drupal_session_destroy_uid($uid);
    }
  }
}


/**
 * Determine if the current user has access to a specific domain
 */

function sirius_domain_access($domain_nid, $root = NULL, $depth = 0) {
	// Avoid infinite loops
	if ($depth > 100) { return FALSE; }

	// Admin can do anything
	if (user_access('sirius admin')) { return TRUE; }

	// Must be a domain!
	$sql = "select type from node where nid = :domain_nid";
	$stmt = db_query($sql, array(':domain_nid' => $domain_nid));
	$hr = $stmt->fetchAssoc();
	if ($hr['type'] != 'sirius_domain') { return FALSE; }

	// Fetch our root
	if (!$root) { $root = sirius_domain_root(); }
	if (!$root) { return FALSE; }
	if ($root->nid == $domain_nid) { return TRUE; }

	// Fetch the parent
	$sql = "select field_sirius_domain_target_id from field_data_field_sirius_domain where entity_type = 'node' and entity_id = :domain_nid ";
	$stmt = db_query($sql, array(':domain_nid' => $domain_nid));
	$hr = $stmt->fetchAssoc();
	$parent_domain_nid = $hr['field_sirius_domain_target_id'];
	if (!$parent_domain_nid) { return FALSE; }
	if ($parent_domain_nid == $root->nid) { return TRUE; }

	// Do I have access to mommy?
	return sirius_domain_access($parent_domain_nid, $root, $depth+1);
}

/**
 * Fetch all the children of a domain
 */


function sirius_domain_tree($root = NULL) {
	$args = array();
	$sql = "select nid, title, field_sirius_name_short_value from node ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_name_short on field_data_field_sirius_name_short.entity_type = 'node' and field_data_field_sirius_name_short.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_domain' ";
	if ($root) {
		$sql .= "and field_sirius_domain_target_id = :root ";
		$args[':root'] = $root;
	} else {
		$sql .= "and field_sirius_domain_target_id IS NULL ";
	}
	$sql .= "order by node.title ";

	$stmt = db_query($sql, $args);

	$tree = array();
	while ($hr = $stmt->fetchAssoc()) {
		$nid = $hr['nid'];
		$title = $hr['title'];

    $tree[$nid]['title'] = $title;
    $tree[$nid]['name_short'] = $hr['field_sirius_name_short_value'];
		$tree[$nid]['children'] = sirius_domain_tree($nid);
	}
	return $tree;
}

function sirius_domain_tree_render_as_select($root = NULL, $depth = 0) {
  $root_nid = NULL; 
  if (is_object($root)) { $root_nid = $root->nid; } else { $root_nid = $root; }
  $tree = sirius_domain_tree($root_nid);
  $options = array();
  _sirius_domain_tree_render_as_select($tree, $options, $depth);
  return $options;
}

function _sirius_domain_tree_render_as_select($tree, &$options, $depth = 0) {
  if ($depth > 100) { return; }
  foreach ($tree as $nid => $obj) {
    $title = '';
    for ($i=0; $i<$depth; ++$i) { $title .= '- '; }
    $title .= $obj['title'];
    $options[$nid] = $title;
    if ($obj['children']) {
      _sirius_domain_tree_render_as_select($obj['children'], $options, $depth+1);
    }
  }
}

function sirius_domain_taxonomy_is_multidomain($vid_or_machine_name) {
  if ($vid_or_machine_name > 0) {
    $sql = "select machine_name from taxonomy_vocabulary where vid = :vid";
    $stmt = db_query($sql, array(':vid' => $vid_or_machine_name));
    $hr = $stmt->fetchAssoc();
    $vocabulary_machine_name = $hr['machine_name'];
  } else {
    $vocabulary_machine_name = $vid_or_machine_name;
  }
  $settings = sirius_taxonomy_settings($vocabulary_machine_name);
  return ($settings['multidomain'] == 'Yes');
}

/**
 * Rewrite taxonomy_get_tree with support for domain restriction
 */

function sirius_domain_taxonomy_get_tree($vid, $parent = 0, $max_depth = NULL, $load_entities = FALSE, $domain_nid = NULL) {
  $children =& drupal_static(__FUNCTION__, array());
  $parents =& drupal_static(__FUNCTION__ . ':parents', array());
  $terms =& drupal_static(__FUNCTION__ . ':terms', array());

  $is_multidomain = sirius_domain_taxonomy_is_multidomain($vid);


  // We cache trees, so it's not CPU-intensive to call taxonomy_get_tree() on a
  // term and its children, too.
  if (!isset($children[$vid])) {
    $children[$vid] = array();
    $parents[$vid] = array();
    $terms[$vid] = array();
    $query = db_select('taxonomy_term_data', 't');
    $query->join('taxonomy_term_hierarchy', 'h', 'h.tid = t.tid');
    if ($is_multidomain == 'Yes') {
      if ($domain_nid) {
        $query->join('field_data_field_sirius_domain', 'd', "d.entity_type = 'taxonomy_term' and d.entity_id = t.tid and d.field_sirius_domain_target_id = $domain_nid" );
      } else {
        $query->leftjoin('field_data_field_sirius_domain', 'd', "d.entity_type = 'taxonomy_term' and d.entity_id = t.tid");
        $query->condition('d.entity_id', NULL, 'IS NULL');
        // @todo
      }
    }
    $result = $query
      ->addTag('translatable')
      ->addTag('taxonomy_term_access')
      ->fields('t')
      ->fields('h', array(
      'parent',
    ))
      ->condition('t.vid', $vid)
      ->orderBy('t.weight')
      ->orderBy('t.name')
      ->execute();
    foreach ($result as $term) {
      $children[$vid][$term->parent][] = $term->tid;
      $parents[$vid][$term->tid][] = $term->parent;
      $terms[$vid][$term->tid] = $term;
    }
  }

  // Load full entities, if necessary. The entity controller statically
  // caches the results.
  if ($load_entities) {
    $term_entities = taxonomy_term_load_multiple(array_keys($terms[$vid]));
  }
  $max_depth = !isset($max_depth) ? count($children[$vid]) : $max_depth;
  $tree = array();

  // Keeps track of the parents we have to process, the last entry is used
  // for the next processing step.
  $process_parents = array();
  $process_parents[] = $parent;

  // Loops over the parent terms and adds its children to the tree array.
  // Uses a loop instead of a recursion, because it's more efficient.
  while (count($process_parents)) {
    $parent = array_pop($process_parents);

    // The number of parents determines the current depth.
    $depth = count($process_parents);
    if ($max_depth > $depth && !empty($children[$vid][$parent])) {
      $has_children = FALSE;
      $child = current($children[$vid][$parent]);
      do {
        if (empty($child)) {
          break;
        }
        $term = $load_entities ? $term_entities[$child] : $terms[$vid][$child];
        if (isset($parents[$vid][$term->tid])) {

          // Clone the term so that the depth attribute remains correct
          // in the event of multiple parents.
          $term = clone $term;
        }
        $term->depth = $depth;
        unset($term->parent);
        $term->parents = $parents[$vid][$term->tid];
        $tree[] = $term;
        if (!empty($children[$vid][$term->tid])) {
          $has_children = TRUE;

          // We have to continue with this parent later.
          $process_parents[] = $parent;

          // Use the current term as parent for the next iteration.
          $process_parents[] = $term->tid;

          // Reset pointers for child lists because we step in there more often
          // with multi parents.
          reset($children[$vid][$term->tid]);

          // Move pointer so that we get the correct term the next time.
          next($children[$vid][$parent]);
          break;
        }
      } while ($child = next($children[$vid][$parent]));
      if (!$has_children) {

        // We processed all terms in this hierarchy-level, reset pointer
        // so that this function works the next time it gets called.
        reset($children[$vid][$parent]);
      }
    }
  }
  return $tree;
}

/**
 * Wrapper functions for variable_get() and variable_set() that make them multi-domain-aware.
 */

function sirius_domain_variable_get($name, $default = NULL, $domain_nid = NULL) {
  return variable_get(sirius_domain_variable_name($name, $domain_nid), $default);
}

function sirius_domain_variable_set($name, $value) {
  return variable_set(sirius_domain_variable_name($name), $value);
}

function sirius_domain_variable_name($name, $domain_nid = NULL) {
  if ($domain_nid === NULL) { $domain_nid = sirius_domain_nid(); }
  if ($domain_nid > 0) { return $domain_nid . '/' . $name; } else { return $name; }
}


function sirius_domain_restrict_taxonomy_fields(&$form, $form_id) {
  if ($form['#entity'] && $form['entity']->field_sirius_domain['und'][0]['target_id']) {
    $domain_nid = $form['entity']->field_sirius_domain['und'][0]['target_id'];
  } else {
    $domain_nid = sirius_domain_nid();
  }

  if (!$domain_nid) { return; }

  foreach ($form as $field_name => &$form_field) {
    if (!preg_match('/^field_/', $field_name)) { continue; }
    $info = field_info_field($field_name);
    if ($info['type'] != 'taxonomy_term_reference') { continue; }
    $vocabulary_machine_name = $info['settings']['allowed_values'][0]['vocabulary'];
    if (!$vocabulary_machine_name) { continue; }
    $options = sirius_taxonomy_load($vocabulary_machine_name, 'tid', 'name') + array('_none' => '- None -');

    // @todo there has to be an easier way.
    if ($form_field['und']['#options']) {
      $form_field['und']['#options'] = array_intersect($form_field['und']['#options'], $options);
    }
    if ($form_field['und'][0]['tid']['#options']) {
      $form_field['und'][0]['tid']['#options'] = array_intersect($form_field['und'][0]['tid']['#options'], $options);
    }
    if ($form_field['und'][1]['tid']['#options']) {
      $form_field['und'][1]['tid']['#options'] = array_intersect($form_field['und'][1]['tid']['#options'], $options);
    }
  }
}

function sirius_domain_form_alter_node(&$form, &$form_state, $form_id) {
  // We only care about nodes with a domain field.
  if (!$form['field_sirius_domain']) { return; }

  // If this is a new node, set the default value for the domain.
  $node = $form['#node'];
  if (!$node->nid) {
    // Accept a default domain from the query. But only if we have access to it.
    $default_domain_nid = $_GET['sirius_domain'];
    if ($default_domain_nid) {
      if (!sirius_domain_access($default_domain_nid)) {
        $form['#access'] = FALSE;
        drupal_set_message("You do not have access to that domain.", 'error');
        return;
      }
    }

    // Otherwise, the default value is our current domain.
    if (!$default_domain_nid) { 
      $default_domain_nid = sirius_domain_nid();
    }

    // Set the default value in the form.
    if ($default_domain_nid) { 
      $form['field_sirius_domain']['und']['#default_value'][0] = $default_domain_nid; 
    }
  }

  // Restrict the list of available domains to those that we have access to.
  $tree = array();
  $my_root_node = sirius_domain_root();
  if ($my_root_node) { 
    $my_root_nid = $my_root_node->nid;
    $tree = array($my_root_nid => $my_root_node->title);
  } else {
    $my_root_nid = NULL;
    $tree = array('_none' => '(root)');
  }
  $tree += sirius_domain_tree_render_as_select($my_root_nid, 1);
  $form['field_sirius_domain']['und']['#options'] = $tree;

  // And, finally ... hide this field from anybody without domain admin permission
  if (!user_access('sirius domain admin')) { $form['field_sirius_domain']['#access'] = FALSE; }
}

function sirius_domain_form_restrict(&$form) {
  $field_names = array(
    'field_grievance_shop_target_id' => NULL,
    'field_grievance_settlement_tags_tid' => 'grievance_settlement_tags',
    'field_sirius_member_status_tid' => 'sirius_member_status',
    'field_sirius_industry_tid' => 'sirius_industry',
    'field_sirius_skill_tid' => 'sirius_skill',
    'field_sirius_dispatch_sib_status_tid' => 'sirius_dispatch_sib_status',
    'field_sirius_worker_dispstatus_tid' => 'sirius_worker_dispatch_status',
    'field_sirius_work_status_tid' => 'sirius_work_status',
    'field_grievance_from_status' => 'grievance_status',
    'field_grievance_next_status' => 'grievance_status',
  );

  foreach ($field_names as $field_name => $taxonomy) {
    if (!$form[$field_name]['#options']) { continue; }
    $keys = array_keys($form[$field_name]['#options']);

    if ($field_name == 'field_grievance_shop_target_id') { $good_options = sirius_employer_list(FALSE); } 
    else { $good_options = sirius_taxonomy_load($taxonomy, 'tid', 'name'); }

    foreach ($keys as $key) {
      if ($key == NULL) { continue; }
      if ($key == 'All') { continue; }
      if ($good_options[$key]) { continue; }
      unset($form[$field_name]['#options'][$key]);
    }
  }
}

function sirius_domain_activity_ts_ranges() {
	return array(
    'hour'      => array('label' => 'Last Hour',      'ts' => strtotime('-1 hour')),
		'day' 			=> array('label' => 'Last 24 Hours', 	'ts' => strtotime('-1 day')),
		'week' 			=> array('label' => 'Last 7 days', 		'ts' => strtotime('-7 days')),
		'month' 		=> array('label' => 'Last 30 days', 	'ts' => strtotime('-30 days')),
		'all_time' 	=> array('label' => 'All Time', 			'ts' => 0),
	);
}
function sirius_domain_activity_render($domain_nids) {
	// Get the node types
	$sql = "select type, name from node_type";
	$stmt = sirius_sql_query($sql, array());
	while ($hr = $stmt->fetchAssoc()) {
		$type = $hr['type'];
		$name = $hr['name'];
		$name = preg_replace('/Sirius /', '', $name);
		$name = preg_replace('/Grievance /', '', $name);
		$type_names[$type] = $name;
	}
	asort($type_names);

	// New nodes
	$ts_ranges = sirius_domain_activity_ts_ranges();

	$node_counts = array();
	$found_node_types = array();
	foreach ($ts_ranges as $range_id => $range) {
		$sql_args = array();
		$sql = "select type, count(*) as c from node ";
		$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
		$sql .= "where field_sirius_domain_target_id in (:domain_nids) ";
		$sql_args[':domain_nids'] = $domain_nids;
		$sql .= "and created > :ts ";
		$sql_args[':ts'] = $range['ts'];
		$sql .= "group by node.type order by node.type ";
		$stmt = sirius_sql_query($sql, $sql_args);
		while ($hr = $stmt->fetchAssoc()) {
			$node_counts[$range_id][$hr['type']] = $hr['c'];
			$found_node_types[$hr['type']] = TRUE;
		}
	}

	$header = array('Type');
	foreach ($ts_ranges as $range_id => $range) { $header[] = $range['label']; }

	$rows = array();
	foreach ($type_names as $type => $type_name) {
		if (!$found_node_types[$type]) { continue; }
		$row = array();
		$row[] = $type_name;
		foreach ($ts_ranges as $range_id => $range) {
			$row[] = $node_counts[$range_id][$type];
		}
		$rows[] = $row;
	}

	$html = '<h2>Content</h2>';
	$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));

	return $html;
}

function sirius_domain_util_migrate_root_to_domain($domain_nid) {
  $domain_node = node_load($domain_nid);
  if (!$domain_node) { die("Domain not found: $domain_nid"); }
  if (!$domain_node->type == 'sirius_domain') { die("Not a domain node: $domain_nid [" . $domain_node->title . "]"); }

  // Get the list of node types that have a domain.
  $info = field_info_field('field_sirius_domain');
  $taxonomies = $info['bundles']['taxonomy_term'];
  $content_types = $info['bundles']['node'];

  // Get this list of nodes
  $node_counts = array();
  foreach ($content_types as $bundle) {
    if ($bundle == 'sirius_domain') { continue; }
    $sql_args = array(':bundle' => $bundle);
    $sql = "select type, nid, title from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "where field_sirius_domain_target_id is null ";
    $sql .= "and type = :bundle ";
    $sql .= "order by nid ";
    $sql .= "limit 1000 ";
    $types = array();
    $stmt = sirius_sql_query($sql, $sql_args);
    $count = 0;
    while ($hr = $stmt->fetchAssoc()) {
      $node = node_load($hr['nid']);
      $node->field_sirius_domain['und'][0]['target_id'] = $domain_nid;
      node_save($node);
      print "Saved: $hr[nid] - " . $node->title . "\n";
      ++$count;
    }
    $node_counts[$bundle] = $count;
  }

  // Get this list of nodes
  $term_counts = array();
  foreach ($taxonomies as $bundle) {
    $sql_args = array(':bundle' => $bundle);
    $sql = "select tid, taxonomy_term_data.name from taxonomy_term_data ";
    $sql .= "left join taxonomy_vocabulary on taxonomy_vocabulary.vid = taxonomy_term_data.vid ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'taxonomy_term' and field_data_field_sirius_domain.entity_id = tid ";
    $sql .= "where field_sirius_domain_target_id is null ";
    $sql .= "and taxonomy_vocabulary.machine_name = :bundle ";
    $sql .= "order by tid ";
    $sql .= "limit 1000 ";
    $types = array();
    $stmt = db_query($sql, $sql_args);
    $count = 0;
    while ($hr = $stmt->fetchAssoc()) {
      $term = taxonomy_term_load($hr['tid']);
      $term->field_sirius_domain['und'][0]['target_id'] = $domain_nid;
      taxonomy_term_save($term);
      print "Saved: $hr[tid] - " . $term->title . "\n";
      ++$count;
    }
    $term_counts[$bundle] = $count;
  }

  // Get this list of users
  $sql_args = array(':bundle' => $bundle);
  $sql = "select uid, name from users ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'user' and field_data_field_sirius_domain.entity_id = uid ";
  $sql .= "where field_sirius_domain_target_id is null ";
  $sql .= 'and uid > 1 ';
  $sql .= "order by uid ";
  $sql .= "limit 1000 ";
  $types = array();
  $stmt = db_query($sql, $sql_args);
  $count = 0;
  while ($hr = $stmt->fetchAssoc()) {
    $u = user_load($hr['uid']);
    print "Saving: " . $u->uid . " / " . $u->name . "\n";
    user_save($u, array('field_sirius_domain' => array('und' => array(0 => array('target_id' => $domain_nid)))));
    print "Saved: $hr[uid] - " . $u->name . "\n";
    ++$user_count;
  }

  // Get the settings
  $sql = "select name from variable where name like 'sirius%' or name like 'grievance%' ";
  $stmt = db_query($sql, array());
  while ($hr = $stmt->fetchAssoc()) {
    $name = $hr['name'];
    $val = variable_get($name);
    sirius_domain_variable_set("$name/$domain_nid", $val);
    print "Set: $name\n";
    ++$variable_count;
  }
  print "Node counts: ";
  print_r($node_counts);

  print "Term counts: ";
  print_r($term_counts);

  print "User count: $user_count\n";
  print "Variable count: $variable_count\n";
}


function sirius_domain_usermanage_roles() {
  $result = array(
    'all' => user_roles(),
    'omit' => array(),
    'optional' => array(),
    'required' => array()
  );
  foreach ($result['all'] as $rid => $role) {
    $avail = variable_get("sirius_domain_userrmanage_$rid", 'omit');
    $result[$avail][$rid] = $role;
  }
  return $result;
}