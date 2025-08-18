<?php

function sirius_smf_node_tab_ehrs_wizard_corrections_access($eup_node) {
	if ($eup_node->field_sirius_category['und'][0]['value'] != 'smf:ehrs') { return FALSE; }
	sirius_require('sirius_smf', 'sirius_smf.ehrs.inc');
	$ehrs_monthly = Sirius_Smf_Ehrs_Monthly::getInstance();
	return $ehrs_monthly->access('eup_view', array('eup_node' => $eup_node));
}

function sirius_smf_node_tab_ehrs_wizard_corrections_quicknav($eup_node) {
	sirius_require('sirius_smf', 'sirius_smf.ehrs.inc');
	$ehrs_monthly = Sirius_Smf_Ehrs_Monthly::getInstance();
	$feed_nodes = $ehrs_monthly->get_corrections_feed_nodes($eup_node);
	if (!$feed_nodes) { return ''; }

  $base_path = 'node/' . $eup_node->nid . '/sirius_smf_ehrs_wizard_correction';

  $links['base_path'] = 'List';

  $links = array();

  $links[] = array('title' => 'List', 'path' => $base_path);

  foreach ($feed_nodes as $feed_nid => $feed_node) {
    $link['title'] = date('Y-m-d h:i a', $feed_node->created);
    $link['path'] = "$base_path/$feed_nid";
    $links[] = $link;
  }

  return sirius_quicknav($links);
}

function sirius_smf_node_tab_ehrs_wizard_corrections($eup_node) {
	$feed_nid = sirius_node_tab_arg(0);
	if ($feed_nid) {
		$feed_node = node_load($feed_nid);

		$probe = sirius_smf_feed_ehrs_get_eup($feed_node);
		if (!$probe || ($probe->nid != $eup_node->nid)) {
			return sirius_form_error("Access denied: the feed node is not associated with the EUP node.");
		}
		$nav = sirius_smf_node_tab_ehrs_wizard_corrections_quicknav($eup_node);
		return $nav . sirius_node_tab_feed_wizard($feed_node);
	} else {
		return drupal_get_form('sirius_smf_node_tab_ehrs_wizard_corrections_form', $eup_node);
	}
}

function sirius_smf_node_tab_ehrs_wizard_corrections_form($form, &$form_state, $eup_node) {
	sirius_require('sirius_smf', 'sirius_smf.ehrs.inc');
	$ehrs_monthly = Sirius_Smf_Ehrs_Monthly::getInstance();

	$form['#tree'] = TRUE;
	$form_state['eup_nid'] = $eup_node->nid;

	$feed_nodes = $ehrs_monthly->get_corrections_feed_nodes($eup_node);
	$form['nav'] = array('#markup' => sirius_smf_node_tab_ehrs_wizard_corrections_quicknav($eup_node));

	if ($feed_nodes) {
		$rows = array();
		foreach ($feed_nodes as $feed_nid => $feed_node) {
			$row = array();
			$row[] = date('Y-m-d h:i a', $feed_node->created);
			$row[] = $feed_node->field_sirius_type['und'][0]['value'];
			$row[] = l('Wizard', current_path() . '/' . $feed_nid, array('attributes' => array('class' => array('tool-button'))));
			$rows[] = $row;
		}

		$header = array();
		$header[] = t('Date');
		$header[] = t('Status');
		$header[] = t('Tools');

		$html = sirius_table_render($header, $rows);

		$form['feeds'] = array('#markup' => $html);
	}

	$form['create'] = array(
		'#type' => 'fieldset',
		'#title' => t('Create a New Correction'),
	);

	$form['create']['confirm'] = array(
		'#type' => 'checkbox',
		'#title' => 'I have a correction file to upload.',
		'#required' => TRUE,
	);

	$main_node = $ehrs_monthly->get_feed_node($eup_node);
	if ($main_node->field_sirius_feed_status['und'][0]['value'] != 'complete') {
		drupal_set_message("Corrections cannot be uploaded until the main feed is complete.", 'warning');
		$form['#disabled'] = TRUE;
	} else {
		$form['create']['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Add a Correction'),
		);
	}

	return $form;
}


function sirius_smf_node_tab_ehrs_wizard_corrections_form_submit($form, &$form_state) {
	$eup_node = node_load($form_state['eup_nid']);

	sirius_require('sirius_smf', 'sirius_smf.ehrs.inc');
	$ehrs_monthly = Sirius_Smf_Ehrs_Monthly::getInstance();

	$feed_node = $ehrs_monthly->add_correction($eup_node);

	drupal_goto(current_path() . '/' . $feed_node->nid);
}
