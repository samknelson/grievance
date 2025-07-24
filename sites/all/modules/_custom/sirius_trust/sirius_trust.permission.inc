<?php

/***
 * Implements hook_permission
 */

function sirius_trust_permission() {
  return array(
    'sirius trust admin' => array(
      'title' => t('Sirius Trust Admin'), 
      'description' => t('Administer all aspects of the Trust'),
    ),
    'sirius trust staff' => array(
      'title' => t('Sirius Trust Staff'), 
      'description' => t('Access to use the TrustFund component as a staff member'),
    ),
    'sirius trust employer' => array(
      'title' => t('Sirius Trust Employer'), 
      'description' => t('Access to use the TrustFund component as an employer'),
    ),
    'sirius trust worker' => array(
      'title' => t('Sirius Trust Worker'),
      'description' => t('Access the TrustFund component as a worker'),
    ),
  );
}

/**
 * Implements hook_node_access
 */

function sirius_trust_node_access($node, $op, $account) {
  // Get the node type
  if (is_object($node)) { $type = $node->type; } else { $type = $node; }

  if ($type == 'sirius_trust_benefit') {
    // Anybody can see the benefit
    if ($op == 'view') { return user_access('sirius'); }

    // Must be trust admin to do anything else to benefits
    if (!user_access('sirius trust admin')) { return NODE_ACCESS_DENY; }
  }

  if ($type == 'sirius_trust_worker_benefit') {
    // Must be trust staff to do anything at all to worker benefits
    if (!user_access('sirius trust staff')) { return NODE_ACCESS_DENY; }
  }

  // We know nothing.
  return NODE_ACCESS_IGNORE;
}


/**
 * Starting to move access controls into this framework, but it's incomplete...
 */

class Sirius_Trust_Access extends Sirius_Access {
  public $type = 'trust';
  public $name = 'Trust';
  public $description = '';

  public function action_component($action) {
    $action_component_map = array(
      'worker_beneficiary_edit' => 'trust:beneficiary',
    );
    return $action_component_map[$action];
  }

  public function check_component($action, $context = array()) {
    $component = $this->action_component($action);
    if ($component && !sirius_domain_component_enabled($component)) {
      return $this->deny("Component not enabled: $component");
    }
    return TRUE;
  }

  public function access($action, $context = array()) {
    global $user;

    if (!$this->check_component($action, $context)) { return FALSE; }

    $actions = array(
      // No parameters
      'admin',
      'staff',
      
      // Elections
      'worker_election_landing',
      'worker_election_list',
      'worker_election_wizard',
      'election_wizard_launch',
      'election_wizard_view',
      'election_wizard_edit',
      'election_wizard_post',

      'worker_benefit_landing',

      'worker_relationship_landing',

      // Beneficiaries
      'worker_beneficiary_edit'
    );

    // Who am I?
    if (user_access('sirius admin') || user_access('sirius trust admin')) { 
      $is_employer = $is_admin = $is_staff = TRUE;
    } else if (user_access('sirius trust admin')) {
      $is_employer = $is_staff = TRUE;
    } else if (user_access('sirius trust emloyer')) {
      $is_employer = TRUE;
    } else if (user_access('sirius trust worker')) {
      $is_worker = TRUE;
    }

    // Handle wizard node context
    if ($context['election_wizard_node']) {
      sirius_require('sirius_trust', 'sirius_trust.election_wizard.inc');

      if ($context['election_wizard_node']->type != 'sirius_log') { return $this->deny("System error: type not sirius_log."); }
      if ($context['election_wizard_node']->field_sirius_category['und'][0]['value'] != 'trust:election_wizard') { return $this->deny("System error: category not trust:election_wizard."); }

      if (!$context['worker_node']) { 
        $context['worker_node'] = sirius_trust_election_wizard_get_worker($context['election_wizard_node']);
      }
    }

    // Handle worker node context
    if ($context['worker_node']) {
      if ($context['worker_node']->type != 'sirius_worker') { return $this->deny("System error: type not sirius_worker."); }

      $my_worker_node = sirius_worker_get();
      if ($my_worker_node && ($context['worker_node']->nid == $my_worker_node->nid)) {
        $is_my_worker = TRUE;
      }
    }

    // 
    // Some actions even admins and staff cannot do.
    //

    // Cannot re-post an election wizard
    if (in_array($action, array('election_wizard_edit'))) {
      $status = sirius_trust_election_wizard_get_status($context['election_wizard_node']);
      if ($status == 'posted') { return $this->deny("Cannot re-post an election wizard"); }
    }

    // Admin can do anything else
    if ($is_admin) { return TRUE; }

    // Staff can do anything else
    if ($is_staff) { return TRUE; }

    // Workers can see their own election landing page
    if (in_array($action, array('worker_election_landing'))) {
      return ($is_worker && $is_my_worker);
    }

    // Workers can change their own elections. 
    if (in_array($action, array('worker_election_wizard', 'election_wizard_view'))) {
      return ($is_worker && $is_my_worker);
    }

    // Workers can edit the election wizard if it is in the status "draft".
    if (in_array($action, array('election_wizard_edit'))) {
      $status = sirius_trust_election_wizard_get_status($context['election_wizard_node']);
      if ($status == 'draft') { return ($is_worker && $is_my_worker);  }

      return $this->deny("The election wizard has status $status.");
    }

    // Workers can edit the their beneficiaries.
    if (in_array($action, array('worker_beneficiary_edit'))) {
      return ($is_worker && $is_my_worker);
    }

    // Workers can see their own benefit landing page
    if (in_array($action, array('worker_benefit_landing'))) {
      return ($is_worker && $is_my_worker);
    }

    // Workers can see their own relationships page
    if (in_array($action, array('worker_relationships_landing'))) {
      return ($is_worker && $is_my_worker);
    }

    // Oh well
    return $this->deny("Access to $action is restricted to staff.");
  }
}
