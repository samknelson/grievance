<?php

function sirius_node_tab_wizard($node, $wizard_name) {
	// Which wizard?
	$info = sirius_wizard_info($wizard_name);
	if (!$info) { return "No wizard found for $wizard_name"; }
	$current_step = $_REQUEST['step'];
	$html = sirius_wizard_render_steps($node, $wizard_name, $info, $current_step);

	return $html;
}


function sirius_wizard_render_steps($node, $wizard_name, $info, $current_step) {
	$json = json_decode($node->field_sirius_json['und'][0]['value'], TRUE);
	$steps = call_user_func($info['callback_step_list'], $node);
	if (!$current_step || !$steps[$current_step]) { $current_step = array_keys($steps)[0]; }
	$html = '<ul>';
	$enabled = TRUE;
	foreach ($steps as $step_name => $step_label) {
		$status = $json['wizard'][$wizard_name]['steps'][$step_name]['status'];
		$html .= '<li>';
		if ($enabled) {
			$html .= '<a href="/' . current_path() . '?step=' . $step_name . '">';
		}
		if ($step_name == $current_step) { $html .= '* '; }
		$html .= $step_label;
		if ($enabled) {
			$html .= '</a>';
		}
		$html .= '</li>';

		if ($step_name == $current_step) { $enabled = FALSE; }
	}
	$html .= '</ul>';
	return $html;
}

function sirius_wizard_info($wizard_name) {
	$wizard_infos = module_invoke_all('sirius_wizard_info');
	return $wizard_infos[$wizard_name];
}

/***
 * Implements hook_sirius_wizard_info() 
 */

function sirius_sirius_wizard_info() {
  $items = array();

  $items['feed'] = array(
    'name' => t('Feed'),
    'description' => 'Basic feed uploader',
    'callback_step_list' => 'sirius_feed_wizard_step_list',
    'callback_step_content' => 'sirius_feed_wizard_content',
  );

  return $items;
}

function sirius_feed_wizard_step_list($node) {
	return array(
		'upload' => 'Upload',
		'worksheet' => 'Worksheet',
		'mapping' => 'Mapping',
		'validate' => 'Validate',
		'process' => 'Process'
	);
}

function sirius_feed_wizard_content($node, $step) {

}