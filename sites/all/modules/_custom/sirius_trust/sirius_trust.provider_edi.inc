<?php

class Sirius_Trust_Provider_Edi_Report_Generator extends Report_Cache_Report_Generator {

	/**
	 * Remove FILTER columns from the default display
	 */

  public function info($result = array()) {
    if (!$result['edi_output_format']) { $result['edi_output_format'] = 'csv'; }

  	$default_display_fields = $this->fields();
    $default_display_fields = array_filter($default_display_fields, function($v) {
      return !preg_match('/FILLER/i', $v);
    });
    $default_display_fields = array_filter($default_display_fields, function($v) {
      return !preg_match('/^RESERVED/i', $v);
    });
    foreach ($this->edi_global_fields() as $field_name => $field_label) {
      unset($default_display_fields[$field_name]);
    }
		$result['preview']['default_display_fields'] = $default_display_fields;
    $result['preview']['preview_links'] = TRUE;

    if ($this->edi_has_header()) {
      $result['render_callbacks']['header'] = array(
        'method' => 'edi_render_header',
        'csv' => TRUE,
      );
    }

    if ($this->edi_has_footer()) {
      $result['render_callbacks']['footer'] = array(
        'method' => 'edi_render_footer',
        'csv' => TRUE,
      );
    }

    if ($this->edi_has_premium()) {
      $result['render_callbacks']['premium'] = array(
        'method' => 'edi_render_premium',
        'csv' => TRUE,
      );
    }

    return parent::info($result);
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
      '#default_value' => date('Y-m-d'),
    );

    $form['report_date'] = array(
      '#title' => 'Report Date', 
      '#type' => 'value', 
      '#value' => date('Y-m-d'),
    );

    $tiers = $this->edi_premium_tiers();
    if ($tiers) {
      $form['premium_rates'] = array(
        '#type' => 'fieldset',
        '#title' => 'Premium Rates',
      );
      foreach ($tiers as $tier_key) {
        $form['premium_rates'][$tier_key] = array(
          '#type' => 'textfield',
          '#size' => 10,
          '#title' => $tier_key,
        );
      }
    }

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

