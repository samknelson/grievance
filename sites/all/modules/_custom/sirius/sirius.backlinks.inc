<?php

/**
 * @todo: Rather than messing about with themeing, I'm just sticking the markup in #markup. Lazy.
 */

function sirius_quickactions_add($link = array()) {
	$links = &drupal_static(__FUNCTION__);
  if (!isset($links)) { $links = array(); }
  if ($link) { $links[] = $link; }
  return $links;
}

function sirius_quickactions_render() {
	$node = menu_get_object('node');
	if ($node && is_object($node)) {
		if ($node->type == 'sirius_worker' && user_access('sirius worker admin')) {
			sirius_quickactions_add(
				array(
					'path' => 'sirius/modal/' . $node->nid . '/quicklog',
					'title' => 'Quick Log',
				)
			);
		}

		if (user_access('sirius bookmark')) {
			if (in_array($node->type, sirius_bookmark_types())) {
				sirius_quickactions_add(
					array(
						'flag' => 'bookmarks',
						'nid' => $node->nid,
					)
				);
			}
		}
	}

	$links = sirius_quickactions_add();
	if (!$links) { return array(); }

	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/quickactions.css');

	foreach ($links as $delta => $link) {
		$links[$delta] = array(
			'#markup' => sirius_sirius_quickaction_render($link),
			'#weight' => 1000,
		);
	}
	return $links;
}

function sirius_sirius_quickaction_render($link) {
  $markup = '<li><span class="sirius_quickaction_link_wrapper">';
	if ($link['flag']) {
		$markup .= flag_create_link($link['flag'], $link['nid']);
	} else {
	  $markup .= l('<span class="sirius_quickaction_link_inner">&neArr; ' . $link['title'] . '</span>', $link['path'], array('html' => TRUE, 'attributes' => array('class' => array('sirius_quickaction_link'))));
	}
  $markup .= '</span></li>';
  return $markup;
}

function sirius_backlink_add($path = NULL, $link = NULL) {
	$links = &drupal_static(__FUNCTION__);
  if (!isset($links)) { $links = array(); }
  if (!$path) { return $links; }
  $links[$path] = $link;
  return $links;
}

function sirius_backlinks_render() {
	$links = sirius_backlink_add();

	$node = menu_get_object('node');
	if ($node && is_object($node)) { $links += sirius_backlinks_from_node($node); }

	$term = menu_get_object('taxonomy_term', 2);
	if ($term && is_object($term)) { $links += sirius_backlinks_from_term($term); }

	if ($_GET['destination']) {
		$links[$_GET['destination']] = array('name' => 'Cancel');
	}

	if (!$links) { return; }

	$html = '';
	foreach ($links as $path => $link) { 
		if (drupal_valid_path($path)) {
			$html .= l("&lArr; " . $link['name'], $path, array('html' => TRUE, 'attributes' => array('title' => $link['title'], 'class' => array('backlink'))));
		}
	}

	return $html;
}

function sirius_backlinks_from_term($term) {
	$links = array();
	$vid = $term->vid;
	$vocabulary = taxonomy_vocabulary_load($vid);
	$path = 'sirius/taxonomy/' . $term->vocabulary_machine_name;
	$links[$path] = array('name' => 'All Options', 'title' => $vocabulary->name);
	return $links;
}

