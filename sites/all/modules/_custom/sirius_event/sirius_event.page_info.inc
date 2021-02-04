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

  $renderable = node_view($event_node, 'full');
  $html .= drupal_render($renderable);

  if ($participant_node) {
    $pstatus_term = taxonomy_term_load($participant_node->field_sirius_event_pstatus['und'][0]['tid']);
    $pstatus_json = sirius_jsonfield_util_term_json_get($pstatus_term);
    $pstatus_options = sirius_event_participant_mypstatus_options($pstatus_json);
    if ($pstatus_options) {
      $html .= '<hr>';
      $html .= '<h2>' . t('Change Registration Status') . '</h2>';
      $html .= drupal_render(drupal_get_form('sirius_node_tab_event_mypstatus_form', $event_node, $participant_node, $pstatus_options));
    }
  }

  return $html;
}

function sirius_node_tab_event_mypstatus_form($form, &$form_state, $event_node, $participant_node, $pstatus_options) {
  $form_state['event_nid'] = $event_node->nid;
  $form_state['participant_nid'] = $participant_node->nid;

  $form['#tree'] = TRUE;

  $form['pstatus_tid'] = array(
    '#type' => 'select',
    '#title' => 'Change registration status to:',
    '#options' => array(NULL => t('-- Select --')) + $pstatus_options,
  );

  $participant_json = sirius_jsonfield_util_node_json_get($participant_node);
  $event_json = sirius_jsonfield_util_node_json_get($event_node);

  $form['#tree'] = TRUE;

  $counts = sirius_event_section_counts($event_node);
  $options_attributes = array();
  $options = sirius_event_section_options($event_node, $counts, $options_attributes);
  if ($options && !$participant_json['section']['uuid']) {
    unset($options_attributes[$participant_json['section']['uuid']]);
    $form['section']['uuid'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#title' => t('Section'),
      '#default_value' => $participant_json['section']['uuid'],
      '#options_attributes' => $options_attributes,
    );

    foreach ($options as $uuid => $title) {
      $section = $event_json['section_list'][$uuid];
      if (!$section || !$section['comments']) { continue; }
      $form['section'][$uuid]['comments'] = array(
        '#type' => 'item',
        '#markup' => '<strong>' . sirius_event_section_title($section) . ':</strong> ' . $section['comments'],
        '#states' => array('visible' => array(":input[name=\"section[uuid]\"]" => array('value' => $uuid))),
      );
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function sirius_node_tab_event_mypstatus_form_submit($form, &$form_state) {
  $participant_node = node_load($form_state['participant_nid']);
  $participant_nid = $participant_node->nid;
  $pstatus_tid = $form_state['values']['pstatus_tid'];
  if ($pstatus_tid) {
    $prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
    $result = sirius_event_participant_pstatus_set($participant_node, $prole_tid, $pstatus_tid);
    if ($result['success']) {
      drupal_set_message("Registration status changed.");
    } else {
      drupal_set_message("Unable to change registration status: " . $result['msg'], 'warning');
    }
  }

  $uuid = $form_state['values']['section']['uuid'];
  if ($uuid) {
    $result = sirius_event_section_assign($participant_nid, $uuid);
    if ($result['success']) {
      drupal_set_message($result['msg']);
    } else {
      drupal_set_message($result['msg'], 'warning');
    }
  }
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