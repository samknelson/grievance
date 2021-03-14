<?php

function sirius_ledger_auth_entities($auth_node, $content_type = NULL) {
	$handlers = node_load_multiple(sirius_fieldvals($auth_node, 'field_sirius_log_handler', 'target_id'));
	if ($content_type) {
		$handlers = array_filter($handlers, function($handler_node) use ($content_type) {
			return $handler_node->type == $content_type;
		});
	}

	return $handlers;
}

function sirius_ledger_auth_entities_set($auth_node, $content_type, $new) {
	$current_all = sirius_fieldvals($auth_node, 'field_sirius_log_handler', 'target_id');
	$current_bytype = array_keys(sirius_ledger_auth_entities($auth_node, $content_type));

	$current = array_diff($current_all, $current_bytype);
	$current = array_merge($current, $new);

	$val = array();
	foreach ($current as $target_id) { $val[] = array('target_id' => $target_id); }
	if ($val) {
		$auth_node->field_sirius_log_handler['und'] = $val;
	} else {
		unset($auth_node->field_sirius_log_handler['und']);
	}
	node_save($auth_node);
}

function sirius_ledger_auth_items($auth_node) {
	$json = sirius_jsonfield_util_node_json_get($auth_node);
	if (!$json || !$json['auth']['items']) { return array(); }
	return $json['auth']['items'];
}

function sirius_ledger_auth_items_set($auth_node, $items) {
	$cols = sirius_ledger_auth_item_cols($auth_node);

  if (!$items) { $items = array(); }
  if ($cols['price']) {
	  foreach ($items as $delta => $item) {
	    $q = $item['qty'];
	    if (!$q) { $q = $item['hours']; }
	    if (!$q) { $q = 0; }
	    $p = $item['price'];
	    if (!$p) { $p = 0; }
	    $items[$delta]['amt'] = $q * $p;
	  }
	}

	foreach ($items as $delta => $item) {
		if (!array_filter(array_values($item))) { unset($items[$delta]); }
	}
	$json = sirius_jsonfield_util_node_json_get($auth_node);
	$json['auth']['items'] = $items;


	$accounts = sirius_ledger_auth_entities($auth_node, 'sirius_ledger_account');
	$total = array();
  foreach ($accounts as $account_nid => $account_node) {
  	$totals[$account_nid] = array('count' => 0, 'qty' => 0, 'hours' => 0, 'amt' => 0);
  }
  $total = array('count' => 0, 'qty' => 0, 'hours' => 0, 'amt' => 0);
  foreach ($items as $delta => $item) {
    if ($item['qty']) { $total['qty'] += $item['qty']; }
    if ($item['hours']) { $total['hours'] += $item['hours']; }
    if ($item['amt']) { $total['amt'] += $item['amt']; }
    $total['count']++;

    $account_nid = $item['account_nid'];
    if ($account_nid) {
	    if ($item['qty']) { $totals[$account_nid]['qty'] += $item['qty']; }
	    if ($item['hours']) { $totals[$account_nid]['hours'] += $item['hours']; }
	    if ($item['amt']) { $totals[$account_nid]['amt'] += $item['amt']; }
	    $totals[$account_nid]['count']++;
    }
  }
  $json['auth']['items_total'] = $total;
  $json['auth']['items_total_byaccount'] = $totals;
	sirius_jsonfield_util_node_json_set($auth_node, $json);
	node_save($auth_node);
	return;
}

function sirius_ledger_auth_set_title(&$auth_node) {
	$auth_json = sirius_jsonfield_util_node_json_get($auth_node);
	$auth_title = $auth_json['auth']['settings']['title'];
	if (!$auth_title) { $auth_title = 'Authorization Form'; }

	$entity_nodes = sirius_ledger_auth_entities($auth_node, 'sirius_event');
	if (!$entity_nodes) { $entity_nodes = sirius_ledger_auth_entities($auth_node, 'sirius_ledger_account'); }
	if ($entity_nodes) {
		$first_entity_node = reset($entity_nodes);
		$title .= $first_entity_node->title . ' - ' . $auth_title;
	}
	if (!$title) { $title = $auth_title; }
	$title .= ' - ' . date('Y-m-d', $auth_node->created);

	if ($title != $auth_node->title) {
		$auth_node->title = substr($title, 0, 255);
		node_save($auth_node);
	}
}

