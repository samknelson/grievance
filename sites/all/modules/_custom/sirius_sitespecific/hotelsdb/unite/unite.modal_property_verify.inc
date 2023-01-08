<?php

function unite_property_verify($pa_node) {
	if ($pa_node->type != 'property_actor') { return 'Access denied: not a property / actor node.'; }
	return drupal_get_form('unite_property_verify_form', $pa_node);
}

function unite_property_verify_form($form, &$form_state, $pa_node) {
	$property_node = node_load($pa_node->field_property['und'][0]['target_id']);

  // dismiss if the form was submitted via a modal
  if($form_state['input']['js'] && $form_state['submitted']) {
    ctools_include('modal');
    ctools_include('ajax');
    $commands = array();

    // Dismiss the modal
    $commands[] = ctools_modal_command_dismiss();

    // Update the list of relationships
    $commands[] = ajax_command_replace(
    	'#hotelsdb_actors_table', 
      unite_relationships_render_property($property_node)
    );

    // Show any messages
    $commands[] = ajax_command_replace(
    	'#sirius_modal_messages', 
      '<div id="sirius_modal_messages">' . theme_status_messages(array()) . '</div>'
    );

    print ajax_render($commands);
    drupal_exit();
  }

  $html = '<div class="hotelsdb_verified_details">';
  $last_verified = $pa_node->field_verified_date['und'][0]['value'];
  if ($last_verified) {
  	$last_verified_ts = strtotime($last_verified . ' GMT');
  	$html .= "This record was last verified on <strong>" . date('m/d/Y', $last_verified_ts) . " (" . format_interval(time() - $last_verified_ts, 2) . " ago)</strong> ";
		$html .= " by ";
		$verified_user = user_load($pa_node->field_verified_user['und'][0]['target_id']);
		if ($verified_user) {
			$verified_user_name = $verified_user->name;
		} else {
			$verified_user_name = '[unknown]';
		}
		$html .= "<strong>$verified_user_name</strong>";
  } else {
  	$html .= "This record has <strong>never been verified</strong>.";
  }
  $html .= '</div>';

	$form['#tree'] = TRUE;
	$form_state['pa_nid'] = $pa_node->nid;

	$form['intro'] = array('#markup' => $html);

	$form['verify'] = array(
		'#type' => 'submit',
		'#value' => t('Verify Now'),
		'#name' => 'verify',
	);

	$form['cancel'] = array(
		'#type' => 'submit',
		'#value' => t('Cancel'),
		'#name' => 'cancel',
	);

	return $form;
}

function unite_property_verify_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $form_state['submitted'] = TRUE;
  $pa_node = node_load($form_state['pa_nid']);

  if ($form_state['clicked_button']['#name'] == 'cancel') { 
  	drupal_set_message("Canceled.");
  	return;
  }

  global $user;
  $pa_node->field_verified_date['und'][0]['value'] = gmdate('Y-m-d H:i:s');
  $pa_node->field_verified_user['und'][0]['target_id'] = $user->uid;
  node_save($pa_node);
  drupal_set_message("Verified relationship: " . $pa_node->title);
}


