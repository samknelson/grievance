<?php

function sirius_vsam_oneoff_smf_dependents() {
	$path = DRUPAL_ROOT . '/' . drupal_get_path('module', 'sirius_vsam') . '/' . 'smf_dependents.txt';
  $fp = fopen($path, "r");
  if (!$fp) { die("Unable to open $path"); }

  $line_count = $start = $end = 1;

  while ($line = fgets($fp)) {
    $line = preg_replace('/[\x0\n\r]/', ' ', $line);

		// Get the length
    if (!preg_match('/PIC/', $line)) { continue; }
    $matches = array();
    preg_match('/PIC([^\.]*)\./', $line, $matches);
    $str = $matches[1];
    $str = trim($str);

    $length = '?';
    if (!$str) {
    	$length = 1;
    } else if (strlen($str) == 1) {
    	$length = 1; 
    } else if (strlen($str) == 2) {
    	$length = 2;
    } else if ($str == '999') {
    	$length = 3;
    } else if (preg_match('/\(/', $str)) {
    	$matches = array();
    	if (!preg_match('/\((\d+)\)/', $str, $matches)) { die("Can't parse: $str"); }
    	$length = $matches[1] + 0;
    }

    // Get the name
    $name = '';
    $matches = array();
    preg_match('/(\d+)\s+(BPA\-[^ ]+)\s/', $line, $matches);
    if ($matches) { $name = $matches[1] . ' ' . $matches[2]; }
    else { $name = '05 FILLER'; }

    // print "$line_count / $length / $name / $line\n";
    $end = $start + $length - 1;
    print ",$name,$start,-,$end\n";
    $start = $end+1;

    ++$line_count;
  }
}