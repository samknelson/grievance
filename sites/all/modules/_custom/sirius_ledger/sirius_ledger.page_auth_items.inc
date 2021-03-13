<?php 

function sirius_ledger_node_tab_auth_items_access($auth_node) {
	if ($auth_node->field_sirius_category['und'][0]['value'] != 'ledger:auth') { return FALSE; }
	return TRUE;
}

function sirius_ledger_node_tab_auth_items($auth_node) {
	return drupal_get_form('sirius_ledger_node_tab_auth_items_form', $auth_node);
}

function sirius_ledger_node_tab_auth_items_form($form, &$form_state, $auth_node) {
  $esignature_status = sirius_esignature_entity_status($auth_node);
  if ($esignature_status['status'] != 'unsigned') {
    $form['intro'] = array('#markup' => '<p><strong>Because this authorization has already been signed, no changes can be made. (Signature status: ' . $esignature_status['status_msg'] . '.)</strong></p>');
  }

	form_load_include($form_state, 'inc', 'sirius_ledger', 'sirius_ledger.page_auth_items');
	$form_state['auth_nid'] = $auth_node->nid;

	$account_names = sirius_ledger_account_list();
	$account_nids = sirius_ledger_auth_entities($auth_node, 'sirius_ledger_account');
	$account_options = array(NULL => t('-- Select --'));
	foreach ($account_nids as $account_nid => $account_node) { $account_options[$account_nid] = $account_names[$account_nid]; }

  $auth_json = sirius_jsonfield_util_node_json_get($auth_node);

	$items = sirius_ledger_auth_items($auth_node);

  $ajax_params = $form_state['triggering_element']['#sirius_ajax'];
  $item_count = $form_state['item_count'];
  $item_count += $ajax_params['qty'];
  if ($item_count < 1) { $item_count = 1; }
  if ($item_count < count($items) + 1) { $item_count = count($items) + 1; }
  $form_state['item_count'] = $item_count;

  $form['#tree'] = TRUE;

  $cols = sirius_ledger_auth_item_cols($auth_node);
  $cols_all = sirius_ledger_auth_item_cols_all($auth_node);

  $form['items']['prefix'] = array('#markup' => '<div id="items">');

  $header_html = '<table class="sirius_data_table sirius_data_table_wide"><tr>';
  foreach ($cols_all as $key => $value) {
    if (!$cols[$key]) { continue; }
    $header_html .= '<th>' . $value . '</th>';
  }
  $header_html .= '</tr>';
  $form['items']['table_open'] = array('#markup' => $header_html);
	for ($i=0; $i<$item_count; ++$i) {
		$form['items'][$i]['tr_open'] = array('#markup' => '<tr>');

    if ($cols['date']) {
      $form['items'][$i]['date'] = array(
   			'#type' => 'date_popup',
   			'#date_format' => 'Y-m-d',
      	'#attributes' => array(
        	'autocomplete' => 'off',
      	),
        '#prefix' => '<td>',
        '#suffix' => '</td>',
    	);
    	if ($items[$i]['date']) { $form['items'][$i]['date']['#default_value'] = $items[$i]['date']; }
    }

    if ($cols['title']) {
      $form['items'][$i]['title'] = array(
        '#type' => 'textfield',
        '#prefix' => '<td>',
        '#suffix' => '</td>',
        '#default_value' => $items[$i]['title'],
      );
    }

    if ($cols['hours']) {
      $form['items'][$i]['hours'] = array(
        '#type' => 'textfield',
        '#prefix' => '<td>',
        '#suffix' => '</td>',
        '#size' => 5,
        '#default_value' => $items[$i]['hours'],
      );
    }

    if ($cols['qty']) {
      $form['items'][$i]['qty'] = array(
        '#type' => 'textfield',
        '#prefix' => '<td>',
        '#suffix' => '</td>',
        '#size' => 5,
        '#default_value' => $items[$i]['qty'],
      );
    }

    if ($cols['price']) {
      $form['items'][$i]['price'] = array(
        '#type' => 'textfield',
        '#prefix' => '<td style="white-space: nowrap;">',
        '#suffix' => '</td>',
        '#size' => 5,
        '#default_value' => $items[$i]['price'],
        '#field_prefix' => '$',
      );
    }

    if ($cols['amt']) {
      $form['items'][$i]['amt'] = array(
        '#type' => 'textfield',
        '#prefix' => '<td style="white-space: nowrap;">',
        '#suffix' => '</td>',
        '#size' => 5,
        '#default_value' => $items[$i]['amt'],
        '#field_prefix' => '$',
      );
      if ($cols['price']) { $form['items'][$i]['amt']['#disabled'] = TRUE; }
    }

    if ($cols['account_nid']) {
      $form['items'][$i]['account_nid'] = array(
        '#type' => 'select',
        '#prefix' => '<td>',
        '#suffix' => '</td>',
        '#options' => $account_options,
        '#default_value' => $items[$i]['account_nid'],
      );
    }

		$form['items'][$i]['tr_close'] = array('#markup' => '</tr>');
	}

  $footer_html = '<tr>';

  foreach ($cols_all as $key => $ignore) {
    if (!$cols[$key]) { continue; }
    $value = '';
    foreach ($account_nids as $account_nid => $account_node) {
      if ($key == 'title')      { $value .= $account_names[$account_nid] . '<br />'; }
      else if ($key == 'qty')   { $value .= number_format($auth_json['auth']['items_total_byaccount'][$account_nid]['qty']) . '<br />'; }
      else if ($key == 'hours') { $value .= number_format($auth_json['auth']['items_total_byaccount'][$account_nid]['hours'], 2) . '<br />'; }
      else if ($key == 'amt')   { $value .= '$' . number_format($auth_json['auth']['items_total_byaccount'][$account_nid]['amt'], 2) . '<br />'; }
      else { $value = '&nbsp;'; }
    }
    if ($key == 'title') { $value .= '<strong>Total</strong>'; }
    else if ($key == 'qty') { $value .= '<strong>' . number_format($auth_json['auth']['items_total']['qty']) . '</strong>'; }
    else if ($key == 'hours') { $value .= '<strong>' . number_format($auth_json['auth']['items_total']['hours'], 2) . '</strong>'; }
    else if ($key == 'amt') { $value .= '<strong>$' . number_format($auth_json['auth']['items_total']['amt'], 2) . '</strong>'; }
    else { $value = '&nbsp;'; }
    $footer_html .= '<td>' . $value . '</td>';
  }
  $footer_html .= '</tr>';
  $footer_html .= '</table>';

	$form['items']['table_close'] = array('#markup' => $footer_html);
  $form['items']['suffix'] = array('#markup' => '</div>');

  if ($esignature_status['status'] == 'unsigned') {
    $form['add_one'] = array(
      '#type' => 'button',
      '#value' => t('Add a Row'),
      '#ajax' => array(
        'callback' => 'sirius_ajax_generic_replace',
      ),
      '#sirius_ajax' => array(
        'qty' => 1,
        'wrapper' => '#items',
        'element' => 'items',
      ),
      '#suffix' => '<br clear="left">',
    );

    $form['cols'] = array(
      '#title' => 'Columns',
      '#type' => 'checkboxes',
      '#options' => $cols_all,
      '#default_value' => array_keys($cols),
      '#prefix' => "<div class=\"checkboxes-inline\">",
      '#suffix' => '</div><br clear="left" />',
    );
  
  	$form['submit'] = array(
  		'#type' => 'submit',
  		'#value' => t('Submit'),
  	);
  }

	return $form;
}

