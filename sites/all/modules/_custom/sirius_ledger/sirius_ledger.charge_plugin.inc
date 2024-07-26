<?php

function sirius_ledger_charge_plugin_trigger_info_all() {
	return array(
		'hourly' => array('name' => 'Hourly'),
		'monthly' => array('name' => 'Monthly'),
		'manual' => array('name' => 'Manual'),
		'event' => array('name' => 'Event'),
	);
}

function sirius_ledger_charge_plugin_trigger_info($trigger_key) {
	return sirius_ledger_charge_plugin_trigger_info_all()[$trigger_key];
}

function sirius_ledger_charge_plugin_charge_all(&$context, $mode = 'test') {
	$trigger_key = $context['trigger'];
	if (!$trigger_key) {
		sirius_minilog('No trigger specified.', 'error');
		return;
	}
	$trigger_info = sirius_ledger_charge_plugin_trigger_info($trigger_key);
	if (!$trigger_info) {
		sirius_minilog("Unknown trigger: $trigger_key", 'error');
		return;
	}
	sirius_minilog("Generating charges: " . $trigger_info['name'] . " / mode = $mode");

	// Make sure that we have a policy node in our context
	$policy_node = sirius_ledger_charge_plugin_policy_lookup($context);

	// Find out what charge plugins we have
	$plugins = sirius_plugin_entity_plugins($context['policy_node'], 'ledger_charge');
	if ($context['plugin_uuid']) {
		$tmp = $context['plugin_uuid'];
		$plugins = array($tmp => $plugins[$tmp]);
	} 
	if (!$plugins) {
		sirius_minilog("No charge plugins were in effect for this context.");
		return array('success' => TRUE);
	}

	// Complete our context
	if (!$context['ts']) {
		$context['ts'] = time();
	}

	if ($context['worker_node'] && !$context['contact_node']) {
		$context['contact_node'] = node_load($context['worker_node']->field_sirius_contact['und'][0]['target_id']);
	}

	if ($context['worker_node'] && !$context['employer_node']) {
		$context['employer_node'] = node_load(sirius_dispatch_emphist_employer_nid_asof($context['worker_node']->nid, $context['ts']));
	}

	$total = $count = 0;
	foreach ($plugins as $plugin_uuid => $plugin) {
		$context['plugin_uuid'] = $plugin_uuid;
		$context['plugin'] = $plugin;

		sirius_minilog_indent();
		$result = sirius_ledger_charge_plugin_charge($context, $mode);
		sirius_minilog_outdent();

		if (!$result['success']) { sirius_minilog("Charge plugin $plugin_info[name] failed: $result[msg]", 'error'); }

		if ($result['amt']) {
			++$count;
			$total += $result['amt'];
		}
	}

	sirius_minilog("Generated $count charges for a total of \$" . number_format($total, 2));

	return array('success' => TRUE, 'amt' => $total);
}

function sirius_ledger_charge_plugin_charge(&$context, $mode = 'test') {
	$plugin = $context['plugin'];
	$plugin_uuid = $context['plugin_uuid'];

	sirius_minilog("Scanning: $plugin[title] ($plugin_uuid) ...");

	$plugin_key = $plugin['plugin'];
	if (!$plugin_key) { return array('success' => FALSE, 'msg' => "No plugin defined."); }

	$plugin_info = sirius_plugin_info('ledger_charge', $plugin_key);
	if (!$plugin_info) { return array('success' => FALSE, 'msg' => "Charge plugin not found: $plugin_key"); }

	if (!$plugin_info['instance']->trigger_enabled($context['trigger'])) { return array('success' => FALSE, 'msg' => "Charge plugin not enabled for $context[trigger]"); }

	$context['policy_settings'] = $plugin_info['instance']->entity_settings($context['policy_node'], $context['plugin_uuid']);
	if (!$context['policy_settings']) { return array('success' => FALSE, 'msg' => "No policy settings found."); }

	if ($plugin_info['instance']->employer_settings_enabled()) {
		$context['employer_settings'] = $plugin_info['instance']->employer_settings($context['employer_node'], $context['plugin_uuid']);
		if (!$context['employer_settings']) { return array('success' => FALSE, 'msg' => "No employer settings found."); }
	}

	sirius_minilog("Executing charge plugin: " . $plugin_info['name']);

	sirius_minilog(NULL, 'indent');
	$result = $plugin_info['instance']->charge($context, $mode);
	sirius_minilog(NULL, 'outdent');

	return $result;
}

function sirius_ledger_charge_plugin_policy_lookup(&$context) {
	if (!$context['policy_node']) {
		sirius_minilog_indent();
		$context['policy_node'] = sirius_ledger_policy_lookup($context['worker_node'], $context['employer_node'], $context['ts']);
		sirius_minilog_outdent();
	}
	return $context['policy_node'];
}

function sirius_ledger_charge_plugins_event() {
	// Use the global default policy node
	$policy_node = sirius_ledger_policy_lookup(NULL);
	if (!$policy_node) { return array(); }

	// Get the plugins 
	$items = sirius_plugin_entity_plugins($policy_node, 'ledger_charge');

	// Only plugins that implement event
	foreach ($items as $item_uuid => $item) {
		if (!$item['info']['instance']) { 
			unset($items[$item_uuid]);
			continue;
		}
		
		if (!$item['info']['instance']->trigger_enabled('event')) {
			unset($items[$item_uuid]);
			continue; 
		}
	}

	return $items;
}
