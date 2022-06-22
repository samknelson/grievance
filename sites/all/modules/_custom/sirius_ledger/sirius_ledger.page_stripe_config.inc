<?php

function sirius_ledger_page_stripe_config() {
	return drupal_get_form('sirius_ledger_page_stripe_config_form');
}

function sirius_ledger_page_stripe_config_form($form, &$form_state) {
  $form['sirius_ledger_stripe_active'] = array(
    '#title' => t('Stripe Active?'),
    '#type' => 'select',
    '#options' => array(
    	NULL => t('-- Select --'),
    	'No' => t('No'),
    	'Yes' => t('Yes'),
    ),
    '#default_value' => sirius_domain_variable_get('sirius_ledger_stripe_active'),
    '#description' => t('Should Stripe payments be accepted for this domain?')
  );

  $payment_types = sirius_taxonomy_load('sirius_payment_type', 'tid', 'name');
	$form['sirius_ledger_stripe_payment_type'] = array(
    '#title' => t('Payment Type'),
    '#type' => 'select',
    '#options' => array(NULL => t('-- Select --')) + $payment_types,
    '#default_value' => sirius_domain_variable_get('sirius_ledger_stripe_payment_type'),
    '#description' => t('What payment type should be used for Stripe payments?')
  );

  $form['sirius_ledger_stripe_user_name'] = array(
    '#title' => t('Run as User'),
    '#type' => 'textfield',
    '#default_value' => sirius_domain_variable_get('sirius_ledger_stripe_user_name'),
    '#description' => t('Stripe commands (such as webhooks) will run as this user.'),
  );

  $form['sirius_ledger_stripe_mode'] = array(
    '#title' => t('Mode?'),
    '#type' => 'select',
    '#options' => array(
    	NULL => t('-- Select --'),
    	'test' => t('Test Mode'),
    	'live' => t('Live Mode'),
    ),
    '#default_value' => sirius_domain_variable_get('sirius_ledger_stripe_mode'),
    '#description' => t('Should Stripe payments be accepted for this domain?')
  );

  $form['sirius_ledger_stripe_test_account_id'] = array(
    '#title' => t('TEST Account ID'),
    '#type' => 'textfield',
    '#default_value' => sirius_domain_variable_get('sirius_ledger_stripe_test_account_id'),
  );

  $form['sirius_ledger_stripe_test_account_secret'] = array(
    '#title' => t('TEST Secret Key'),
    '#type' => 'textfield',
    '#description' => t('Leave blank unless you want to change this value.'),
    // '#default_value' => sirius_domain_variable_get('sirius_ledger_stripe_test_account_secret'),
  );

  $form['sirius_ledger_stripe_live_account_id'] = array(
    '#title' => t('LIVE Account ID'),
    '#type' => 'textfield',
    '#default_value' => sirius_domain_variable_get('sirius_ledger_stripe_live_account_id'),
  );

  $form['sirius_ledger_stripe_live_account_secret'] = array(
    '#title' => t('LIVE Secret Key'),
    '#type' => 'textfield',
    '#description' => t('Leave blank unless you want to change this value.'),
    // '#default_value' => sirius_domain_variable_get('sirius_ledger_stripe_live_account_secret'),
  );

  $form['submit'] = array(
  	'#type' => 'submit',
  	'#value' => t('Submit'),
  );

  return $form;
}

function sirius_ledger_page_stripe_config_form_validate($form, &$form_state) {
	sirius_require('sirius_ledger', 'sirius_ledger.stripe.inc');

	if ($form_state['values']['sirius_ledger_stripe_test_account_id']) {
		if (!preg_match('/^pk_test_/', $form_state['values']['sirius_ledger_stripe_test_account_id'])) {
			form_set_error('sirius_ledger_stripe_test_account_id', 'Account ID should begin "pk_test_".');
		}
	}

	if ($form_state['values']['sirius_ledger_stripe_live_account_id']) {
		if (!preg_match('/^pk_live_/', $form_state['values']['sirius_ledger_stripe_live_account_id'])) {
			form_set_error('sirius_ledger_stripe_live_account_id', 'Account ID should begin "pk_live_".');
		}
	}

	if ($form_state['values']['sirius_ledger_stripe_test_account_secret']) {
		if (!preg_match('/^sk_test_/', $form_state['values']['sirius_ledger_stripe_test_account_secret'])) {
			form_set_error('sirius_ledger_stripe_test_account_secret', 'Secret Key should begin "sk_test_".');
		}
	}

	if ($form_state['values']['sirius_ledger_stripe_live_account_secret']) {
		if (!preg_match('/^sk_live_/', $form_state['values']['sirius_ledger_stripe_live_account_secret'])) {
			form_set_error('sirius_ledger_stripe_live_account_secret', 'Secret Key should begin "sk_live_".');
		}
	}

  $user_name = $form_state['values']['sirius_ledger_stripe_user_name'];
  if ($user_name) {
    $remote_user = user_load_by_name($user_name);
    if (!$remote_user) {
      form_set_error('sirius_ledger_stripe_user_name', "User $user_name not found.");
    } else if ($remote_user->field_sirius_domain['und'][0]['target_id'] != sirius_domain_nid()) {
      form_set_error('sirius_ledger_stripe_user_name', "User $user_name is not in the current domain.");
    }
  }
}

function sirius_ledger_page_stripe_config_form_submit($form, &$form_state) {
	foreach (array(
		'sirius_ledger_stripe_active', 
		'sirius_ledger_stripe_payment_type',
    'sirius_ledger_stripe_user_name',
		'sirius_ledger_stripe_mode',
		'sirius_ledger_stripe_test_account_id', 
		'sirius_ledger_stripe_test_account_secret',
		'sirius_ledger_stripe_live_account_id', 
		'sirius_ledger_stripe_live_account_secret') as $key) {

		if ($key == 'sirius_ledger_stripe_test_account_secret' && !$form_state['values'][$key]) { continue; }
		if ($key == 'sirius_ledger_stripe_live_account_secret' && !$form_state['values'][$key]) { continue; }

		sirius_domain_variable_set($key, trim($form_state['values'][$key]));
	}

	drupal_set_message("Settings saved.");
}