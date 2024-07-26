<?php


function sirius_access_info_all() {
  $infos = &drupal_static(__FUNCTION__);
  if (!isset($infos)) {
    $infos = module_invoke_all('sirius_access_info');
    ksort($infos);
  }
  return $infos;
}

function sirius_access_info($type) {
  $infos = &drupal_static(__FUNCTION__); 
  if (!isset($infos)) { $infos = sirius_access_info_all(); }
  return $infos[$type];
}

function sirius_access_instance($type) {
  $info = sirius_access_info($type);
  return $info['instance'];
}

/** 
 * Implements hook_sirius_access_info
 */

function sirius_sirius_access_info() {
	sirius_require('sirius', 'sirius.customfield_access_submitandlock.inc');

  $infos = array();
  Sirius_Access_Grantall::getInstance()->setInfo($infos);
  Sirius_Access_Denyall::getInstance()->setInfo($infos);
  Sirius_Access_Permission_Single::getInstance()->setInfo($infos);
	Sirius_Access_Permission_Roles::getInstance()->setInfo($infos);
	Sirius_Customfield_Access_SubmitAndLock::getInstance()->setInfo($infos);
  return $infos;
}

class Sirius_Access extends Sirius_Singleton {
	public $error_msg = '';
  /**
   * Does a user have permission on an action?
   * 
   * Return TRUE if access is granted, FALSE if it is denied.
   * If access is denied, the reason for denial is sent to minilog().
   */

  public function access($action, $context = array()) { 
  	return TRUE;
  }

  public function settings_form() { return array(); }

  public function deny($msg, $action = NULL, $context = NULL) {
  	$tmp = '';
  	if ($action) { $tmp = "Access denied - $action: "; }
  	$tmp .= $msg;
  	$this->error_msg = $msg;
  	sirius_minilog("Access denied - $action: $msg", 'access');
  	return FALSE; 
  }

  public function error_msg() { return $this->error_msg; }
}

class Sirius_Access_Grantall extends Sirius_Access {
	public $type = 'grantall';
	public $name = 'Grant All';
	public $description = 'Grant access always.';
  public function access($action, $context = array()) { return TRUE; }
}

class Sirius_Access_Denyall extends Sirius_Access {
	public $type = 'denyall';
	public $name = 'Deny All';
	public $description = 'Deny access always.';
  public function access($action, $context = array()) { return $this->deny("Access is always denied.", $action, $context); }
}

class Sirius_Access_Permission_Single extends Sirius_Access {
	public $type = 'permission';
	public $name = 'Permission (Single)';
	public $description = 'Access is controlled by a single permission.';

  public function access($action, $context = array()) {
  	if (!$context['settings']['permission']) { return $this->deny("Permission check called with no permission.", $action, $context); }

  	if (user_access($context['settings']['permission'])) { return TRUE; }
  	return $this->deny('User does not have permission [' . $context['settings']['permission'] . '].', $action, $context); 
  }

	public function settings_form() {
		$all_permissions = user_permission_get_modules();
		$options = array();
		foreach ($all_permissions as $permission => $module) {
			$options[$permission] = "$module: $permission";
		}
		asort($options);

		$form['permission'] = array(
			'#title' => 'Permission',
			'#type' => 'select',
			'#options' => array(NULL => t('-- Select --')) + $options,
		);

		return $form;
	}
}

class Sirius_Access_Permission_Roles extends Sirius_Access {
	public $type = 'roles';
	public $name = 'Role (Multiple)';
	public $description = 'User must have one or more of the specified roles.';

  public function access($action, $context = array()) {
  	$rids = $context['settings']['rids'];
  	if (!$rids) { $rids = array(); }
  	$rids = array_filter(array_values($rids));

  	if (!$rids) { return $this->deny('Roles check called with no enabled roles.', $action, $context); }

  	global $user;
  	if (!$user || !$user->roles) { return $this->deny('User is not logged in, or does not have any roles.', $action, $context); }

		$my_rids = array_filter(array_keys($user->roles));
		if (!array_intersect($rids, $my_rids)) { return $this->deny('The user does not have one of the required roles.', $action, $context); }

		return TRUE;
  }

	public function settings_form() {
		$form['rids'] = array(
			'#title' => 'Roles',
			'#type' => 'checkboxes',
			'#options' => user_roles(TRUE),
			'#checkall' => TRUE,
		);

		return $form;
	}
}

