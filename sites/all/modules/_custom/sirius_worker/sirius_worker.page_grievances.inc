<?php

function sirius_node_tab_worker_grievances($worker_node) {
  $worker_id = $worker_node->field_sirius_id['und'][0]['value'];
  if (!$worker_id) { return sirius_page_error("This worker has no ID."); }
  return views_embed_view('sirius_worker_grievances', 'page_1', $worker_id);
}