function sirius_ledger_auth_item_cols($auth_node) {
	$cols_all = sirius_ledger_auth_item_cols_all();
  $auth_json = sirius_jsonfield_util_node_json_get($auth_node);
  $cols = $auth_json['auth']['settings']['items']['cols'];
  if (!$cols) { $cols = array('date', 'title', 'hours', 'account_nid'); }
  $cols = array_filter(array_values($cols));
  $x = array();
  foreach ($cols as $col) { $x[$col] = $cols_all[$col]; }
  return $x;
}

function sirius_ledger_auth_item_cols_all() {
	return array(
    'date' => t('Date'),
    'title' => t('Item'),
    'hours' => t('Hours'),
    'qty' => t('Quantity'),
    'price' => t('Price'),
    'amt' => t('Amount'),
    'account_nid' => t('Grant'),
  );
}

function sirius_ledger_auth_render_items($auth_node) {
	$account_names = sirius_ledger_account_list();
	$items = sirius_ledger_auth_items($auth_node);
  $auth_json = sirius_jsonfield_util_node_json_get($auth_node);

  $cols = sirius_ledger_auth_item_cols($auth_node);
  $cols_all = sirius_ledger_auth_item_cols_all($auth_node);

  $header = array();
  foreach ($cols_all as $key => $value) {
    if (!$cols[$key]) { continue; }
  	$header[] = $value; 
  }

  $rows = array();
  foreach ($items as $item) {
  	$row = array();
    if ($cols['date']) { $row[] = $item['date']; }
    if ($cols['title']) { $row[] = $item['title']; }
    if ($cols['hours']) { $row[] = sirius_number_format($item['hours'], 2); }
    if ($cols['qty']) { $row[] = sirius_number_format($item['qty'], 2); }
    if ($cols['price']) { $row[] = '$' . sirius_number_format($item['price'], 2); }
    if ($cols['amt']) { $row[] = '$' . sirius_number_format($item['amt'], 2); }
    if ($cols['account_nid']) { $row[] = $account_names[$item['account_nid']]; }
    $rows[] = $row;
	}

	$account_names = sirius_ledger_account_list();
	$account_nids = sirius_ledger_auth_entities($auth_node, 'sirius_ledger_account');

	if ($cols['qty'] || $cols['hours'] || $cols['amt']) {
		$row = array();
	  foreach ($cols_all as $key => $ignore) {
	    if (!$cols[$key]) { continue; }

	    $value = '';
	    foreach ($account_nids as $account_nid => $account_node) {
	      if ($key == 'title')      { $value .= $account_names[$account_nid] . '<br />'; }
	      else if ($key == 'qty')   { $value .= sirius_number_format($auth_json['auth']['items_total_byaccount'][$account_nid]['qty']) . '<br />'; }
	      else if ($key == 'hours') { $value .= sirius_number_format($auth_json['auth']['items_total_byaccount'][$account_nid]['hours'], 2) . '<br />'; }
	      else if ($key == 'amt')   { $value .= '$' . sirius_number_format($auth_json['auth']['items_total_byaccount'][$account_nid]['amt'], 2) . '<br />'; }
	      else { $value = '&nbsp;'; }
	    }
	    if ($key == 'title') { $value .= '<strong>Total</strong>'; }
	    else if ($key == 'qty') { $value .= '<strong>' . sirius_number_format($auth_json['auth']['items_total']['qty']) . '</strong>'; }
	    else if ($key == 'hours') { $value .= '<strong>' . sirius_number_format($auth_json['auth']['items_total']['hours'], 2) . '</strong>'; }
	    else if ($key == 'amt') { $value .= '<strong>$' . sirius_number_format($auth_json['auth']['items_total']['amt'], 2) . '</strong>'; }
	    else if ($key == 'date') { $value = '<strong>Total</strong>'; }
	    else { $value = '&nbsp;'; }

	    $row[] = $value;
	  }
	  $rows[] = $row;
	}

	return theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('sirius_data_table sirius_data_table_wide'))));
}