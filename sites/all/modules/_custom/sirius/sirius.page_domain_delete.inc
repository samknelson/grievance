<?php

function sirius_node_tab_sirius_domain_delete_access($domain_node) {
	if (user_access('sirius admin')) { return TRUE; }
	if (!user_access('sirius domain delete')) { return FALSE; }
	return sirius_domain_access($domain_node->nid);
}

function sirius_node_tab_sirius_domain_delete($domain_node) {
	return drupal_get_form('sirius_node_tab_sirius_domain_delete_form', $domain_node);
}

function sirius_node_tab_sirius_domain_delete_form($form, &$form_state, $root_domain_node) {
	$form_state['domain_node'] = $root_domain_node;

	$form['intro'] = array(
		'#markup' => '<div class="sirius_form_intro">' . t('The form below will permanently completely delete the domain <strong>@title</strong>. All subdomains, users, contacts, settings, and contacts associated with this domain will be removed from the system. This action cannot be undone.', array('@title' => $root_domain_node->title)) . '</div>',
	);

	$subdomains = sirius_domain_tree_render_as_select($root_domain_node->nid);
	if (count($subdomains) > 1) {
		$html = t("The following subdomains will also be deleted: ");
		$html .= '<ul>';
		foreach ($subdomains as $subdomain_nid => $subdomain_name) {
			$html .= "<li><strong>$subdomain_name</strong></li>";
		}
		$html .= "</ul>"; 
		$form['intro_subdomains'] = array('#markup' => '<div class="sirius_form_intro">' . $html . '</div>');
	}

	$form['mode'] = array(
		'#type' => 'select',
		'#title' => 'Mode',
		'#options' => array(
			NULL => t('-- Select --'),
			'test' => t('Test - nothing will be deleted'),
			'live' => t('Live - Domain will be deleted')
		),
		'#required' => TRUE,
	);

	$field_info = field_info_field('field_sirius_domain');
	$node_bundles = drupal_map_assoc($field_info['bundles']['node']);
	unset($node_bundles['sirius_domain']);
	$taxonomy_bundles = drupal_map_assoc($field_info['bundles']['taxonomy_term']);

	$form['node_bundles'] = array(
		'#type' => 'checkboxes',
		'#title' => 'Delete these node types',
		'#options' => $node_bundles,
		'#checkall' => TRUE,
	);

	$form['taxonomy_term_bundles'] = array(
		'#type' => 'checkboxes',
		'#title' => 'Delete these taxonomy terms',
		'#options' => $taxonomy_bundles,
		'#checkall' => TRUE,
	);

	$form['user_bundles'] = array(
		'#type' => 'checkboxes',
		'#title' => 'Delete the users?',
		'#options' => array('user' => 'Yes, also delete the users in these domains.'),
	);

	$form['report_cache'] = array(
		'#type' => 'select',
		'#title' => 'Delete the reports?',
		'#options' => array(
			'No' => t('No, leave any generated reports.'),
			'Yes' => t('Yes, delete generated reports.'),
		),
		'#description' => t('If deleteing the domain, please check ALL content and taxonomy types.'),
	);

	$form['delete_domain'] = array(
		'#type' => 'select',
		'#title' => 'Delete the domain?',
		'#options' => array(
			'No' => t('No, delete the content checked above but leave the domain.'),
			'Yes' => t('Yes, delete the domain as well.'),
		),
		'#description' => t('If deleteing the domain, please check ALL content and taxonomy types.'),
	);

	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#title' => t('Yes, permanently and completely delete the domain @title. This action cannot be undone.', array('@title' => $root_domain_node->title)),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_sirius_domain_delete_form_submit($form, &$form_state) {
	$root_domain_node = $form_state['domain_node'];
	$mode = $form_state['values']['mode'];

	$form_state['rebuild'] = TRUE;

	$subdomains = sirius_domain_tree_render_as_select($root_domain_node->nid);
	$domains_to_delete = array($root_domain_node->nid => $root_domain_node->title);
	$domains_to_delete += $subdomains;

	$operations = array();
	$entity_types = array('node', 'taxonomy_term');
	foreach ($domains_to_delete as $domain_nid => $domain_node) {
		foreach (array('node', 'taxonomy_term', 'user') as $entity_type) {
			foreach (array_values($form_state['values']["${entity_type}_bundles"]) as $bundle) {
				if (!$bundle) { continue; }
				$entity_ids = sirius_domain_delete_inventory($domain_nid, $entity_type, $bundle);
				$chunks = array_chunk($entity_ids, 100, TRUE);
				foreach ($chunks as $chunk) {
					$operations[] = array(
						'sirius_domain_delete_batch', 
						array($mode, $domain_nid, $entity_type, $bundle, array_values($chunk)),
					);
				}
			}
		}

		if ($form_state['values']['report_cache'] == 'Yes') {
			$operations[] = array(
				'sirius_domain_delete_report_cache', 
				array($mode, $domain_nid),
			);
		}

	}

	// Put the domain nodes themselves as the very end.
	if ($form_state['values']['delete_domain'] == 'Yes') {
		$operations[] = array(
			'sirius_domain_delete_batch', 
			array($mode, $root_domain_node->nid, 'node', 'sirius_domain', array_keys($domains_to_delete)),
		);
	}

	// Put the domain nodes themselves as the very end.
	if ($form_state['values']['delete_domain'] == 'Yes') {
		$operations[] = array(
			'sirius_domain_delete_batch', 
			array($mode, $root_domain_node->nid, 'node', 'sirius_domain', array_keys($domains_to_delete)),
		);
	}
	
	// Launch the batch processor.
  $batch = array(
    'operations' => $operations,
		'finished' => 'sirius_domain_delete_batch_finished',
  );
	batch_set($batch);
}

