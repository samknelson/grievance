<?php

function sirius_smf_node_tab_election_oneoff_linked_benefit_form($form, &$form_state, $election_node) {
	$form_state['election_nid'] = $election_node->nid;

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

function sirius_smf_node_tab_election_oneoff_linked_benefit_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$election_node = node_load($form_state['election_nid']);
	$mode = $form_state['values']['mode'];

	$runner = Sirius_Smf_Batchrunner_Oneoff_Linked_Benefit::getInstance();
	$context = array('mode' => $form_state['values']['mode']);
	return $runner->process_record($form_state['election_nid'], $context);
	// return $runner->process_batch($context);
	// return sirius_smf_oneoff_linked_benefit($form_state['values']['mode'], $election_node);
}

sirius_require('sirius', 'Sirius/Singleton.php');

class Sirius_Batchrunner extends Sirius_Singleton {
	public $type = 'batchrunner_base';
	public $name = 'Batchrunner Base';

	public function mode($context) {
		return $context['mode'];
	}

	public function record_pk($record) {
		return $record->nid;
	}

	public function record_title($record) {
		return $record->title . ' (' . $this->record_pk($record) . ')';
	}

	public function batch_size(&$context = array()) {
		$batch_size = $context['batch_size'];
		if (!$batch_size) { $batch_size = $this->batch_size_default($context); }
		return $batch_size;
	}

	public function batch_size_default(&$context = array()) {
		return 100;
	}

	public function last_pk_variable_name(&$context = array()) {
		return 'sirius_batchrunner_' . $this->get_type();
	}

	public function last_pk(&$context = array()) {
		return variable_get($this->last_pk_variable_name($context), 0);
	}

	public function last_pk_set($pk, &$context = array()) {
		return variable_set($this->last_pk_variable_name($context), $pk);
	}

	public function get_next_batch(&$context = array()) {
		$batch_size = $this->batch_size($context);
		$last_pk = $this->last_pk($context);
		$content_type = $this->content_type_impl($context);
		if (!$content_type) { return array(); }

		$sql = "select nid from node where type = :content_type and nid > :last_pk order by nid limit $batch_size";
		$stmt = sirius_sql_query($sql, array(':content_type' => $content_type, ':last_pk' => $last_pk));

		$pks = array();
		while ($hr = $stmt->fetchAssoc()) {
			$pks[] = $hr['nid'];
		}
		print_r($pks);
		return $pks;
	}

	public function grand_total_pending(&$context = array()) {
		$content_type = $this->content_type_impl($context);
		if (!$content_type) { return 0; }

		$last_pk = $this->last_pk($context);
		$sql = "select count(*) as c from node where type = :content_type and nid > :last_pk ";
		$stmt = sirius_sql_query($sql, array(':content_type' => $content_type, ':last_pk' => $last_pk));
		$hr = $stmt->fetchAssoc();
		return $hr['c'];
	}

	public function process_batch(&$context = array()) {
		sirius_denorm_realtime(FALSE);
		$batch_size = $this->batch_size($context);
		$last_pk = $this->last_pk($context);
		
		$context['grand_total_pending'] = $this->grand_total_pending($context);

		$start_ts = time();

		sirius_minilog("BATCH STARTING");
		sirius_minilog("Type:       " . $this->get_type());
		sirius_minilog("Batch size: " . $batch_size);
		sirius_minilog("Last PK:    " . $last_pk);
		sirius_minilog("Remaining:  " . number_format($context['grand_total_pending']));
		sirius_minilog("Started:    " . date('c', $start_ts));

		$pks = $this->get_next_batch($context);
		if (!$pks) {
			sirius_minilog("No more records to process.");
			return array('success' =>TRUE);
		}

		$count = 0;
		$batch_size = count($pks);
		$batch_estimate_ts = 0;
		foreach ($pks as $pk) {
			sirius_minilog("Processing $count / $batch_size / $context[grand_total_pending] / " . format_interval($batch_estimate_ts));

			$result = $this->process_record($pk, $context);
			if ($this->mode($context) == 'live') {
				$this->last_pk_set($pk);
			}

			++$count;
			$ts = time();
			$elapsed_ts = $ts - $start_ts;
			$per_record_ts = $elapsed_ts / $count;
			$batch_estimate_ts = ($batch_size - $count) * $per_record_ts;
		}

		$end_ts = time();
		$elapsed_ts = $end_ts - $start_ts;
		$grand_estimate_ts = ($context['grand_total_pending'] - $batch_size) * $elapsed_ts / $batch_size;

		sirius_minilog("Ended:      " . date('c', $end_ts));
		sirius_minilog("Elapsed:    " . format_interval($elapsed_ts, 2));
		sirius_minilog("Records Left: " . number_format($context['grand_total_pending'] - $batch_size));
		sirius_minilog("Estimate Remaining: " . format_interval($grand_estimate_ts));
	}

