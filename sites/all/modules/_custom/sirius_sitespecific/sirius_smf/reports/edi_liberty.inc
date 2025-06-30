<?php

class Sirius_Smf_Report_Edi_Liberty extends Sirius_Trust_Provider_Edi_Report_Generator {
  public $type = 'sirius-smf-edi-liberty';
  public $edi_output_format = 'fixed';

  public function benefit_nid() { return $this->benefit_nid_from_id('L'); }

  public function info($result = array()) {
    $result = array(
      'name' => t('EDI - Liberty'),
      'description' => '',
      'access' => 'sirius trust staff',
      'module' => 'sirius_smf',
      'file' => 'reports/edi_liberty.inc',
      'row_encoding' => 'json',
      'edi_carrier_name' => 'LIBERTY',
      'edi_group_number' => 'SMUHH',
    );

    return parent::info($result);
  }

  public function edi_has_premium() { return TRUE; }

  public function edi_premium_amt($context) {
    return '666.66';
  }

  public function edi_premium_tier($context) { 
    return $this->tier_name($context);
  }

  public function edi_fixed_fields($which = '') {
    $fields = array();

    $fields['PLanID'] = array('width' => 30);
    $fields['SubscriberNumber'] = array('width' => 30);
    $fields['Status'] = array('width' => 30);
    $fields['TierName'] = array('width' => 30);
    $fields['LastName'] = array('width' => 35);
    $fields['FirstName'] = array('width' => 25);
    $fields['MiddleName'] = array('width' => 25);
    $fields['Address1'] = array('width' => 55);
    $fields['Address2'] = array('width' => 55);
    $fields['City'] = array('width' => 30);
    $fields['State'] = array('width' => 2);
    $fields['Zip'] = array('width' => 15);
    $fields['County'] = array('width' => 30);
    $fields['HomePhone'] = array('width' => 80);
    $fields['Email'] = array('width' => 80);
    $fields['Gender'] = array('width' => 1);
    $fields['DateOfBirth'] = array('width' => 8);
    $fields['RelationshipCode'] = array('width' => 3);
    $fields['EffectiveDate'] = array('width' => 8);
    $fields['TerminationDate'] = array('width' => 8);
    $fields['ReinstatementDate'] = array('width' => 8);
    $fields['PrimaryLanguage'] = array('width' => 7);
    $fields['Ethnicity'] = array('width' => 3);
    $fields['RPLastName'] = array('width' => 35);
    $fields['RPFirstName'] = array('width' => 25);
    $fields['RPMiddleName'] = array('width' => 25);
    $fields['RPAddress1'] = array('width' => 55);
    $fields['RPAddress2'] = array('width' => 55);
    $fields['RPCity'] = array('width' => 30);
    $fields['RPState'] = array('width' => 2);
    $fields['RPZip'] = array('width' => 15);
    $fields['RPHomePhone'] = array('width' => 80);
    $fields['RPEmail'] = array('width' => 80);
    $fields['RPGender'] = array('width' => 1);
    $fields['RPDateOfBirth'] = array('width' => 8);
    $fields['Custom Field'] = array('width' => 80);

    return $fields;
  }

  public function batch($wb_nids, $values, &$batch_context) {
    $benefit_nid = $this->benefit_nid();

    $asof_date = date('Y-m-d 00:00:00', strtotime($values['parameters']['asof_date']));

    $rows = array();
    foreach ($wb_nids as $wb_nid) {
      $context = $this->context($wb_nid, $asof_date);

      $row = array();

      $row['PLanID'] = 'SMUHH';
      $row['SubscriberNumber'] = $this->ssn($context, 'subscriber_node');
      $row['Status'] = $this->cobrastatus($context);
      $row['TierName'] = $this->tier_name($context);
      $row['LastName'] = $context['worker_node']->field_sirius_name['und'][0]['family'];
      $row['FirstName'] = $context['worker_node']->field_sirius_name['und'][0]['given'];
      $row['MiddleName'] = $context['worker_node']->field_sirius_name['und'][0]['middle'];
      $row['Address1'] = $context['worker_node']->field_sirius_address['und'][0]['thoroughfare'];
      $row['Address2'] = $context['worker_node']->field_sirius_address['und'][0]['premise'];
      $row['City'] = $context['worker_node']->field_sirius_address['und'][0]['locality'];
      $row['State'] = $context['worker_node']->field_sirius_address['und'][0]['administrative_area'];
      $row['Zip'] = $context['worker_node']->field_sirius_address['und'][0]['postal_code'];
      $row['County'] = '';
      $row['HomePhone'] = $this->phone($context, 'worker_node');
      $row['Email'] = $this->email($context, 'worker_node');
      $row['Gender'] = $this->gender($context);
      $row['DateOfBirth'] = $this->dob($context, 'worker_node');
      $row['RelationshipCode'] = $this->relationship_code($context);
      $row['EffectiveDate'] = $this->coverage_start_date($context);
      $row['TerminationDate'] = $this->coverage_end_date($context);
      $row['ReinstatementDate'] = '';
      $row['PrimaryLanguage'] = '';
      $row['Ethnicity'] = '';
      $row['RPLastName'] = '';
      $row['RPFirstName'] = '';
      $row['RPMiddleName'] = '';
      $row['RPAddress1'] = '';
      $row['RPAddress2'] = '';
      $row['RPCity'] = '';
      $row['RPState'] = '';
      $row['RPZip'] = '';
      $row['RPHomePhone'] = '';
      $row['RPEmail'] = '';
      $row['RPGender'] = '';
      $row['RPDateOfBirth'] = '';
      $row['Custom Field'] = '';

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
    Relationship type:
    18 = Self (subscriber)
    01 = Spouse, Domestic Partner
    19 = Protected Person, Child, Adopted Child, Step Child, Handicapped Child
    G8 = QMSCO Child"
  */

  public function relationship_code($context) {
    $reltype_id = $this->relationship_type($context['relationship_node']);
    if (!$reltype_id) { return '18'; }
    if (in_array($reltype_id, array('C', 'G', 'AC', 'H'))) { return '19'; }
    if (in_array($reltype_id, array('SP', 'DP'))) { return '01'; }
    if (in_array($reltype_id, array('QMSCO'))) { return 'G8'; }
    return '';
  }

  public function gender($context) {
    $id = $this->gender_id($context['worker_node']);
    if ($id == 'M') { return 'M'; }
    if ($id == 'F') { return 'F'; }
    return 'U';
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