<?php

function sirius_node_tab_entity_contacts_add_existing($entity_node) {
	return drupal_get_form('sirius_node_tab_entity_contacts_add_existing_form', $entity_node);
}

function sirius_node_tab_entity_contacts_add_existing_form($form, &$form_state, $entity_node) {
  drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/event.css', 'file');
  form_load_include($form_state, 'inc', 'sirius', 'sirius.page_entity_contacts_add_existing');

	$form_state['entity_nid'] = $entity_node->nid;

	$form['#tree'] = TRUE;

  $form['table_open'] = array('#markup' => '<table class="columns_table">');
  $form['col1_open'] = array('#markup' => '<tr><td style="width: 66%; vertical-align: top;">');

  $contact_types = sirius_taxonomy_load('grievance_contact_types', 'tid', 'name');
  if ($contact_types) {
	  $form['type'] = array(
	  	'#type' => 'checkboxes',
	  	'#title' => 'Contact Type',
	  	'#options' => $contact_types,
	  );
	}

  $form['contact'] = array(
    '#title' => 'Contact',
    '#type' => 'textfield',
    '#autocomplete_path' => 'sirius/ajax/contact-autocomplete',
    '#ajax' => array(
      'callback' => 'sirius_ajax_generic_replace',
    ),
    '#sirius_ajax' => array(
      'wrapper' => '#sirius_contact_preview',
      'element' => 'contact_preview',
      'files' => 'sirius.page_entity_contacts_add_existing.inc',
      'module' => 'sirius',
    ),
    '#description' => t("Search by contact name"),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  $form['col1_close'] = array('#markup' => '</td>');
  $form['col2_open'] = array('#markup' => '<td style="width: 33%; vertical-align: top;">');

  $contact_str = $form_state['input']['contact'];
  if ($contact_str) {
    $result = sirius_node_tab_entity_contacts_add_existing_lookup($contact_str);
    $preview = '<h2>' . t('Contact Selected') . '</h2>';
    if (!$result['success']) {
      $preview .= '<em>' . $result['msg'] . '</em>';
    } else {
      $preview .= sirius_dispatch_view_render_field($result['contact_node'], 'field_sirius_name', 'Name');
      $preview .= sirius_dispatch_view_render_field($result['contact_node'], 'field_sirius_phone');
      $preview .= sirius_dispatch_view_render_field($result['contact_node'], 'field_sirius_email');
      $preview .= sirius_dispatch_view_render_field($result['contact_node'], 'field_sirius_address');

      $path = 'node/' . $result['contact_nid'];
      if (drupal_valid_path($path)) {
        $preview .= l('View Contact', $path, array('attributes' => array('class' => array('tool-button'))));
      }
    }
  }

  $form['contact_preview']['prefix'] = array('#markup' => '<div id="sirius_contact_preview">');
  $form['contact_preview']['body']  = array('#markup' => $preview);
  $form['contact_preview']['suffix'] = array('#markup' => '</div>');
 
  $form['col2_close'] = array('#markup' => '</td></tr></table>');

	return $form;
}

function sirius_node_tab_entity_contacts_add_existing_form_submit($form, &$form_state) {
  $entity_node = node_load($form_state['entity_nid']);
  $result = sirius_node_tab_entity_contacts_add_existing_lookup($form_state['values']['contact']);
  if (!$result['success']) {
    $form_state['rebuild'] = TRUE;
    drupal_set_message($result['msg'], 'error');
    return;
  }
  $contact_nid = $result['contact_nid'];

  $type_tids = array();
  if ($form_state['values']['type']) {
    $type_tids = array_filter(array_values($form_state['values']['type']));
  }
  sirius_contact_entity_add($entity_node, $contact_nid, $type_tids);
  drupal_set_message("Contact added.");
}

function sirius_node_tab_entity_contacts_add_existing_lookup($contact_str) {
  if (!$contact_str) { return array('success' => FALSE, 'msg' => 'Please enter the name of a contact.'); }
  $matches = array();
  if (preg_match('/\s+\((\d+)\)/', $contact_str, $matches) && $matches[1]) { 
    $contact_nid = $matches[1];
  }
  if (!$contact_nid) { return array('success' => FALSE, 'msg' =>'Please make sure that you have selected a matching contact.'); }

  $contact_node = node_load($contact_nid);
  if ($contact_node->type != 'sirius_contact') { return array('success' => FALSE, 'msg' => 'That is not a contact.'); }

  $domain_nid = sirius_domain_nid();
  if ($domain_nid != $contact_node->field_sirius_domain['und'][0]['target_id']) { return array('success' => FALSE, 'msg' => 'That contact is not in the current domain.'); }

  return array(
    'success' => TRUE,
    'contact_nid' => $contact_nid,
    'contact_node' => $contact_node,
  );
}

