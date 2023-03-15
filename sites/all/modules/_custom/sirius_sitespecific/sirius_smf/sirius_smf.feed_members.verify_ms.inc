<?php 

function sirius_smf_feed_members_ms_lookup_by_industry($industry_tid) {
  $lookup = &drupal_static(__FUNCTION__);
  if (!isset($lookup)) {
    $ms_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');

    $lookup = array();
    foreach ($ms_terms as $ms_tid => $ms_term) {
      $industry_tid = $ms_term->field_sirius_industry['und'][0]['tid'];
      if ($industry_tid) {
        $lookup[$industry_tid] = $ms_tid;
      }
    }
  }
  
  return $lookup[$industry_tid];
}


function sirius_smf_feed_members_row_verify_ms($feed_node, $info, $data, &$row, $fields) {
  // return array('success' => TRUE);

  // Global variables
  $domain_nid = sirius_domain_nid();
  global $user;

  // $row['wb_nodes'] = array();

  // Get worker's coverage records
  /*
  if ($row['worker_nid']) {
    $wb_nodes = sirius_trust_worker_benefits_search(array('worker_nid' => $row['worker_nid']));
    if ($wb_nodes) {
      foreach ($wb_nodes as $wb_nid => $wb_node) {
        $date = $wb_node->field_sirius_date_start['und'][0]['value'];
        if (!$date) { return array('success' => FALSE, 'msg' => 'This worker has a prior worker benefit with no start date.'); }
        // if ($row['elections'][$date]) { return array('success' => FALSE, 'msg' => 'This worker has multiple prior elections with the same start date.'); }
        // $row['elections'][$date] = $election_node;

        $benefit_nid = $wb_node->field_sirius_trust_benefit['und'][0]['target_id'];
        if (!$benefit_nid) { continue; }
        $row['wb_nodes'][$benefit_nid][$date] = $wb_node;
      }
    }
  }
  */

  $ssn = $row['ssn'];

  $ms_assignments = array();
  for ($coverage_month = 1; $coverage_month <= 16; ++$coverage_month) {
    // Load the coverage record
    $coverage_record = sirius_smf_feed_members_coverage_record($fields, $row, $coverage_month);
    if (!$coverage_record['10 SMCEE-CWELCODE'] && !$coverage_record['10 SMCEE-CDENCODE']) { continue; }

    // Start date
    $year = substr($coverage_record['10 SMCEE-CELIGDATE'], 0, 4);
    $month = substr($coverage_record['10 SMCEE-CELIGDATE'], 4, 2);
    $day = '01';
    if (!$year || $year <= 1900 || !$month || $month < 1 || $month > 12) { return array('success' => FALSE, 'msg' => "For coverage month $coverage_month, invalid date."); }

    $employer_nid = NULL;
    if ($coverage_record['10 SMCEE-CEMPR']) {
      $employer_id_from = $coverage_record['10 SMCEE-CEMPR'];
      $employer_record = sirius_smf_feed_members_employer_mapping($employer_id_from);
      if (!isset($employer_record)) {
        return array('success' => FALSE, 'msg' => "For coverage month $coverage_month, don't know which employer ID corresponds to the incoming ID $employer_id_from");
      }
      $employer_id_to = $employer_record['employer_id'];
      if ($employer_id_to) {
        $employer_nid = sirius_find_nid_by_id($employer_id_to, 'grievance_shop');
        if (!$employer_nid) {
          return array('success' => FALSE, 'msg' => "For coverage month $coverage_month, employer not found: $employer_id_to");
        }
      }
    }

    if (!$employer_nid) { continue; }
    $employer_node = node_load($employer_nid);
    if (!$employer_node) { continue; }
    $industry_tid = $employer_node->field_sirius_industry['und'][0]['tid'];
    if (!$industry_tid) { continue; }
    $ms_tid = sirius_smf_feed_members_ms_lookup_by_industry($industry_tid);
    if (!$ms_tid) { continue; }
    $ms_term = taxonomy_term_load($ms_tid);
    if (!$ms_term) { continue; }

    // drupal_set_message($row['ssn'] . "In $year-$month-$day, employer was $employer_nid, so industry was $industry_tid and ms was $ms_tid  ");
    $ms_assignments["$year-$month-$day 00:00:00"] = array(
      'ms_tid' => $ms_tid,
      'ms_term' => $ms_term,
      'employer_name' => $employer_node->title,
      'msg' => 'Import: Setting member status to ' . $ms_term->name . " because member was employed at " . $employer_node->title,
    );
  }
  ksort($ms_assignments);
  $row['ms_assignments'] = $ms_assignments;

  return array('success' => TRUE);
}