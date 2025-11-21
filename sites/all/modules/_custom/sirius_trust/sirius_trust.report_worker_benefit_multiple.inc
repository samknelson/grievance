<?php

class Sirius_Trust_Report_Worker_Benefit_Multiple extends Report_Cache_Report_Generator {

 public $type = 'sirius-trust-worker-benefit-multiple';

  public function info($result = array()) {
    $result = array(
      'name' => t('Coverage - Data Integrity - Multiple'),
      'description' => '',
      'access' => 'sirius trust staff',
      'preview' => array(
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'sirius_trust.report_worker_benefit_multiple.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function render_parameters($parameters) {
    $parameters = $parameters['parameters'];

    $html = sirius_jsonfield_util_render_all_values_new(
      $this->form(), 
      $parameters, 
      array(
        'skip_empty' => TRUE,
        'fieldset_title_prefix' => '<h2>',
        'fieldset_title_suffix' => '</h2>',
        'no_fieldsets' => TRUE,
      )
    );
    return $html;
  }

  public function preview_links($row) {
    $links = array();

    if ($row['worker_nid']) {
      $links['node/' . $row['worker_nid'] . "/sirius_trust_worker_benefits"] = 'Worker';
    }

    return $links;
  }

  function fields() {
    return array(
      'worker_nid' => 'Worker NID',
      'worker_ssn' => 'Worker SSN',
      'worker_name' => 'Worker Name',
      'benefit_nid' => 'Benefit NID',
      'benefit_name' => 'Benefit Name',
      'benefit_type_tid' => 'Benefit Type TID',
      'benefit_type_name' => 'Benefit Type Name',
      'count' => 'Count',
      'count_deps' => 'Count (Dependent)',
      'count_subs' => 'Count (Subscribers)',
    );
  }

  function form() {
    $form['asof'] = array(
      '#title' => t('As Of'),
      '#type' => 'date_popup',
      '#attributes' => array(
        'autocomplete' => 'off',
      ),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#default_value' => date('Y-m-d'),
    );

    $form['group_by'] = array(
      '#title' => t('Group By'),
      '#type' => 'select',
      '#required' => TRUE,
      '#options' => array(
        'benefit' => t('Benefit'),
        'benefit_type' => t('Benefit Type'),
      ),
    );

    return $form;
  }

  function pks(&$form_state) {
    $asof = $form_state['values']['parameters']['asof'] . ' 00:00:00';
    if (!$asof) { $asof = date('Y-m-d 00:00:00'); }

    $group_by = $form_state['values']['parameters']['group_by'];

    $domain_nid = sirius_domain_nid();
    $sql_args = array();
    $sql_args[':asof'] = $asof;

    $sql = "select field_sirius_worker_target_id, field_sirius_trust_benefit_target_id, field_sirius_trust_benefit_type_tid, count(*) as c, count(field_sirius_contact_relation_target_id) as deps from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_date_end on field_data_field_sirius_date_end.entity_type = 'node' and field_data_field_sirius_date_end.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_trust_benefit on field_data_field_sirius_trust_benefit.entity_type = 'node' and field_data_field_sirius_trust_benefit.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_contact_relation on field_data_field_sirius_contact_relation.entity_type = 'node' and field_data_field_sirius_contact_relation.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
    $sql .= "left join node benefit_node on field_sirius_trust_benefit_target_id = benefit_node.nid ";
    $sql .= "left join field_data_field_sirius_trust_benefit_type on field_data_field_sirius_trust_benefit_type.entity_type = 'node' and field_data_field_sirius_trust_benefit_type.entity_id = benefit_node.nid ";
    $sql .= "where node.type = 'sirius_trust_worker_benefit' ";
    if ($domain_nid) { 
      $sql .= "and field_sirius_domain_target_id = :domain_nid ";
      $sql_args[':domain_nid'] = $domain_nid;
    } else {
      $sql .= "and field_sirius_domain_target_id is NULL ";
    }
    $sql .= "and field_sirius_date_start_value <= :asof ";
    $sql .= "and (field_sirius_date_end_value >= :asof or field_sirius_date_end_value is null) ";
    if ($group_by == 'benefit_type') {
      $sql .= "group by field_sirius_worker_target_id, field_sirius_trust_benefit_type_tid ";
    } else {
      $sql .= "group by field_sirius_worker_target_id, field_sirius_trust_benefit_target_id ";
    }
    $sql .= "having c > 1 ";
    $sql .= "order by field_sirius_worker_target_id ";

    $stmt = sirius_sql_query($sql, $sql_args);
    $wbs = array();
    while ($hr = $stmt->fetchAssoc()) {
      $wbs[] = $hr['field_sirius_worker_target_id'] . '|' . $hr['field_sirius_trust_benefit_target_id'] . '|' . $hr['field_sirius_trust_benefit_type_tid'] . '|' . $hr['c'] . '|' . $hr['deps'];
    }
    return array_chunk($wbs, 100, TRUE);
  }

  function batch($pks, $values, &$context) {
    $rows = array('is_multiple' => TRUE, 'rows' => array());
    $group_by = $values['parameters']['group_by'];

    $benefit_names = sirius_trust_benefit_options(FALSE);

    foreach ($pks as $pk) {
      list($worker_nid, $benefit_nid, $benefit_type_tid, $count, $count_deps) = explode('|', $pk);

      $count_subs = $count - $count_deps;

      $sql = "select field_sirius_ssn_value from field_data_field_sirius_ssn ";
      $sql .= "where entity_type = 'node' and entity_id = :worker_nid ";
      $stmt = sirius_sql_query($sql, array(':worker_nid' => $worker_nid));
      $hr = $stmt->fetchAssoc();
      $ssn = $hr['field_sirius_ssn_value'];

      $row = array();
      $row['worker_nid'] = $worker_nid;
      $row['worker_name'] = sirius_node_title($worker_nid);
      $row['worker_ssn'] = $ssn;
      $row['benefit_nid'] = $benefit_nid;
      $row['benefit_name'] = $benefit_names[$benefit_nid];
      $row['benefit_type_tid'] = $benefit_type_tid;
      $row['benefit_type_name'] = sirius_term_title($benefit_type_tid);
      $row['count'] = $count;
      $row['count_deps'] = $count_deps;
      $row['count_subs'] = $count_subs;
      $rows['rows'][$pk] = $row;
      $context['results']['dashboard']['count']++;
    }
    return $rows;
  }

}