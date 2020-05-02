<?php

function sirius_node_access_news($log_node, $op, $account) {
  // Check admin permissions
  if (user_access('sirius news admin', $account)) { return NODE_ACCESS_ALLOW; }

  return NODE_ACCESS_DENY;
}

function sirius_news_popup_block_view() {
	global $user;
	if (!$user) { return NULL; }
	if (!$user->roles) { return NULL; }

  // Find any pending news announcements good for a popup
  $domain = sirius_domain();
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_news');
  $query->fieldCondition('field_sirius_roles', 'value', array_keys($user->roles), 'in');
  $query->fieldCondition('field_sirius_boolean', 'value', 'yes');
  $query->fieldCondition('field_sirius_datetime_completed', 'value', date('Y-m-d H:i:s'), '>=');
  if ($domain) {
	  $query->fieldCondition('field_sirius_domain', 'target_id', $domain->nid);
  }
  $query->propertyOrderBy('created', 'desc');
  $stmt = $query->execute();
  if (!$stmt['node']) { return NULL; }
  $news_nids = array_keys($stmt['node']);

  foreach ($news_nids as $news_nid) {
  	$category = 'news:view';
  	$type = 'popup';
	  $notes = $news_nid;

  	// Search the log for an indication that we've already viewed this news item.
		$query = new EntityFieldQuery();
	  $query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'sirius_log');
	  $query->fieldCondition('field_sirius_log_handler', 'target_id', $user->uid);
	  // @todo: should be checking handler, but EntityFieldQuery can't check multiple values at the same time.
	  $query->fieldCondition('field_sirius_notes', 'value', $notes); 
	  $query->fieldCondition('field_sirius_category', 'value', $category);
	  $query->fieldCondition('field_sirius_type', 'value', $type);
	  $stmt = $query->execute();
	  if ($stmt['node']) { continue; }

	  // Didn't find it.  Load the news item.
	  $news_node = node_load($news_nid);

	  // Log it
	  $message = 'User ' . $user->name . ' viewed popup news announcement: ' . $news_node->title;
	  $summary = '';
	  $handler_nids = array($user->uid, $news_node->nid);
	  $files = array();
		sirius_log($category, $type, $message, $summary, $notes, $handler_nids, $files);

		// Popup it up.
		drupal_add_js(drupal_get_path('module', 'sirius') . '/js/sirius.news_popup.js');
		drupal_add_css(drupal_get_path('module', 'sirius') . '/css/sirius.news_popup.css');
		$settings_for_js = array(
	  	'width' => 300,
	    'height' => 500,
	    'delay' => 10,
		);
		drupal_add_js(array('sirius_news_popup' => $settings_for_js), 'setting');
	  $html = '<div style="display:none" id="popup-announcement-overlay"></div>';
		$html .= '<div style="display:none" id="popup-announcement-wrap">';
		$html .= '<div id="popup-announcement">';
		$renderable = node_view($news_node, 'default');
		$html .= drupal_render($renderable);
		$html .= '</div>';
		$html .= '<div id="popup-announcement-close"></div>';
		$html .= '</div>';

		// Return the block.  (We only show one item per page.)
		$block = array();
	  $block['subject'] = '';
	  $block['content'] = $html;
	  return $block;
	}
}