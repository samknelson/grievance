<?php

function sirius_node_tab_customfield_multiple_list_access($set_node) {
	// Must be a multple-type customfield
	if (!sirius_customfield_multiple($set_node)) { return FALSE; }

	$contxt = array('set_node' => $set_node);
	return sirius_customfield_access('list_all', $contxt);
}

function sirius_node_tab_customfield_multiple_list($set_node) {
	$settings = sirius_customfield_access_instance_settings($set_node); 
  $context = array(
  	'set_node' => $set_node,
  	'settings' => $settings,
  );

  $page_size = 50;
  $page = pager_find_page();
  $offset = $page_size * $page;

  $sql_count = "select count(*) as c ";
  $sql_fields = "select node.nid ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
	$sql .= "where type = 'sirius_log' ";
	$sql .= "and field_sirius_category_value = 'customfield' ";
	$sql .= "and field_sirius_type_value = :set_nid ";
	$sql_args[':set_nid'] = $set_node->nid;
	$sql .= "order by created desc, node.nid desc ";
  $sql_limit = "limit $page_size offset $offset ";

  $stmt = sirius_sql_query($sql_count . $sql, $sql_args);
  $hr = $stmt->fetchAssoc();
  $count = $hr['c'];
  $pager = pager_default_initialize($count, $page_size);

  $stmt = sirius_sql_query($sql_fields . $sql . $sql_limit, $sql_args);
  $rows = array();

	$instance = sirius_customfield_access_instance($set_node);
	if (method_exists($instance, 'status_options_all')) {
		$status_options_all = $instance->status_options_all($context);
	}

	$header = array();
	$header[] = t('Date');
	$header[] = t('User');
	if ($status_options_all) {
		$header[] = t('Status');
	}
	$header[] = t('Host Record');
	$header[] = t('Tools');

	$rows = array();
	while ($hr = $stmt->fetchAssoc()) {
		$data_node = node_load($hr['nid']);
		$context = sirius_customfield_multiple_context($data_node);
		$tools = array(
			'node/' . ($context['entity_node']->nid) . '/sirius_dispatch_job_group_customfield_tab/' . ($context['set_node']->nid) => t('Host Record'),
			'node/' . $hr['nid'] => t('View'),
			'node/' . $hr['nid'] . '/sirius_log_customfield_multiple_edit' => t('Form'),
			'node/' . $hr['nid'] . '/sirius_log_customfield_multiple_status' => t('Status'),
			'node/' . $hr['nid'] . '/sirius_log_materials' => t('Materials'),
		);

		$tools_render = '';
		foreach ($tools as $path => $name) {
			if (drupal_valid_path($path)) {
				$tools_render .= l($name, $path, array('attributes' => array('class' => array('tool-button'))));
			}
		}

		$author = user_load($context['data_node']->uid);

		$row = array();
		$row[] = date('n/j/Y g:ia', $context['data_node']->created);
		$row[] = $author->name;
		if ($status_options_all) {
			$status = sirius_customfield_status($context);
			$row[] = $status_options_all[$status];
		}
		$row[] = $context['entity_node']->title;
		$row[] = $tools_render;
		$rows[] = $row;
	}

	if ($rows) {
		$html = sirius_table_render($header, $rows);
	  $html .= theme('pager');
	} else {
		$html = '<p>' . t('No entries found.') . '</p>';
	}

	return $html;
}