  public function pks(&$form_state) {
    global $user;
    $debug = ($user->uid == 1);

    $benefit_nid = $this->benefit_nid();
    if (!$benefit_nid) { return array(); }

    $asof_date = date('Y-m-d 00:00:00', strtotime($form_state['values']['parameters']['asof_date']));
    $start_date = date('Y-m-d 00:00:00', strtotime('-2 months', strtotime($asof_date)));

    $sql_args = array();
    $sql = "select node.nid as wb_nid, node.title as title, ";
    $sql .= "field_sirius_trust_subscriber_target_id as subscriber_nid, ";
    $sql .= "field_sirius_worker_target_id as worker_nid, ";
    $sql .= "field_sirius_date_end_value as end_date, ";
    $sql .= "field_sirius_contact_relation_target_id as relationship_nid, ";
    $sql .= "field_sirius_contact_reltype_tid as relationship_type_tid ";
    $sql .= "from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_date_end on field_data_field_sirius_date_end.entity_type = 'node' and field_data_field_sirius_date_end.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_trust_benefit on field_data_field_sirius_trust_benefit.entity_type = 'node' and field_data_field_sirius_trust_benefit.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_trust_subscriber on field_data_field_sirius_trust_subscriber.entity_type = 'node' and field_data_field_sirius_trust_subscriber.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_contact_relation on field_data_field_sirius_contact_relation.entity_type = 'node' and field_data_field_sirius_contact_relation.entity_id = node.nid ";
    $sql .= "left join node relationship_node on relationship_node.nid = field_sirius_contact_relation_target_id ";
    $sql .= "left join field_data_field_sirius_contact_reltype on field_data_field_sirius_contact_reltype.entity_type = 'node' and field_data_field_sirius_contact_reltype.entity_id = relationship_node.nid ";

    $sql .= "where node.type = 'sirius_trust_worker_benefit' ";

    if (is_array($benefit_nid)) {
      $sql .= "and field_sirius_trust_benefit_target_id in (:benefit_nids) ";
      $sql_args[':benefit_nids'] = $benefit_nid;
    } else {
      $sql .= "and field_sirius_trust_benefit_target_id = :benefit_nid ";
      $sql_args[':benefit_nid'] = $benefit_nid;
    }

    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = sirius_domain_nid();

    $sql .= "and field_sirius_date_start_value <= :asof_date ";
    $sql_args[':asof_date'] = $asof_date;

    $sql .= "and (field_sirius_date_end_value >= :start_date or field_sirius_date_end_value is null) ";
    $sql_args[':start_date'] = $start_date;

    if ($_REQUEST['worker_nid']) {
      $sql .= "and field_sirius_worker_target_id = :input_worker_nid ";
      $sql_args[':input_worker_nid'] = $_REQUEST['worker_nid'];
    }

    if ($debug) {
      // $sql .= "and field_sirius_worker_target_id in (2528940, 3123041, 3123037, 2520114) ";
    }

    $sql .= "order by field_sirius_trust_subscriber_target_id, field_sirius_worker_target_id ";

    $stmt = sirius_sql_query($sql, $sql_args);

    // Fetch all the records. We can only send each worker once, so we're going to index by worker and then dedupe
    $workers = array();
    while ($hr = $stmt->fetchAssoc()) {
      // Generate "fake" end dates for sorting
      if (!$hr['end_date']) { $hr['end_date'] = '2100-01-01 00:00:00'; }
      $hr['is_subscriber'] = ($hr['worker_nid'] == $hr['subscriber_nid']);
      $hr['dependent_relationship_nids'] = array();
      
      $workers[$hr['worker_nid']][] = $hr;
    }

    // Dedupe. If a worker has multiple records:
    // - Active beats inactive.
    // - Subscriber beats dependent
    $pks = array();
    foreach ($workers as $worker_nid => $records) {
      $best = 0;

      if (count($records) > 1) {
        foreach ($records as $delta => $record) {
          if ($delta == 0) { continue; }

          // Best end date wins
          if ($records[$best]['end_date'] > $records[$delta]['end_date'])   { continue; }
          if ($records[$best]['end_date'] < $records[$delta]['end_date'])   { $best = $delta; continue; }

          // Subscriber beats dependent
          if ($records[$best]['is_subscriber'] && !$records[$delta]['is_subscriber']) { continue; }
          if (!$records[$best]['is_subscriber'] && $records[$delta]['is_subscriber']) { $best = $delta; continue; }
        }
      }

      $workers[$worker_nid] = $records[$best];
    }

    // Remove any dependents with a termination date (always)
    foreach ($workers as $worker_nid => $worker) {
      if ($worker['is_subscriber']) { continue; }
      if (!$worker['end_date']) { continue; }
      if ($worker['end_date'] > $asof_date) { continue; }
      unset($workers[$worker_nid]);
    }

    // Count the subscribers and dependents
    foreach ($workers as $worker_nid => $worker) {
      if ($worker['is_subscriber']) { continue; }
      $workers[$worker['subscriber_nid']]['dependent_relationship_nids'][] = $worker['relationship_nid']; 
    }

    /*
    // Load the relationship terms
    $relationship_type_lookup = sirius_taxonomy_load('sirius_contact_relationship_types', 'tid', 'full');

    // Sort the workers
    uksort($workers, function($a, $b) use ($workers, $relationship_type_lookup) {
      // First sort by nid
      $tmp = ($workers[$a]['subscriber_nid'] <=> $workers[$b]['subscriber_nid']);
      if ($tmp) { return $tmp; }

      // Next, subscribers are first
      if ($workers[$a]['is_subscriber'] && !$workers[$b]['is_subscriber']) { return -1; }
      if (!$workers[$a]['is_subscriber'] && $workers[$b]['is_subscriber']) { return 1; }

      // Next, spouses.
      // @todo: hardcoding this because we haven't set up relationship type weights.
      $reltype_a = $workers[$a]['relationship_type_tid'];
      $reltype_b = $workers[$a]['relationship_type_tid'];
      if ($relationship_type_lookup[$reltype_a]->field_sirius_id['und'][0]['value'] == 'SP') { return -1; }
      if ($relationship_type_lookup[$reltype_b]->field_sirius_id['und'][0]['value'] == 'SP') { return 1; }

      return 0;
    });
    */

    // if ($debug) { drupal_set_message('<pre>' . print_r($workers, 1) . '</pre>'); }

    return array_chunk($workers, 100);
  }

  public function fields() {
    $fields = $this->edi_global_fields();

    $fields += array_combine(array_keys($this->edi_fields()), array_keys($this->edi_fields()));

    return $fields;
  }

