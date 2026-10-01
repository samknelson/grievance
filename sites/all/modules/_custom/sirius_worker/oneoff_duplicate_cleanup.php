<?php

/**
 * One-off: fix duplicate sirius_id values on worker nodes.
 *
 * Finds groups of sirius_worker nodes that share the same field_sirius_id
 * (within the same domain), keeps the oldest node in each group, and re-saves
 * every newer node with a blank ID so that sirius_worker_node_presave()
 * assigns it a fresh one from sirius_worker_nextid().
 *
 * Run from the command line (any directory inside the site; drush changes to
 * the Drupal root during bootstrap, so a bare relative require() will not
 * resolve):
 *
 *   drush --user=1 php-eval "require(drupal_get_path('module', 'sirius_worker') . '/oneoff_duplicate_cleanup.php'); oneoff_duplicate_cleanup('test');"
 *   drush --user=1 php-eval "require(drupal_get_path('module', 'sirius_worker') . '/oneoff_duplicate_cleanup.php'); oneoff_duplicate_cleanup('live');"
 *
 * As a cron job (crontab), e.g. nightly at 2:15am:
 *
 *   15 2 * * * cd /path/to/drupal && drush --user=1 php-eval "require(drupal_get_path('module', 'sirius_worker') . '/oneoff_duplicate_cleanup.php'); oneoff_duplicate_cleanup('live');" >> /var/log/sirius_duplicate_cleanup.log 2>&1
 *
 * It is also registered as an hourly scan in the in-app cron system (see
 * sirius_worker_sirius_cron_scan_info()), so it can be enabled, disabled, and
 * run manually in test or live mode from /sirius/cron.
 *
 * Modes:
 *   'test' - (default) log everything that would change; save nothing.
 *   'live' - actually re-save the affected nodes.
 *
 * Returns array('success' => bool, 'msg' => string), as expected by the
 * sirius_cron scan runner.
 */

if (!defined('DRUPAL_ROOT')) { die("This script must be run via drush.\n"); }

