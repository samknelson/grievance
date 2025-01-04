<?php

function sirius_service_server_active() {
  return (sirius_domain_variable_get('sirius_service_server_active') == 'Yes');
}

/**
 * Implements hook_services_authentication().
 */

function sirius_service_services_authentication_info() {
  return array(
    'title'            => t('Sirius authentication'),
    'description'      => t('Username and password are sent in each request; domain access is verified'),
    'authenticate_call' => 'sirius_service_server_authenticate',
  );
}

/**
 * Implements hook_rest_server_execute_errors_alter().
 *
 * HTTP Basic Authentication must add a "WWW-Authenticate" header on
 * unauthorized requests. Some clients (i.e. lynx) will not work properly
 * without this.
 *
 * This hook was introduced in the services module version 3.3 and will have no
 * effect on versions prior to this.
 */
function sirius_service_rest_server_execute_errors_alter(&$error, $controller, $arguments) {
  if ($error['code'] == 401) {
    drupal_add_http_header('WWW-Authenticate', 'Basic realm="' . check_plain(variable_get('site_name'), 'Drupal') . '"');
  }
  elseif ($error['code'] == 403 && user_is_anonymous()) {
    // The user has not yet authenticated, so we should return a 401 status
    // instead of the default 403 status code.
    // @see https://www.drupal.org/node/2158563
    $error['code'] = 401;
    $error['header_message'] = '401 Unauthorized';
    drupal_add_http_header('WWW-Authenticate', 'Basic realm="' . check_plain(variable_get('site_name'), 'Drupal') . '"');
  }
}

/**
 * Authenticates a call using HTTP basic authentication to verify the request.
 * 
 * Copied from service_basic_auth, modified to support domains
 *
 * @param array $settings
 *   The settings for the authentication module.
 * @param array $method
 *   The method that's being called.
 * @param array $args
 *   The arguments that are being used to call the method.
 *
 * @return void|string
 *   Returns nothing, or a error message if authentication fails.
 */

function sirius_service_server_authenticate($settings, $method, $args) {
  // Disable page caching for any request that requires an authentication
  // check.
  drupal_page_is_cacheable(FALSE);

  // PHP FastCGI doesn't support HTTP Basic Authentication out of the box so we
  // need this workaround. Requires a patch to .htaccess.
  // @see http://drupal.org/node/1864628.
  $fastcgi_header = variable_get('services_basic_auth_fastcgi_header', 'HTTP_AUTHORIZATION');
  if (isset($_SERVER[$fastcgi_header])
      && !empty($_SERVER[$fastcgi_header])
      && strtolower(substr($_SERVER[$fastcgi_header], 0, 5)) == 'basic') {

    $credentials = base64_decode(substr($_SERVER[$fastcgi_header], 6));
    if (empty($credentials)) {
      // The string is too short or is not correctly Base64 encoded.
      return 'Malformed HTTP Authorization header.';
    }
    list($account_id, $access_token) = explode(':', $credentials, 2);
  }
  $account_id = trim($account_id);
  $access_token = trim($access_token);

  // Which domain? (Account ID is just a UUID.)
  if (!$account_id) { return "No account id."; }
  if (!$access_token) { return "No access token."; }

	$nids = entity_get_id_by_uuid('node', array($account_id), FALSE);
  if ($nids) {
  	$nids = array_values($nids);
  	$domain_nid = $nids[0];
  	if (!$domain_nid) { return "No node found with that account ID."; }
  } else {
    // Legacy support: if UUID is for user 1, then we can connect without a domain
    $uids = entity_get_id_by_uuid('user', array($account_id), FALSE);
    if (!$uids) { return "No node found with that account ID."; }
    $uids = array_values($uids);
    $uid = $uids[0];
    if ($uid != 1) { return "No node found with that account ID, and it's not the UUID for user #1 ($uid)."; }
    $domain_nid = 0;
  }

  if ($domain_nid) {
  	$domain_node = node_load($domain_nid);
  	if (!$domain_node) { return "No node found with that domain NID."; }
  	if ($domain_node->type != 'sirius_domain') { return "The specified node is not a domain."; }
  }

	// Check domain components
	sirius_domain($domain_nid);
  if (!sirius_domain_component_enabled('service')) { return "The 'Service' component is not enabled for this domain."; }
  if (!sirius_domain_component_enabled('service:server')) { return "The 'Service - Server' component is not enabled for this domain."; }

  // Check that the server is turned on
  if (!sirius_service_server_active()) { return 'The server has not been set to "Active".'; }

  // Check IP allowlist
  if (sirius_domain_variable_get('sirius_service_server_ips_restrict') != 'No') {
  	$ip_range = sirius_domain_variable_get('sirius_service_server_ips');
  	if (!$ip_range) { return 'No IP range has been configured for this domain. All access is denied.'; }
  	$remote_ip = sirius_loopback_remote_ip();
  	if (!$remote_ip) { return 'Unable to determine the request IP.'; }
  	if (!sirius_ip_check($remote_ip, $ip_range)) { return 'Your IP, ' . $remote_ip . ', does not have access to this server.'; }
  }

	// Check access token
  $valid_access_token = sirius_domain_variable_get('sirius_service_server_access');
  if (!$valid_access_token) { return "No access token has been set up for this domain."; }
  if ($valid_access_token != $access_token) { return "Invalid access token."; }

  // Switch user
  $user_name = sirius_domain_variable_get('sirius_service_user_name');
  if ($user_name) {
    global $user;
    $user = user_load_by_name($user_name);
    if (!$user) {
      return "Unable to switch to user $user_name.";
    }
  }
}