  public function edi_premium_amt($context, $tier = NULL) {
    if (!isset($tier)) { $tier = $this->edi_premium_tier($context); }
    return $context['parameters']['premium_rates'][$tier];
  }

  public function edi_premium_tiers() {
    return $this->info()['edi_tiers'];
  }

  public function edi_premium_tier($context) {
    return NULL;
  }

  public function edi_global_fields() {
    return array(
      'subscriber_nid' => 'Subscriber NID',
      'worker_nid' => 'Worker NID',
      'contact_nid' => 'Contact NID',
      'edi_is_subscriber' => 'Subscriber?',
      'edi_ssn' => 'SSN',
      'edi_lastname' => 'Last Name',
      'edi_firstname' => 'First Name',

      'edi_premium_amt' => 'Premium Amt',
      'edi_premium_tier' => 'Premium Tier',
    );
  }

  public function edi_row($row, $context) {
    foreach ($row as $key => $value) { $row[$key] = strtoupper($value); }

    $row['worker_nid'] = $context['worker_node']->nid;
    $row['subscriber_nid'] = $context['subscriber_node']->nid;
    $row['contact_nid'] = $context['contact_node']->nid;
    $row['edi_is_subscriber'] = $this->is_subscriber($context);
    $row['edi_ssn'] = $this->ssn($context, 'worker_node');
    $row['edi_lastname'] = $context['worker_node']->field_sirius_name['und'][0]['family'];
    $row['edi_firstname'] = $context['worker_node']->field_sirius_name['und'][0]['given'];

    $row['edi_premium_tier'] = $this->edi_premium_tier($context);
    $row['edi_premium_amt'] = $this->edi_premium_amt($context);

    return $row;
  }

  public function preview_links($row) {
    $links = array();

    if ($row['worker_nid']) { $links['node/' . $row['worker_nid']] = 'Dependent'; }
    if ($row['subscriber_nid']) { $links['node/' . $row['subscriber_nid']] = 'Subscriber'; }

    return $links;
  }

  public function load($report_id) {
    $report_type = $this->type;
    $query = db_select('report_cache');
    $query->fields('report_cache');
    $query->condition('report_id', $report_id);
    $query->condition('report_type', $report_type);
    $stmt = $query->execute();
    $data = array();
    while ($hr = $stmt->fetchAssoc()) {
      $data[] = $this->row_decode($hr['report_data']);
    }

    return $data;
  }

  public function rowcount($report_id) {
    $report_type = $this->type;
    $query = db_select('report_cache');
    $query->fields('report_cache');
    $query->condition('report_id', $report_id);
    $query->condition('report_type', $report_type);
    $stmt = $query->execute();
    $count = 0;
    while ($hr = $stmt->fetchAssoc()) { ++$count; }
    return $count;
  }

  public function edi_has_header() { return FALSE; }
  public function edi_render_header($report_id, $output = 'page') { return '[Override this function]'; }
  public function edi_has_footer() { return FALSE; }
  public function edi_render_footer($report_id, $output = 'page') { return '[Override this function]'; }

  public function edi_has_premium() { return TRUE; }

  public function edi_carrier_name() { return 'DEFAULT'; }

