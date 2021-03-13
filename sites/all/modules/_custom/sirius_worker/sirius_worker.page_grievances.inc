<?php

function sirius_node_tab_worker_grievances($worker_node) {
  return views_embed_view('sirius_worker_grievances', 'page_1', $worker_node->field_sirius_id['und'][0]['value']);
}