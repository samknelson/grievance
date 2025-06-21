<?php

class Sirius_Trust_Report_WB_Summary extends Report_Cache_Report_Generator {
  public $type = 'sirius-trust-worker-summary';

  public function info($result = array()) {
    $result = array(
      'name' => t('Coverage - Summary'),
      'description' => '',
      'access' => 'sirius trust staff',
      'render_callbacks' => array(
        'View' => array(
          'method' => 'render_view',
          'csv' => TRUE,
        ),
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_trust',
      'file' => 'sirius_trust.report_worker_benefit_summary.inc',
    );

    return parent::info($result);
  }

  public function fields() {
    return array(
      'key' => 'Key',
      'car' => 'Current / Add / Remove',
      'car_name' => 'Current / Add / Remove Name',
      'reltype' => 'Relationship Type TID',
      'reltype_name' => 'Relationship Type Name',
      'employer' => 'Employer',
      'employer_name' => 'Employer Name',
      'benefit' => 'Benefit',
      'benefit_name' => 'Benefit Name',
      'election_type' => 'Election Type',
      'election_type_name' => 'Election Type Name',
      'record_count' => 'Records',
      'subscriber_count' => 'Subscriber Count',
      'dependent_count' => 'Dependent Count',
    );
  }

  public function breakout_options() {
    return array(
      'car' => t('Current / Add / Remove'),
      'employer' => t('Employer'),
      'benefit' => t('Benefit'),
      'reltype' => t('Relationship Type'),
      'election_type' => t('Election Type'),
    );
  }

  public function form() {
    $form['general'] = array(
      '#type' => 'fieldset',
      '#title' => t('Options'),
      '#tree' => TRUE,
    );

    $form['general']['asof'] = array(
      '#title' => t('As Of'),
      '#type' => 'date_popup',
      '#attributes' => array(
        'autocomplete' => 'off',
      ),
      '#date_format' => 'Y-m-d',
    );


    $form['general']['breakout']['#type'] = 'fieldset';
    for ($i = 0; $i<3; ++$i) {
      $form['general']['breakout'][$i] = array(
        '#title' => t('Breakout #@k', array('@k' => $i+1)),
        '#type' => 'select',
        '#options' => array(NULL => t('-- Select --')) + $this->breakout_options(),
      );
    }

    //////////////////////////////////////////////////
    // Current / Add / Remove
    //////////////////////////////////////////////////

    $form['breakout_car'] = array(
      '#type' => 'fieldset',
      '#title' => t('Filter: Current / Add / Remove'),
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['breakout_car']['days'] = array(
      '#type' => 'textfield',
      '#size' => 4,
      '#title' => t('Days Back'),
      '#default_value' => 7,
    );

    $form['breakout_car']['filter'] = array(
      '#title' => t('Current / Add / Remove'),
      '#type' => 'checkboxes',
      '#options' => array(
        'current' => t('Records active as of the "as of" date'),
        'add' => t('Records added within "days back" days before the "as of" date'),
        'remove' => t('Records removed within "days back" days before the "as of" date'),
      ),
      '#checkall' => TRUE,
    );

    //////////////////////////////////////////////////
    // Relationship Type
    //////////////////////////////////////////////////

    $form['breakout_reltype'] = array(
      '#title' => t('Filter - Relationship Type'),
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $options = array('_self' => t('Self')) + sirius_taxonomy_load('sirius_contact_relationship_types', 'tid', 'name');
    $form['breakout_reltype']['filter'] = array(
      '#title' => t('Relationship Types'),
      '#type' => 'checkboxes',
      '#options' => $options,
      '#checkall' => TRUE,
    );

    //////////////////////////////////////////////////
    // Employer
    //////////////////////////////////////////////////

    $form['breakout_employer'] = array(
      '#title' => t('Filter - Employer'),
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['breakout_employer']['filter'] = array(
      '#title' => t('Employers'),
      '#type' => 'checkboxes',
      '#options' => sirius_employer_list(),
      '#checkall' => TRUE,
    );

    //////////////////////////////////////////////////
    // Benefit
    //////////////////////////////////////////////////

    $form['breakout_benefit'] = array(
      '#title' => t('Filter - Benefit'),
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['breakout_benefit']['filter'] = array(
      '#title' => t('Benefits'),
      '#type' => 'checkboxes',
      '#options' => sirius_trust_benefit_options(),
      '#checkall' => TRUE,
    );

    //////////////////////////////////////////////////
    // Benefit
    //////////////////////////////////////////////////

    $form['breakout_election_type'] = array(
      '#title' => t('Filter - Election Type'),
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['breakout_election_type']['filter'] = array(
      '#title' => t('Election Types'),
      '#type' => 'checkboxes',
      '#options' => sirius_taxonomy_load('sirius_trust_election_type', 'tid', 'name'),
      '#checkall' => TRUE,
    );

    return $form;
  }

  public function render_parameters($parameters) {
    $breakouts = $this->breakouts($parameters);
    $brakout_options = $this->breakout_options();
    $parameters = $parameters['parameters'];

    $html = sirius_jsonfield_util_render_all_values_new(
      $this->form(), 
      $parameters, 
      array(
        'skip_empty' => TRUE,
        'fieldset_title_prefix' => '<h2>',
        'fieldset_title_suffix' => '</h2>',
        'no_fieldsets' => TRUE,
      )
    );
    // $html .= sirius_fakerender_field('Tags:', join(' / ', $tag_names));
    return $html;
  }

  public function pks(&$form_state) {
    $breakouts = $this->breakouts($form_state['values']);
    return $this->_pks_recursive($form_state['values']['parameters'], $breakouts, '', 0);
  }

  public function _pks_recursive($parameters, $breakouts, $prefix, $depth) {
    ++$depth;
    if ($depth > 100) { drupal_set_message("Infinite recursion, call the developer.", 'error'); return array(); }

    $pks = array();
    if ($breakouts) {
      $breakout = array_shift($breakouts);
      $values = array_filter(array_values($parameters["breakout_$breakout"]['filter']));
      if (!$values) { $values = array_filter(array_keys($parameters["breakout_$breakout"]['filter'])); }

      foreach ($values as $value) {
        $sub_prefix = "$prefix|$value";
        $pks = array_merge(
          $pks, 
          $this->_pks_recursive($parameters, $breakouts, $sub_prefix, $depth)
        );
      }
    }
    $pks[] = preg_replace('/^\|/', '', $prefix);
    return $pks;
  }

  public function breakouts($values = array()) {
    $tmp = $values['parameters']['general']['breakout'];
    if (!$tmp) { return array(); }
    return array_filter($tmp);
  }

  public function breakout_keys($key, $values) {
    // Figure out our breakout options. This is some ugly code, designed to do this:
    // -- If our key is e.g. 123|456
    // -- and our breakouts are [employer] / [benefit]
    // -- Then our keys are $keys['employer'] => 123, $keys['benefit'] => 456
    $key_parts = explode('|', $key);
    $keys = array();
    $breakouts = $this->breakouts($values);
    foreach ($breakouts as $i => $breakout) {
      if ($breakout) { $keys[$breakout] = $key_parts[$i]; }
    }
    return $keys;
  }

  public function batch($key, $values, &$context) {
    $keys = $this->breakout_keys($key, $values);

    $row = array();
    $row['key'] = $key;

    $breakout_options = array_keys($this->breakout_options());
    foreach ($breakout_options as $breakout_option) { $row[$breakout_option] = $keys[$breakout_option]; }
    if ($keys['employer']) { $row['employer_name'] = sirius_node_title($keys['employer']); }
    if ($keys['benefit']) { $row['benefit_name'] = sirius_node_title($keys['benefit']); }
    if ($keys['election_type']) { $row['election_type_name'] = sirius_term_title($keys['election_type']); }
    if ($keys['reltype']) {
      if ($keys['reltype'] == '_self') {
        $row['reltype_name'] = t('Self');
      } else {
        $term = taxonomy_term_load($keys['reltype']); 
        $row['reltype_name'] = $term->name;
      }
    }
    if ($keys['car']) { $row['car_name'] = $keys['car']; }

    $days = $values['parameters']['breakout_car']['days'];

    $asof = $values['parameters']['general']['asof'];
    if (!$asof) { $asof = date('Y-m-d'); }

    foreach (array('employer', 'reltype', 'benefit', 'election_type') as $breakout_option) {
      if (!$keys[$breakout_option]) {
        if ($values['parameters']["breakout_$breakout_option"]['filter']) {
          $tmp = array_filter(array_values($values['parameters']["breakout_$breakout_option"]['filter']));
          if ($tmp) { $keys[$breakout_option . "s"] = $tmp; }
        }
      }
    }

    /*
    // Show the individual rows, for debugging
    $stmt = sirius_trust_report_worker_benefit_query($asof, $days, $keys, FALSE);
    $count = 0;
    while ($hr = $stmt->fetchAssoc()) {
      ++$count; if ($count > 10) { break; }
      dpm($hr);
    }
    */

    $stmt = $this->query($asof, $days, $keys, TRUE);
    $hr = $stmt->fetchAssoc();

    if (!$hr['record_count']) { return NULL; }
    $row['record_count'] = $hr['record_count'];
    $row['subscriber_count'] = $hr['subscriber_count'];
    $row['dependent_count'] = $hr['dependent_count'];

    return $row;
  }

  public function load($report_id) {
    $info = $this->info();
    $parameters = $this->parameters($report_id);
    $breakouts = $this->breakouts($parameters);
    $report_type = $this->type;

    $query = db_select('report_cache');
    $query->fields('report_cache');
    $query->condition('report_id', $report_id);
    $query->condition('report_type', $report_type);
    $stmt = $query->execute();
    $data = array();
    while ($hr = $stmt->fetchAssoc()) {
      $data[] = unserialize($hr['report_data']);
    }

    // Sort the data
    $election_types = sirius_taxonomy_load('sirius_trust_election_type', 'tid', 'full');
    usort($data, function($a, $b) use ($breakouts, $election_types) {
      foreach ($breakouts as $breakout) {
        // Election types try to sort by "weight" from the taxonomy
        if ($breakout == 'election_type') {
          $a_tid = $a['election_type'];
          $b_tid = $b['election_type'];

          if ($a_tid) { $a_weight = $election_types[$a_tid]->weight; } else { $a_weight = 1000000; }
          if (!$a_weight) { $a_weight = 0; }

          if ($b_tid) { $b_weight = $election_types[$b_tid]->weight; } else { $b_weight = 1000000; }
          if (!$b_weight) { $b_weight = 0; }

          if ($a_weight != $b_weight) { return $a_weight <=> $b_weight; }
        }

        // Everything else sort by name alphabetically
        $a_name = $a[$breakout . "_name"];
        $b_name = $b[$breakout . "_name"];
        if ($a_name == $b_name) { continue; }
        if (!$a_name) { return 1; }
        if (!$b_name) { return -1; }
        return strcmp($a_name, $b_name);
      }
    });

    return $data;
  }

  public function render_view($report_id, $output = 'page') {
    $info = $this->info();
    $parameters = $this->parameters($report_id);
    $breakouts = $this->breakouts($parameters);
    $data = $this->load($report_id);

    foreach ($data as $hr) {
      $row = array();
      foreach ($breakouts as $breakout) {
        $name = $hr[$breakout . "_name"];
        if (!$name) {
          if ($breakout == 'car') { $name = '[CURRENT]'; } else { $name = 'TOTAL'; }
        }
        $row[] = $name;
      }
      // $row[] = $hr['record_count'];

      $row[] = $hr['subscriber_count'];
      $row[] = $hr['dependent_count'] - $hr['subscriber_count'];
      $row[] = $hr['dependent_count'];
      $rows[] = $row;
    }

    $breakout_options = $this->breakout_options();
    foreach ($breakouts as $breakout) {
      $header[] = $breakout_options[$breakout];
    }
    // $header[] = 'Record Count';
    $header[] = 'Subscribers';
    $header[] = 'Dependents';
    $header[] = 'Covered Lives';

    return array('header' => $header, 'rows' => $rows);
  }

  public function dashboard_render_full($prefs, $options, $report_id) {
    $report_type = $this->type;
    $info = report_cache_info($report_type);
    $parameters = $this->parameters($report_id);
    $breakouts = $this->breakouts($parameters);
    $data = $this->load($report_id);

    $rows = array();
    foreach ($data as $hr) {
      $row = array();

      $names = array();
      foreach ($breakouts as $breakout) {
        $name = $hr[$breakout . "_name"];
        if (!$name) { 
          // if ($breakout == 'car') { $name = '[CURRENT]'; } else { $name = 'TOTAL'; }
        }
        if ($name) { $names[] = $name; }
      }
      if (!$names) { continue; }

      $details = join(' / ', $names);
      $details .= "<br />";
      $details .= "<strong>$hr[subscriber_count]</strong> subscribers / ";
      $details .= "<strong>$hr[dependent_count]</strong> covered lives";

      $row = array();
      $row['tools'] = l('View', "sirius/reports/$report_type", array('query' => array('report_id' => $report_id, 'action' => 'render', 'callback-name' => 'View'), 'attributes' => array('class' => array('tool-button'))));
      $count = $parameters['meta']['dashboard']['count'];
      $row['count'] = $hr['record_count'];
      $row['details'] = $details;

      $rows[] = $row;
    }

    return $rows;
  }

  public function query($asof, $days, $keys, $is_summary) {
    if (!$asof) { $asof = date('Y-m-d'); }
    $car = $keys['car'];
    $asof .= ' 00:00:00';

    if (!$days) { $days = 0; }
    $cutoff = date('Y-m-d 00:00:00', strtotime("-$days days", strtotime($asof)));

    //////////////////////////////////////////////////
    // The Query!!!
    //////////////////////////////////////////////////

    if ($is_summary) {
      $sql = "select count(distinct node.nid) as record_count,  ";
      $sql .= "count(distinct dependent_node.nid) as dependent_count, ";
      $sql .= "count(distinct subscriber_node.nid) as subscriber_count ";
    } else {
      $sql = "select distinct node.nid ";
      // $sql .= ", election_node.nid as election_nid ";
    }
    $sql .= "from node ";

    // Join to the domain
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";

    // Join to the benefit
    $sql .= "left join field_data_field_sirius_trust_benefit on field_data_field_sirius_trust_benefit.entity_type = 'node' and field_data_field_sirius_trust_benefit.entity_id = node.nid ";

    // Join to the dependent
    $sql .= "left join field_data_field_sirius_worker dependent_worker on dependent_worker.entity_type = 'node' and dependent_worker.entity_id = node.nid ";
    $sql .= "left join node dependent_node on dependent_node.nid = dependent_worker.field_sirius_worker_target_id ";

    // Join to the relationship node
    $sql .= "left join field_data_field_sirius_contact_relation on field_data_field_sirius_contact_relation.entity_type = 'node' and field_data_field_sirius_contact_relation.entity_id = node.nid ";
    $sql .= "left join node relationship_node on relationship_node.nid = field_data_field_sirius_contact_relation.field_sirius_contact_relation_target_id ";

    // Join to the subscriber worker
    $sql .= "left join field_data_field_sirius_trust_subscriber on field_data_field_sirius_trust_subscriber.entity_type = 'node' and field_data_field_sirius_trust_subscriber.entity_id = node.nid ";
    $sql .= "left join node subscriber_node on subscriber_node.nid = field_sirius_trust_subscriber_target_id ";

    // Join to the relationship type
    $sql .= "left join field_data_field_sirius_contact_reltype on field_data_field_sirius_contact_reltype.entity_type = 'node' and field_data_field_sirius_contact_reltype.entity_id = relationship_node.nid ";

    // Join to the start and end dates
    $sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_date_end on field_data_field_sirius_date_end.entity_type = 'node' and field_data_field_sirius_date_end.entity_id = node.nid ";

    // Join to the employer
    $sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid ";

    // Join to the election
    $sql .= "left join field_data_field_sirius_trust_election on field_data_field_sirius_trust_election.entity_type = 'node' and field_data_field_sirius_trust_election.entity_id = node.nid ";
    $sql .= "left join node election_node on election_node.nid = field_sirius_trust_election_target_id ";

    // Join to the election type
    $sql .= "left join field_data_field_sirius_trust_election_type on field_data_field_sirius_trust_election_type.entity_type = 'node' and field_data_field_sirius_trust_election_type.entity_id = election_node.nid ";
    /*
    $sql .= "left join node election_node on election_node.nid = (";
    $sql .= "  select max(nid) from node sq_node ";
    $sql .= "  left join field_data_field_sirius_worker sq_worker on sq_worker.entity_type = 'node' and sq_worker.entity_id = sq_node.nid ";
    $sql .= "  left join field_data_field_sirius_date_start sq_start on sq_start.entity_type = 'node' and sq_start.entity_id = sq_node.nid ";
    $sql .= "  left join field_data_field_sirius_date_end sq_end on sq_end.entity_type = 'node' and sq_end.entity_id = sq_node.nid ";
    $sql .= "  where sq_node.type = 'sirius_trust_worker_election' ";
    $sql .= "  and ( ";
    $sql .= "    sq_worker.field_sirius_worker_target_id = subscriber_node_self.nid or ";
    $sql .= "    sq_worker.field_sirius_worker_target_id = subscriber_node_other.nid ";
    $sql .= "  ) ";
    $sql .= "  and ( ";
    $sql .= "    sq_start.field_sirius_date_start_value <= :asof ";
    $sql .= "    and (sq_end.field_sirius_date_end_value is null or sq_end.field_sirius_date_end_value >= :asof) ";
    $sql .= "  ) ";
    $sql .= ") ";
    
    // Join to the election type
    $sql .= "left join field_data_field_sirius_trust_election_type on field_data_field_sirius_trust_election_type.entity_type = 'node' and field_data_field_sirius_trust_election_type.entity_id = election_node.nid ";
    */

    $sql .= "where node.type = 'sirius_trust_worker_benefit' ";

    $domain_nid = sirius_domain_nid();
    if ($domain_nid) {
      $sql .= "and field_sirius_domain_target_id = :domain_nid ";
      $sql_args[':domain_nid'] = $domain_nid;
    } else {
      $sql .= "and field_sirius_domain_target_id IS NULL ";
    }

    if ($keys['benefit']) {
      $sql .= 'and field_sirius_trust_benefit_target_id = :benefit ';
      $sql_args[':benefit'] = $keys['benefit'];
    }

    if ($keys['benefits']) {
      $sql .= 'and field_sirius_trust_benefit_target_id in (:benefits) ';
      $sql_args[':benefits'] = $keys['benefits'];
    }

    if ($keys['election_type']) {
      $sql .= 'and field_sirius_trust_election_type_tid = :election_type ';
      $sql_args[':election_type'] = $keys['election_type'];
    }

    if ($keys['election_types']) {
      $sql .= 'and field_sirius_trust_election_type_tid in (:election_types) ';
      $sql_args[':election_types'] = $keys['election_types'];
    }

    if ($keys['reltype']) {
      if ($keys['reltype'] == '_self') {
        $sql .= 'and field_sirius_contact_reltype_tid is null ';
      } else {
        $sql .= 'and field_sirius_contact_reltype_tid = :reltype ';
        $sql_args[':reltype'] = $keys['reltype'];
      }
    }

    if ($keys['reltypes']) {
      $sql .= 'and (';
      $sql .= 'field_sirius_contact_reltype_tid in (:reltypes) ';
      if (in_array('_self', $keys['reltypes'])) {
        $sql .= 'or field_sirius_contact_reltype_tid is null ';
      }
      $sql .= ') ';
      $sql_args[':reltypes'] = $keys['reltypes'];
    }

    if ($keys['employer']) {
      $sql .= 'and field_grievance_shop_target_id = :employer ';
      $sql_args[':employer'] = $keys['employer'];
    }

    if ($keys['employers']) {
      $sql .= 'and field_grievance_shop_target_id in (:employers) ';
      $sql_args[':employers'] = $keys['employers'];
    }

    $sql_args[':asof'] = $asof;
    if (!$car || ($car == 'current')) {
      // drupal_set_message("... start_date <= $asof and (end date is null or end_date >= $asof)");
      $sql .= "and ( ";
      $sql .= "  field_sirius_date_start_value <= :asof ";
      $sql .= "  and (field_sirius_date_end_value is null or field_sirius_date_end_value >= :asof) ";
      $sql .= ") ";
    } else if ($car == 'add') {
      // drupal_set_message("... $cutoff <= start_date <= $asof ");
      $sql .= "and ( ";
      $sql .= "  field_sirius_date_start_value >= :cutoff ";
      $sql .= "  and field_sirius_date_start_value <= :asof ";
      $sql .= ") ";
      $sql_args[':cutoff'] = $cutoff;
    } else if ($car == 'remove') {
      // drupal_set_message("... $cutoff <= end_date <= $asof");
      $sql .= "and ( ";
      $sql .= "  field_sirius_date_end_value >= :cutoff ";
      $sql .= "  and field_sirius_date_end_value <= :asof ";
      $sql .= ") ";
      $sql_args[':cutoff'] = $cutoff;
    } else {
      drupal_set_mesasge("What is a $car?", 'error');
    }

    return sirius_sql_query($sql, $sql_args);
  }
}