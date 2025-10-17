<?php

class Sirius_Employer_Monthly_Upload_Access extends Sirius_Access {
  public function is_admin() { return user_access('sirius admin'); }

  /**
   * Should be overridden
   */

  public function is_staff() { return NULL; }

  /**
   * Should be overridden
   */
  
  public function is_employer() { return NULL; }

  public function access($action, $context = array()) {
    global $user;

    $actions = array(
      // No parameters
      'admin',
      'staff',
      
      // Employer
      'employer_landing',

      // EUP
      'eup_view',
    );

    // Who am I?
    if ($this->is_admin()) {
      $is_employer = $is_admin = $is_staff = TRUE;
    } else if ($this->is_staff()) {
      $is_employer = $is_staff = TRUE;
    } else if ($this->is_employer()) {
      $is_employer = TRUE;
    } else if (user_access('sirius trust worker')) {
      $is_worker = TRUE;
    }

    // Admin can do anything
    if ($is_admin) { return TRUE; }

    // Staff can do anything
    if ($is_staff) { return TRUE; }

    if ($context['eup_node']) {
      if (!$context['employer_node']) {
        $context['employer_node'] = sirius_log_handler($context['eup_node'], 'grievance_shop');
      }
    }

    // Is this "my" employer?
    $is_my_employer = FALSE;
    if ($is_employer) {
      $my_employer_node = sirius_employer_get();
      if ($context['employer_node'] && 
        $context['employer_node']->type == 'grievance_shop' && 
        $context['employer_node']->nid == $my_employer_node->nid) {
        $is_my_employer = TRUE;
      }
    }

    // Employers can view their own landing page
    if (in_array($action, array('employer_landing'))) {
      return ($is_employer && $is_my_employer);
    }

    // Employers can view their own EUP nodes
    if (in_array($action, array('eup_view'))) {
      return ($is_employer && $is_my_employer);
    }

    return $this->deny("Unknown action: $action");
  }
}