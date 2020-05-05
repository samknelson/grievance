<?php

function sirius_bulk_config() {
  $form = array();

	$media = sirius_bulk_media();

  foreach ($media as $medium => $medium_label) {
	  $form["sirius_bulk_batch_$medium"] = array(
	    '#type' => 'textfield',
	    '#size' => 6,
	    '#title' => t('Batch size: @medium', array('@medium' => $medium_label)),
	    '#default_value' => variable_get("sirius_bulk_batch_$medium", 50),
	  );

	  $form["sirius_bulk_delay_$medium"] = array(
	    '#type' => 'textfield',
	    '#size' => 6,
	    '#title' => t('Seconds between batches: @medium', array('@medium' => $medium_label)),
	    '#default_value' => variable_get("sirius_bulk_delay_$medium", 300),
	  );
  }

  return system_settings_form($form);
}

function sirius_bulk_page_poll() {
	return drupal_get_form('sirius_bulk_page_poll_form');
}

function sirius_bulk_page_poll_form($form, &$form_state) {
	$form['force'] = array(
		'#type' => 'select',
		'#title' => t('Force?'),
		'#options' => array(
			'no' => t('No, poll normally'),
			'yes' => t('Yes, force-deliver all messages'),
		),
		'#description' => t('Force-deliver will disregard time limitations and flood limitations for services.'),
		'#default_value' => 'no',
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if ($form_state['results']) {
		$form['results'] = array(
			'#markup' => '<h2>' . t('Results') . '</h2><hr>' . $form_state['results'],
		);
	}

	return $form;
}

function sirius_bulk_page_poll_form_submit($form, &$form_state) {
	if ($form_state['values']['force'] == 'yes') { $force = TRUE; } else { $force = FALSE; }

	$result = sirius_bulk_deliver($force);
	drupal_set_message($result['msg']);
	sirius_minilog($result['msg']);

	$form_state['rebuild'] = TRUE;
	$form_state['results'] = sirius_minilog_render_html();
}
