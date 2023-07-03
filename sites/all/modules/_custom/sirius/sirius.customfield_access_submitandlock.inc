<?php

class Sirius_Customfield_Access_SubmitAndLock extends Sirius_Access {
	public $type = 'customfield_submitandlock';
	public $name = 'Customfield Submit and Lock';
	public $description = 'Customfields may be submitted by one role, after which they are "locked" and can only be seen or edited by other roles.';

  public function access($action, $context = array()) {
  	$submit_rids = $context['settings']['submit_rids'];
  	if (!$submit_rids) { $submit_rids = array(); }
  	$submit_rids = array_filter(array_values($submit_rids));

  	$admin_rids = $context['settings']['admin_rids'];
  	if (!$admin_rids) { $admin_rids = array(); }
  	$admin_rids = array_filter(array_values($admin_rids));

  	global $user;
  	$my_rids = array();
  	if ($user && $user->roles) { 
			$my_rids = array_filter(array_keys($user->roles));
		}
		if (!$my_rids) {
			return $this->deny("User is not logged in, or does not have any roles.", $action, $context);
		}

		// You can do anything if you have an admin role
		if (array_intersect($admin_rids, $my_rids)) { return TRUE; }

		// Otherwise, you must have one of the submit roles
		if (!array_intersect($submit_rids, $my_rids)) { return $this->deny("User does not have any of the submit or admin roles.", $action, $context); }

		// Submitters can list and add
		if (in_array($action, array('list', 'add', 'list_all'))) { return TRUE; }

		// For the rest of these entries, we need to know which node we're talking about
		$data_node = $context['data_node'];
		if (!$data_node) { return $this->deny("No data node.", $action, $context); }

		// I have no permission on any nodes except my own.
		if ($data_node->uid != $user->uid) { return $this->deny("User is not the author of this record.", $action, $context); }

		// I can view my own
		if ($action == 'view') { return TRUE; }

		// I can edit my own only if it's unlocked
		$status = sirius_customfield_status($context);
		if ($status) { return $this->deny("The record is locked."); }

		return TRUE;
  }

	public function settings_form() {
		$form['submit_rids'] = array(
			'#title' => t('Submit Roles'),
			'#type' => 'checkboxes',
			'#options' => user_roles(TRUE),
			'#checkall' => TRUE,
			'#description' => t('These roles have permission to create new entries; list their own entries; and view or edit their own entries (until they are submitted).'),
		);

		$form['admin_rids'] = array(
			'#title' => t('Admin Roles'),
			'#type' => 'checkboxes',
			'#options' => user_roles(TRUE),
			'#checkall' => TRUE,
			'#description' => t('These roles have permission to perform any action on entries, even after they has been submitted.'),
		);

		return $form;
	}

	public function status_options_all($context) {
		return array(
			NULL => t('Draft'),
			'locked' => t('Locked'),
		);
	}

	public function status_options($context) {
		// If you can edit, you can submit or un-submit
		if ($this->access('edit', $context)) { return $this->status_options_all($context); }

		// Otherwise, you can't change status at all
		return array();
	}
}

