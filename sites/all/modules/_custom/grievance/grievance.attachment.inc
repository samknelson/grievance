<?php

function grievance_node_tab_attachments($node) {
  drupal_set_title($node->title . ': ' . t('Attachments'));
  $entity_nid = $node->nid;

  $result = '';

  $grievance_mailattach_user = variable_get('grievance_mailattach_user', '');
  $grievance_id = $node->field_grievance_id['und'][0]['value'];
  if ($grievance_id && $grievance_mailattach_user) {
  	$result .= '<p>';
  	$result .= "(Attachments can also be added by sending email to the address <a href=\"mailto:$grievance_mailattach_user?subject=$grievance_id\" target=\"_blank\">$grievance_mailattach_user</a> ";
 		$result .= "using the subject \"$grievance_id\".)";
		$result .= "</p>";
  }

  $result .= grievance_node_tab_get_addlink("grievance-attachment", array("grievance" => $entity_nid), 'Add a new attachment');
  $result .= views_embed_view('grievance_attachments', 'block', $entity_nid);
  $result .= grievance_node_tab_get_addlink("grievance-attachment", array("grievance" => $entity_nid), 'Add a new attachment');

  return $result;
}

