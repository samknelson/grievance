<?php

function sirius_training_config($form, &$form_state) {
  $form['intro'] = array('#markup' => t('Use the links above to configure training center settings.'));

  return $form;
}