/**
 * Implements hook_services_resources()
 */

function sirius_service_services_resources() {
  $resource = array(
    'sirius' => array(
      'actions' => array(
        'generic' => array(
          'callback' => 'sirius_service_generic_action',
          'args' => array(array('name' => 'args', 'optional' => TRUE, 'source' => 'data', 'type' => 'array')),
          'access callback' => 'sirius_service_access',
          'access arguments' => array(),
          'access arguments append' => TRUE,
        ),
      )
    )
  );

  return $resource;
}

function sirius_service_access($args = NULL) {
  return TRUE;
}

function sirius_service_ping($arg_0, $arg_1, $arg_2) {
  return array(
    'ts' => time(),
  	'msg' => 'Ping succeeded',
    'arg_0' => $arg_0,
    'arg_1' => $arg_1,
    'arg_2' => $arg_2,
  );
}

function sirius_service_func_info_all() {
  $functions = array(
    'taxonomy_get_vocabularies',
    'sirius_service_ping',
    'sirius_proxy_node_load',
    'sirius_taxonomy_load',
    'sirius_contact_relationships',
    'sirius_employer_list',
    'sirius_worker_get_by_ssn',
    'sirius_trust_worker_election',
    'sirius_trust_benefits',
    'sirius_trust_policy_benefits_offered',
    'sirius_trust_benefit_options',
    'sirius_ledger_policy_list',
    'sirius_ledger_policy_lookup',
    'sirius_esignature_defs',
    'sirius_dashboard_item_data',
    'sirius_teamsters631_switch_authenticate',
    'sirius_teamsters631_sirius_node_tab_worker_contact_form_submit_remote',
    'sirius_teamsters631_sirius_worker_skill_set_remote',
    'sirius_teamsters631_sirius_worker_member_status_set_remote',
    'sirius_teamsters631_node_insert_worker_remote',
    'sirius_teamsters631_sirius_worker_work_status_set_remote',
    'sirius_teamsters631_taxonomy_term_updateinsert_remote',
    'sirius_teamsters631_sirius_training_level_graduate_remote',
    'sirius_worker_nextid',
    'sirius_service_server_call',
  );

  $functions_nocache = array(
    'sirius_teamsters631_switch_authenticate',
    'sirius_teamsters631_sirius_node_tab_worker_contact_form_submit_remote',
    'sirius_teamsters631_sirius_worker_skill_set_remote',
    'sirius_teamsters631_sirius_worker_member_status_set_remote',
    'sirius_teamsters631_node_insert_worker_remote',
    'sirius_teamsters631_sirius_worker_work_status_set_remote',
    'sirius_teamsters631_taxonomy_term_updateinsert_remote',
    'sirius_teamsters631_sirius_training_level_graduate_remote',
    'sirius_worker_nextid',
    'sirius_service_ping',
  );

  $info = array();
  foreach ($functions as $function_name) {
    $info[$function_name] = array();
    $info[$function_name]['callback'] = $function_name;

    if (!in_array($function_name, $functions_nocache)) {
      $info[$function_name]['cache'] = TRUE;
    }
  }

  $info['sirius_service_ping']['description'] = 'Respond to a "ping" request; used to test that the server is responding correctly.';

  // Default enable all functions. 
  $settings = sirius_domain_variable_get('sirius_service_server_funcs', array());
  foreach ($functions as $function_name) {
    if ($settings[$function_name]['enabled'] == 'No') {
      $info[$function_name]['enabled'] = 'No';
    } else {
      $info[$function_name]['enabled'] = 'Yes';
    }
  }

  drupal_alter('sirius_service_func_info_all', $info);

  return $info;
}

function sirius_service_func_info($func) {
  $info_all = &drupal_static(__FUNCTION__);
  if (!isset($info_all)) { $info_all = sirius_service_func_info_all(); }
  return $info_all[$func];
}

function sirius_service_is_remote($value = NULL) {
  $is_remote = &drupal_static(__FUNCTION__);
  if (isset($value)) { $is_remote = $value; }
  return $is_remote;
}

