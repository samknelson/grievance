<?php

function grievance_node_tab_attachments($node) {
  $entity_nid = $node->nid;

  if ($_REQUEST['mailattach_fetch']) {
    drupal_set_message("Fetching messages ...");
    sirius_mailattach_fetch();
    drupal_set_message(sirius_minilog_render_html());
    drupal_set_message("... Done.");
  }

  $result = '';

  $grievance_mailattach_user = sirius_domain_variable_get('sirius_mailattach_user', '');
  $grievance_id = $node->field_grievance_id['und'][0]['value'];
  if ($grievance_id && $grievance_mailattach_user) {
  	$result .= '<p>';
  	$result .= "Attachments can also be added by sending email to the address <a href=\"mailto:$grievance_mailattach_user?subject=$grievance_id:+description+of+attachment\" target=\"_blank\">$grievance_mailattach_user</a> ";
 		$result .= "using the subject \"$grievance_id\".  You may also include a description of the attachment by placing it in the subject after the colon, e.g. \"$grievance_id: company's information request response\". ";
    $result .= '</p>';

    $result .= '<p>';
    $result .= 'New attachments are fetched automatically, or if you just sent an attachment you can <a href=/' . current_path() . '?mailattach_fetch=1>fetch now</a>.';
		$result .= "</p>";
  }

  $result .= grievance_node_tab_get_addlink("grievance-attachment", array("grievance" => $entity_nid), 'Add a new attachment');
  $result .= views_embed_view('grievance_attachments', 'block', $entity_nid);
  $result .= grievance_node_tab_get_addlink("grievance-attachment", array("grievance" => $entity_nid), 'Add a new attachment');


  return $result;
}

function grievance_attachment_views_exposed_form_alter(&$form, &$form_state) {
  if (isset($form['field_grievance_document_type_tid']['#options'])) {
    foreach ($form['field_grievance_document_type_tid']['#options'] as $tid => $tmp) {
      if ($tid === 'All') { continue; }
      if (!is_string($tmp)) { continue; }
      $term = taxonomy_term_load($tid);
      if ($term->field_grievance_can_attach['und'][0]['value'] != 'Yes') {
        $to_erase[] = $tid;
      }
    }

    if ($to_erase) {
	    foreach ($to_erase as $tid) {
	      unset($form['field_grievance_document_type_tid']['#options'][$tid]);
	    }
	  }
  }
}
