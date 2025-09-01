<?php 

class Sirius_Trust_Changequeue extends Sirius_Singleton {
	function status_options() {
		// We expect the queue entries to be:
		// - "Pending": Need to review
		// - "Approved": Value is approved and updated
		// - "Rejected": Value is rejected
		// - "Blocked": Value is rejected and should never be queued again

		return array(
			'pending' => t('Pending'),
			'approved' => t('Approved'),
			'rejected' => t('Rejected'),
			'blocked' => t('Blocked'),
		);
	}

	public function get_id($cq_item) {
		$id = $cq_item['type'];
		$id .= '|';
		$id .= $cq_item['node_field']['nid'];
		$id .= '|';
		$id .= $cq_item['node_field']['field_name'];
		$id .= '|';
		$id .= preg_replace('/\|/', ':', print_r($cq_item['node_field']['val'], 1));
		return $id;
	}

	public function get_status($cq_node) {
		return $cq_node->field_sirius_type['und'][0]['value'];
	}

	public function set_status($cq_node, $new_status) {
		// Skip if the status is unchanged
		if ($this->get_status($cq_node) == $new_status) { return array('success' => TRUE, 'msg' => 'Status unchanged.'); }

		$cq_json = sirius_json_get($cq_node);
		$cq_item = $cq_json['cq_item'];

		// If status is "Approved", set the value
		if ($new_status == 'approved') {
			$type = $cq_item['type'];
			if (!$type) { return array('success' => FALSE, 'msg' => 'No queue item type.'); }

			// For now we only handle "node_field"
			if ($cq_item['type'] != 'node_field') { return array('success' => FALSE, 'msg' => 'Unknown queue item type: ' . $cq_item['type']); }

			// Make sure we have an NID, a field, and a value for the field
			$nid = $cq_item['node_field']['nid'];
			if (!$nid) { return array('success' => FALSE, 'msg' => 'Queue item does not contain an NID.'); }

			$field_name = $cq_item['node_field']['field_name'];
			if (!$field_name) { return array('success' => FALSE, 'msg' => 'Queue item does not contain a field name.'); }

			$val = $cq_item['node_field']['val'];
			if (!$val) { return array('success' => FALSE, 'msg' => 'Queue item does not contain a val.'); }

			// Apply the change
			$node = node_load($nid);
			$node->{$field_name}['und'][0] = $val;
			node_save($node);
		}

		// Any other changes with the same ID should also be updated.
		$id = $this->get_id($cq_item);
		if ($id && ($new_status != 'pending')) {
			$search_params = array();
			$search_params['id'] = $id;
			$other_cq_nodes = $this->search($search_params);
			foreach ($other_cq_nodes as $other_cq_nid => $other_cq_node) {
				if ($other_cq_nid == $cq_node->nid) { continue; }
				if ($other_cq_node->field_sirius_type['und'][0]['value'] == $new_status) { continue; }
				$other_cq_node->field_sirius_type['und'][0]['value'] = $new_status;
				node_save($other_cq_node);
			}
		}
		
		// Set the status
		$cq_node->field_sirius_type['und'][0]['value'] = $new_status;
		node_save($cq_node);
		return array('success' => TRUE);
	}

	function queue($cq_item) {
		$type = $cq_item['type'];
		if (!$type) { return array('success' => FALSE, 'msg' => 'No queue item type.'); }

		// For now we only handle "node_field"
		if ($cq_item['type'] != 'node_field') { return array('success' => FALSE, 'msg' => 'Unknown queue item type: ' . $cq_item['type']); }

		// Make sure we have an NID, a field, and a value for the field
		$nid = $cq_item['node_field']['nid'];
		if (!$nid) { return array('success' => FALSE, 'msg' => 'Queue item does not contain an NID.'); }

		$field_name = $cq_item['node_field']['field_name'];
		if (!$field_name) { return array('success' => FALSE, 'msg' => 'Queue item does not contain a field name.'); }

		$val = $cq_item['node_field']['val'];
		if (!$val) { return array('success' => FALSE, 'msg' => 'Queue item does not contain a val.'); }

		$id = $this->get_id($cq_item);
		$is_blocked = $this->is_blocked($id);
		if ($is_blocked) { return array('success' => TRUE, 'msg' => 'This update has been blocked.'); }

		// Don't create a queue entry if the field is changed.
		$node = node_load($nid);
		$node_new = clone $node;
		$node_new->{$field_name}['und'][0] = $val;
		if (!sirius_revision_changed($node, $node_new)) { return array('success' => TRUE, 'msg' => 'Node is unchanged.'); }

		// Queue the entry
		$msg = $cq_item['msg'];
		if (!$msg) {
			global $user;
			$msg = "Entry created by " . $user->name;
		}

		$handler_nids = $cq_item['handler_nids'];
		if (!$handler_nids) { $handler_nids = array(); }

		$json = array(); 
		$json['cq_item'] = $cq_item;
		$cq_node = sirius_log(
  		'changequeue',
  		'pending',
  		$msg, 
  		$msg, 
  		$id, 
  		$handler_nids, 
  		array(),
  		$json
  	);

		return array('success' => TRUE, 'msg' => 'Change Queue Entry Created', 'cq_nid' => $cq_node->nid);
	}

	public function is_blocked($id) {
		$params = array();
		$params['status'] = 'blocked';
		$params['id'] = $id;
		$params['nids_only'] = TRUE;
		$params['limit'] = 1;
		$probe = $this->search($params);
		if ($probe) { return reset($probe); }
	}

