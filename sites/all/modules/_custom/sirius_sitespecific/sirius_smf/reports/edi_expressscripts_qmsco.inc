<?php

sirius_require('sirius_smf', 'reports/edi_expressscripts.inc');

class Sirius_Smf_Report_Edi_ExpressScripts_QMSCO extends Sirius_Smf_Report_Edi_ExpressScripts {
  public $type = 'sirius-smf-edi-expressscripts-qmsco';
  public $edi_output_format = 'fixed';

  public function info($result = array()) {
    $result = parent::info($result);
    $result['name'] = t('EDI - Express Scripts - QMSCO');
    $result['file'] = 'reports/edi_expressscripts_qmsco.inc';
    return $result;
  }

  public function is_qmsco_only() { return TRUE; }
}