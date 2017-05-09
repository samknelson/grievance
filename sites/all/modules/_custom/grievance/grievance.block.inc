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

/**
 * Return the content of the "Time Left" block for a grievance
 *
 * This content consists of three parts:
 * - A "time remaining" notification before the next task must be completed, if any
 * - A description of the next task to be completed, if any
 * - A view of previous grievances by this member, if any
 *
 * @todo: These three (or at least the "previous grievances by this member") should probably be separate blocks, not one
 */

function grievance_block_timeleft() {
  // Get the current grievance node
  $grievance_node = menu_get_object();
  if (!$grievance_node) { return ''; }
  if ($grievance_node->type != 'grievance') { return ''; }

  // Get the "alert term" and "alert date" -- i.e. the next task to be completed, and the due date for that task.
  $alert_tid = $grievance_node->field_grievance_alert_tid['und'][0]['tid'];
  $alert_term = taxonomy_term_load($alert_tid);
  if ($alert_term) {
    $alert = $alert_term->name;
  }
  $alert_date = $grievance_node->field_grievance_alert_date['und'][0]['value'];
  $alert_date = strtotime($alert_date);
  $actor = $alert_term->field_grievance_actor['und'][0]['value'];

  $need_alert = TRUE;
  if (!$alert) { $need_alert = FALSE; }
  if (!$alert_date) { $need_alert = FALSE; }
  if ($alert_date < date('-3000 days')) { $need_alert = FALSE; }

  $status_tid = $grievance_node->field_grievance_status['und'][0]['tid'];
  $status_term = taxonomy_term_load($status_tid);
  if ($status_term && $status_term->description) {
    $status_msg = $status_term->description;
  }

  if ($need_alert) {
    if (time() <= $alert_date) {
      $interval = format_interval($alert_date - time(), 2) . t(' remaining');
    } else {
      $interval = format_interval(time() - $alert_date, 2) . t(' overdue');
    }

    $datestr = date('l, F j', $alert_date);

    $alert_msg = t(
      'This grievance has status <strong>@status</strong>. The <strong>@actor</strong> must <strong>@alert</strong> not later than <strong>@deadline</strong> (@interval)',
      array(
        '@actor' => $actor, 
        '@status' => $status_term->name,
        '@alert' => $alert_term->name, 
        '@deadline' => $datestr,
        '@interval' => $interval
      )
    );
  }


  if ($alert_msg && $status_msg) { 
    $msg = $alert_msg . '<hr>' . $status_msg;
  } elseif ($alert_msg) {
    $msg = $alert_msg;
  } elseif ($status_msg) {
    $msg = $status_msg;
  }

  if ($grievance_node->field_grievance_id['und'][0]['value']) {
    if ($msg) { $msg .= ' '; }
    $msg .= '[' . t('Grievance ID ') . '<strong>' . $grievance_node->field_grievance_id['und'][0]['value'] . '</strong>' . ']';
  }

  // @todo
  // $msg .= '&nbsp;[' . l("Add a Log Entry", 'grievance/modal/' . $grievance_node->nid . '/log-add') . ']';

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


  $msg = "<div class=\"$class\">$msg</div>";

  $prior_content = views_embed_view('grievance_prior_grievances', 'block', $grievance_node->field_grievance_min['und'][0]['value'], $grievance_node->nid);
  $probe = trim(strip_tags($prior_content));
  if ($probe) {
    drupal_add_js('misc/form.js');
    drupal_add_js('misc/collapse.js');

    $msg .= '<fieldset class="collapsible collapsed">';
    $msg .= '<legend><span class="fieldset-legend">';
    $msg .= t('Other grievances by this employee');
    $msg .= '</span></legend>';
    $msg .= '<div class="fieldset-wrapper">';
    $msg .= $prior_content;
    $msg .= '</div>';
    $msg .= '</fieldset>';
  }

  return $msg;
}

