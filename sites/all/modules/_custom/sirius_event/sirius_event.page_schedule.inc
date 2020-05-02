<?php

function sirius_node_tab_event_schedule_access($event_node) {
  // Admins can create
  if (user_access('sirius event admin')) { return TRUE; }

  // Check our creation rules
  $event_type_tid = $event_node->field_sirius_event_type['und'][0]['tid'];
  $event_type_term = taxonomy_term_load($event_type_tid);
  $json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
  $rule = sirius_event_paccess_rule_match($event_node->nid, $json['edit']['rules']);
  if ($rule) { return TRUE; }

  // Oh well
  return FALSE;
}

function sirius_node_tab_event_schedule($event_node) {
	return drupal_get_form('sirius_node_tab_event_schedule_form', $event_node->nid);
}

function sirius_node_tab_event_schedule_form($form, &$form_state, $event_nid) {
	$form_state['event_nid'] = $event_nid;
	$event_node = node_load($event_nid);
  $event_type_tid = $event_node->field_sirius_event_type['und'][0]['tid'];
  $event_type_term = taxonomy_term_load($event_type_tid);
  $event_type_json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);

  if ($event_type_json['schedule']['repeat'] == 'Yes') { $repeat = TRUE; } else { $repeat = FALSE; }

  if ($repeat) {
  	$date_count = $form_state['date_count'];
  	if (!$date_count && !$event_node->field_sirius_daterepeat['und']) { $date_count = 1; }
  	else if (!$date_count) { $date_count = count($event_node->field_sirius_daterepeat['und']) + 1; }
    if ($form_state['clicked_button']['#value'] == 'Add one') { $date_count++; }
    if ($form_state['clicked_button']['#value'] == 'Add ten') { $date_count += 10; }
    $form_state['date_count'] = $date_count;
  } else {
    $form_state['date_count'] = $date_count = 1;
  }

	$form['prefix'] = array('#markup' => '<div class="sirius-form">');

  $form['dates'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="dates">',
    '#suffix' => '</div>',
  );
  for ($i=0; $i<$date_count; ++$i) {
    $form['dates']["date_${i}"] = array('#markup' => '<h2>' . t('Date #@i', array('@i' => $i+1)) . '</h2>');

    $form['dates']["date_wrap_open_{$i}"] = array('#markup' => '<div class="container-inline">');
    $form['dates']["start_date_${i}"] = array(
      '#title' => t('Date'),
			'#type' => 'date_popup',
			'#timepicker' => 'timepicker',
			'#timepicker_options' => array(
				'rows' => 6,
				'minutes' => array(
				  'starts' => 0,
				  'ends' => 56,
				  'interval' => 5,
				),
			),
			'#attributes' => array(
				'autocomplete' => 'off',
			),
    );

    $form['dates']["duration_${i}"] = array(
      '#title' => t('Duration', array('@i' => $i+1)),
			'#type' => 'select',
			'#options' => sirius_event_duration_options($event_type_term),
    );
    $form['dates']["date_wrap_close_{$i}"] = array('#markup' => '</div>');

    $ts = sirius_date_ts($event_node, 'field_sirius_daterepeat', $i, 'value');
    if ($ts > 0) { $form['dates']["start_date_${i}"]['#default_value'] = date('Y-m-d H:i:s', $ts); }

    $ts2 = sirius_date_ts($event_node, 'field_sirius_daterepeat', $i, 'value2');
    $duration = $ts2 - $ts;
    $form['dates']["duration_${i}"]['#default_value'] = sirius_event_duration_round($duration);
  }

  if ($repeat) {
    $form['dates']['add_one'] = array(
      '#type' => 'button',
      '#value' => t('Add one'),
      '#ajax' => array(
        'callback' => 'sirius_node_tab_event_schedule_form_addone',
        'wrapper' => 'newsections',
      ),
    );

    $form['dates']['add_ten'] = array(
      '#type' => 'button',
      '#value' => t('Add ten'),
      '#ajax' => array(
        'callback' => 'sirius_node_tab_event_schedule_form_addone',
        'wrapper' => 'newsections',
      ),
    );
  }

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Save',
	);

	$form['suffix'] = array('#markup' => '</div>');

	return $form;
}

function sirius_node_tab_event_schedule_form_submit($form, &$form_state) {
	$event_nid = $form_state['event_nid'];
	$event_node = node_load($event_nid);
	unset($event_node->field_sirius_daterepeat['und']);
	$date_count = $form_state['date_count'];
	for ($delta = 0; $delta < $date_count; ++$delta) {
		$ts = strtotime($form_state['values']["start_date_$delta"]);
		if ($ts > 0) { 
			$ts2 = $ts + $form_state['values']["duration_$delta"];
			if ($ts2 <= 0) { $ts2 = $ts; }
			$event_node->field_sirius_daterepeat['und'][$delta]['value'] = gmdate('Y-m-d H:i:s', $ts);
			$event_node->field_sirius_daterepeat['und'][$delta]['value2'] = gmdate('Y-m-d H:i:s', $ts2);
		}

	}

	node_save($event_node);
}

function sirius_node_tab_event_schedule_form_addone($form, $form_state) {
  $commands = array();
  $commands[] = ajax_command_replace("#dates", render($form['dates']));
  return array(
    '#type' => 'ajax',
    '#commands' => $commands
  );
}
