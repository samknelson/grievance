<?php

class Sirius_Teamsters631_M12 extends Sirius_Singleton {
  public function status_options() {
    return array(
      'draft' => 'Draft',
    );
  }

  public function get_status($m12_node) {
    return $m12_node->field_sirius_type['und'][0]['value'];
  }

  public function set_status($m12_node, $status) {
    $m12_node->field_sirius_type['und'][0]['value'] = $status;
    node_save($m12_node);
  }

  public function search($search_params) {
    $domain_nid = sirius_domain_nid();

    $sql_args = array();
    $sql = "select distinct nid from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_notes on field_data_field_sirius_notes.entity_type = 'node' and field_data_field_sirius_notes.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_log_handler on field_data_field_sirius_log_handler.entity_type = 'node' and field_data_field_sirius_log_handler.entity_id = node.nid ";
    $sql .= "where node.type = 'sirius_log' ";
    $sql .= "and field_sirius_category_value = 'teamsters631:m12' ";
    if ($search_params['key']) {
      $sql .= "and field_sirius_notes_value = :key ";
      $sql_args[':key'] = $search_params['key'];
    }
    if ($search_params['employer_nid']) {
      $sql .= "and field_sirius_log_handler_target_id = :employer_nid ";
      $sql_args[':employer_nid'] = $search_params['employer_nid'];
    }
    if ($domain_nid) { 
      $sql .= "and field_sirius_domain_target_id = :domain_nid ";
      $sql_args[':domain_nid'] = $domain_nid;
    } else {
      $sql .= "and field_sirius_domain_target_id is null ";
    }
    $limit = $search_params['limit'];
    if (!$limit) { $limit = 1000; }
    $sql .= "order by nid desc ";
    $sql .= "limit $limit";

    $stmt = sirius_sql_query($sql, $sql_args);
    $m12_nids = array();
    while ($hr = $stmt->fetchAssoc()) { $m12_nids[] = $hr['nid']; }
    if ($search_params['nids_only']) { return $m12_nids; }
    return node_load_multiple($m12_nids);
  }

  public function fetch($employer_nid, $year, $month, $create = FALSE) {
    if (!$employer_nid) { return NULL; }
    if (!($year > 0)) { return NULL; }
    if (!($month > 0)) { return NULL; }

    $search_params = array();
    $search_params['employer_nid'] = $employer_nid;
    $search_params['key'] = $this->key($employer_nid, $year, $month);
    $search_params['limit'] = 1;
    $m12_nodes = $this->search($search_params);
    $m12_node = reset($m12_nodes);
    if ($m12_node) { return $m12_node; }

    if (!$create) { return FALSE; }

    $json = array();
    $json['teamsters631_m12']['year'] = $year;
    $json['teamsters631_m12']['month'] = $month;
    $json['teamsters631_m12']['date'] = "$year-$month-12";
    $json['teamsters631_m12']['employer_nid'] = $employer_nid;
    $json['teamsters631_m12']['employer_name'] = sirius_node_title($employer_nid);

    $m12_node = sirius_log(
      'teamsters631:m12',
      'draft',
      'Temporary Title',
      '',
      $this->key($employer_nid, $year, $month),
      array($employer_nid),
      NULL,
      $json
    );

    $this->title_set($m12_node);

    return $m12_node;
  }

  public function title_set($m12_node) {
    $m12_json = sirius_json_get($m12_node);
    $employer_node = $this->employer($m12_node);
    $title = $employer_node->title . ": 12th of the Month: " . $m12_json['teamsters631_m12']['month'] . '/' . $m12_json['teamsters631_m12']['year'];
    if ($title == $m12_node->title) { return; }

    $m12_node->title = $m12_node->field_sirius_message['und'][0]['value'] = $title;
    node_save($m12_node);
  }

  public function key($employer_nid, $year, $month) {
    return "$employer_nid/$year/$month";
  }

