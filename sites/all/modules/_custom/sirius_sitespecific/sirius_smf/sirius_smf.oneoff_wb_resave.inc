<?php

// while true ; do drush --user=1 php-eval "sirius_require('sirius_smf', 'sirius_smf.oneoff_wb_resave.inc'); sirius_minilog_echo_active(TRUE); sirius_smf_oneoff_wb_resave();" ; sleep 10; done

// drush --user=1 php-eval "sirius_require('sirius_smf', 'sirius_smf.oneoff_wb_resave.inc'); sirius_smf_oneoff_wb_resave();"

function sirius_smf_oneoff_wb_resave() {
  $last = variable_get('sirius_smf_oneoff_wb_resave_last', 0);

  $sql = "select count(*) as c from node ";
  $sql .= "where node.type = 'sirius_trust_worker_benefit' ";
  $sql .= "and nid > $last ";
  $stmt = db_query($sql, array());
  $hr = $stmt->fetchAssoc();
  $total = $hr['c'];

  $limit = 5000;
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
    node_save($node);
    print "Saved $count / $limit of $total: $nid\n";
    variable_set('sirius_smf_oneoff_wb_resave_last', $nid);
  }

}
