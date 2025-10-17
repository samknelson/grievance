<?php

class Sirius_Trust_Report_Missing_Demog extends Report_Cache_Report_Generator {
  public $type = 'sirius-trust-missing-demog';

  public function info($result = array()) {
    $result = array(
      'name' => t('Missing Demographics'),
      'description' => '',
      'access' => 'sirius trust staff',
      'preview' => array(
        /*
        'default_display_fields' => array(
          'relationship_nid',
          'worker_name',
        ),
        */
        'preview_links' => TRUE,
      ),
     'render_callbacks' => array(
        'Dashboard' => array(
          'method' => 'render_dashboard',
          'csv' => FALSE,
        ),
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'sirius_trust.report_missing_demog.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function form() {
    $form['benefit'] = array(
      '#title' => t('Benefit'),
      '#type' => 'select',
      '#options' => array(NULL => t('-- Select --')) + sirius_trust_benefit_options(),
      '#description' => t('The benefit will determine who is a subscriber and who is a dependant. Leave blank to scan all workers.'),
    );

    $form['roles'] = array(
      '#title' => t('Roles'),
      '#type' => 'checkboxes',
      '#options' => array(
        'subscriber' => 'Subscriber',
        'dependent' => 'Dependent',
        'none' => 'None',
      ),
      '#checkall' => TRUE,
      '#description' => t('Make sure to select "None" if no benefit is selected, since we won\'t know if the member is a dependent or subscriber'),
    );

    $form['scan'] = array(
      '#title' => t('Scan'),
      '#type' => 'checkboxes',
      '#options' => array(
        'ssn' => 'SSN',
        'gender' => 'Gender',
        'email' => 'Email',
        'phone' => 'Phone',
        'dob' => 'DoB',
        'address' => 'Address',
      ),
      '#checkall' => TRUE,
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

  public function fields() {
    $fields = array();
    $fields['worker_nid'] = 'Worker NID';
    $fields['role'] = 'Role';
    $fields['first'] = 'First';
    $fields['last'] = 'Last';
    $fields['ssn'] = 'SSN';
    $fields['gender'] = 'Gender';
    $fields['phone'] = 'Phone';
    $fields['email'] = 'Email';
    $fields['dob'] = 'DoB';
    $fields['address'] = 'Address';

    return $fields;
  }

  public function preview_links($row) {
    $links = array();
    $links['node/' . $row['relationship_nid']] = 'Relationship';

    if ($row['worker_nid']) {
      $links['node/' . $row['worker_nid']] = 'Worker';
    }

    return $links;
  }

  public function pks(&$form_state) {
    $parameters = $form_state['values']['parameters'];

    $scan = array_filter(array_values($parameters['scan']));
    if (!$scan) { return array(); }

    $roles = array_filter(array_values($parameters['roles']));
    if (!$roles) { return array(); }

    $sql = "select distinct node.nid from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";

    $sql .= "left join field_data_field_sirius_ssn on field_data_field_sirius_ssn.entity_type = 'node' and field_data_field_sirius_ssn.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_gender_nota_calc on field_data_field_sirius_gender_nota_calc.entity_type = 'node' and field_data_field_sirius_gender_nota_calc.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_email on field_data_field_sirius_email.entity_type = 'node' and field_data_field_sirius_email.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_phone on field_data_field_sirius_phone.entity_type = 'node' and field_data_field_sirius_phone.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_dob on field_data_field_sirius_dob.entity_type = 'node' and field_data_field_sirius_dob.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_address on field_data_field_sirius_address.entity_type = 'node' and field_data_field_sirius_address.entity_id = node.nid ";

    if ($parameters['benefit']) {
      $sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.bundle = 'sirius_trust_worker_benefit' and field_data_field_sirius_worker.field_sirius_worker_target_id = node.nid ";

      $sql .= "left join node wb_node on wb_node.nid = field_data_field_sirius_worker.entity_id ";

      $sql .= "left join field_data_field_sirius_active on field_data_field_sirius_active.entity_type = 'node' and field_data_field_sirius_active.entity_id = wb_node.nid ";
      $sql .= "left join field_data_field_sirius_trust_benefit on field_data_field_sirius_trust_benefit.entity_type = 'node' and field_data_field_sirius_trust_benefit.entity_id = wb_node.nid ";
    }

    $sql .= "where node.type = 'sirius_worker' ";
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = sirius_domain_nid();

    if ($parameters['benefit']) {
      $sql .= "and field_sirius_trust_benefit_target_id = :benefit ";
      $sql_args[':benefit'] = $parameters['benefit'];
      $sql .= "and field_sirius_active_value = 'Yes' ";
    }

    $tmp = array();
    if (in_array('address', $scan)) {
      $tmp[] = "field_sirius_address_thoroughfare is null";
      $tmp[] = "field_sirius_address_thoroughfare = ''";
    }
    if (in_array('dob', $scan)) {
      $tmp[] = "field_sirius_dob_value is null";
      $tmp[] = "field_sirius_dob_value  = ''";
    }
    if (in_array('phone', $scan)) {
      $tmp[] = "field_sirius_phone_value is null";
      $tmp[] = "field_sirius_phone_value  = ''";
    }
    if (in_array('email', $scan)) {
      $tmp[] = "field_sirius_email_value is null";
      $tmp[] = "field_sirius_email_value  = ''";
    }
    if (in_array('gender', $scan)) {
      $tmp[] = "field_sirius_gender_nota_calc_value is null";
      $tmp[] = "field_sirius_gender_nota_calc_value  = ''";
    }
    if (in_array('ssn', $scan)) {
      $tmp[] = "field_sirius_ssn_value is null";
      $tmp[] = "field_sirius_ssn_value  = ''";
    }

    $sql .= "and (" . join(' or ', $tmp) . ') ';

    $sql .= "order by nid ";

    $stmt = sirius_sql_query($sql, $sql_args);
    $pks = array();
    while ($hr = $stmt->fetchAssoc()) {
      $pks[] = $hr['nid'];
    }
    return array_chunk($pks, 100);
  }

  public function batch($worker_nids, $values, &$context) {
    $parameters = $values['parameters'];

    $scan = $parameters['scan'];
    if (!$scan) { $scan = array(); }
    $scan = array_filter(array_values($scan));

    $roles = array_filter(array_values($parameters['roles']));
    if (!$roles) { $roles =  array(); }
    $roles = array_filter(array_values($roles));

    $rows = array();

    foreach ($worker_nids as $worker_nid) {
      // Load the worker
      $worker_node = node_load($worker_nid);

      $probe['ssn'] = $worker_node->field_sirius_ssn['und'][0]['value'];
      $probe['gender'] = $worker_node->field_sirius_gender['und'][0]['tid'];
      $probe['phone'] = $worker_node->field_sirius_phone['und'][0]['value'];
      $probe['email'] = $worker_node->field_sirius_email['und'][0]['value'];
      $probe['address'] = $worker_node->field_sirius_address['und'][0]['thoroughfare'];
      $probe['dob'] = $worker_node->field_sirius_dob['und'][0]['value'];

      $ok = TRUE;
      foreach ($scan as $key) { 
        if ($probe[$key]) {
          continue;
        }
        $ok = FALSE; break;
      }
      if ($ok) { continue; }

      $role = 'none';
      if ($parameters['benefit']) {
        $wb_probe = sirius_trust_worker_benefits_search(
          array(
            'worker_nid' => $worker_nid,
            'active' => 'Yes',
            'benefit_nid' => $parameters['benefit'],
          )
        );
        $wb_node = reset($wb_probe);
        if ($wb_node) {
          if ($wb_node->field_sirius_contact_relation['und'][0]['target_id']) {
            $role = 'dependent';
          } else {
            $role = 'subscriber';
          }
        }
      }

      if (!in_array($role, $roles)) { continue; }

      $row['worker_nid'] = $worker_nid;
      $row['first'] = $worker_node->field_sirius_name['und'][0]['given'];
      $row['last'] = $worker_node->field_sirius_name['und'][0]['family'];
      $row['ssn'] = $probe['ssn'];
      $row['gender'] = $worker_node->field_sirius_gender_nota_calc['und'][0]['value'];
      $row['email'] = $probe['email'];
      $row['phone'] = $probe['phone'];

      if ($worker_node->field_sirius_address['und'][0]['thoroughfare']) {
        $row['address'] = sirius_addr_format_oneline($worker_node->field_sirius_address['und'][0]);
      }

      if ($probe['dob']) {
        $row['dob'] = date('Y-m-d', strtotime($probe['dob']));
      } else {
        $row['dob'] = '';
      }
      
      $row['role'] = $role;

      $rows[$worker_nid] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }

  public function render_dashboard($report_id, $output = 'page') {
    $info = $this->info();
    $parameters = $this->parameters($report_id);
    $report_type = $this->type;

    $variables = array();
    $variables['ssn'] = 'SSN';
    $variables['dob'] = 'DoB';
    $variables['gender'] = 'Gender';
    $variables['address'] = 'Addr';

    $query = db_select('report_cache');
    $query->fields('report_cache');
    $query->condition('report_id', $report_id);
    $query->condition('report_type', $report_type);
    $stmt = $query->execute();
    $data = array();
    while ($hr = $stmt->fetchAssoc()) {
      $row = $this->row_decode($hr['report_data']);
      $role = $row['role'];

      foreach ($variables as $key => $label) {
        if (!$row[$key]) { ++$data[$role][$key]; }
      }
    }

    $rows = array();
    foreach ($variables as $key => $label) {
      $data['subscriber'][$key] += 0;
      $data['dependent'][$key] += 0;

      $row = array();
      $row[] = $label;
      $row[] = $data['subscriber'][$key];
      $row[] = $data['dependent'][$key];
      $rows[] = $row;
    }

    $header[] = 'Variable';
    $header[] = 'Subscribers';
    $header[] = 'Dependents';

    return array('header' => $header, 'rows' => $rows);
  }

  public function dashboard_render_full($prefs, $options, $report_id) {
    $report_type = $this->type;

    $parameters = $this->parameters($report_id);

    // $details = $parameters['meta']['dashboard']['details'];

    $tmp = $this->render_dashboard($report_id, $output = 'page');
    $details .= sirius_table_render($tmp['header'], $tmp['rows']);

    $result['tools'] = l('View', "sirius/reports/$report_type", array('query' => array('report_id' => $report_id, 'action' => 'preview'), 'attributes' => array('class' => array('tool-button'))));


    $count = $parameters['meta']['dashboard']['count'];
    $result['count'] = $count;
    $ts = $parameters['meta']['ts']['completed'];
    if (!$ts) { $ts = $parameters['meta']['ts']['started']; }
    if (!$ts) {
      $result['details'] = 'This report has never been run';
    } elseif (isset($count)) {
      $result['details'] = "<strong>$count records</strong> found " . format_interval(time() - $ts) . ' ago.';
    } else {
      $result['details'] = "Last report run " . format_interval(time() - $ts) . ' ago.';
    }
    if ($details) { $result['details'] .= '<br>' . $details; }
    return array($result);
  }
}
