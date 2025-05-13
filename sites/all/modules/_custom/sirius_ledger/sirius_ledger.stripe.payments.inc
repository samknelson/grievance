<?php

trait Sirius_Ledger_Service_Stripe_Payments {
	public function payment_node_view($payment_node, $view_mode, $langcode) {
		$payment_json = sirius_json_get($payment_node);
		$paymentintent_id = $payment_json['stripe']['paymentintent']['id'];
		if (!$paymentintent_id) { return; }

	  drupal_add_css('sites/all/modules/_custom/sirius_ledger/css/stripe.css');

	  $html = $this->paymentintent_render($payment_json['stripe']['paymentintent']);
	  $html .= $this->paymentintent_render_events($payment_node);

		$payment_node->content['stripe'] = array(
			'#weight' => 100,
			'#markup' => $html,
		);
	}

	public function paymentintent_get_event_nids($payment_nid) {
		if (!$payment_nid) { return array(); }

	  // SQL for speed
	  $sql = "select distinct nid from node ";
	  $sql .= "join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
	  $sql .= "join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
	  $sql .= "join field_data_field_sirius_log_handler on field_data_field_sirius_log_handler.entity_type = 'node' and field_data_field_sirius_log_handler.entity_id = node.nid ";
	  $sql .= "where node.type = 'sirius_log' ";
	  $sql .= "and field_sirius_log_handler_target_id = :payment_nid ";
	  $sql .= "and field_sirius_category_value = 'stripe:paymentintent:webhook' ";
	  $sql .= "order by nid desc limit 1000 ";
	  $stmt = db_query($sql, array(':payment_nid' => $payment_nid));

	  $event_nids = array();
	  while ($hr = $stmt->fetchAssoc()) { $event_nids[] = $hr['nid']; }
	  return $event_nids;
	}

	public function paymentintent_render_events($payment_node) {
		$event_nids = $this->paymentintent_get_event_nids($payment_node->nid);
		if (!$event_nids) { return ''; }
		$event_nodes = node_load_multiple($event_nids);

		$html .= '<h2>' . t('Events') . '</h2>';

		$header = array();
		/*
		$header[] = t('Event');
		$header[] = t('Time');
		$header[] = t('Tools');
		*/

		$rows = array();
		$types = $this->paymentintent_event_types();
		foreach ($event_nodes as $event_nid => $event_node) {
			$event_json = sirius_json_get($event_node);
			$type = $event_node->field_sirius_type['und'][0]['value'];
			$type_title = $types[$type]['title'];
			if (!$type_title) { $type_title = "($type)"; }
			$payment_status = $types[$type]['status'];
			$color = sirius_ledger_payment_status_color($payment_status);

			$notes = '';
			if ($event_json['data']['object']['last_payment_error']['message']) {
				$notes = $event_json['data']['object']['last_payment_error']['message'];
			}

			$row = array();
			$row[] = $type_title;
			$row[] = date('m/d/Y h:i a', $event_node->created) . ' (' . format_interval(time() - $event_node->created) . ' ago)';
			$row[] = $notes;
			$row[] = l('View', "node/$event_nid", array('attributes' => array('class' => array('tool-button'))));
			$rows[] = array(
				'class' => array($color), 
				'data' => $row
			);
		}

		$html .= sirius_table_render($header, $rows);
		$html .= '</div>';
		return $html;
	}

	public function paymentintent_render($paymentintent_json) {
		$html = '<div class="sirius_ledger_stripe_paymentintent">'; 
		$html .= '<div class="sirius_ledger_stripe_customer_links">';
		$url = $this->object_url($paymentintent_json);
		$html .= "<a href=\"$url\" class=\"tool-button\" target=\"_blank\">";
		$html .= '<span class="sirius_ledger_stripe_link_icon">';
		$html .= '<i class="fab fa-cc-stripe"></i>';
		$html .= '</span>';
		$html .= "View in Stripe";
		$html .= '</a>';
		$html .= '</div>';
		$html .= '</div>';

		return $html;
	}

	public function paymentintent_fetch_by_id($id, $nid_only = FALSE) {
		// id = paymentintent_id

		$sql_args = array();
		$sql = "select nid ";
		$sql .= "from node ";
		$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";

		$sql .= "where field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = sirius_domain_nid();

		$sql .= "and json_denorm_external_id = :external_id ";
		$sql_args[':external_id'] = $id;

		$sql .= "order by nid desc limit 1 ";

		$stmt = sirius_sql_query($sql, $sql_args);
		$hr = $stmt->fetchAssoc();
		if (!$hr['nid']) { return NULL; }

		if ($nid_only) { return $hr['nid']; }
		return node_load($hr['nid']);
	}

	public function paymentintent_event_types() {
	  $event_types['canceled'] = array(
	  	'title' => 'Canceled',
	  	'status' => 'Canceled',
	  );

		$event_types['created'] = array(
			'title' => 'Created',
			'status' => 'Pending',
		);
		$event_types['partially_funded'] = array(
			'title' => 'Partially Funded',
			'status' => 'Pending',
		);
		$event_types['payment_failed'] = array(
			'title' => 'Payment Failed',
			'status' => 'Failed',
		);
		$event_types['processing'] = array(
			'title' => 'Started Processing',
			'status' => 'Pending',
		);
		$event_types['requires_action'] = array(
			'title' => 'Reuires Action',
			'status' => 'Failed',
		);
		$event_types['succeeded'] = array(
			'title' => 'Succeeded',
			'status' => 'Cleared',
		);

		return $event_types;
	}


	function paymentintent_webhook_handle($event_json) {
  	$paymentintent_id = $event_json['data']['object']['id'];
  	if (!$paymentintent_id) { return $this->webhook_error($orig_domain_nid, "No paymentintent_id found in the webhook."); }

  	$payment_node = $this->paymentintent_fetch_by_id($paymentintent_id);
  	if (!$payment_node) { return array('success' => FALSE, 'msg' => 'Payment not found: ' . $paymentintent_id); }

	  list ($event_category, $event_type) = explode('.', $event_json['type']);

	  $event_types = $this->paymentintent_event_types();

		if (!$event_types[$event_type]) { return array('success' => FALSE, 'msg' => "Don't know how to handle payment_intent.$event_type"); }

		// Never update payments which are "Cleared"
		if ($payment_node->field_sirius_payment_status['und'][0]['value'] != 'Cleared') {
			$params = array();
			$params['payment_status'] = $event_types[$event_type]['status'];
			$params['allocate'] = 'Yes';
			$result = sirius_ledger_payment_update($payment_node, $params);
		}

		$notes = $result['msg'];


		// Log the request
	  $log_node = sirius_log(
      'stripe:paymentintent:webhook',
      $event_type,
      $event_types[$event_type]['title'],
      $result['msg'],
      $event_json['id'],
      array($payment_node->nid),
      array(),
      $event_json
	  );

  	$result = array('success' => FALSE, 'msg' => 'Payment intent webhooks not implemented yet.');
	}
}