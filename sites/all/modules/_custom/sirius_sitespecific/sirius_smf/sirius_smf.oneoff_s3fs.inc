<?php

function sirius_smf_oneoff_s3fs() {
	$sql = "select * from file_managed order by fid desc";
	$stmt = db_query($sql, array());
	while ($hr = $stmt->fetchAssoc()) {
		$uri = $hr['uri'];
		if (!preg_match('/^private/', $uri)) { continue; }

		file_unmanaged_copy($uri, 'public://ick');
		print "Processing: $uri\n";
		//break;
	}
	print "Dammit\n";
}