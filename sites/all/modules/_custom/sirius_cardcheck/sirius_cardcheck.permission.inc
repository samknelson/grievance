<?php

/***
 * Implements hook_permission
 */

function sirius_cardcheck_permission() {
  return array(
    'sirius cardcheck admin' => array(
      'title' => t('Sirius Cardcheck Admin'), 
      'description' => t('Permission to manage card checks.'),
    ),
    'sirius cardcheck worker' => array(
      'title' => t('Sirius Cardcheck Worker'), 
      'description' => t('Permission to view and sign your own card checks.'),
    ),
  );
}


class Sirius_Cardcheck_Access extends Sirius_Access {
  public $type = 'cardcheck';
  public $name = 'Card Check';
  public $description = '';

  public function access($action, $context = array()) {
    global $user;
    $user_full = user_load($user->uid);
    $instance = Sirius_Cardcheck::getInstance();

    $actions = array(
      // No parameters
      'config',
      'staff', 
    );

    // Who am I?
    if (user_access('sirius admin')) { 
      $is_admin = $is_staff = $is_worker = TRUE;
    } else if (user_access('sirius cardcheck admin')) {
      $is_staff = $is_worker = TRUE; 
    } else if (user_access('sirius cardcheck worker')) { 
      $is_worker = TRUE;
    }

    // Admins can do pretty much anything.
    if ($is_admin) { 
      if (!in_array($action, array('create worker cardcheck', 'create offline cardcheck'))) { return TRUE; }
    }

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
      if ($context['definition_node']->field_sirius_type['und'][0]['value'] != 'sirius:cardcheck') { return FALSE; }
      if ($context['definition_node']->field_sirius_domain['und'][0]['target_id'] != sirius_domain_nid()) { return FALSE; }
    }

    // If we are passed in a worker node, we must have access to it.
    if (isset($context['cardcheck_node'])) {
      if (!$context['cardcheck_node']) { return FALSE; }
      if ($context['cardcheck_node']->type != 'sirius_log') {  return FALSE; }
      if ($context['cardcheck_node']->field_sirius_domain['und'][0]['target_id'] != sirius_domain_nid()) { return FALSE; }

      if (!$is_staff) {
        $my_worker_node = sirius_worker_get();
        $cardcheck_worker_node = sirius_log_handler($context['cardcheck_node'], 'sirius_worker');
        if (!$my_worker_node || !$cardcheck_worker_node) { return FALSE; }
        if ($my_worker_node->nid != $cardcheck_worker_node->nid) { return FALSE; }
      }
    }

    // Staff can do "staff" actions
    if ($action == 'staff') {
      if ($is_staff) { return TRUE; }
    }

    // Workers can list cardchecks
    if ($action == "list worker cardchecks") {
      if (!$context['worker_node']) { return FALSE; }
      if ($is_worker) { return TRUE; }
    }

    // Workers can view their own cardchecks
    if ($action == "view cardcheck") {
      if (!$context['cardcheck_node']) { return FALSE; }
      if ($is_worker) { return TRUE; }
    }

    // Staff can handle offline cardchcks
    if ($action == "create offline cardcheck") {
      // Must have a cardcheck node
      if (!$context['cardcheck_node']) { return FALSE; }

      // Must be unsigned
      if ($instance->status($context['cardcheck_node']) != 'unsigned') { return FALSE; }

      // Must be staff
      if ($is_staff) { return TRUE; }
    }

    // Workers can create their own cardchecks, if they don't have a signed or unsigned 
    // one already existing
    if ($action == "create worker cardcheck") {
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
