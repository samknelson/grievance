<?php

class Sirius_Worker_Access extends Sirius_Access {
  public $type = 'worker';
  public $name = 'Worker';
  public $description = '';

  public function access($action, $context = array()) {
    global $user;
    $user_full = user_load($user->uid);

    $actions = array(
      // No parameters
      'config',
      'staff', 

      // optional employer_nid
      'create worker',

      // worker
      'view',
      'view status',
      'update addr',
      'update phone',
      'update email',
      'update password',
    );

    // Who am I?
    if (user_access('sirius admin')) { 
      $is_admin = $is_staff = TRUE;
    } else if (user_access('sirius worker admin')) {
      $is_staff = TRUE; 
    } else if (user_access('sirius dispatch employer') || 
        user_access('sirius employer user manager') || 
        user_access('sirius employer contact manager')) { 
      $is_employer = TRUE;
    } else if (user_access('sirius worker')) {
      $is_worker = TRUE;
    }

    // If we have an employer node in the context, it must be valid
    if (isset($context['employer_node'])) {
      if (!$context['employer_node']) { return FALSE; }
      if ($context['employer_node']->type != 'grievance_shop') {  return FALSE; }
      if ($context['employer_node']->field_sirius_domain['und'][0]['target_id'] != sirius_domain_nid()) { return FALSE; }

      if ($is_employer) {
        $my_employer_node = sirius_employer_get();
        if ($my_employer_node->nid != $context['employer_node']->nid) { return FALSE; }
      }
    }

    // Handle worker node context
    if (isset($context['worker_node'])) {
      if (!$context['worker_node']) { return FALSE; }
      if ($context['worker_node']->type != 'sirius_worker') {  return FALSE; }
      if ($context['worker_node']->field_sirius_domain['und'][0]['target_id'] != sirius_domain_nid()) { return FALSE; }

      if ($is_worker) {
        $my_worker_node = sirius_worker_get();
        if (!$my_worker_node) { return $this->deny("The user is not associated with a worker."); }
        if ($my_worker_node->nid != $context['worker_node']->nid) { return $this->deny("Not my worker."); }
      }
    }

    // Can only update the password if we are set up properly
    if ($action == 'update password') {
      // "user sync" must be enabled
      if (!sirius_domain_component_enabled('worker:usersync')) { return $this->deny("User sync is not enabled."); }

      // Must have a user
      $worker_user = sirius_worker_get_user($context['worker_node']->nid);
      if (!$worker_user) { return $this->deny("This worker is not associated with a user."); }

      $sql = "select module from authmap where uid = :uid";
      $stmt = db_query($sql, array(':uid' => $worker_user->uid));
      $hr = $stmt->fetchAssoc();
      if ($hr['module']) {
        return $this->deny("This worker is authenticated through $hr[module].");
      }
    }

    // Admins can do anything
    if ($is_admin) { return TRUE; }

    // Staff can do anything
    if ($is_staff) { return TRUE; }

    if ($action == 'staff') {
      if ($is_staff) { return TRUE; }
    }

    if ($action == 'create worker') {
      if (user_access('sirius worker create')) { return TRUE; }
    }

    if (in_array($action, array('view', 'update addr', 'update phone', 'update email', 'update password'))) {
      // Workers can update their own address
      if ($is_worker) { return TRUE; }
    }

    if (in_array($action, array('view status'))) {
      // Some workers can view their own status
      if ($is_worker && user_access('sirius worker view own status')) { return TRUE; }
    }

    // Oh well
    return $this->deny("No access granted for $action.");
  }
}