function sirius_domain_delete_report_cache($mode, $domain_nid, &$context) {
	$sql = "select report_type, count(*) as c from report_cache where report_domain_nid = :domain_nid group by report_type ";
	$stmt = db_query($sql, array(':domain_nid' => $domain_nid));
	$report_types = array();
	while ($hr = $stmt->fetchAssoc()) {
		$report_types[$hr['report_type']] = $hr['c'];
	}

	$total = 0;
	foreach ($report_types as $report_type => $count) {
		$total += $count;
		$msg = "Deleting: Domain #$domain_nid: Report of type $report_type";
		if ($mode == 'live') {
			$sql = "delete from report_cache where report_type = :report_type and report_domain_nid = :domain_nid";
			$stmt = db_query($sql, array(':report_type' => $report_type, ':domain_nid' => $domain_nid));
		}

		if ($mode == 'live') { $msg .= " [LIVE MODE]"; } else { $msg .= " [TEST MODE, NO RECORDS DELETED]"; }
		$context['results']['msgs'][] = $msg;
		$context['results']['deleted']['report_cache'][$report_type]['count'] += $count;
	}

	$msg = "Deleting: Domain #$domain_nid: Total of $total reports";
	$context['message'] = $msg;
	$context['results']['mode'] = $mode;
}

function sirius_domain_delete_batch($mode, $domain_nid, $entity_type, $bundle, $entity_ids, &$context) {
	$count = count($entity_ids);
	$first = $entity_ids[0];
	$last = $entity_ids[$count-1];

	$msg = "Deleting: Domain #$domain_nid: $entity_type, $bundle ($count entities, ids $first to $last)";

	if ($mode == 'live') {
		if ($entity_type == 'node') { node_delete_multiple($entity_ids); }
		if ($entity_type == 'user') { user_delete_multiple($entity_ids); }
		if ($entity_type == 'taxonomy_term') {
			foreach ($entity_ids as $tid) { taxonomy_term_delete($tid); }
		}
	}

	if ($mode == 'live') { $msg .= " [LIVE MODE]"; } else { $msg .= " [TEST MODE, NO RECORDS DELETED]"; }
	$context['results']['msgs'][] = $msg;
	$context['results']['deleted'][$entity_type][$bundle]['count'] += $count;
	$context['message'] = $msg;
	$context['results']['mode'] = $mode;
}

function sirius_domain_delete_batch_finished($success, $results, $operations) {
	$mode = $results['mode'];
	if ($results['deleted']) {
		foreach ($results['deleted'] as $entity_type => $bundles) {
			foreach ($bundles as $bundle => $details) {
				$count = $details['count'];
				$msg = "Deleted: $count records of type $entity_type, bundle $bundle.";
				if ($mode == 'live') { $msg .= " [LIVE MODE]"; } else { $msg .= " [TEST MODE, NO RECORDS DELETED]"; }
				drupal_set_message($msg);
			}
		}
	}
	if ($mode == 'live') { drupal_goto('<front>'); }
}

function sirius_domain_delete_inventory($domain_nid, $entity_type, $bundle) {
	$inventory = array();
	$sql = "select entity_type, bundle, entity_id from field_data_field_sirius_domain ";
	$sql .= "where field_sirius_domain_target_id = :domain_nid ";
	$sql .= "and entity_type = :entity_type ";
	$sql .= "and bundle = :bundle ";
	$sql .= "order by bundle, entity_id ";
	$stmt = db_query($sql, array(':domain_nid' => $domain_nid, ':entity_type' => $entity_type, ':bundle' => $bundle));
	while ($hr = $stmt->fetchAssoc()) {
		$inventory[] = $hr['entity_id'];
	}

	return $inventory;
}