<?php

function sirius_training_evaluations_fetch_by_worker($worker_nid, $type = NULL) {
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->fieldCondition('field_sirius_log_handler', 'target_id', $worker_nid);
  $query->fieldCondition('field_sirius_category', 'value', 'worker:evaluation');
  if ($type) { $query->fieldCondition('field_sirius_type', 'value', $type); }
  $domain_nid = sirius_domain_nid();
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  $query->propertyOrderBy('created', 'desc');
  $stmt = $query->execute();
  if (!$stmt['node']) { return array(); }
  return node_load_multiple(array_keys($stmt['node']));
}

function sirius_training_evaluations_fetch_by_type($type) {
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->fieldCondition('field_sirius_category', 'value', 'worker:evaluation');
  $query->fieldCondition('field_sirius_type', 'value', $type);
  $query->range(0,100);
  $domain_nid = sirius_domain_nid();
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  $query->propertyOrderBy('created', 'desc');
  $stmt = $query->execute();
  if (!$stmt['node']) { return array(); }
  return node_load_multiple(array_keys($stmt['node']));
}


function sirius_training_evaluations_render($evaluation_node, $options = array()) {
	drupal_add_css('sites/all/modules/_custom/sirius_training/css/evaluation.css');
	$worker_nid = $evaluation_node->field_sirius_log_handler['und'][0]['target_id'];
	$worker_node = node_load($worker_nid);

	$author_user = user_load($evaluation_node->uid);
	if ($author_user->field_sirius_worker['und'][0]['target_id']) {
		$author_worker_node = node_load($author_user->field_sirius_worker['und'][0]['target_id']);
		$author_name = $author_worker_node->title;
	} else {
		$author_name = $author_user->name;
	}

	$status = $evaluation_node->field_sirius_type['und'][0]['value'];
	$html .= "<div class=\"sirius_training_evaluation sirius_training_evaluation_{$status}\">";
	$html .= '<h2>';
	$html .= "Evaluation by "; 
	$html .= $author_name;
	$html .= " of ";
	$html .= $worker_node->title;
	$html .= '</h2>';
	$html .= sirius_fakerender_field('Status:', $evaluation_node->field_sirius_type['und'][0]['value']);
	if (!$options['hide_worker']) {
		$html .= sirius_fakerender_field('Worker:', $worker_node->title . ' / ' . l($worker_node->field_sirius_id['und'][0]['value'], "node/$worker_nid"));
	}
	$html .= sirius_fakerender_field('Submitted:', date('l, F d g:i A', $evaluation_node->created));

	$json = sirius_jsonfield_util_node_json_get($evaluation_node);
	$form = sirius_jsonfield_util_form_get($json, 'evaluation');
	$results = sirius_jsonfield_util_results_get($json, 'evaluation');
	$html .= sirius_jsonfield_util_render_all_values($form, $results);

	if (!$options['hide_edit_link']) {
		$html .= l('Edit', 'node/' . $evaluation_node->nid . '/sirius_log_tc/evaluation_edit', array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => current_path())));
	}
	$html .= '</div>';

	return $html;
}

function sirius_training_evaluation_status_options() {
	return array(
		'draft' => t('Draft'),
		'accepted' => t('Accepted'),
		'rejected' => t('Rejected'),
	);
}

function sirius_training_evaluations_enabled() {
	return sirius_domain_variable_get('sirius_training_evaluations_enabled', '') == 'Yes';
}

function sirius_training_evaluations_enabled_by_worker($worker_node) {
	// Are they globally disabled?
	if (!sirius_training_evaluations_enabled()) { return FALSE; }

	// Are they enabled for any of our member statuses?
	$ms_tids = sirius_fieldvals($worker_node, 'field_sirius_member_status', 'tid');
	foreach ($ms_tids as $ms_tid) {
		if (sirius_training_evaluations_enabled_by_ms_tid($ms_tid)) { return TRUE; }
	}

	return FALSE;
}

function sirius_training_evaluations_enabled_by_ms_tid($ms_tid) {
	$ms_term = taxonomy_term_load($ms_tid);
	$json = json_decode($ms_term->field_sirius_json['und'][0]['value'], TRUE);
	return $json['training']['evaluations_enabled'] == 'Yes';
}