function sirius_backlinks_from_node($node) {
	$links = array();

	if ($node->type == 'sirius_contact') { $contact_nid = $node->nid; }
	if (!$contact_nid) { $contact_nid = $node->field_sirius_contact['und'][0]['target_id']; }
	if ($contact_nid) { $worker_nid = sirius_worker_get_by_contact_nid($contact_nid, TRUE); }
	if ($contact_nid && !$worker_nid) {
		$name = 'Contact'; 

		if ($node->type == 'sirius_event_participant') 			{ $path = "node/$contact_nid"; }
		else if ($node->type == 'sirius_contact_relationship') { $path = "node/$contact_nid/sirius_contact_relationships"; $name = 'FROM Contact'; }
		else 																								{ $path = ""; }

		if ($path) { $links[$path] = array('name' => $name, 'title' => sirius_backlink_title($contact_nid)); }
	}

	if (!$worker_nid) { $worker_nid = $node->field_sirius_worker['und'][0]['target_id']; }
	if ($worker_nid) {
		$name = 'Worker';

		if ($node->type == 'sirius_payperiod') 										{ $path = "node/$worker_nid/sirius_worker_hours"; }
		else if ($node->type == 'sirius_dispatch') {
			if (preg_match('/emphist/', current_path()))            { $path = "node/$worker_nid/sirius_worker_emphist"; }
			else if (sirius_domain_component_enabled('dispatch:live'))     { $path = "node/$worker_nid/sirius_worker_dispatches"; }
			else if (sirius_domain_component_enabled('dispatch:emphist'))  { $path = "node/$worker_nid/sirius_worker_emphist"; }
			else                                                    { $path = "node/$worker_nid"; }
		} else if ($node->type == 'sirius_event_participant') 		{ $path = "node/$worker_nid/sirius_worker_events"; }
		else if ($node->type == 'sirius_contact_relationship') 		{ $path = "node/$worker_nid/sirius_worker_relationships"; $name = 'FROM Worker'; }
		else if ($node->type == 'sirius_trust_worker_benefit') 		{ $path = "node/$worker_nid/sirius_trust_worker_benefits"; $name = 'Dependent'; }
		else if ($node->type == 'sirius_trust_worker_election') 	{ $path = "node/$worker_nid/sirius_trust_worker_election"; }
		else if ($node->type == 'sirius_trust_worker_beneficiary'){ $path = "node/$worker_nid/sirius_trust_worker_beneficiary"; }
		else if ($node->type == 'sirius_worker')									{ $path = ""; }
		else 																											{ $path = "node/$worker_nid"; }

		if ($path) { $links[$path] = array('name' => $name, 'title' => sirius_backlink_title($worker_nid)); }
	}

	if ($node->type == 'sirius_contact_relationship') { 
		$other_contact_nid = $node->field_sirius_contact_alt['und'][0]['target_id'];
		$other_worker_nid = sirius_worker_get_by_contact_nid($other_contact_nid, TRUE);
		if ($other_worker_nid) {
			$links["node/$other_worker_nid/sirius_worker_relationships"] = array('name' => 'TO Worker', 'title' => sirius_backlink_title($other_worker_nid));
		} else {
			$links["node/$other_contact_nid/sirius_worker_relationships"] = array('name' => 'TO Contact ', 'title' => sirius_backlink_title($other_contact_nid));
		}
	}

	if ($node->type == 'sirius_trust_worker_benefit') {
		$relationship_nid = $node->field_sirius_contact_relation['und'][0]['target_id'];
		if ($relationship_nid) {
			$relationship_node = node_load($relationship_nid);
			$subscriber_contact_nid = $relationship_node->field_sirius_contact['und'][0]['target_id'];
			if ($subscriber_contact_nid) {
				$subscriber_worker_nid = sirius_worker_get_by_contact_nid($subscriber_contact_nid, TRUE);	
			}
			if ($subscriber_worker_nid) {
				$links["node/$subscriber_worker_nid/sirius_trust_worker_benefits"] = array('name' => 'Subscriber', 'title' => sirius_backlink_title($subscriber_worker_nid));
			}
			$links["node/$relationship_nid"] = array('name' => 'Relationship', 'title' => '');
		}
	}

	if ($node->type == 'sirius_operator') { $links['sirius/office/operator/list'] = array('name' => 'Operator List'); }
	if ($node->type == 'sirius_help') { $links['sirius/admin/help'] = array('name' => 'All Help Text'); }
	if ($node->type == 'sirius_phonenumber') { $links['sirius/phonenumber'] = array('name' => 'Phone Number List'); }
	if ($node->type == 'sirius_trust_benefit') { $links['sirius/trust/benefits'] = array('name' => 'Benefit List'); }
	if ($node->type == 'sirius_letterhead') { $links['sirius/admin/letterhead'] = array('name' => 'Letterhead List'); }
	if ($node->type == 'sirius_news') {
		$links['sirius/news'] = array('name' => 'News');
		$links['sirius/news/admin'] = array('name' => 'Admin');
	}

	if ($node->type == 'grievance_contract_template') {
		$links['grievance/admin/contract-templates'] = array('name' => 'Contract List');
	}

	if ($node->type == 'grievance_contract_section' || $node->type == 'grievance_contract_clause') {
		$template_nid = $node->field_grievance_contract_tplt['und'][0]['target_id'];
		if ($template_nid) {
			$links["node/$template_nid/section/list"] = array('name' => 'Template', 'title' => sirius_backlink_title($template_nid));
		}
	}

	if ($node->type == 'grievance_contract_clause') {
		$section_nid = $node->field_grievance_contract_section['und'][0]['target_id'];
		if ($section_nid) {
			$links["node/$section_nid//clauses/list"] = array('name' => 'Article', 'title' => sirius_backlink_title($section_nid));
		}
	}

	if ($node->type == 'sirius_feed') {
		$feed_type = $node->field_sirius_type['und'][0]['value'];
		if ($feed_type) {
			$path = "sirius/feeds/$feed_type";
			$links[$path] = array('name' => 'Feed List', 'title' => '');
		}
	}

	$job_nid = $node->field_sirius_dispatch_job['und'][0]['target_id'];
	if ($job_nid) {
		$path = "node/$job_nid/sirius_dispatch_job_dispatches";
		if ($path) { $links[$path] = array('name' => 'Job', 'title' => sirius_backlink_title($job_nid)); }
	}

	$employer_nid = $node->field_grievance_shop['und'][0]['target_id'];
	if ($employer_nid) {
		if ($node->type == 'sirius_payperiod') { 
			/*
			$ts = sirius_date_ts($node, 'field_sirius_datetime');
			$date = gmdate('Y-m', $ts);
			list ($year, $month) = explode('-', $date);
			$path = "node/$employer_nid/sirius_employer_hours/$year/$month";
			*/
			$path = '';
		}
		else if ($node->type == 'sirius_worker') 		{ $path = ''; }
		else 																				{ $path = "node/$employer_nid"; }

		if ($path) { $links[$path] = array('name' => 'Employer', 'title' => sirius_backlink_title($employer_nid)); }
	}

	$event_nid = $node->field_sirius_event['und'][0]['target_id'];
	if ($event_nid) {
		$path = "node/$event_nid/sirius_event_info";
		if ($path) { $links[$path] = array('name' => 'Event', 'title' => sirius_backlink_title($event_nid)); }
	}

	if ($node->type == 'sirius_log') {
		$handler_nids = sirius_fieldvals($node, 'field_sirius_log_handler', 'target_id');
		if ($handler_nids) {
			$sql = "select nid, type, title, field_sirius_category_value from node ";
			$sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
			$sql .= "where nid in (:handler_nids)";
			$sql_args = array(':handler_nids' => $handler_nids);
			$stmt = sirius_sql_query($sql, $sql_args);
			$log_type = $node->field_sirius_type['und'][0]['value'];
			$log_category = $node->field_sirius_category['und'][0]['value'];
			while ($hr = $stmt->fetchAssoc()) {
				$handler_type = $hr['type'];
				$handler_nid = $hr['nid'];
				$handler_category = $hr['field_sirius_category_value'];
				$handler_log_type = $hr['field_sirius_type_value'];

				$name = sirius_content_type_name($handler_type);
				$path = '';

				if ($log_category == 'customfield') { $path = "node/$handler_nid/sirius_dispatch_job_group_customfield_tab/$log_type"; }
				else if ($handler_type == 'sirius_event' && $log_category == 'ledger:auth')									{ $path = "node/$handler_nid/sirius_event_auths"; }
				else if ($handler_type == 'sirius_event' && $log_category == 'event:intake_application'){ $path = "node/$handler_nid/sirius_event_intake/applications"; }
				else if ($handler_type == 'sirius_event') 																							{ $path = "node/$handler_nid/sirius_event_info"; }
				else if ($handler_type == 'sirius_worker' && $log_category == 'worker:certification') 	{ $path = "node/$handler_nid/sirius_worker_tc/certifications"; }
				else if ($handler_type == 'sirius_worker' && $log_category == 'tor') 										{ $path = "node/$handler_nid/sirius_worker_schedule/tor"; }
				else if ($handler_type == 'sirius_worker' && $log_category == 'steward:checkin')				{ $path = "node/$handler_nid/sirius_worker_steward/checkin"; }
				else if ($handler_type == 'grievance_shop' && $log_category == 'steward:sched')					{ $path = "node/$handler_nid/sirius_employer_steward/sched"; }
				else if ($handler_type == 'sirius_worker' && $log_category == 'steward:sched')					{ $path = "node/$handler_nid/sirius_worker_steward/checkin-history"; }
				else if ($handler_type == 'sirius_worker' && $log_category == 'cardcheck')							{ 
					$path = "node/$handler_nid/sirius_worker_cardchecks";
					$name = "All Card Checks";
				}
				else if ($log_category == 'letter') {
					$path = "node/$handler_nid/${handler_type}_letter";
				}
				else if ($handler_type == 'sirius_log') {
					if ($handler_category == 'twilio:recording') { 
						$path = "node/$handler_nid/sirius_twilio_recording";
						$name = "Recording";
					} else if ($handler_category == 'twilio:call') { 
						$path = "node/$handler_nid/sirius_twilio_call";
						$name = "Call";
					} else {
						$path = "node/$handler_nid";
					}
				}
				else if ($handler_type == 'grievance_shop' && $log_category == 'labproj') {
					$path = "node/$handler_nid/employer_labproj";
					$name = 'Employer Labor Projections';
				}
				else if ($handler_type == 'sirius_dispatch_facility' && $log_category == 'steward:checkin') {
					$path = "node/$handler_nid/sirius_steward_dispatch_facility_checkin";
					$name = 'Facility';
				}
				else if ($handler_type == 'sirius_json_definition') { $path = ''; }
				else { $path = "node/$handler_nid/${handler_type}_log"; }

				$links[$path] = array('name' => $name, 'title' => $hr['title']);
			}
		}

		if ($log_category == 'customfield') {
			$set_nid = $log_type;
			$links["node/$set_nid/sirius_event_participant_definition_form"] = array('name' => 'Custom Field Set Definition', 'title' => sirius_backlink_title($set_nid));
		}
	}

	if ($node->type == 'sirius_json_definition') {
		$definition_type = $node->field_sirius_type['und'][0]['value'];
		if ($definition_type == 'sirius_event:catalog')	{ $links['sirius/event/catalog/list'] = array('name' => 'All Catalogs'); }
		else if ($definition_type == 'sirius_ledger:policy') { $links['sirius/config/ledger/policies'] = array('name' => 'All Policies'); }
		else if ($definition_type == 'customfield') { $links['sirius/customfield/definition/list'] = array('name' => 'All Custom Field Sets'); }
		else if ($definition_type == 'sirius:disclaimer') { $links['sirius/disclaimer/list'] = array('name' => 'All Disclaimers'); }
		else if ($definition_type == 'sirius:cardcheck') { $links['sirius/cardcheck/definitions'] = array('name' => 'All Card Checks'); }
	}

	if ($node->type == 'grievance_basic_page') {
		$links['grievance/admin/basic-pages'] = array('name' => 'All Basic Pages');
	}

	if ($node->type == 'sirius_trust_provider') {
		$links['sirius/trust/providers'] = array('name' => 'All Providers');
	}

	if ($node->type == 'sirius_trust_service') {
		$links['sirius/trust/services'] = array('name' => 'All Services');
	}

	if ($node->type == 'sirius_ledger_account') {
		$links['sirius/ledger/accounts'] = array('name' => 'All Accounts');
	}

	drupal_alter('sirius_backlinks_from_node', $links, $node);

	return $links;
}

function sirius_backlink_title($nid) {
	$sql = "select title from node where nid = :nid";
	$sql_args = array(':nid' => $nid);
	$stmt = sirius_sql_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();
	return $hr['title'];
}