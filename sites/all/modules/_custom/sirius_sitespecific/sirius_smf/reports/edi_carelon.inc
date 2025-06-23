<?php

class Sirius_Smf_Report_Edi_Carelon extends Sirius_Trust_Provider_Edi_Report_Generator {
  public $type = 'sirius-smf-edi-carelon';
  public $edi_output_format = 'fixed';

  public function info($result = array()) {
    $result = array(
      'name' => t('EDI - Carelon'),
      'description' => '',
      'access' => 'sirius trust staff',
      'module' => 'sirius_smf',
      'file' => 'reports/edi_carelon.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function edi_fixed_fields() {
    $fields = array();
    $fields['MEMBNO'] = array('width' => 15);
    $fields['SOCSEC'] = array('width' => 9);
    $fields['LSTNAM'] = array('width' => 25);
    $fields['FSTNAM'] = array('width' => 20);
    $fields['MIDNAM'] = array('width' => 1);
    $fields['ADRLN1'] = array('width' => 40);
    $fields['ADRLN2'] = array('width' => 40);
    $fields['CITYCD'] = array('width' => 25);
    $fields['STACOD'] = array('width' => 2);
    $fields['ZIPCOD'] = array('width' => 10);
    $fields['HOMPHN'] = array('width' => 12);
    $fields['WRKPHN'] = array('width' => 12);
    $fields['BTHDAT'] = array('width' => 8);
    $fields['RELCOD'] = array('width' => 2);
    $fields['SEXCOD'] = array('width' => 1);
    $fields['GRPEFF'] = array('width' => 8);
    $fields['GRPEXP'] = array('width' => 8);
    $fields['GRPNUM'] = array('width' => 25);
    $fields['BENPKG'] = array('width' => 25);
    $fields['TIERCD'] = array('width' => 4);
    $fields['MSTACD'] = array('width' => 1);
    $fields['ALTNUM'] = array('width' => 15);
    $fields['CLIDEF'] = array('width' => 295);
    return $fields;
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

  public function benefit_nid() {
    return $this->benefit_nid_from_id('CARELONEAP');
  }

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

      $row['MEMBNO'] = $this->ssn($context, 'subscriber_node'); 
      $row['SOCSEC'] = $this->ssn($context, 'worker_node'); 
      $row['LSTNAM'] = $context['worker_node']->field_sirius_name['und'][0]['family'];
      $row['FSTNAM'] = $context['worker_node']->field_sirius_name['und'][0]['given'];
      $row['MIDNAM'] = substr($context['worker_node']->field_sirius_name['und'][0]['middle'], 0, 1);
      $row['ADRLN1'] = $context['worker_node']->field_sirius_address['und'][0]['thoroughfare'];
      $row['ADRLN2'] = $context['worker_node']->field_sirius_address['und'][0]['premise'];
      $row['CITYCD'] = $context['worker_node']->field_sirius_address['und'][0]['locality'];
      $row['STACOD'] = $context['worker_node']->field_sirius_address['und'][0]['administrative_area'];
      $row['ZIPCOD'] = $context['worker_node']->field_sirius_address['und'][0]['postal_code'];
      $row['HOMPHN'] = $this->phone($context, 'worker_node');
      $row['WRKPHN'] = '';
      $row['BTHDAT'] = $this->dob($context, 'worker_node');
      $row['RELCOD'] = $this->relcod($context);
      $row['SEXCOD'] = $this->gender($context);
      $row['GRPEFF'] = $this->coverage_start_date($context);
      $row['GRPEXP'] = $this->coverage_end_date($context);
      $row['GRPNUM'] = $this->grpnum($context);
      $row['BENPKG'] = $row['GRPNUM'];
      $row['TIERCD'] = $this->tiercd($context);
      $row['MSTACD'] = $this->mstacd($context);
      $row['ALTNUM'] = '';
      $row['CLIDEF'] = '';

      foreach ($row as $key => $value) {
        $row[$key] = strtoupper($value);
      }

      $rows[$context['pk']] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }

  public function relcod($context) {
    $reltype_id = $this->relationship_type($context['relationship_node']); 

    if (in_array($reltype_id, array('SP', 'DP'))) { return '02'; }
    if (in_array($reltype_id, array('C', 'AC', 'H', 'QMSCO', 'SC', 'G'))) { return '03'; }
    return '01';
  }

  public function gender($context) {
    $id = $this->gender_id($context['worker_node']);
    if ($id == 'M') { return 'M'; }
    if ($id == 'F') { return 'F'; }
    return 'U';
  }

  public function grpnum($context) {
    if (!$context['worker_node']) { return ''; }

    $benefit_type = &drupal_static(__FUNCTION__);
    if (!isset($benefit_type)) {
      $benefit_type = Sirius_Trust_Cobra::getInstance()->cobra_benefit_type();
      if (!$benefit_type) {
        drupal_set_message("No COBRA benefit type has been set. (I'm assuming that the COBRA benefit type is the medical benefit.)", 'error');
      }
    }
    if (!$benefit_type) { return ''; }

    $search_params = array();
    $search_params['worker_nid'] = $context['worker_node']->nid;
    $search_params['ts'] = $context['asof_ts'];
    $search_params['benefit_type'] = $benefit_type;
    $wb_nodes = sirius_trust_worker_benefits_search($search_params);
    if (!$wb_nodes) { return ''; }
    $wb_node = reset($wb_nodes);
    $benefit_node = node_load($wb_node->field_sirius_trust_benefit['und'][0]['target_id']);
    return $benefit_node->field_sirius_id['und'][0]['value'];
  }

  public function mstacd($context) {
    if ($this->is_cobra($context)) { return 'C'; }
    return 'A'; 
  }

  public function tiercd($context) {
    if ($this->is_dependent($context)) { return ''; }

    $has_spouse = $has_child = FALSE;
    foreach ($context['dependent_relationship_nodes'] as $relationship_nid => $relationship_node) {
      $reltype_id = $this->relationship_type($relationship_node);

      if (in_array($reltype_id, array('DP', 'ES', 'SP'))) { $has_spouse = TRUE; }
      if (in_array($reltype_id, array('C', 'AC', 'H', 'QMSCO', 'SC'))) { $has_child = TRUE; }
    }

    if ($has_spouse && $has_child) {
      $indicator = 'FMLY';
    } else if ($has_spouse) {
      $indicator = 'SEMP';
    } else if ($has_child) {
      $indicator = 'AEMP';
    } else {
      $indicator = 'EEMP';
    }

    return $indicator;
  }
}