<?php

/**
 * Implements hook_block_info
 */
 
function grievance_block_info() {
  $blocks['grievance_timeleft'] = array(
    'info' => t('Grievance Time Left'),
    'cache' => DRUPAL_CACHE_PER_ROLE,
  );

  $blocks['grievance_messaging_footer'] = array(
    'info' => t('Grievance Messaging - Footer'),
    'cache' => DRUPAL_CACHE_PER_ROLE,
  );

  $blocks['grievance_quicksearch'] = array(
    'info' => t('Grievance QuickSearch'),
    'cache' => DRUPAL_CACHE_PER_ROLE,
  );

  return $blocks;
}

function grievance_block_view($delta = '') {
  if ($delta == 'grievance_timeleft') { 
    $block['subject'] = t('');
    $block['content'] = grievance_block_timeleft();
  }

  if ($delta == 'grievance_messaging_footer') { 
    $block['subject'] = t('');
    $block['content'] = variable_get('grievance_messaging_footer', '');
  }

  if ($delta == 'grievance_quicksearch') { 
    $block['subject'] = t('QuickSearch');
    $block['content'] = drupal_get_form('grievance_quicksearch_form');
  }

  return $block;
}

function grievance_block_timeleft() {
  $grievance_node = menu_get_object();
  if (!$grievance_node) { return ''; }
  if ($grievance_node->type != 'grievance') { return ''; }

  $alert_tid = $grievance_node->field_grievance_alert_tid['und'][0]['tid'];
  $alert_term = taxonomy_term_load($alert_tid);
  if ($alert_term) {
    $alert = $alert_term->name;
  }
  $alert_date = $grievance_node->field_grievance_alert_date['und'][0]['value'];
  $alert_date = strtotime($alert_date);

  $need_alert = TRUE;
  if (!$alert) { $need_alert = FALSE; }
  if (!$alert_date) { $need_alert = FALSE; }
  if ($alert_date < date('-3000 days')) { $need_alert = FALSE; }

  if ($need_alert) {
    if (time() <= $alert_date) {
      $interval = format_interval($alert_date - time(), 2) . t(' remaining');
    } else {
      $interval = format_interval(time() - $alert_date, 2) . t(' overdue');
    }

    $datestr = date('l, F j, Y', $alert_date);

    $alert_msg = '<strong>' . 
      t('The Union must') . 
      '</strong> <u>' . 
      $alert . 
      '</u> <strong>' . 
      t('not later than') . 
      '</strong> <u>' .
      $datestr . 
      '</u> <strong>(' .
      $interval . 
      ')</strong>';
  }


  $status_tid = $grievance_node->field_grievance_status['und'][0]['tid'];
  $status_term = taxonomy_term_load($status_tid);
  if ($status_term && $status_term->description) {
    $status_msg = $status_term->description;
  }

  if ($alert_msg && $status_msg) { 
    $msg = $alert_msg . '<hr>' . $status_msg;
  } elseif ($alert_msg) {
    $msg = $alert_msg;
  } elseif ($status_msg) {
    $msg = $status_msg;
  }

  $class = "alert-box ";

  if ($need_alert) {
    if (time() >= $alert_date) { $class .= "alert-box-overdue "; }
    for ($i=1; $i<=5; ++$i) {
      $days = variable_get('grievance_alert_days_warning' . $i, 0);
      if ($days) {
        if (time() >= $alert_date - $days*24*60*60) {
          $class .= "alert-box-warning-$i "; 
        }
      }
    }
  }

  return "<div class=\"$class\">$msg</div>";
}

