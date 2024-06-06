<?php

function sirius_edls_employer_switch() {
  $access = sirius_access_instance('edls');
  if (!$access->access('staff')) {
    return array('markup' => '', 'employer_node' => sirius_employer_get());
  }

  drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');
  drupal_add_css(drupal_get_path('module', 'sirius_edls') . '/css/employer.css');

  $incoming_employer_nid = $_GET['employer_nid'];

  $html = '<div class="sirius_edls_employer_switch">';
  $html .= '<form method="get">';
  $html .= t('Employer:');
  $html .= '<select name="employer_nid">';
  $html .= '<option value="">' . t('-- Select --') . '</option>';
  $employers = sirius_edls_employer_list();
  foreach ($employers as $employer_nid => $employer_name) {
    if ($employer_nid == $incoming_employer_nid) { $selected = ' selected'; } else { $selected = ''; }
    $html .= "<option value=\"$employer_nid\" $selected>$employer_name</option>";
  }
  $html .= '</select>';
  $html .= '<input type="submit" value="' . t('Go') . '">';
  $html .= '</form>';
  $html .= '</div>';
  $html .= '<br clear="right">';

  if ($incoming_employer_nid && $employers[$incoming_employer_nid]) { $employer_node = node_load($incoming_employer_nid); }
  return array('markup' => $html, 'employer_node' => $employer_node);
}

function sirius_edls_employer_active($employer_nid) {
	$cache =& drupal_static(__FUNCTION__);

	if (!isset($cache[$employer_nid])) {
		$employer_node = node_load($employer_nid);
		$employer_json = sirius_json_get($employer_node);
		$cache[$employer_nid] = ($employer_json['edls']['active'] == 'Yes');
	}

	return $cache[$employer_nid];
}

function sirius_edls_employer_list() {
	$employers = sirius_employer_list();
	foreach ($employers as $employer_nid => $employer_name) {
		if (!sirius_edls_employer_active($employer_nid)) { unset($employers[$employer_nid]); }
	}
	return $employers;
}