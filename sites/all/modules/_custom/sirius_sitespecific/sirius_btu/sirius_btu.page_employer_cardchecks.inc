<?php

function sirius_btu_node_tab_employer_cardchecks_access($employer_node) {
	// Staff can access
  if (user_access('sirius admin') || user_access('sirius trust admin') || user_access('sirius trust staff')) { return TRUE; }

  // Building reps can access their own shop
  if (user_access('sirius btu building rep')) {
  	$worker_node = sirius_worker_get();
  	if (!$worker_node) { return FALSE; }

  	$worker_employer_nid = $worker_node->field_grievance_shop['und'][0]['target_id'];
  	if ($worker_employer_nid == $employer_node->nid) { return TRUE; }
  }

  // Oh well.
  return FALSE;
}

function sirius_btu_node_tab_employer_cardchecks($employer_node) {
	$params = array();
	$params['employer_nid'] = $employer_node->nid;
	$params['statuses'] = array('accepted');
	$params['return_workers'] = TRUE;
	$params['nids_only'] = TRUE;
	$params['order'] = 'field_sirius_name_family, field_sirius_name_given, field_sirius_name_middle';
	$worker_nids = sirius_dispatch_search($params);
	if (!$worker_nids) { return sirius_page_error("No employees found."); }

	$instance = Sirius_Cardcheck::getInstance();
	$id = 'BTU_DUES';
	$cardcheck_definition = $instance->get_definition_by_id($id);
	if (!$cardcheck_definition) {
		return sirius_form_error("Please make sure that there is a card check definition with the ID \"$id\".");
	}

	$rows = array();

	foreach ($worker_nids as $worker_nid) {
		$msg = '';

		$search_params = array();
		$search_params['worker_nid'] = $worker_nid;
		$search_params['definition_nid'] = $cardcheck_definition->nid;
		$search_params['status'] = 'signed';
		$probe = $instance->search($search_params);

    $worker_node = node_load($worker_nid);

		if ($probe) {
			$cardcheck_node = reset($probe);

      $cardcheck_json = sirius_json_get($cardcheck_node);
      $cardcheck = $cardcheck_json['cardcheck'];

      $cardcheck_bu_nid = $cardcheck['bu'];
      $worker_bu_nid = $worker_node->field_sirius_bu['und'][0]['target_id'];
      if ($cardcheck_bu_nid == $worker_bu_nid) { continue; }
      $msg = "Card check on file has the wrong bargaining unit.";
		} else {
			$msg = "No signed card check on file.";
		}

		$row = array();
		$row[] = sirius_node_title($worker_nid);
		$row[] = $worker_node->field_sirius_email['und'][0]['value'];
		$row[] = $worker_node->field_sirius_phone['und'][0]['value'];
		$row[] = $msg;
		$rows[] = $row;
	}

	if (!$row) {
		return '<p>' . t("No workers found with missing or invalid card checks.") . '</p>';
	}

	$header = array();
	$header[] = 'Worker';
	$header[] = 'Email';
	$header[] = 'Phone';
	$header[] = '';

	return sirius_table_render($header, $rows);
}