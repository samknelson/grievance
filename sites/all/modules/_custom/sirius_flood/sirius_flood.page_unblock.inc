<?php

function sirius_flood_page_unblock() {
	return drupal_get_form('sirius_flood_page_unblock_form');
}

function sirius_flood_page_unblock_form($form, &$form_state) {
  $entries = sirius_flood_list();
  $ip_limit = variable_get('user_failed_login_ip_limit', 50);
  $user_limit = variable_get('user_failed_login_user_limit', 5);

  $header = array(
    'title'    			=> t('Type of block'),
    'identifier'   	=> t('Identifier'),
    'count'   			=> t('Count'),
    'limit'   			=> t('Limit'),
    'blocked'				=> t('Blocked?'),
  );

  $info_all = sirius_flood_info_all();

  $options = array();
  foreach ($entries as $key => $entry) {
  	$event = $entry['event'];
  	$info = $info_all[$event];
  	if ($info) {
  		$title = $info['title'] . '<br />' . $event;
  		$limit = $info['limit'];
  		if ($limit <= $entry['count']) { $blocked = 'Yes'; } else { $blocked = 'No'; }
  	} else {
  		$title = $event;
  		$limit = 'Unknown';
  		$blocked = 'Unknown';
  	}

    $options[$key] = array(
      'title'    		=> $title,
      'identifier' 	=> $entry['identifier'],
      'count'   		=> $entry['count'],
      'limit'				=> $limit,
      'blocked'			=> $blocked,
    );
  }

  $form['table'] = array(
    '#type'    => 'tableselect',
    '#header'  => $header,
    '#options' => $options,
    '#empty'   => t('No flood events found.'),
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Clear flood'),
  );

  if (count($entries) == 0) {
    $form['submit']['#disabled'] = TRUE;
  }

  return $form;
}

function sirius_flood_page_unblock_form_validate($form, &$form_state) {
  $count = 0;
  foreach ($form_state['values']['table'] as $value) {
    if ($value <> "0") {
      $count++;
    }
  }
  if ($count == 0) {
    form_set_error('', t('Please make a selection.'));
  }
}

function sirius_flood_page_unblock_form_submit($form, &$form_state) {
  foreach ($form_state['values']['table'] as $value) {
  	list ($event, $identifier) = explode('|', $value);
  	drupal_set_message("Clearing: $event, $identifier");
    sirius_flood_clear($event, $identifier);
  }
}