  public function employers($year = NULL, $month = NULL) {
    if (!$year) { $year = date('Y'); }
    if (!$month) { $month = date('n'); }

    // Get the industry
    $industry_tid = $this->industry();
    if (!$industry_tid) {
      drupal_set_message("Industry (CONV) not found.", 'error');
      return array();
    }

    // Get employers with that industry
    $employers = sirius_employer_list(TRUE, $industry_tid);
    return $employers;
  }

  public function industry() {
    return sirius_taxonomy_lookup_by_anything('sirius_industry', 'CONV');
  }

  public function employer($m12_node) {
    return sirius_log_handler($m12_node, 'grievance_shop');
  }

  // ///////////////////////////////////////////////////////////////////////////
  // Dispatches: 
  // How many workers were active at the employer
  // ///////////////////////////////////////////////////////////////////////////

  public function dispatches_list($m12_node, $asof_date) {
    $m12_json = sirius_json_get($m12_node);

    $employer_node = $this->employer($m12_node);
    if (!$employer_node) { return array();}

    $asof_ts = strtotime($asof_date);
    if ($asof_ts === FALSE) { 
      $year = $m12_json['teamsters631_m12']['year'];
      $month = $m12_json['teamsters631_m12']['month'];
      $asof_date = "$year-$month-1";
      $asof_ts = strtotime($asof_date);
    }
    if ($asof_ts === FALSE) {
      $asof_ts = time();
    }

    $search_params = array();
    $search_params['ts'] = $asof_ts;
    $search_params['limit'] = 10000;
    $search_params['nids_only'] = TRUE;
    $search_params['employer_nid'] = $employer_node->nid;
    $probe = sirius_dispatch_search($search_params);
    return $probe;
  }

  public function dispatches_count($m12_node, $asof_date) {
    $probe = $this->dispatches_list($m12_node, $asof_date);
    $m12_json = sirius_json_get($m12_node);

    $asof_ts = strtotime($asof_date);
    if ($asof_ts === FALSE) { 
      $year = $m12_json['teamsters631_m12']['year'];
      $month = $m12_json['teamsters631_m12']['month'];
      $asof_date = "$year-$month-1";
      $asof_ts = strtotime($asof_date);
    }
    if ($asof_ts === FALSE) {
      $asof_ts = time();
    }

    $dispatch_count = count($probe);
    $m12_json['teamsters631_m12']['dispatches'] = array(
      'count' => $dispatch_count,
      'ts' => $asof_ts,
    );
    sirius_json_set($m12_node, $m12_json);
    node_save($m12_node);

    $this->title_set($m12_node);
  }

  public function dispatches_render($m12_node) {
    $m12_json = sirius_json_get($m12_node);

    $count = $m12_json['teamsters631_m12']['dispatches']['count'];
    if (!$count) { $count = 0; }

    if ($m12_json['teamsters631_m12']['dispatches']['ts']) {
      $date = date('Y-m-d', $m12_json['teamsters631_m12']['dispatches']['ts']);
    }

    $msg = "<span class=\"sirius_teamsters631_m12_dispatches\">";
    $msg .= number_format($count);
    if ($date) {
      $msg .= " ($date)";
    }
    $msg .= '</span>';

    return $msg;
  }

  // ///////////////////////////////////////////////////////////////////////////
  // Reminder: 
  // Was a reminder sent to the company?
  // ///////////////////////////////////////////////////////////////////////////

  public function reminder_get($m12_node) {
    $m12_json = sirius_json_get($m12_node);
    $reminder = $m12_json['teamsters631_m12']['reminder'];
    if (!$reminder) { $reminder = array(); }
    return $reminder;
  }

  public function reminder_set($m12_node) {
    $m12_json = sirius_json_get($m12_node);

    $reminder = $m12_json['teamsters631_m12']['reminder'];
    if (!$reminder) { $reminder = array(); }
    $reminder['count']++;
    $reminder['ts'] = time();
    $reminder['status'] = 'sent';
    $m12_json['teamsters631_m12']['reminder'] = $reminder;

    sirius_json_set($m12_node, $m12_json);
    node_save($m12_node);

    $this->title_set($m12_node);
  }