	public function search($params = array()) {
		$sql_args = array();
		if ($params['count_only']) {
			$sql = "select count(nid) as c "; 
		} else {
			$sql = "select nid ";
		}
		$sql .= "from node ";
		$sql .= "left join field_data_field_sirius_notes on field_data_field_sirius_notes.entity_type = 'node' and field_data_field_sirius_notes.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
		$sql .= "where type = 'sirius_log' ";
		$sql .= "and field_sirius_category_value = 'changequeue' ";

		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = sirius_domain_nid();

		if ($params['status']) {
			$sql .= "and field_sirius_type_value = :status ";
			$sql_args[':status'] = $params['status'];
		}

		if ($params['id']) {
			$sql .= "and field_sirius_notes_value = :id ";
			$sql_args[':id'] = $params['id'];
		}

		$sql .= "order by changed desc, nid desc ";

		$limit = $params['limit'];
		if (!$limit) { $limit = 100; }

		$offset = $params['offset'];
		if (!$offset) { $offset = 0; }
		$sql .= "limit $limit offset $offset ";

		$stmt = sirius_sql_query($sql, $sql_args);

		if ($params['count_only']) {
			$hr = $stmt->fetchAssoc();
			return $hr['c'];
		}

		$cq_nids = array();
		while ($hr = $stmt->fetchAssoc()) {
			$cq_nids[] = $hr['nid'];
		}

		if ($params['nids_only']) { return $cq_nids; }

		return node_load_multiple($cq_nids);
	}

	public function render_list($cq_nodes, $options = array()) {
		if (!$cq_nodes) {
			$form['empty'] = array('#markup' => '<p>' . t('No matching items found.') . '</p>');
			return $form;
		}

		$form['table_open'] = array('#markup' => '<table>');

		$html = '<tr>';
		$html .= '<th>' . t('Created') . '</th>';
		$html .= '<th>' . t('Message') . '</th>';
		$html .= '<th>' . t('Record') . '<br />' . t('Field') . '</th>';
		$html .= '<th>' . t('Current Value') . '</th>';
		$html .= '<th>' . t('Proposed Value') . '</th>';
		$html .= '<th>' . t('Action') . '</th>';
		$html .= '<th>' . t('Tools') . '</th>';
		$html .= '</tr>';
		$form['header'] = array('#markup' => $html);

		$status_options = $this->status_options();

		$rows = array();
		global $user;
		foreach ($cq_nodes as $cq_nid => $cq_node) {
			$cq_json = sirius_json_get($cq_node);
			$cq_item = $cq_json['cq_item'];

			$entity_nid = $cq_item['node_field']['nid'];
			$entity_node = node_load($entity_nid);
			$field_name = $cq_item['node_field']['field_name'];

			if ($entity_node) {
      	$entity_title = $entity_node->title;
      	$field_info = field_info_instance('node', $field_name, $entity_node->type);
      	$current_val = $this->_util_render_field($entity_node, $field_name);

      	$entity_node_new = clone $entity_node;
				$entity_node_new->{$field_name}['und'][0] = $cq_item['node_field']['val'];
      	$new_val = $this->_util_render_field($entity_node_new, $field_name);
      	$field_label = $field_info['label'];
      } else {
      	$field_info = array();
      	$current_val = $new_val = $entity_title = '';
      	$entity_title = '[Record missing]';
      	$field_label = '[no field]';
      }

			$tools = array();
			$tools[] = l('Record', "node/$entity_nid", array('attributes' => array('class' => array('tool-button'))));
			if ($user->uid == 1) {
				$tools[] = l('CQ', "node/$cq_nid/sirius_trust_cq", array('attributes' => array('class' => array('tool-button'))));
			}
			// $tools[] = l('Record to Update', "node/$entity_nid", array('attributes' => array('class' => array('tool-button'))));


			$form[$cq_nid]['tr_open'] = array('#markup' => '<tr>');
			$form[$cq_nid]['created'] = array('#markup' => '<td>' . date('m/d/Y h:i a', $cq_node->created) . '<br />' . $status_options[$this->get_status($cq_node)] . '</td>');
			$form[$cq_nid]['msg'] = array('#markup' => '<td>' . $cq_item['msg'] . '</td>');
			$form[$cq_nid]['entity_title'] = array('#markup' => '<td>' . $entity_title . '<br>' . $field_label . '</td>');
			$form[$cq_nid]['current_val'] = array('#markup' => '<td>' . $current_val . '</td>');
			$form[$cq_nid]['new_val'] = array('#markup' => '<td>' . $new_val . '</td>');

			$tmp = $status_options;
			$form[$cq_nid]['action'] = array(
				'#type' => 'radios',
				'#options' => $status_options,
				'#default_value' => $this->get_status($cq_node),
				'#prefix' => '<td>',
				'#suffix' => '</td>',
			);

			$form[$cq_nid]['tools'] = array('#markup' => '<td>' . join('', $tools) . '</td>');
			$form[$cq_nid]['tr_close'] = array('#markup' => '</tr>');
		}
		$form['table_close'] = array('#markup' => '</table>');

		return $form;
	}

	function _util_render_field($node, $field_name) {
		$mode = 'full';
		$renderable = field_view_field('node', $node, $field_name, 'full');
		$renderable['#label_display'] = 'hidden';
		return render($renderable);
	}
}
