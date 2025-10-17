<?php

/***
 * Implements hook_permission
 */

function sirius_employer_permission() {
  return array(
    'sirius employer' => array(
      'title' => t('Sirius Employer'),
      'description' => t('Operate as an employer within Sirius'),
    ),

    'sirius employer worker list' => array(
      'title' => t('Sirius Employer Worker List'),
      'description' => t('List workers for your employer'),
    ),
    'sirius employer worker list headshot' => array(
      'title' => t('Sirius Employer Worker List Headshot'),
      'description' => t('Show headshots on the worker list'),
    ),
    'sirius employer worker list contact' => array(
      'title' => t('Sirius Employer Worker List Contact'),
      'description' => t('Show contact information on the worker list'),
    ),
    'sirius employer worker list ssn' => array(
      'title' => t('Sirius Employer Worker List SSN'),
      'description' => t('Show SSNs on the worker list'),
    ),

    'sirius employer worker manage' => array(
      'title' => t('Sirius Employer Worker Manage'),
      'description' => t('Create or edit workers for your employer'),
    ),



    'sirius employer user manager' => array(
      'title' => t('Sirius Employer User Manager'), 
      'description' => t('Create or disable other users in your own shop.'),
    ),
    'sirius employer contact manager' => array(
      'title' => t('Sirius Employer Contact Manager'), 
      'description' => t('Administer shop contacts.'),
    ),
    'sirius employer labor projection manager' => array(
      'title' => t('Sirius Employer Labor Projection Manager'), 
      'description' => t('View and edit labor projections in your own shop.'),
    ),
    'sirius employer admin' => array(
      'title' => t('Sirius Employer Admin'), 
      'description' => t('Administer employers.'),
    ),
  );
}

/**
 * Implements hook_node_access
 */

function sirius_employer_node_access($node, $op, $account) {
  // Get the node type
  if (is_object($node)) { $type = $node->type; } else { $type = $node; }

  // Specific checks by node type
  if ($type == 'sirius_bu') { return sirius_employer_node_access_bu($node, $op, $account); }

  // We know nothing.
  return NODE_ACCESS_IGNORE;
}

function sirius_employer_node_access_bu($node, $op, $account) {
  if ($op == 'view') { return user_access('sirius'); }
  return user_access('sirius employer admin');
}

/**
 * @todo: We are beginning the process of moving access control into the Access class.
 *        This process is JUST STARTING.
 *        So don't expect too much from this at the moment. In the mean time, access control is
 *        spread out over various locations.
 */

class Sirius_Employer_Access extends Sirius_Access {
  public $type = 'employer';
  public $name = 'Employer';
  public $description = '';

  public function access($action, $context = array()) {
    global $user;

    $actions = array(
      'worker_list',
    );

    // Who am I?
    if (user_access('sirius admin') || user_access('sirius employer admin')) {
      $is_admin = TRUE;
    } 

    // Admins can do anything
    if ($is_admin) { return TRUE; }

    // Must have an employer
    if (!$context['employer_node']) { return $this->deny("No employer node."); }

    // Must be one of the employers we have access to. For now we only recognize your main employer
    $my_employer = sirius_employer_get();
    if (!$my_employer) { return $this->deny('No employer set for this user.'); }
    if ($my_employer->nid != $context['employer_node']->nid) { return $this->deny("Access denied to this employer."); }

    // Anybody can access the worker list
    if (in_array($action, array('worker_list'))) {
      return user_access('sirius employer worker list');
    }

    // Need permission to see the phone, email, etc.
    if (in_array($action, array('worker_list_headshot'))) {
      return user_access('sirius employer worker list headshot');
    }
    if (in_array($action, array('worker_list_contact'))) {
      return user_access('sirius employer worker list contact');
    }
    if (in_array($action, array('worker_list_ssn'))) {
      return user_access('sirius employer worker list ssn');
    }
    if (in_array($action, array('worker_manage'))) {
      return user_access('sirius employer worker manage');
    }

    return FALSE;
  }
}
