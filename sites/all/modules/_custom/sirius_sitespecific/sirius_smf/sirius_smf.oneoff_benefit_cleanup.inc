<?php

function sirius_smf_node_tab_worker_oneoff_benefit_cleanup_form($form, &$form_state, $worker_node) {
  $form_state['worker_nid'] = $worker_node->nid;

  $form['#tree'] = TRUE;

  $form['mode'] = array(
    '#title' => t('Mode'),
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => array(
      'test' => t('Test'),
      'live' => t('Live'),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  $log = sirius_minilog_render_html();
  if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

  return $form;
}

function sirius_smf_node_tab_worker_oneoff_benefit_cleanup_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $worker_nid = $form_state['worker_nid'];
  sirius_smf_oneoff_benefit_cleanup_impl($worker_nid, $form_state['values']['mode']);
}

// while true ; do drush --user=1 php-eval "sirius_require('sirius_smf', 'sirius_smf.oneoff_benefit_cleanup.inc'); sirius_minilog_echo_active(TRUE); sirius_smf_oneoff_benefit_cleanup();" ; sleep 10; done

// drush --user=1 php-eval "sirius_require('sirius_smf', 'sirius_smf.oneoff_benefit_cleanup.inc'); sirius_smf_oneoff_benefit_cleanup();"

function sirius_smf_oneoff_benefit_cleanup() {
  sirius_domain(2457501);
  sirius_minilog_echo_active(TRUE);

	$worker_nid = 2598700;
	$result = sirius_smf_oneoff_benefit_cleanup_impl($worker_nid);
	sirius_minilog("Result: $result[success] / $result[msg]");
}

function sirius_smf_oneoff_benefit_cleanup_impl($worker_nid, $mode = 'test') {
	$worker_node = node_load($worker_nid);

  $raw_nodes = sirius_trust_worker_benefits_search(array('worker_nid' => $worker_nid));

  if (!$raw_nodes) { return array('success' => TRUE, 'msg' => 'No benefit nodes.'); }

  // Break out by benfit; make sure every node has a start date
  $wb_nodes = array();
  foreach ($raw_nodes as $wb_nid => $wb_node) {
    $date = $wb_node->field_sirius_date_start['und'][0]['value'];
    if (!$date) { return array('success' => FALSE, 'msg' => 'This worker has a prior worker benefit with no start date.'); }
    $benefit_nid = $wb_node->field_sirius_trust_benefit['und'][0]['target_id'];
    if (!$benefit_nid) { continue; }

    $wb_nodes[$benefit_nid][$date] = $wb_node;
  }

  // For each benefit, merge adjacent nodes if possible.
  foreach ($wb_nodes as $benefit_nid => $ignore) {
  	sirius_minilog("... Scanning benefit $benefit_nid");
    ksort($wb_nodes[$benefit_nid]);
    $prev_wb_node = NULL;
    foreach ($wb_nodes[$benefit_nid] as $date => $wb_node) {
      // Can we merge wb_node into prev_wb_ndoe?
      $can_merge = TRUE; 
      sirius_minilog("... ... Can we merge $date / " . $wb_node->nid . " with " . $prev_wb_node->field_sirius_date_end['und'][0]['value'] . " / " . $prev_wb_node->nid . "?");

      // Only if there's a previous one
      if (!$prev_wb_node) {
        $can_merge = FALSE; 
        sirius_minilog("... ... No previous node.");
      }

      // Only if they have the same employer
      else if ($wb_node->field_grievance_shop['und'][0]['target_id'] != $prev_wb_node->field_grievance_shop['und'][0]['target_id']) {
        $can_merge = FALSE; 
        sirius_minilog("... ... Different employers (" . $wb_node->field_grievance_shop['und'][0]['target_id'] . " and " . $prev_wb_node->field_grievance_shop['und'][0]['target_id'] . ")");
      }

      // Only if the previous one ended just a day or so ago
      else if ($prev_wb_node->field_sirius_date_end['und'][0]['value'] < date('Y-m-d 00:00:00', strtotime('-5 days', strtotime($date)))) {
        $can_merge = FALSE; 
        sirius_minilog("... ... Date gap");
      }

      if (!$can_merge) {
        $prev_wb_node = $wb_node;
        continue;
      }

      if ($can_merge) {
        sirius_minilog('... ... Yes, we can!');
        // Set the prevous node's end date to our end date
        $prev_date = $prev_wb_node->field_sirius_date_start['und'][0]['value'];

        $wb_nodes[$benefit_nid][$prev_date]->field_sirius_date_end['und'][0]['value'] = max(
          $wb_nodes[$benefit_nid][$prev_date]->field_sirius_date_end['und'][0]['value'],
          $wb_nodes[$benefit_nid][$date]->field_sirius_date_end['und'][0]['value']
        );
        $wb_nodes[$benefit_nid][$prev_date]->needs_save = TRUE;

        sirius_minilog(
        	"... ... Merging benefit nodes $prev_date = " . 
        	l($wb_nodes[$benefit_nid][$prev_date]->nid, "node/" . $wb_nodes[$benefit_nid][$prev_date]->nid) . 
        	" and $date = " . 
        	l($wb_nodes[$benefit_nid][$date]->nid, "node/" . $wb_nodes[$benefit_nid][$date]->nid)
        );

        if ($mode == 'live') {
          node_save($wb_nodes[$benefit_nid][$prev_date]);
          node_delete($wb_nodes[$benefit_nid][$date]->nid);
        } else {
          sirius_minilog("... ... (Skipping save, because mode = $mode)");
        }
      } else {
        $prev_wb_node = $wb_node;
      }
    }
  }

  return array('success' => TRUE);
}
