<?php

/***
 * Implements hook_permission
 */

function sirius_sib_permission() {
  return array(
    'sirius sib admin' => array(
      'title' => t('Sirius Access Restriction Admin'), 
      'description' => t('Permission to configure access restrictions.'),
    ),
    'sirius sib assign' => array(
      'title' => t('Sirius Access Restriction Assign'), 
      'description' => t('Permission to assign or revoke access restrictions'),
    ),
  );
}


class Sirius_Sib_Access extends Sirius_Access {
  public $type = 'sib';
  public $name = 'Access Restrictions';
  public $description = '';

  public function access($action, $context = array()) {
    global $user;
    $user_full = user_load($user->uid);
    $instance = Sirius_Sib::getInstance();

    $actions = array(
      // No parameters
      'config',
      'staff', 

      // optional employer_nid
      'create worker',
    );

    // Who am I?
    if (user_access('sirius admin')) { 
      $is_admin = $is_staff = $is_worker = TRUE;
    } else if (user_access('sirius sib admin')) {
      $is_staff = $is_worker = TRUE; 
    } else if (user_access('sirius sib worker')) { 
      $is_worker = TRUE;
    }

    // Admins can do anything
    if ($is_admin) { return TRUE; }

    // If we are passed in a worker node, we must have access to it.
    if (isset($context['worker_node'])) {
      if (!$context['worker_node']) { return FALSE; }
      if ($context['worker_node']->type != 'sirius_worker') {  return FALSE; }
      if ($context['worker_node']->field_sirius_domain['und'][0]['target_id'] != sirius_domain_nid()) { return FALSE; }

      if (!$is_staff) {
        $my_worker_node = sirius_worker_get();
        if ($my_worker_node->nid != $context['worker_node']->nid) { return FALSE; }
      }
    }

    // If we are passed in a definition node, we must have access to it.
    if (isset($context['definition_node'])) {
      if (!$context['definition_node']) { return FALSE; }
      if ($context['definition_node']->type != 'sirius_json_definition') {  return FALSE; }
      if ($context['definition_node']->field_sirius_type['und'][0]['value'] != 'sirius:sib') { return FALSE; }
      if ($context['definition_node']->field_sirius_domain['und'][0]['target_id'] != sirius_domain_nid()) { return FALSE; }
    }

    // If we are passed in a worker node, we must have access to it.
    if (isset($context['sib_node'])) {
      if (!$context['sib_node']) { return FALSE; }
      if ($context['sib_node']->type != 'sirius_log') {  return FALSE; }
      if ($context['sib_node']->field_sirius_domain['und'][0]['target_id'] != sirius_domain_nid()) { return FALSE; }

      if (!$is_staff) {
        $my_worker_node = sirius_worker_get();
        $sib_worker_node = sirius_log_handler($context['sib_node'], 'sirius_worker');
        if (!$my_worker_node || !$sib_worker_node) { return FALSE; }
        if ($my_worker_node->nid != $sib_worker_node->nid) { return FALSE; }
      }
    }

    // Staff can do "staff" actions
    if ($action == 'staff') {
      if ($is_staff) { return TRUE; }
    }

    // Workers can list sibs
    if ($action == "list worker sibs") {
      if (!$context['worker_node']) { return FALSE; }
      if ($is_worker) { return TRUE; }
    }

    // Workers can view their own sibs
    if ($action == "view sib") {
      if (!$context['sib_node']) { return FALSE; }
      if ($is_worker) { return TRUE; }
    }

    // Workers can create their own sibs, if they don't have a signed or unsigned 
    // one already existing
    if ($action == "create worker sib") {
      if (!$context['worker_node']) { return FALSE; }
      if (!$context['definition_node']) { return FALSE; }

      $probe = $instance->search(
        array(
          'worker_nid' => $context['worker_node']->nid,
          'definition_nid' => $context['definition_node']->nid,
          'statuses' => array('signed', 'unsigned'),
          'nids_only' => TRUE,
        )
      );
      if ($probe) { return FALSE; }
      if ($is_worker) { return TRUE; }
    }

    return FALSE;
  }
}
