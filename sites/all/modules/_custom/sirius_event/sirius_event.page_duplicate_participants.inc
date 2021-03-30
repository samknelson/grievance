<?php

/**
 * This is basically a oneoff, though somebody else might find it useful some day.
 */

function sirius_event_page_duplicate_participants() {
  $sql = "select field_sirius_event_target_id as event_nid, ";
  $sql .= "group_concat(distinct nid order by nid separator ',') as participant_nids, ";
  $sql .= "count(distinct nid) as c ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_event_participant' ";
  $sql .= "group by field_sirius_event_target_id, field_sirius_contact_target_id ";
  $sql .= "having count(nid) > 1 ";
  $sql .= "order by nid ";

  $rows = array();
  $stmt = sirius_sql_query($sql, array());
  while ($hr = $stmt->fetchAssoc()) {
    $participant_nids = explode(',', $hr['participant_nids']);
    $participant_tools = '';
    foreach ($participant_nids as $participant_nid) {
      $participant_tools .= l($participant_nid, "node/$participant_nid/edit", array('attributes' => array('class' => 'tool-button')));
    }
    $row = array();
    $row[] = l($hr['event_nid'], 'node/' . $hr['event_nid']);
    $row[] = $participant_tools;
    $rows[] = $row;
  }

  $header = array(
    'Event',
    'Participants'
  );

  return theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
}