function sirius_ledger_node_tab_auth_items_form_validate($form, &$form_state) {
	if ($form_state['values']['items']) {
		foreach ($form_state['values']['items'] as $delta => $item) {
      if ($item['qty'] && !is_numeric($item['qty'])) {
        form_set_error('items][' . $delta . '][qty', 'Quantity must be a number.');
      }
      if ($item['amt'] && !is_numeric($item['amt'])) {
        form_set_error('items][' . $delta . '][amt', 'Amount must be a number.');
      }
      if ($item['price'] && !is_numeric($item['price'])) {
        form_set_error('items][' . $delta . '][price', 'Price must be a number.');
      }
      if ($item['hours'] && !is_numeric($item['hours'])) {
        form_set_error('items][' . $delta . '][hours', 'Hours must be a number.');
      }
		}
	}
}

function sirius_ledger_node_tab_auth_items_form_submit($form, &$form_state) {
	$auth_node = node_load($form_state['auth_nid']);

  $items = $form_state['values']['items'];
	sirius_ledger_auth_items_set($auth_node, $items);
  $auth_json = sirius_jsonfield_util_node_json_get($auth_node);
  $auth_json['auth']['settings']['items']['cols'] = $form_state['values']['cols'];
  sirius_jsonfield_util_node_json_set($auth_node, $auth_json);
  node_save($auth_node);
  sirius_ledger_auth_set_title($auth_node);
	drupal_set_message("Items saved.");
}