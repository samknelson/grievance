<?php

function sirius_hour_node_tab_payperiod_entries_edit($payperiod_node) {
	return drupal_get_form('sirius_hour_node_tab_payperiod_entries_edit_form', $payperiod_node);
}

function sirius_hour_node_tab_payperiod_entries_edit_form($form, &$form_state, $payperiod_node) {
	drupal_add_css(drupal_get_path('module', 'sirius_hour') . '/css/hours.css', 'file');
	drupal_add_js(drupal_get_path('module', 'sirius_hour') . '/js/entry_form.js', 'file');


	$form_state['payperiod_nid'] = $payperiod_node->nid;
	$json = json_decode($payperiod_node->field_sirius_json['und'][0]['value'], TRUE);
	$hour_types = sirius_taxonomy_load('sirius_hour_type', 'tid', 'name', TRUE);
	$departments = sirius_taxonomy_load('grievance_department', 'tid', 'name', TRUE);
	$payperiod_type = $payperiod_node->field_sirius_payperiod_type['und'][0]['value'];

	$extra_entries = $form_state['extra_entries'];
  if ($form_state['clicked_button']['#value'] == 'Add one') { $extra_entries++; }
  if ($form_state['clicked_button']['#value'] == 'Add ten') { $extra_entries += 10; }
  if (!$extra_entries) { $extra_entries = 2; }
  $form_state['extra_entries'] = $extra_entries;

  $sources = sirius_hour_payperiod_sources($json);

  $start_ts = sirius_date_ts($payperiod_node, 'field_sirius_datetime');
  $month_days = date('t', $start_ts);
  $day_options = array(NULL => '-');
  for ($i=1; $i<=$month_days; ++$i) { $day_options[$i] = $i;}

  $form['#tree'] = TRUE;
	$form['wrap_open'] = array('#markup' => '<div class="sirius-form">');

	if ($json['reconcile']['msg'] && ($json['reconcile']['status'] == 'warning')) {
		$warning .= '<div class="sirius_hour_warning">Warning: ' . $json['reconcile']['msg'] . '</div>';
		$form['warning'] = array('#markup' => $warning);
	}


	$form['entries']['wrap_open'] = array('#markup' => "<div id=\"sirius_entries_wrap\">");
  foreach ($sources as $source_key => $source_name) {

  	if (!$source_name) { $source_name = '[no name]'; }
		$form['entries'][$source_key]['intro'] = array('#markup' => '<h2>' . $source_name . '</h2>');

		$form['entries'][$source_key]['source_name'] = array(
			'#type' => 'textfield',
			'#title' => t('Name'),
			'#default_value' => $source_name,
		);

		$form['entries'][$source_key]['status'] = array(
			'#type' => 'select',
			'#options' => array(
				'live' => t('Live - These entries will be posted'),
				'draft' => t('Draft - Incomplete entries which should not be posted'),
				'overridden' => t('Overridden - Entries have been overridden by an upload'),
			),
			'#title' => t('Status'),
		);
		if ($json['entries'][$source_key]['status']) { $form['entries'][$source_key]['status']['#default_value'] = $json['entries'][$source_key]['status']; }

		if ($json['entries'][$source_key]['warning']) {
			$form['entries'][$source_key]['warning'] = array(
				'#markup' => '<div class="sirius_entry_warning"><strong>There is a warning associated with these entries:</strong> ' . $json['entries'][$source_key]['warning'] . '</div>'
			);
			$form['entries'][$source_key]['warning_review'] = array(
				'#type' => 'select',
				'#options' => array(
					NULL => t('Not Reviewed'),
					'reviewed' => t('Reviewed'),
				),
				'#title' => t('Reviewed'),
			);
			if ($json['entries'][$source_key]['warning_review']) { $form['entries'][$source_key]['warning_review']['#default_value'] = $json['entries'][$source_key]['warning_review']; }
		}

		$form['entries'][$source_key]['comment'] = array(
			'#type' => 'textarea',
			'#title' => t('Comment'),
		);
		if ($json['entries'][$source_key]['comment']) { $form['entries'][$source_key]['comment']['#default_value'] = $json['entries'][$source_key]['comment']; }


 		$form['entries'][$source_key]['table_open'] = array('#markup' => 
 			'<table class="sirius_hours_entries_form">' . 
 			'<tr>' . 
 			'<th>' . t('Day') . '</th>' .
 			'<th>' . t('Hour Type') . '</th>' .
 			'<th>' . t('Department') . '</th>' .
 			'<th>' . t('Hours') . '</th>' .
 			'<th>' . t('Actions') . '</th>' .
 			'</tr>');

 		$entry_count = 0;
 		if ($json['entries'][$source_key]['rows']) { $entry_count = count($json['entries'][$source_key]['rows']); }


 		$entry_count += $extra_entries;
	  for ($i=0; $i<$entry_count; ++$i) {
	  	$form['entries'][$source_key]['rows'][$i]['row_open'] = array('#markup' => '<tr>');

			// $form['entries'][$source_key]['rows'][$i]['rownum'] = array('#markup' => '<td>' . ($i+1) . '</td>');

			$form['entries'][$source_key]['rows'][$i]['day'] = array(
				'#type' => 'select',
				'#options' => $day_options,
				'#prefix' => '<td>',
				'#suffix' => '</td>',
			);
			if ($json['entries'][$source_key]['rows'][$i]['day']) { $form['entries'][$source_key]['rows'][$i]['day']['#default_value'] = $json['entries'][$source_key]['rows'][$i]['day']; }

			$form['entries'][$source_key]['rows'][$i]['hour_type'] = array(
				'#type' => 'select',
				'#options' => $hour_types,
				'#prefix' => '<td>',
				'#suffix' => '</td>',
			);
			if ($json['entries'][$source_key]['rows'][$i]['hour_type']) { $form['entries'][$source_key]['rows'][$i]['hour_type']['#default_value'] = $json['entries'][$source_key]['rows'][$i]['hour_type']; }

			$form['entries'][$source_key]['rows'][$i]['department'] = array(
				'#type' => 'select',
				'#options' => $departments,
				'#prefix' => '<td>',
				'#suffix' => '</td>',
			);
			if ($json['entries'][$source_key]['rows'][$i]['department']) { $form['entries'][$source_key]['rows'][$i]['department']['#default_value'] = $json['entries'][$source_key]['rows'][$i]['department']; }

			$form['entries'][$source_key]['rows'][$i]['qty'] = array(
				'#type' => 'textfield',
				'#size' => 6,
				'#prefix' => '<td>',
				'#suffix' => '</td>',
			);
			if ($json['entries'][$source_key]['rows'][$i]['qty'] > 0) { $form['entries'][$source_key]['rows'][$i]['qty']['#default_value'] = $json['entries'][$source_key]['rows'][$i]['qty']; }

			$tools = '<a href="#" class="sirius_table_detailrow_toggle">' . t('show details') . '</a>';
 			if ($json['entries'][$source_key]['rows'][$i]['note']) { $tools .= ' / ' . $json['entries'][$source_key]['rows'][$i]['note']; }
			$form['entries'][$source_key]['rows'][$i]['tools'] = array('#markup' => '<td>' . $tools . '</td>');

	  	$form['entries'][$source_key]['rows'][$i]['row_close'] = array('#markup' => '</tr>');

	  	$form['entries'][$source_key]['rows'][$i]['row2_open'] = array('#markup' => '<tr class="sirius_table_detailrow"><td colspan="5"><div class="sirius_table_detailrow_details">');

			$form['entries'][$source_key]['rows'][$i]['comment'] = array(
				'#type' => 'textarea',
				'#title' => t('Comment'),
			);
			if ($json['entries'][$source_key]['rows'][$i]['comment']) { $form['entries'][$source_key]['rows'][$i]['comment']['#default_value'] = $json['entries'][$source_key]['rows'][$i]['comment']; }

			$form['entries'][$source_key]['rows'][$i]['start'] = array(
				'#type' => 'date',
				'#title' => t('Punch In'),
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
				'#prefix' => '<div class="container-inline">',
				'#suffix' => '</div>',
			);
			if ($json['entries'][$source_key]['rows'][$i]['start'] > 0) { $form['entries'][$source_key]['rows'][$i]['start']['#default_value'] = $json['entries'][$source_key]['rows'][$i]['start']; }

			$form['entries'][$source_key]['rows'][$i]['end'] = array(
				'#type' => 'date',
				'#title' => t('Punch Out'),
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
				'#prefix' => '<div class="container-inline">',
				'#suffix' => '</div>',
			);
			if ($json['entries'][$source_key]['rows'][$i]['end'] > 0) { $form['entries'][$source_key]['rows'][$i]['end']['#default_value'] = $json['entries'][$source_key]['rows'][$i]['end']; }

			$form['entries'][$source_key]['rows'][$i]['delete'] = array(
				'#type' => 'checkbox',
				'#title' => t('Delete'),
			);

	  	$form['entries'][$source_key]['rows'][$i]['row2_close'] = array('#markup' => '</div></td></tr>');
	  }
  	$form['entries'][$source_key]['table_close'] = array('#markup' => '</table>');
  }
	$form['entries']['wrap_close'] = array('#markup' => "</div>");

  $form['add_one'] = array(
    '#type' => 'button',
    '#value' => t('Add one'),
    '#ajax' => array('callback' => 'sirius_generic_entries_addone'),
  );

  $form['add_ten'] = array(
    '#type' => 'button',
    '#value' => t('Add ten'),
    '#ajax' => array('callback' => 'sirius_generic_entries_addone'),
    '#suffix' => '<br />',
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	$form['wrap_close'] = array('#markup' => '</div>');

	return $form;
}

function sirius_hour_node_tab_payperiod_entries_edit_form_submit($form, &$form_state) {
	$payperiod_nid = $form_state['payperiod_nid'];
	$payperiod_node = node_load($payperiod_nid);
	$payperiod_type = $payperiod_node->field_sirius_payperiod_type['und'][0]['value'];
	$json = json_decode($payperiod_node->field_sirius_json['und'][0]['value'], TRUE);
	$json['entries'] = $form_state['values']['entries'];
	sirius_hour_payperiod_compute($json);
	$payperiod_node->field_sirius_json['und'][0]['value'] = json_encode($json);
	node_save($payperiod_node);
}

function sirius_generic_entries_addone($form, $form_state) {
  $commands = array();
  $commands[] = ajax_command_replace('#sirius_entries_wrap', render($form['entries']));
  return array(
    '#type' => 'ajax',
    '#commands' => $commands
  );
}
