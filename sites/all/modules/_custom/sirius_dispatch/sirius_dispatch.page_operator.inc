<?php

use Twilio\Rest\Client;
use Twilio\Jwt\ClientToken;

function sirius_dispatch_page_operator() {
	drupal_add_js('https://media.twiliocdn.com/sdk/js/client/v1.6/twilio.min.js', 'external');
	drupal_add_js(drupal_get_path('module', 'sirius_dispatch') . '/js/sirius_dispatch.js', 'file');
	drupal_add_css(drupal_get_path('module', 'sirius_dispatch') . '/css/sirius_dispatch.css');
  drupal_add_library('system', 'ui.tabs');

  try {
    $client = sirius_twilio_client_get();
    $token = sirius_twilio_capability_token_get();
    drupal_add_js(array('sirius_twilio' => array('token' => $token)), array('type' => 'setting'));

    $capability = new ClientToken($accountSid, $authToken);
    $capability->allowClientOutgoing($appSid);
    $capability->allowClientIncoming('joey');
    $token = $capability->generateToken();

  } catch (Exception $e) {
    drupal_set_message("Error: " . $e->getCode() . ": " . $e->getMessage(), 'error');
  }

  // Status
  $html .= '<div id="sirius_dispatch_operator_statusbar">';
  $html .= 'Status: ';
  $html .= '<span id="sirius_dispatch_operator_status">Ready</span> / ';
  $html .= '<span id="sirius_dispatch_operator_refresh_status">Never refreshed</span> / ';
  $html .= '<a href="#" id="sirius_dispatch_operator_refresh">Refresh Now</a>';
  $html .= '<span id="sirius_dispatch_operator_volume">';
  $html .= '<div id="sirius_dispatch_operator_volume_input_wrapper"><div id="sirius_dispatch_operator_volume_input"></div></div>';
  $html .= '<div id="sirius_dispatch_operator_volume_output_wrapper"><div id="sirius_dispatch_operator_volume_output"></div></div>';
  $html .= '</span>';
  $html .= '</div>';
  // $html .= '<a href="#" id="sirius_dispatch_operator_refresh_auto_toggle">Auto-Refresh: On</a>';

  $html .= '<table>';
  $html .= '<tr>';

  // Phone and queue
  $html .= '<td valign="top" width="50%" style="vertical-align: top">';
  $html .= '<div id="sirius_dispatch_operator_call_controls">';
  $html .= '<input id="phone-number" type="text" placeholder="Enter phone # or client name" />';
  $html .= '<button id="button-search">Lookup</button>';
  $html .= '<button id="button-call">Call</button>';
  $html .= '<button id="button-hangup">Hangup</button>';
  $html .= '</div>';
  $html .= '<div id="sirius_dispatch_operator_queue">Loading...</div>';
  $html .= '</td>';


  $html .= '<td valign="top" width="50%" style="vertical-align: top">';
  $html .= '<div id="sirius_dispatch_operator_dispatch"></div>';
  $html .= '</td>';
  $html .= '</tr>';
  $html .= '</table>';


  // (Hidden) device selection
  $html .= '<div id="sirius_dispatch_operator_devices">';
  $html .= 'Ringtone Devices';
  $html .= '<select id="sirius_dispatch_operator_ringtone_devices" multiple></select>';
  $html .= '<label>Speaker Devices</label>';
  $html .= '<select id="sirius_dispatch_operator_speaker_devices" multiple></select><br/>';
  $html .= '<a id="get-devices">Seeing unknown devices?</a>';
  $html .= '</div>';

  return $html;
}

function sirius_dispatch_page_operator_ajax($action) {
  if ($action == 'queue') {
    $html = sirius_dispatch_page_operator_ajax_queue();
  } else if ($action == 'dispatch_details') {
    $html = sirius_dispatch_page_operator_ajax_dispatch_details();
  } else if ($action == 'notify') {
    $html = sirius_dispatch_page_operator_ajax_notify();
  } else {
    $html = "Unknown action: $action";
  }
  print $html;
  exit();
}