  public function edi_render_premium($report_id, $output = 'page') { 
    $info = $this->info();
    $parameters = $this->parameters($report_id);
    $parameters = $parameters['parameters'];

    $fields = array();
    $fields['edi_carrier_name'] = 'CARRIER NAME';
    $fields['edi_group_number'] = 'GROUP NUMBER';
    $fields['edi_asof_month'] = 'COVERAGE MONTH';
    $fields['edi_premium_tier'] = 'COVERAGE TIER';
    $fields['edi_premium_amt'] = 'AMOUNT';
    $fields['edi_ssn'] = 'SSN';
    $fields['edi_lastname'] = 'LAST NAME';
    $fields['edi_firstname'] = 'FIRST NAME';
    $fields['edi_payment_month'] = 'PAYMENT MONTH';

    $header = $fields;
    $data = $this->load($report_id);

    $rows = array();

    $edi_asof_month = date('Y-m', strtotime($parameters['asof_date']));
    $edi_payment_month = date('Y-m', strtotime('+1 month', strtotime($parameters['asof_date'])));

    $total = 0;
    foreach ($data as $hr) {
      if (!$hr['edi_is_subscriber']) { continue; }
      $row = array();
      $row['edi_carrier_name'] = $info['edi_carrier_name'];
      $row['edi_group_number'] = $info['edi_group_number'];
      $row['edi_asof_month'] = $edi_asof_month;
      $row['edi_premium_tier'] = $hr['edi_premium_tier'];
      $row['edi_premium_amt'] = $hr['edi_premium_amt'];
      $row['edi_ssn'] = $hr['edi_ssn'];
      $row['edi_firstname'] = strtoupper($hr['edi_firstname']);
      $row['edi_lastname'] = strtoupper($hr['edi_lastname']);
      $row['edi_premium_amt'] = $hr['edi_premium_amt'];
      $row['edi_payment_month'] = $edi_payment_month;
      $rows[] = $row;

      if ($hr['edi_premium_amt']) { $total += $row['edi_premium_amt']; }
    }
    $total = round($total, 2);

    $row = array();
    $row['edi_carrier_name'] = '';
    $row['edi_group_number'] = '';
    $row['edi_asof_month'] = '';
    $row['edi_premium_tier'] = 'TOTAL';
    $row['edi_premium_amt'] = $total;
    $row['edi_ssn'] = '';
    $row['edi_firstname'] = '';
    $row['edi_lastname'] = '';
    $row['edi_payment_month'] = '';
    $rows[] = $row;

    return array('header' => $header, 'rows' => $rows);
  }

	public function edi_output_format() { return $this->info()['edi_output_format']; }

	public function edi_fields() { return array(); }

  public function edi_format_fixed_field($val, $field_data) {
    if (!isset($val)) { $val = ''; }
    $width = $field_data['width'];
    $format = '%-' . $width . '.' . $width . 's';
    return sprintf($format, $val);

    //$result .= "[[val = $val / width = $width / format = $format]]\n";
    //$result .= '[[';
    //$result .= sprintf($format, $val);
    //$result .= ']]';
    //$result .= "\n";
    //return $result;
  }

  public function edi_output_stream($fp, $report_id, $format = NULL, $premium = FALSE) {
    if (!$format || ($format == 'default')) {
      $format = $this->edi_output_format();
    }

    if ($format == 'csv') {
      return $this->edi_output_stream_csv($fp, $report_id, $format, $premium);
    } else {
      return $this->edi_output_stream_fixed($fp, $report_id, $format, $premium);
    }
  }

	public function edi_output_stream_fixed($fp, $report_id, $format = NULL, $premium = FALSE) {
		$report_type = $this->type;
		$domain_nid = sirius_domain_nid(); if (!$domain_nid) { $domain_nid = 0; }
		$info = $this->info();

    if ($premium) {
      fwrite($fp, '[Fixed-width premium output is not implemented]');
      /*
      $result = $this->edi_render_premium($report_id);
      fputcsv($fp, array_values($result['header']));
      foreach ($result['rows'] as $row) {
        fputcsv($fp, array_values($row));
      }
      */
    } else {
  		$query = db_select('report_cache');
  		$query->fields('report_cache');
  		$query->condition('report_type', $report_type);
  		$query->condition('report_id', $report_id);
  		$query->condition('report_domain_nid', $domain_nid);
  		$stmt = $query->execute();

      if ($this->edi_has_header()) {
        $fields = $this->edi_fields('header');
        $result = $this->edi_render_header($report_id);
        foreach ($result['rows'] as $hr) {
          foreach ($fields as $field_name => $field_data) {
            fwrite($fp, $this->edi_format_fixed_field($hr[$field_name], $field_data));
          }
          fwrite($fp, "\n");
        }
      }

  		$fields = $this->edi_fields();
      if ($format == 'debug') {
        fwrite($fp, print_r($fields, 1));
        foreach ($fields as $field_name => $field_data) {
          fwrite($fp, $this->edi_format_fixed_field($field_name, $field_data));
        }
        fwrite($fp, "\n");
      }
  		while ($hr = $stmt->fetchAssoc()) {
  			$hr = $info['generator']->row_decode($hr['report_data']);
  			foreach ($fields as $field_name => $field_data) {
          fwrite($fp, $this->edi_format_fixed_field($hr[$field_name], $field_data));
  			}
  			fwrite($fp, "\n");
  		}

      if ($this->edi_has_footer()) {
        $fields = $this->edi_fields('footer');
        $result = $this->edi_render_footer($report_id);
        foreach ($result['rows'] as $hr) {
          foreach ($fields as $field_name => $field_data) {
            fwrite($fp, $this->edi_format_fixed_field($hr[$field_name], $field_data));
          }
          fwrite($fp, "\n");
        }
      }
    }

		// fclose($fp);
	}

