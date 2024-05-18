<?php

function sirius_comm_send($medium, $params) {
	$instance = sirius_comm_instance($medium);
	return $instance->send($params);
}

function sirius_comm_info_all() {
  $infos = &drupal_static(__FUNCTION__);
  if (!isset($infos)) {
    $infos = module_invoke_all('sirius_comm_info');
    ksort($infos);
  }
  return $infos;
}

function sirius_comm_info($medium) {
  $infos = &drupal_static(__FUNCTION__); 
  if (!isset($infos)) { $infos = sirius_comm_info_all(); }
  return $infos[$medium];
}

function sirius_comm_instance($medium_or_object) {
	if (is_object($medium_or_object)) { 
		if ($medium_or_object->field_sirius_type['und'][0]['value'] == 'event') { return NULL; }
		$medium = $medium_or_object->field_sirius_category['und'][0]['value'];
	} else {
		$medium = $medium_or_object;
	}
  $info = sirius_comm_info($medium);
  return $info['instance'];
}

/**
 * Implements hook_sirius_comm_info()
 */

function sirius_sirius_comm_info() {
  $infos = array();
  Sirius_Comm_Dummy::getInstance()->setInfo($infos);
  Sirius_Comm_Email::getInstance()->setInfo($infos);
  Sirius_Comm_Sms::getInstance()->setInfo($infos);
  Sirius_Comm_Voice::getInstance()->setInfo($infos);
  return $infos;
}

class Sirius_Comm extends Sirius_Singleton {
	public $type = 'dummy';
	public $name = 'Dummy';

	// Status options:
	// - 'error': Failed to send; no external_id
	// - 'sending': Message is being sent
	// - 'sent': Message was sent
	// - 'failed': Message was sent, but not delivered

	public function status_options() {
		return array(
			'error' => 'Error',
			'sending' => 'Sending',
			'sent' => 'Sent',
			'failed' => 'Failed'
		);
	}

	public function get_status($comm_node) {
		return $comm_node->field_sirius_type['und'][0]['value'];
	}

	public function medium() {
		return $this->type;
	}

	public function send(&$params) {
		// Must have an (internal) ID
		if (!$params['id']) { $params['id'] = $params['X-Sirius-Id']; }
		if (!$params['id']) { $params['id'] = uuid_generate(); }

		// Must have a body (sometimes passed as e.g. $params['sms'])
		$medium = $this->medium();
		if (!$params['body']) { $params['body'] = $params[$medium]; }

		// Sometimes "from" is passed in differently
		if (!$params['from']) { $params['from'] = $params[$medium . "_from"]; }

    $result = $this->send_possible($params);
    if (!$result['success']) { return $result; }

    $attachments = $params['attachments'];

		$result = $this->send_impl($params);

		if ($result['success']) { $log_type = 'sending'; } else { $log_type = 'error'; }

		$log_json = array(
			'params' => $params, 
			'result' => $result,
			'external_id' => $result['external_id'],
		);

		$to_render = $this->render_addr($params['to'], 'oneline_txt');

		$medium = $this->medium();

		$files = array();
		if ($attachments) {
			foreach ($attachments as $attachment) {
			  if ($attachment['filecontent']) {
				  $file = file_save_data(
				    $attachment['filecontent'], 
				    'private://' . uuid_generate() . '_' . $attachment['filename'], 
				    FILE_EXISTS_RENAME
				  );
				  $file->display = 1;
				  $files[] = (array) $file;
				}
			}
		}

    $comm = sirius_log(
      $medium, 
      $log_type, 
      $to_render, 
      strip_tags($params['body']),
      $params['id'],
      $params['handler_nids'],
      $files,
      $log_json
    );

    $result['comm'] = $comm;

		return $result;
	}

	public function send_possible($params) {
		if ($this->already_sent($params['id'])) { return array('success' => FALSE, 'msg' => 'Already sent ID ' . $params['id']); }
		$result = $this->send_possible_impl($params);
		return $result;
	}