function sirius_dispatch_page_operator_ajax_notify() {
  $medium = $_REQUEST['medium'];
  $dispatch_nid = $_REQUEST['dispatch_nid'];

  sirius_dispatch_notify(node_load($dispatch_nid), $medium);
  return 'Notification sent';
}

function sirius_dispatch_page_operator_ajax_dispatch_details() {
  $dispatch_nid = $_REQUEST['dispatch_nid'];
  $dispatch_node = node_load($dispatch_nid);
  $renderable = node_view($dispatch_node, 'teaser');
  $dispatch_html = drupal_render($renderable);

  $job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
  $renderable = node_view($job_node, 'teaser');
  $job_html = drupal_render($renderable);

  $worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);
  $renderable = node_view($worker_node, 'teaser');
  $worker_html = drupal_render($renderable);

  $employer_node = node_load($job_node->field_grievance_shop['und'][0]['target_id']);

  $log = $dispatch_node->field_sirius_log['und'][0]['value'];
  if ($log) {
    $log_html = sirius_log_render($log);
  }

  $conversation_node = sirius_twilio_conversation_get($dispatch_node->nid, NULL, FALSE);
  if ($conversation_node) {
    $log = $conversation_node->field_sirius_log['und'][0]['value'];
    if ($log) {
      $conversation_html = sirius_log_render($log);
    }
  }

  $html .= '<div class="sirius_dispatch_operator_dispatch_details">';
  /*
  $html .= '<h2>';
  $html .= $worker_node->field_sirius_name['und'][0]['given'] . ' ' . $worker_node->field_sirius_name['und'][0]['family'];
  if ($employer_node) {
    $html .= ' / ' . $employer_node->title;
  }
  $html .= '</h2>';
  */
  $html .= '[';
  $html .= l(
    'Call', 
    '/sirius/dispatch/operator/ajax/dispatch_call', 
    array(
      'attributes' => array(
        'class' => array('sirius_dispatch_operator_dispatch_call_link'),
        'data-dispatch-nid' => $dispatch_node->nid,
        'data-conversation-nid' => $conversation_node->nid,
        'data-dispatch-phone' => $conversation_node->field_sirius_phone['und'][0]['value'],
      ),
      'query' => array(
        'dispatch_nid' => $dispatch_node->nid,
      )
    )
  );
  $html .= '] ';

  $html .= '[';
  $html .= l(
    'Dismiss', 
    '/', 
    array(
      'attributes' => array(
        'class' => array('sirius_dispatch_operator_dispatch_dismiss_link'),
        'data-dispatch-nid' => $dispatch_node->nid,
        'data-conversation-nid' => $conversation_node->nid,
        'data-dispatch-phone' => $conversation_node->field_sirius_phone['und'][0]['value'],
      ),
      'query' => array(
        'dispatch_nid' => $dispatch_node->nid,
      )
    )
  );
  $html .= '] ';

  $status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
  if ($status == 'requested' || $status == 'notified' || $status == 'notification failed') {
    $html .= '[';
    $html .= l(
      'Notify SMS', 
      '/', 
      array(
        'attributes' => array(
          'class' => array('sirius_dispatch_operator_dispatch_notify_sms_link'),
          'data-dispatch-nid' => $dispatch_node->nid,
          'data-conversation-nid' => $conversation_node->nid,
          'data-dispatch-phone' => $conversation_node->field_sirius_phone['und'][0]['value'],
        ),
        'query' => array(
          'dispatch_nid' => $dispatch_node->nid,
        )
      )
    );
    $html .= '] ';

    $html .= '[';
    $html .= l(
      'Notify Voice', 
      '/', 
      array(
        'attributes' => array(
          'class' => array('sirius_dispatch_operator_dispatch_notify_voice_link'),
          'data-dispatch-nid' => $dispatch_node->nid,
          'data-conversation-nid' => $conversation_node->nid,
          'data-dispatch-phone' => $conversation_node->field_sirius_phone['und'][0]['value'],
        ),
        'query' => array(
          'dispatch_nid' => $dispatch_node->nid,
        )
      )
    );
    $html .= '] ';
  }


  $html .= '<br /><br />';

  $html .= '<div id="sirius_dispatch_operator_dispatch_tabs" style="font-size: 12px !important;">';
  $html .= '<ul>';
  $html .= '<li><a href="#sirius_dispatch_operator_dispatch_tabs_1">Dispatch</a></li>';
  if ($log_html) { $html .= '<li><a href="#sirius_dispatch_operator_dispatch_tabs_4">Log</a></li>'; }
  if ($conversation_html) { $html .= '<li><a href="#sirius_dispatch_operator_dispatch_tabs_5">Conversation</a></li>'; }
  $html .= '</ul>';

  $html .= '<div id="sirius_dispatch_operator_dispatch_tabs_1">' . 
    $worker_html . '[<a href="#">View Worker</a>]<hr> ' . $dispatch_html . '<hr>' . $job_html . '[<a href="#">View Job</a>]</div>';
  if ($log_html) { $html .= '<div id="sirius_dispatch_operator_dispatch_tabs_4">' . $log_html . '</div>'; }
  if ($conversation_html) { $html .= '<div id="sirius_dispatch_operator_dispatch_tabs_5">' . $conversation_html . '</div>'; }

  $html .= '</div>';

  $html .= '</div>';

  return $html;
}