	public function get_record($pk, &$context = array()) {
		return node_load($pk);
	}

	public function process_record($pk, &$context = array()) {
		$record = node_load($pk);
		$mode = $this->mode($context);

		sirius_minilog('Processing: ' . $this->record_title($record) . " (mode = $mode)");
		sirius_minilog_indent();
		$result = $this->process_record_impl($record, $context);
		sirius_minilog_outdent();

		return $result;
	}

	// Sirius_Batchrunner_Myclass::getInstance()->cl('test')
	public function cl($mode, $batch_size = 0, &$context = array()) {
		$context['mode'] = $mode;
		if ($batch_size) { $context['batch_size'] = $batch_size; }
		sirius_minilog_echo_active(TRUE);
		$this->process_batch($context);
	}

	public function process_record_impl($election_node, &$context = array()) {
		sirius_minilog("Please implement " . __METHOD__, 'error');
	}

	public function content_type_impl(&$context = array()) {
		sirius_minilog("Please implement " . __METHOD__, 'error');
	}
}

class Sirius_Smf_Batchrunner_Oneoff_Linked_Benefit extends Sirius_Batchrunner {
	public $type = 'smf_oneoff_linked_benefit';
	public $name = 'Sirius_Smf_Oneoff_Linked_Benefit';

	public function content_type_impl(&$context = array()) {
		return 'sirius_trust_worker_election';
	}

	public function process_record_impl($election_node, &$context = array()) {
		$linked_benefits = $this->linked_benefits();

		$benefit_nids = sirius_fieldvals($election_node, 'field_sirius_trust_benefits', 'target_id');

		$new_benefit_nids = array();
		foreach ($benefit_nids as $benefit_nid) {
			sirius_minilog("Checking: " . $this->benefit_name($benefit_nid));
			if (!$linked_benefits[$benefit_nid]) { continue; }

			sirius_minilog_indent();
			foreach ($linked_benefits[$benefit_nid] as $linked_benefit_nid) {
				sirius_minilog("Checking linked: " . $this->benefit_name($linked_benefit_nid));
				if (in_array($linked_benefit_nid, $benefit_nids)) { continue; }
				sirius_minilog("... Adding " . $this->benefit_name($linked_benefit_nid) . " to the election");
				$new_benefit_nids[] = $linked_benefit_nid;
			}
			sirius_minilog_outdent();
		}

		if (!$new_benefit_nids) {
			sirius_minilog("No changes needed");
			return; 
		}

		foreach ($new_benefit_nids as $new_benefit_nid) {
			$election_node->field_sirius_trust_benefits['und'][] = array('target_id' => $new_benefit_nid);
		}
		if ($this->mode($context) == 'live') {
			node_save($election_node);
			sirius_minilog("Election updated.");
		} else {
			sirius_minilog("(Skipping save because mode = " . $this->mode($context) . ')');
		}
	}

	public function benefit_name($benefit_nid) {
	  $names = &drupal_static(__FUNCTION__);
	  if (!$names) { $names = sirius_trust_benefit_options(FALSE); }
	  return $names[$benefit_nid];
	}

	public function linked_benefits() {
	  // Get the "linked" benefits
	  // - MLK should create:
	  // - - VSP Enhanced
	  // - - Express Scripts
	  // - - Hinge PT
	  // - - Life Insurance
	  // - - Carelon EAP
	  // - - Carelon Behavioral Health
	  // - - AD&D
	  // 
	  // - Healthnet, Kaiser & Kaiser Enhanced should create:
	  // - - VSP
	  // - - Express Scripts
	  // - - Life Insurance
	  // - - Carelon EAP
	  // - - AD&D

	  $linked_benefits = &drupal_static(__FUNCTION__);
	  if (!$linked_benefits) {
	    $map_by_id = array();
	    $map_by_id['M'] = array('3E', 'EXPRESSSCRIPTS', 'HINGEPT', '2', 'CARELONEAP', 'CARELONBEHAVIORALHEALTH', 'ADD');
	    $map_by_id['H'] = array('3', 'EXPRESSSCRIPTS', '2', 'CARELONEAP', 'ADD');
	    $map_by_id['K'] = $map_by_id['KE'] = $map_by_id['H'];

	    foreach ($map_by_id as $from_id => $to_ids) {
	      $from_nid = sirius_find_nid_by_id($from_id, 'sirius_trust_benefit');
	      if (!$from_nid) { return array('success' => FALSE, 'msg' => "Unable to find the benefit NID for ID [$from_id]"); }

	      foreach ($to_ids as $to_id) {
	        $to_nid = sirius_find_nid_by_id($to_id, 'sirius_trust_benefit');
	        if (!$to_nid) { return array('success' => FALSE, 'msg' => "Unable to find the benefit NID for ID [$to_id]"); }

	        $linked_benefits[$from_nid][] = $to_nid;
	      }
	    }
	  }

	  return $linked_benefits;
	}
}
