<?php

/***
 * Implements hook_permission
 */

function sirius_twilio_permission() {
  return array(
    'sirius twilio admin' => array(
      'title' => t('Sirius Twilio Admin'), 
      'description' => t('Configure Twilio access.'),
    ),
  );
}


function sirius_twilio_node_access($node, $op, $account) {
  if ($node->type != 'sirius_twilio_sms_log') { return NODE_ACCESS_IGNORE; }
  if (user_access('sirius twilio admin', $account)) { return NODE_ACCESS_ALLOW; }
  return NODE_ACCESS_DENY;
}
