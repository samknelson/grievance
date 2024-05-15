<?php

function sirius_cardcheck_node_tab_worker_access($worker_node) {
	$access = Sirius_Cardcheck_Access::getInstance();
	return $access->access("list worker cardchecks", array('worker_node' => $worker_node));
}

function sirius_cardcheck_node_tab_worker($worker_node) {
	$access = Sirius_Cardcheck_Access::getInstance();
	$instance = Sirius_Cardcheck::getInstance();

	$action = sirius_node_tab_arg(0);

	// Handle "create"
	if ($action == 'create') {
		$definition_nid = sirius_node_tab_arg(1);
		$definition_node = node_load($definition_nid);
		$context = array('worker_node' => $worker_node, 'definition_node' => $definition_node);
		if (!$access->access('create worker cardcheck', $context)) {
			return sirius_page_error("You do not have permission to create this card check record.");
		}
		$cardcheck_node = $instance->create($definition_node, array('entity_node' => $worker_node));
		drupal_goto("node/" . $cardcheck_node->nid . "/sirius_log_cardcheck");
	}
	if ($action) { return sirius_page_error("Unknown action."); }

	$cardcheck_nodes = $instance->search(array('worker_nid' => $worker_node->nid));

  if ($cardcheck_nodes) {
	  $rows = array();
	  $status_options = $instance->status_options();
  	foreach ($cardcheck_nodes as $cardcheck_nid => $cardcheck_node) {
  		$cardcheck_json = sirius_json_get($cardcheck_node);
  		$cardcheck = $cardcheck_json['cardcheck'];
  		$acceptance = $cardcheck['acceptance'];
  		$revocation = $cardcheck['revocation'];

  		$notes = array();
  		if ($acceptance['ts']) {
  			$notes[] = 'Accepted: ' . date('l, F j, g:i A', $acceptance['ts']);
  		}
  		if ($revocation['ts']) {
  			$notes[] = 'Revoked: ' . date('l, F j, g:i A', $revocation['ts']);
  		}

  		$definition_node = sirius_log_handler($cardcheck_node, 'sirius_json_definition');
  		$row = array();
  		$status = $instance->status($cardcheck_node);
  		$tools = array();
  		$tools[] = l('View', 'node/' . $cardcheck_node->nid . '/sirius_log_cardcheck', array('attributes' => array('class' => array('tool-button'))));
  		$row[] = $definition_node->title;
  		$row[] = $status_options[$status];
  		$row[] = join('<br />', $notes);
  		$row[] = join('', $tools);
  		$rows[] = array('class' => array($instance->status_color($status)), 'data' => $row);
  	}

  	$header = array();
  	$header[] = t('Card Check');
  	$header[] = t('Status');
  	$header[] = t('Notes');
  	$header[] = t('Tools');

  	$html .= sirius_table_render($header, $rows);
  }

	$definition_nodes = $instance->definition_list();
	foreach ($definition_nodes as $definition_nid => $definition_node) {
		$context = array('worker_node' => $worker_node, 'definition_node' => $definition_node);
		if (!$access->access('create worker cardcheck', $context)) { unset($definition_nodes[$definition_nid]); }
	}
	if ($definition_nodes) {
		$html .= '<h2>' . t('Sign a Card Check') . '</h2>';
		foreach ($definition_nodes as $definition_nid => $definition_node) {
			$html .= l(
				$definition_node->title,
				current_path() . '/create/' . $definition_nid, 
				array('attributes' => array('class' => array('tool-button')))
			);
		}
	}

	if (!$html) {
		return sirius_page_error("There are no signed card checks and no card checks available to sign.");
	}

	return $html;
}