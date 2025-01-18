<?php

/***
 * RSR: "Reserved Sheet Request"
 *
 * This is a log entry that indicates that a given worker should be placed on a given reserved sheet for a given day.
 */

class Sirius_Edls_Rsr extends Sirius_Singleton {

	public function status_options() {
		return array(
			'requested' => t('Requested'),
			'processed' => t('Processed'),
			'failed' => t('Failed'),
			'canceled' => t('Canceled'),
		);
	}

	public function status($rsr_node) {
		return $rsr_node->field_sirius_type['und'][0]['value'];
	}

	public function status_color($status) {
		if ($status == 'requested') { return 'yellow'; }
		if ($status == 'processed') { return 'green'; }
		if ($status == 'failed') 		{ return 'red'; }
		if ($status == 'canceled') 	{ return 'gray'; }
	}

	public function status_set($rsr_node, $new_status) {
		$rsr_node->field_sirius_type['und'][0]['value'] = $new_status;
		node_save($rsr_node);
	}


	public function rsr_form($form, &$form_state, $rsr_node, $options = array()) {
		drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');

		$form['#tree'] = TRUE;
		$form_state['rsr_nid'] = $rsr_node->nid;
		$status = $this->status($rsr_node);

		$actions = array();
		if ($status == 'unsigned') {
			// Can only accept if unsigned
			$actions['accept'] = t('Accept');
		}

		if ($status == 'signed') {
			// Can revoke or clear if signed
			$actions['revoke'] = t('Revoke');
			$form['confirm'] = array(
				'#type' => 'checkbox',
				'#title' => 'I want to revoke this card check',
				'#required' => TRUE,
			);
		}

		if ($status == 'revoked') {
			// Can only clear if revoked
		}

		if ($options['can_clear']) {
			$actions['clear'] = t('Clear');
		}

		foreach ($actions as $action => $action_name) {
			$form[$action] = array(
				'#type' => 'submit',
				'#value' => $action_name,
				'#id' => 'sirius_esig_submit',
				'#sirius' => array('action' => $action),
			);
		}

		return $form;
	}

	public function rsr_form_submit($form, &$form_state) {
		$action = $form_state['triggering_element']['#sirius']['action'];
		if (!$action) {
			$form_state['rebuild'] = TRUE;
			drupal_set_message("Please click one of the submit buttons.", 'error');
			return;
		}

		$rsr_node = node_load($form_state['rsr_nid']);

		$result = SiriusResult::create(array('success' => FALSE, 'msg' => "Missing or unknown action."));

		$result->drupalSetMessage();
	}

	public function process($rsr_node) {
		global $user;

		if ($this->status($rsr_node) != 'requested') {
			return array('success' => FALSE, 'msg' => "Only requested assignments can be processed.");
		}

		// Save the acceptance
		$cardcheck['acceptance'] = array();
		$cardcheck['acceptance']['ts'] = time();
		$cardcheck['acceptance']['uid'] = $user->uid;
		$cardcheck['acceptance']['user_name'] = $user->name;
		$cardcheck_json['cardcheck'] = $cardcheck;
		sirius_json_set($cardcheck_node, $cardcheck_json);

		$this->status_set($cardcheck_node, 'signed');
		node_save($cardcheck_node);

		// Accept the disclaimer
		sirius_disclaimer_accept($cardcheck_node, array($disclaimer_node));

		return array('success' => TRUE, 'msg' => 'Cardcheck accepted.');
	}

	public function create($plugin_key, $worker_id, $date, $options = array()) {
		$title = '';

		$rsr_node = sirius_log(
      'edls:rsr',
      'requested',
      $title,
      '',
      $options['notes'],
      $handlers,
      NULL,
      array()
    );

    $rsr_node->title = $title;
    node_save($rsr_node);
    return $rsr_node;
	}

	public function search($options) {
		// @todo: For now, this is just a simple search by worker.
		if (!$options['worker_nid']) { return array(); }

		$query = new EntityFieldQuery;
	  $query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'sirius_log');
	  $query->fieldCondition('field_sirius_category', 'value', 'edls:rsr');
	  $query->fieldCondition('field_sirius_domain', 'target_id', sirius_domain_nid());
	  if ($options['worker_nid']) {
	  	$query->fieldCondition('field_sirius_log_handler', 'target_id', $options['worker_nid']);
	  }
	  if ($options['definition_nid']) {
	  	$query->fieldCondition('field_sirius_log_handler', 'target_id', $options['definition_nid']);
	  }
	  if ($options['status']) {
	  	$query->fieldCondition('field_sirius_type', 'value', $options['status']);
	  }
	  if ($options['statuses']) {
	  	$query->fieldCondition('field_sirius_type', 'value', $options['statuses'], 'in');
	  }
	  $query->propertyOrderBy('nid', 'desc');
	  $stmt = $query->execute();
	  $cardcheck_nodes = array();
	  if ($stmt['node']) {
	  	$cardcheck_nodes = node_load_multiple(array_keys($stmt['node']));
	  }
	  return $cardcheck_nodes;
	}
}