function sirius_dispatch_page_operator_ajax_queue() {
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_twilio_conversation');
  $query->fieldCondition('field_sirius_twilio_conv_status', 'value', array('callback requested', 'forwarded to operator'), 'in');
  $query->propertyOrderBy('changed', 'desc');
  $stmt = $query->execute();

  $rows = array();
  $conversation_nids = array_keys($stmt['node']);
  $conversation_nodes = node_load_multiple($conversation_nids);

  foreach ($conversation_nodes as $conversation_nid => $conversation_node) {
    $rows[] = rename_this_function($conversation_node);
  }

  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_twilio_conversation');
  // $query->fieldCondition('field_sirius_twilio_conv_status', 'value', array('callback requested', 'forwarded to operator'), 'not in');
  $query->propertyOrderBy('changed', 'desc');
  $query->range(0, 10);
  $stmt = $query->execute();

  $conversation_nids = array_keys($stmt['node']);
  $conversation_nodes = node_load_multiple($conversation_nids);

  foreach ($conversation_nodes as $conversation_nid => $conversation_node) {
    if ($conversation_node->field_sirius_twilio_conv_status['und'][0]['value'] == 'callback requested') { continue; }
    if ($conversation_node->field_sirius_twilio_conv_status['und'][0]['value'] == 'forwarded to operator') { continue; }
    $rows[] = rename_this_function($conversation_node);
  }

  $header = array('Updated', 'Name', 'Status');
  return theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));

  return $html;
}

function rename_this_function($conversation_node) {
  $row = array();
  $dispatch_node = node_load($conversation_node->field_sirius_twilio_handler['und'][0]['target_id']);
  if (!$dispatch_node) { return array(); }
  $worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);
  $phone = $conversation_node->field_sirius_phone['und'][0]['value'];

  $row[] = format_interval(time() - $conversation_node->changed) . " ago";
  $row[] = l(
    $worker_node->field_sirius_name['und'][0]['given'] . ' ' . $worker_node->field_sirius_name['und'][0]['family'], 
    '/', 
    array(
      'attributes' => array(
        'class' => array('sirius_dispatch_operator_dispatch_details_link'),
        'id' => 'sirius_dispatch_queue_' . $conversation_node->nid,
        'data-conversation-nid' => $conversation_node->nid,
        'data-dispatch-nid' => $dispatch_node->nid,
        'data-dispatch-phone' => $conversation_node->field_sirius_phone['und'][0]['value'],
      ),
    )
  );

  $display_status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
  $conversation_status = $conversation_node->field_sirius_twilio_conv_status['und'][0]['value'];
  if ($conversation_status == 'callback requested' || $conversation_status == 'forwarded to operator') { 
    $display_status .= " / <strong>$conversation_status</strong>";
  }

  $row[] = $display_status;
  return array(
    'class' => array(
      'sirius_dispatch_queue_entry',
      'sirius_dispatch_queue_entry_' . $conversation_node->nid),
    'data' => $row
  );
}