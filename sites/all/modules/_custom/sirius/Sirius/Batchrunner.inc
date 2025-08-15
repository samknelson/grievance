<?php

class Sirius_Batchrunner extends Sirius_Singleton {
	public $type = 'batchrunner_base';
	public $name = 'Batchrunner Base';

	public function mode($context) {
		return $context['mode'];
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

	public function reset(&$context = array()) {
		return $this->last_pk_set(0);
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
		$change_count = 0;
		foreach ($pks as $pk) {
			sirius_minilog("Processing " . number_format($count) . " / " . number_format($batch_size) . " / " . number_format($context['grand_total_pending']) . " / " . format_interval($batch_estimate_ts));

			$result = $this->process_record($pk, $context);
			if ($this->mode($context) == 'live') {
				$this->last_pk_set($pk);
			}
			if ($result['changed']) { ++$change_count; }

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
		sirius_minilog("Changed:    " . $change_count);
		sirius_minilog("Records Left: " . number_format($context['grand_total_pending'] - $batch_size));
		sirius_minilog("Estimate Remaining: " . format_interval($grand_estimate_ts));
	}


	public function process_record($pk, &$context = array()) {
		$record = $this->record_load($pk);
		if (is_object($record) && isset($record->field_sirius_domain)) {
			sirius_domain($record->field_sirius_domain['und'][0]['target_id']);
		}
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
		sirius_minilog('========= ========= ========= ========= ========= ========= ========= ');
		$this->process_batch($context);
	}

	public function get_next_batch(&$context = array()) {
		sirius_minilog("Please implement " . __METHOD__, 'error');
	}

	public function grand_total_pending(&$context = array()) {
		sirius_minilog("Please implement " . __METHOD__, 'error');
	}

	public function record_pk($record) {
		sirius_minilog("Please implement " . __METHOD__, 'error');
	}

	public function record_title($record) {
		sirius_minilog("Please implement " . __METHOD__, 'error');
	}

	public function record_load($pk, &$context = array()) {
		sirius_minilog("Please implement " . __METHOD__, 'error');
	}

	public function process_record_impl($record, &$context = array()) {
		sirius_minilog("Please implement " . __METHOD__, 'error');
	}

	public function content_type_impl(&$context = array()) {
		sirius_minilog("Please implement " . __METHOD__, 'error');
	}
}