function oneoff_duplicate_cleanup($mode = 'test', $echo = TRUE) {
  // Echo the log to the console when run from drush. (Not from the cron
  // runner, which captures the minilog and displays it in the web UI.)
  if ($echo) { sirius_minilog_echo_active(TRUE); }

  if ($mode != 'test' && $mode != 'live') {
    $msg = "Unknown mode \"$mode\". Use 'test' or 'live'.";
    sirius_minilog($msg, 'error');
    return array('success' => FALSE, 'msg' => $msg);
  }

  global $user;
  $mode_label = strtoupper($mode);
  sirius_minilog("=== Duplicate sirius_id cleanup: $mode_label mode (uid $user->uid) ===");
  if ($mode == 'test') { sirius_minilog("TEST mode: no records will be saved."); }

  // ////////////////////////////////////////////////////////////
  // 1. Find groups of workers sharing a sirius_id.
  //
  // sirius_id is only expected to be unique within a domain (see
  // sirius_find_nid_by_id()), so group by domain + id.
  // ////////////////////////////////////////////////////////////

  $sql = "select ";
  $sql .= "field_sirius_id_value as sirius_id, ";
  $sql .= "coalesce(field_sirius_domain_target_id, 0) as domain_nid, ";
  $sql .= "count(*) as cnt ";
  $sql .= "from field_data_field_sirius_id ";
  $sql .= "join node on node.nid = field_data_field_sirius_id.entity_id and node.type = 'sirius_worker' ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid and field_data_field_sirius_domain.deleted = 0 ";
  $sql .= "where field_data_field_sirius_id.entity_type = 'node' ";
  $sql .= "and field_data_field_sirius_id.bundle = 'sirius_worker' ";
  $sql .= "and field_data_field_sirius_id.deleted = 0 ";
  $sql .= "and field_sirius_id_value is not null ";
  $sql .= "and field_sirius_id_value <> '' ";
  $sql .= "group by field_sirius_id_value, coalesce(field_sirius_domain_target_id, 0) ";
  $sql .= "having count(*) > 1 ";
  $sql .= "order by field_sirius_id_value ";

  $stmt = db_query($sql);
  $groups = array();
  while ($hr = $stmt->fetchAssoc()) { $groups[] = $hr; }

  $group_count = count($groups);
  sirius_minilog("Found $group_count duplicate sirius_id group(s).");

  if (!$group_count) {
    sirius_minilog("Nothing to do.");
    return array('success' => TRUE, 'msg' => 'No duplicate sirius_ids found.');
  }

  // ////////////////////////////////////////////////////////////
  // 2. For each group, list the members and pick the ones to fix.
  // ////////////////////////////////////////////////////////////

  $stats = array('examined' => 0, 'kept' => 0, 'planned' => 0, 'changed' => 0, 'failed' => 0);

  foreach ($groups as $group) {
    $sirius_id = $group['sirius_id'];
    $domain_nid = $group['domain_nid'];
    $domain_label = $domain_nid ? "domain $domain_nid" : 'no domain';

    sirius_minilog("---");
    sirius_minilog("sirius_id $sirius_id ($domain_label): $group[cnt] workers");
    sirius_minilog_indent();

    $sql = "select node.nid, node.title, node.created ";
    $sql .= "from field_data_field_sirius_id ";
    $sql .= "join node on node.nid = field_data_field_sirius_id.entity_id and node.type = 'sirius_worker' ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid and field_data_field_sirius_domain.deleted = 0 ";
    $sql .= "where field_data_field_sirius_id.entity_type = 'node' ";
    $sql .= "and field_data_field_sirius_id.bundle = 'sirius_worker' ";
    $sql .= "and field_data_field_sirius_id.deleted = 0 ";
    $sql .= "and field_sirius_id_value = :sirius_id ";
    $sql .= "and coalesce(field_sirius_domain_target_id, 0) = :domain_nid ";
    $sql .= "order by node.created asc, node.nid asc ";

    $stmt = db_query($sql, array(':sirius_id' => $sirius_id, ':domain_nid' => $domain_nid));
    $members = array();
    while ($hr = $stmt->fetchAssoc()) { $members[] = $hr; }
    $stats['examined'] += count($members);

    if (count($members) < 2) {
      sirius_minilog("Group shrank to " . count($members) . " member(s) since the first query; skipping.", 'warning');
      sirius_minilog_outdent();
      continue;
    }

    if (count($members) > 2) {
      sirius_minilog("Group has more than 2 members; the oldest will be kept and all newer ones re-assigned.", 'warning');
    }

    // Oldest is first; it keeps its ID. Everything newer gets a new one.
    $keep = array_shift($members);
    sirius_minilog("KEEP   nid $keep[nid] \"$keep[title]\" (created " . date('Y-m-d H:i:s', $keep['created']) . ")");
    ++$stats['kept'];

    // ////////////////////////////////////////////////////////////
    // 3. Clear the ID and re-save; presave assigns a new one.
    // ////////////////////////////////////////////////////////////

    foreach ($members as $member) {
      $nid = $member['nid'];
      $created = date('Y-m-d H:i:s', $member['created']);

      $node = node_load($nid, NULL, TRUE);
      if (!$node || $node->type != 'sirius_worker') {
        sirius_minilog("FAIL   nid $nid could not be loaded as a sirius_worker.", 'error');
        ++$stats['failed'];
        continue;
      }

      $old_id = $node->field_sirius_id['und'][0]['value'];
      if ($old_id != $sirius_id) {
        sirius_minilog("FAIL   nid $nid \"$node->title\": loaded sirius_id \"$old_id\" does not match \"$sirius_id\"; skipping.", 'error');
        ++$stats['failed'];
        continue;
      }

      ++$stats['planned'];

      if ($mode == 'test') {
        sirius_minilog("WOULD  nid $nid \"$node->title\" (created $created): clear sirius_id $old_id and re-save to get a new ID.");
        continue;
      }

      sirius_minilog("SAVE   nid $nid \"$node->title\" (created $created): clearing sirius_id $old_id ...");

      // sirius_worker_nextid() reads the counter for the *current* domain, so
      // switch into the worker's domain (as the UI would be) while saving.
      $worker_domain_nid = $node->field_sirius_domain['und'][0]['target_id'];
      if ($worker_domain_nid) { sirius_domain_push($worker_domain_nid); }

      $node->field_sirius_id['und'][0]['value'] = '';
      node_save($node);

      if ($worker_domain_nid) { sirius_domain_pop(); }

      // ////////////////////////////////////////////////////////////
      // 4. Verify the new ID.
      // ////////////////////////////////////////////////////////////

      $reloaded = node_load($nid, NULL, TRUE);
      $new_id = $reloaded->field_sirius_id['und'][0]['value'];

      if (!$new_id) {
        sirius_minilog("FAIL   nid $nid \"$node->title\": after save, sirius_id is blank (was $old_id).", 'error');
        ++$stats['failed'];
        continue;
      }

      if ($new_id == $old_id) {
        sirius_minilog("FAIL   nid $nid \"$node->title\": after save, sirius_id is still $old_id.", 'error');
        ++$stats['failed'];
        continue;
      }

      $sql = "select count(*) as cnt ";
      $sql .= "from field_data_field_sirius_id ";
      $sql .= "join node on node.nid = field_data_field_sirius_id.entity_id and node.type = 'sirius_worker' ";
      $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid and field_data_field_sirius_domain.deleted = 0 ";
      $sql .= "where field_data_field_sirius_id.entity_type = 'node' ";
      $sql .= "and field_data_field_sirius_id.bundle = 'sirius_worker' ";
      $sql .= "and field_data_field_sirius_id.deleted = 0 ";
      $sql .= "and field_sirius_id_value = :sirius_id ";
      $sql .= "and coalesce(field_sirius_domain_target_id, 0) = :domain_nid ";
      $stmt = db_query($sql, array(':sirius_id' => $new_id, ':domain_nid' => $domain_nid));
      $hr = $stmt->fetchAssoc();

      if ($hr['cnt'] != 1) {
        sirius_minilog("FAIL   nid $nid \"$node->title\": new sirius_id $new_id is held by $hr[cnt] workers; expected exactly 1.", 'error');
        ++$stats['failed'];
        continue;
      }

      sirius_minilog("OK     nid $nid \"$node->title\": sirius_id $old_id -> $new_id (verified unique)");
      ++$stats['changed'];
    }

    sirius_minilog_outdent();
  }

  // ////////////////////////////////////////////////////////////
  // 5. Summary.
  // ////////////////////////////////////////////////////////////

  sirius_minilog("---");
  sirius_minilog("=== Summary ($mode_label mode) ===");
  sirius_minilog("Duplicate groups:   $group_count");
  sirius_minilog("Workers examined:   $stats[examined]");
  sirius_minilog("Workers kept as-is: $stats[kept]");
  if ($mode == 'test') {
    sirius_minilog("Workers to change:  $stats[planned] (nothing saved in test mode)");
  } else {
    sirius_minilog("Workers changed:    $stats[changed]");
  }
  if ($stats['failed']) {
    sirius_minilog("Failures:           $stats[failed]", 'error');
  }

  $msg = "$group_count duplicate group(s), $stats[examined] workers examined, ";
  $msg .= ($mode == 'test') ? "$stats[planned] would be changed" : "$stats[changed] changed";
  if ($stats['failed']) { $msg .= ", $stats[failed] failed"; }
  $msg .= '.';

  return array('success' => $stats['failed'] == 0, 'msg' => $msg);
}
