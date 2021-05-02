<?php

/**
 * @todo: Eventually, this should provide a framework for deleting things which require other things to be deleted first.
 *        For now, it's just a stub. And uncalled.
 */

function sirius_delete_cascades() {
	return array(
		'grievance' => array(
			'field_grievance' => array(
				'grievance_log', 'grievance_attachment', 'grievance_settlement'
			),
		),
		'grievance_contract_template' => array(
			'field_grievance_contract_tplt' => array(
				'grievance_contract_clause', 'grievance_contract_rule', 'grievance_contract_section'
			),
		),
		'grievance_contract_section' => array(
			'field_grievance_contract_section' => array(
				'grievance_contract_clause'
			),
		),
		'sirius_contact' => array(
			'field_sirius_contact' => array(
				'sirius_event_participant'
			),
		),
	);
}