<?php

function sirius_dispatch_report_member_status_ratios_form($form, &$form_state, $report_type) {
  report_cache_form_prepare($report_type, $form, $form_state);

	$member_status_options = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');

  $form['ratio_1'] = array(
  	'#title' => t('For each employer, show me the ratio of employed workers with THIS member status ...'),
  	'#type' => 'select',
  	'#options' => $member_status_options,
  );

  $form['ratio_2'] = array(
  	'#title' => t('... to THIS member status.'),
  	'#type' => 'select',
  	'#options' => $member_status_options,
  );

  $form['industry_tid'] = array(
    '#title' => t('Industry'),
    '#type' => 'select',
    '#options' => array(NULL => t('ALL INDUSTRIES')) + sirius_taxonomy_load('sirius_industry', 'tid', 'name'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function sirius_dispatch_report_member_status_ratios_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }



function sirius_dispatch_report_member_status_ratios_fields() {
	$fields = array();
	$fields['shop_nid'] = 'Employer NID';
	$fields['shop_name'] = 'Employer';

	$tmp = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
	foreach ($tmp as $tid => $name) {
		$fields['status_' . $tid] = $name;
	}
	return $fields;
}

function sirius_dispatch_report_member_status_ratios_pks(&$form_state) {
	$employers = sirius_employer_list();
	return array_keys($employers);
}

function sirius_dispatch_report_member_status_ratios_batch($shop_nid, $values, $report_id, $report_action, &$context) {
  // Find the name of this shop
  $row = array();
  $row['shop_nid'] = $shop_nid;

  // Fetch the name of the shop
  $sql = "select title from node where nid = :shop_nid";
  $stmt = db_query($sql, array(':shop_nid' => $shop_nid));
  $hr = $stmt->fetchAssoc();
  $row['shop_name'] = $hr['title'];

  // Count workers with this employer, grouping by member status.
  $domain_nid = sirius_domain_nid();

  $sql = "select count(distinct node.nid) as worker_count, ";
  $sql .= "field_sirius_member_status_tid as member_status_tid ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_sirius_member_status on field_data_field_sirius_member_status.entity_type = 'node' and field_data_field_sirius_member_status.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";

  $sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.field_sirius_worker_target_id = node.nid ";
  $sql .= "left join node dispatch_node on dispatch_node.nid = field_data_field_sirius_worker.entity_id ";

  $sql .= "left join field_data_field_sirius_dispatch_status on field_data_field_sirius_dispatch_status.entity_type = 'node' and field_data_field_sirius_dispatch_status.entity_id = dispatch_node.nid ";

  $sql .= "left join field_data_field_sirius_dispatch_job on field_data_field_sirius_dispatch_job.entity_type = 'node' and field_data_field_sirius_dispatch_job.entity_id = dispatch_node.nid ";
  $sql .= "left join node job_node on job_node.nid = field_data_field_sirius_dispatch_job.field_sirius_dispatch_job_target_id ";

  $sql .= "left join field_data_field_sirius_industry on field_data_field_sirius_industry.entity_type = 'node' and field_data_field_sirius_industry.entity_id = job_node.nid ";
  $sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = job_node.nid ";
  $sql .= "left join node shop_node on shop_node.nid = field_grievance_shop_target_id ";

  $sql .= "where node.type = 'sirius_worker' ";
  $sql .= "and field_sirius_dispatch_status_value in ('accepted', 'accepted_secondary') ";
  $sql .= "and shop_node.nid = :shop_nid ";
  if ($values['industry_tid']) {
    $sql .= "and field_sirius_industry_tid = :industry_tid ";
    $sql_args[':industry_tid'] = $values['industry_tid'];
  }
  if ($domain_nid) {
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  }
  $sql .= "group by field_sirius_member_status_tid ";

  $sql_args[':shop_nid'] = $shop_nid;
  $stmt = sirius_sql_query($sql, $sql_args);
  while ($hr = $stmt->fetchAssoc()) {
  	$row['status_' . $hr['member_status_tid']] = $hr['worker_count'];
  }

  return $row;
}

function sirius_dispatch_report_member_status_ratios_view($report_type, $report_id) {
  // Load the parameters
  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type . ' :: PARAMETERS');
  $stmt = $query->execute();
  $hr = $stmt->fetchAssoc();
  $parameters = unserialize($hr['report_data']);
  $ratio_1_tid = $parameters['ratio_1'];
  $ratio_2_tid = $parameters['ratio_2'];
  $ratio_1_term = taxonomy_term_load($ratio_1_tid);
  $ratio_2_term = taxonomy_term_load($ratio_2_tid);
  $industry_tid = $parameters['industry_tid'];

  $html = '';
  $html .= '<h2>';
  $html .= 'Ratio of employed workers with the member status <strong>' . $ratio_1_term->name . '</strong> to <strong>' . $ratio_2_term->name . '</strong>';
  if ($industry_tid) {
    $html .= '<br />';
    $industry_term = taxonomy_term_load($industry_tid);
    $html .= 'Industry: ' . $industry_term->name;
  }
  $html .= '</h2>';

  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $data = array();
  $grand_total_1 = 0;
  $grand_total_2 = 0;
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);

    $count_1 = $hr['status_' . $ratio_1_tid];
    $count_2 = $hr['status_' . $ratio_2_tid];

    $grand_total_1 += $count_1;
    $grand_total_2 += $count_2;

    $total = $count_1 + $count_2; 
    if (!$total) { continue; }

    $key = $hr['shop_name'];

    $row = array();
    $row[] = $key;
    $row[] = $total;
    $row[] = $count_1;
    $row[] = (100 *number_format($count_1 / $total, 2)) . ' %';
    $row[] = $count_2;
    $row[] = (100 *number_format($count_2 / $total, 2)) . ' %';

    $data[$key] = $row;
  }

  ksort($data);
  $rows = array_values($data);

  $grand_total = $grand_total_1 + $grand_total_2;
  if ($grand_total) {
  	$row = array();
  	$row[] = '<strong>' . 'Total' . '</strong>';
    $row[] = '<strong>' . $grand_total . '</strong>';
    $row[] = '<strong>' . $grand_total_1 . '</strong>';
    $row[] = '<strong>' . (100 *number_format($grand_total_1 / $grand_total, 2)) . ' %' . '</strong>';
    $row[] = '<strong>' . $grand_total_2;
    $row[] = '<strong>' . (100 *number_format($grand_total_2 / $grand_total, 2)) . ' %' . '</strong>';
    $rows[] = $row;
  }

  $header = array(
  	'Shop Name',
  	'Total Workers',
  	'Count: ' . $ratio_1_term->name,
  	'Fraction: ' . $ratio_1_term->name,
  	'Count: ' . $ratio_2_term->name,
  	'Fraction: ' . $ratio_2_term->name,
  );

  $attributes = array();
  $html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));

  return $html;
}