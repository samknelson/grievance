<?php

/**
 * First appearance of hours at an employer (the "triggering" employer)
 *  - Name
 *  - ID
 *  - NID
 *  - Phone
 *  - Address
 *  - Email
 *  - Current employer (from the Worker record)
 *  - Current work status
 *  - Current memeber status
 *  - Triggering employer
 *  - Total hours within the range
 *  - Total hours at triggering employer
*/

class Sirius_Trust_Report_Worker_Benefit_Ending extends Report_Cache_Report_Generator {
  public $type = 'sirius-trust-worker-benefit-ending';

  public function info($result = array()) {
    $result = array(
      'name' => t('Coverage - Ending'),
      'access' => 'sirius trust staff',
      'module' => 'sirius_trust',
      'file' => 'sirius_trust.report_worker_benefit_ending.inc',
      'dashboard' => TRUE,
      'preview' => array(
        'default_display_fields' => array(
          'worker_name',
          'benefit_type_name',
          'benefit_name',
          'wb_end_date',
        ),
        'preview_links' => TRUE,
      ),
      'operations' => array(
        'contact' => array(
          'field_contact_nid' => 'contact_nid',
        ),
      ),
    );

    return parent::info($result);
  }

  public function form() {
    $form['start_date'] = array(
      '#title' => t('Start Date'),
      '#type' => 'date_popup',
      '#attributes' => array(
        'autocomplete' => 'off',
      ),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#default_value' => date('Y-m-15', strtotime('-1 month')),
      '#prefix' => '<p>' . t('Search for benefits which end between these two dates.') . '</p>',
    );

    $form['end_date'] = array(
      '#title' => t('End Date'),
      '#type' => 'date_popup',
      '#attributes' => array(
        'autocomplete' => 'off',
      ),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#default_value' => date('Y-m-15'),
    );

    $form['benefit_types'] = array(
      '#type' => 'checkboxes',
      '#options' => sirius_taxonomy_load('sirius_trust_benefit_type', 'tid', 'name'),
      '#title' => t('Benefit Type'),
      '#checkall' => TRUE,
    );

    return $form;
  }

  public function fields() {
    $fields = array();
    $fields['pk'] = 'PK';
    $fields['contact_nid'] = 'Contact NID';
    $fields['worker_nid'] = 'Worker NID';
    $fields['worker_id'] = 'Worker ID';
    $fields['worker_name'] = 'Worker Name';
    $fields['worker_phone'] = 'Worker Phone';
    $fields['worker_email'] = 'Worker Email';
    foreach (sirius_addr_cols() as $col => $name) {
      $fields["worker_address_$col"] = "Worker $name";
    }

    $fields['benefit_type_tid'] = 'Benefit Type TID';
    $fields['benefit_type_name'] = 'Benefit Type';
    $fields['benefit_nid'] = 'Benefit NID';
    $fields['benefit_name'] = 'Benefit Name';
    $fields['wb_end_date'] = 'End Date';

    return $fields;
  }

  public function preview_links($row) {
    return array(
      'node/' . $row['worker_nid'] => 'Worker',
      'node/' . $row['worker_nid'] . '/sirius_trust_worker_benefits' => 'Benefits',
    );
  }

