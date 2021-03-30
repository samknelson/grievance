<?php

function sirius_hour_node_tab_payperiod_entries_view($payperiod_node) {
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

  	if ($json['entries'][$source_key]['rows']) {
  		$table_rows = array();
  		foreach ($json['entries'][$source_key]['rows'] as $row) {

				$table_row = array();
				$table_row[] = $row['day'];
				$table_row[] = $hour_types[$row['hour_type']];
				$table_row[] = $departments[$row['department']];
				$table_row[] = number_format($row['qty'], 2);
				$table_rows[] = $table_row;
  		}

			$header = array(
				t('Day'),
				t('Hour Type'),
				t('Department'),
				t('Hours'),
			);
			$html .= theme_table(array('header' => $header, 'rows' => $table_rows, 'attributes' => array('class' => array('sirius_hours_entries'))));
  	}
  }

	$html .= drupal_render(field_view_field('node', $payperiod_node, 'field_sirius_attachments', 'full'));

  return $html;
}
