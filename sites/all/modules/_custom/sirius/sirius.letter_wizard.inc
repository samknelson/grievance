<?php

function sirius_letter_wizard_info() {
  $items = array();
  $items['letter'] = array(
    'name' => 'letter',
    'title' => 'Letter',
    'description' => 'Send a Letter.',
    'callback_step_list' => 'sirius_letter_wizard_steps',
  );
  return $items;
}

function sirius_letter_wizard_steps($data, $entity_node) {
  $steps = array();
  $steps['which'] = array(
  	'label' => 'Select Letter', 
  	'callback' => 'sirius_letter_wizard_which',
  	'file' => 'sirius.letter_wizard.which.inc',
  	'module' => 'sirius',
  );
  if ($data['format'] == 'email') {
  	$steps['recipients'] = array(
  		'label' => 'Recipients', 
  		'callback' => 'sirius_letter_wizard_recipients_email',
	  	'file' => 'sirius.letter_wizard.recipients_email.inc',
	  	'module' => 'sirius',
  	);
  } else if ($data['format'] == 'sms') {
  	$steps['recipients'] = array(
  		'label' => 'Recipients', 
  		'callback' => 'sirius_letter_wizard_recipients_phone',
	  	'file' => 'sirius.letter_wizard.recipients_sms.inc',
	  	'module' => 'sirius',
  	);
  } else if ($data['format'] == 'fax') {
  	$steps['recipients'] = array(
  		'label' => 'Recipients', 
  		'callback' => 'sirius_letter_wizard_recipients_fax',
	  	'file' => 'sirius.letter_wizard.recipients_fax.inc',
	  	'module' => 'sirius',
  	);
  }
  $steps['body'] = array(
  	'label' => 'Subject and Body', 
  	'callback' => 'sirius_letter_wizard_body',
  	'file' => 'sirius.letter_wizard.body.inc',
  	'module' => 'sirius',
  );
  $steps['send'] = array(
  	'label' => 'Send', 
  	'callback' => 'sirius_letter_wizard_send',
  	'file' => 'sirius.letter_wizard.send.inc',
  	'module' => 'sirius',
  );
  $steps['review'] = array(
  	'label' => 'Review', 
  	'callback' => 'sirius_letter_wizard_review',
  	'file' => 'sirius.letter_wizard.review.inc',
  	'module' => 'sirius',
  );

  return $steps;
}
