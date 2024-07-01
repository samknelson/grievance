<?php

/**
 * Wrapper for form, because we can't call drupal_get_form() on a class method
 */

function sirius_cardcheck_definition_form($form, &$form_state, $definition_node = NULL) {
	return Sirius_Cardcheck::getInstance()->definition_form($form, $form_state, $definition_node);
}

function sirius_cardcheck_definition_form_submit($form, &$form_state) {
	return Sirius_Cardcheck::getInstance()->definition_form_submit($form, $form_state);
}

require_once DRUPAL_ROOT . '/sites/all/modules/_custom/sirius_cardcheck/sirius_cardcheck.cardcheck.definition.inc';

class Sirius_Cardcheck extends Sirius_Singleton {
	use SiriusCardcheckDefinition;

	public function cardcheck_form($form, &$form_state, $cardcheck_node, $options = array()) {
		drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');

		$form['#tree'] = TRUE;
		$form_state['cardcheck_nid'] = $cardcheck_node->nid;
		$status = $this->status($cardcheck_node);

		$definition_node = $this->get_definition_node($cardcheck_node);
		$definition_json = sirius_json_get($definition_node);
		$definition = $definition_json['cardcheck_definition'];

		$cardcheck_json = sirius_json_get($cardcheck_node);
		$cardcheck = $cardcheck_json['cardcheck'];

		$disclaimer_nid = $definition['disclaimer_nid'];
		if (!$disclaimer_nid) { return sirius_form_error("Please specify a disclaimer."); }
		$disclaimer_node = node_load($disclaimer_nid);
		$disclaimer_json = sirius_json_get($disclaimer_node);
		$disclaimer = $disclaimer_json['disclaimer'];

		if ($status == 'signed' || $status == 'revoked') {
			$acceptance = $cardcheck['acceptance'];
			if ($acceptance) {
				$html = "<div class=\"sirius_callout_basic sirius_callout_success\">";
				$html .= "<h2>" . t('Card Check Accepted') . '</h2>';
				$html .= sirius_fakerender_field('Accepted on:', date('l, F j, g:i A', $acceptance['ts']));
				$html .= sirius_fakerender_field('Accepted by:', $acceptance['user_name']);
				$html .= '</div>';
				$form['acceptance'] = array('#markup' => $html);
			}
		}

		if ($status == 'revoked') {
			$revocation = $cardcheck['revocation'];
			if ($revocation) {
				$html = "<div class=\"sirius_callout_basic sirius_callout_error\">";
				$html .= "<h2>" . t('Card Check Revoked') . '</h2>';
				$html .= sirius_fakerender_field('Revoked on:', date('l, F j, g:i A', $revocation['ts']));
				$html .= sirius_fakerender_field('Revoked by:', $revocation['user_name']);
				$html .= '</div>';
				$form['revocation'] = array('#markup' => $html);
			}
		}

		// The disclaimer
		$form['disclaimer'] = sirius_disclaimer_render_form(
			$disclaimer, 
			$cardcheck_json['disclaimer'][$disclaimer_node->nid]['acceptances'][0]
		);

		// The form
		if ($definition['customfield_nid']) {
			$set_nid = $definition['customfield_nid'];
			$set_node = node_load($set_nid);
		  $set_json = sirius_json_get($set_node);

		  $customfield_form = sirius_jsonfield_util_form_get($set_json, 'default');
		  $customfield_results = sirius_jsonfield_util_results_get($cardcheck_json, 'customfield');
		  sirius_jsonfield_util_set_defaults($customfield_form, $customfield_results);

		  $form['customfield'] = $customfield_form;
		  $form['customfield']['#prefix'] = $set_json['customfield']['tab']['headfoot']['head'] . '<div class="sirius-form sirius_callout_basic">';
		  $form['customfield']['#suffix'] = '</div>' . $set_json['customfield']['tab']['headfoot']['foot'];
		  if ($status != 'unsigned') {
		  	$form['customfield']['#disabled'] = TRUE;
		  }
		}

		// The signature
		sirius_esignature_init();
		if ($status == 'unsigned') {
			sirius_esignature_form_prepare(
				$form, 
				'esig',
				$disclaimer_json['disclaimer']['title'], 
				$disclaimer_json['disclaimer']['body'], 
				array(
					'hide_title' => TRUE,
					'hide_statement' => TRUE,
					'hide_submit' => TRUE,
				)
			);
		} else {
			$esignature = sirius_jsonfield_util_esignature_get($cardcheck_json, 'esig');
			if ($esignature) { 
				$html = '<div class="sirius_callout_basic">';
				$html .= sirius_esignature_render($esignature, '', $cardcheck_node->nid, 'esig');
				$html .= '</div>';
				$form['esig'] = array('#markup' => $html);
			}
		}

		// Add materials. This is a kludge.
		if (sirius_materials_access('list', $cardcheck_node)) {
			$html = sirius_node_tab_entity_materials_list($cardcheck_node, array('hide_add' => TRUE, 'hide_empty' => TRUE, 'hide_header' => TRUE, 'hide_tools' => TRUE));
			if ($html) {
				$html = '<div class="sirius_callout_basic">' . $html . '</div>';
				$form['materials'] = array('#markup' => $html);
			}
		}

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

	public function cardcheck_form_submit($form, &$form_state) {
		$action = $form_state['triggering_element']['#sirius']['action'];
		if (!$action) {
			$form_state['rebuild'] = TRUE;
			drupal_set_message("Please click one of the submit buttons.", 'error');
			return;
		}

		$cardcheck_node = node_load($form_state['cardcheck_nid']);

		if ($action == 'accept') {
			$result = SiriusResult::create($this->accept($cardcheck_node, $form, $form_state));
		} else if ($action == 'clear') {
			$result = SiriusResult::create($this->clear($cardcheck_node));
		} else if ($action == 'revoke') {
			$result = SiriusResult::create($this->revoke($cardcheck_node));
		} else {
			$result = SiriusResult::create(array('success' => FALSE, 'msg' => "Missing or unknown action."));
		}

		$result->drupalSetMessage();
	}

	public function accept($cardcheck_node, $form, $form_state) {
		global $user;

		if ($this->status($cardcheck_node) != 'unsigned') {
			return array('success' => FALSE, 'msg' => "Only unsigned card checks can be accepted.");
		}

		$definition_node = $this->get_definition_node($cardcheck_node);

		$definition_json = sirius_json_get($definition_node);
		$definition = $definition_json['cardcheck_definition'];

		$cardcheck_json = sirius_json_get($cardcheck_node);
		$cardcheck = $cardcheck_json['cardcheck'];

		$disclaimer_nid = $definition['disclaimer_nid'];
		$disclaimer_node = node_load($disclaimer_nid);
		$disclaimer_json = sirius_json_get($disclaimer_node);
		$disclaimer = $disclaimer_json['disclaimer'];

		// Save the esignature
		$signature = sirius_esignature_form_get_json(
			$form, 
			$form_state,
			'esig'
		);
		sirius_jsonfield_util_esignature_set($cardcheck_json, 'esig', $signature);

		// Save the custom fields
	  sirius_jsonfield_util_results_set($cardcheck_json, 'customfield', $form_state['values']['customfield']);

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

	public function revoke($cardcheck_node) {
		global $user;

		if ($this->status($cardcheck_node) != 'signed') {
			return array('success' => FALSE, 'msg' => "Only signed card checks can be revoked.");
		}

		$cardcheck_json = sirius_json_get($cardcheck_node);
		$cardcheck = $cardcheck_json['cardcheck'];
		$cardcheck['revocation'] = array();
		$cardcheck['revocation']['ts'] = time();
		$cardcheck['revocation']['uid'] = $user->uid;
		$cardcheck['revocation']['user_name'] = $user->name;
		$cardcheck_json['cardcheck'] = $cardcheck;
		sirius_json_set($cardcheck_node, $cardcheck_json);

		$this->status_set($cardcheck_node, 'revoked');
		node_save($cardcheck_node);

		return array('success' => TRUE, 'msg' => 'Cardcheck revoked.');
	}

	public function clear($cardcheck_node) {
		global $user;

		$cardcheck_json = sirius_json_get($cardcheck_node);
		unset($cardcheck_json['cardcheck']);
		unset($cardcheck_json['disclaimer']);
		unset($cardcheck_json['esig']);
		unset($cardcheck_json['customfield']);
		sirius_json_set($cardcheck_node, $cardcheck_json);

		$this->status_set($cardcheck_node, 'unsigned');
		node_save($cardcheck_node);

		return array('success' => TRUE, 'msg' => 'Cardcheck cleared.');
	}

	public function status_options() {
		return array(
			'unsigned' => t('Unsigned'),
			'signed' => t('Signed'),
			'revoked' => t('Revoked'),
		);
	}

	public function status($cardcheck_node) {
		return $cardcheck_node->field_sirius_type['und'][0]['value'];
	}

	public function status_color($status) {
		if ($status == 'unsigned') { return 'yellow'; }
		if ($status == 'signed') { return 'green'; }
		if ($status == 'revoked') { return 'red'; }
	}

	public function status_set($cardcheck_node, $new_status) {
		$cardcheck_node->field_sirius_type['und'][0]['value'] = $new_status;
		node_save($cardcheck_node);
	}

	public function create($definition_node, $options = array()) {
		$title = '';
		$handlers = array($definition_node->nid);

		if ($options['entity_node']) {
			$title = $options['entity_node']->title;
			$handlers[] = $options['entity_node']->nid;
		}
		else {
			$title = '[No Handler]';
		}

		$title .= ': ';
		$title .= $definition_node->title;

		$cardcheck_node = sirius_log(
      'cardcheck',
      'unsigned',
      $title,
      '',
      $options['notes'],
      $handlers,
      NULL,
      array()
    );

    $cardcheck_node->title = $title;
    node_save($cardcheck_node);
    return $cardcheck_node;
	}

	public function test_cardcheck_node($definition_nid) {
		$id = 'CARDCHECK_TEST';
		$domain_nid = sirius_domain_nid();

	  $query = new EntityFieldQuery;
	  $query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'sirius_log');
	  $query->fieldCondition('field_sirius_category', 'value', 'cardcheck');
	  $query->fieldCondition('field_sirius_notes', 'value', $id);
	  $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid);
	  $query->fieldCondition('field_sirius_log_handler', 'target_id', $definition_nid);
	  $query->propertyOrderBy('nid', 'desc');
	  $query->range(0, 1);
	  $stmt = $query->execute();
	  if ($stmt['node']) {
	  	return node_load(array_keys($stmt['node'])[0]);
	  }

	  return $this->create(node_load($definition_nid), array('notes' => $id));
	}

	public function search($options) {
		// @todo: For now, this is just a simple search by worker.
		if (!$options['worker_nid']) { return array(); }

		$query = new EntityFieldQuery;
	  $query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'sirius_log');
	  $query->fieldCondition('field_sirius_category', 'value', 'cardcheck');
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
