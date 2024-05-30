<?php

class Sirius_Teamsters631_M12 extends Sirius_Singleton {
  public function status_options() {
    return array(
      'draft' => 'Draft',
      'sent' => 'Sent',
    );
  }

  public function get_status($m12_node) {
    return $m12_node->field_sirius_type['und'][0]['value'];
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
      $this->title($employer_nid, $year, $month),
      '',
      $this->key($employer_nid, $year, $month),
      array($employer_nid),
      NULL,
      $json
    );

    $m12_node->title = $this->title($employer_nid, $year, $month);
    node_save($m12_node);

    return $m12_node;
  }

  public function title($employer_nid, $year, $month) {
    return "12th of the Month for $year/$month: " . sirius_node_title($employer_nid);
  }

  public function key($employer_nid, $year, $month) {
    return "$employer_nid/$year/$month";
  }
}