<?php

class Sirius_Smf_Report_Edi_Hinge extends Sirius_Trust_Provider_Edi_Report_Generator {
  public $type = 'sirius-smf-edi-hinge';

  public function benefit_nid() { return $this->benefit_nid_from_id('HINGEPT'); }

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

  public function batch($wb_nids, $values, &$batch_context) {
    $rows = array();
    foreach ($wb_nids as $wb_nid) {
      $context = $this->context($wb_nid, $asof_date);

      $row = array();
      $row['subscriber_id'] = $context['worker_node']->field_sirius_id['und'][0]['value'];
      $row['first_name'] = $context['worker_node']->field_sirius_name['und'][0]['given'];
      $row['last_name']  = $context['worker_node']->field_sirius_name['und'][0]['family'];
      $row['dob'] = $this->dob($context, 'worker_node');

      $sex = $context['worker_node']->field_sirius_gender['und'][0]['tid'];
      if ($sex) {
        $sex_term = taxonomy_term_load($sex);
        $sex = $sex_term->field_sirius_name_short['und'][0]['value'];
        if (!in_array($sex, array('M', 'F'))) {
          $sex = 'U';
        }
      }
      $row['sex'] = $sex;

      if ($context['relationship_node']) {
        $reltype_tid = $context['relationship_node']->field_sirius_contact_reltype['und'][0]['tid'];
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

      $this->is_qmsco($context);

      $row['address1'] = $context['worker_node']->field_sirius_address['und'][0]['thoroughfare'];
      $row['address2'] = $context['worker_node']->field_sirius_address['und'][0]['premise'];
      $row['city'] = $context['worker_node']->field_sirius_address['und'][0]['locality'];
      $row['state'] = $context['worker_node']->field_sirius_address['und'][0]['administrative_area'];
      $row['zip'] = $context['worker_node']->field_sirius_address['und'][0]['postal_code'];
      $row['email'] = $context['worker_node']->field_sirius_email['und'][0]['value'];
      $row['start_date'] = $this->coverage_start_date($context);
      $row['term_date'] = $this->coverage_end_date($context);
      $row['language'] = $this->lang($context);

      $row = $this->edi_row($row, $context);

      $rows[$context['pk']] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }
}
