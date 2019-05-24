<?php

function sirius_node_tab_worker_dispatches_access($worker_node) { return node_access('view', $worker_node); }
function sirius_node_tab_worker_dispatches_current_access($worker_node) { return node_access('view', $worker_node); }
function sirius_node_tab_worker_dispatches_archive_access($worker_node) { return node_access('view', $worker_node); }
function sirius_node_tab_worker_dispatches_all_access($worker_node) { return node_access('view', $worker_node); }

function sirius_node_tab_worker_dispatches($worker_node) { drupal_goto('node/' . $worker_node->nid . '/sirius_worker_dispatches/status'); }
function sirius_node_tab_worker_dispatches_current($worker_node) { return views_embed_view('sirius_worker_dispatches', 'block_1', $worker_node->nid); }
function sirius_node_tab_worker_dispatches_archive($worker_node) { return views_embed_view('sirius_worker_dispatches', 'block_2', $worker_node->nid); }
function sirius_node_tab_worker_dispatches_all($worker_node) { return views_embed_view('sirius_worker_dispatches', 'block', $worker_node->nid); }