  public function reminder_render($m12_node) {
    $status_options = array(
      'missing' => t('Not Sent'),
      'sent' => t('Sent'),
    );

    $reminder = $this->reminder_get($m12_node);

    $status = $reminder['status'];
    if (!$status) { $status = 'missing'; }

    if ($reminder['ts']) { $date = date('Y-m-d', $reminder['ts']); }

    $count = $reminder['count'];

    $msg = "<span class=\"sirius_teamsters631_m12_received sirius_teamsters631_m12_reminder_$status\">";
    $msg .= $status_options[$status];
    if ($count > 1) { $msg .= " ($count reminders)"; }
    if ($date) {
      $msg .= " ($date)";
    }
    $msg .= '</span>';

    return $msg;
  }

  public function reminder_clear($m12_node) {
    $m12_json = sirius_json_get($m12_node);
    $m12_json['teamsters631_m12']['reminder'] = array();
    sirius_json_set($m12_node, $m12_json);
    node_save($m12_node);
    $this->title_set($m12_node);
  }

  // ///////////////////////////////////////////////////////////////////////////
  // Received: 
  // Was the report received from the company?
  // ///////////////////////////////////////////////////////////////////////////

  public function received_get($m12_node) {
    $m12_json = sirius_json_get($m12_node);
    $result = $m12_json['teamsters631_m12']['received'];
    if (!$result) { $result = array(); }
    return $result;
  }

  public function received_set($m12_node, $asof_date) {
    $m12_json = sirius_json_get($m12_node);

    $asof_ts = strtotime($asof_date);
    if ($asof_ts === FALSE) { $asof_ts = time(); }

    $year = $m12_json['teamsters631_m12']['year'];
    $month = $m12_json['teamsters631_m12']['month'];

    if ($asof_ts >= strtotime("$year-$month-13")) {
      $status = 'late';
    } else {
      $status = 'ok';
    }

    $m12_json['teamsters631_m12']['received'] = array(
      'ts' => $asof_ts,
      'status' => $status,
    );

    sirius_json_set($m12_node, $m12_json);
    node_save($m12_node);

    $this->title_set($m12_node);
  }

  public function received_render($m12_node) {
    $status_options = array(
      'ok' => t('OK'),
      'missing' => t('Missing'),
      'late' => t('Late'),
    );

    $m12_json = sirius_json_get($m12_node);

    $status = $m12_json['teamsters631_m12']['received']['status'];
    if (!$status) { $status = 'missing'; }

    if ($m12_json['teamsters631_m12']['received']['ts']) {
      $date = date('Y-m-d', $m12_json['teamsters631_m12']['received']['ts']);
    }

    $msg = "<span class=\"sirius_teamsters631_m12_received sirius_teamsters631_m12_received_$status\">";
    $msg .= $status_options[$status];
    if ($date) {
      $msg .= " ($date)";
    }
    $msg .= '</span>';

    return $msg;
  }

  public function received_clear($m12_node) {
    $m12_json = sirius_json_get($m12_node);
    $m12_json['teamsters631_m12']['received'] = array();
    sirius_json_set($m12_node, $m12_json);
    node_save($m12_node);
    $this->title_set($m12_node);
  }

  // ///////////////////////////////////////////////////////////////////////////
  // Layoffs: 
  // Workers that the Union is requesting to be laid off.
  // ///////////////////////////////////////////////////////////////////////////