	public function edi_output_stream_csv($fp, $report_id, $format = NULL, $premium = FALSE) {
		$report_type = $this->type;

		$domain_nid = sirius_domain_nid(); if (!$domain_nid) { $domain_nid = 0; }

		$info = $this->info();

    if ($premium) {
      $result = $this->edi_render_premium($report_id);
      fputcsv($fp, array_values($result['header']));
      foreach ($result['rows'] as $row) {
        fputcsv($fp, array_values($row));
      }
    } else {
  		$query = db_select('report_cache');
  		$query->fields('report_cache');
  		$query->condition('report_type', $report_type);
  		$query->condition('report_id', $report_id);
  		$query->condition('report_domain_nid', $domain_nid);
  		$stmt = $query->execute();


      if ($this->edi_has_header()) {
        $fields = array_combine(array_keys($this->edi_fields('header')), array_keys($this->edi_fields('header')));
        fputcsv($fp, array_values($fields));

        $result = $this->edi_render_header($report_id);
        foreach ($result['rows'] as $hr) {
          $row = array();
          foreach ($fields as $field_name => $field_data) {
            $row[] = strip_tags($hr[$field_name]);
          }

          fputcsv($fp, $row);
        }
        fputcsv($fp, array());
      }

      $fields = $info['preview']['default_display_fields'];

      if (!$info['edi_csv_hide_header']) {
        fputcsv($fp, $fields);
      }

  		while ($hr = $stmt->fetchAssoc()) {
  			$hr = $info['generator']->row_decode($hr['report_data']);
  			$row = array();
  			foreach ($fields as $name => $prompt) {
  				$row[] = strip_tags($hr[$name]);
  			}
  			fputcsv($fp, $row);
  		}

      if ($this->edi_has_footer()) {
        $fields = array_combine(array_keys($this->edi_fields('footer')), array_keys($this->edi_fields('footer')));
        fputcsv($fp, array());
        fputcsv($fp, array_values($fields));

        $result = $this->edi_render_footer($report_id);
        foreach ($result['rows'] as $hr) {
          $row = array();
          foreach ($fields as $field_name => $field_data) {
            $row[] = strip_tags($hr[$field_name]);
          }

          fputcsv($fp, $row);
        }
      }
    }
	}

  public function context($worker, $asof_date, $parameters) {
    $wb_nid = $worker['wb_nid'];

    $wb_node = node_load($wb_nid);
    $worker_node = node_load($wb_node->field_sirius_worker['und'][0]['target_id']);
    $subscriber_node = node_load($wb_node->field_sirius_trust_subscriber['und'][0]['target_id']);
    $relationship_nid = $wb_node->field_sirius_contact_relation['und'][0]['target_id'];
    $relationship_node = node_load($relationship_nid);
    $contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);
    $employer_node = node_load($wb_node->field_grievance_shop['und'][0]['target_id']);

    $format = '%010d';

    $pk_parts = array();

    // Sort first by subscriber NID
    $pk_parts[] = sprintf($format, $subscriber_node->nid);

    // Subscribers before depedants
    if ($relationship_node) {
      $pk_parts[] = '1';
    } else {
      $pk_parts[] = '0';
    }

    // Spouses before kids; @todo this needs to deal with relationship weights etc.
    if ($this->relationship_type($relationship_node) == 'SP') {
      $pk_parts[] = '1';
    } else {
      $pk_parts[] = '0';
    }

    // Next worker NID (for uniqueness)
    $pk_parts[] = sprintf($format, $worker_node->nid);

    // Next WB NID (just in case)
    $pk_parts[] = sprintf($format, $wb_node->nid);

    $pk = join('|', $pk_parts);

    $context = array();
    $context['parameters'] = $parameters;
    $context['pk'] = $pk;
    $context['employer_node'] = $employer_node;
    $context['worker_node'] = $worker_node;
    $context['subscriber_node'] = $subscriber_node;
    $context['wb_node'] = $wb_node;
    $context['relationship_node'] = $relationship_node;
    $context['asof_date'] = $asof_date;
    $context['asof_ts'] = strtotime($asof_date);
    $context['contact_node'] = $contact_node;
    $context['dependent_relationship_nodes'] = node_load_multiple($worker['dependent_relationship_nids']);

