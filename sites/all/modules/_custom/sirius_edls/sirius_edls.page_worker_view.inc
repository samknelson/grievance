<?php

function sirius_edls_page_worker_view($worker_id) {
	$result = sirius_edls_employer_switch();
	$html = $result['markup'];
	$employer_node = $result['employer_node'];
	if (!$employer_node) {
		$html .= sirius_page_error('Please select an employer.');
		return $html;
	}

	$worker = sirius_edls_worker_fetch($employer_node->nid, $worker_id);

	if (!$worker) { return sirius_page_error('Worker Not Found'); }

	$html .= sirius_fakerender_field('Name:', $worker['worker_name']);
	$html .= sirius_fakerender_field('ID:', $worker['worker_id']);
	$html .= sirius_fakerender_field('Member Status ID:', $worker['worker_ms']);
	$html .= sirius_fakerender_field('Phone:', $worker['worker_phone']);

	$params = array();
	$params['grievance_shop'] = $employer_node->nid;
	$params['worker_id'] = $worker_id;
	$params['order'] = 'field_sirius_date_start_value desc';
  $assignments = sirius_edls_assignment_search($params);

  $render_options = array();
  $render_options['hide_employer'] = TRUE;
  $render_options['hide_worker_id'] = TRUE;
  $render_options['hide_worker_name'] = TRUE;
  $render_options['hide_worke_ms'] = TRUE;
  $html .= sirius_edls_assignment_list_render($assignments, $render_options);

	return $html;
}
