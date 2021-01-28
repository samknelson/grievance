<?php

function sirius_node_tab_event_info_access($event_node) {
  // Admins can create
  if (user_access('sirius event admin')) { return TRUE; }

  // Check our creation rules
  $event_type_tid = $event_node->field_sirius_event_type['und'][0]['tid'];
  $event_type_term = taxonomy_term_load($event_type_tid);
  $json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
  $rule = sirius_event_paccess_rule_match($event_node->nid, $json['view']['rules']);
  if ($rule) { return TRUE; }

  // Oh well
  return FALSE;
}

function sirius_node_tab_event_info($event_node) {
  sirius_event_disclaimer_redirect($event_node);
  if (sirius_node_tab_event_bullpen_view_access($event_node)) {
    return sirius_node_tab_event_bullpen_view($event_node);
  }

  $renderable = node_view($event_node, 'full');

  $participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
  $participant_node = node_load($participant_nid);
  if ($participant_node) {
    $html .= sirius_dispatch_view_render_field($participant_node, 'field_sirius_event_prole', t('Your Role'));
    $html .= sirius_dispatch_view_render_field($participant_node, 'field_sirius_event_pstatus', t('Your Status'));

    $section = sirius_event_section_get_from_participant($event_node, $participant_node);
    if ($section) {
      $section_title = sirius_event_section_title($section);
      if ($section['comments']) { $section_title .= '<br />' . $section['comments']; }
      $html .= sirius_fakerender_field('Section: ', $section_title);
    }

    $html .= '<br />';
  }

  $html .= drupal_render($renderable);

  return $html;
}


/**
 * @todo: This should be in a different file
 */

function sirius_node_tab_event_exim($event_node) {
  return drupal_get_form('sirius_json_definition_exim_form', $event_node->nid, 'ALL');
}

function sirius_node_tab_event_clone($event_node) {
  return drupal_get_form('sirius_node_tab_event_clone_form', $event_node);
}

function sirius_node_tab_event_clone_form($form, &$form_state, $event_node) {
  $form_state['event_nid'] = $event_node->nid;

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => 'Event Title',
    '#default_value' => $event_node->title . ' COPY',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function sirius_node_tab_event_clone_form_submit($form, &$form_state) {
  $event_node = node_load($form_state['event_nid']);

  foreach ($event_node as $key => $value) {
    // Keep all the fields
    if (preg_match('/^field_/', $key)) { continue; }
    if (in_array($key, array('uid', 'body', 'status', 'type', 'language'))) { continue; }
    unset($event_node->{$key});
  }
  $event_node->title = $form_state['values']['title'];
  node_object_prepare($event_node);
  $event_node = node_submit($event_node);
  node_save($event_node);

  drupal_set_message("Success! New event created: " . l($event_node->title, 'node/' . $event_node->nid));
}