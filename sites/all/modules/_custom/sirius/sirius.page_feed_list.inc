<?php

function sirius_feed_page_list_access($wizard_name) {
	// Must be a legitimate feed
	$info = sirius_wizard_info($wizard_name);
	if (!$info) { return FALSE; }

	// Admins can access
	if (user_access('sirius admin')) { return TRUE; }
	if (user_access('sirius domain admin')) { return TRUE; }

	// @todo should be in a callback
	if ($info['feed']['employer_access'] && user_access('sirius employer user manager')) { 
		$employer_node = sirius_employer_get();
		if (!$employer_node) { return FALSE; }
		return TRUE;
	}

	return FALSE;
}

function sirius_feed_page_list($wizard_name) {
	sirius_backlink_add('sirius/feeds', array('name' => 'All Feeds'));

	$domain_nid = sirius_domain_nid();
	$info = sirius_wizard_info($wizard_name);
	if (!$info) { return "No such wizard."; }

	drupal_set_title("Feed list: " . $info['title']);

	// Is there a callback?
	$callback = $info['feed']['callback_list'];
	if ($callback) {
		return $callback($wizard_name);
	}

  $domain_nid = sirius_domain_nid();
  $info = sirius_wizard_info($wizard_name);
  if (!$info) { return "No such wizard."; }

  $employer = sirius_employer_get();

  $page_size = 50;
  $page = pager_find_page();
  $offset = $page_size * $page;

  $sql_count = "select count(*) as c ";
  $sql_fields = "select node.nid, node.created, field_sirius_feed_status_value, field_sirius_json_value, field_grievance_shop_target_id, shop_node.title as shop_title ";
  $sql = "from node ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_feed_status on field_data_field_sirius_feed_status.entity_type = 'node' and field_data_field_sirius_feed_status.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
  $sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid ";
  $sql .= "left join node shop_node on shop_node.nid = field_grievance_shop_target_id ";
  $sql .= "where node.type = 'sirius_feed' ";
  $sql .= "and field_sirius_domain_target_id = :domain_nid ";
  $sql_args[':domain_nid'] = $domain_nid;
  $sql .= "and field_sirius_type_value = :wizard_name ";
  $sql_args[':wizard_name'] = $wizard_name;
  if ($employer) {
    $sql .= "and field_grievance_shop_target_id = :employer_nid ";
    $sql_args[':employer_nid'] = $employer->nid;
  }
  $sql .= "order by node.created desc ";
  $sql_limit = "limit $page_size offset $offset ";

  $stmt = sirius_sql_query($sql_count . $sql, $sql_args);
  $hr = $stmt->fetchAssoc();
  $count = $hr['c'];
  $pager = pager_default_initialize($count, $page_size);

  $stmt = sirius_sql_query($sql_fields . $sql . $sql_limit, $sql_args);
  $rows = array();
  while ($hr = $stmt->fetchAssoc()) {
    $json = json_decode($hr['field_sirius_json_value'], TRUE);

    // Source is:
    // - The source field from the JSON (if it's not auto-generated)
    // - Followed by the original file name (if any)
    $source = '';
    if (!preg_match('/^Import \d\d\d\d/', $json['wizard'][$wizard_name]['source'])) {
      $source .= $json['wizard'][$wizard_name]['source'];
    }
    if ($json['wizard'][$wizard_name]['origname']) {
      if ($source) { $source .= ' / '; }
      $source .= $json['wizard'][$wizard_name]['origname'];
    }

    $row = array();
    $row[] = date('l, F j, Y h:i a', $hr['created']);
    $row[] = $hr['field_sirius_feed_status_value'];
    $row[] = $source;
    if ($info['employer_access']) {
    	$row[] = $hr['shop_title'];
    }
    $row[] = l('Wizard', 'node/' . $hr['nid'] . '/sirius_feed_wizard', array('attributes' => array('class' => array('tool-button'))));
    $rows[] = $row;
  }

  $header = array();
  $header[] = t('Date');
  $header[] = t('Status');
  $header[] = t('Source');
  if ($info['employer_access']) { $header[] = t('Shop'); }
  $header[] = t('Tools');

  $html .= l('Add a New Entry', 'sirius/feeds/launch/' . $wizard_name, array('attributes' => array('class' => array('tool-button', 'action-button'))));

  $html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));

  $html .= theme('pager');

  return $html;

  /*
	// Oh well, no callback. In that case we just hardcode some view.
	if (user_access('sirius admin') || user_access('sirius domain admin')) {
		// admins see the full list
		$html .= views_embed_view('sirius_feeds', 'block_2', $wizard_name, $domain_nid);
	} else {
		// Non-admins see a filtered list
		$employer_node = sirius_employer_get();
		if (!$employer_node) { return 'No employer set for this user.'; }
		$html .= views_embed_view('sirius_feeds', 'block_1', $wizard_name, $domain_nid, $employer_node->nid);
	}
	*/

	return $html;
}