  public function layoff_search_names($m12_node, $names) {
    $m12_json = sirius_json_get($m12_node);

    //
    // Clean up our input names
    //

    $limit = 100;
    $names_input = explode("\n", $names);
    if (count($names_input) > $limit) {
      drupal_set_message("Limiting input to $limit names.", 'warning');
      $names_input = array_slice($names_input, 0, $limit);
    }
    $data = array();
    foreach ($names_input as $delta => $name) {
      $name_clean = $name;
      $name_clean = strtolower($name_clean);
      $name_clean = trim($name_clean);
      $name_clean = preg_replace('/[\-\s]+/', ' ', $name_clean);
      $name_clean = preg_replace('/[^a-z0-9 ]/', '', $name_clean);
      if (!$name_clean) {
        continue;
      }

      $data[$delta]['input'] = $name;
      $data[$delta]['parts'] = explode(' ', $name_clean);
      $data[$delta]['candidates'] = array();
    }

    //
    // Worker search on each name
    //

    foreach ($data as $delta => $record) {
      $domain_nid = sirius_domain_nid();

      $sql_args = array();
      $sql = "select nid, title, field_sirius_id_value from node ";
      $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
      $sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = node.nid ";
      $sql .= "where type = 'sirius_worker' ";

      if ($domain_nid) {
        $sql .= "and field_sirius_domain_target_id = :domain_nid ";
        $sql_args[':domain_nid'] = $domain_nid;
      } else {
        $sql .= "and field_sirius_domain_target_id is null ";
      }

      foreach ($record['parts'] as $part_delta => $part) {
        $sql .= "and concat(title, ' ', field_sirius_id_value) like :part_$part_delta ";
        $sql_args[":part_$part_delta"] = '%' . $part . '%';
      }

      // For some reason, including this clause inhibits use of the node_type index
      // resulting in a full-table scan of all the titles, which takes prohibitively long.
      // I don't know why this would be. But fuck it, we'll just skip the sort and fetch only as many as we need.
      // $sql .= "order by title limit 10 ";

      $stmt = sirius_sql_query($sql, $sql_args);
      $count = 0;
      $limit = 10;
      while ($hr = $stmt->fetchAssoc()) {
        ++$count;
        if ($count > $limit) { break; }
        $data[$delta]['candidates'][$hr['nid']] = $hr;
      }
    }

    //
    // Dispatch search on each candidate
    //

    foreach ($data as $delta => $record) {
      foreach ($record['candidates'] as $worker_nid => $candidate) {
        $data[$delta]['candidates'][$worker_nid] += $this->layoff_search_worker($m12_node, $worker_nid);
      }
    }

    return $data;
  }

  public function layoff_search_worker($m12_node, $worker_nid) {
    $m12_json = sirius_json_get($m12_node);

    // Work status
    $worker_node = node_load($worker_nid);
    $layoff = array();
    $layoff['ws'] = sirius_term_title($worker_node->field_sirius_work_status['und'][0]['tid']);

    // Disaptches
    $search_params = array();
    $search_params['employer_nid'] = $m12_json['teamsters631_m12']['employer_nid'];
    $search_params['worker_nid'] = $worker_nid;
    $search_params['statuses'] = array('accepted', 'accepted_secondary');
    $search_params['limit'] = 10;
    $dispatches = sirius_dispatch_search($search_params);

    if (!$dispatches) {

      $msg = '<em>Not dispatched</em>';
      $elig = '';
      $color = 'grey';

    } else {
      $msgs = $eligs = array();
      $elig_flag = FALSE;

      $filters = array();
      $filters['ignore_redispatch'] = TRUE;
      $filters['ignore_dispatch_status'] = TRUE;
      $filters['ignore_plugins']['nosecondaryatprimary'] = TRUE;

      foreach ($dispatches as $dispatch_nid => $dispatch_node) {
    
        $job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
        $start_date = $dispatch_node->field_sirius_date_start['und'][0]['value'];
        if ($start_date) { $start_date = " on " . date('m/d/Y', strtotime($start_date)); }
        $job_type = sirius_term_title($job_node->field_sirius_dispatch_job_type['und'][0]['tid']);
        $dispatch_status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
        $msgs[] = "$dispatch_status $job_type$start_date";

        $result = sirius_dispatch_elig_explain($job_node, $worker_nid, $filters);
        if ($result['success']) {
          $eligs[] = 'Eligible';
          $color = 'green';
        } else {
          $eligs[] = '<strong>Ineligible: ' . $result['msg'] . '</strong>';
          $color = 'red';
        }
      }

      $msg = join(' / ', $msgs);
      $elig = join(' / ', array_unique($eligs));
    }

    $layoff['msg'] = $msg;
    $layoff['elig'] = $elig;
    $layoff['color'] = $color;
    return $layoff;
  }

