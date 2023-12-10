<?php

function sirius_edls_page_landing() {
	global $user;
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');
	drupal_add_css(drupal_get_path('module', 'sirius_edls') . '/css/sheet.css');

	$result = sirius_edls_employer_switch();
	$employer_node = $result['employer_node'];
	$html .= $result['markup'];

	if ($employer_node && sirius_access_instance('edls')->access('employer_dashboard', array('employer_nid' => $employer_node->nid))) {
		$html .= sirius_edls_summary($employer_node, date('Y-m-d'));
	}

	$html .= '<h2>' . t('My Sheets') . '</h2>';
	$html .= '<p>' . l('Create a New Sheet', 'sirius/edls/sheet-create', array('attributes' => array('class' => array('tool-button')))) . '</p>';

	$params = array();
	$params['supervisor'] = $user->uid;
	$params['date_min'] = date('Y-m-d', strtotime('-1 day'));
  $sheet_nodes = sirius_edls_sheet_search($params);
  $render_options = array();
  $render_options['hide_supervisor'] = TRUE;
  $render_options['empty_msg'] = t('You have no upcoming sheets. To find earlier sheets or sheets owned by other users, please go to the "Sheets" tab in the menu above.');
  if ($employer_node) { $render_options['hide_employer'] = TRUE; }
  $html .= sirius_edls_sheet_list_render($sheet_nodes, $render_options);

  return $html;
}
