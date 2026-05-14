<?php

class Sirius_Smf_Report_Edi_Progeny extends Sirius_Trust_Provider_Edi_Report_Generator {
  public $type = 'sirius-smf-edi-progeny';

  public function benefit_nid() { return $this->benefit_nid_from_id('PROGYNY'); }

  public function info($result = array()) {
    $result = array(
      'name' => t('EDI - Progeny'),
      'description' => '',
      'access' => 'sirius trust staff',
      'module' => 'sirius_smf',
      'file' => 'reports/edi_progeny.inc',
      'row_encoding' => 'json',
      'edi_carrier_name' => 'Progeny',
      'edi_account_key' => 'AC-167633',
      'edi_tiers' => array('blended'),
      'edi_group_number' => 'LB391A',
      'edi_output_format' => 'csv',
      'edi_health_plan_id' => 'MLK_EPO',
      'edi_health_plan_election' => 'EPO',
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
    $field_names = array(
      'member_id',
      'subscriber_id',
      'first_name',
      'last_name',
      'street_1',
      'street_2',
      'city',
      'postal_code',
      'date_of_birth',
      'coverage_start_date',
      'coverage_end_date',
      'gender',
      'relationship',
      'state',
      'country',
      'email',
      'phone',
      'health_plan_id',
      'last_4_ssn',
      'employment_start_date',
      'employment_end_date',
      'carrier_name',
      'health_plan_election',
      'health_plan_tier',
      'full_part_time',
      'department',
      'level',
      'cobra_status',
      'other_principal subdivision',
      'EEID',
      'account_key',
    );
    $fields = array();
    foreach ($field_names as $field_name) { $fields[$field_name] = $field_name; }
    return $fields;
  }

  public function edi_has_premium() { return TRUE; }

  public function batch($workers, $values, &$batch_context) {
    $benefit_nid = $this->benefit_nid();

    $asof_date = date('Y-m-d 00:00:00', strtotime($values['parameters']['asof_date']));

    $rows = array();
    foreach ($workers as $worker_nid => $worker) {
      $wb_nid = $worker['wb_nid'];
      $context = $this->context($worker, $asof_date, $values['parameters']);
      if (!$context['worker_node']) { continue; }

      // Only want spouses
      $member_type = $this->member_type($context);
      if (!in_array($member_type, array('M', 'S'))) { continue; }

      $row = array();
      $row['member_id'] = $this->ssn($context, 'worker_node');
      $row['subscriber_id'] = $this->ssn($context, 'subscriber_node');
      $row['first_name'] = $context['worker_node']->field_sirius_name['und'][0]['given'];
      $row['last_name'] = $context['worker_node']->field_sirius_name['und'][0]['family'];
      $row['street_1'] = $context['worker_node']->field_sirius_address['und'][0]['thoroughfare'];
      $row['street_2'] = $context['worker_node']->field_sirius_address['und'][0]['premise'];
      $row['city'] = $context['worker_node']->field_sirius_address['und'][0]['locality'];
      $row['postal_code'] = $this->zip($context, 'worker_node');
      $row['date_of_birth'] = $this->dob($context, 'worker_node');
      $row['coverage_start_date'] = $this->coverage_start_date($context);
      $row['coverage_end_date'] = $this->coverage_end_date($context);
      $row['gender'] = $this->gender($context);
      $row['relationship'] = $this->progeny_relationship_type($context);
      $row['state'] = $context['worker_node']->field_sirius_address['und'][0]['administrative_area'];
      $row['country'] = $context['worker_node']->field_sirius_address['und'][0]['country'];
      $row['email'] =  $this->email($context, 'worker_node');
      $row['phone'] = $this->phone($context, 'worker_node');
      $row['health_plan_id'] = $this->info()['edi_health_plan_id'];
      $row['last_4_ssn'] = substr($row['member_id'], -4);
      $row['employment_start_date'] = '';
      $row['employment_end_date'] = '';
      $row['carrier_name'] = 'MLK'; 
      $row['health_plan_election'] = $this->info()['edi_health_plan_election'];
      $row['health_plan_tier'] = '';
      $row['full_part_time'] = '';
      $row['department'] = '';
      $row['level'] = '';
      $row['cobra_status'] = $this->progeny_cobra_status($context);
      $row['other_principal subdivision'] = '';
      $row['EEID'] = '';
      $row['account_key'] = $this->info()['edi_account_key'];

      /*
      $row['Health Net Group Number'] = $this->info()['edi_group_number'];
      $row['Reserved 1'] = '';
      $row['File Date'] = date('Ymd');
      $row['Transaction Type (Activity Flag)'] = '';
      $row['Coverage Begin Date'] = $this->coverage_start_date($context);
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
      $row['Zip Code'] = $this->zip($context, 'worker_node');
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
      */

      $row = $this->edi_row($row, $context);

      $rows[$context['pk']] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }


  public function progeny_relationship_type($context) {
    if ($this->is_subscriber($context)) { return 'Subscriber'; }
    return 'Dependent';
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
    if (in_array($reltype_id, array('C', 'AC', 'H', 'SC', 'G'))) { return 'D'; }
    if (in_array($reltype_id, array('SP'))) { return 'S'; }
    if (in_array($reltype_id, array('QMSCO'))) { return 'Q'; }
  }

  public function gender($context) {
    $id = $this->gender_id($context['worker_node']);
    if ($id == 'F') { return 'F'; }
    if ($id == 'M') { return 'M'; }
    return 'U';
  }

  public function hire_date($context) {
    if ($this->is_dependent($context)) { return ''; }
    if (!$context['worker_node']) { return ''; }
    if (!$context['employer_node']) { return ''; }

    // drupal_set_message($context['worker_node']->title);
    $tss = array();

    // Can't be later than asof_ts
    $tss[] = $context['asof_ts'];

    // Get date of first hours
    $sql = 'select year, month '; 
    $sql .= 'from sirius_hours_cache ';
    $sql .= "where worker_nid = :worker_nid ";
    $sql .= "order by year, month ";
    $stmt = sirius_sql_query($sql, array(':worker_nid' => $context['worker_node']->nid));
    $hr = $stmt->fetchAssoc();
    if ($hr) {
      $tss[] = strtotime($hr['year'] . '-' . $hr['month'] . '-1');
    }

    // Can't be later than start date
    $date = $this->coverage_start_date($context);
    if ($date) { $tss[] = strtotime($date); }

    // Can't be later than start date
    $date = $this->coverage_end_date($context);
    if ($date) { $tss[] = strtotime($date); }

    $ts = min($tss);
    // foreach ($tss as $ts) { drupal_set_message(date('Ymd', $ts)); }
    // drupal_set_message("Min: " . date('Ymd', $ts));

    return date('Ymd', $ts);
  }

  public function progeny_cobra_status($context) {
    if ($this->is_cobra($context)) { return 'Active'; }
  }

  public function number_covered($context) {
    if ($this->is_dependent($context)) { return ''; }

    return count($context['dependent_relationship_nodes']) + 1;
  }

  /*
  -Single: If coverage is for Member Only, then ContractType = 1
  -Two-Party: If coverage is for Member + One Dependent (regardless of whether the dependent is a spouse, child, or other), then ContractType = 2
  -Family: If coverage includes Member + More Than One Dependent, then ContractType = 3
  */

  public function contract_type($context) {
    if ($this->is_dependent($context)) { return ''; }

    $count = count($context['dependent_relationship_nodes']);

    if ($count == 0) { return '1'; }
    if ($count == 1) { return '2'; }
    return '3';
  }
}