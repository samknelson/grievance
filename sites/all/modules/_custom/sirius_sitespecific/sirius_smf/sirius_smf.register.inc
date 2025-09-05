<?php

/**
 * Implements hook_user_insert
 */

function sirius_smf_hook_user_insert(&$edit, $account, $category) {
  sirius_domain_push($account->field_sirius_domain['und'][0]['target_id']);
  drupal_set_message("I was called.");
  sirius_domain_pop();
}

class Sirius_Smf_Register extends Sirius_Singleton {
  public function send_postal($account_uid) {
    if (!$account_uid) {
      return array('success' => FALSE, 'msg' => 'No user specified.');
    }

    $account = user_load($account_uid);

    if (!$account || !$account->uid) {
      return array('success' => FALSE, 'msg' => "User $account_uid could not be loaded.");
    }

    if (sirius_domain_variable_get('sirius_smf_register_letter_postal') != 'Yes') {
      return array('success' => FALSE, 'msg' => 'Postal registration letters are not enabled.');
    }

    $template_nid = sirius_domain_variable_get('sirius_smf_register_letter_postal_template_nid');
    if (!$template_nid) {
      return array('success' => FALSE, 'msg' => 'No postal template has been configured.');
    }

    $worker_node = sirius_worker_get($account->uid);
    if (!$worker_node) {
      return array('success' => FALSE, 'msg' => "User $account_uid is not associated with a worker.");
    }

    return array('success' => TRUE, 'msg' => "Letter sent.");
  }
}