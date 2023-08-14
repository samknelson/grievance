<?php

/**
 * Implements hook_node_update
 */

function sirius_sched_node_update($node) { return sirius_sched_denorm_queue($node); }

/**
 * Implements hook_node_insert
 */

function sirius_sched_node_insert($node) { return sirius_sched_denorm_queue($node); }

/**
 * Implements hook_node_delete
 */

function sirius_sched_node_delete($node) { return sirius_sched_denorm_queue($node); }

/**
 * Whenever a node is saved, we need to decide if it's going to generate a sched entry.
 * 
 * We do this by looking up all the plugins, and asking each if it's interested in this node.
 */

function sirius_sched_denorm_queue($node) {
	$infos = sirius_sched_info_all();

	$needs_queue = FALSE; 
	foreach ($infos as $key => $info) {
		if (!$info['callback_denorm_check']) { continue; }
		if ($info['callback_denorm_check']($node)) {
			$needs_queue = TRUE;
			break;
		}
	}

	if (!$needs_queue) { return; }
  $denorm_id = sirius_denorm_queue_add($node->nid, $node->nid, 'sirius_sched');

  // Debug -- denorm runs after page load, so it's a pain to debug. 
  $items = sirius_denorm_queue_search(array('denorm_id' => $denorm_id));
  $item = reset($items);
  sirius_sched_sirius_denorm($item);
}

/**
 * Implements hook_sirius_denorm
 * 
 * If we're being called with our denorm trigger, then we iterate through all the plugins
 * and, if any of them are interested in this node, we allow that plugin to create 
 * schedule entries from it.
 */

function sirius_sched_sirius_denorm($item) {
	// Only interested in our trigger
	if ($item['denorm_trigger'] != 'sirius_sched') { return; }

  $handler_node = node_load($item['denorm_source_nid']);
  if (!$handler_node) { return; }

	$infos = sirius_sched_info_all();
	foreach ($infos as $key => $info) {
		if (!$info['callback_denorm_check']) { continue; }
		if (!$info['callback_denorm']) { continue; }

		if (!$info['callback_denorm_check']($handler_node)) { continue; }

		$scheds = $info['callback_denorm']($handler_node);
		if (!$scheds) { $scheds = array(); }
		sirius_sched_db_delete(array('handler_nid' => $handler_node->nid));
	  foreach ($scheds as $sched) {
	  	$sched['type'] = $key; 
	  	$sched['handler_nid'] = $handler_node->nid;
	  	$sched['title'] = $handler_node->title;
	    sirius_sched_db_insert($sched);
	  }
	}
}