  public function pks(&$form_state) {
    $domain_nid = sirius_domain_nid();
    $sql_args = array();

    if ($form_state['values']['parameters']['benefit_types']) {
      $benefit_type_tids = array_filter(array_values($form_state['values']['parameters']['benefit_types']));
    }

    $start_date_format = date('Y-m-d', strtotime($form_state['values']['parameters']['start_date']));
    $end_date_format = date('Y-m-d', strtotime($form_state['values']['parameters']['end_date']));

    $sql = "select node.nid ";
    $sql .= "from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_contact_relation on field_data_field_sirius_contact_relation.entity_type = 'node' and field_data_field_sirius_contact_relation.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_date_end on field_data_field_sirius_date_end.entity_type = 'node' and field_data_field_sirius_date_end.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_trust_benefit on field_data_field_sirius_trust_benefit.entity_type = 'node' and field_data_field_sirius_trust_benefit.entity_id = node.nid ";

    $sql .= "left join node benefit_node on benefit_node.nid = field_sirius_trust_benefit_target_id ";
    $sql .= "left join field_data_field_sirius_trust_benefit_type on field_data_field_sirius_trust_benefit_type.entity_type = 'node' and field_data_field_sirius_trust_benefit_type.entity_id = benefit_node.nid ";

    $sql .= "where node.type = 'sirius_trust_worker_benefit' ";
    $sql .= "and field_sirius_date_end_value >= :start_date_format ";
    $sql .= "and field_sirius_date_end_value <= :end_date_format ";
    $sql .= "and field_sirius_contact_relation_target_id is null ";
    $sql_args[':start_date_format'] = $start_date_format;
    $sql_args[':end_date_format'] = $end_date_format;

    if ($benefit_type_tids) {
      $sql .= "and field_sirius_trust_benefit_type_tid in (:benefit_type_tids) ";
      $sql_args[':benefit_type_tids'] = $benefit_type_tids;
    }


    if ($domain_nid) { 
      $sql .= "and field_sirius_domain_target_id = :domain_nid ";
      $sql_args[':domain_nid'] = $domain_nid;
    } else {
      $sql .= "and field_sirius_domain_target_id is NULL ";
    }
    $sql .= "order by node.nid desc ";
    // $sql .= "limit 10 ";

    $stmt = sirius_sql_query($sql, $sql_args);
    $pks = array();
    while ($hr = $stmt->fetchAssoc()) {
      $pks[] = $hr['nid'];
    }
    return array_chunk($pks, 100, TRUE);
  }

  public function batch($pks, $values, &$context) {
    $start_date_format = date('Ymd', strtotime($values['parameters']['start_date']));
    $end_date_format = date('Ymd', strtotime($values['parameters']['end_date']));

    $rows = array('is_multiple' => TRUE, 'rows' => array());
    foreach ($pks as $pk) {
      $wb_nid = $pk;
      $wb_node = node_load($wb_nid);
      $benefit_node = node_load($wb_node->field_sirius_trust_benefit['und'][0]['target_id']);
      $worker_node = node_load($wb_node->field_sirius_worker['und'][0]['target_id']);
      $contact_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];

      $row = array();
      $row['pk'] = $pk;
      $row['contact_nid'] = $contact_nid;
      $row['worker_nid'] = $worker_node->nid;
      $row['worker_id'] = $worker_node->field_sirius_id['und'][0]['value'];
      $row['worker_name'] = $worker_node->title;
      $row['worker_phone'] = $worker_node->field_sirius_phone['und'][0]['value'];
      $row['worker_email'] = $worker_node->field_sirius_email['und'][0]['value'];
      foreach (sirius_addr_cols() as $col => $name) {
        $row["worker_address_$col"] = $worker_node->field_sirius_address['und'][0][$col];
      }
      $row['benefit_type_tid'] = $benefit_node->field_sirius_trust_benefit_type['und'][0]['tid'];
      $row['benefit_type_name'] = sirius_term_title($row['benefit_type_tid']);
      $row['benefit_nid'] = $benefit_node->nid;
      $row['benefit_name'] = $benefit_node->title;
      if ($wb_node->field_sirius_date_end['und'][0]['value']) {
        $row['wb_end_date'] = date('Y-m-d', strtotime($wb_node->field_sirius_date_end['und'][0]['value']));
      }

      $rows['rows'][$pk] = $row;
    }

    return $rows;
  }

  public function render_parameters($parameters) {
    $parameters = $parameters['parameters'];
    $html .= sirius_fakerender_field('Start Date:', $parameters['start_date']);
    $html .= sirius_fakerender_field('End Date:', $parameters['end_date']);

    if ($parameters['benefit_types']) {
      $benefit_type_tids = array_filter(array_values($parameters['benefit_types']));
      if ($benefit_type_tids) {
        $tmp = array();
        foreach ($benefit_type_tids as $tid) { $tmp[] = sirius_term_title($tid); }
        $html .= sirius_fakerender_field('Benefit Types:', join(',', $tmp));
      }
    }

    return $html;
  }
}