function sirius_service_generic_action($args) {
  sirius_service_is_remote(TRUE);

  // Accept either
  //    {"1":"sirius_service_ping","2":"Echo Text"}
  // or
  //    ["sirius_service_ping","Echo Text"]
  if (is_array($args)) { $args = array_values($args); }

  // The first argument is a function name
  $key = array_shift($args);
  if (!$key) { return array('success' => FALSE, 'msg' => "The first argument to this service must be a service-enabled function key."); }

  // We don't let the server run all functions, just the ones which are enabled
  $info = sirius_service_func_info($key);
  if (!$info) { return array('success' => FALSE, 'msg' => "$key is not a valid service-enabled function."); }
  if ($info['enabled'] != 'Yes')  { return array('success' => FALSE, 'msg' => "Access to $key has been disabled by the server administrator."); }

  // Prepare our result payload
  $result = array();
  $result['success'] = TRUE;
  $result['ts'] = time();
  $result['is_remote'] = TRUE;

  // Prepare the callback
  $args = _sirius_service_array_to_node($args);
  $callback = $info['callback'];
  if (!$callback) { return array('success' => FALSE, 'msg' => "$key is not associated with a valid callback function."); }
  if (!function_exists($callback)) { return array('success' => FALSE, 'msg' => "The function $callback does not exist."); }
  $result['data'] = $callback($args[0], $args[1], $args[2], $args[3], $args[4], $args[5], $args[6], $args[7], $args[8]);

  // If the function logged debugging information, include the minilog
  $result['minilog'] = sirius_minilog_render_html();

  // If the function generated any drupal messages, let's send them to the client and *not* show them on the server
  $result['drupal_messages'] = drupal_get_messages();

  return $result;
}

/////////////////////////////////////////////////////
// New class stuff. This is only half-integrated. 
/////////////////////////////////////////////////////
//
// @todo The server_call() function is the new way of doing things. But it's not fully integrated yet.
//       At the moment, all calls route through generic_action(), which chains to server_call().
//       Someday all calls should route through server_call(), instantiating the correct Server object.
//       But not today.

function sirius_service_server_call($request_serialized) {
  $request = unserialize($request_serialized);
  $server_type = $request->server_type();

  $instance = sirius_service_server_instance($server_type);
  if (!$instance) {
    $result = new Sirius_Server_Response(array('success' => FALSE, 'msg' => "Server type $server_type not found."));
  } else {
    $result = $instance->respond($request); 
  }

  return serialize($result);
}

class Sirius_Server_Request {
  public function __construct($arr = array()) {
    foreach ($arr as $key => $value) { $this->{$key} = $value; }
  }
  
  public function server_type() {
    return $this->server_type;
  }
}

class Sirius_Server_Response {
  public function __construct($arr = array()) {
    foreach ($arr as $key => $value) { $this->{$key} = $value; }
  }

  public function success() {
    return $this->success;
  }

  public function msg() {
    return $this->msg;
  }
}









function sirius_service_server_info_all() {
  $infos = &drupal_static(__FUNCTION__);
  if (!isset($infos)) {
    $infos = module_invoke_all('sirius_service_server_info');
    ksort($infos);
  }
  return $infos;
}

function sirius_service_server_info($type = NULL) {
  $infos = &drupal_static(__FUNCTION__); 
  if (!isset($infos)) { $infos = sirius_service_server_info_all(); }
  return $infos[$type];
}

function sirius_service_server_instance($type = NULL) {
  if (!$type) { $type = sirius_service_server_type(); }
  $info = sirius_service_server_info($type);
  return $info['instance'];
}

function sirius_service_server_type() {
  return sirius_domain_variable_get('sirius_service_server_type', 'default');
}

function sirius_service_server_settings() {
  $settings = json_decode(sirius_domain_variable_get('sirius_service_server_settings'), TRUE);
  return $settings;
}

/** 
 * Implements hook_sirius_service_server_info
 */

function sirius_service_sirius_service_server_info() {
  $infos = array();
  Sirius_Service_Server_Default::getInstance()->setInfo($infos);
  Sirius_Service_Server_Proxy::getInstance()->setInfo($infos);
  return $infos;
}

class Sirius_Service_Server extends Sirius_Singleton {
  public function settings_form() { return array(); }
  public function respond($request) { 
    return $this->respond_error('Root respond() called. Classes must override this method.');
  }

  public function respond_error($msg) {
    return new Sirius_Server_Response(array('success' => FALSE, 'msg' => $msg)); 
  }
}

class Sirius_Service_Server_Default extends Sirius_Service_Server {
  public $type = 'default';
  public $name = 'Default Sirius Server';
  public $description = 'Default Sirius server; responds to a sirius client. NOT YET IMPLEMENTED.';

  public function settings_form() { 
    return $form;
  }
}
