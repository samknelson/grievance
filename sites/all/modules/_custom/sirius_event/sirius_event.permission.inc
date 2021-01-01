<?php

/***
 * Implements hook_permission
 */

function sirius_event_permission() {
  return array(
    'sirius event admin' => array(
      'title' => t('Sirius Event Admin'), 
      'description' => t('Administer Events.'),
    ),
  );
}

function sirius_event_node_access($node, $op, $account) {
	if ($node->type == 'sirius_event') { return sirius_event_node_access_event($node, $op, $account); }
  if ($node->type == 'sirius_event_participant') { return sirius_event_node_access_participant($node, $op, $account); }

	return NODE_ACCESS_IGNORE;
}

function sirius_event_node_access_event($node, $op, $account) {
  if (user_access('sirius event admin', $account)) { return NODE_ACCESS_IGNORE; }
  return NODE_ACCESS_DENY;
}

function sirius_event_node_access_participant($node, $op, $account) {
  if (user_access('sirius event admin', $account)) { return NODE_ACCESS_IGNORE; }
  return NODE_ACCESS_DENY;
}



/**
 * We will often need to determine permissions to perform actions based on an event type, a participant role and a participant status.
 * For example, for an event of type "class", we might decide the event can be edited by
 * - Anybody with the user role "Class Admin", or
 * - Anybody who is currently registered with the event with the participant role "Teacher" and the status "Active".
 */

function sirius_event_paccess_rule_match($event_nid, $rules, $multiple = FALSE) {
  global $user;
  $my_uid = $user->uid;
  $participant_nid = sirius_event_participant_nid_from_user($event_nid);
  $participant_node = node_load($participant_nid);
  $my_prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
  $my_pstatus_tid = $participant_node->field_sirius_event_pstatus['und'][0]['tid'];
  $my_rids = array_keys($user->roles);

  $matches = array();
  if ($rules) {
    $delta = -1;
    foreach ($rules as $delta => $rule) {
      $rule['delta'] = $delta;
      if (!$rule['paccess']) { continue; }
      foreach ($rule['paccess'] as $paccess) {
        list ($prole_tid, $rest) = explode('|', $paccess);

        $found = FALSE;
        if ($prole_tid == 'GLOBAL') {
          $rid = $rest;
          $found = in_array($rid, $my_rids);
        } else {
          $pstatus_tid = $rest;
          $found = ( ($prole_tid == $my_prole_tid) && ($pstatus_tid == $my_pstatus_tid) );
        }

        if ($found) { 
          if ($multiple) { 
            $matches[] = $rule;
            break;
          } else {
            return $rule;
          }
        }
      }
    }
  }

  return $matches;
}

function sirius_event_paccess_options($event_type_term, $settings = array()) {
  $prole_tids = sirius_fieldvals($event_type_term, 'field_sirius_event_proles', 'tid');
  $prole_terms = taxonomy_term_load_multiple($prole_tids);

  $paccess_options = array();

  if ($settings['null']) {
    $paccess_options[NULL] = $settings['null'];
  }

  if (!$settings['exclude_participants']) {
    foreach ($prole_terms as $prole_tid => $prole_term) {
      $pstatus_terms = sirius_event_participant_role_status_tids($prole_tid);
      foreach ($pstatus_terms as $pstatus_tid => $pstatus_name) {
        $paccess_options["$prole_tid|$pstatus_tid"] = $prole_term->name . ': ' . $pstatus_name;
      }
    }
  }

  if (!$settings['exclude_non_participants']) {
    $user_roles = user_roles();
    foreach ($user_roles as $rid => $role_name) {
      $paccess_options["GLOBAL|$rid"] = t('Non-participant: @role', array('@role' => $role_name));
    }
  }
  return $paccess_options;
}

function sirius_event_rule_widget_exists($rules, $widget) {
  foreach ($rules as $delta => $rule) {
    if ($rule['widget'][$widget]) { return TRUE; }
  }
  return FALSE;
}
