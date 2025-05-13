<?php

class Sirius_Worker_Tos_Access extends Sirius_Access {
  public $type = 'worker_tos';
  public $name = 'Worker TOS';
  public $description = '';

  public function access($action, $context = array()) {
    global $user;

    $actions = array(
      // No parameters
      'staff',
      'config',
      'list',
      
    	// Parameter worker_node
      'worker_view',
      'worker_view_archive',

      // Parameter tos_node
      'edit',
    );

    // Check our TOS node
    if ($context['tos_node']) {
      if ($context['tos_node']->type != 'sirius_log') { return $this->deny("That is not a log node."); }
      if ($context['tos_node']->field_sirius_category['und'][0]['value'] != 'tos') { return $this->deny("That is not a TOS node."); }
    }

    // Who am I?
    if (user_access('sirius admin')) { $is_admin = $is_staff = TRUE; }
    if (user_access('sirius worker tos admin')) { $is_staff = TRUE; }
    if (user_access('sirius worker tos worker')) { $is_worker = TRUE; }
    if (user_access('sirius worker tos employer')) { $is_employer = TRUE; }

    // Admins can do anything
    if ($is_admin) { return TRUE; }

    // Workers are restricted to their own record
    if ($is_worker && $context['worker_node']) {
      $my_worker_node = sirius_worker_get();
      if (!$my_worker_node) { return $this->deny("User is not associated with a worker."); }
      if ($my_worker_node->nid != $context['worker_node']->nid) { return $this->deny("Not my worker record."); }
    }

    if (in_array($action, array('worker_view'))) {
      if ($is_staff) { return TRUE; }
      // Workers can view their own records
      if ($is_worker) { return TRUE; }
    }

    if (in_array($action, array('worker_view_archive'))) {
      if ($is_staff) { return TRUE; }
    }

    if (in_array($action, array('edit'))) {
      if ($is_staff) { return TRUE; }
    }

    if (in_array($action, array('list'))) {
      if ($is_staff) { return TRUE; }
    }

    // Only admin can edit
    if (in_array($action, array('edit'))) {
      // Need a TOS node
      if (!$context['tos_node']) { return $this->deny("No TOS node."); }

      // STaff can edit
      if ($is_staff) { return TRUE; }
    }

    // Oh well
    return $this->deny("Access denied");
  }
}
