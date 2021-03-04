<?php


function sirius_ledger_page_delinquency_worker() { 
	return 'Generate worker delinquency letters.';
}

function sirius_ledger_page_delinquency_employer() { 
	return drupal_get_form('sirius_ledger_page_delinquency_employer_form');
}

function sirius_ledger_page_delinquency_employer_form($form, &$form_state) {
	$form['format'] = array(
		'#type' => 'select',
		'#title' => t('Format'),
		'#options' => array(
			NULL => t('-- Select --'),
			'html' => t('Web'),
			'pdf' => t('PDF'),
		),
		'#required' => TRUE,
	);

	$accounts = array(NULL => t('-- Select --')) + sirius_ledger_account_list();
	$form['account_nid'] = array(
		'#type' => 'select',
		'#title' => t('Account'),
		'#checkall' => TRUE,
		'#options' => $accounts,
	);

	$letter_templates = sirius_letter_template_list('grievance_shop');
	if (!$letter_templates) { drupal_set_message("You do not have access to any letter templates for this content type"); }
	$letter_options = array(NULL => t('-- Select --'));
	if ($letter_templates) {
		foreach ($letter_templates as $letter_template_nid => $letter_template_node) {
			$letter_options[$letter_template_nid] = $letter_template_node->title;
		}
	}
	$form["letter_template_nid_$i"] = array(
		'#type' => 'select',
		'#title' => t('Letter Template'),
		'#options' => $letter_options,
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if ($form_state['results']) {
		$form['results'] = array('#markup' => $form_state['results']);
	}

	return $form;
}

function sirius_ledger_page_delinquency_employer_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$account_nid = $form_state['values']['account_nid'];
	$format = $form_state['values']['format'];

	$all_employer_nids = sirius_employer_list();
	$employer_nids = array();
	foreach ($all_employer_nids as $employer_nid => $employer_name) {
		$balance = sirius_ledger_balance_fetch($employer_nid, $account_nid);
		if ($balance <= 0) { continue; }
		$employer_nids[] = $employer_nid;
	}

	if (!$employer_nids) {
		drupal_set_message("No records have a positive balance in the selected account.");
		return;
	}

	dpm($employer_nids);
	return;
	$html = '';
	$is_first = TRUE;
	foreach ($employer_nodes as $employer_nid => $employer_node) {
		if ($employer_node->field_sirius_domain['und'][0]['target_id'] != $domain_nid) {
			return 'You do not have access to one of the specified employers (wrong domain).';
		}
		if ($is_first) {
			$is_first = FALSE;
			$html .= '<div style="margin: 0; padding: 0;" class="sirius_ledger_transmittal_wrapper">';
		} else {
			$html .= '<div style="page-break-before: always; margin: 0; padding: 0;" class="sirius_ledger_transmittal_wrapper">';
		}
		$html .= sirius_ledger_node_tab_employer_transmittal_print($employer_node, $account_nids, $form_state['values']['hours']);
		$html .= '</div>';
	}

	if ($format == 'html') {
		$form_state['results'] = $html;
		return;
	} 

	$params = array();
	$params['post']['body'] = $html;
	$params['post']['css_paths'] = 'sites/all/modules/_custom/sirius_ledger/css/transmittal.css';
	$params['letterhead'] = sirius_domain_variable_get('sirius_ledger_transmittal_letterhead', '');
	$params['filename'] = 'employer_delinquency_batch_' . date('Ymd') . '.pdf';
	sirius_pdf_stream(sirius_loopback_url() . '/sirius/loopback/echo', $params);
}
