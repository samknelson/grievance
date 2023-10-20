<?php

function sirius_trust_node_tab_worker_election($worker_node) {
  $result = sirius_proxy_call('sirius_trust_worker_election', $worker_node);
  if (!$result['success']) { return sirius_page_error($result['msg']); }
  $election_rnode = $result['data'];

  $html = '<h2>' . t('Current Election') . '</h2>';
  if (!$election_rnode) {
     $html .= '<p>' . t('There is no current active election.') . '</p>';
  } else {
    $renderable = node_view($election_rnode, 'full');
    $html .= drupal_render($renderable);
  }

  return $html;
}
