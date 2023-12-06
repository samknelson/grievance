<?php

function sirius_edls_worker_classification_options() {
  $options =& drupal_static(__FUNCTION__);

  if (!isset($options)) {
    $terms = sirius_taxonomy_load('grievance_job_classification', 'tid', 'full');
    $options = array();
    foreach ($terms as $tid => $term) {
      if ($term->field_sirius_name_short['und'][0]['value']) {
        $options[$tid] = $term->field_sirius_name_short['und'][0]['value'];
      } else {
        $options[$tid] = $term->name;
      }
    }
  }

  return $options;
}

function sirius_edls_worker_list($employer_nid, $date, $filters = array()) {
	// @todo: Both dispatched and outsourced
  return sirius_edls_outsource_list($employer_nid, $date, $filters);
}

function sirius_edls_worker_fetch($employer_nid, $worker_id) {
  if (preg_match('/^O/', $worker_id)) {
    return sirius_edls_outsource_worker_fetch($employer_nid, $worker_id);
  }
  return NULL;
}

function sirius_edls_ms_list($employer_node, $type = 'name') {
  return sirius_edls_outsource_ms_list($employer_node, $type);
}