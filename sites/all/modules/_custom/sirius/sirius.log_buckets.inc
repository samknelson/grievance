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
				'permissions' => array('sirius dispatch staff'),
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

			'worker_ws' => array(
				'title' => 'Log',
				'content_type' => 'sirius_worker',
				'permissions' => array('sirius dispatch staff'),
				'path' => 'sirius_worker_work_status/log',
				'weight' => 7,
				'categories' => array('worker:work_status'),
				'can_add' => FALSE,
			),

			'worker_ms' => array(
				'title' => 'Log',
				'content_type' => 'sirius_worker',
				'permissions' => array('sirius dispatch staff'),
				'path' => 'sirius_worker_industry/log',
				'weight' => 7,
				'categories' => array('worker:member_status'),
				'can_add' => FALSE,
			),

			'worker_dispatch' => array(
				'title' => 'Log',
				'content_type' => 'sirius_worker',
				'permissions' => array('sirius dispatch staff'),
				'path' => 'sirius_worker_dispatches/log',
				'weight' => 7,
				'categories' => array('worker:employer', 'dispatch:status', 'worker:dispstatus', 'worker:availdate', 'worker:dispstatus_block', 'dispatch:dnc'),
				'can_add' => FALSE,
			),
		);

		foreach ($buckets as $bucket_id => $bucket) {
			$buckets[$bucket_id]['id'] = $bucket_id;
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