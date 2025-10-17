<?php

sirius_require('sirius', 'Sirius/Batchrunner.inc');
sirius_require('sirius', 'Sirius/Batchrunner_Node.inc');

// drush --user=1 php-eval "sirius_require('sirius_smf', 'sirius_smf.oneoff_election_add.inc'); Sirius_Smf_Batchrunner_Oneoff_Election_Add::getInstance()->cl('test');"


function sirius_smf_node_tab_oneoff_election_add_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	$form['#tree'] = TRUE;
	$form['mode'] = array(
		'#type' => 'select',
		'#title' => t('Mode'),
		'#options' => array(
			'test' => t('Test'),
			'live' => t('Live'),
		),
		'#require' => TRUE,
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

  $log = sirius_minilog_render_html();
  if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

	return $form;
}

function sirius_smf_node_tab_oneoff_election_add_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$worker_node = node_load($form_state['worker_nid']);
	$mode = $form_state['values']['mode'];

	$runner = Sirius_Smf_Batchrunner_Oneoff_Election_Add::getInstance();
	$context = array('mode' => $form_state['values']['mode']);
	return $runner->process_record($form_state['worker_nid'], $context);
	// return $runner->process_batch($context);
	// return sirius_smf_oneoff_linked_benefit($form_state['values']['mode'], $election_node);
}

class Sirius_Smf_Batchrunner_Oneoff_Election_Add extends Sirius_Batchrunner_Node {
	public $type = 'smf_oneoff_election_add';
	public $name = 'Sirius_Smf_Oneoff_Election_Add';




	public function get_next_batch(&$context = array()) {
		$batch_size = $this->batch_size($context);
		$last_pk = $this->last_pk($context);
		$all_worker_nids = $this->worker_nids();

		$pks = array();
		$count = 0;
		foreach ($all_worker_nids as $worker_nid) {
			if ($worker_nid < $last_pk) { continue; }
			$pks[] = $worker_nid;
			++$count;
			if ($count >= $batch_size) { break; }
		}

		return $pks;
	}

	public function grand_total_pending(&$context = array()) {
		$all_worker_nids = $this->worker_nids();
		$last_pk = $this->last_pk($context);
		$all_worker_nids = $this->worker_nids();

		foreach ($all_worker_nids as $worker_nid) {
			if ($worker_nid < $last_pk) { continue; }
			++$count;
		}

		return $count;
	}

	public function content_type_impl(&$context = array()) {
		return 'sirius_trust_worker';
	}

	public function process_record_impl($worker_node, &$context = array()) {
		$lookups = &drupal_static(__FUNCTION__);
		if (!isset($lookups['add'])) {
			$lookups['add'] = sirius_find_nid_by_id('ADD', 'sirius_trust_benefit');
			$lookups['2'] = sirius_find_nid_by_id('2', 'sirius_trust_benefit');
		}
		if (!$lookups['add'] || !$lookups['2']) {
			sirius_minilog("Didn't find benefit NID for [ADD] or [2]", 'error');
			return;
		}

		$election_node = sirius_trust_worker_election($worker_node);
		if (!$election_node) {
			sirius_minilog("No currently active election found. Skipping.");
			return array('success' => TRUE);
		}

		$probe = sirius_fieldvals($election_node, 'field_sirius_trust_benefits', 'target_id');

		if (in_array($lookups['add'], $probe) && in_array($lookups['2'], $probe)) {
			sirius_minilog("Worker had both benefits previously.");
			return array('success' => TRUE);
		}

		$probe[] = $lookups['add'];
		$probe[] = $lookups['2'];
		$probe = array_unique($probe);

		$newval = array();
		foreach ($probe as $nid) { $newval[] = array('target_id' => $nid); }
		$election_node->field_sirius_trust_benefits['und'] = $newval;

		$mode = $this->mode($context);
		if ($mode != 'live') {
			sirius_minilog("(Skipping save because mode = $mode)");
			return array('success' => TRUE);
		}

		node_save($election_node);
		sirius_minilog("*** Election saved: " . $election_node->title . ' (' . $election_node->nid . ')');
	}

	public function worker_nids() {
		sirius_require('sirius_smf', 'sirius_smf.oneoff_election_add.nids.inc');
		$nids = sirius_smf_oneoff_election_add_nids();
		sort($nids);
		return $nids;
	}
}
