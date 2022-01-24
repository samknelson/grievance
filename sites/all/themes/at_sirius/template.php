<?php

function at_sirius_is_popup() {
  if ($_REQUEST["popup"]) { return TRUE; }
  $parts = explode('/', current_path());
  if ($parts && array_pop($parts) == 'popup') { return TRUE; }
  return FALSE;
}

/**
 * Override or insert variables into page templates.
 */

function at_sirius_preprocess_page(&$vars) {
  // RFC2822 date format
  if ($rfc = date("r" , time())) {
    $vars['datetime_rfc'] = t('@time', array('@time' => $rfc));
  }
  else {
    $rfc = '';
    $vars['datetime_rfc'] = '';
  }
  // ISO 8601 date format
  if ($iso = gmdate('Y-m-d\TH:i:sO')) {
    $vars['datetime_iso'] = $iso;
  }
  else {
    $iso = '';
    $vars['datetime_iso'] = '';
  }
  
  $vars['content_header_attributes_array']['class'][] = 'branding-elements';
  $vars['content_header_attributes_array']['role'][] = 'banner';


  if (variable_get('sirius_banner_css', '')) {
    drupal_add_css(variable_get('sirius_banner_css', ''), 'inline');
  }

	drupal_add_js('sites/all/themes/at_sirius/js/main.js');

  $vars['backlinks'] = sirius_backlinks_render();


  $quickactions = sirius_quickactions_render();
  if ($quickactions && $vars['primary_local_tasks']) {
    foreach ($quickactions as $quickaction) {
      $vars['primary_local_tasks'][] = $quickaction;
    }
  }
}

function at_sirius_preprocess_html(&$vars) {
  if ($vars['user']) {
    foreach($vars['user']->roles as $key => $role){
      $role_class = 'role-' . str_replace(' ', '-', $role);
      $vars["classes_array"][] = $role_class;
    }
  }
}

/**
 * Alter the search block form.
 */
function at_sirius_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'search_block_form') {
    $form['search_block_form']['#title'] = t('Search');
    $form['search_block_form']['#title_display'] = 'invisible';
    $form['search_block_form']['#size'] = 20;
    $form['search_block_form']['#attributes']['placeholder'] = t('Search');
    $form['actions']['submit']['#value'] = t('Go');
  }
}


function at_sirius_preprocess_field(&$vars) {
  //check to see if the field is a boolean
  if ($vars['element']['#field_type'] == 'list_boolean') {
    if ($vars['element']['#items'][0]['value'] == '1') {
      $vars['classes_array'][] = 'boolean-is-true';
    } else {
      $vars['classes_array'][] = 'boolean-is-false';
    }
  }
}

function at_sirius_file_formatter_table($variables) {
  $rows = array();
  foreach ($variables['items'] as $delta => $item) {
    $rows[] = theme('file_link', array('file' => (object) $item));
  }

  return empty($rows) ? '' : theme_item_list(array(
		'items' => $rows, 
		'title' => t('Attachments'),
		'attributes' => array(),
		'type' => 'ul'
	));
}


function at_sirius_date_display_remaining($variables) {
  $remaining_days = $variables['remaining_days'];
  $output = '';
  $show_remaining_text = t(' (Less than 1 day remaining!)');
  if ($remaining_days) {
    $show_remaining_text = format_plural($remaining_days, ' (There is 1 day remaining!)', ' (There are @count days remaining.)');
  }

  return '<span class="date-display-remaining">' . $show_remaining_text . '</span>';
}
