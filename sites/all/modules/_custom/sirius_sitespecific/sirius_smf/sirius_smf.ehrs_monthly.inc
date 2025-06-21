<?php

class Sirius_Smf_Ehrs_Monthly extends Sirius_Employer_Monthly_Upload {
  public function log_category() { return 'smf:ehrs'; }
  public function feed_type() { return 'sirius_smf_feed_ehrs'; }

  public function get_access() { return Sirius_Smf_Ehrs_Monthly_Access::getInstance(); }
  public function render_tools($eup_node) {
    $html = l('Main Upload', 'node/' . $eup_node->nid . '/sirius_smf_ehrs_wizard', array('attributes' => array('class' => array('tool-button'))));

    $html .= l('Corrections', 'node/' . $eup_node->nid . '/sirius_smf_ehrs_wizard_correction', array('attributes' => array('class' => array('tool-button'))));

    return $html;
  }

  public function get_feed_node($eup_node) {
    // Get the feed node
  	$feed_nodes = sirius_log_handlers($eup_node, 'sirius_feed');
    foreach ($feed_nodes as $feed_nid => $probe) {
      if ($probe->field_sirius_type['und'][0]['value'] != $this->feed_type()) { continue; }
      $feed_node = $probe;
      break;
    }

    // If it doesn't exist, create it.
  	if (!$feed_node) {
  		$feed_node = sirius_feed_wizard_launch($this->feed_type(), TRUE);
  		sirius_log_handler_set($eup_node, $feed_node->nid);
  		node_save($eup_node);
  	}

    // Connect the employer
  	$employer_node = $this->get_employer($eup_node);
  	if ($feed_node->field_grievance_shop['und'][0]['target_id'] != $employer_node->nid) {
  		$feed_node->field_grievance_shop['und'][0]['target_id'] = $employer_node->nid;
  		node_save($feed_node);
  	}

    // The ehrs node has the same status as the feed node
    $this->set_status($eup_node, $feed_node->field_sirius_feed_status['und'][0]['value']);

  	return $feed_node;
  }

  public function get_corrections_feed_nodes($eup_node) {
    // Get the feed node
    $feed_nodes = sirius_log_handlers($eup_node, 'sirius_feed');
    foreach ($feed_nodes as $feed_nid => $probe) {
      if ($probe->field_sirius_type['und'][0]['value'] != $this->feed_type_correction()) { 
        unset($feed_nodes[$feed_nid]);
      }
    }

    return $feed_nodes;
  }

  public function add_correction($eup_node) {
    $feed_node = sirius_feed_wizard_launch($this->feed_type_correction(), TRUE);
    sirius_log_handler_set($eup_node, $feed_node->nid);
    node_save($eup_node);
    return $feed_node;
  }
}

