<?php

function sirius_banner_config() {
  $form = array();

  $form['sirius_banner_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Site Name'),
    '#default_value' => variable_get('sirius_banner_name', ''),
    '#description' => t('May contain HTML'),
  );

  $form['sirius_banner_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Banner Text'),
    '#default_value' => variable_get('sirius_banner_text', ''),
    '#description' => t('Usually contact information. May contain HTML.'),
  );

	$form['sirius_banner_logo'] = array(
		'#title' => t('Logo'),
		'#type' => 'managed_file',
		'#default_value' => variable_get('sirius_banner_logo', ''),
		'#upload_location' => 'public://sirius/banner/logo',
	);

	$form = system_settings_form($form);
	$form['#submit'][] = 'sirius_banner_config_submit';	
  return $form;
}


function sirius_banner_config_submit($form, &$form_state) {
	if (isset($form_state['values']['sirius_banner_logo'])) {
		$file = file_load($form_state['values']['sirius_banner_logo']);
		if ($file) {
			$file->status = FILE_STATUS_PERMANENT;
			file_save($file);
			variable_set('sirius_banner_logo', $file->fid);
		}
	}
	else {
		$file = file_load(variable_get('sirius_banner_logo', ''));
		if ($file->fid) {
			file_delete($file, TRUE);
		}
	}
}