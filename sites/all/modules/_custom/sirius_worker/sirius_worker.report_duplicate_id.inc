<?php

function sirius_worker_report_duplicate_id_form($form, &$form_state, $report_type) {
  report_cache_form_prepare($report_type, $form, $form_state);

  $form['key'] = array(
  	'#title' => t('Search for duplicates on'),
  	'#type' => 'select',
  	'#options' => array(
      'id' => t('Id'),
      'id2' => t('Id2'),
      'id3' => t('Id3'),
      'ssn' => t('ssn'),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function sirius_worker_report_duplicate_id_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }



function sirius_worker_report_duplicate_id_fields() {
	$fields = array();
	$fields['worker_nid'] = 'Worker NID';
  $fields['worker_title'] = 'Name';
  $fields['identifier'] = 'Identifier';

	return $fields;
}

function sirius_worker_report_duplicate_id_pks(&$form_state) {
  $key = $form_state['values']['key'];
  $table = 'field_data_field_sirius_' . $key;
  $col = 'field_sirius_' . $key . '_value';
  $sql = "select $col as identifier from $table where entity_type = 'node' and bundle = 'sirius_worker' and $col is not null and $col != '' group by $col having count(*) > 1 order by entity_id";
  $stmt = db_query($sql, array());
  $identifiers = array();
  while ($hr = $stmt->fetchAssoc()) {
    $identifiers[] = $hr['identifier'];
  }
	return array_chunk($identifiers, 100, TRUE);
}

function sirius_worker_report_duplicate_id_batch($identifiers, $values, $report_id, $report_action, &$context) {
  $key = $values['key'];
  $table = 'field_data_field_sirius_' . $key;
  $col = 'field_sirius_' . $key . '_value';

  $rows = array('is_multiple' => TRUE, 'rows' => array());

  foreach ($identifiers as $identifier) {
    $sql = "select * from node, $table where node.nid = $table.entity_id and bundle = 'sirius_worker' and $col = :identifier";
    $stmt = db_query($sql, array(':identifier' => $identifier));
    while ($hr = $stmt->fetchAssoc()) {
      $row = array();
      $row['worker_nid'] = $hr['nid'];
      $row['worker_title'] = $hr['title'];
      if (!$row['worker_title']) { $row['worker_title'] = '(no name)'; }
      $row['identifier'] = $identifier;
      $rows['rows'][$identifier . '/' . $hr["nid"]] = $row;
    }
  }
  return $rows;
}

function sirius_worker_report_duplicate_id_view($report_type, $report_id) {
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

  $html = '';
  $html .= '<h2>';
  $html .= 'Duplicate workers by ' . $parameters['key'];
  $html .= '</h2>';

  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $data = array();
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);
    $identifier = $hr['identifier'];
    $data[$identifier][] = $hr;
  }

  ksort($data);

  $rows = array();
  foreach ($data as $identifier => $subdata) {
    $is_first = TRUE;
    foreach ($subdata as $delta => $hr) {
      $prev_worker_nid = $subdata[$delta-1]['worker_nid'];
      $next_worker_nid = $subdata[$delta+1]['worker_nid'];

      $tools = l('View', 'node/' . $hr['worker_nid'], array('attributes' => array('class' => 'tool-button', 'target' => '_blank')));
      if ($prev_worker_nid) { 
        $tools .= ' ' . l('Merge from Above', 'node/' . $hr['worker_nid'] . '/sirius_worker_merge', array('attributes' => array('class' => 'tool-button', 'target' => '_blank'), 'query' => array('from_worker_nid' => $prev_worker_nid)));
      }
      if ($next_worker_nid) { 
        $tools .= ' ' . l('Merge from Below', 'node/' . $hr['worker_nid'] . '/sirius_worker_merge', array('attributes' => array('class' => 'tool-button', 'target' => '_blank'), 'query' => array('from_worker_nid' => $next_worker_nid)));
      }

      $row = array(); 
      if ($is_first) {
        $is_first = FALSE;
        $row[] = '<strong>' . $identifier . '</strong>';
      } else { 
        $row[] = '';
      }
      $row[] = $hr['worker_title'];
      $row[] = $tools;
      $rows[] = $row;
    }
  }

  $header = array(
  	'Identifier',
  	'Worker Name',
  	'Tools',
  );

  $attributes = array();
  $html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));
  return $html;
}