    return $context;
  }

  public function benefit_nid_from_id($benefit_id) {
    $result = sirius_feed_lookup_by_field('sirius_trust_benefit', 'field_data_field_sirius_id', 'field_sirius_id_value', $benefit_id);
    if (!$result['success']) {
      drupal_set_message("Please make sure that there is a benefit with the ID $benefit_id.", 'error');
      return NULL;
    }
    return $result['nid'];
  }

  public function relationship_type($relationship_node) {
    $reltype_tid = $relationship_node->field_sirius_contact_reltype['und'][0]['tid'];
    if (!$reltype_tid) { return NULL; }
    $reltype_term = taxonomy_term_load($reltype_tid);
    if (!$reltype_term) { return NULL; }
    return $reltype_term->field_sirius_id['und'][0]['value'];
  }

  public function gender_id($worker_node) {
		$lookup = &drupal_static(__FUNCTION__);
    if (!isset($lookup)) { $lookup = sirius_taxonomy_load('sirius_gender', 'tid', 'full'); }
    $tid = $worker_node->field_sirius_gender['und'][0]['tid'];
    if (!$lookup[$tid]) { return ''; }
    return $lookup[$tid]->field_sirius_id['und'][0]['value'];
  }

  public function is_subscriber($context) {
    if ($context['relationship_node']) { return FALSE; }
    return TRUE;
  }

  public function is_dependent($context) {
    return !($this->is_subscriber($context));
  }

  public function is_cobra($context) {
    if (!$context['employer_node']) { return ''; }
    return ($context['employer_node']->field_sirius_id['und'][0]['value'] == 'COBRA');
  }

  public function is_qmsco($context) {
    return ($this->relationship_type($context['relationship_node']) == 'QMSCO');
  }

  public function qmsco_context(&$context) {
    if (!$this->is_qmsco($context)) { return; }
    $qmsco = Sirius_Trust_Qmsco::getInstance();
    $context['qmsco_rp_worker_node'] = $qmsco->rp_worker_node($context['worker_node']);
  }

  public function coverage_start_date($context) {
    if (!$context['wb_node']) { return ''; }
    if (!$context['wb_node']->field_sirius_date_start['und'][0]['value']) { return ''; }
    return date('Ymd', strtotime($context['wb_node']->field_sirius_date_start['und'][0]['value']));
  }

  public function coverage_end_date($context) {
    if (!$context['wb_node']) { return ''; }
    if (!$context['wb_node']->field_sirius_date_end['und'][0]['value']) { return ''; }

    // Don't send end dates after the asof date
    $end_ts = strtotime($context['wb_node']->field_sirius_date_end['und'][0]['value']);
    $asof_ts = strtotime($context['asof_date']);
    if ($end_ts > $asof_ts) { return ''; }

    return date('Ymd', $end_ts);
  }

  public function phone($context, $which) {
    return preg_replace('/[^0-9]/', '', $context[$which]->field_sirius_phone['und'][0]['value']);
  }

  public function email($context, $which) {
    return $context[$which]->field_sirius_email['und'][0]['value'];
  }

  public function dob($context, $which) {
  	$date = $context[$which]->field_sirius_dob['und'][0]['value'];
  	if (!$date) { return ''; }
  	return date('Ymd', strtotime($date));
  }

  public function lang($context) {

  }

  public function ssn($context, $which) {
  	return preg_replace('/[^0-9]/', '', $context[$which]->field_sirius_ssn['und'][0]['value']);
  }

  public function zip($context, $which) {
    return substr(trim($context[$which]->field_sirius_address['und'][0]['postal_code']), 0, 5);
  }

  public function medical_coverage_id($context) {
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
    // $search_params['ts'] = $context['asof_ts'];
    $search_params['benefit_type'] = $benefit_type;
    $wb_nodes = sirius_trust_worker_benefits_search($search_params);
    if (!$wb_nodes) { return ''; }
    $wb_nodes =  sirius_trust_worker_benefits_sort_by_date_only($wb_nodes);

    $wb_node = reset($wb_nodes);
    $benefit_node = node_load($wb_node->field_sirius_trust_benefit['und'][0]['target_id']);
    return $benefit_node->field_sirius_id['und'][0]['value'];
  }
}
