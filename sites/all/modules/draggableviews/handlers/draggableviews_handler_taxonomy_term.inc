<?php

/**
 * @file
 * The taxonomy term weight handler plugin.
 */

$plugin = array(
  'label' => 'Taxonomy term',
  'handler' => array(
    'class' => 'draggableviews_handler_taxonomy_term',
  ),
);

class draggableviews_handler_taxonomy_term extends draggableviews_handler {

  public function get($field, $index) {
    $row = $field->view->result[$index];
    return $row->taxonomy_term_data_weight;
  }

  function set($form_state) {
    foreach ($form_state['values']['draggableviews'] as $item) {
      // Make sure id is available.
      if (!isset($item['id'])) {
        continue;
      }

      $term = taxonomy_term_load($item['id']);
      if ($term->weight != $item['weight']) {
        $term->weight = $item['weight'];
        taxonomy_term_save($term);
      }
    }
  }
}
