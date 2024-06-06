<?php

function sirius_ledger_monthly_charge_batch_reset() {
	sirius_domain_variable_set('sirius_ledger_monthly_batch_status', NULL);
}

function sirius_ledger_monthly_charge_batch($mode, $force, $options = array()) {
	$ts = $options['ts'];
	if (!$ts) { $ts = time(); }

	$batch_size = sirius_domain_variable_get('sirius_ledger_monthly_batch_size', 1000);
	$batch_status = json_decode(sirius_domain_variable_get('sirius_ledger_monthly_batch_status'), TRUE);
	$today = date('Y-m-d', $ts);
	if ($batch_status['date'] != $today) {
		$batch_status = array('date' => $today);
		sirius_minilog("Starting monthly charging for $today"); 
	} else {
		if ($batch_status['complete']) { 
			return array('success' => TRUE, 'msg' => "Batch processing for $today is complete."); 
		} else {
			sirius_minilog("-- Continuing previously started batch"); 
		}
	}

	$domain_nid = sirius_domain_nid();
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_worker');
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  if ($batch_status['last_worker_nid']) { $query->propertyCondition('nid', $batch_status['last_worker_nid'], '>'); }
  $query->propertyOrderBy('nid');
  $query->range(0, $batch_size);
  $stmt = $query->execute();
  $worker_nids = array();
  if ($stmt['node']) { $worker_nids = array_keys($stmt['node']); }

  $count = 0;
  foreach ($worker_nids as $worker_nid) {
  	sirius_ledger_monthly_charge_worker($worker_nid, $mode, $ts);
		$batch_status['last_worker_nid'] = $worker_nid;
		++$count;
  }
  if ($count < $batch_size) { 
  	$batch_status['complete'] = TRUE;
  	sirius_minilog("-- All records for $today processed."); 
  }

	if ($mode == 'live') { 
		sirius_domain_variable_set('sirius_ledger_monthly_batch_status', json_encode($batch_status));
	}
	return array('success' => TRUE, 'msg' => "Scanned $count workers.");
}

function sirius_ledger_monthly_charge_worker($worker_nid, $mode, $ts) {
	// Build the context
	$context = array();
	$context['worker_node'] = node_load($worker_nid);
	$context['ts'] = $ts;
	$context['trigger'] = 'monthly';
	sirius_minilog("Generating monthly charges for " . $context['worker_node']->title);
	sirius_minilog_indent();
	$result = sirius_ledger_charge_plugin_charge_all($context, $mode);
	sirius_minilog_outdent();
	return $result;
}

