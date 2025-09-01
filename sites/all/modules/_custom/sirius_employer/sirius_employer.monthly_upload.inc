<?php

class Sirius_Employer_Monthly_Upload extends Sirius_Singleton {
  /**
   * Must be overridden
   */

  public function log_category() { return ''; }
  public function feed_type() { return ''; }

  public function status_options() {
    return array(
      'missing' => 'Missing',
      'draft' => 'Draft',
      'complete' => 'Complete',
    );
  }

  public function get_status($eup_node) {
    if (!$eup_node) { return 'missing'; }
    return $eup_node->field_sirius_type['und'][0]['value'];
  }

  public function set_status($eup_node, $new_status) {
    if ($eup_node->field_sirius_type['und'][0]['value'] != $new_status) {
      $eup_node->field_sirius_type['und'][0]['value'] = $new_status;
      node_save($eup_node);
    }
  }

  public function get_key($eup_node) {
    return $eup_node->field_sirius_notes['und'][0]['value'];
  }

  public function get_year($eup_node) {
    $key = $this->get_key($eup_node);
    $parts = explode('/', $key);
    return $parts[1];
  }

  public function get_month($eup_node) {
    $key = $this->get_key($eup_node);
    $parts = explode('/', $key);
    return $parts[2];
  }

  public function get_ym($eup_node) {
    return $this->get_year($eup_node) . '-' . $this->get_month($eup_node);
  }

  public function title($employer_nid, $year, $month) {
    return "Monthly Upload $year/$month: " . sirius_node_title($employer_nid);
  }

  public function key($employer_nid, $year, $month) {
    return "$employer_nid/$year/$month";
  }

  public function get_employer($eup_node) {
    return sirius_log_handler($eup_node, 'grievance_shop');
  }






  public function fetch($employer_nid, $year, $month, $create = FALSE) {
    if (!$employer_nid) { return NULL; }
    if (!($year > 0)) { return NULL; }
    if (!($month > 0)) { return NULL; }

    $search_params = array();
    $search_params['employer_nid'] = $employer_nid;
    $search_params['key'] = $this->key($employer_nid, $year, $month);
    $search_params['limit'] = 1;
    $eup_nodes = $this->search($search_params);
    $eup_node = reset($eup_nodes);
    if ($eup_node) { return $eup_node; }

    if (!$create) { return FALSE; }

    $json = array();
    $json['eup']['year'] = $year;
    $json['eup']['month'] = $month;
    $json['eup']['date'] = "$year-$month-15";
    $json['eup']['employer_nid'] = $employer_nid;
    $json['eup']['employer_name'] = sirius_node_title($employer_nid);

    $eup_node = sirius_log(
      $this->log_category(),
      'draft',
      $this->title($employer_nid, $year, $month),
      '',
      $this->key($employer_nid, $year, $month),
      array($employer_nid),
      NULL,
      $json
    );

    $eup_node->title = $this->title($employer_nid, $year, $month);
    node_save($eup_node);

    return $eup_node;
  }

  public function employer_landing_form($form, &$form_state, $employer_node) {
    $search_params = array();
    $search_params['employer_nid'] = $employer_node->nid;
    $eup_nodes = $this->search($search_params);

    $form['#tree'] = TRUE;
    $form_state['employer_nid'] = $employer_node->nid;

    $month_options = array();
    for ($i=1; $i<=12; ++$i) { $month_options[$i] = $i; }
    $form['month'] = array(
      '#type' => 'select',
      '#title' => t('Work Month'),
      '#options' => $month_options,
      '#required' => TRUE,
      // '#default_value' => date('n'),
    );

    $year_options = array();
    $y_now = date('Y');
    for ($i=0; $i<12; ++$i) {
      $year_options[$y_now - $i] = $y_now - $i;
    }
    $form['year'] = array(
      '#type' => 'select',
      '#title' => t('Work Year'),
      '#options' => $year_options,
      '#required' => TRUE,
      // '#default_value' => date('Y'),
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Create or Edit Upload'),
    );

    $form['list'] = array('#markup' => '<h2>' . t('Prior Uploads') . '</h2>' . $this->render_list($eup_nodes));

    return $form;
  }

  public function get_color($status) {
    if ($status == 'draft') { return 'grey'; }
    if ($status == 'error') { return 'red'; }
    if ($status == 'complete') { return 'green'; }
  }

  public function employer_landing_form_submit($form, &$form_state) {
    $employer_nid = $form_state['employer_nid'];
    $year = $form_state['values']['year'];
    $month = $form_state['values']['month'];
    $eup_node = $this->fetch($employer_nid, $year, $month, TRUE);
    drupal_goto("node/" . $eup_node->nid . '/sirius_smf_ehrs_wizard'); // @todo
  }

  public function render_tools($eup_node) {
    return l('View', 'node/' . $eup_nid, array('attributes' => array('class' => array('tool-button'))));
  }

  public function render_list($eup_nodes, $render_options = array()) {
    if (!$eup_nodes) { return '<p>' . t('No records found.') . '</p>'; }
    $status_options = $this->status_options();

    uksort($eup_nodes, function($a, $b) use ($eup_nodes) {
      $ay = $this->get_year($eup_nodes[$a]);
      $by = $this->get_year($eup_nodes[$b]);
      if ($ay < $by) { return 1; }
      if ($ay > $by) { return -1; }

      $am = $this->get_month($eup_nodes[$a]);
      $bm = $this->get_month($eup_nodes[$b]); // ha ha you said bm
      if ($am < $bm) { return 1; }
      if ($am > $bm) { return -1; }

      return 0;
    });

    $rows = array();
    foreach ($eup_nodes as $eup_nid => $eup_node) {
      $status = $this->get_status($eup_node);
      $color = $this->get_color($status);
      $status_render = $status_options[$status];
      if (!$status_render) { $status_render = $status; }

      $row = array();
      $row[] = $this->get_year($eup_node) . '-' . $this->get_month($eup_node);
      $row[] = $status_render;
      $row[] = $this->render_tools($eup_node);

      $rows[] = array('data' => $row, 'class' => array($color));
    }

    $header = array();
    $header[] = t('Date');
    $header[] = t('Status');
    $header[] = t('Tools');

    return sirius_table_render($header, $rows);
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

    $sql .= "and field_sirius_category_value = :log_category ";
    $sql_args[':log_category'] = $this->log_category();
    if (!$sql_args[':log_category']) { return array(); }

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
    $eup_nids = array();
    while ($hr = $stmt->fetchAssoc()) { $eup_nids[] = $hr['nid']; }
    if ($search_params['nids_only']) { return $eup_nids; }
    return node_load_multiple($eup_nids);
  }

  public function get_access() {
    return Sirius_Employer_Monthly_Access::getInstance();
  }
  
  public function access($action, $context = array()) {
    return $this->get_access()->access($action, $context);
  }

  //
  // Support for "Corrections" -- secondary uploads after the first upload
  //

  public function log_category_correction() {
    return $this->log_category() . ':correction';
  }

  public function feed_type_correction() {
    return $this->feed_type() . '_correction';
  }
}
