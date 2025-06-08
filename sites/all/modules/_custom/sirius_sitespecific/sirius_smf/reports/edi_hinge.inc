<?php

class Sirius_Smf_Report_Edi_Hinge extends Sirius_Trust_Provider_Edi_Report_Generator {
  public $type = 'sirius-smf-edi-hinge';

  public function info($result = array()) {
    $result = array(
      'name' => t('EDI - Hinge'),
      'description' => '',
      'access' => 'sirius trust staff',
      'module' => 'sirius_smf',
      'file' => 'reports/edi_hinge.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {
    $field_names = array(
      'subscriber_id',
      'first_name',
      'last_name',
      'dob',
      'relationship',
      'sex',
      'address1',
      'address2',
      'city',
      'state',
      'zip',
      'email',
      'start_date',
      'term_date',
      'language',
    );

    $fields = array();
    foreach ($field_names as $field_name) { $fields[$field_name] = $field_name; }
    return $fields;
  }

  public function preview_links($row) {
    $links = array();
    $links['node/' . $row['worker_nid'] . '/sirius_worker_hours'] = 'Worker';
    $links['node/' . $row['payperiod_nid'] . '/sirius_payperiod_simple'] = 'Payperiod';
    return $links;
  }

  public function form() {
    $form['asof_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('As Of'),
      '#attributes' => array(
        'autocomplete' => 'off',
      ),
      '#date_format' => 'Y-m-d',
      '#required' => TRUE,
      '#default_value' => date('Y-m-d', strtotime('-1 month')),
    );

    return $form;
  }

  public function render_parameters($parameters) {
    $parameters = $parameters['parameters'];

    $html = sirius_jsonfield_util_render_all_values_new(
      $this->form(), 
      $parameters, 
      array(
        'skip_empty' => TRUE,
        'no_fieldsets' => TRUE,
      )
    );

    return $html;
  }

  public function benefit_nid() {
    $benefit_id = 'HINGEPT';
    $result = sirius_feed_lookup_by_field('sirius_trust_benefit', 'field_data_field_sirius_id', 'field_sirius_id_value', $benefit_id);
    if (!$result['success']) {
      drupal_set_message("Please make sure that there is a benefit with the ID $benefit_id.", 'error');
      return NULL;
    }
    return $result['nid'];
  }

  public function pks(&$form_state) {
    $benefit_nid = $this->benefit_nid();
    if (!$benefit_nid) { return array(); }

    $asof_date = date('Y-m-d 00:00:00', strtotime($form_state['values']['parameters']['asof_date']));

    $sql_args = array();
    $sql = "select node.nid ";
    $sql .= "from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_date_end on field_data_field_sirius_date_end.entity_type = 'node' and field_data_field_sirius_date_end.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_trust_benefit on field_data_field_sirius_trust_benefit.entity_type = 'node' and field_data_field_sirius_trust_benefit.entity_id = node.nid ";

    $sql .= "where node.type = 'sirius_trust_worker_benefit' ";

    $sql .= "and field_sirius_trust_benefit_target_id = :benefit_nid ";
    $sql_args[':benefit_nid'] = $benefit_nid;

    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = sirius_domain_nid();

    $sql .= "and field_sirius_date_start_value <= :asof_date ";
    $sql .= "and (field_sirius_date_end_value >= :asof_date or field_sirius_date_end_value is null) ";
    $sql_args[':asof_date'] = $asof_date;

    $sql .= "order by node.nid limit 100 ";

    $stmt = sirius_sql_query($sql, $sql_args);

    $pks = array();
    while ($hr = $stmt->fetchAssoc()) {
      $pks[] = $hr['nid'];
    }
    return array_chunk($pks, 100);
  }

  public function batch($wb_nids, $values, &$context) {
    $rows = array();
    foreach ($wb_nids as $wb_nid) {
      $pk = $wb_nid;

      $wb_node = node_load($wb_nid);
      if (!$wb_node) { continue; }

      $worker_node = node_load($wb_node->field_sirius_worker['und'][0]['target_id']);
      if (!$worker_node) { continue; }

      $relationship_node = node_load($wb_node->field_sirius_contact_relation['und'][0]['target_id']);

      $contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);

      $row = array();
      $row['subscriber_id'] = $worker_node->field_sirius_id['und'][0]['value'];
      $row['first_name'] = $worker_node->field_sirius_name['und'][0]['given'];
      $row['last_name']  = $worker_node->field_sirius_name['und'][0]['family'];

      $dob = $worker_node->field_sirius_dob['und'][0]['value'];
      if ($dob) { $dob = date('m/d/Y', strtotime($dob)); }
      $row['dob'] = $dob;

      $sex = $worker_node->field_sirius_gender['und'][0]['tid'];
      if ($sex) {
        $sex_term = taxonomy_term_load($sex);
        $sex = $sex_term->field_sirius_name_short['und'][0]['value'];
        if (!in_array($sex, array('M', 'F'))) {
          $sex = 'U';
        }
      }
      $row['sex'] = $sex;

      if ($relationship_node) {
        $reltype_tid = $relationship_node->field_sirius_contact_reltype['und'][0]['tid'];
        $reltype_term = taxonomy_term_load($reltype_tid);
        $reltype_id = $reltype_term->field_sirius_id['und'][0]['value'];

        if (in_array($reltype_id, array('DB', 'ES', 'SP'))) {
          $relationship = 'Spouse';
        } else if (in_array($reltype_id, array('G', 'C', 'AC', 'H', 'QMSCO', 'SC'))) {
          $relationship = 'Child';
        } else {
          $relationship = 'Other';
        }
      } else {
        $relationship = 'EE';
      }
      $row['relationship'] = $relationship;

      $row['address1'] = $worker_node->field_sirius_address['und'][0]['thoroughfare'];
      $row['address2'] = $worker_node->field_sirius_address['und'][0]['premise'];
      $row['city'] = $worker_node->field_sirius_address['und'][0]['locality'];
      $row['state'] = $worker_node->field_sirius_address['und'][0]['administrative_area'];
      $row['zip'] = $worker_node->field_sirius_address['und'][0]['postal_code'];
      $row['email'] = $worker_node->field_sirius_email['und'][0]['value'];

      $row['start_date'] = date('m/d/Y', strtotime($wb_node->field_sirius_date_start['und'][0]['value']));

      $term_date = $wb_node->field_sirius_date_end['und'][0]['value'];
      if ($term_date) { $term_date = date('m/d/Y', strtotime($term_date)); }
      $row['term_date'] = $term_date;

      $row['language'] = $contact_node->field_sirius_lang['und'][0]['value'];

      $rows[$pk] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }
}