	public function send_impl(&$params) {
		return array('success' => FALSE, 'msg' => 'Root send_impl called; please specify a medium.');
	}

	public function already_sent($id) {
		if (!$id) { return FALSE; }
	  // SQL for speed
	  $sql = "select * from node ";
	  $sql .= "join field_data_field_sirius_notes on field_data_field_sirius_notes.entity_type = 'node' and field_data_field_sirius_notes.entity_id = node.nid ";
	  $sql .= "join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
	  $sql .= "join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
	  $sql .= "where node.type = 'sirius_log' ";
	  $sql .= "and field_sirius_notes_value = :id ";
	  $sql .= "and field_sirius_category_value = :type ";
	  $sql .= "and field_sirius_type_value in ('sending', 'sent', 'failed') ";
	  $stmt = db_query($sql, array(':id' => $id, ':type' => $this->medium()));
	  $hr = $stmt->fetchAssoc();
	  if ($hr) { return TRUE; }
	  return FALSE;
	}

	public function find_by_external_id($external_id) {
		if (!$external_id) { return NULL; }
	  // SQL for speed
	  $sql = "select nid from node ";
	  $sql .= "join field_data_field_sirius_notes on field_data_field_sirius_notes.entity_type = 'node' and field_data_field_sirius_notes.entity_id = node.nid ";
	  $sql .= "join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	  $sql .= "join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
	  $sql .= "join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
	  $sql .= "where node.type = 'sirius_log' ";
	  $sql .= "and field_sirius_json_value ->> '$.external_id' = :external_id ";
	  $sql .= "and field_sirius_category_value = :type ";
	  $sql .= "and field_sirius_type_value in ('sending', 'sent', 'failed') ";
	  $sql .= "order by nid desc limit 1 ";
	  $stmt = sirius_sql_query($sql, array(':external_id' => $external_id, ':type' => $this->medium()));
	  $hr = $stmt->fetchAssoc();
	  if ($hr) { return $hr['nid']; }
	  return NULL;
	}

	public function render($comm_node, $options = array()) {
		drupal_add_css(drupal_get_path('module', 'sirius') . '/css/comm.css');

		$status = $this->get_status($comm_node);

		$html = "<div class=\"sirius_comm sirius_comm_status_$status\">";

		$html .= $this->render_status($comm_node);

		$html .= '<div class="sirius_comm_fromto">';
		$html .= $this->render_from($comm_node);
		$html .= $this->render_to($comm_node);
		$html .= '</div>';
		// $html .= $this->render_options($comm_node);
		$html .= $this->render_body($comm_node);
		$html .= $this->render_attachments($comm_node);
		$html .= $this->render_events($comm_node);

		$html .= '</div>';

		return $html;
	}

	public function get_from($comm_node) {
		$json = sirius_json_get($comm_node);
		if ($json['params']['from']) { return $json['params']['from']; }
		return $this->get_from_default();
	}

	public function get_from_default() {
		return '';
	}

	public function get_to($comm_node) {
		$json = sirius_json_get($comm_node);
		return $json['params']['to'];
	}

	public function get_body($comm_node) {
		$json = sirius_json_get($comm_node);
		return $json['params']['body'];
	}

	public function get_options($comm_node) {
		$json = sirius_json_get($comm_node);
		return $json['params']['options'];
	}

	public function render_addr($addr, $format) {
		if (!$addr) { return ''; }
		if (is_array($addr)) {
			if ($format == 'short_html') {
				$render = join(', ', array_filter(array_values($addr))); 
			} else {
				$render = join('<br />', array_filter(array_values($addr))); 
			}
		} else {
			$render = $addr;
		}

		return $render;
	}

	public function get_id($comm_node) {
		return $comm_node->field_sirius_notes['und'][0]['value'];
	}

	public function get_external_id($comm_node) {
		$json = sirius_json_get($comm_node);
		return $json['external_id'];
	}

