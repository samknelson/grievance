<?php

function sirius_bulk_config() {
  $form = array();

  $media = array(
  	'email' => 'Email',
  	'sms' => 'SMS (text message)',
  	'voice' => 'Voice (robocall)',
  	'choice' => 'Choice (user selects email or SMS)'
  );

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


