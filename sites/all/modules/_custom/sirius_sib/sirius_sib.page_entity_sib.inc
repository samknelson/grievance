<?php

function sirius_sib_node_tab_entity_sib($entity_node) {
	/*
	$json = sirius_json_get($entity_node);
	unset($json['sib']);
	sirius_json_set($entity_node, $json);
	node_save($entity_node);
	*/

	$sib_obj = Sirius_Sib::getInstance();
	$sib_entries = $sib_obj->list($entity_node);

	$restriction_info_all = $sib_obj->restriction_info_all();

	$html .= '<h2>' . t('All Blocks') . '</h2>';

	$html .= $sib_obj->render($sib_entries);

	$html .= '<h2>' . t('Access Restrictions') . '</h2>';
	foreach ($restriction_info_all as $restriction_key => $restriction_info) {
		$html .= 'Access to: ' . $restriction_info['title'];
		$html .= "<br />";
		$ok = $sib_obj->access($restriction_key, array('entity_node' => $entity_node));
		if ($ok) {
			$html .= "Access granted";
		} else {
			$html .= '<strong>' . "Access denied: " . $sib_obj->error_msg() . '</strong>';
		}
		$html .= '<br />';
		$html .= '<br />';
	}

	return $html;
}
