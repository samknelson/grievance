<?php

class Sirius_Service_Server_Proxy extends Sirius_Service_Server {
  public $type = 'proxy';
  public $name = 'Sirius Proxy Server';
  public $description = 'Handles proxy requests';
  public $methods = array(
  	'page_fetch' => array(
  		'title' => 'Page Fetch',
  	),
  	'user_lookup' => array(
  		'title' => 'User Lookup',
  	),
  );

  public function respond($request) {
  	// @todo: Some of this should be handled by the root respond() function.

		// The Sirius component must be enabled
		if (!sirius_domain_component_enabled('service:proxy:server')) { return $this->respond_error('The component service:proxy:server must be enabled.'); }

  	$method = $request->method;
  	if (!$this->methods[$method]) {
  		return $this->respond_error("Invalid method: $method");
  	}

  	return $this->{$method}($request->args);
  }

  public function page_fetch($args) {
  	// What path do we want to fetch?
		$path = $args['path'];
		if (!$path) { return $this->respond_error("No path."); }
  	drupal_set_message("Looking up: $path");
  	// return $this->respond_error("Hmm...");

  	// Change to the remote user
  	$response = $this->user_lookup($args);
  	if (!$response->success()) { return $response; }
  	if (!$response->account) { return $this->respond_error($response->msg()); }
  	global $user;
  	$user = user_load($response->account['uid']);

  	// Change the base path
  	global $GLOBALS;
  	$keys = array('base_path', 'base_url', 'base_root');
  	foreach ($keys as $key) {
  		if (isset($args['GLOBALS'][$key])) {
		  	$GLOBALS[$key] = $args['GLOBALS'][$key]; 
  		}
  	}

  	// Change the current_path()
  	global $_GET;
  	$_GET = $args['_GET'];
  	$_GET['q'] = $path;

  	// Generate the page
  	drupal_set_message("About to execute.");
		$page_result = menu_execute_active_handler($path, FALSE);

		// Construct a resopnse
  	$response = new Sirius_Server_Response();
  	$response->success = TRUE;
		$response->msg = "Retrieved: $path";
		$response->page['title'] = drupal_get_title();
		// $response->page['body'] = $page_result;
		$response->page['js'] = drupal_add_js();
		$response->page['css'] = drupal_add_css();
		$response->page['GLOBALS'] = $GLOBALS;

		return $response;
  }

  public function user_lookup($args) {
		sirius_require('uuid', 'uuid.inc');

  	$account_uuid = $args['account_uuid'];
  	if (!$account_uuid) { return $this->respond_error("No account UUID received."); }
  	if (!uuid_is_valid($account_uuid)) { return $this->respond_error("Does not look like a valid UUID: $account_uuid"); }

  	$sql = "select uid, name from users ";
  	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'user' and field_data_field_sirius_domain.entity_id = users.uid ";
  	$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'user' and field_data_field_sirius_json.entity_id = users.uid ";

  	$sql_args[':domain_nid'] = sirius_domain_nid();
  	$sql .= "where field_sirius_domain_target_id = :domain_nid ";

  	$sql_args[':account_uuid'] = $account_uuid;
  	$sql .= "and json_unquote(json_extract(field_sirius_json_value, '\$.proxy.server.client_uuid')) = :account_uuid ";

  	$stmt = sirius_sql_query($sql, $sql_args);
  	$hr = $stmt->fetchAssoc();

  	$response = new Sirius_Server_Response();
  	$response->success = TRUE;
  	if (!$hr) {
  		$response->msg = "There is no server user currently associated with the proxy user $account_uuid."; 
  	} else {
  		$response->msg = "The server user $hr[name] ($hr[uid]) is associated with the proxy user $account_uuid.";
  		$response->account = $hr;
  	}
  	return $response;
  }
}
