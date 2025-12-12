<?php

class Sirius_Smf_Report_Electionrequest extends Report_Cache_Report_Generator {
  public $type = 'sirius-smf-electionrequest';

  public function info($result = array()) {
    $result = array(
      'name' => t('Election Request'),
      'description' => '',
      'access' => 'sirius trust staff',
      'preview' => array(
        // 'default_display_fields' => array(),
        'preview_links' => TRUE,
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'reports/electionrequest.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {
    $fields = array();
    $fields['worker_nid'] = 'NID';
    $fields['worker_name'] = 'Name';

    $cols = sirius_addr_cols();
    foreach ($cols as $col => $title) { $fields["worker_address_$col"] = $title; }

    $fields['employer_nid'] = 'Employer NID';
    $fields['employer_name'] = 'Employer Name';

    return $fields;
  }

  public function preview_links($row) {
    return array(
      'node/' . $row['worker_nid'] => 'Worker',
      'node/' . $row['worker_nid'] . '/sirius_worker_hours' => 'Hours',
    );
  }

  public function form() {
    $form['exclude_tag_tid'] = array(
      '#type' => 'select',
      '#title' => 'Exclude workers with this tag',
      '#options' => array(NULL => t('-- Select --')) + sirius_taxonomy_load('sirius_contact_tags', 'tid', 'name'),
    );

    return $form;
  }

  public function render_parameters($parameters) {
    $parameters = $parameters['parameters'];

    $html = sirius_jsonfield_util_render_all_values_new(
      $this->form(), 
      $parameters, 
      array(
        'skip_empty' => TRUE,
        'no_fieldsets' => TRUE,
      )
    );

    if (!$html) { $html = '&nbsp;'; }

    return $html;
  }

  public function pks(&$form_state) {
    $parameters = $form_state['values']['parameters'];

    $sql_args = array();
    $sql = "select distinct worker_nid ";
    $sql .= "from sirius_hours_cache ";
    // $sql .= "where worker_nid = 12019238 ";
    $sql .= "order by worker_nid desc ";
    // $sql .= "limit 10 ";

    $stmt = sirius_sql_query($sql, $sql_args);

    $pks = array();
    while ($hr = $stmt->fetchAssoc()) {
      $pks[] = $hr['worker_nid'];
    }

    return array_chunk($pks, 100);
  }

  public function batch($worker_nids, $values, &$context) {
    $employer_industry_map = &drupal_static(__FUNCTION__);

    // For buildup, we need an employer. Assume COBRA employer for no good reas.
    $cobra = Sirius_Trust_Cobra::getInstance();
    $employer_node = $cobra->cobra_employer();
    $policy_node = sirius_ledger_policy_lookup(NULL, $employer_node);

    $benefit_types = sirius_taxonomy_load('sirius_trust_benefit_type', 'tid', 'full');
    $benefits = sirius_trust_benefits();
    $election_complete_nids = array();
    foreach ($benefits as $benefit_nid => $benefit_node) {
      $type_tid = $benefit_node->field_sirius_trust_benefit_type['und'][0]['tid'];
      $type_term = $benefit_types[$type_tid];
      $type_id = $type_term->field_sirius_id['und'][0]['value'];

      if (in_array($type_id, array('MEDICAL', 'DENTAL'))) {
        $election_complete_nids[] = $benefit_nid;
      }
    }

    $exclude_tag_tid = $values['parameters']['exclude_tag_tid'];

    $rows = array();
    foreach ($worker_nids as $worker_nid) {
      $row = array();

      $worker_node = node_load($worker_nid);

      if ($exclude_tag_tid) {
        $contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);
        $tag_tids = sirius_contact_tags_get($contact_node);
        if (in_array($exclude_tag_tid, $tag_tids)) { continue; }
      }

      $sql = "select employer_nid from sirius_hours_cache ";
      $sql .= "where worker_nid = :worker_nid ";
      $sql .= "order by year desc, month desc, day desc limit 1";
      $stmt = sirius_sql_query($sql, array(':worker_nid' => $worker_nid));
      $hr = $stmt->fetchAssoc();
      $row['employer_nid'] = $hr['employer_nid'];
      $row['employer_name'] = sirius_node_title($row['employer_nid']);

      $elig_plugin_info = sirius_trust_eligibility_plugin_info('smf-buildup');
      $settings = array();
      $params = array();
      $params['employer_node'] = $employer_node;
      $params['policy_node'] = $policy_node;
      $params['subscriber_worker_node'] = $worker_node;
      $params['is_election'] = TRUE;
      $result = SiriusResult::create($elig_plugin_info['instance']->check($settings, $params));
      if (!$result->success()) { continue; }

      $election_node = sirius_trust_worker_election($worker_node);

      if ($election_node) {
        $benefit_nids = sirius_fieldvals($election_node, 'field_sirius_trust_benefits', 'target_id');
        if (array_intersect($benefit_nids, $election_complete_nids)) { continue; }
      }

      $row['worker_nid'] = $worker_nid;
      $row['worker_name'] = $worker_node->title;
      $row['worker_phone'] = $worker_node->field_sirius_phone['und'][0]['value'];
      $row['worker_email'] = $worker_node->field_sirius_email['und'][0]['value'];

      $cols = sirius_addr_cols();
      foreach ($cols as $col => $title) {
        $row["worker_address_$col"] = $worker_node->field_sirius_address['und'][0][$col];
      }

      // $row['employer_nid'] = $election_node->field_grievance_shop['und'][0]['target_id'];
      // $row['employer_name'] = sirius_node_title($row['employer_nid']);

      $rows[$worker_nid] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }
}
