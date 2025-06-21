<?php

/***
 * Implements hook_permission
 */

function sirius_ledger_permission() {
  return array(
    'sirius ledger admin' => array(
      'title' => t('Sirius Ledger Admin'), 
      'description' => t('Permission to manage all aspects of the ledger system.'),
    ),
    'sirius ledger employer' => array(
      'title' => t('Sirius Ledger Employer'), 
      'description' => t('Permission to access the A/R system as an employer.'),
    ),
    'sirius ledger employer paymentmethods' => array(
      'title' => 'Sirius Ledger Employer Paymentmethods',
      'description' => 'Permission to manage or charge payment method for an employer',
    ),
    'sirius ledger worker' => array(
      'title' => t('Sirius Ledger Worker'), 
      'description' => t('Permission to access the A/R system as a worker.'),
    ),
    'sirius ledger worker paymentmethods' => array(
      'title' => 'Sirius Ledger Worker Paymentmethods',
      'description' => 'Permission to manage or charge payment method for a worker',
    ),
  );
}

/**
 * Implements hook_node_access
 */

function sirius_ledger_node_access($node, $op, $account) {
  // Get the node type
  if (is_object($node)) { $type = $node->type; } else { $type = $node; }

  // Specific checks by node type
	if ($node->type == 'sirius_payment') { return sirius_ledger_node_access_payment($node, $op, $account); }

  // We know nothing.
  return NODE_ACCESS_IGNORE;
}

function sirius_ledger_node_access_payment($node, $op, $account) {
  // Sirius Admin can do anything
  if (user_access('sirius admin')) { return NODE_ACCESS_ALLOW; }

  // The only operation that can be performed directly on these nodes is "view".
  if ($op != 'view') { return NODE_ACCESS_DENY; }

  // Must have "sirius ledger" permission
  if (!user_access('sirius ledger admin')) { return NODE_ACCESS_DENY; }

  // Ok
  return NODE_ACCESS_IGNORE;
}

class Sirius_Ledger_Access extends Sirius_Access {
  public $type = 'ledger';
  public $name = 'Ledger';
  public $description = '';

  public function access($action, $context = array()) {
    global $user;
    $ea = Sirius_Ledger_EA::getInstance();

    $actions = array(
      // No parameters
      'admin',
      'staff',
      
      // Parameter entity_node
      'entity_view',

      // Parameter entity_node, account_node
      'ea_view',
      'ea_invoices',
      'ea_payments',
      'ea_payment_create',
      'ea_view_employer_workers',

      'paymentmethods_view',
      'paymentmethods_edit',
      'paymentmethods_charge',

      // Parameter payment_node
      'payment_view',
    );

    // Check node type
    if ($context['ea_node']) {
      if ($context['ea_node']->type != 'sirius_log') { return $this->deny("System error: type not sirius_log."); }
      if ($context['ea_node']->field_sirius_category['und'][0]['value'] != 'ledger:ea') { return $this->deny("Log entry is not of type ledger:ea."); }
    }

    // Who am I?
    if (user_access('sirius admin') || user_access('sirius ledger admin')) { 
      $is_admin = $is_staff = TRUE;
    } else if (user_access('sirius ledger admin')) {
      $is_staff = TRUE;
    }

    // If we have an EA node, then set our entity and account
    if ($context['ea_node']) {
      if (!$context['entity_node']) { $context['entity_node'] = $ea->entity_node($context['ea_node']); }
      if (!$context['account_node']) { $context['account_node'] = $ea->account_node($context['ea_node']); }

      if (!$context['entity_node']) { return $this->deny("No entity node."); }
      if (!$context['account_node']) { return $this->deny("No account node."); }
    }

    // EA Invoices are restricted by setting
    if ($action == 'ea_invoices') {
      $account_json = sirius_json_get($context['account_node']);
      if ($account_json['settings']['ea']['invoices'] != 'Yes') { return FALSE; }
    }

    $account_json = sirius_json_get($context['account_node']);
    $participant_type = $account_json['settings']['ea']['participant'];

    // "ea_view" is restricted by type (i.e. workers or employers)
    if (in_array($action, array('ea_view', 'ea_invoices'))) {
      if ( ($context['entity_node']->type == 'sirius_worker') && ($participant_type != 'worker')) {
        return $this->deny("Account participant must be 'worker'.");
      }

      if ( ($context['entity_node']->type == 'grievance_shop') && ($participant_type != 'employer')) {
        return $this->deny("Account participant must be 'employer'.");
      }
    }

    // "ea_view_employer_workers" are specially restricted
    if ($action == 'ea_view_employer_workers') {
      // Must by viewed by an employer ...
      if ($context['entity_node']->type != 'grievance_shop') { return $this->deny("Entity node is not an empoyer."); }

      // But the "participant type" must be "worker"
      if ($participant_type != 'worker') { return $this->deny("Account participant must be 'worker'."); }
    }

    // Admins can do anything
    if ($is_admin) { return TRUE; }

    // Staff can do pretty much anything
    if ($action == 'staff') { return $is_staff; }


    // I may be restricted by "entity". For exmaple, if I am an employer,
    // I might be restricted to only see A/R entries or features within the context of
    // my specific employer.
    $is_my_entity = FALSE;
    if (user_access('sirius ledger employer')) {
      $my_employer_node = sirius_employer_get();
      if ($context['entity_node'] && 
        $context['entity_node']->type == 'grievance_shop' && 
        $context['entity_node']->nid == $my_employer_node->nid) {
        $is_my_entity = TRUE;
      }
    }

    if (user_access('sirius ledger worker')) {
      $my_worker_node = sirius_worker_get();
      if ($context['entity_node'] && 
        $context['entity_node']->type == 'sirius_worker' && 
        $context['entity_node']->nid == $my_worker_node->nid) {
        $is_my_entity = TRUE;
      }
    }

    // If we have an entity node, it must be MY entity node
    if ($context['entity_node'] && !$is_my_entity) {
      return FALSE;
    }

    // View entity page
    if ($action == 'entity_view') {
      return $is_my_entity;
    }

    // View or edit payment methods
    if (in_array($action, array('paymentmethods_view', 'paymentmethods_edit', 'paymentmethods_charge'))) {
      if (!$context['entity_node']) { return FALSE; }

      if ($context['entity_node']->type == 'sirius_worker') {
        return user_access('sirius ledger worker paymentmethods');
      }

      if ($context['entity_node']->type == 'grievance_shop') {
        return user_access('sirius ledger employer paymentmethods');
      }
    }

    // View ea page
    if (in_array($action, array('ea_view', 'ea_invoices', 'ea_view_employer_workers', 'ea_payments'))) {
      // Must have an account node
      if (!$context['account_node']) { return FALSE; }

      // Make sure that we have one of the roles that can access this account
      if (!$user->roles) { return FALSE; }
      $account_json = sirius_json_get($context['account_node']);
      if (!$account_json['settings']['ea']['access']['view']['roles']) { return FALSE; }
      if (!array_intersect(array_keys($user->roles), array_filter(array_values($account_json['settings']['ea']['access']['view']['roles'])))) {
        return FALSE;
      }

      return TRUE;
    }

    // Oh well
    return FALSE;
  }
}
