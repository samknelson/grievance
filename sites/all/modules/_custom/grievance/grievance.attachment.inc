<?php

function grievance_node_tab_attachments($node) {
  drupal_set_title($node->title . ': ' . t('Attachments'));
  $entity_nid = $node->nid;

  $result = '';

  $grievance_mailattach_user = variable_get('grievance_mailattach_user', '');
  $grievance_id = $node->field_grievance_id['und'][0]['value'];
  if ($grievance_id && $grievance_mailattach_user) {
  	$result .= '<p>';
  	$result .= "(Attachments can also be added by sending email to the address <a href=\"mailto:$grievance_mailattach_user?subject=$grievance_id:+description+of+attachment\" target=\"_blank\">$grievance_mailattach_user</a> ";
 		$result .= "using the subject \"$grievance_id\".  You may also include a description of the attachment by placing it in the subject after the colon, e.g. \"$grievance_id: company's information request response\".)";
		$result .= "</p>";
  }

  $result .= grievance_node_tab_get_addlink("grievance-attachment", array("grievance" => $entity_nid), 'Add a new attachment');
  $result .= views_embed_view('grievance_attachments', 'block', $entity_nid);
  $result .= grievance_node_tab_get_addlink("grievance-attachment", array("grievance" => $entity_nid), 'Add a new attachment');

  return $result;
}

function grievance_attachment_views_exposed_form_alter($form, &$form_state) {
  if (isset($form['field_grievance_document_type_tid']['#options'])) {
    foreach ($form['field_grievance_document_type_tid']['#options'] as $delta => $tmp) {
      if ($delta === 'All') { continue; }
      $tid = array_keys($tmp->option)[0];
      $term = taxonomy_term_load($tid);
      if ($term->field_grievance_can_attach['und'][0]['value'] != 'Yes') {
        $to_erase[] = $delta;
        unset($tmp->option[$tid]);
      }
    }

    /*
    $to_erase = array_reverse($to_erase);
    foreach ($to_erase as $delta) {
      drupal_set_message("Erasing: $delta");
      unset($form['field_grievance_document_type_tid']['#options'][$delta]);
    }

    unset($form['field_grievance_document_type_tid']);
    dpm($form['field_grievance_document_type_tid']['#options']);
    */
  }
}