  function layoff_add($m12_node, $worker_nid) {
    $m12_json = sirius_json_get($m12_node);

    $record = $m12_json['teamsters631_m12']['layoffs'][$worker_nid];

    if (!$record) { $record = array(); }
    if (!$record['status']) { $record['status'] = 'requested'; }
    $record['request'] = $this->layoff_search_worker($m12_node, $worker_nid);
    $record['request']['ts'] = time();

    $record['worker']['nid'] = $worker_nid;
    $record['worker']['title'] = sirius_node_title($worker_nid);

    $m12_json['teamsters631_m12']['layoffs'][$worker_nid] = $record;
    sirius_json_set($m12_node, $m12_json);
    node_save($m12_node);
  }

  function layoff_cancel($m12_node, $worker_nid) {
    $m12_json = sirius_json_get($m12_node);
    unset($m12_json['teamsters631_m12']['layoffs'][$worker_nid]);
    sirius_json_set($m12_node, $m12_json);
    node_save($m12_node);
  }

  function layoff_render($m12_node, $format = 'html') {
    $m12_json = sirius_json_get($m12_node);
    // sirius_dpm($m12_json);

    $checked_status_options = array(
      '' => t('[not checked]'),
      'layoff_complete' => t('Layoff Complete'),
      'not_notified' => t('Not Notified'),
      'too_soon_to_check' => t('Too Soon to Check'),
      'violation_layoff_late' => t('Violation: Laid Off Late'),
      'violation_employed' => t('Violation: Still Employed'),
      'never_employed' => t('Never Employed'),
    );

    $layoffs = $m12_json['teamsters631_m12']['layoffs'];
    if (!$layoffs) {
      if ($format == 'html') { return '<p>' . t('No workers have been marked as "ineligible".') . '</p>'; }
      if ($format == 'tableselect') {
        return array(
          '#type' => 'tableselect',
          '#options' => array(),
          '#header' => array(),
          '#empty' => t('No workers have been marked as "ineligible".'),
        );
      }

      if ($format == 'summary') {
        return 'No workers have been marked as "ineligible".';
      }
      return array();
    }

    $header = array();
    $header['worker'] = 'Worker';

    if ($format != 'email') {
      $header['scan_dates'] = 'Initial Date<hr>Follow-up Date';
      $header['ws'] = 'Initial Work Status<hr>Follow-up Work Status';
      $header['request_msg'] = 'Initial Message<hr>Follow-up Message';
      $header['request_elig'] = 'Initial Eligibility<hr>Follow-up Eligibility';
      $header['notify_date'] = 'Notified Date';
      $header['tools'] = 'Tools';
    }

    if ($format == 'html') { $header = array_values($header); }

    $rows = array();
    foreach ($layoffs as $worker_nid => $layoff) {
      $row = array();
      $row['worker'] = $layoff['worker']['title'];


      $htmls = array();
      if ($layoff['request']['ts']) { $htmls[] = date('m/d/Y', $layoff['request']['ts']); } else { $htmls[] = '[none]'; }
      if ($layoff['checked']['ts']) { $htmls[] = date('m/d/Y', $layoff['checked']['ts']); } else { $htmls[] = '[none]'; }
      $row['scan_dates'] = join('<hr>', $htmls);

      $row['ws'] = join('<hr>', array($layoff['request']['ws'], $layoff['checked']['elig']['ws']));

      $htmls = array();
      if ($layoff['checked']['status']) { $htmls[] = $checked_status_options[$layoff['checked']['status']]; }
      if ($layoff['checked']['layoff_ts']) { $htmls[] = 'Laid off on ' . date('m/d/Y', $layoff['checked']['layoff_ts']); }
      $checked_msg = join('<br>', $htmls);
      $request_msg = $layoff['request']['msg'];
      $row['request_msg'] = join('<hr>', array($request_msg, $checked_msg));

      $row['request_elig'] = join('<hr>', array($layoff['request']['elig'], $layoff['checked']['elig']['elig']));

      if ($layoff['notify']) {
        $row['notify_date'] = date('m/d/Y h:i A', $layoff['notify']['ts']);
      } else {
        $row['notify_date'] = '';
      }

      $row['tools'] = l('Worker', "node/$worker_nid", array('attributes' => array('class' => array('tool-button'))));

      if ($format == 'html') { 
        $rows[] = $row;
      } else {
        $rows[$worker_nid] = $row;
      }
    }

    if ($format == 'rows') { return $rows; }
    if ($format == 'html') { return sirius_table_render($header, $rows); }

    if ($format == 'tableselect') { 
      return array(
        '#type' => 'tableselect',
        '#options' => $rows,
        '#header' => $header,
      );
    }

    if ($format == 'email') {
      $html = '';

      foreach ($rows as $worker_nid => $row) {
        $html .= '<p>';
        foreach ($header as $key => $prompt) {
          $html .= $row[$key] . '<br />';
        }
        $html .= '</p>';
      }

      return $html;
    }

    if ($format == 'summary') {
      return count($rows) . " ineligible workers.";
    }
  }