	public function render_oneline($comm_node) {
		$status_options = $this->status_options();

		$html = '<div class="sirius_comm_oneline">';
		$html .= t('To: ') . $this->render_addr($this->get_to($comm_node), 'short_html');
		$html .= '&nbsp::&nbsp;';
		$html .= t('Status: ') . $status_options[$this->get_status($comm_node)];
		$html .= '</div>';
		return $html;
	}

	public function get_external_url($comm_node) {
		return '';
	}

	public function render_status($comm_node) {
		$json = sirius_json_get($comm_node);

		$status = $this->get_status($comm_node);
		$id = $this->get_id($comm_node);
		$external_id = $this->get_external_id($comm_node);

		$html = '<div class="sirius_comm_section sirius_comm_status">';
		$status_options = $this->status_options();
		$html .= sirius_fakerender_field(t('Medium:'), $this->medium());
		$html .= sirius_fakerender_field(t('Status:'), $status_options[$status]);
		if ($id) { $html .= sirius_fakerender_field(t('Internal ID:'), $id); }
		if ($external_id) {
			$render = $external_id;
			$external_link = $this->get_external_url($comm_node);
			if ($external_link) { $render = "<a href=\"$external_link\" target=\"_blank\">$external_id</a>"; }

			$html .= sirius_fakerender_field(t('External ID:'), $render);
		}
		if (!$json['result']['success']) {
			$msg = $json['result']['msg'];
			if ($msg) {
				$html .= sirius_fakerender_field(t('Error Message:'), $msg);
			}
		}

		$html .= '</div>';
		return $html;
	}

	public function render_from($comm_node) {
		$from = $this->get_from($comm_node);
		if (!$from) { return ''; }

		$html .= '<div class="sirius_comm_section sirius_comm_from">';
		$html .= '<h2>' . t('Sender') . '</h2>';
		$html .= $this->render_addr($from, 'long_html');
		$html .= '</div>';
		return $html;
	}

	public function render_to($comm_node) {
		$to = $this->get_to($comm_node);
		if (!$to) { return ''; }

		$html .= '<div class="sirius_comm_section sirius_comm_to">';
		$html .= '<h2>' . t('Recipient(s)') . '</h2>';
		$html .= $this->render_addr($to, 'long_html');
		$html .= '</div>';
		return $html;
	}

	public function render_options($comm_node) {
		$options = $this->get_options($comm_node);
		$options = $this->get_options($comm_node);
		if (!$options) { return TRUE; }

		$html = '';
		$html .= '<div class="sirius_comm_section sirius_comm_options">';
		$html .= '<h2>' . t('Options') . '</h2>';
		foreach ($options as $key => $value) {
			$key_render = $this->render_option_label($key, $value);
			$value_render = $this->render_option_value($key, $value);
			$html .= sirius_fakerender_field($key_render . ':', $value_render);
		}
		$html .= '</div>';
		return $html;
	}

	public function render_option_label($key, $value) {
		return $key; 
	}

	public function render_option_value($key, $value) {
		return $value;
	}

	public function render_attachments($comm_node) {
		$records = $comm_node->field_sirius_attachments['und'];
		if (!$records) { return ''; }
		$html .= '<div class="sirius_comm_section sirius_comm_attachments">';
		$html .= '<h2>' . t('Attachments') . '</h2>';

		$renderable = field_view_field('node', $comm_node, 'field_sirius_attachments', 'full');
		$renderable['#title'] = 'Attachment';
		$html .= drupal_render($renderable);

		$html .= '</div>';
		return $html;
	}

	public function render_body($comm_node) {
		$body = $this->get_body($comm_node);
		if (!$body) { return ''; }
		$html .= '<div class="sirius_comm_section sirius_comm_body">';
		$html .= '<h2>' . t('Message Body') . '</h2>';
		$html .= $body;

		$html .= '</div>';
		return $html;
	}

