<?php

/**
 * Implemented in a hook_menu_alter to allow public access to any node that's set to public, no matter what.
 */

function sirius_public_node_access($node) {
	if ($node->field_sirius_public['und'][0]['value'] == 'Yes') { return TRUE; }
	return node_access('view', $node);
}

/**
 * Hide the "is public" field for non-admins; for admins, make it render a little more attractively.
 */

function sirius_public_node_view($node, $view_mode, $langcode) {
	if (!isset($node->field_sirius_public)) { return; }
	if (!isset($node->content['field_sirius_public'])) { return; }

	if (!user_access('sirius public admin')) {
		$$html = '';
	} else {
	 	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/public_admin.css');

	 	if ($node->field_sirius_public['und'][0]['value'] == 'Yes') {
			$html = '<div class="sirius_public_admin_note">';
	 		$html .= "This page is public. It can be viewed by anonymous users.";
	 		$html .= '</div>';
	 	} else {
			$html .= '';
		}
	}

	$node->content['field_sirius_public'] = array('#markup' => $html);
}

/*
function sirius_page_public_access() {
	// We should be doing this. But menu_get_item() from within a _access causes an infinite loop. 
	$possible = sirius_public_possible(sirius_page_public_arguments());
	if (!$possible['sucess']) { 
		drupal_set_message($possible['msg']);
		return FALSE;
	}

	return TRUE;
}

function sirius_page_public() {
 	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/public.css');

 	$arguments = sirius_page_public_arguments();
	$possible = sirius_public_possible($arguments);
	if (!$possible['success']) { 
		drupal_set_title("Public Page Access Denied");
		drupal_set_message($possible['msg'], 'error');
		return 'Access denied.';
	}

	list ($domain_nid, $content_nid, $tab_name) = $arguments;
	$content_node = node_load($content_nid);
	$content_node->sirius_fullpage = TRUE; 
	$render = node_view($content_node, 'default');
	drupal_set_title($content_node->title);
	return drupal_render($render);
}

function sirius_public_possible($arguments) {
	if (!$arguments) { return array('success' => FALSE, 'msg' => 'No arguments passed to the page.'); }
	list ($domain_nid, $content_nid, $tab_name) = $arguments;
	if (!$domain_nid || !$content_nid) { return array('success' => FALSE, 'msg' => 'Please pass in a domain NID and a content NID.'); }

	$domain_node = node_load($domain_nid);
	if (!$domain_node || ($domain_node->type != 'sirius_domain')) { return array('success' => FALSE, 'msg' => "The domain NID [$domain_nid] does not correspond to a valid Sirius domain."); }

	if (!sirius_domain_component_enabled('public', $domain_node)) { return array('success' => FALSE, 'msg' => "The domain does not have the \"public\" component enabled."); }

	$content_node = node_load($content_nid);
	if ($content_node->field_sirius_domain['und'][0]['target_id'] != $domain_nid) { return array('success' => FALSE, 'msg' => "The content node [$content_nid] is not in the domain [$domain_nid]"); }

	if ($content_node->field_sirius_public['und'][0]['value'] != 'Yes') { return array('success' => FALSE, 'msg' => "The content node [$content_nid] does not have public access."); }

	if ($content_node->type == 'grievance_basic_page') {
		return array('success' => TRUE);
	} else {
		return array('success' => FALSE, 'msg' => "This page type is not public.");
	}

}

function sirius_page_public_denied($msg) {
	return "Access denied: $msg";
}

function sirius_page_public_arguments() {
  $item = &drupal_static(__FUNCTION__, NULL);
  if (!isset($item)) { $item = menu_get_item(); } 
  if (!$item) { return array(); }
  return $item['page_arguments'];
}
*/