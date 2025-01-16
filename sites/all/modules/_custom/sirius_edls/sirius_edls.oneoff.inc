<?php

function sirius_edls_oneoff() {
	$sql = "select * from node where type = 'sirius_term_proxy' order by nid";
	$stmt = db_query($sql, array());
	while ($hr = $stmt->fetchAssoc()) {
		$nid = $hr['nid'];
		print "Deleting: $hr[title] / $nid\n";
		node_delete($nid);
	}
}
