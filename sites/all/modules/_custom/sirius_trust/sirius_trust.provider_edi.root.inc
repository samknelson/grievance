<?php

class Sirius_Trust_Provider_Edi extends Sirius_Plugin {
	public $type = 'root';
	public $name = 'Root';
	public $description = 'Default EDI class for a provider';

	public function report_type_options() {
		$report_type_infos = report_cache_info_all();

		$options = array();
		foreach ($report_type_infos as $key => $info) {
			if (! ($info['generator'] instanceof Sirius_Trust_Provider_Edi_Report_Generator) ) { continue; }
			$options[$key] = $info['name'];
		}
		return $options;
	}

	public function report_options($entity_node, $plugin_uuid) {
	  $report_type = $this->report_type($entity_node, $plugin_uuid);
	  $info = report_cache_info($report_type);
	  $generator = $info['generator'];
		$reports = $generator->search(array('limit' => 100, 'parameters' => array('entity_nid' => $entity_node->nid, 'plugin_uuid' => $plugin_uuid)));
		if (!$reports) { return array(); }
		return array_combine(array_keys($reports), array_keys($reports));
	}

	public function plugin_class() { return 'trust_provider_edi'; }

	public function entity_quicknav($plugin_uuid) {
		$base_path = current_path();
		$base_path = preg_replace("/$plugin_uuid\/.+/", $plugin_uuid, $base_path);

		$links = array();

		$link = array();
		$link['title'] = 'Settings';
		$link['path'] = $base_path;
		$links[] = $link;

		$link = array();
		$link['title'] = 'Generate';
		$link['path'] = "$base_path/generate";
		$links[] = $link;

		$link = array();
		$link['title'] = 'Download';
		$link['path'] = "$base_path/download";
		$links[] = $link;

		$link = array();
		$link['title'] = 'Push';
		$link['path'] = "$base_path/push";
		$links[] = $link;

		return sirius_quicknav($links);
	}

	//
	// Base settings form
	// 

	public function entity_settings_form($form, &$form_state, $entity_node, $plugin_uuid) {
		$form = parent::entity_settings_form($form, $form_state, $entity_node, $plugin_uuid);
		$form['nav2'] = array('#markup' => $this->entity_quicknav($plugin_uuid), '#weight' => -5);
		unset($form['settings']);
		$action = sirius_node_tab_arg(1);

		if (!$action) 									{ return $this->entity_settings_form_impl($form, $form_state, $entity_node, $plugin_uuid); } 
		else if ($action == 'generate') { return $this->entity_generate_form($form, $form_state, $entity_node, $plugin_uuid); }
		else if ($action == 'push') 		{ return $this->entity_push_form($form, $form_state, $entity_node, $plugin_uuid); }
		else if ($action == 'download') { return $this->entity_download_form($form, $form_state, $entity_node, $plugin_uuid); }
		else 														{ return sirius_form_error("Unknown action: $action"); }
	}

	public function entity_settings_form_submit($form, &$form_state) {
		$action = sirius_node_tab_arg(1);
		if (!$action) 									{ return $this->entity_settings_form_impl_submit($form, $form_state); } 
		else if ($action == 'generate') { return $this->entity_generate_form_submit($form, $form_state); }
		else if ($action == 'push') 		{ return $this->entity_push_form_submit($form, $form_state); }
		else if ($action == 'download') { return $this->entity_download_form_submit($form, $form_state); }
		else 														{ drupal_set_message("Unknown action: $action"); }
	}

	//
	// "Real" settings form
	// 

	public function entity_settings_form_impl($form, &$form_state, $entity_node, $plugin_uuid) {
		$this->report_type_options();

		$form['settings'] = array();

		$form['settings']['report_type'] = array(
			'#type' => 'select',
			'#title' => t('Report'),
			'#options' => array(NULL => t('-- Select --')) + $this->report_type_options(),
		);

		$form['settings']['filename'] = array(
			'#type' => 'textfield',
			'#title' => t('Filename'),
		);


	  $report_type = $this->report_type($entity_node, $plugin_uuid);
	  if ($report_type) {
		  $info = report_cache_info($report_type);
		  $form['settings']['report_parameters'] = $info['generator']->form();
		  unset($form['settings']['report_parameters']['asof_date']);
		  unset($form['settings']['report_parameters']['report_date']);

		  if ($form['settings']['report_parameters']) {
			  $form['settings']['report_parameters']['#type'] = 'fieldset';
		  	$form['settings']['report_parameters']['#title'] = t('Report Settings');
			}
		}

		$form['settings']['sftp'] = array(
			'#type' => 'fieldset',
			'#title' => 'SFTP',
		);

		$form['settings']['sftp']['host'] = array(
			'#type' => 'textfield',
			'#title' => t('Host'),
		);

		$form['settings']['sftp']['port'] = array(
			'#type' => 'textfield',
			'#title' => t('Port'),
			'#size' => 4,
		);

		$form['settings']['sftp']['user'] = array(
			'#type' => 'textfield',
			'#title' => t('User'),
		);

		$form['settings']['sftp']['password'] = array(
			'#type' => 'textfield',
			'#title' => t('Password'),
		);

		$form['settings']['sftp']['public_key'] = array(
			'#type' => 'textarea',
			'#title' => t('Public Key'),
		);

		$form['settings']['sftp']['private_key'] = array(
			'#type' => 'textarea',
			'#title' => t('Private Key'),
		);

		$form['settings']['sftp']['private_key_password'] = array(
			'#type' => 'textfield',
			'#title' => t('Private Key Password'),
		);

		$form['settings']['sftp']['directory'] = array(
			'#type' => 'textfield',
			'#title' => t('Directory'),
		);

		sirius_jsonfield_util_set_defaults($form['settings'], $this->entity_settings($entity_node, $plugin_uuid));

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
			'#weight' => 10,
		);

