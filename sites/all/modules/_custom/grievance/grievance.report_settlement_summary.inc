<?php

class Grievance_Report_Settlement_Summary extends Report_Cache_Report_Generator {
  public $type = 'grievance-report-settlement-summary';

  public function info($result = array()) {
    $result = array(
      'name' => t('Settlements'),
      'access' => 'grievance report',
      'render_callbacks' => array(
        'Summary by Specialist' => array(
          'method' => 'render_summary_by_specialist',
          'csv' => TRUE,
        ),
        'Summary by Employer' => array(
          'method' => 'render_summary_by_employer',
          'csv' => TRUE,
        ),
        'Detail by Grievance' => array(
          'method' => 'render_detail_by_grievance',
          'csv' => TRUE,
        ),
        'Detail by Settlement' => array(
          'method' => 'render_detail_by_settlement',
          'csv' => TRUE,
        ),
      ),
      'module' => 'grievance',
      'file' => 'grievance.report_settlement_summary.inc',
      'dashboard' => FALSE,
      'preview' => array(
        'default_display_fields' => array(
          'specialist_name',
          'company_name',
          'employer_name',
          'grievance_id',
          'grievant_name_first',
          'grievant_name_last',
          'date',
          'amt',
        ),
      ),
    );
    return parent::info($result);
  }

  public function fields() {
    return array(
      'settlement_nid' => 'Settlement NID',
      'grievance_nid' => 'Grievance NID',
      'employer_nid' => 'Employer NID',
      'company_nid' => 'Company NID',
      'specialist_name' => 'Specialist',
      'company_name' => 'Company',
      'employer_name' => 'Employer',
      'grievance_id' => 'Grievance ID',
      'grievant_name_first' => 'Grievant First Name',
      'grievant_name_last' => 'Grievant Last Name',
      'classification' => 'Classification',
      'ts' => 'Timestamp',
      'date' => 'Date',
      'year' => 'Year',
      'month' => 'Month',
      'day' => 'Day',
      'amt' => 'Amount',
      'amt_rcvd' => 'Amount Received',
      'settlement_tags' => 'Settlement Tags',
    );
  }

  public function form() {
    $form['from_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Start Date'),
      '#required' => TRUE,
      '#date_format' => 'm/d/Y',
    );

    $form['to_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('End Date'),
      '#required' => TRUE,
      '#date_format' => 'm/d/Y',
    );

    $form['company'] = array(
      '#type' => 'textfield',
      '#title' => t('Company'),
      '#description' => t('(substring ok)'),
    );

    $form['employer'] = array(
      '#type' => 'textfield',
      '#title' => t('Employer'),
      '#description' => t('(substring ok)'),
    );

