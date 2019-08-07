<?php

function sirius_signature_render($account) {
	$html = '<div class="sirius_signature_wrapper">';

  $name = $account->name;
  $fid = $account->field_sirius_signature['und'][0]['fid'];
  if ($fid) { $file = file_load($fid); }
  if ($file) {
  	$url = file_create_url($file->uri);
  	$html .= '<img src="' . $url . '?loopback=1" alt="' . htmlspecialchars($name) . '" />';
  } else {
	  $html = htmlspecialchars($name);
	}
	$html .= '</div>';

	return $html;
}

/**
 * It turns out that when you combine filefield_paths, imagefield_crop, and private directories bad things happen.
 * In particular, imagefield_crop copies the original uploaded image before filefield_paths has a chance to rename it over to the private
 * directory. Which means that the original file is left sitting in an unprotected directory.
 * This function should be called from cron to delete the originals, leaving only the cropped versions.
 *
 * — In a hook_cron, fetch all entries from the file_managed / file_usage tables where
 * — — the timestamp is more than six hours ago
 * — — the “module” is imagefield_cache
 * — — The “path” is “public://filefield_paths”
 * — Call file_usage_delete() on each
 * — Call “file_delete()” on each
 * It means we won’t be storing the original file; the crop will have to happen during the upload. But since imagefield_crop doesn’t work with private files anyway, we already don’t have the ability to re-crop. So I think we’re good enough.
 */

function sirius_signature_cleanup() {
	$last_ts = variable_get('sirius_signature_cleanup_last_ts', 0);
	$now_ts = time();
	if ($now_ts - $last_ts < 60*60) { return; }
	variable_set('sirius_signature_cleanup_last_ts', $now_ts);

	$sql = "select * from file_managed, file_usage ";
	$sql .= "where file_managed.fid = file_usage.fid ";
	$sql .= "and module = 'imagefield_crop' ";
	$sql .= "and uri like '%filefield_paths%' ";
	$sql .= "and timestamp < :cutoff ";
	$stmt = db_query($sql, array(':cutoff' => $now_ts - 60*60));
	$to_delete = array();
	while ($hr = $stmt->fetchAssoc()) {
		$to_delete[] = $hr;
	}

	$count = 0;
	foreach ($to_delete as $hr) {
		++$count;
		$file = (object) $hr;
		file_usage_delete($file, 'imagefield_crop');
		file_delete($file);
		watchdog('sirius_signature_cleanup', 'Deleted imagefield_cache original file: <pre>' . print_r($hr, 1) . '</pre>');
	}
	watchdog('sirius_signature_cleanup', "Signature cleanup complete. Deleted $count files.");
}