		return $form;
	}

	public function local_keypath($entity_node, $plugin_uuid, $which) {
		$path = $this->local_directory($entity_node, $plugin_uuid);
		$path .= "/$which";
		$path .= ".txt";
		return $path;
	}

	public function entity_settings_form_impl_submit($form, &$form_state) {
		$plugin_uuid = $form_state['plugin_uuid'];
		$entity_node = node_load($form_state['entity_nid']);

		foreach (array('private_key', 'public_key') as $which) {
			$value = $form_state['values']['settings']['sftp'][$which];
			if ($value) {
				$path = $this->local_keypath($entity_node, $plugin_uuid, $which);
				$fp = fopen($path, "w");
				if (!$fp) { drupal_set_message("Unable to write to $path.", 'error'); }
				fwrite($fp, $value);
				fclose($fp);
			}
		}

		return parent::entity_settings_form_submit($form, $form_state);
	}

	//
	// Push form
	// 

	public function entity_push_form($form, &$form_state, $entity_node, $plugin_uuid) {
		$form_state['plugin_uuid'] = $plugin_uuid;
		$form_state['entity_nid'] = $entity_node->nid;

		$report_options = $this->report_options($entity_node, $plugin_uuid);
		if (!$report_options) { return sirius_form_error("This report has not been run yet."); }

		$form['report_id'] = array(
			'#type' => 'select',
			'#title' => 'Report',
			'#options' => array(NULL => t('-- Select --')) + $report_options,
			'#required' => TRUE,
		);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Push'),
			'#weight' => 10,
		);

		if ($form_state['submitted']) {
			$html = '<hr>';
			$html .= '<h2>' . t('Results') . '</h2>';
			$html .= sirius_minilog_render_html();
			$form['results'] = array('#markup' => $html);
		}

		return $form;
	}

	public function entity_push_form_submit($form, &$form_state) {
		$form_state['rebuild'] = TRUE;
		$plugin_uuid = $form_state['plugin_uuid'];
		$entity_nid = $form_state['entity_nid'];
		$report_id = $form_state['values']['report_id'];
		return $this->entity_push($entity_nid, $plugin_uuid, $report_id);
	}

	//
	// From command line:
	// drush --user=1 php-eval "sirius_require('sirius_trust', 'sirius_trust.provider_edi.root.inc'); sirius_minilog_echo_active(TRUE); print_r(Sirius_Trust_Provider_Edi::getInstance()->entity_push('13263972', '6149a28d-4d54-45cb-9a5e-a40f5b6a30f5', '2025-06-30 12:59:20'));"
	// 

	public function entity_push($entity_nid, $plugin_uuid, $report_id) {
	  sirius_minilog("Entity NID:  " . $entity_nid);
	  sirius_minilog("Plugin UUID: " . $plugin_uuid);
	  sirius_minilog("Report ID:   " . $report_id);

		$entity_node = node_load($entity_nid);
	  $report_type = $this->report_type($entity_node, $plugin_uuid);
	  $info = report_cache_info($report_type);

		// Get our settings
		$settings = $this->entity_settings($entity_node, $plugin_uuid);

		$host = $settings['sftp']['host'];
		if (!$host) { return array('success' => FALSE, 'msg' => 'Please enter an SFTP host.'); }

		$port = $settings['sftp']['port'];
		if (!$port) { return array('success' => FALSE, 'msg' => 'Please enter an SFTP port.'); }

		$username = $settings['sftp']['user'];
		if (!$username) { return array('success' => FALSE, 'msg' => 'Please enter a user.'); }

		// @todo: Password only for the moment
		$password = $settings['sftp']['password'];
		$public_key = $settings['sftp']['public_key'];
		$private_key = $settings['sftp']['private_key'];

		if (!$password && (!$public_key || !$private_key)) { return array('success' => FALSE, 'msg' => 'Please enter a password or a public and private key.'); }

		$remote_directory = $settings['sftp']['directory'];
		if (!$remote_directory) { $remote_directory = '/'; }

		$filename = $this->filename($entity_node, $plugin_uuid);

		// Write local
		$local_directory = $this->local_directory($entity_node, $plugin_uuid);
		$local_path = "$local_directory/$filename";
		sirius_minilog("Writing to $local_path");
		$fp = fopen($local_path, 'w');
		if (!$fp) { return array('success' => FALSE, 'msg' => "Unable to open $local_path"); }
		$info['generator']->edi_output_stream($fp, $report_id);
		$size = filesize($local_path);
		sirius_minilog("... wrote " . format_size($size) . " bytes.");

		// Read local
		sirius_minilog("Reading from $local_path");
		$input_stream = fopen($local_path, 'r');
		if (!$input_stream) { return array('success' => FALSE, 'msg' => "Unable to read from $local_path"); }

		// Connect
		sirius_minilog("Connecting to $host:$port ...");
		$conn = ssh2_connect($host, $port);
		if (!$conn) { return array('success' => FALSE, 'msg' => "SFTP Connection to $host port $port failed."); }
		sirius_minilog("... Connection ok");

		// Login
		sirius_minilog("Authenticating as $username ...");
		if ($public_key) {
			sirius_minilog("... Using public key authentication");
			$public_path = $this->local_keypath($entity_node, $plugin_uuid, 'public_key');
			$private_path = $this->local_keypath($entity_node, $plugin_uuid, 'private_key');
			$private_key_password = $settings['sftp']['private_key_password'];
			if (!$private_key_password) { $private_key_password = ''; }
			$result = ssh2_auth_pubkey_file($conn, $username, $public_path, $private_path, $private_key_password);
		} else { 
			sirius_minilog("... Using password authentication");
			$result = ssh2_auth_password($conn, $username, $password);
		}

		if (!$result) {
			return array('success' => FALSE, 'msg' => "Login to $host / $port as $username failed.");
		}
		sirius_minilog("... Authentication ok");

		// Get an SFTP object
		$sftp = ssh2_sftp($conn);

		// Handle relative remote directory
		if (!preg_match('/^\//', $remote_directory)) {
			$remote_directory = ssh2_sftp_realpath($sftp, ".") . '/' . $remote_directory;
		}
		$remote_path = "ssh2.sftp://" . intval($sftp) . "$remote_directory/$filename";
		// $remote_path = '/tmp/whatever.txt';
		sirius_minilog("Opening a stream to $remote_path ...");
		$output_stream = fopen($remote_path, 'w');
		if (!$output_stream) { return array('success' => FALSE, 'msg' => "Unable to open connection to $remote_path"); }

		sirius_minilog("... stream opened");

		sirius_minilog("Writing data ...");
		$chunk = 0;
		while ($data = fread($input_stream, 4000000)) {
			++$chunk;
			sirius_minilog("... Read chunk $chunk: " . format_size(strlen($data)));
			fwrite($output_stream, $data);
			sirius_minilog("... Wrote chunk $chunk: " . format_size(strlen($data)));
		}
		sirius_minilog("... upload complete");
		if ($output_stream) {
			$fstat = fstat($output_stream);
			sirius_minilog("Wrote " . format_size($fstat['size']));
		}

		sirius_minilog("Disconnecting.");
		fclose($output_stream);
		ssh2_disconnect($conn);

		sirius_minilog("Cleaning up");
		unlink($local_path);

		return array('success' => TRUE, 'msg' => "Uploaded file $filename");
	}

	//
	// Generate form
	// 

	public function report_type($entity_node, $plugin_uuid) {
		$settings = $this->entity_settings($entity_node, $plugin_uuid);
		return $settings['report_type'];
	}

	public function entity_generate_form($form, &$form_state, $entity_node, $plugin_uuid) {
		$settings = $this->entity_settings($entity_node, $plugin_uuid);

		$form_state['plugin_uuid'] = $plugin_uuid;
		$form_state['entity_nid'] = $entity_node->nid;

	  $report_type = $this->report_type($entity_node, $plugin_uuid);
	  if (!$report_type) { return sirius_form_error("Please select a report type."); }

	  $info = report_cache_info($report_type);
	  $generator = $info['generator'];
	  report_cache_form_prepare($report_type, $form, $form_state);

	  // We hide some of the report_cache paraphernalia
	  unset($form['report_cache']);
	  $form['report_type'] = array('#type' => 'value', '#value' => $report_type);

	  // Generate the form, with some deafults
		$form['parameters'] = $info['generator']->form();
		sirius_jsonfield_util_set_defaults($form['parameters'], $settings['report_parameters']);
		$form['parameters']['entity_nid'] = array('#type' => 'value', '#value' => $form_state['entity_nid']);
		$form['parameters']['plugin_uuid'] = array('#type' => 'value', '#value' => $form_state['plugin_uuid']);


	  $form['submit'] = array(
	    '#type' => 'submit',
	    '#value' => 'Run the Report',
	    '#weight' => 10,
	  );
	  return $form;
	}

	public function entity_generate_form_submit($form, &$form_state) {
		$form_state['values']['redirect'] = array('path' => current_path());
		$form_state['values']['success_msg'] = 'The report has been generated.';
	  $result = report_cache_form_submit($form, $form_state);
	  return array('success' => TRUE, 'msg' => 'Report generated.');
	}

	//
	// Download form
	// 

	public function entity_download_form($form, &$form_state, $entity_node, $plugin_uuid) {
		$form_state['plugin_uuid'] = $plugin_uuid;
		$form_state['entity_nid'] = $entity_node->nid;

		$report_options = $this->report_options($entity_node, $plugin_uuid);
		if (!$report_options) { return sirius_form_error("This report has not been run yet."); }

	  $report_type = $this->report_type($entity_node, $plugin_uuid);
	  $info = report_cache_info($report_type);
	  $generator = $info['generator'];

		$form['report_id'] = array(
			'#type' => 'select',
			'#title' => 'Report',
			'#options' => array(NULL => t('-- Select --')) + $report_options,
			'#required' => TRUE,
		);

		$form['format'] = array(
			'#type' => 'select',
			'#title' => 'Format',
			'#options' => array(
				'default' => 'Default for this report',
				'csv' => 'CSV',
			),
			'#required' => TRUE,
		);

		if ($generator->edi_has_premium()) {
			$form['premium'] = array(
				'#type' => 'select',
				'#title' => 'Premium?',
				'#options' => array(
					FALSE => 'No, download the raw file',
					TRUE => 'Yes, download the premium file',
				),
				'#required' => TRUE,
			);
		}


		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Download'),
	    '#weight' => 10,
		);

		return $form;
	}

	public function entity_download_form_submit($form, &$form_state) {
		$plugin_uuid = $form_state['plugin_uuid'];
		$entity_node = node_load($form_state['entity_nid']);

		$premium = $form_state['values']['premium'];

	  $report_type = $this->report_type($entity_node, $plugin_uuid);
	  $report_id = $form_state['values']['report_id'];
	  $info = report_cache_info($report_type);

		$format = $form_state['values']['format'];

		$filename = $this->filename($entity_node, $plugin_uuid, $premium);

		// A hack
		if ($format == 'csv') { $filename = preg_replace('/\.txt/', '.csv', $filename); }

		drupal_add_http_header('Content-Type', 'text/csv');
		drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
		$fp = fopen('php://output', 'w');

	  $info['generator']->edi_output_stream($fp, $report_id, $format, $premium);

	  exit();
	}

	public function filename($entity_node, $plugin_uuid, $premium = FALSE) {
		// Get our settings
		$settings = $this->entity_settings($entity_node, $plugin_uuid);
		$filename = $settings['filename'];
		if (!$filename) { $filename = 'nofilename_[current-date:custom:Ymd_His].txt'; }


		$tokens['entity_node'] = $entity_node;
		$aliases['entity_node'] = 'node';

		$filename = token_replace($filename, $tokens, array('clear' => TRUE, 'aliases' => $aliases));

		if ($premium) { $filename = "premium_" . $filename; }

		/*
		$filename = $entity_node->title;
		$filename .= '_';
		$filename .= $this->name;
		$filename .= '_';
		$filename .= date('Ymd');
		$filename = preg_replace('/[^a-zA-Z0-9_]/', '_', $filename);
		$filename .= '.' . $this->filename_suffix($entity_node, $plugin_uuid);
		*/

		return $filename;
	}

	public function filename_suffix($entity_node, $plugin_uuid) {
	  $report_type = $this->report_type($entity_node, $plugin_uuid);
	  $report_id = $form_state['values']['report_id'];
	  $info = report_cache_info($report_type);
	  $format = $info['generator']->edi_output_format();

	  if ($format == 'fixed') {
	  	return 'txt';
	  } else {
			return 'csv';
		}
	}

	public function local_directory($entity_node, $plugin_uuid) {
		$dir = drupal_realpath("private://");
		$dir .= '/sirius_trust_edi/';
		$dir .= $entity_node->nid;
		$dir .= "/";
		$dir .= $plugin_uuid;

		if (!is_dir($dir)) { drupal_mkdir($dir, NULL, TRUE); }

		return $dir;
	}
}