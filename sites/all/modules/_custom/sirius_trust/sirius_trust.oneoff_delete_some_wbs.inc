<?php

// drush --user=1 php-eval "sirius_require('sirius_trust', 'sirius_trust.oneoff_delete_some_wbs.inc'); sirius_trust_oneoff_delete_subscriber_wbs(12345);"
function sirius_trust_oneoff_delete_subscriber_wbs($subscriber_nid, $mode = 'test') {
	sirius_domain(2457501);
	$params = array();
	$params['subscriber_nid'] = $subscriber_nid;
	print_r($params);
	$wb_nodes = sirius_trust_worker_benefits_search($params);
	foreach ($wb_nodes as $wb_nid => $wb_node) {
		print "Deleting: $wb_nid " . $wb_node->title . "\n";

		if ($mode == 'live') { node_delete($wb_nid); }
	}
}

