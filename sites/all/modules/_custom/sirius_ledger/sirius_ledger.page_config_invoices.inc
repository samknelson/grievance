<?php

// This file is obsoelte.
function sirius_ledger_config_invoices_form($form, &$form_state) {
  $form['#tree'] = TRUE;

  $form['invoice']['sirius_ledger_invoice_header'] = array(
    '#type' => 'text_format',
    '#title' => 'Invoice Header',
    '#default_value' => sirius_domain_variable_get('sirius_ledger_invoice_header'),
  );
  $form['invoice']['sirius_ledger_invoice_footer'] = array(
    '#type' => 'text_format',
    '#title' => 'Invoice Footer',
    '#default_value' => sirius_domain_variable_get('sirius_ledger_invoice_footer', ''),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function sirius_ledger_config_invoices_form_submit($form, &$form_state) {
  $values = $form_state['values']['invoice'];
  $values['sirius_ledger_invoice_header'] = sirius_richtext_handle_input($values['sirius_ledger_invoice_header']);
  $values['sirius_ledger_invoice_footer'] = sirius_richtext_handle_input($values['sirius_ledger_invoice_footer']);

  foreach ($values as $key => $value) {
    sirius_domain_variable_set($key, $value);
  }

  drupal_set_message("Settings saved.");
}