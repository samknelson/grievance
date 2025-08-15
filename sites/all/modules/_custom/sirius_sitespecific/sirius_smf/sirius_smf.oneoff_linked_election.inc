<?php

sirius_require('sirius', 'Sirius/Batchrunner.inc');
sirius_require('sirius', 'Sirius/Batchrunner_Node.inc');

function sirius_smf_node_tab_election_oneoff_linked_benefit_form($form, &$form_state, $election_node) {
	$form_state['election_nid'] = $election_node->nid;

	$form['#tree'] = TRUE;
	$form['mode'] = array(
		'#type' => 'select',
		'#title' => t('Mode'),
		'#options' => array(
			'test' => t('Test'),
			'live' => t('Live'),
		),
		'#require' => TRUE,
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

  $log = sirius_minilog_render_html();
  if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

	return $form;
}

function sirius_smf_node_tab_election_oneoff_linked_benefit_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$election_node = node_load($form_state['election_nid']);
	$mode = $form_state['values']['mode'];

	$runner = Sirius_Smf_Batchrunner_Oneoff_Linked_Benefit::getInstance();
	$context = array('mode' => $form_state['values']['mode']);
	return $runner->process_record($form_state['election_nid'], $context);
	// return $runner->process_batch($context);
	// return sirius_smf_oneoff_linked_benefit($form_state['values']['mode'], $election_node);
}

class Sirius_Smf_Batchrunner_Oneoff_Linked_Benefit extends Sirius_Batchrunner_Node {
	public $type = 'smf_oneoff_linked_benefit';
	public $name = 'Sirius_Smf_Oneoff_Linked_Benefit';

	public function content_type_impl(&$context = array()) {
		return 'sirius_trust_worker_election';
	}

	public function process_record_impl($election_node, &$context = array()) {
		$linked_benefits = $this->linked_benefits();

		$benefit_nids = sirius_fieldvals($election_node, 'field_sirius_trust_benefits', 'target_id');

		$new_benefit_nids = array();
		foreach ($benefit_nids as $benefit_nid) {
			sirius_minilog("Checking: " . $this->benefit_name($benefit_nid));
			if (!$linked_benefits[$benefit_nid]) { continue; }

			sirius_minilog_indent();
			foreach ($linked_benefits[$benefit_nid] as $linked_benefit_nid) {
				sirius_minilog("Checking linked: " . $this->benefit_name($linked_benefit_nid));
				if (in_array($linked_benefit_nid, $benefit_nids)) { continue; }
				sirius_minilog("... Adding " . $this->benefit_name($linked_benefit_nid) . " to the election");
				$new_benefit_nids[] = $linked_benefit_nid;
			}
			sirius_minilog_outdent();
		}

		if (!$new_benefit_nids) {
			sirius_minilog("No changes needed");
			return; 
		}

		foreach ($new_benefit_nids as $new_benefit_nid) {
			$election_node->field_sirius_trust_benefits['und'][] = array('target_id' => $new_benefit_nid);
		}
		if ($this->mode($context) == 'live') {
			node_save($election_node);
			sirius_minilog("Election updated.");
		} else {
			sirius_minilog("(Skipping save because mode = " . $this->mode($context) . ')');
		}
		return array("success" => TRUE, "changed" => TRUE);
	}

	public function benefit_name($benefit_nid) {
	  $names = &drupal_static(__FUNCTION__);
	  if (!$names) { $names = sirius_trust_benefit_options(FALSE); }
	  return $names[$benefit_nid];
	}

	public function linked_benefits() {
	  // Get the "linked" benefits
	  // - MLK should create:
	  // - - VSP Enhanced
	  // - - Express Scripts
	  // - - Hinge PT
	  // - - Life Insurance
	  // - - Carelon EAP
	  // - - Carelon Behavioral Health
	  // - - AD&D
	  // 
	  // - Healthnet, Kaiser & Kaiser Enhanced should create:
	  // - - VSP
	  // - - Express Scripts
	  // - - Life Insurance
	  // - - Carelon EAP
	  // - - AD&D

	  $linked_benefits = &drupal_static(__FUNCTION__);
	  if (!$linked_benefits) {
	    $map_by_id = array();
	    $map_by_id['M'] = array('3E', 'EXPRESSSCRIPTS', 'HINGEPT', '2', 'CARELONEAP', 'CARELONBEHAVIORALHEALTH', 'ADD');
	    $map_by_id['H'] = array('3', 'EXPRESSSCRIPTS', '2', 'CARELONEAP', 'ADD');
	    $map_by_id['K'] = $map_by_id['KE'] = $map_by_id['H'];

	    foreach ($map_by_id as $from_id => $to_ids) {
	      $from_nid = sirius_find_nid_by_id($from_id, 'sirius_trust_benefit');
	      if (!$from_nid) { return array('success' => FALSE, 'msg' => "Unable to find the benefit NID for ID [$from_id]"); }

	      foreach ($to_ids as $to_id) {
	        $to_nid = sirius_find_nid_by_id($to_id, 'sirius_trust_benefit');
	        if (!$to_nid) { return array('success' => FALSE, 'msg' => "Unable to find the benefit NID for ID [$to_id]"); }

	        $linked_benefits[$from_nid][] = $to_nid;
	      }
	    }
	  }

	  return $linked_benefits;
	}
}
