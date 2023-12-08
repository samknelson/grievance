<?php

function sirius_edls_page_landing() {
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');

	$result = sirius_edls_employer_switch();
	$my_employer_node = $result['employer_node'];
	$html .= $result['markup'];

	if ($my_employer_node && sirius_access_instance('edls')->access('employer_dashboard', array('employer_nid' => $my_employer_node->nid))) {
		$html .= '<div class="sirius_callout_basic">';
		$html .= '<h2>' . $my_employer_node->title . ': ' . date('l, F j, Y') . '</h2>';
		$html .= '<div style="height: 250px;"></div>';
		$html .= '</div>';
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
  if ($my_employer_node) { $render_options['hide_employer'] = TRUE; }
  $html .= sirius_edls_sheet_list_render($sheet_nodes, $render_options);

  return $html;
}
