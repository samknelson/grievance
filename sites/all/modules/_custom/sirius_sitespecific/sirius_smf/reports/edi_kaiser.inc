<?php

class Sirius_Smf_Report_Edi_Kaiser extends Sirius_Trust_Provider_Edi_Report_Generator {
  public $type = 'sirius-smf-edi-kaiser';

  public function benefit_nid() { return $this->benefit_nid_from_id('K'); }

  public function info($result = array()) {
    $result = array(
      'name' => t('EDI - Kaiser'),
      'description' => '',
      'access' => 'sirius trust staff',
      'module' => 'sirius_smf',
      'file' => 'reports/edi_kaiser.inc',
      'row_encoding' => 'json',
      'edi_tiers' => array('single', 'single+1', 'family'),
      'edi_output_format' => 'fixed',
    );

    return parent::info($result);
  }

  public function edi_premium_tier($context) {
    if (!($this->is_subscriber($context))) { return NULL; }

    $count = count($context['dependent_relationship_nodes']);
    if ($count == 0) { return 'single'; }
    if ($count == 1) { return 'single+1'; }
    return 'family';
  }

  public function edi_fields() {
    $fields = array();
    $fields['Region Code'] = array('width' => 3);
    $fields['Record Type'] = array('width' => 1);
    $fields['Customer ID'] = array('width' => 9);
    $fields['Enrollment Unit'] = array('width' => 4);
    $fields['FILLER1'] = array('width' => 36);
    $fields['Activity Date'] = array('width' => 8);
    $fields['Transaction Type'] = array('width' => 1);
    $fields['Record Code'] = array('width' => 1);
    $fields['FILLER2'] = array('width' => 38);
    $fields['Last Name'] = array('width' => 25);
    $fields['First Name'] = array('width' => 25);
    $fields['Middle Name'] = array('width' => 25);
    $fields['Account Role'] = array('width' => 2);
    $fields['FILLER3'] = array('width' => 10);
    $fields['Birth Date'] = array('width' => 8);
    $fields['Marital Status'] = array('width' => 2);
    $fields['FILLER4'] = array('width' => 10);
    $fields['Gender'] = array('width' => 2);
    $fields['FILLER5'] = array('width' => 5);
    $fields['FILLER6'] = array('width' => 1);
    $fields['FILLER7'] = array('width' => 2);
    $fields['Subscriber SSN'] = array('width' => 9);
    $fields['Member SSN'] = array('width' => 9);
    $fields['FILLER8'] = array('width' => 2);
    $fields['Employee ID'] = array('width' => 9);
    $fields['Supplemental ID'] = array('width' => 16);
    $fields['Employer ID'] = array('width' => 4);
    $fields['Employment Status'] = array('width' => 2);
    $fields['FILLER9'] = array('width' => 5);
    $fields['Hire Date'] = array('width' => 8);
    $fields['Home Phone'] = array('width' => 10);
    $fields['Work'] = array('width' => 10);
    $fields['FILLER10'] = array('width' => 30);
    $fields['Address Line 1'] = array('width' => 40);
    $fields['Address Line 2'] = array('width' => 40);
    $fields['FILLER11'] = array('width' => 30);
    $fields['City'] = array('width' => 45);
    $fields['FILLER12'] = array('width' => 45);
    $fields['State'] = array('width' => 2);
    $fields['ZIP Code'] = array('width' => 5);
    $fields['FILLER13'] = array('width' => 2);
    $fields['ZIP Plus 4'] = array('width' => 4);
    $fields['FILLER14'] = array('width' => 45);
    $fields['Enrollment  Reason'] = array('width' => 2);
    $fields['FILLER15'] = array('width' => 10);
    $fields['Effective Date'] = array('width' => 8);
    $fields['FILLER16'] = array('width' => 8);
    $fields['FILLER17'] = array('width' => 2);
    $fields['FILLER18'] = array('width' => 10);
    $fields['Termination Date'] = array('width' => 8);
    $fields['FILLER19'] = array('width' => 2);
    $fields['FILLER20'] = array('width' => 8);
    $fields['Current Eligibility Status'] = array('width' => 1);
    $fields['Current Dues Amount'] = array('width' => 7);
    $fields['Current Rate Code'] = array('width' => 5);
    $fields['Retroactivity Date'] = array('width' => 8);
    $fields['Retroactive Dues Amount'] = array('width' => 7);
    $fields['Retroactive Rate Code'] = array('width' => 5);
    $fields['Additional Retroactivity'] = array('width' => 220);
    $fields['FILLER21'] = array('width' => 7);
    $fields['Eligibility Date'] = array('width' => 8);
    $fields['Dues Amount or Rate Code'] = array('width' => 7);
    $fields['Eligibility Status'] = array('width' => 1);
    $fields['Additional Eligibility Grid Information'] = array('width' => 160);
    $fields['FILLER22'] = array('width' => 36);
    return $fields;
  }

