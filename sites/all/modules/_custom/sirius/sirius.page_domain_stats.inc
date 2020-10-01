<?php

function sirius_node_tab_sirius_domain_stats_total($domain_node) { 
	return sirius_node_tab_sirius_domain_stats($domain_node, TRUE);
}

function sirius_node_tab_sirius_domain_stats($domain_node, $include_subdomains = FALSE) {
	$info_all = sirius_domain_stats_info_all();
	$json = sirius_jsonfield_util_node_json_get($domain_node);
	if ($include_subdomains) { $stats = $json['stats_total']; } else { $stats = $json['stats']; }

	$rows = array();
	foreach ($info_all as $key => $info) {
		$count = $stats[$key];
		if (!$count) { $count = 0; }
		$count = number_format($count);
		if (!$count) { continue; }

		$title = $info['title_full'];

		$row = array();
		$row[] = $title;
		$row[] = $count;
		$rows[] = $row;
	}
	$header = array(
		t('Variable'),
		t('Value'),
	);
	$html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array('class' => array('sirius_data_table'))));

	$updated_ts = $json['stats']['updated_ts'];
	if ($updated_ts) { $updated_date = format_interval(time() - $updated_ts, 2) . ' ago on ' . date('l, F j, Y H:i:s A', $updated_ts); } else { $updated_date = 'Never'; }
	$html .= '<br /><br />';
	$html .= "<em>" . t('The statistics for this domain were last updated: ') . "<strong>$updated_date</strong></em>";
	return $html;
}

function sirius_node_tab_sirius_domain_stats_calc($domain_node) {
	return drupal_get_form('sirius_node_tab_sirius_domain_stats_calc_form', $domain_node);
}

function sirius_node_tab_sirius_domain_stats_calc_form($form, &$form_state, $domain_node) {
	$form_state['domain_nid'] = $domain_node->nid;

	$form['submit'] = array('#type' => 'submit', '#value' => t('Recaculate Domain Stats Now'));

  $log = sirius_minilog_render_html();
  if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

  return $form;
}

function sirius_node_tab_sirius_domain_stats_calc_form_submit($form, &$form_state) {
	$domain_nid = $form_state['domain_nid'];
	$form_state['rebuild'] = TRUE;
	sirius_domain_stats_calc($domain_nid);
	sirius_domain_stats_calc_tree($domain_nid);
	drupal_set_message("Domain stats recalculated.");
}

