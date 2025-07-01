<?php

class Sirius_Smf_Report_Edi_Vsp extends Sirius_Trust_Provider_Edi_Report_Generator {
  public $type = 'sirius-smf-edi-vsp';
  public $edi_output_format = 'fixed';

  public function benefit_nid_standard() { return $this->benefit_nid_from_id('3'); }
  public function benefit_nid_enhanced() { return $this->benefit_nid_from_id('3E'); }

  public function info($result = array()) {
    $result = array(
      'name' => t('EDI - VSP'),
      'description' => '',
      'access' => 'sirius trust staff',
      'module' => 'sirius_smf',
      'file' => 'reports/edi_vsp.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function edi_fixed_fields() {
    return array(
      'Subscriber Record ID' => array('width' => 3),
      'Full Replacement Files' => array('width' => 1),
      'Control Number' => array('width' => 5),
      'Subscriber ID' => array('width' => 30),
      'Subscriber SSN' => array('width' => 9),
      'Alternate Subscriber ID' => array('width' => 30),
      'Previous Subscriber ID' => array('width' => 30),
      'Dependent SSN' => array('width' => 9),
      'Subscriber Last Name' => array('width' => 18),
      'Subscriber First Name' => array('width' => 12),
      'Subscriber Middle Initial' => array('width' => 1),
      'Subscriber Name-Suffix' => array('width' => 3),
      'Gender' => array('width' => 1),
      'Family-Indicator' => array('width' => 1),
      'Date of Birth' => array('width' => 8),
      'Vision Coverage Begin Date' => array('width' => 8),
      'Vision Coverage End Date' => array('width' => 8),
      'Family Indicator Change Effective Date' => array('width' => 8),
      'Division-Code' => array('width' => 30),
      'Cross-Reference Code' => array('width' => 16),
      'Subscriber Residence Address Line 1' => array('width' => 30),
      'Subscriber Residence Address Line 2' => array('width' => 30),
      'Subscriber Residence City' => array('width' => 19),
      'Subscriber Residence State Code' => array('width' => 2),
      'Subscriber Residence ZIP Code' => array('width' => 10),
      'Subscriber Residence Phone Number' => array('width' => 10),
      'Subscriber Residence Country' => array('width' => 3),
      'Subscriber Home Email Address' => array('width' => 50),
      'Subscriber Work Email Address' => array('width' => 50),
      'Subscriber Mailing Address Line 1' => array('width' => 30),
      'Subscriber Mailing Address Line 2' => array('width' => 30),
      'Subscriber Mailing City' => array('width' => 19),
      'Subscriber Mailing State' => array('width' => 2),
      'Subscriber Mailing ZIP Code' => array('width' => 10),
      'Subscriber Mailing Country' => array('width' => 3),
      'Subscriber Work Phone Number' => array('width' => 10),
      'Subscriber Message Phone' => array('width' => 10),
      'Subscriber Native Language' => array('width' => 3),
      'Subscriber Spoken Language' => array('width' => 3),
      'Subscriber Reading Language' => array('width' => 3),
      'Subscriber Ethnicity' => array('width' => 1),
      'Subscriber Status Code' => array('width' => 1),
      'Subscriber Citizenship Status Code' => array('width' => 2),
      'Subscriber Status' => array('width' => 2),
      'Subscriber Marital Status Code' => array('width' => 1),
      'Subscriber Medicare Plan Code' => array('width' => 1),
      'Filler2' => array('width' => 34),
    );
  }

  public function pks(&$form_state) {
    $benefit_nid_standard = $this->benefit_nid_standard();
    if (!$benefit_nid_standard) { return array(); }

    $benefit_nid_enhanced = $this->benefit_nid_enhanced();
    if (!$benefit_nid_enhanced) { return array(); }

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

    $sql .= "and field_sirius_trust_benefit_target_id in (:benefit_nids) ";
    $sql_args[':benefit_nids'] = array($benefit_nid_standard, $benefit_nid_enhanced);

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
    $benefit_nid_standard = $this->benefit_nid_standard();
    $benefit_nid_enhanced = $this->benefit_nid_enhanced();

    $asof_date = date('Y-m-d 00:00:00', strtotime($values['parameters']['asof_date']));

    $rows = array();
    foreach ($wb_nids as $wb_nid) {
      $context = $this->context($wb_nid, $asof_date);

      $row = array();
      $row['Subscriber Record ID'] = $this->record_id($context);
      $row['Full Replacement Files'] = 'R';
      $row['Control Number'] = 52638;
      $row['Subscriber ID'] = ''; // $context['subscriber_node']->field_sirius_id['und'][0]['value'];
      $row['Subscriber SSN'] = $this->ssn($context, 'subscriber_node');
      $row['Alternate Subscriber ID'] = '';
      $row['Previous Subscriber ID'] = '';

      if ($this->is_dependent($context)) {
        $row['Dependent SSN'] = $this->ssn($context, 'worker_node');
      }
      
      $row['Subscriber Last Name'] = $context['worker_node']->field_sirius_name['und'][0]['family'];
      $row['Subscriber First Name'] = $context['worker_node']->field_sirius_name['und'][0]['given'];
      $row['Subscriber Middle Initial'] = substr($context['worker_node']->field_sirius_name['und'][0]['middle'], 0, 1);
      $row['Subscriber Name-Suffix'] = '';

      $sex = $context['worker_node']->field_sirius_gender['und'][0]['tid'];
      if ($sex) {
        $sex_term = taxonomy_term_load($sex);
        $sex = $sex_term->field_sirius_name_short['und'][0]['value'];
        if (!in_array($sex, array('M', 'F'))) {
          $sex = 'U';
        }
      }
      $row['Gender'] = $sex;

      $row['Family-Indicator'] = $this->family_indicator($context);
      $row['Date of Birth'] = $this->dob($context, 'worker_node');
      $row['Vision Coverage Begin Date'] = $this->coverage_start_date($context);
      $row['Vision Coverage End Date'] = $this->coverage_end_date($context);
      $row['Family Indicator Change Effective Date'] = '';

      $is_enhanced = ($wb_node->field_sirius_trust_benefit['und'][0]['target_id'] == $benefit_nid_enhanced);
      $is_cobra = $this->is_cobra($context);
      if ($is_enhanced && $is_cobra) {
        $division_code = 2002;
      } else if ($is_enhanced && !$is_cobra) {
        $division_code = 2001;
      } else if (!$is_enhanced && $is_cobra) {
        $division_code = 1002;
      } else if (!$is_enhanced && !$is_cobra) {
        $division_code = 1001;
      }
      $row['Division-Code'] = $division_code;

      $row['Cross-Reference Code'] = '';

      $row['Subscriber Residence Address Line 1'] = $context['worker_node']->field_sirius_address['und'][0]['thoroughfare'];
      $row['Subscriber Residence Address Line 2'] = $context['worker_node']->field_sirius_address['und'][0]['premise'];
      $row['Subscriber Residence City'] = $context['worker_node']->field_sirius_address['und'][0]['locality'];
      $row['Subscriber Residence State Code'] = $context['worker_node']->field_sirius_address['und'][0]['administrative_area'];
      $row['Subscriber Residence ZIP Code'] = $context['worker_node']->field_sirius_address['und'][0]['postal_code'];
      $row['Subscriber Residence Phone Number'] = preg_replace('/[^0-9]/', '', $context['worker_node']->field_sirius_phone['und'][0]['value']);
      $row['Subscriber Residence Country'] = $context['worker_node']->field_sirius_address['und'][0]['country'];

      $row['Subscriber Home Email Address'] = $context['worker_node']->field_sirius_email['und'][0]['value'];
      $row['Subscriber Work Email Address'] = $context['worker_node']->field_sirius_email['und'][0]['value'];

      $row['Subscriber Mailing Address Line 1'] = $context['worker_node']->field_sirius_address['und'][0]['thoroughfare'];
      $row['Subscriber Mailing Address Line 2'] = $context['worker_node']->field_sirius_address['und'][0]['premise'];
      $row['Subscriber Mailing City'] = $context['worker_node']->field_sirius_address['und'][0]['locality'];
      $row['Subscriber Mailing State'] = $context['worker_node']->field_sirius_address['und'][0]['administrative_area'];
      $row['Subscriber Mailing ZIP Code'] = $context['worker_node']->field_sirius_address['und'][0]['postal_code'];
      $row['Subscriber Mailing Country'] = $context['worker_node']->field_sirius_address['und'][0]['country'];

      $row['Subscriber Work Phone Number'] = $this->phone($context, 'subscriber_node');
      $row['Subscriber Message Phone'] = $this->phone($context, 'subscriber_node');

      $row['Subscriber Native Language'] = $context['contact_node']->field_sirius_lang['und'][0]['value'];
      $row['Subscriber Spoken Language'] = $context['contact_node']->field_sirius_lang['und'][0]['value'];
      $row['Subscriber Reading Language'] = $context['contact_node']->field_sirius_lang['und'][0]['value'];
      $row['Subscriber Ethnicity'] = '';

      $row['Subscriber Status Code'] = $this->status_code($context);
      $row['Subscriber Citizenship Status Code'] = '';
      $row['Subscriber Status'] = '';
      $row['Subscriber Marital Status Code'] = '';
      $row['Subscriber Medicare Plan Code'] = '';
      $row['Filler2'] = '';

      $row = $this->edi_row($row, $context);

      $rows[$context['pk']] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }

  public function benefit_nid() {
    return array($this->benefit_nid_standard(), $this->benefit_nid_enhanced());
  }

  public function status_code($context) {
    if ($this->is_cobra($context)) { return 'C'; } else { return 'A'; }
  }

  public function record_id($context) {
    if ($this->is_dependent($context)) { return 'DEP'; } else { return 'MEM'; }
  }

  public function family_indicator($context) {
    if ($this->is_dependent($context)) {
      // For dependents:
      // S = Spouse
      // P = Domestic Partner (only if the plan supports it)
      // C = Child, Adopted Child, Step Child, Protected Person, QMSCO Dependent
      // H = Handicapped Child

      $reltype_id = $this->relationship_type($context['relationship_node']);
      if (in_array($reltype_id, array('SP')))                       { $indicator = 'S'; }
      if (in_array($reltype_id, array('DP', 'ES')))                 { $indicator = 'P'; }
      if (in_array($reltype_id, array('C', 'AC', 'QMSCO', 'SC')))   { $indicator = 'C'; }
      if (in_array($reltype_id, array('H')))                        { $indicator = 'H'; }
    } else {
      // For members:
      // IF  NOT HasSpousePartner  AND  NOT HasChild
      //  ⇒ Family-Indicator = 'C'     # Employee only
      // ELSE IF  HasSpousePartner  AND  NOT HasChild
      //  ⇒ Family-Indicator = 'B'     # Employee + Spouse (no kids yet)
      // ELSE IF  NOT HasSpousePartner  AND  HasChild
      //  ⇒ Family-Indicator = 'D'     # Employee + Child / Children
      // ELSE                              # Has both spouse/partner AND ≥1 child
      //  ⇒ Family-Indicator = 'A'     # Family (all dependents)

      $has_spouse = $has_child = FALSE;
      foreach ($context['dependent_relationship_nodes'] as $relationship_nid => $relationship_node) {
        $reltype_id = $this->relationship_type($relationship_node);
        if (in_array($reltype_id, array('DP', 'ES', 'SP'))) { $has_spouse = TRUE; }
        if (in_array($reltype_id, array('C', 'AC', 'H', 'QMSCO', 'SC'))) { $has_child = TRUE; }
      }

      if ($has_spouse && $has_child) {
        $indicator = 'A';
      } else if ($has_spouse && !$has_child) {
        $indicator = 'B';
      } else if (!$has_spouse && $has_child) {
        $indicator = 'D';
      } else {
        $indicator = 'C';
      }
    }

    return $indicator;
  }
}
