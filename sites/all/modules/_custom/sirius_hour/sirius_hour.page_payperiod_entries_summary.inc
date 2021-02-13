<?php

function sirius_hour_node_tab_payperiod_entries_summary($payperiod_node) {
	drupal_add_css(drupal_get_path('module', 'sirius_hour') . '/css/hours.css', 'file');
	$json = sirius_jsonfield_util_node_json_get($payperiod_node);
	$hour_types = sirius_taxonomy_load('sirius_hour_type', 'tid', 'name');
	$departments = sirius_taxonomy_load('grievance_department', 'tid', 'name');

	if (!$json['entries']) { return t('There are no entries in this payperiod.'); }

  $sources = sirius_hour_payperiod_sources($json);


  foreach ($sources as $source_key => $source_name) {
  	$html .= '<h2>' . $source_name . '</h2>';
  	$html .=  sirius_fakerender_field('Status:', $json['entries'][$source_key]['status']);
  	$html .=  sirius_fakerender_field('Comment:', $json['entries'][$source_key]['comment']);

	  $rows = array();
  	if ($json['entries'][$source_key]['totals']['hours']['by_type_dept']) {
  		foreach ($json['entries'][$source_key]['totals']['hours']['by_type_dept'] as $hour_type_tid => $by_type) {
  			foreach ($by_type as $department_tid => $qty) {
  				$row = array();
  				$row[] = $hour_types[$hour_type_tid];
  				$row[] = $departments[$department_tid];
  				$row[] = $qty;
  				$rows[] = $row;
  			}
  			$row = array();
  			$row[] = $hour_types[$hour_type_tid];
  			$row[] = '<strong>Total</strong>';
  			$row[] = $json['entries'][$source_key]['totals']['hours']['by_type'][$hour_type_tid];
  			$rows[] = $row;
  		}
			$row = array();
			$row[] = '<strong>Total</strong>';
			$row[] = '';
			$row[] = $json['entries'][$source_key]['totals']['hours']['total'];
			$rows[] = $row;
  	}
		$header = array(
			t('Hour Type'),
			t('Department'),
			t('Hours'),
		);
		$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('sirius_hours_entries'))));
  }


  return $html;
}
