<?php

function sirius_node_tab_worker_user($worker_node) {
	return drupal_get_form('sirius_worker_user_create_form', $worker_node);
}

function sirius_worker_user_create_form($form, &$form_state, $worker_node) {
	$worker_user = sirius_worker_get_user($worker_node->nid);
	if ($worker_user) {

  	$html .= t('This worker is currently associated with: ');
  	$html .= '<strong>';
		$html .= 'User #' . $worker_user->uid . ' - ' . $worker_user->name . ' (' . $worker_user->mail . ')';
  	$html .= '</strong>';
  	$html .= l('Edit', 'user/' . $worker_user->uid . '/edit', ['attributes' => ['class' => ['tool-button']]]);
	} else {
		$html = 'There is no user currently associated with this worker.';
	}

	$form['intro'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="sirius_form_intro">' . $html . '</div>'
	);

	if (sirius_worker_usersync_enabled()) {
		$form_state['worker_node'] = $worker_node;

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Sync Now'),
		);
	}

	return $form;
}

function sirius_worker_user_create_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$worker_node = $form_state['worker_node'];
	$result = sirius_worker_usersync($worker_node->nid);
	if ($result['success']) {
		drupal_set_message("Sync complete: " . $result['msg']);
	} else {
		drupal_set_message("Sync failed: " . $result['msg'], 'error');
	}
}