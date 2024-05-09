<?php

class Report_Cache_Report_Generator_Legacy extends Report_Cache_Report_Generator {
	public $info = array();

 	public function info($result = array()) {
 		if ($result) { $this->info = $result; }
    return parent::info($this->info);
  }

  public function form() {
  	$callback = $this->info()['parameters_form_callback'];
  	if (!$callback) { return array(); }
		return $callback($this->type);
  }

  public function pks(&$form_state, $report_type = NULL, $report_id = NULL) {
  	$callback = $this->info()['pk_list_callback'];
  	if (!$callback) {
	  	drupal_set_message(t('No pk_list_callback.'), 'error');
	  	return array();
  	}
  	return $callback($form_state, $report_type, $report_id);
  }

  public function batch($pk, $values, &$context) {
  	$callback = $this->info()['batch_callback'];
  	if (!$callback) {
			drupal_set_message(t('No batch_callback.'), 'error');
			return;
		}

		return $callback($pk, $values, $context['results']['report_id'], $context['results']['report_action'], $context);
	}
}

class Report_Cache_Generator_Auto_grievance_report_average_time extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_grievance_report_spi_term extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_grievance_report_open_summary_with_log extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_grievance_report_shop_monthly extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_grievance_report_status_summary extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_grievance_report_log_include_exclude extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_sirius_bac1_iu_hours extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_sirius_dispatch_member_status_ratios extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_sirius_dispatch_notification_preferences extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_sirius_dispatch_prior_employed_list extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_sirius_event_wwe extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_sirius_ledger_discrepancy extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_sirius_ledger_transmittal_balances extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_sirius_trust_eligibility_compare extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_sirius_trust_worker_benefit extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_sirius_trust_worker_summary extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_sirius_trust_worker_change extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_sirius_trust_relationship_sequence extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_sirius_trust_worker_benefit_warning extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_sirius_trust_worker_benefit_multiple extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_sirius_worker_duplicate_id extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_sirius_worker_multiple_member_status extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_sirius_worker_skill_log extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_sirius_worker_ssn_missing extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_sirius_worker_ssn_invalid extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_sirius_worker_ssn_duplicate extends Report_Cache_Report_Generator_Legacy {}

class Report_Cache_Generator_Auto_grievance_uh_resproc_cancelby extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_grievance_uh23_report_summary extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_grievance_uh226_report_spi_term_summary extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_grievance_uh226_report_spi_term_detail extends Report_Cache_Report_Generator_Legacy {}
class Report_Cache_Generator_Auto_grievance_uh226_report_violation extends Report_Cache_Report_Generator_Legacy {}