    $options = sirius_taxonomy_load('grievance_settlement_tags', 'tid', 'name');
    $form['settlement_tags'] = array(
      '#type' => 'checkboxes',
      '#checkall' => TRUE,
      '#title' => t('Settlement Tags'),
      '#options' => $options,
    );
    return $form;
  }

  public function render_parameters($parameters) {
    $parameters = $parameters['parameters'];
    $html .= sirius_fakerender_field('Start Date:', $parameters['from_date']);
    $html .= sirius_fakerender_field('End Date:', $parameters['to_date']);

    if ($parameters['company']) { $html .= sirius_fakerender_field('Company:', $parameters['company']); }
    if ($parameters['employer']) { $html .= sirius_fakerender_field('Employer:', $parameters['employer']); }
    if ($parameters['settlement_tags']) {
      $tag_tids = array_filter(array_values($parameters['settlement_tags']));
      if ($tag_tids) {
        $tag_names = array();
        foreach ($tag_tids as $tag_tid) {
          $tag_names[] = sirius_term_title($tag_tid);
        }
        $html .= sirius_fakerender_field('Tags:', join(' / ', $tag_names));
      }
    }
    return $html;
  }

  public function pks(&$form_state) {
    $from_ts = strtotime($form_state['values']['parameters']['from_date']);
    $to_ts = strtotime($form_state['values']['parameters']['to_date']);
    $from_date = date('Y-m-d 00:00:00', $from_ts);
    $to_date = date('Y-m-d 00:00:00', strtotime('+1 day', $to_ts));

    $query = new EntityFieldQuery;
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'grievance_settlement');
    $query->fieldCondition('field_grievance_date_1', 'value', $from_date, '>=');
    $query->fieldCondition('field_grievance_date_1', 'value', $to_date, '<');
    $query->fieldCondition('field_grievance_amt', 'value', 0, '>');
    $query->fieldOrderBy('field_grievance_date_1', 'value', 'desc');
    $stmt = $query->execute();
    $nids = array();
    if ($stmt['node']) {
      $nids = array_keys($stmt['node']);
    }
  	return array_chunk($nids, 100, TRUE);
  }

  public function preview_links($row) {
    return array(
      'node/' . $row['settlement_nid'] => 'Settlement',
      'node/' . $row['grievance_nid'] => 'Grievance',
    );
  }

  public function batch($settlement_nids, $values, &$context) {
  	$rows = array(
  		'is_multiple' => TRUE,
  		'rows' => array(),
  	);

  	foreach ($settlement_nids as $settlement_nid) {
  		$row = $this->batch_record($settlement_nid, $lookups, $values);
  		if ($row) {
  			$rows['rows'][$settlement_nid] = $row;
  		}
  	}
  	return $rows;
  }

  function batch_record($settlement_nid, $lookups, $values) {

    $parameters = $values['parameters'];

    // Our result row
    $row = array();
    $row['settlement_nid'] = $settlement_nid;

  	// Load the grievance
  	$settlement_node = node_load($settlement_nid);
    $grievance_node = node_load($settlement_node->field_grievance['und'][0]['target_id']);
    $employer_node = node_load($grievance_node->field_grievance_shop['und'][0]['target_id']);
    $company_node = node_load($employer_node->field_grievance_company['und'][0]['target_id']);

    $row['grievance_nid'] = $grievance_node->nid;
    $row['grievance_id'] = $grievance_node->field_grievance_id['und'][0]['value'];
    $row['grievant_name_first'] = $grievance_node->field_grievance_first_name['und'][0]['value'];
    $row['grievant_name_last'] = $grievance_node->field_grievance_last_name['und'][0]['value'];

    $row['employer_nid'] = $employer_node->nid;
    $row['employer_name'] = $employer_node->title;

    $row['company_nid'] = $company_node->nid;
    $row['company_name'] = $company_node->title;

    // Filter on company, employer.
    // @todo: This should be done in the query, but I'm feeling lazy. It's an EntityFieldQuery which doesn't do joins well,
    //        and I can't be bothered writing a bunch of SQL. So fetch them all and let preg_match sort them out.
    if ($parameters['company']) {
      if (!preg_match('/' . preg_quote($parameters['company'], '/') . '/', $row['company_name'])) { return NULL; }
    }
    if ($parameters['employer']) {
      if (!preg_match('/' . preg_quote($parameters['employer'], '/') . '/', $row['employer_name'])) { return NULL; }
    }

    if ($parameters['settlement_tags']) {
      $tag_tids = array_filter(array_values($parameters['settlement_tags']));
    }
    if ($tag_tids) {
      $tag_found = FALSE;
    } else {
      $tag_found = TRUE;
      $tag_tids = array();
    }

    $tags = array();
    if ($settlement_node->field_grievance_settlement_tags['und'][0]['tid']) {
      foreach ($settlement_node->field_grievance_settlement_tags['und'] as $record) {
        $tags[] = sirius_term_title($record['tid']);
        if (in_array($record['tid'], $tag_tids)) { $tag_found = TRUE; }
      }
    }
    if (!$tag_found) { return NULL; }
    $row['settlement_tags'] = join(' / ', $tags);

    // Specialist name
    // @todo user_load is expensive.
    if ($grievance_node->field_grievance_rep_organizer['und'][0]['target_id']) {
      $user = user_load($grievance_node->field_grievance_rep_organizer['und'][0]['target_id']);
      $row['specialist_name'] = $user->name;
    }

    $row['amt'] = $settlement_node->field_grievance_amt['und'][0]['value'];
    $row['amt_rcvd'] = $settlement_node->field_grievance_amt_rcvd['und'][0]['value'];

    $ts = strtotime($settlement_node->field_grievance_date_1['und'][0]['value']);
    $row['ts'] = $ts;
    $row['year'] = date('Y', $ts);
    $row['month'] = date('m', $ts);
    $row['day'] = date('j', $ts);
    $row['date'] = date('m/j/Y', $ts);

    if ($grievance_node->field_grievance_class_tid['und'][0]['tid']) {
      $row['classification'] = sirius_term_title($grievance_node->field_grievance_class_tid['und'][0]['tid']);
    } else {
      $row['classification'] = $grievance_node->field_grievance_classification['und'][0]['value'];
    }

    return $row;
  }

  public function render_summary_by_specialist($report_id, $output = 'page') {
    return $this->_render_summary($report_id, 'specialist', $output);
  }

  public function render_summary_by_employer($report_id, $output = 'page') {
    return $this->_render_summary($report_id, 'employer', $output);
  }

  private function _render_summary($report_id, $which, $output = 'page') {
    $query = db_select('report_cache');
    $query->fields('report_cache');
    $query->condition('report_id', $report_id);
    $query->condition('report_type', $this->type);
    $stmt = $query->execute();
    $data = array();
    $grand_total = 0;
    while ($hr = $stmt->fetchAssoc()) {
      $hr = unserialize($hr['report_data']);
      $total += $hr['amt'];

      $key1 = $hr[$which . '_name'];
      if (!$key1) { $key1 = '[Unknown]'; }
      $key2 = $hr['year'];

      $data[$key1][$key2] += $hr['amt'];
      $grand_total += $hr['amt'];
    }

    ksort($data);

    $header = array();
    if ($which == 'specialist') {
      $header[] = 'Specialist';
    } else {
      $header[] = 'Employer';
    }
    $header[] = 'Year';
    $header[] = 'Amount';

    $rows = array();
    foreach ($data as $specialist_name => $years) {
      ksort($years);
      foreach ($years as $year => $amt) {
        $row = array();
        $row[] = $specialist_name;
        $row[] = $year;
        $row[] = '$' . number_format($amt, 2);
        $rows[] = $row;
      }
    }

    $row = array();
    $row[] = '<strong>' . t('Total') . '</strong>';
    $row[] = '';
    $row[] = '<strong>' . '$' . number_format($grand_total, 2) . '</strong>';
    $rows[] = $row;

    return array('header' => $header, 'rows' => $rows);
  }

  public function render_detail_by_grievance($report_id, $output = 'page') {
    return $this->_render_detail($report_id, 'grievance', $output);
  }

  public function render_detail_by_settlement($report_id, $output = 'page') {
    return $this->_render_detail($report_id, 'settlement', $output);
  }

  private function _render_detail($report_id, $which, $output = 'page') {
    $query = db_select('report_cache');
    $query->fields('report_cache');
    $query->condition('report_id', $report_id);
    $query->condition('report_type', $this->type);
    $stmt = $query->execute();

    $data = array();
    $total = $total_rcvd = 0;
    while ($hr = $stmt->fetchAssoc()) {
      $hr = unserialize($hr['report_data']);

      if ($which == 'grievance') {
        $pk = $hr['grievance_nid'];
      } else {
        $pk = $hr['settlement_nid'];
      }
      if (!$pk) { continue; }

      $data[$pk]['company_name'] = $hr['company_name'];
      $data[$pk]['employer_name'] = $hr['employer_name'];
      $data[$pk]['grievance_id'] = $hr['grievance_id'];
      $data[$pk]['grievant_name_first'] = $hr['grievant_name_first'];
      $data[$pk]['grievant_name_last'] = $hr['grievant_name_last'];
      $data[$pk]['classification'] = $hr['classification'];
      $data[$pk]['amt'] += $hr['amt'];
      $data[$pk]['amt_rcvd'] += $hr['amt_rcvd'];
      if ($which == 'settlement') {
        $data[$pk]['date'] = $hr['date'];
        $data[$pk]['settlement_tags'] = $hr['settlement_tags'];
      }
      $total += $hr['amt'];
      $total_rcvd += $hr['amt_rcvd'];
    }

    ksort($data);
    foreach ($data as $pk => $record) {
      $data[$pk]['amt'] = '$' . number_format($data[$pk]['amt'], 2);
      $data[$pk]['amt_rcvd'] = '$' . number_format($data[$pk]['amt_rcvd'], 2);
    }
    $rows = array_values($data);

    $header = array();
    $header[] = 'Company';
    $header[] = 'Employer';
    $header[] = 'Grievance ID';
    $header[] = 'First Name';
    $header[] = 'Last Name';
    $header[] = 'Classification';
    $header[] = 'Settlement Amount';
    $header[] = 'Amount Received';
    if ($which == 'settlement') {
      $header[] = 'Date';
      $header[] = 'Tags';
    }

    $row = array();
    $row[] = '<strong>' . t('Total') . '</strong>';
    $row[] = '';
    $row[] = '';
    $row[] = '';
    $row[] = '';
    $row[] = '';
    $row[] = '<strong>' . '$' . number_format($total, 2) . '</strong>';
    $row[] = '<strong>' . '$' . number_format($total_rcvd, 2) . '</strong>';
    if ($which == 'settlement') {
      $row[] = '';
      $row[] = '';
    }
    $rows[] = $row;

    return array('header' => $header, 'rows' => $rows);
  }
}


