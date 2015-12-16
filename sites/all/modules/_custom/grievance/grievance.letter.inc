<?php

/*














function unite_jobs_node_tab_job_rejection_letter($node) {
	$letter_nid = $_REQUEST["letter_nid"];
	if (!$letter_nid) {
		return views_embed_view('job_rejection_letters', 'block_1');
	}
	
	$letter_node = node_load($letter_nid);
	
	return drupal_get_form('unite_jobs_rejection_letter_form', $node, $letter_node);
}

function unite_jobs_rejection_letter_form($form, &$form_state, $application_node, $letter_node) {
	$form_state['application_node'] = $application_node;
	$form_state['letter_node'] = $letter_node;
	
	$default_subject = token_replace($letter_node->field_job_subject['und'][0]['value'], array('node' => $application_node));

  $form['subject'] = array(
    '#title' => t('Subject'),
    '#type' => 'textfield',
		'#default_value' => $default_subject,
   );

	$default_body = token_replace($letter_node->field_job_body['und'][0]['value'], array('node' => $application_node));

  $form['body'] = array(
    '#title' => t('Body'),
    '#type' => 'textarea',
		'#rows' => 20,
		'#default_value' => $default_body,
   );

  $options = taxonomy_allowed_values(field_info_field('field_job_status'));
  $form['job_status'] = array(
		'#title' => t('Status'),
		'#description' => t('Change the application status at the same time as the letter is sent.'),
		'#type' => 'select',
		'#options' => $options,
		'#default_value' => $application_node->field_job_status['und'][0]['tid'],
	);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Send Rejection Letter',
  );
	
	return $form;
}

function unite_jobs_rejection_letter_form_submit($form, &$form_state) {
	global $user;

	$subject = $form_state['values']['subject'];
	$body = $form_state['values']['body'];
	$job_status = $form_state['values']['job_status'];

	$application_node = $form_state['application_node'];
	$letter_node = $form_state['letter_node'];

	$comment = (object) array(
		'nid' => $application_node->nid,
		'cid' => 0,
		'pid' => 0,
		'uid' => 1,
		'mail' => '',
		'is_anonymous' => 0,
		'homepage' => '',
		'status' => COMMENT_PUBLISHED,
		'subject' => t('Letter Sent: ') . $subject,
		'language' => LANGUAGE_NONE,
		'comment_body' => array(
		  LANGUAGE_NONE => array(
		    0 => array (
		      'value' => t('Letter sent: ') . $subject . '</strong>' . "\n\n" . $body . '</pre>',
		      'format' => 'plain_text'
		    )
		  )
		),
	);
	comment_submit($comment);
	comment_save($comment);

	if ($job_status != $application_node->field_job_status['und'][0]['tid']) {
		$application_node->field_job_status['und'][0]['tid'] = $job_status;
		node_save($application_node);
		$job_status_term = taxonomy_term_load($job_status);
		drupal_set_message(t("Set job status to: ") . $job_status_term->name);
	}

	$params = array();
	$params['body'] = $body;
	$params['subject'] = $subject;

	drupal_mail(
		'unite_jobs', 
		'rejection_letter', 
		$application_node->field_job_email['und'][0]['email'], 
		LANGUAGE_NONE, 
		$params
	);
	
	drupal_set_message("Sent rejection letter: $subject");
}

*/