  // ///////////////////////////////////////////////////////////////////////////
  // Notify: 
  // Notify the employer about requested layoffs
  // ///////////////////////////////////////////////////////////////////////////

  function notify_set($m12_node) {
    $m12_json = sirius_json_get($m12_node);

    if (!$m12_json['teamsters631_m12']['layoffs']) { $m12_json['teamsters631_m12']['layoffs'] = array(); }

    foreach ($m12_json['teamsters631_m12']['layoffs'] as $worker_nid => $record) {
      $m12_json['teamsters631_m12']['layoffs'][$worker_nid]['notify'] = array(
        'ts' => time()
      );
    }

    $m12_json['teamsters631_m12']['notify']['ts'] = time();

    sirius_json_set($m12_node, $m12_json);
    node_save($m12_node);
  }

  public function notify_render($m12_node) {
    $m12_json = sirius_json_get($m12_node);

    $ts = $m12_json['teamsters631_m12']['notify']['ts'];
    if (!$ts) { return 'Not sent'; }
    return 'Sent (' . date('Y-m-d', $ts) . ')';
  }

  // ///////////////////////////////////////////////////////////////////////////
  // Check: 
  // Check to see if layoffs have been completed.
  // ///////////////////////////////////////////////////////////////////////////

  function checked_set($m12_node) {
    $m12_json = sirius_json_get($m12_node);

    if (!$m12_json['teamsters631_m12']['layoffs']) { $m12_json['teamsters631_m12']['layoffs'] = array(); }

    foreach ($m12_json['teamsters631_m12']['layoffs'] as $worker_nid => $record) {
      $record['checked'] = array();

      $record['checked']['ts'] = time();

      // Load the work status
      $worker_node = node_load($worker_nid);
      $record['checked']['ws'] = sirius_term_title($worker_node->field_sirius_work_status['und'][0]['tid']);

      // Find out if they're CURRENTLY dispatched
      $search_params = array();
      $search_params['employer_nid'] = $m12_json['teamsters631_m12']['employer_nid'];
      $search_params['worker_nid'] = $worker_nid;
      $search_params['statuses'] = array('accepted', 'accepted_secondary');
      $search_params['limit'] = 1;
      $probe = sirius_dispatch_search($search_params);
      if ($probe) {
        $record['checked']['employed'] = TRUE;
      }

      // If they're not currently employed, find out the date of their last layoff
      if (!$probe) {
        $record['checked']['employed'] = FALSE;
        $search_params = array();
        $search_params['employer_nid'] = $m12_json['teamsters631_m12']['employer_nid'];
        $search_params['worker_nid'] = $worker_nid;
        $search_params['limit'] = 1;
        $search_params['order'] = 'field_sirius_date_end_value desc';
        $probe = sirius_dispatch_search($search_params);
        if ($probe) {
          $last_dispatch = reset(array_values($probe));
          $layoff_ts = strtotime($last_dispatch->field_sirius_date_end['und'][0]['value']);
          if ($layoff_ts) {
            $layoff_ts += 24*60*60;
            $record['checked']['layoff_ts'] = $layoff_ts;
          }
        }
      }

      // Calculate the status

      $notify_ts = $record['notify']['ts'];
      $checked_ts = $record['checked']['ts'];
      $layoff_ts = $record['checked']['layoff_ts'];

      if (!$notify_ts) {
        $status = 'not_notified';
      } else {
        if (($checked_ts - $notify_ts) < 48*60*60) {
          $status = 'too_soon_to_check';
        } else {
          if ($record['checked']['employed']) {
            $status = 'violation_employed';
          } else {
            if (!$layoff_ts) {
              $status = 'never_employed';
            } else {
              if ($layoff_ts - $notify_ts > 48*60*60) {
                $status = 'violation_layoff_late';
              } else {
                $status = 'layoff_complete';
              }
            }
          }
        }
      }
      $record['checked']['status'] = $status;

      $record['checked']['elig'] = $this->layoff_search_worker($m12_node, $worker_nid);

      $m12_json['teamsters631_m12']['layoffs'][$worker_nid]['checked'] = $record['checked'];
    }

    sirius_json_set($m12_node, $m12_json);
    node_save($m12_node);
  }

