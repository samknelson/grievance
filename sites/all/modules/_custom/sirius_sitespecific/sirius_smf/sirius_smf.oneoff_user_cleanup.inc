<?php

// while true ; do drush --user=1 php-eval "sirius_require('sirius_smf', 'sirius_smf.oneoff_user_cleanup.inc'); sirius_minilog_echo_active(TRUE); sirius_smf_oneoff_user_cleanup();" ; sleep 10; done

// drush --user=1 php-eval "sirius_require('sirius_smf', 'sirius_smf.oneoff_user_cleanup.inc'); sirius_minilog_echo_active(TRUE); sirius_smf_oneoff_user_cleanup();"

function sirius_smf_oneoff_user_cleanup() {
  $sql = "select count(*) as c from users ";
  // $sql .= "where mail like 'worker_%' ";
  // $sql .= "and mail like '%teamsters631%' ";
  $sql .= "where name rlike '^[0-9]+$' ";
  $stmt = db_query($sql, array());
  $hr = $stmt->fetchAssoc();
  $total = $hr['c'];

  $limit = 5000;
  $sql = "select * from users ";
  // $sql .= "where mail like 'worker_%' ";
  // $sql .= "and mail like '%teamsters631%' ";
  // $sql .= "and name rlike '^[0-9]+$' ";
  $sql .= "where name rlike '^[0-9]+$' ";
  $sql .= "order by uid limit $limit ";
  $stmt = db_query($sql, array());

  $uids = array();
  while ($hr = $stmt->fetchAssoc()) {
    $uids[] = $hr;
  }

  $count = 0;
  foreach ($uids as $record) {
    ++$count;
    $uid = $record['uid'];
    sirius_minilog("Deleting: $count / $limit / $total: $uid ($record[mail], $record[name])");

    $sql = "select count(*) as c from node where uid = :uid ";
    $stmt = db_query($sql, array(':uid' => $uid));
    $hr = $stmt->fetchAssoc();
    $c = $hr['c'];
    if ($c) { die("Worker owns a node!"); }

    $account = user_load($uid);
    $roles = $account->roles;
    unset($roles[2]);
    if ($roles) { die("Worker has roles!"); }

    user_delete($uid);
  }
}
