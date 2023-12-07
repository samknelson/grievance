<?php

function sirius_edls_page_landing() {
	global $user;
	$my_employer_node = sirius_employer_get();

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
