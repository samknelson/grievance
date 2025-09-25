<?php

class Sirius_Revision_Log extends Sirius_Singleton {
  public function loggables() {
    // @todo: This should be info() callbacks 
    return array(
      'sirius_worker' => array(
        'field_sirius_bu' => array(
          'category' => 'worker:bu',
        ),
        'field_sirius_title' => array(
          'category' => 'worker:title',
        ),
      ),
    );
  }

  public function handle_update($node) {
    if (!$node->original) { return; }

    $loggables = $this->loggables();
    if (!$loggables[$node->type]) { return array('success' => TRUE); }

    foreach ($loggables[$node->type] as $field_name => $field_info) {
      $orig_val = $node->original->{$field_name};
      if (!$orig_val) { $orig_val = array(); }

      $new_val = $node->{$field_name};
      if (!$new_val) { $new_val = array(); }

      $tmp = sirius_revision_changed_impl($orig_val, $new_val, 1, $field_name);
      if (!$tmp) { $tmp = sirius_revision_changed_impl($new_val, $orig_val, 1); }
      if (!$tmp) { continue; }

      $orig_render = $this->_util_render_field($node->original, $field_name);
      $new_render = $this->_util_render_field($node, $field_name);

      $args = array();
      $args['category'] = $field_info['category'];
      $args['type'] = 'update';
      $args['msg'] = "Changed from $orig_render to $new_render";
      $args['nid'] = $node->nid;
      sirius_denorm_queue_add($node->nid, $node->nid, 'sirius_revision_log_denorm', $args);
    }

    return array('success' => TRUE);
  }

  public function denorm($denorm_item) {
    $args = $denorm_item['denorm_trigger_args'];
    if ($args) { $args = json_decode($args, TRUE); }
    if (!$args) { $args = array(); }
    if (!$args['nid'] || !$args['type'] || !$args['category'] || !$args['msg']) { return array('success' => TRUE); }

    sirius_log($args['category'], $args['type'], $args['msg'], '', '', array($args['nid']), array());

    return array('success' => TRUE);
  }

  public function _util_render_field($node, $field_name) {
    // There's no fucking way to render the new value for a field in a hook_update, because 
    // the function doesn't set the safe_value until the record is about to be saved. So
    // safe_value still shows the original.
    if ($node->{$field_name}['und'][0]['value'] != $node->{$field_name}['und'][0]['safe_value']) {
      $node->{$field_name}['und'][0]['safe_value'] = $node->{$field_name}['und'][0]['value'];
    }

    $mode = 'full';
    $renderable = field_view_field('node', $node, $field_name, 'full');
    $renderable['#label_display'] = 'hidden';
    $result = strip_tags(render($renderable));
    if (!$result) { $result = '[no value]'; }
    return $result;
  }

}