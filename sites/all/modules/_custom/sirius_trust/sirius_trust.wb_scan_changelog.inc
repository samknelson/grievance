<?php

function sirius_trust_wb_scan_changelog_purge_cron($mode = 'test', $force = FALSE) {
	$ts = strtotime('-1 month');
	sirius_minilog("Purging as of " . date('c', $ts));

	if ($mode != 'live') {
		sirius_minilog("Can't run in test mode.", 'warning');
		return array('success' => TRUE);
	}

	$result = Sirius_Trust_Wb_Scan_Changelog::getInstance()->purge($ts);
	sirius_minilog($result['msg']);
	return array('success' => TRUE);
}

class Sirius_Trust_Wb_Scan_Changelog extends Sirius_Singleton {
	public function log($change_data) {
		$change_data['ts'] = time();


		$sql = 'insert into sirius_trust_wb_scan_changelog ';
		$sql .= '(ts, mode, scan, wb_nid, subscriber_worker_nid, dependent_worker_nid, relationship_nid, benefit_nid, msg, action) ';
		$sql .= 'values ';
		$sql .= '(:ts, :mode, :scan, :wb_nid, :subscriber_worker_nid, :dependent_worker_nid, :relationship_nid, :benefit_nid, :msg, :action) ';
		$stmt = sirius_sql_query(
			$sql, array(
				':ts' => $change_data['ts'],
				':mode' => $change_data['mode'],
				':scan' => $change_data['scan'],
				':wb_nid' => $change_data['wb_nid'],
				':subscriber_worker_nid' => $change_data['subscriber_worker_nid'],
				':dependent_worker_nid' => $change_data['dependent_worker_nid'],
				':relationship_nid' => $change_data['relationship_nid'],
				':benefit_nid' => $change_data['benefit_nid'],
				':msg' => $change_data['msg'],
				':action' => $change_data['action'],
			)
		);

		/*
		$parts = array();
		$parts[] = $change_data['scan'];
		$parts[] = sirius_node_title($change_data['subscriber_worker_nid']);
		$parts[] = sirius_node_title($change_data['dependent_worker_nid']);
		$parts[] = sirius_node_title($change_data['benefit_nid']);
		$parts[] = $change_data['action'];
		$parts[] = $change_data['msg'];
		drupal_set_message(join(' // ', $parts));
		*/
	}

	public function purge($ts) {
		$sql = "delete from sirius_trust_wb_scan_changelog where ts < :ts ";
		$stmt = sirius_sql_query($sql, array(':ts' => $ts));
		return array('success' => TRUE, 'rowcount' => $stmt->rowcount(), 'msg' => "Purged " . $stmt->rowcount() . " records.");
	}

	public function count() {
		$sql = "select count(*) as c from sirius_trust_wb_scan_changelog";
		$stmt = sirius_sql_query($sql, array());
		$hr = $stmt->fetchAssoc();
		return $hr['c'];
	}

	public function latest_record() {
		$sql = "select * from sirius_trust_wb_scan_changelog order by log_nid desc limit 1";
		$stmt = sirius_sql_query($sql, array());
		$hr = $stmt->fetchAssoc();
		return $hr;
	}

	public function render_record($change_data) {
		$html = '<div class="sirius_trust_wb_scan_changelog_entry">';

		$html .= sirius_fakerender_field('Log ID:', $change_data['log_nid']);
		$html .= sirius_fakerender_field('Date:', date('c', $change_data['ts']));
		$html .= sirius_fakerender_field('Mode:', $change_data['mode']);
		$html .= sirius_fakerender_field('WB:', l(sirius_node_title($change_data['wb_nid']), 'node/' . $change_data['wb_nid']));
		$html .= sirius_fakerender_field('Subscriber:', l(sirius_node_title($change_data['subscriber_worker_nid']), 'node/' . $change_data['subscriber_worker_nid']));
		$html .= sirius_fakerender_field('Dependent:', l(sirius_node_title($change_data['dependent_worker_nid']), 'node/' . $change_data['dependent_worker_nid']));
		$html .= sirius_fakerender_field('Benefit:', sirius_node_title($change_data['benefit_nid']));
		$html .= sirius_fakerender_field('Action:', $change_data['action']);
		$html .= sirius_fakerender_field('Message:', $change_data['msg']);

		$html .= '</div>';
		return $html;
	}

	public function output_stream($fp) {
		$sql = "select sirius_trust_wb_scan_changelog.*, ";
		$sql .= "wb_node.title as wb, ";
		$sql .= "subscriber_worker_node.title as subscriber, ";
		$sql .= "dependent_worker_node.title as dependent, ";
		$sql .= "benefit_node.title as benefit ";
		$sql .= "from sirius_trust_wb_scan_changelog ";
		$sql .= "left join node wb_node on wb_node.nid = wb_nid ";
		$sql .= "left join node subscriber_worker_node on subscriber_worker_node.nid = subscriber_worker_nid ";
		$sql .= "left join node dependent_worker_node on dependent_worker_node.nid = dependent_worker_nid ";
		$sql .= "left join node relationship_node on relationship_node.nid = relationship_nid ";
		$sql .= "left join node benefit_node on benefit_node.nid = benefit_nid ";
		$sql .= "order by log_nid desc ";

		$header = array();
		$header[] = 'log_nid';
		$header[] = 'ts';
		$header[] = 'mode';
		$header[] = 'scan';
		$header[] = 'wb_nid';
		$header[] = 'subscriber_worker_nid';
		$header[] = 'dependent_worker_nid';
		$header[] = 'relationship_nid';
		$header[] = 'benefit_nid';
		$header[] = 'msg';
		$header[] = 'action';
		$header[] = 'wb';
		$header[] = 'subscriber';
		$header[] = 'dependent';
		$header[] = 'benefit';

		fputcsv($fp, $header);
		$stmt = sirius_sql_query($sql, array());
		while ($hr = $stmt->fetchAssoc()) {
			$hr['ts'] = date('Y-m-d H:i:s', $hr['ts']);
			fputcsv($fp, array_values($hr));
		}
	}
}