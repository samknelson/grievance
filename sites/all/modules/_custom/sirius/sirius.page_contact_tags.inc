<?php

function sirius_node_tab_contact_tags_form($form, &$form_state, $contact_node) {
	$form['#tree'] = TRUE;
	$form_state['contact_nid'] = $contact_node->nid;

	$vocabulary = taxonomy_vocabulary_machine_name_load('sirius_contact_tags');
  $terms = sirius_domain_taxonomy_get_tree($vocabulary->vid, 0, NULL, TRUE, sirius_domain_nid());

  $options = array();
  foreach ($terms as $term) {
    $tid = $term->tid;
    $name = $term->name;
    $depth = $term->depth;
    $parent = $term->parents[0];

    $field_prefix = '';
    for ($i=0; $i<$depth; ++$i) { $field_prefix .= '&nbsp;--'; }
    $options[$tid] = $field_prefix . $name;
	}

	if (!$options) { return sirius_form_error('No contact tags have been defined.'); }

	$form['tags'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Tags'),
		'#options' => $options,
		'#default_value' => sirius_contact_tags_get($contact_node),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_contact_tags_form_submit($form, &$form_state) {
	$contact_node = node_load($form_state['contact_nid']);
	$value = array_filter(array_values($form_state['values']['tags']));
	$result = sirius_contact_tags_set($contact_node, $value);
	if ($result) {
		drupal_set_message("Tags saved.");
	} else {
		drupal_set_message("Tags could not be saved: " . $result['msg']);
	}
}