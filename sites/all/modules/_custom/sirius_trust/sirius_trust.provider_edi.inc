<?php

class Sirius_Trust_Provider_Edi_Report_Generator extends Report_Cache_Report_Generator {
	public $edi_output_format = 'csv';

	/**
	 * Remove FILTER columns from the default display
	 */

  public function info($result = array()) {
  	$default_display_fields = $this->fields();
		$default_display_fields = array_filter($default_display_fields, function($v) {
		  return !preg_match('/FILLER/i', $v);
		});
		$result['preview']['default_display_fields'] = $default_display_fields;

    return parent::info($result);
  }

	public function edi_output_format() { return $this->edi_output_format; }

	public function edi_fixed_fields() { return array(); }

	public function edi_output_stream_fixed($fp, $report_id) {
		$report_type = $this->type;
		$domain_nid = sirius_domain_nid(); if (!$domain_nid) { $domain_nid = 0; }
		$info = $this->info();

		$query = db_select('report_cache');
		$query->fields('report_cache');
		$query->condition('report_type', $report_type);
		$query->condition('report_id', $report_id);
		$query->condition('report_domain_nid', $domain_nid);
		$stmt = $query->execute();

		$fields = $this->edi_fixed_fields();
		while ($hr = $stmt->fetchAssoc()) {
			$hr = $info['generator']->row_decode($hr['report_data']);
			foreach ($fields as $field_name => $field_data) {
				$width = $field_data['width'];
				$val = $hr[$field_name];
        $format = '%-' . $width . '.' . $width . 's';
        // fwrite($fp, "$field_name: ($format): [");
        fprintf($fp, $format, $val);
        // fwrite($fp, "]\n");
			}
			fwrite($fp, "\n");
		}
		// fclose($fp);
	}

	public function edi_output_stream($fp, $report_id, $format = NULL) {
		if (!$format || ($format == 'default')) {
			$format = $this->edi_output_format();
		}

		if ($format == 'fixed') {
			return $this->edi_output_stream_fixed($fp, $report_id);
		} else {
			return $this->edi_output_stream_csv($fp, $report_id);
		}
	}

	public function edi_output_stream_csv($fp, $report_id) {
		$report_type = $this->type;

		$domain_nid = sirius_domain_nid(); if (!$domain_nid) { $domain_nid = 0; }

		$info = $this->info();

		$fields = $info['fields'];

		fputcsv($fp, $fields);

		$query = db_select('report_cache');
		$query->fields('report_cache');
		$query->condition('report_type', $report_type);
		$query->condition('report_id', $report_id);
		$query->condition('report_domain_nid', $domain_nid);
		$stmt = $query->execute();

		while ($hr = $stmt->fetchAssoc()) {
			$hr = $info['generator']->row_decode($hr['report_data']);
			$row = array();
			foreach ($fields as $name => $prompt) {
				$row[] = strip_tags($hr[$name]);
			}
			fputcsv($fp, $row);
		}
	}

  public function context($wb_nid, $asof_date) {
    $wb_node = node_load($wb_nid);
    $worker_node = node_load($wb_node->field_sirius_worker['und'][0]['target_id']);
    $subscriber_node = node_load($wb_node->field_sirius_trust_subscriber['und'][0]['target_id']);
    $relationship_nid = $wb_node->field_sirius_contact_relation['und'][0]['target_id'];
    $relationship_node = node_load($relationship_nid);
    $contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);
    $employer_node = node_load($wb_node->field_grievance_shop['und'][0]['target_id']);

    $context = array();
    $context['pk'] = $wb_nid;
    $context['employer_node'] = $employer_node;
    $context['worker_node'] = $worker_node;
    $context['subscriber_node'] = $subscriber_node;
    $context['wb_node'] = $wb_node;
    $context['relationship_node'] = $relationship_node;
    $context['asof_date'] = $asof_date;
    $context['asof_ts'] = strtotime($asof_date);
    $context['contact_node'] = $contact_node;
    $context['dependent_relationship_nodes'] = $this->dependent_relationship_nodes($context);

