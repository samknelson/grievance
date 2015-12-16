<?php

/**
 * Implements hook_block_info
 */
 
function grievance_block_info() {
  $blocks['grievance_timeleft'] = array(
    'info' => t('Grievance Time Left'),
    'cache' => DRUPAL_CACHE_PER_ROLE,
  );

  return $blocks;
}

function grievance_block_view($delta = '') {
  if ($delta == 'grievance_timeleft') { 
    $block['subject'] = t('');
    $block['content'] = grievance_block_timeleft();
  }

  return $block;
}

function grievance_block_timeleft() {
  $grievance_node = menu_get_object();
  if (!$grievance_node) { return ''; }
  if ($grievance_node->type != 'grievance') { return ''; }

  $alert = $grievance_node->field_grievance_alert['und'][0]['value'];
  $alert_date = $grievance_node->field_grievance_alert_date['und'][0]['value'];
  $alert_date = strtotime($alert_date);

  if (!$alert) { return ''; }
  if (!$alert_date) { return ''; }
  if ($alert_date < date('-3000 days')) { return ''; }

  if (time() <= $alert_date) {
    $interval = format_interval($alert_date - time(), 2) . t(' remaining');
  } else {
    $interval = format_interval(time() - $alert_date, 2) . t(' overdue');
  }

  $datestr = date('l, F j, Y', $alert_date);

  $msg = '<strong>' . 
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

  $class = "alert-box ";

  if (time() >= $alert_date) { $class .= "alert-box-overdue "; }
  for ($i=1; $i<=5; ++$i) {
    $days = variable_get('grievance_alert_days_warning' . $i, 0);
    if ($days) {
      if (time() >= $alert_date - $days*24*60*60) {
        $class .= "alert-box-warning-$i "; 
      }
    }
  }

  return "<div class=\"$class\">$msg</div>";
}

