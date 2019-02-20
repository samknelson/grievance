<?php

/**
 * Try: drush --user=1 php-eval "grievance_oneoffs_unitehere_local23contactsimport('                                                 /var/www/vhosts/grievances.unitehere23.org/private/emilio_10_6.csv');"
 */

function grievance_oneoffs_unitehere_local23contactsimport($path) {
	print "Starting UNITE HERE Local 23 Contacts Import\n";

	// Open the CSV
	$handle = fopen($path, "r");
	if (!$handle) { die("Unable to open $path."); }

	// Get the headers
	$headers = fgetcsv($handle, 10000, ",");
	$col = 0;
	foreach ($headers as $header) {
		$header_map[$header] = $col;
		++$col;
	}
	print "Headers:\n";
	print_r($header_map);

	// Load all rows into memory.
	$rows = array();
	while ($row = fgetcsv($handle, 10000, ",")) {
		$rows[] = $row;
	}
  fclose($handle);

  // For each row
  foreach ($rows as $row) {
  	// Find 
  }
}