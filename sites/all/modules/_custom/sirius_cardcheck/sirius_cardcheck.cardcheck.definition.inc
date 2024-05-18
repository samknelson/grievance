<?php

trait SiriusCardcheckDefinition {
	public function get_definition_node($cardcheck_node) {
		return sirius_log_handler($cardcheck_node, 'sirius_json_definition');
	}

	public function definition_list($options = array()) {
		$domain_nid = sirius_domain_nid();

		$sql_args = array();
		$sql = "select nid from node ";
		$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
		$sql .= "where node.type = 'sirius_json_definition' ";
		$sql .= "and field_sirius_type_value = 'sirius:cardcheck' ";
		if ($domain_nid) { 
			$sql .= "and field_sirius_domain_target_id = :domain_nid ";
			$sql_args[':domain_nid'] = $domain_nid;
		}
		if (!$options['include_inactive']) {
			$sql .= "and json_extract(field_sirius_json_value, '\$.cardcheck_definition.active') = 'Yes' ";
		}
		$sql .= "order by title ";

		$stmt = sirius_sql_query($sql, $sql_args);
		$definition_nids = array();
		while ($hr = $stmt->fetchAssoc()) {
			$definition_nids[] = $hr['nid'];
		}
		if ($options['nids_only']) { return $definition_nids; }
		return node_load_multiple($definition_nids);
	}

	public function definition_form($form, &$form_state, $definition_node = NULL) {
		if (!$definition_node) { $definition_node = new stdClass(); }
		$form_state['definition_nid'] = $definition_node->nid;
		$definition_json = sirius_json_get($definition_node);
		$definition = $definition_json['cardcheck_definition'];
		if (!$definition['title']) { $definition['title'] = $definition_node->title; }

		$form['#tree'] = TRUE;

		$form['cardcheck_definition']['title'] = array(
			'#type' => 'textfield',
			'#title' => t('Title'),
			'#default_value' => $definition['title'],
		);

		$form['cardcheck_definition']['active'] = array(
			'#type' => 'select',
			'#title' => 'Active?',
			'#default_value' => $definition['active'],
			'#options' => array(
				'Yes' => t('Yes'),
				'No' => t('No'),
			),
		);

		$form['cardcheck_definition']['customfield_nid'] = array(
			'#type' => 'select',
			'#title' => t('Custom Fields'),
			'#default_value' => $definition['customfield_nid'],
			'#options' => array(NULL => t('-- Select --')) + sirius_customfield_set_list_all('sirius_log', 'cardcheck'),
		);

		$form['cardcheck_definition']['disclaimer_nid'] = array(
			'#type' => 'select',
			'#title' => 'Disclaimer',
			'#default_value' => $definition['disclaimer_nid'],
			'#options' => array(NULL => t('-- Select --')) + sirius_disclaimer_list(),
		);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);

		return $form;
	}

	public function definition_form_submit($form, &$form_state) {
		$definition_node = node_load($form_state['definition_nid']);

		if (!$definition_node->nid) {
			global $user;
			$domain_nid = sirius_domain_nid();
		  $definition_node = new stdClass();
		  $definition_node->type = 'sirius_json_definition';
		  $definition_node->no_recurse = TRUE;
		  $definition_node->language = LANGUAGE_NONE;
		  $definition_node->uid = $user->uid;
		  $definition_node->status = 1;
		  $definition_node->promote = 0;
		  $definition_node->comment = 0;
		  if ($domain_nid) { $definition_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
		  $definition_node->field_sirius_feed_status['und'][0]['value'] = 'draft';
		  $definition_node->field_sirius_type['und'][0]['value'] = 'sirius:cardcheck';
		  $definition_node = node_submit($definition_node);
		  node_object_prepare($definition_node);
		}

		$definition_json = sirius_json_get($definition_node);
		$definition_json['cardcheck_definition'] = $form_state['values']['cardcheck_definition'];
		sirius_json_set($definition_node, $definition_json);
		$definition_node->title = $form_state['values']['cardcheck_definition']['title'];
		node_save($definition_node);

		drupal_set_message("Settings saved.");
	}
}