  public function checked_render($m12_node) {
    $m12_json = sirius_json_get($m12_node);
    $layoffs = $m12_json['teamsters631_m12']['layoffs'];
    if (!$layoffs) { $layoffs = array(); }

    $count = $count_violation = 0;
    foreach ($layoffs as $layoff) {
      if (!$layoff['checked']) { continue; }
      ++$count;
      if (in_array($layoff['checked']['status'], array('violation_employed', 'violation_layoff_late'))) {
        ++$count_violation; 
      } else {
        ++$count_ok;
      }
    }

    if (!$count) { return 'Not checked.'; }

    return "$count checked with $count_violation violations.";
  }

  /*
  public function employer_emails($employer_nid) {
    $tid = &drupal_static(__FUNCTION__);
    if (!isset($tid)) {
      $tid = sirius_taxonomy_lookup_by_anything('grievance_contact_types', 'Labor Department');
    }

    if (!$tid) {
      drupal_set_message("Contact type not found: Labor Department", 'error');
      return array();
    }

    $sql_args = array();
    $sql = "select field_grievance_co_email_email from node ";
    $sql .= "left join field_data_field_grievance_co_email on field_data_field_grievance_co_email.entity_type = 'node' and field_data_field_grievance_co_email.entity_id = node.nid ";
    $sql .= "left join field_data_field_grievance_shops on field_data_field_grievance_shops.entity_type = 'node' and field_data_field_grievance_shops.entity_id = node.nid ";
    $sql .= "left join field_data_field_grievance_contact_types on field_data_field_grievance_contact_types.entity_type = 'node' and field_data_field_grievance_contact_types.entity_id = node.nid ";
    $sql .= "where node.type = 'grievance_shop_contact' ";
    $sql .= "and field_grievance_shops_target_id = :employer_nid ";
    $sql_args[':employer_nid'] = $employer_nid;

    $sql .= "and field_grievance_contact_types_tid = :tid ";
    $sql_args[':tid'] = $tid;

    $stmt = sirius_sql_query($sql, $sql_args);
    $emails = array();
    while ($hr = $stmt->fetchAssoc()) {
      $emails[] = $hr['field_grievance_co_email_email'];
    }
    $emails = array_filter(array_unique($emails));
    return $emails;
  }
  */
}
