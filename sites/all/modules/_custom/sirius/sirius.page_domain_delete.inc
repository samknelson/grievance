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

	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#title' => t('Yes, permanently and completely delete the domain @title. This action cannot be undone.', array('@title' => $domain_node->title)),
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

	$inventory = array();
	foreach ($domains_to_delete as $domain_nid => $domain_node) {
		$inventory = array_merge_recursive($inventory, sirius_domain_delete_inventory($domain_nid));
	}

	// Put the domain nodes themselves as the very end.
	foreach ($domains_to_delete as $domain_nid => $domain_node) {
		$inventory['zzz']['sirius_domain'][] = $domain_nid;
	}

	$operations = array();
	foreach ($inventory as $entity_type => $bundles) {
		foreach ($bundles as $bundle => $entity_ids) {
			if ($entity_type == 'zzz') { $entity_type = 'node'; }
			$chunks = array_chunk($entity_ids, 100, TRUE);
			foreach ($chunks as $chunk) {
				$operations[] = array(
					'sirius_domain_delete_batch', 
					array($mode, $entity_type, $bundle, array_values($chunk)),
				);
			}
		}
	}
	
	// Launch the batch processor.
  $batch = array(
    'operations' => $operations,
		'finished' => 'sirius_domain_delete_batch_finished',
  );
	batch_set($batch);
}

function sirius_domain_delete_batch($mode, $entity_type, $bundle, $entity_ids, &$context) {
	$count = count($entity_ids);
	$first = $entity_ids[0];
	$last = $entity_ids[$count-1];

	$msg = "Deleting: $entity_type, $bundle ($count entities, ids $first to $last)";

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
	foreach ($results['deleted'] as $entity_type => $bundles) {
		foreach ($bundles as $bundle => $details) {
			$count = $details['count'];
			$msg = "Deleted: $count records of type $entity_type, bundle $bundle.";
			if ($mode == 'live') { $msg .= " [LIVE MODE]"; } else { $msg .= " [TEST MODE, NO RECORDS DELETED]"; }
			drupal_set_message($msg);
		}
	}
	if ($mode == 'live') { drupal_goto('<front>'); }
}

function sirius_domain_delete_inventory($domain_nid) {
	$inventory = array();
	$sql = "select entity_type, bundle, entity_id from field_data_field_sirius_domain ";
	$sql .= "where field_sirius_domain_target_id = :domain_nid ";
	$sql .= "and bundle != 'sirius_domain' ";
	$sql .= "order by bundle, entity_id ";
	$stmt = db_query($sql, array(':domain_nid' => $domain_nid));
	while ($hr = $stmt->fetchAssoc()) {
		$inventory[$hr['entity_type']][$hr['bundle']][] = $hr['entity_id'];
	}

	return $inventory;
}