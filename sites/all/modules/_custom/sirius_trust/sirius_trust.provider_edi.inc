<?php

class Sirius_Trust_Provider_Edi_Report_Generator extends Report_Cache_Report_Generator {
	public function edi_output_stream($fp, $report_id) {
		return $this->edi_output_stream_csv($fp, $report_id);
	}

	public function edi_output_stream_csv($fp, $report_id) {
		$report_type = $this->type;

		$domain_nid = sirius_domain_nid(); if (!$domain_nid) { $domain_nid = 0; }

		$info = $this->info();

		$fields = $info['fields'];

		fputcsv($fp, $fields);

		$query = db_select('report_cache');
		$query->fields('report_cache');
		$query->condition('report_type', $report_type);
		$query->condition('report_id', $report_id);
		$query->condition('report_domain_nid', $domain_nid);
		$stmt = $query->execute();

		while ($hr = $stmt->fetchAssoc()) {
			$hr = $info['generator']->row_decode($hr['report_data']);
			$row = array();
			foreach ($fields as $name => $prompt) {
				$row[] = strip_tags($hr[$name]);
			}
			fputcsv($fp, $row);
		}
	}
}
