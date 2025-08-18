<?php

sirius_require('sirius_trust', 'sirius_trust.report_worker_benefit_ending.inc');

class Sirius_Smf_Report_Benefit_Ending_Xtra extends Sirius_Trust_Report_Worker_Benefit_Ending {
  public $type = 'sirius-smf-benefit-ending-xtra';

  public function info($result = array()) {
    $result = parent::info($result);
    $result['name'] = "Coverage - Ending - With Cobra Rate";
    $result['module'] = 'sirius_smf';
    $result['file'] = 'reports/benefit_ending_xtra.inc';
    $result['preview']['default_display_fields'][] = 'cobra_rate';
    return $result;
  }

  public function fields() {
    $fields = parent::fields();
    $fields['cobra_rate'] = 'COBRA Rate';
    return $fields;
  }

  public function batch($worker_nids, $values, &$context) {
    $result = parent::batch($worker_nids, $values, $context);
    if (!$result['rows']) { return $result; }

    $cobra = Sirius_Smf_Cobra::getInstance();
    foreach ($result['rows'] as $pk => $row) {
      $worker_node = node_load($row['worker_nid']);
      $cobra_result = $cobra->rate_from_worker($worker_node);
      if (!$cobra_result['success']) { continue; }
      $result['rows'][$pk]['cobra_rate'] = $cobra_result['rate'];
    }
    return $result;
  }
}
