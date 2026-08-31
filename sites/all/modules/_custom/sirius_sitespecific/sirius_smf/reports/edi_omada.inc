<?php

class Sirius_Smf_Report_Edi_Omada extends Sirius_Trust_Provider_Edi_Report_Generator {
  public $type = 'sirius-smf-edi-omada';

  public function benefit_nid() { return $this->benefit_nid_from_id('OMADA'); }

  public function info($result = array()) {
    $result = array(
      'name' => t('EDI - Omada'),
      'description' => '',
      'access' => 'sirius trust staff',
      'module' => 'sirius_smf',
      'file' => 'reports/edi_omada.inc',
      'row_encoding' => 'json',
      'edi_carrier_name' => 'OMADA',
      'edi_group_number' => 'BAO',
      'edi_tiers' => array('blended'),
      'edi_output_format' => 'csv',
    );

    return parent::info($result);
  }

  public function edi_premium_tier($context) {
    if (!($this->is_subscriber($context))) { return NULL; }
    return 'blended';
  }

  public function edi_has_premium() { return TRUE; }

  public function edi_fields($which = '') {
    $fields = array();

    $fields['unique_id'] = array();
    $fields['group_number'] = array();
    $fields['dependent'] = array();
    $fields['first_name'] = array();
    $fields['middle_name'] = array();
    $fields['last_name'] = array();
    $fields['dob'] = array();
    $fields['gender'] = array();
    $fields['primary_phone'] = array();
    $fields['mobile_phone'] = array();
    $fields['address_1'] = array();
    $fields['address_2'] = array();
    $fields['city'] = array();
    $fields['state'] = array();
    $fields['zip'] = array();
    $fields['email'] = array();
    $fields['at_risk_htn'] = array();
    $fields['at_risk_t1d'] = array();
    $fields['at_risk_t2d'] = array();
    $fields['at_risk_msk'] = array();
    $fields['ssn'] = array();

    return $fields;
  }

  public function batch($workers, $values, &$batch_context) {
    $benefit_nid = $this->benefit_nid();

    $asof_date = date('Y-m-d 00:00:00', strtotime($values['parameters']['asof_date']));

    $info = $this->info();

    $rows = array();
    foreach ($workers as $worker_nid => $worker) {
      $wb_nid = $worker['wb_nid'];
      $context = $this->context($worker, $asof_date, $values['parameters']);
      if (!$context['worker_node']) { continue; }
      $this->qmsco_context($context);


      // Dependents need to be counted within this file. Ugh.
      $current_subscriber = $this->ssn($context, 'subscriber_node');
      if ($batch_context['results']['current_subscriber'] != $current_subscriber) {
        $batch_context['results']['current_subscriber'] = $current_subscriber;
        $batch_context['results']['dependent_count'] = 0;
      }


      $reltype_id = $this->relationship_type($context['relationship_node']);
      $dep_code = '';
      if (!$reltype_id) {
        $dep_code = '1';
      } else if (in_array($reltype_id, array('SP', 'DP'))) { 
        $dep_code = '2';
      } else {
        $batch_context['results']['dependent_count']++;
        $dep_code = 2 + $batch_context['results']['dependent_count'];
      }

      $row = array();

      $row['unique_id'] = $context['worker_node']->field_sirius_id['und'][0]['value'];
      $row['group_number'] = $info['edi_group_number'];
      $row['dependent'] = $dep_code;
      $row['first_name'] = $context['worker_node']->field_sirius_name['und'][0]['given'];
      $row['middle_name'] = $context['worker_node']->field_sirius_name['und'][0]['middle'];
      $row['last_name'] = $context['worker_node']->field_sirius_name['und'][0]['family'];
      $row['dob'] = $this->dob($context, 'worker_node');
      $row['gender'] = $this->gender($context, 'worker_node');
      $row['primary_phone'] = $this->phone($context, 'worker_node');
      $row['mobile_phone'] = '';
      $row['address_1'] = $context['worker_node']->field_sirius_address['und'][0]['thoroughfare'];
      $row['address_2'] = $context['worker_node']->field_sirius_address['und'][0]['premise'];
      $row['city'] = $context['worker_node']->field_sirius_address['und'][0]['locality'];
      $row['state'] = $context['worker_node']->field_sirius_address['und'][0]['administrative_area'];
      $row['zip'] = $context['worker_node']->field_sirius_address['und'][0]['postal_code'];
      $row['email'] = $this->email($context, 'worker_node');
      $row['at_risk_htn'] = '';
      $row['at_risk_t1d'] = '';
      $row['at_risk_t2d'] = '';
      $row['at_risk_msk'] = '';
      $row['ssn'] = $this->ssn($context, 'worker_node');

      $row = $this->edi_row($row, $context);

      $rows[$context['pk']] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }

  public function cobrastatus($context) {
    if ($this->is_cobra($context)) { return 'COBRA'; } else { return 'ACTIVE'; }
  }

  public function member_type($context) {
    if ($this->is_subscriber($context)) { return 'SUB'; }
    return 'DEP';
  }

  /**
    Subscriber = 1; 
    Spouse/Domestic Partner = 2; 
    Children sequentially numbered beginning with 3.
  */

  public function relationship_code($context) {
    $reltype_id = $this->relationship_type($context['relationship_node']);
    if (!$reltype_id) { return '1'; }
    if (in_array($reltype_id, array('SP', 'DP'))) { return '2'; }

    // Needs work...
    return '3';
  }

  public function gender($context, $which = 'worker_node') {
    $id = $this->gender_id($context['worker_node']);
    if ($id == 'M') { return 'M'; }
    if ($id == 'F') { return 'F'; }
    return 'N';
  }

  public function tier_name($context) {
    if ($this->is_dependent($context)) { return ''; }

    $has_spouse = $has_child = 0;
    foreach ($context['dependent_relationship_nodes'] as $relationship_nid => $relationship_node) {
      $reltype_id = $this->relationship_type($relationship_node);
      if (in_array($reltype_id, array('DP', 'ES', 'SP'))) { $has_spouse = TRUE; }
      if (in_array($reltype_id, array('C', 'AC', 'H', 'QMSCO', 'SC'))) { $has_child++; }
    }

    if ($has_spouse && $has_child) {
      $indicator = 'Employee + Family (Spouse + Child(ren))';
    } else if ($has_spouse && !$has_child) {
      $indicator = 'Employee + Spouse';
    } else if (!$has_spouse && $has_child) {
      if ($has_child == 1) {
        $indicator = 'Employee + Child';
      } else {
        $indicator = 'Employee + Children';
      }
    } else {
      $indicator = 'Employee Only';
    }

    return $indicator;
  }
}