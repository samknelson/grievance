<?php

/**
 * Much of this code is copied from the contributed module flood_unblock. However, that module had some limitations:
 * - It only handled two built-in types of floods, for login blocks by IP and user
 * - It hadn't been updated since 2011
 * - It's deprecated in favor of flood_control, which hasn't been updated for D7 and doesn't have tools to clear flods
 * So we rolled our own.
 */

function sirius_flood_page_unblock() {
	return drupal_get_form('sirius_flood_page_unblock_form');
}

function sirius_flood_page_unblock_form($form, &$form_state) {
  $entries = sirius_flood_list();

  $header = array(
    'title'    			=> t('Type of block'),
    'identifier'   	=> t('Identifier'),
    'count'   			=> t('Count'),
    'window'        => t('Window'),
    'threshold'     => t('Threshold'),
    'blocked'				=> t('Blocked?'),
  );

  $info_all = sirius_flood_info_all();

  $options = array();
  foreach ($entries as $key => $entry) {
    // "key" is "event|identifier"

    // What type of event (e.g. "Login Failure")
  	$event = $entry['event'];

    // Look up that event type
  	$info = $info_all[$event];
  	if ($info) {
  		$title = sirius_tooltip_render($info['description'], $info['title']) . '<br />' . $event;
  		$threshold = $info['threshold'];
      $window = number_format($info['window'], 0) . ' Seconds';
  		if (sirius_flood_is_allowed($event)) { $blocked = 'No'; } else { $blocked = 'Yes'; }
  	} else {
  		$title = $event;
      $window = 'Unknown';
  		$threshold = 'Unknown';
  		$blocked = 'Unknown';
  	}

    $options[$key] = array(
      'title'    		=> $title,
      'identifier' 	=> $entry['identifier'],
      'count'   		=> $entry['count'],
      'window'      => $window,
      'threshold'   => $threshold,
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
  	if (!$event || !$identifier) { continue; }
  	drupal_set_message("Clearing: $event, $identifier");
    sirius_flood_clear($event, $identifier);
  }
}
