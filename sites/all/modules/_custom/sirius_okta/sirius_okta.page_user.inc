<?php

function sirius_okta_page_user($account) {
	return drupal_get_form('sirius_okta_page_user_form', $account);
}

function sirius_okta_page_user_form($form, &$form_state, $account_uid) {
	$form_state['account_uid'] = $account_uid;
	$form['#tree'] = TRUE;

	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');

	$okta = Sirius_Okta::getInstance();

	$result = SiriusResult::create($okta->user_fetch($account_uid));

	$html = '<div class="sirius_callout_basic">';
	$html .= '<h2>' . t('Okta User Data') . '</h2>';
	if ($result->success()) {
		$data = $result->get('data');

		$id = $data['id'];

		$render = $id;
		if ($id && variable_get('sirius_okta_domain_admin')) {
			$url = 'https://' . variable_get('sirius_okta_domain_admin') . '/admin/user/profile/view/' . $data['id'];
			$render .= "&nbsp;&nbsp;<a href=\"$url\" target=\"_blank\">" .'<i class="fas fa-user-lock"></i>' . t('View in Okta') . '</a>';
		}
		$html .= sirius_fakerender_field("Okta ID:", $render);
		$html .= sirius_fakerender_field("Login:", $data['profile']['login']);
		$html .= sirius_fakerender_field("Status:", $data['status']);
		$html .= sirius_fakerender_field("Created:", $data['created']);
		$html .= sirius_fakerender_field("Activated:", $data['activated']);
		$html .= sirius_fakerender_field("Last Login:", $data['lastLogin']);
	} else {
		$html .= "No okta user found: <em>" . $result->msg() . '</em>';
	}
	$html .= '</div>';
	$form['intro'] = array('#markup' => $html);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Sync to Okta'),
	);

	return $form;
}

function sirius_okta_page_user_form_submit($form, &$form_state) {
	$account_uid = $form_state['account_uid'];

	$okta = Sirius_Okta::getInstance();

	$result = SiriusResult::create($okta->user_sync($account_uid));
	$result->drupalSetMessage();
}