<?php

function sirius_node_tab_sirius_domain_clone_access($domain_node) {
	if (user_access('sirius admin')) { return TRUE; }
	if (!user_access('sirius domain clone')) { return FALSE; }
	return sirius_domain_access($domain_node->nid);
}

function sirius_node_tab_sirius_domain_clone($domain_node) {
	return drupal_get_form('sirius_node_tab_sirius_domain_clone_form', $domain_node);
}

function sirius_node_tab_sirius_domain_clone_form($form, &$form_state, $source_domain_node) {
	$form_state['source_domain_nid'] = $source_domain_node->nid;

	$form['intro'] = array(
		'#markup' => '<div class="sirius_form_intro">' . t('The form below will make a copy FROM the domain <strong>@title</strong>. Settings and dropdown lists will be copied. This action cannot be undone. Content elements ("nodes") such as contacts, feeds, and events will not be copied.', array('@title' => $source_domain_node->title)) . '</div>',
	);

	$domains = sirius_domain_tree_render_as_select();
	$form['target_domain_nid'] = array(
		'#type' => 'select',
		'#options' => $domains,
		'#title' => t('Copy FROM @title INTO:', array('@title' => $source_domain_node->title)),
		'#description' => t('Settings FROM the current domain, @title, will be copied INTO the domain that you select.', array('@title' => $source_domain_node->title)),
		'#required' => TRUE,
	);

	$taxonomy_bundles = array();
	$sql = "select vid, machine_name, name from taxonomy_vocabulary order by name";
	$stmt = db_query($sql, array());
	while ($hr = $stmt->fetchAssoc()) { $taxonomy_bundles[$hr['machine_name']] = $hr['name']; }
	$form['taxonomy_term_bundles'] = array(
		'#type' => 'checkboxes',
		'#title' => 'Clone these taxonomy terms',
		'#options' => $taxonomy_bundles,
	);

	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#title' => t('Yes, clone into this domain.'),
		'#required' => TRUE,
	);

  $log = sirius_minilog_render_html();
  if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_sirius_domain_clone_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$source_domain_nid = $form_state['source_domain_nid'];
	$target_domain_nid = $form_state['values']['target_domain_nid'];
	if (!$source_domain_nid || !$target_domain_nid || $source_domain_nid == $target_domain_nid) { 
		drupal_set_message("Please make sure that the source and target domains are different.", 'error'); 
		return;
	}

	// Brute force: copy each taxonomy twice, with a reset in between. This is silly but what the heck.
	foreach ($form_state['values']['taxonomy_term_bundles'] as $ignore => $vocabulary_machine_name) {
		if ($vocabulary_machine_name) {
			sirius_minilog("Clone pass #1: $vocabulary_machine_name");
			sirius_domain_clone_taxonomy($vocabulary_machine_name, $source_domain_nid, $target_domain_nid);
		}
	}

	sirius_minilog("Resetting the reference cache.");
	sirius_domain_get_target_tid(0, 0, TRUE); 

	foreach ($form_state['values']['taxonomy_term_bundles'] as $ignore => $vocabulary_machine_name) {
		if ($vocabulary_machine_name) {
			sirius_minilog("Clone pass #2: $vocabulary_machine_name");
			sirius_domain_clone_taxonomy($vocabulary_machine_name, $source_domain_nid, $target_domain_nid);
		}
	}

	$sql = "select name from variable where name like :source_pattern order by name";
	$sql_args = array(':source_pattern' => "$source_domain_nid/%");
	$stmt = db_query($sql, $sql_args);
	while ($hr = $stmt->fetchAssoc()) {
		$name = $hr['name'];
		$value = variable_get($name);
		$name = preg_replace("/$source_domain_nid/", $target_domain_nid, $name);
		sirius_minilog("Setting $name to " . substr($value, 0, 100));
		variable_set($name, $value);
	}
}
