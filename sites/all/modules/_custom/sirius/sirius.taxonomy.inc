<?php

function sirius_taxonomy_checkboxes(&$form, &$form_state, $field_name, $vocabulary_name) {
  $vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary_name);
  $terms = sirius_domain_taxonomy_get_tree($vocabulary->vid, 0, NULL, TRUE, sirius_domain_nid());

  foreach ($terms as $term) {
    $tid = $term->tid;
    $name = $term->name;
    $depth = $term->depth;
    $parent = $term->parents[0];

    $field_prefix = '';
    for ($i=0; $i<$depth; ++$i) {
      $field_prefix .= '&nbsp;--';
    }
    $states = NULL;
    if ($parent) {
      $states = array(
        'visible' => array(
          array(':input[name="' . $field_name . '_checkbox_' . $parent . '"]' => array('checked' => TRUE)),
        )
      );
    }

    $form["${field_name}_checkbox_{$tid}"] = array(
      '#type' => 'checkbox',
      '#value' => $tid,
      '#title' => $name,
      '#field_prefix' => $field_prefix,
      '#states' => $states,
    );
  }
}


/**
 * A stupid utility function to create dropdowns and mappings from taxonomies.
 * Key can be:
 * - 'tid' to create a mapping by tid
 * - 'name' to create a mapping by name
 * Value can be:
 * - 'full' to return the fully loaded term
 * - 'name' to return the term name
 * - field_whatever to return the value of "field_whatever"
 */

function sirius_taxonomy_load($vocabulary_name, $key_type, $value_type, $include_null = FALSE) {
  $domain_nid = sirius_domain_nid();
  $vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary_name);
  if ($value_type == 'name') { $load_entities = FALSE; } else { $load_entities = TRUE; }
  $terms = sirius_domain_taxonomy_get_tree($vocabulary->vid, 0, NULL, $load_entities, sirius_domain_nid());

  $map = array();
  if ($include_null) {
    $map[NULL] = t('-- Select --');
  }
  
  foreach ($terms as &$term) {
    if ($value_type == 'full') {
      $value = $term;
    } else if ($value_type == 'name') {
      $value = $term->name;
    } else {
      $value = $term->{$value_type}['und'][0]['value'];
    }

    if ($key_type == 'tid') {
      $key = $term->tid;
    } else if ($key_type == 'field_sirius_id') {
      $key = $term->field_sirius_id['und'][0]['value'];
    } else if ($key_type == 'field_sirius_id2') {
      $key = $term->field_sirius_id2['und'][0]['value'];
    } else {
      $key = $term->name;
    }

    $map[$key] = $value;
  }
  return $map;
}

function sirius_taxonomy_lookup_by_anything($vocabulary_name, $value) {
  static $preload;

  if (!$preload[$vocabulary_name]) {
    $tmp = sirius_taxonomy_load($vocabulary_name, 'tid', 'full');
    foreach ($tmp as $tid => $term) {
      $keys = array();
      $keys[] = trim(strtolower($term->name));
      $keys[] = trim(strtolower($term->field_sirius_name_short['und'][0]['value']));
      $keys[] = trim(strtolower($term->field_sirius_id['und'][0]['value']));
      $keys[] = trim(strtolower($term->field_sirius_id2['und'][0]['value']));
      $keys[] = trim(strtolower($term->field_sirius_id3['und'][0]['value']));
      $keys[] = $term->tid;
      foreach ($keys as $key) {
        if ($key) {
          $preload[$vocabulary_name][$key] = $tid;
        }
      }
    }
  }

  return $preload[$vocabulary_name][trim(strtolower($value))];
}