  public function batch($workers, $values, &$batch_context) {
    $benefit_nid = $this->benefit_nid();

    $asof_date = date('Y-m-d 00:00:00', strtotime($values['parameters']['asof_date']));

    $rows = array();
    foreach ($workers as $worker_nid => $worker) {
      $wb_nid = $worker['wb_nid'];
      $context = $this->context($worker, $asof_date, $values['parameters']);
      if (!$context['worker_node']) { continue; }

      $row = array();

      $row['Region Code'] = 'SCR';
      $row['Record Type'] = '1';
      $row['Customer ID'] = '000226111';
      $row['Enrollment Unit'] = $this->enrollment_unit($context);
      $row['Activity Date'] = date('Ymd');
      $row['Transaction Type'] = '';
      $row['Record Code'] = $this->record_code($context);
      $row['Last Name'] = $context['worker_node']->field_sirius_name['und'][0]['family'];
      $row['First Name'] = $context['worker_node']->field_sirius_name['und'][0]['given'];
      $row['Middle Name'] = $context['worker_node']->field_sirius_name['und'][0]['middle'];
      $row['Account Role'] = $this->account_role($context);
      $row['Birth Date'] = $this->dob($context, 'worker_node');
      $row['Marital Status'] = '';
      $row['Gender'] = $this->gender($context);
      $row['Subscriber SSN'] = $this->ssn($context, 'subscriber_node');
      $row['Member SSN'] = $this->ssn($context, 'worker_node');
      $row['Employee ID'] = '';
      $row['Supplemental ID'] = '';
      $row['Employer ID'] = '';
      $row['Employment Status'] = '';
      $row['Hire Date'] = '';
      $row['Home Phone'] = preg_replace('/[^0-9]/', '', $context['worker_node']->field_sirius_phone['und'][0]['value']);
      $row['Work'] = '';
      $row['Address Line 1'] = $context['worker_node']->field_sirius_address['und'][0]['thoroughfare'];
      $row['Address Line 2'] = $context['worker_node']->field_sirius_address['und'][0]['premise'];
      $row['City'] = $context['worker_node']->field_sirius_address['und'][0]['locality'];
      $row['State'] = $context['worker_node']->field_sirius_address['und'][0]['administrative_area'];
      $row['ZIP Code'] = $this->zip($context, 'worker_node');
      $row['ZIP Plus 4'] = '';
      $row['Enrollment  Reason'] = '';
      $row['Effective Date'] = $this->coverage_start_date($context);
      $row['Termination Date'] = $this->coverage_end_date($context);
      $row['Current Eligibility Status'] = '';
      $row['Current Dues Amount'] = $this->kaiser_encode_number($this->edi_premium_amt($context));
      $row['Current Rate Code'] = '';
      $row['Retroactivity Date'] = '';
      $row['Retroactive Dues Amount'] = '';
      $row['Retroactive Rate Code'] = '';
      $row['Additional Retroactivity'] = '';
      $row['Eligibility Date'] = '';
      $row['Dues Amount or Rate Code'] = '';
      $row['Eligibility Status'] = '';
      $row['Additional Eligibility Grid Information'] = '';

      $row = $this->edi_row($row, $context);

      $rows[$context['pk']] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }

  public function ssn($context, $which) {
    $str = parent::ssn($context, $which);
    while (strlen($str) < 9) { $str = "0$str"; }
    return $str;
  }

  public function enrollment_unit($context) {
    if ($this->is_cobra($context)) { return '7000'; } else { return '0000'; }
  }

  public function record_code($context) {
    if ($this->is_subscriber($context)) { return 'A'; } else { return 'D'; }
  }

  /*
   * 01:Subscriber
   * 02: Non-Covered Subscriber
   * 05: Domestic Partner
   * 06: Child
   * 07: Spouse
   * 08: Student
   * 13: Disabled
   */

  public function account_role($context) {
    $reltype_id = $this->relationship_type($context['relationship_node']); 

    if (in_array($reltype_id, array('DP'))) { return '05'; }
    if (in_array($reltype_id, array('C', 'AC', 'H', 'QMSCO', 'SC', 'G'))) { return '06'; }
    if (in_array($reltype_id, array('SP'))) { return '07'; }
    return '01';
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
    $id = $this->gender_id($context['worker_node']);
    if ($id == 'M') { return '01'; }
    if ($id == 'F') { return '02'; }
    return '03';
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
    if ($this->is_cobra($context)) { return 'CO'; } else { return 'CA'; }
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

  public function kaiser_encode_number($val) {
    if (!$val) { $val = 0; }
    $val = round(100 * $val);
    $indicator = $val % 10;

    if ($val >= 0) {
      $map = array(
        0 => '{',
        1 => 'A',
        2 => 'B', 
        3 => 'C', 
        4 => 'D', 
        5 => 'E', 
        6 => 'F', 
        7 => 'G', 
        8 => 'H', 
        9 => 'I', 
      );
    } else {
      $map = array(
        0 => '}',
        1 => 'J',
        2 => 'K', 
        3 => 'L', 
        4 => 'M', 
        5 => 'N', 
        6 => 'O', 
        7 => 'P', 
        8 => 'Q', 
        9 => 'R', 
      );
    }

    $val = abs($val);
    $indicator = $map[$val % 10];

    $val = preg_replace('/.$/', $indicator, $val);
    while (strlen($val) < 7) { $val = "0$val"; }

    return $val;
  }
}