    return $context;
  }

  public function benefit_nid_from_id($benefit_id) {
    $result = sirius_feed_lookup_by_field('sirius_trust_benefit', 'field_data_field_sirius_id', 'field_sirius_id_value', $benefit_id);
    if (!$result['success']) {
      drupal_set_message("Please make sure that there is a benefit with the ID $benefit_id.", 'error');
      return NULL;
    }
    return $result['nid'];
  }

  public function relationship_type($relationship_node) {
    $reltype_tid = $relationship_node->field_sirius_contact_reltype['und'][0]['tid'];
    if (!$reltype_tid) { return NULL; }
    $reltype_term = taxonomy_term_load($reltype_tid);
    if (!$reltype_term) { return NULL; }
    return $reltype_term->field_sirius_id['und'][0]['value'];
  }

  public function gender_id($worker_node) {
		$lookup = &drupal_static(__FUNCTION__);
    if (!isset($lookup)) { $lookup = sirius_taxonomy_load('sirius_gender', 'tid', 'full'); }
    $tid = $worker_node->field_sirius_gender['und'][0]['tid'];
    if (!$lookup[$tid]) { return ''; }
    return $lookup[$tid]->field_sirius_id['und'][0]['value'];
  }

  public function is_subscriber($context) {
    if ($context['relationship_node']) { return FALSE; }
    return TRUE;
  }

  public function is_dependent($context) {
    return !($this->is_subscriber($context));
  }

  public function is_cobra($context) {
    if (!$context['employer_node']) { return ''; }
    return ($context['employer_node']->field_sirius_id['und'][0]['value'] == 'COBRA');
  }

  public function dependent_relationship_nodes($context) {
    if ($this->is_dependent($context)) { return array(); }

    $sql_args = array();

    $sql = "select field_sirius_contact_relation_target_id ";
    $sql .= "from node ";
    $sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_date_end on field_data_field_sirius_date_end.entity_type = 'node' and field_data_field_sirius_date_end.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_trust_subscriber on field_data_field_sirius_trust_subscriber.entity_type = 'node' and field_data_field_sirius_trust_subscriber.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_contact_relation on field_data_field_sirius_contact_relation.entity_type = 'node' and field_data_field_sirius_contact_relation.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_trust_benefit on field_data_field_sirius_trust_benefit.entity_type = 'node' and field_data_field_sirius_trust_benefit.entity_id = node.nid ";

    $sql .= "where node.type = 'sirius_trust_worker_benefit' ";

    $benefit_nid = $this->benefit_nid($context);
    if (is_array($benefit_nid)) {
      $sql .= "and field_sirius_trust_benefit_target_id in (:benefit_nid) ";
      $sql_args[':benefit_nid'] = $benefit_nid;
    } else {
      $sql .= "and field_sirius_trust_benefit_target_id = :benefit_nid ";
      $sql_args[':benefit_nid'] = $benefit_nid;
    }

    $sql .= "and field_sirius_date_start_value <= :asof_date ";
    $sql .= "and (field_sirius_date_end_value >= :asof_date or field_sirius_date_end_value is null) ";
    $sql_args[':asof_date'] = $context['asof_date'];

    $sql .= "and field_sirius_trust_subscriber_target_id = :subscriber_nid ";
    $sql_args[':subscriber_nid'] = $context['subscriber_node']->nid;

    $sql .= "order by field_sirius_trust_subscriber_target_id, field_sirius_contact_relation_target_id ";

    $stmt = sirius_sql_query($sql, $sql_args);

    $relationship_nids = array();
    while ($hr = $stmt->fetchAssoc()) {
      $relationship_nids[] = $hr['field_sirius_contact_relation_target_id'];
    }
    return node_load_multiple($relationship_nids);
  }

  public function coverage_start_date($context) {
    if (!$context['wb_node']) { return ''; }
    if (!$context['wb_node']->field_sirius_date_start['und'][0]['value']) { return ''; }
    return date('Ymd', strtotime($context['wb_node']->field_sirius_date_start['und'][0]['value']));
  }

  public function coverage_end_date($context) {
    if (!$context['wb_node']) { return ''; }
    if (!$context['wb_node']->field_sirius_date_end['und'][0]['value']) { return ''; }
    return date('Ymd', strtotime($context['wb_node']->field_sirius_date_end['und'][0]['value']));
  }

  public function phone($context, $which) {
  	return preg_replace('/[^0-9]/', '', $context[$which]->field_sirius_phone['und'][0]['value']);
  }

  public function dob($context, $which) {
  	$date = $context[$which]->field_sirius_dob['und'][0]['value'];
  	if (!$date) { return ''; }
  	return date('Ymd', strtotime($date));
  }

  public function ssn($context, $which) {
  	return preg_replace('/[^0-9]/', '', $context[$which]->field_sirius_ssn['und'][0]['value']);
  }
}
