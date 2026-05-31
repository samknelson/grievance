<?php

sirius_require('sirius_trust', 'sirius_trust.report_worker_benefit_ending.inc');

class Sirius_Smf_Report_Benefit_Ending_Xtra extends Sirius_Trust_Report_Worker_Benefit_Ending {
  public $type = 'sirius-smf-benefit-ending-xtra';

  public function info($result = array()) {
    $result = parent::info($result);
    $result['name'] = "Coverage - Ending - With Cobra Rate";
    $result['module'] = 'sirius_smf';
    $result['file'] = 'reports/benefit_ending_xtra.inc';

    $result['preview']['default_display_fields'][] = 'election_employer';
    $result['preview']['default_display_fields'][] = 'cobra_deps';
    $result['preview']['default_display_fields'][] = 'cobra_deps';

    return $result;
  }

  public function fields() {
    $fields = parent::fields();
    $fields['election_employer'] = 'Employer';
    $fields['cobra_rate'] = 'COBRA Rate';
    $fields['cobra_deps'] = 'COBRA Dependents';
    $fields['wb_scan_msg_type'] = 'Scan Message';
    return $fields;
  }

  public function batch($worker_nids, $values, &$context) {
    $result = parent::batch($worker_nids, $values, $context);
    if (!$result['rows']) { return $result; }

    $cobra = Sirius_Smf_Cobra::getInstance();
    foreach ($result['rows'] as $pk => $row) {
      $worker_node = node_load($row['worker_nid']);
      $cobra_result = $cobra->rate_from_worker($worker_node);
      if ($cobra_result['success']) { 
        $result['rows'][$pk]['cobra_rate'] = $cobra_result['rate'];
      }

      $cobra_result = $cobra->dependents_from_worker($worker_node);
      if ($cobra_result['success']) {
        $result['rows'][$pk]['cobra_deps'] = join(', ', $cobra_result['dependents']);
      }

      $election_node =  sirius_trust_worker_election($worker_node);
      if ($election_node) {
        $result['rows'][$pk]['election_employer'] = sirius_node_title($election_node->field_grievance_shop['und'][0]['target_id']);
      }



      $msg = $result['rows'][$pk]['wb_scan_msg'];
      $msg_type = '';
      if (preg_match('/Hours worked in/i', $msg)) {
        $sql = "select sirius_hours_cache.* ";
        $sql .= "from sirius_hours_cache ";
        $sql .= "where worker_nid = :worker_nid ";
        $sql .= "order by year desc, month desc, day desc, hours_type_name, department_name ";
        $sql .= "limit 1 ";
        $stmt = db_query($sql, array(':worker_nid' => $row['worker_nid']));
        $hr = $stmt->fetchAssoc();

        if (preg_match('/terminated/i', $hr['hours_type_name'])) {
          $msg_type = 'Job Loss';
        } else {
          $msg_type = 'Reduction in Work Hours';
        }
      } else if (preg_match('/has an age of/i', $msg)) {
        $msg_type = 'Loss of Dependent Child Status';
      } else if (preg_match('/The amount due as of the end of/i', $msg)) {
        $msg_type = 'SPECIAL CASE: No COBRA Letter — Arrears / Nonpayment';
      } else if (preg_match('/The relationship ended/i', $msg) || preg_match('/The election does not include the relationship/i', $msg)) {
        $msg_type = 'SPECIAL CASE: Manual Review — Possible Divorce or Death';
      } else {
        $msg_type = 'SPECIAL CASE: Manual Review — Unknown Cause of Benefit Loss';
      }
      $result['rows'][$pk]['wb_scan_msg_type'] = $msg_type;
    }
    return $result;
  }
}
