<?php

// while true ; do drush --user=1 php-eval "sirius_require('sirius_smf', 'sirius_smf.oneoff_wb_end_of_month.inc'); sirius_smf_oneoff_end_of_month();" ; sleep 10; done

// drush --user=1 php-eval "sirius_require('sirius_smf', 'sirius_smf.oneoff_wb_end_of_month.inc'); sirius_smf_oneoff_end_of_month();"

function sirius_smf_oneoff_end_of_month() {
  $last = variable_get('sirius_smf_oneoff_end_of_month', 0);

  $sql = "select count(*) as c from node ";
  $sql .= "where node.type = 'sirius_trust_worker_benefit' ";
  $sql .= "and nid > $last ";
  $stmt = db_query($sql, array());
  $hr = $stmt->fetchAssoc();
  $total = $hr['c'];
  print "Total: $total\n";

  $limit = 1000;
  $sql = "select * from node ";
  $sql .= "where node.type = 'sirius_trust_worker_benefit' ";
  $sql .= "and nid > $last ";
  $sql .= "order by nid limit $limit ";

  $stmt = db_query($sql, array());

  $nids = array();

  $count = 0;
  while ($hr = $stmt->fetchAssoc()) {
    $nids[] = $hr['nid'];
  }

  foreach ($nids as $nid) {
    ++$count;
    $node = node_load($nid);
    print "Checking $count / $limit of $total: $nid\n";

    $end_date = $node->field_sirius_date_end['und'][0]['value'];
    if (!$end_date) { continue; }

    $ts = strtotime($end_date);
    $correct_end_date = date('Y-m-t 00:00:00', $ts);
    if ($end_date != $correct_end_date) {
      print "Correcting: $end_date to $correct_end_date for $nid\n";
      $node->field_sirius_date_end['und'][0]['value'] = $correct_end_date;
      node_save($node);
    }

    variable_set('sirius_smf_oneoff_end_of_month', $nid);
  }
}
