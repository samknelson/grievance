<?php

function sirius_log_buckets() {
	$buckets = &drupal_static(__FUNCTION__);

	if (!isset($buckets)) {
		$buckets = array(
			'dispatch' => array(
				'title' => 'Log',
				'content_type' => 'sirius_dispatch',
				'permissions' => array('sirius dispatch staff'),
				'path' => 'sirius_dispatch_log',
				'weight' => 20,
				'can_add' => TRUE,
			),

			'dispatch_job' => array(
				'title' => 'Log',
				'content_type' => 'sirius_dispatch_job',
				'permissions' => array('sirius dispatch staff'),
				'path' => 'sirius_dispatch_job_log',
				'weight' => 20,
				'can_add' => TRUE,
			),

			'dispatch_facility' => array(
				'title' => 'Log',
				'content_type' => 'sirius_dispatch_facility',
				'permissions' => array('sirius dispatch staff'),
				'path' => 'sirius_dispatch_facility_log',
				'weight' => 20,
				'can_add' => TRUE,
			),

			'dispatch_facility_hall' => array(
				'title' => 'Log',
				'content_type' => 'sirius_dispatch_facility_hall',
				'permissions' => array('sirius dispatch staff'),
				'path' => 'sirius_dispatch_facility_hall_log',
				'weight' => 20,
				'can_add' => TRUE,
			),

			'sirius_phonenumber' => array(
				'title' => 'Log',
				'content_type' => 'sirius_phonenumber',
				'permissions' => array('sirius domain admin'),
				'path' => 'sirius_phonenumber_log',
				'weight' => 20,
				'can_add' => TRUE,
			),

			'shop' => array(
				'title' => 'Log',
				'content_type' => 'grievance_shop',
				'permissions' => array('sirius employer admin'),
				'path' => 'grievance_shop_log',
				'weight' => 20,
				'can_add' => TRUE,
			),

			'payment' => array(
				'title' => 'Log',
				'content_type' => 'sirius_payment',
				'permissions' => array('sirius ledger admin'),
				'path' => 'sirius_payment_log',
				'weight' => 20,
				'can_add' => TRUE,
			),

			'payperiod' => array(
				'title' => 'Log',
				'content_type' => 'sirius_payperiod',
				'permissions' => array('sirius hour admin'),
				'path' => 'sirius_payperiod_log',
				'weight' => 20,
				'can_add' => TRUE,
			),

			'contact' => array(
				'title' => 'Log',
				'content_type' => 'sirius_contact',
				'permissions' => array('sirius dispatch staff'),
				'path' => 'sirius_contact_log',
				'weight' => 20,
				'can_add' => TRUE,
			),

			'worker' => array(
				'title' => 'Log',
				'content_type' => 'sirius_worker',
				'permissions' => array('sirius worker admin'),
				'path' => 'sirius_worker_log',
				'weight' => 20,
				'can_add' => TRUE,
			),

			'worker_tc' => array(
				'title' => 'Log',
				'content_type' => 'sirius_worker',
				'permissions' => array('sirius training staff'),
				'path' => 'sirius_worker_tc/log',
				'weight' => 20,
				'categories' => array('worker:level'),
				'can_add' => TRUE,
			),

			'worker_skill' => array(
				'title' => 'Log',
				'content_type' => 'sirius_worker',
				'permissions' => array('sirius training staff'),
				'path' => 'sirius_worker_skill/log',
				'weight' => 7,
				'categories' => array('worker:skill'),
				'can_add' => FALSE,
			),

			'worker_status' => array(
				'title' => 'Log',
				'content_type' => 'sirius_worker',
				'permissions' => array('sirius dispatch staff'),
				'path' => 'sirius_worker_status/log',
				'weight' => 50,
				'categories' => array('worker:work_status', 'worker:member_status'),
				'can_add' => FALSE,
			),

			'worker_dispatch' => array(
				'title' => 'Log',
				'content_type' => 'sirius_worker',
				'permissions' => array('sirius dispatch staff'),
				'path' => 'sirius_worker_dispatches/log',
				'weight' => 30,
				'categories' => array('worker:employer', 'dispatch:status', 'worker:dispstatus', 'worker:availdate', 'worker:dispstatus_block', 'dispatch:dnc'),
				'can_add' => FALSE,
			),

			'trust_worker_benefit' => array(
				'title' => 'Log',
				'content_type' => 'sirius_trust_worker_benefit',
				'permissions' => array('sirius domain admin'),
				'path' => 'sirius_log/log',
				'weight' => 7,
				'can_add' => FALSE,
			),
		);

		/*
		$cf_sets = sirius_customfield_set_list_all();
		if ($cf_sets) {
			$cf_nodes = node_load_multiple(array_keys($cf_sets));
			foreach ($cf_nodes as $cf_nid => $cf_node) {
				$cf_json = sirius_json_get($cf_node);

				if ($cf_json['customfield']['multiple'] != 'Yes') { continue; }
				$content_types = sirius_customfield_content_types($cf_json);

				$log_key = 'customfield:';
				if ($cf_json['machine_name']) { $log_key .= $cf_json['machine_name']; } else { $log_key .= $cf_nid; }

				foreach ($content_types as $content_type => $record) {
					$buckets["customfield_$cf_nid"] = array(
						'title' => $cf_node->title,
						'content_type' => $content_type,
						'permissions' => array($record['permission']),
						'path' => "customfield_$cf_nid",
						'weight' => 30,
						'categories' => array($log_key),
						'can_add' => TRUE,
						'customfield_nid' => $cf_nid,
					);
				}
			}
		}
		*/

		foreach ($buckets as $bucket_id => $bucket) {
			$buckets[$bucket_id]['id'] = $bucket_id;
		}

		if (sirius_domain_variable_get('sirius_log_quicklog_sirius_worker_bucketview', '') == 'Yes') {
			$buckets['worker']['views'] = array(
				'quicklog' => array(
					'title' => 'Quicklog Only',
					'filters' => array(
						'category_types' => array_keys(sirius_log_quicklog_category_type_options('sirius_worker')),
					),
				),
				'all' => array(
					'title' => 'All Entries',
				),
			);
		}

		drupal_alter('sirius_log_buckets', $buckets);

		$my_excluded_categories = sirius_log_my_excluded_categories();
		if ($my_excluded_categories) {
			foreach ($buckets as $bucket_id => $bucket) {
				if (!$buckets[$bucket_id]['categories_exclude']) { $buckets[$bucket_id]['categories_exclude'] = array(); }
				$buckets[$bucket_id]['categories_exclude'] = array_unique($buckets[$bucket_id]['categories_exclude'] + $my_excluded_categories);
			}
		}
	}

	return $buckets;
}

function sirius_log_bucket($bucket_id) {
	return sirius_log_buckets()[$bucket_id];
}
