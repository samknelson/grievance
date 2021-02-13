<?php

function sirius_node_tab_event_disclaimer_list_access($event_node) {
	return sirius_node_tab_event_disclaimer_settings_access($event_node); 
}

function sirius_node_tab_event_disclaimer_list($event_node) {
  sirius_event_disclaimer_redirect($event_node);

  $options = array();
  if (preg_match('/yes$/', current_path())) {
  	$options['yes'] = TRUE;
  } else {
  	$options['no'] = TRUE;
  }

  $records = sirius_event_disclaimer_list($event_node, $options);
  if (!$records) { 
    return 'There are no matching participants.';
  }

  $pstatuses = sirius_taxonomy_load('sirius_event_participant_status', 'tid', 'name');
  $proles = sirius_taxonomy_load('sirius_event_participant_role', 'tid', 'name');

  $rows = array();
  foreach ($records as $record) {
    $tools = '';

    $path = 'node/' . $record['participant_nid'] . '/sirius_event_participant_disclaimer';
    if (drupal_valid_path($path)) {
      $tools .= l('Manage', $path, array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => current_path())));
    }

    $row = array();
    $row[] = $record['contact_title'];
    $row[] = $proles[$record['prole_tid']];
    $row[] = $pstatuses[$record['pstatus_tid']];
    $row[] = $tools;
    $rows[] = $row;
  }
  $count = count($records);

  $header = array(
    t('Participant'),
    t('Role'),
    t('Status'),
    t('Tools'),
  );
  
  $html = theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
  if (count($records) > 999) {
    $html .= '<br /><br /><em>' . t('Note: Only the first 1000 records are displayed.') . '</em>';
  }
  return $html;
}