	public function render_events($comm_node) {
		$event_nids = $this->get_event_nids($comm_node->nid);
		if (!$event_nids) { return ''; }
		$event_nodes = node_load_multiple($event_nids);

		$html .= '<div class="sirius_comm_section sirius_comm_events">';
		$html .= '<h2>' . t('Events') . '</h2>';

		$header = array();
		/*
		$header[] = t('Event');
		$header[] = t('Time');
		$header[] = t('Tools');
		*/

		$rows = array();
		foreach ($event_nodes as $event_nid => $event_node) {
			$event_status = $event_node->field_sirius_message['und'][0]['value'];
			$comm_status = $this->comm_status_from_event_status($event_status);
			if ($comm_status == 'sending') { $class = 'blue'; }
			else if ($comm_status == 'failed' || $comm_status == 'error') { $class = 'red'; }
			else if ($comm_status == 'sent') { $class = 'green'; }
			else { $class = ''; }

			$msg = $this->render_event_status($event_status);
			if ($event_node->field_sirius_summary['und'][0]['value'] != $event_status) {
				$msg .= ' ' . $event_node->field_sirius_summary['und'][0]['value'];
			}

			$row = array();
			$row[] = $msg;
			$row[] = date('m/d/Y h:i a', $event_node->created) . ' (' . format_interval(time() - $event_node->created) . ' ago)';
			$row[] = l('View', "node/$event_nid/sirius_log_comm_event", array('attributes' => array('class' => array('tool-button'))));
			$rows[] = array(
				'class' => array($class), 
				'data' => $row
			);
		}
		$html .= sirius_table_render($header, $rows);
		$html .= '</div>';
		return $html;
	}

	public function comm_status_from_event_status($event_status) {
		return $this->event_status_info($event_status)['comm_status'];
	}

	public function event_status_info_all() {
		return array();
	}

	public function event_status_info($event_status) {
		static $info_all;
		if (!isset($info_all)) { $info_all = $this->event_status_info_all(); }

		$result = $info_all[$event_status];
		if (!$result) { $result = array(); }
		if (!$result['title']) { $result['title'] = $event_status; }
		if (!$result['comm_status']) { $result['comm_status'] = 'sending'; }
		return $result;
	}

	public function render_event_status($event_status) {
		return $this->event_status_info($event_status)['title'];
	}

	public function get_event_nids($comm_nid) {
		if (!$comm_nid) { return array(); }
	  // SQL for speed
	  $sql = "select distinct nid from node ";
	  $sql .= "join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
	  $sql .= "join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
	  $sql .= "join field_data_field_sirius_log_handler on field_data_field_sirius_log_handler.entity_type = 'node' and field_data_field_sirius_log_handler.entity_id = node.nid ";
	  $sql .= "where node.type = 'sirius_log' ";
	  $sql .= "and field_sirius_log_handler_target_id = :comm_nid ";
	  $sql .= "and field_sirius_category_value = :type ";
	  $sql .= "and field_sirius_type_value = 'event' ";
	  $sql .= "order by nid desc limit 1000 ";
	  $stmt = db_query($sql, array(':comm_nid' => $comm_nid, ':type' => $this->medium()));

	  $event_nids = array();
	  while ($hr = $stmt->fetchAssoc()) { $event_nids[] = $hr['nid']; }
	  return $event_nids;
	}


