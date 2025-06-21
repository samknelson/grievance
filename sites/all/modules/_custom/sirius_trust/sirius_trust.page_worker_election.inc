<?php

function sirius_trust_node_tab_worker_election_access($worker_node) {
  return Sirius_Trust_Access::getInstance()->access('worker_election_landing', array('worker_node' => $worker_node));
}

function sirius_trust_node_tab_worker_election($worker_node) {
  $election_node = sirius_trust_worker_election($worker_node);

  $html .= '<h2>' . t('Current Election') . '</h2>';
  if (!$election_node) {
     $html .= '<p>' . t('There is no current active election.') . '</p>';
  } else {
    $renderable = node_view($election_node, 'full');
    $html .= drupal_render($renderable);
  }

  return $html;
}
