<?php

class Sirius_Smf_Report_Edi_Healthnet extends Sirius_Trust_Provider_Edi_Report_Generator {
  public $type = 'sirius-smf-edi-healthnet';
  public $edi_output_format = 'fixed';

  public function info($result = array()) {
    $result = array(
      'name' => t('EDI - Healthnet'),
      'description' => '',
      'access' => 'sirius trust staff',
      'module' => 'sirius_smf',
      'file' => 'reports/edi_healthnet.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function edi_fixed_fields() {
    return array(
      'Health Net Group Number' => array('width' => 6),
      'Reserved 1' => array('width' => 2),
      'File Date' => array('width' => 8),
      'Transaction Type (Activity Flag)' => array('width' => 1),
      'Coverage Begin Date' => array('width' => 8),
      'Subscriber SSN' => array('width' => 9),
      'Dependent SSN' => array('width' => 9),
      'Member Type' => array('width' => 1),
      'Reserved 2' => array('width' => 3),
      'Last Name & Suffix' => array('width' => 17),
      'First Name' => array('width' => 10),
      'Middle Initial' => array('width' => 1),
      'Gender' => array('width' => 1),
      'Date of Birth' => array('width' => 8),
      'Address Line 1' => array('width' => 25),
      'Address Line 2' => array('width' => 25),
      'City' => array('width' => 17),
      'State' => array('width' => 2),
      'Zip Code' => array('width' => 5),
      'Zip Code +4 Extension' => array('width' => 4),
      'Work Telephone' => array('width' => 10),
      'Residence Telephone' => array('width' => 10),
      'Provider ID' => array('width' => 4),
      'Physician Last Name' => array('width' => 20),
      'Physician First Name' => array('width' => 20),
      'Physician Middle Initial' => array('width' => 1),
      '4-Digit PPG ID' => array('width' => 4),
      '6-Digit PCP ID' => array('width' => 6),
      'Current Patient Indicator' => array('width' => 1),
      'Hire Date' => array('width' => 8),
      'Employee Number' => array('width' => 6),
      'Department' => array('width' => 6),
      'COBRA End Date' => array('width' => 6),
      'Pay Status Code' => array('width' => 2),
      'Contract Type' => array('width' => 1),
      'Number Covered' => array('width' => 2),
      'Coverage End Date' => array('width' => 8),
      'Foreign Address Flag' => array('width' => 1),
      'Correspondence Indicator' => array('width' => 3),
      'Ethnicity Indicator' => array('width' => 3),
      'Student Indicator' => array('width' => 1),
      'Medicare Part A Indicator' => array('width' => 1),
      'Medicare Part B Indicator' => array('width' => 1),
      'Medicare Parts A & B Indicator' => array('width' => 1),
      'Medicare Part D Indicator' => array('width' => 1),
      'Disabled Indicator' => array('width' => 1),
      'Filler 1' => array('width' => 13),
      'Health Insurance Claim Number (for Medicare COB)' => array('width' => 13),
      'Coordination of Benefits' => array('width' => 1),
      'Insurance Line Code' => array('width' => 3),
      'Current Premium Amount' => array('width' => 8),
      'Retroactive Debit Amount' => array('width' => 8),
      'Retroactive Credit Amount' => array('width' => 8),
      'Record End Designator' => array('width' => 5),
    );
  }

  public function fields() {
    return array_combine(array_keys($this->edi_fixed_fields()), array_keys($this->edi_fixed_fields()));
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

  public function benefit_nid() { return $this->benefit_nid_from_id('H'); }

  public function pks(&$form_state) {
    $benefit_nid = $this->benefit_nid();
    if (!$benefit_nid) { return array(); }

    $asof_date = date('Y-m-d 00:00:00', strtotime($form_state['values']['parameters']['asof_date']));

    $sql_args = array();
    $sql = "select nid ";
    $sql .= "from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_date_end on field_data_field_sirius_date_end.entity_type = 'node' and field_data_field_sirius_date_end.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_trust_benefit on field_data_field_sirius_trust_benefit.entity_type = 'node' and field_data_field_sirius_trust_benefit.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_trust_subscriber on field_data_field_sirius_trust_subscriber.entity_type = 'node' and field_data_field_sirius_trust_subscriber.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_contact_relation on field_data_field_sirius_contact_relation.entity_type = 'node' and field_data_field_sirius_contact_relation.entity_id = node.nid ";

    $sql .= "where node.type = 'sirius_trust_worker_benefit' ";

    $sql .= "and field_sirius_trust_benefit_target_id = :benefit_nid ";
    $sql_args[':benefit_nid'] = $benefit_nid;

    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = sirius_domain_nid();

    $sql .= "and field_sirius_date_start_value <= :asof_date ";
    $sql .= "and (field_sirius_date_end_value >= :asof_date or field_sirius_date_end_value is null) ";
    $sql_args[':asof_date'] = $asof_date;

    $sql .= "order by field_sirius_trust_subscriber_target_id, field_sirius_contact_relation_target_id ";

    $stmt = sirius_sql_query($sql, $sql_args);

    $pks = array();
    while ($hr = $stmt->fetchAssoc()) {
      $pks[] = $hr['nid'];
    }
    return array_chunk($pks, 100);
  }

  public function batch($wb_nids, $values, &$batch_context) {
    $benefit_nid = $this->benefit_nid();

    $asof_date = date('Y-m-d 00:00:00', strtotime($values['parameters']['asof_date']));

    $rows = array();
    foreach ($wb_nids as $wb_nid) {
      $context = $this->context($wb_nid, $asof_date);

      $row = array();

      $row['Health Net Group Number'] = '132501278';
      $row['Reserved 1'] = '';
      $row['File Date'] = date('Ymd');
      $row['Transaction Type (Activity Flag)'] = '';
      $row['Coverage Begin Date'] = date('Ymd', strtotime($wb_node->field_sirius_date_start['und'][0]['value']));
      $row['Subscriber SSN'] = $this->ssn($context, 'subscriber_node');
      $row['Dependent SSN'] = $this->ssn($context, 'worker_node');
      $row['Member Type'] = $this->member_type($context);
      $row['Reserved 2'] = '';
      $row['Last Name & Suffix'] = $context['worker_node']->field_sirius_name['und'][0]['family'];
      $row['First Name'] = $context['worker_node']->field_sirius_name['und'][0]['given'];
      $row['Middle Initial'] = substr($context['worker_node']->field_sirius_name['und'][0]['middle'], 0, 1);
      $row['Gender'] = $this->gender($context);
      $row['Date of Birth'] = $this->dob($context, 'worker_node');
      $row['Address Line 1'] = $context['worker_node']->field_sirius_address['und'][0]['thoroughfare'];
      $row['Address Line 2'] = $context['worker_node']->field_sirius_address['und'][0]['premise'];
      $row['City'] = $context['worker_node']->field_sirius_address['und'][0]['locality'];
      $row['State'] = $context['worker_node']->field_sirius_address['und'][0]['administrative_area'];
      $row['Zip Code'] = $context['worker_node']->field_sirius_address['und'][0]['postal_code'];
      $row['Zip Code +4 Extension'] = '';
      $row['Work Telephone'] = $this->phone($context, 'subscriber_node');
      $row['Residence Telephone'] = '';
      $row['Provider ID'] = '';
      $row['Physician Last Name'] = '';
      $row['Physician First Name'] = '';
      $row['Physician Middle Initial'] = '';
      $row['4-Digit PPG ID'] = '';
      $row['6-Digit PCP ID'] = '';
      $row['Current Patient Indicator'] = '';
      $row['Hire Date'] = $this->hire_date($context);
      $row['Employee Number'] = '';
      $row['Department'] = '';
      $row['COBRA End Date'] = '';
      $row['Pay Status Code'] = $this->pay_status_code($context);
      $row['Contract Type'] = $this->contract_type($context);
      $row['Number Covered'] = $this->number_covered($context);
      $row['Coverage End Date'] = $this->coverage_end_date($context);
      $row['Foreign Address Flag'] = '';
      $row['Correspondence Indicator'] = '';
      $row['Ethnicity Indicator'] = '';
      $row['Student Indicator'] = '';
      $row['Medicare Part A Indicator'] = '';
      $row['Medicare Part B Indicator'] = '';
      $row['Medicare Parts A & B Indicator'] = '';
      $row['Medicare Part D Indicator'] = '';
      $row['Disabled Indicator'] = '';
      $row['Filler 1'] = '';
      $row['Health Insurance Claim Number (for Medicare COB)'] = '';
      $row['Coordination of Benefits'] = '';
      $row['Insurance Line Code'] = 'HMO';
      $row['Current Premium Amount'] = '';
      $row['Retroactive Debit Amount'] = '';
      $row['Retroactive Credit Amount'] = '';
      $row['Record End Designator'] = 'HNPES';

      foreach ($row as $key => $value) {
        $row[$key] = strtoupper($value);
      }

      $rows[$context['pk']] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }

  /**
   * "Relationship type:
      M = Self (subscriber)
      P = Domestic Partner
      D = Protected Person, Child, Adopted Child, QMSCO Child, Step Child, Handicapped Child
      S = Spouse
    */

  public function member_type($context) {
    if ($this->is_subscriber($context)) { return 'M'; }
    $reltype_tid = $context['relationship_node']->field_sirius_contact_reltype['und'][0]['tid'];
    if (!$reltype_tid) { return 'M'; }
    $reltype_term = taxonomy_term_load($reltype_tid);
    if (!$reltype_term) { return 'M'; }
    $reltype_id = $reltype_term->field_sirius_id['und'][0]['value'];
    if (!$reltype_id) { return 'M'; }

    if (in_array($reltype_id, array('DP'))) { return 'P'; }
    if (in_array($reltype_id, array('C', 'AC', 'H', 'QMSCO', 'SC', 'G'))) { return 'D'; }
    if (in_array($reltype_id, array('SP'))) { return 'S'; }
  }

  public function gender($context) {
    $lookup = &drupal_static(__FUNCTION__);
    if (!isset($lookup)) {
      $lookup = sirius_taxonomy_load('sirius_gender', 'tid', 'full');
    }

    $tid = $context['worker_node']->field_sirius_gender['und'][0]['tid'];
    $term = $lookup[$tid];
    if ($term->field_sirius_id['und'][0]['value'] == 'F') { return 'F'; }
    if ($term->field_sirius_id['und'][0]['value'] == 'M') { return 'M'; }
    return '';
  }

  public function phone($context, $which) {
    if ($this->is_dependent($context)) { return ''; }
    return parent::phone($context, $which);
  }

  public function hire_date($context) {
    if ($this->is_dependent($context)) { return ''; }
    if (!$context['worker_node']) { return ''; }
    if (!$context['employer_node']) { return ''; }

    $industry_tid = $context['employer_node']->field_sirius_industry['und'][0]['tid'];
    $ms_tid = sirius_worker_ms_by_industry($context['worker_node'], $industry_tid);
    if (!$ms_tid) { return ''; }

    $history = sirius_worker_history($context['worker_node'], 'worker:member_status');
    if (!$history) { return ''; }

    $changes = array_reverse($history['changes'], TRUE);
    if (!$changes) { return ''; }
 
    $last_ts = $context['asof_ts'];
    foreach ($changes as $ts => $record) {
      if ($ts > $last_ts) { continue; }
      if (!in_array($ms_tid, $record['new_value'])) { break; }
      $last_ts = $ts;
    }
    return date('Ymd', $last_ts);
  }

  public function pay_status_code($context) {
    if ($this->is_dependent($context)) { return ''; }
    if ($this->is_cobra($context)) { return 'CO'; } else { return 'AC'; }
  }

  public function number_covered($context) {
    if ($this->is_dependent($context)) { return ''; }

    return count($context['dependent_relationship_nodes']) + 1;
  }

  public function contract_type($context) {
    if ($this->is_dependent($context)) { return ''; }

    $reltypes = &drupal_static(__FUNCTION__);
    if (!isset($$reltypes)) { $reltypes = sirius_taxonomy_load('sirius_contact_relationship_types', 'tid', 'full'); }

    $has_spouse = $has_child = FALSE;
    foreach ($context['dependent_relationship_nodes'] as $relationship_nid => $relationship_node) {
      $reltype_tid = $relationship_node->field_sirius_contact_reltype['und'][0]['tid'];
      if (!$reltype_tid) { continue; }
      $reltype_term = $reltypes[$reltype_tid];
      if (!$reltype_term) { continue; }
      $reltype_id = $reltype_term->field_sirius_id['und'][0]['value'];
      if (!$reltype_id) { continue; }

      if (in_array($reltype_id, array('DP', 'ES', 'SP'))) { $has_spouse = TRUE; }
      if (in_array($reltype_id, array('C', 'AC', 'H', 'QMSCO', 'SC'))) { $has_child = TRUE; }
    }

    if ($has_spouse && $has_child) {
      $indicator = '3';
    } else if ($has_spouse && !$has_child) {
      $indicator = '2';
    } else if (!$has_spouse && $has_child) {
      $indicator = '4';
    } else {
      $indicator = '1';
    }

    return $indicator;
  }
}