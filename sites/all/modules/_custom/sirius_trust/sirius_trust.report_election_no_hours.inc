<?php

class Sirius_Trust_Report_Election_No_Hours extends Report_Cache_Report_Generator {
  public $type = 'sirius-trust-election-no-hours';

  public function info($result = array()) {
    $result = array(
      'name' => t('Election - No Hours'),
      'description' => t('Show any active elections where there are no hours for the worker at the employer.'),
      'access' => 'sirius trust staff',
      'module' => 'sirius_trust',
      'file' => 'sirius_trust.report_election_no_hours.inc',
      'dashboard' => TRUE,
      'preview' => array(
        'preview_links' => TRUE,
      ),
    );

    return parent::info($result);
  }

  public function form() {
    $form['intro'] = array('#markup' => '<div>' . t('Find workers with no hours at their elected employer in the specified month and year.') . '</div>');
    $form += sirius_date_yearmonth_widget(array());
    return $form;
  }

  public function fields() {
    $fields = array();
    $fields['pk'] = 'PK';
    $fields['election_nid'] = 'Election NID';
    $fields['subscriber_worker_nid'] = 'Subscriber NID';
    $fields['subscriber_worker_name'] = 'Subscriber Name';
    $fields['msg'] = 'Message';

    return $fields;
  }

  public function preview_links($row) {
    return array(
      'node/' . $row['subscriber_worker_nid'] . '/sirius_trust_worker_election' => 'Subscriber',
    );
  }

  public function pks(&$form_state) {
    $sql_args = array();

    $sql = "select distinct nid from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_active on field_data_field_sirius_active.entity_type = 'node' and field_data_field_sirius_active.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_contact_relations on field_data_field_sirius_contact_relations.entity_type = 'node' and field_data_field_sirius_contact_relations.entity_id = node.nid ";
    $sql .= "where node.type = 'sirius_trust_worker_election' ";
    $sql .= "and field_sirius_active_value = 'Yes' ";
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = sirius_domain_nid();
    $sql .= "and field_sirius_contact_relations_target_id is not null ";
    $sql .= "order by nid desc ";
    $sql .= "limit 100 ";

    $stmt = sirius_sql_query($sql, $sql_args);
    $pks = array();
    while ($hr = $stmt->fetchAssoc()) {
      $pks[] = $hr['nid'];
    }
    return array_chunk($pks, 100, TRUE);
  }

  public function batch($pks, $values, &$context) {

    $rows = array('is_multiple' => TRUE, 'rows' => array());
    dpm($values);
    return;

    foreach ($pks as $pk) {
      $hours = 
      $election_nid = $pk;

      $election_node = node_load($election_nid);

      $benefit_nids = sirius_fieldvals($election_node, 'field_sirius_trust_benefits', 'target_id');
      if (!$benefit_nids) { continue; }

      $subscriber_worker_nid = $election_node->field_sirius_worker['und'][0]['target_id'];
      if (!$subscriber_worker_nid) { continue; }

      $relation_nids = sirius_fieldvals($election_node, 'field_sirius_contact_relations', 'target_id');
      if (!$relation_nids) { continue; }

      $dependent_worker_nids = array();
      foreach ($relation_nids as $relation_nid) {
        $relation_node = node_load($relation_nid);
        $dependent_contact_nid = $relation_node->field_sirius_contact_alt['und'][0]['target_id'];
        if (!$dependent_contact_nid) { continue; }
        $dependent_worker_nid = sirius_worker_get_by_contact_nid($dependent_contact_nid, TRUE);
        if (!$dependent_worker_nid) { continue; }
        $dependent_worker_nids[] = $dependent_worker_nid;
      }
      if (!$dependent_worker_nids) { continue; }

      $found = FALSE;
      $msgs = array();
      foreach ($benefit_nids as $benefit_nid) {
        // drupal_set_message("Does " . sirius_node_title($subscriber_worker_nid) . " have " . sirius_node_title($benefit_nid) . "?");
        if (!sirius_trust_worker_benefits_check($subscriber_worker_nid, $benefit_nid)) { continue; }
        // drupal_set_message("Yep. ");
        foreach ($dependent_worker_nids as $dependent_worker_nid) {
          // drupal_set_message("Does " . sirius_node_title($dependent_worker_nid) . " have " . sirius_node_title($benefit_nid) . "?");
          if (sirius_trust_worker_benefits_check($dependent_worker_nid, $benefit_nid)) { continue; }

          $found = TRUE;
          $msgs[] = "Subscriber: " . sirius_node_title($subscriber_worker_nid) . " has " . sirius_node_title($benefit_nid) . " but " . sirius_node_title($dependent_worker_nid) . " does not.";
        }
      }
      if (!$found) { continue; }

      $row = array();
      $row['subscriber_worker_nid'] = $subscriber_worker_nid;
      $row['subscriber_worker_name'] = sirius_node_title($subscriber_worker_nid);
      $row['election_nid'] = $election_nid;
      $row['msg'] = join('<br>', $msgs);
      $rows['rows'][$pk] = $row;
    }

    return $rows;
  }
}


