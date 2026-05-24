<?php

class Sirius_Smf_Report_Edi_Progeny extends Sirius_Trust_Provider_Edi_Report_Generator {
  public $type = 'sirius-smf-edi-progyny';

  public function benefit_nid() { return $this->benefit_nid_from_id('PROGYNY'); }

  public function info($result = array()) {
    $result = array(
      'name' => t('EDI - Progyny'),
      'description' => '',
      'access' => 'sirius trust staff',
      'module' => 'sirius_smf',
      'file' => 'reports/edi_progeny.inc',
      'row_encoding' => 'json',
      'edi_carrier_name' => 'Progyny',
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

      if ($context['worker_node']->field_sirius_address['und'][0]['thoroughfare']) {
        $which = 'worker_node';
      } else {
        $which = 'subscriber_node';
      }

      $row['street_1'] = $context[$which]->field_sirius_address['und'][0]['thoroughfare'];
      $row['street_2'] = $context[$which]->field_sirius_address['und'][0]['premise'];
      $row['city'] = $context[$which]->field_sirius_address['und'][0]['locality'];
      $row['postal_code'] = $this->zip($context, $which);
      $row['state'] = $context[$which]->field_sirius_address['und'][0]['administrative_area'];

      $x = $context[$which]->field_sirius_address['und'][0]['country'];
      if ((!$x) || (preg_match('/us/i', $x))) { $x = 'USA'; }
      $row['country'] = $x;

      $x = $this->dob($context, 'worker_node');
      if ($x) { $x = date('Y-m-d', strtotime($x)); }
      $row['date_of_birth'] = $x;

      $x = $this->coverage_start_date($context);
      if ($x) { $x = date('Y-m-d', strtotime($x)); }
      $row['coverage_start_date'] = $x;

      $x = $this->coverage_end_date($context);
      if ($x) { $x = date('Y-m-d', strtotime($x)); }
      $row['coverage_end_date'] = $x;

      $row['gender'] = $this->gender($context);
      $row['relationship'] = $this->progeny_relationship_type($context);

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

    return date('Y-m-d', $ts);
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