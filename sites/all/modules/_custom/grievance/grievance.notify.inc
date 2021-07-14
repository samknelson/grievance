<?php

function grievance_notify_company_contacts($params = array()) {
	$shop_nid = $params['shop_nid'];
	$status_nid = $params['status_nid'];
	$include_contact_type_tid = $params['include_contact_type_tid'];
	$exclude_contact_type_tid = $params['exclude_contact_type_tid'];

	$contact_type_lookup = sirius_taxonomy_load('grievance_contact_types', 'tid', 'name');

	// Find shop contacts associated with the shop
	$recipients = array();
	$shop_node = node_load($shop_nid);

	if ($shop_nid) {
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'grievance_shop_contact');
	  $query->fieldCondition('field_grievance_shops', 'target_id', $shop_nid);
	  if ($include_contact_type_tid) {
	  	$query->fieldCondition('field_grievance_contact_types', 'tid', $include_contact_type_tid);
	  }
		$stmt = $query->execute();
		if ($stmt['node']) {
			$contact_nids = array_keys($stmt['node']);
			$contact_nodes = node_load_multiple($contact_nids);
			foreach ($contact_nodes as $contact_nid => $contact_node) {
				$contact_type_tids = sirius_fieldvals($contact_node, 'field_grievance_contact_types', 'tid');
				if ($exclude_contact_type_tid && in_array($exclude_contact_type_tid, $contact_type_tids)) { continue; }

				$recipient = array();

				$recipient['contact_type_names'] = array();
				foreach ($contact_type_tids as $contact_type_tid) {
					$recipient['contact_type_names'][] = $contact_type_lookup[$contact_type_tid];
				}

				$role = $contact_node->field_grievance_co_role['und'][0]['value'];
				if ($role) {
					$recipient['type'] = $role . t(' for ') . $shop_node->title;
				} else {
					$recipient['type'] = t('Shop contact for: ') . $shop_node->title;
				}

				$recipient['nid'] = $contact_node->nid;
				$recipient['name'] = $contact_node->field_grievance_co_name['und'][0]['value'];
				$recipient['email'] = $contact_node->field_grievance_co_email['und'][0]['email'];
				$recipient['phone'] = $contact_node->field_grievance_co_phone['und'][0]['value'];
				$recipient['fax'] = $contact_node->field_grievance_co_fax['und'][0]['value'];
				$recipient['shop_contact'] = TRUE;
				$recipient['contact_types'] = $contact_node->field_grievance_contact_types['und'];
				$recipients[$contact_node->nid] = $recipient;
			}
		}
	}

	// Find contacts associated with the company
	$company_nid = $shop_node->field_grievance_company['und'][0]['target_id'];
	$company_node = node_load($company_nid);

	if ($company_nid) {
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'grievance_shop_contact');
	  $query->fieldCondition('field_grievance_company', 'target_id', $company_nid);
	  if ($include_contact_type_tid) {
	  	$query->fieldCondition('field_grievance_contact_types', 'tid', $include_contact_type_tid);
	  }
		$stmt = $query->execute();
		if ($stmt['node']) {
			$contact_nids = array_keys($stmt['node']);
			$contact_nodes = node_load_multiple($contact_nids);
			foreach ($contact_nodes as $contact_nid => $contact_node) {

				if ($exclude_contact_type_tid) {
					$found = FALSE;
					if ($contact_node->field_grievance_contact_types) {
						foreach ($contact_node->field_grievance_contact_types['und'] as $tmp) {
							if ($tmp['tid'] == $exclude_contact_type_tid) {
								$found = TRUE;
							}
						}
					}
					if ($found) { continue; }
				}

				$recipient = array();

				$role = $contact_node->field_grievance_co_role['und'][0]['value'];
				if ($role) {
					$recipient['type'] = $role . t(' for ') . $company_node->title;
				} else {
					$recipient['type'] = t('Copmany contact for: ') . $company_node->title;
				}

				$recipient['nid'] = $contact_node->nid;
				$recipient['name'] = $contact_node->field_grievance_co_name['und'][0]['value'];
				$recipient['email'] = $contact_node->field_grievance_co_email['und'][0]['email'];
				$recipient['company_contact'] = TRUE;
				$recipient['phone'] = $contact_node->field_grievance_co_phone['und'][0]['value'];
				$recipient['fax'] = $contact_node->field_grievance_co_fax['und'][0]['value'];
				$recipient['contact_types'] = $contact_node->field_grievance_contact_types['und'];
				$recipients[$contact_node->nid] = $recipient;
			}
		}
	}

	return $recipients;
}

function grievance_email_trim($email, $length) {
	if (strlen($email) < $length) { return $email; }
	return substr($email, 0, $length-3) . '&hellip;';
}