	public function webhook($params = array()) {
		// Parse our webhook
		$result = $this->webhook_impl($params);
		if (!$result['success']) {
			watchdog('sirius_comm', get_class($this) . ' webhook failed: ' . $result['msg']);
			return $result;
		}

		// Log the event
		$event_id = $result['event_id'];
		if (!$event_id) { $event_id = uuid_generate(); }
		$external_id = $result['external_id'];
		$handler_nids = array();
		$comm_nid = $this->find_by_external_id($external_id);
		if ($comm_nid) { $handler_nids[] = $comm_nid; }
		$event_status = $result['status'];
		$medium = $this->medium();
		$msg = $result['msg'];
		if (!$msg) { $msg = $event_status; }

    $event_node = sirius_log(
      $medium, 
      'event', 
      $event_status, 
      $msg,
      $event_id,
      $handler_nids,
      NULL,
      $result
    );

    // Update the comm node
    if ($comm_nid && $event_status) {
    	$comm_node = node_load($comm_nid);
    	$comm_status = $this->comm_status_from_event_status($event_status);
    	if ($comm_status != $comm_node->field_sirius_type['und'][0]['value']) {
    		$comm_node->field_sirius_type['und'][0]['value'] = $comm_status;
    		node_save($comm_node);
    	}
    }

		return $result;
	}

	public function list_from_handler($handler_nid, $options = array()) {
		$infos = sirius_comm_info_all();
		$media = array();
		foreach ($infos as $info) { $media[] = $info['instance']->medium(); }

		$sql_args = array();
		$sql = "select distinct nid from node ";
		$sql .= "left join field_data_field_sirius_log_handler on field_data_field_sirius_log_handler.entity_type = 'node' and field_data_field_sirius_log_handler.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
		$sql .= "where node.type = 'sirius_log' ";

		$sql .= "and field_sirius_log_handler_target_id = :entity_nid ";
		$sql_args[':entity_nid'] = $handler_nid;

		$sql .= "and field_sirius_category_value in (:media) ";
		$sql_args[':media'] = $media;

		$sql .= "order by created limit 1000 ";
		$stmt = sirius_sql_query($sql, $sql_args);
		$comm_nids = array();
		while ($hr = $stmt->fetchAssoc()) { $comm_nids[] = $hr['nid']; }

		return node_load_multiple($comm_nids);
	}

	public function render_multiple($comm_nodes, $options = array()) {
		$html_id = "sirius_comm_accordion_" . uuid_generate();

		drupal_add_library('system', 'ui.accordion');
  	drupal_add_js("jQuery(document).ready(function () { jQuery(\"#$html_id\").accordion({active: false, collapsible: true, autoehight: true, heightStyle: \"content\"}); });", 'inline');
  	
  	$html = "<div id=\"$html_id\">";

		foreach ($comm_nodes as $comm_nid => $comm_node) {
			$instance = sirius_comm_instance($comm_node);
			if (!$instance) { continue; }
			$html .= '<div class="sirius_accordion_label">';
			$html .= $instance->render_oneline($comm_node);
			$html .= '</div>';
			$html .= '<div class="sirius_accordion_body">';
			$html .= $instance->render($comm_node);
			$html .= '</div>';
		}
		$html .= '</div>';
		return $html;
	}

	public function update_handlers($comm_node) {
		$handlers = sirius_log_handlers($comm_node);
		foreach ($handlers as $handler_node) {
			$this->update_handler($comm_node, $handler_node);
		}
	}

	public function update_handler($comm_node, $handler_node) {
		// This should be implemented as callbacks and listeners. But it's not.
		if ($handler_node->type == 'sirius_log') {
			if ($handler_node->field_sirius_category['und'][0]['value'] == 'bulk:queue') {
				$handler_status = $handler_node->field_sirius_type['und'][0]['value'];
				$comm_status = $this->get_status($comm_node);
				if ($comm_status != $handler_status) {
					$handler_node->field_sirius_type['und'][0]['value'] = $comm_status;
					node_save($handler_node);
				}
			}
		}
	}
}

function sirius_comm_update_handlers($comm_nid) {
	$comm_node = node_load($comm_nid);
	$instance = sirius_comm_instance($comm_node);
	if (!$instance) { return; }
	$instance->update_handlers($comm_node);
}

class Sirius_Comm_Dummy extends Sirius_Comm {
	public function send_impl(&$params) {
		return array('success' => TRUE, 'msg' => 'Dummy implementation.');
	}
}