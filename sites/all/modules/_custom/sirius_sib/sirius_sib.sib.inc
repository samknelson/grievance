<?php

class Sirius_Sib extends Sirius_Access {
	public $type = 'sib';
	public $name = 'Access Restriction';
	public $description = 'Root sib plugin; must be overridden.';

	public function restriction_info_all() {
		return array(
			'dispatch_signin_self' => array(
				'title' => 'Dispatch - Signin - Self',
				'description' => 'Prohibit self-signin to to the dispatch "out of work" list.'
			),
			'event_bullpen_register_self' => array(
				'title' => 'Event - Bullpen - Register - Self',
				'description' => 'Prohibit self-signin into bullpens.'
			),
			'event_bullpen_register' => array(
				'title' => 'Event - Bullpen - Register',
				'description' => 'Prohibit staff from signing the worker into bullpens.'
			),
		);
	}

	public function restriction_info($restriction_key) {
		$info_all = $this->restriction_info_all();
		return $info_all[$restriction_key];
	}

	public function assign($entity_node, $sib_tid, $comment) {
		$entity_json = sirius_json_get($entity_node);
		$sib = $entity_json['sib'][$sib_tid];

		global $user;
		$sib['active'] = TRUE;
		$sib['assign'] = array();
		$sib['assign']['ts'] = time();
		$sib['assign']['uid'] = $user->uid;
		$sib['assign']['user_name'] = $user->name;
		$sib['assign']['comment'] = $comment;
		$sib['cancel'] = array();

		$entity_json['sib'][$sib_tid] = $sib;
		sirius_json_set($entity_node, $entity_json);
		node_save($entity_node);

		$category = 'sib';
		$type = 'assign';
		$msg = $entity_node->title . ": access restriction assigned: " . sirius_term_title($sib_tid);
		$handler_nids = array($entity_node->nid);
		node_save($entity_node);
		sirius_log($category, $type, $msg, $comment, '', $handler_nids, $attachments, $json);
	}

	public function cancel($entity_node, $sib_tid, $comment) {
		$entity_json = sirius_json_get($entity_node);
		$sib = $entity_json['sib'][$sib_tid];

		global $user;
		$sib['active'] = FALSE;
		$sib['cancel'] = array();
		$sib['cancel']['ts'] = time();
		$sib['cancel']['uid'] = $user->uid;
		$sib['cancel']['user_name'] = $user->name;
		$sib['cancel']['comment'] = $comment;

		$entity_json['sib'][$sib_tid] = $sib;
		sirius_json_set($entity_node, $entity_json);
		node_save($entity_node);

		$category = 'sib';
		$type = 'cancel';
		$msg = $entity_node->title . ": access restriction canceled: " . sirius_term_title($sib_tid);
		$handler_nids = array($entity_node->nid);
		node_save($entity_node);
		sirius_log($category, $type, $msg, $comment, '', $handler_nids, $attachments, $json);
	}

	public function list($entity_node) {
		$entity_json = sirius_json_get($entity_node);

		$entries = $entity_json['sib'];
		if (!$entries) { return array(); }
		foreach ($entries as $tid => $entry) {
			$entries[$tid]['term'] = taxonomy_term_load($tid);
		}
		return $entries;
	}

	public function render($sib_entries) {
		if (!$sib_entries) {
			return '<p>' . t('No access restrictions found.') . '</p>';
		}

		$rows = array();
		foreach ($sib_entries as $sib_tid => $sib_entry) {
			$active = $sib_entry['active'];

			$row = array();
			$row[] = $sib_entry['term']->name;

			if ($active) {
				$color = 'red';
				$row[] = 'Yes';
			} else {
				$color = 'grey';
				$row[] = 'No';
			}

			$row[] = $sib_entry['assign']['user_name'];
			$ts = $sib_entry['assign']['ts'];
			if ($ts) {
				$row[] = date('m/d/Y', $ts) . ' (' . format_interval(time() - $ts) . ' ago)';
			} else {
				$row[] = '';
			}
			$row[] = $sib_entry['assign']['comment'];

			$row[] = $sib_entry['cancel']['user_name'];
			$ts = $sib_entry['cancel']['ts'];
			if ($ts) {
				$row[] = date('m/d/Y', $ts) . ' (' . format_interval(time() - $ts) . ' ago)';
			} else {
				$row[] = '';
			}
			$row[] = $sib_entry['cancel']['comment'];

			$rows[] = array('class' => array($color), 'data' => $row);
		}

		$header = array();
		$header[] = 'Block Type';
		$header[] = 'Active?';

		$header[] = 'Assigned By';
		$header[] = 'Assigned On';
		$header[] = 'Comment';

		$header[] = 'Canceled By';
		$header[] = 'Canceled On';
		$header[] = 'Comment';

		return sirius_table_render($header, $rows);
	}

	public function term_settings($term) {
		$json = sirius_json_get($term);
		if (!$json['sib']['settings']) { return array(); }
		return $json['sib']['settings'];
	}

	public function term_settings_set($term, $settings) {
		$json = sirius_json_get($term);
		$json['sib']['settings'] = $settings;
		sirius_json_set($term, $json);
		taxonomy_term_save($term);
	}

	public function term_restrictions($term) {
		$settings = $this->term_settings($term);
		if (!$settings['restriction']) { return array(); }
		return array_filter(array_values($settings['restriction']));
	}

	public function access($action, $context = array()) {
		if (!sirius_domain_component_enabled('sib')) { return TRUE; }

		if (!$context['entity_node']) { return $this->deny("No entity node."); }

		$info = $this->restriction_info($action);
		if (!$info) { return $this->deny("Unknown restriction: $action"); }

		$sib_entries = $this->list($context['entity_node']);
		if (!$sib_entries) { return TRUE; }

		foreach ($sib_entries as $sib_tid => $sib_entry) {
			if (!$sib_entry['active']) { continue; }
			$restrictions = $this->term_restrictions($sib_entry['term']);
			if (in_array($action, $restrictions)) {
				return $this->deny("Access restriction " . $sib_entry['term']->name);
			}
		}

		return TRUE;
	}
}
