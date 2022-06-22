<?php

function sirius_trust_node_tab_worker_election($worker_node) {
  $election_node = sirius_trust_worker_election($worker_node);
  if (!$election_node) {
    return '<p>' . t('There is no current active election.') . '</p>';
  }
  $renderable = node_view($election_node, 'full');
  return drupal_render($renderable);
}
