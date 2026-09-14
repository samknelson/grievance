<?php

/**
 * New To Benefit
 *
 * Workers whose first-ever coverage under a selected benefit begins on or
 * after a cutoff date. "First-ever" means: no prior election carrying that
 * benefit, and no prior worker-month-benefit (WMB) for that benefit where
 * the worker was the subscriber. Coverage as someone else's dependent does
 * not count as prior history.
 *
 * pks():   one query returning worker_nid : first_start : election_nids,
 *          chunked. All the "never had it before" logic lives here.
 * batch(): node_load per worker for name, member ID, and mailing address.
 */

class Sirius_Smf_Report_New_To_Benefit extends Report_Cache_Report_Generator {
  public $type = 'sirius-smf-new-to-benefit';

  public function info($result = array()) {
    $result = array(
      'name' => t('New To Benefit'),
      'description' => t('Workers with a first-ever election for a given benefit on or after a cutoff date.'),
      'access' => 'sirius trust staff',
      'preview' => array(
        'default_display_fields' => array(
          'worker_nid',
          'member_id',
          'worker_name',
          'benefit_title',
          'first_start',
          'address_street',
          'address_city',
          'address_state',
          'address_zip',
        ),
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'reports/new_to_benefit.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {
    $fields = array();
    $fields['worker_nid']     = 'Worker NID';
    $fields['member_id']      = 'Member ID';
    $fields['worker_name']    = 'Worker';
    $fields['benefit_nid']    = 'Benefit NID';
    $fields['benefit_title']  = 'Benefit';
    $fields['first_start']    = 'Coverage Start';
    $fields['election_nids']  = 'Election NIDs';
    $fields['address_name']   = 'Address Name';
    $fields['address_street'] = 'Street';
    $fields['address_street2'] = 'Street 2';
    $fields['address_city']   = 'City';
    $fields['address_state']  = 'State';
    $fields['address_zip']    = 'ZIP';
    $fields['address_country'] = 'Country';
    $fields['address_source'] = 'Address Source';
    $fields['phone']          = 'Phone';
    $fields['email']          = 'Email';
    return $fields;
  }

  public function preview_links($row) {
    $links = array();
    if ($row['worker_nid']) { $links['node/' . $row['worker_nid']] = 'Worker'; }
    return $links;
  }

  public function form() {
    $form['benefit_nid'] = array(
      '#type' => 'select',
      '#title' => t('Benefit'),
      '#options' => $this->benefit_options(),
      '#required' => TRUE,
      '#empty_option' => t('- Select -'),
    );

    $form['cutoff'] = array(
      '#type' => 'date_popup',
      '#title' => t('Cutoff Date'),
      '#description' => t('Include workers whose coverage under this benefit starts on or after this date, and who have no coverage history for it before this date.'),
      '#attributes' => array('autocomplete' => 'off'),
      '#date_format' => 'Y-m-d',
      '#required' => TRUE,
      '#default_value' => date('Y-m-d'),
    );

    return $form;
  }

  /**
   * Benefit nid => title, for the selector. Built from the benefit nodes
   * themselves so the list never goes stale.
   */
  public function benefit_options() {
    $sql = "select n.nid, n.title from node n ";
    $sql .= "where n.type = 'sirius_trust_benefit' ";
    $sql .= "and n.status = 1 ";
    $sql .= "order by n.title ";
    $stmt = sirius_sql_query($sql, array());

    $options = array();
    while ($hr = $stmt->fetchAssoc()) {
      $options[$hr['nid']] = $hr['title'];
    }
    return $options;
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

    return $html;
  }

  public function pks(&$form_state) {
    $parameters = $form_state['values']['parameters'];

    $benefit_nid = $parameters['benefit_nid'] + 0;
    $cutoff = date('Y-m-d', strtotime($parameters['cutoff'])) . ' 00:00:00';

    $sql = "select w.nid as worker_nid, ";
    $sql .= "min(ds.field_sirius_date_start_value) as first_start, ";
    $sql .= "group_concat(distinct e.nid order by e.nid separator ',') as election_nids ";
    $sql .= "from node e ";
    $sql .= "join field_data_field_sirius_worker fw ";
    $sql .= "  on fw.entity_type = 'node' and fw.entity_id = e.nid ";
    $sql .= "  and fw.bundle = 'sirius_trust_worker_election' and fw.deleted = 0 ";
    $sql .= "join node w ";
    $sql .= "  on w.nid = fw.field_sirius_worker_target_id and w.type = 'sirius_worker' ";
    $sql .= "join field_data_field_sirius_trust_benefits fb ";
    $sql .= "  on fb.entity_type = 'node' and fb.entity_id = e.nid ";
    $sql .= "  and fb.bundle = 'sirius_trust_worker_election' and fb.deleted = 0 ";
    $sql .= "join field_data_field_sirius_active fa ";
    $sql .= "  on fa.entity_type = 'node' and fa.entity_id = e.nid ";
    $sql .= "  and fa.bundle = 'sirius_trust_worker_election' and fa.deleted = 0 ";
    $sql .= "join field_data_field_sirius_date_start ds ";
    $sql .= "  on ds.entity_type = 'node' and ds.entity_id = e.nid ";
    $sql .= "  and ds.bundle = 'sirius_trust_worker_election' and ds.deleted = 0 ";
    $sql .= "where e.type = 'sirius_trust_worker_election' ";
    $sql .= "and e.status = 1 ";
    $sql .= "and fb.field_sirius_trust_benefits_target_id = :benefit_nid ";
    $sql .= "and fa.field_sirius_active_value = 'Yes' ";
    $sql .= "and ds.field_sirius_date_start_value >= :cutoff ";

    // No earlier election carrying this benefit, any status.
    $sql .= "and not exists ( ";
    $sql .= "  select 1 ";
    $sql .= "  from field_data_field_sirius_worker fw2 ";
    $sql .= "  join field_data_field_sirius_trust_benefits fb2 ";
    $sql .= "    on fb2.entity_type = 'node' and fb2.entity_id = fw2.entity_id ";
    $sql .= "    and fb2.bundle = 'sirius_trust_worker_election' and fb2.deleted = 0 ";
    $sql .= "  left join field_data_field_sirius_date_start ds2 ";
    $sql .= "    on ds2.entity_type = 'node' and ds2.entity_id = fw2.entity_id ";
    $sql .= "    and ds2.bundle = 'sirius_trust_worker_election' and ds2.deleted = 0 ";
    $sql .= "  where fw2.entity_type = 'node' ";
    $sql .= "  and fw2.bundle = 'sirius_trust_worker_election' and fw2.deleted = 0 ";
    $sql .= "  and fw2.field_sirius_worker_target_id = w.nid ";
    $sql .= "  and fb2.field_sirius_trust_benefits_target_id = :benefit_nid ";
    $sql .= "  and (ds2.field_sirius_date_start_value < :cutoff ";
    $sql .= "    or ds2.field_sirius_date_start_value is null) ";
    $sql .= ") ";

    // No earlier WMB for this benefit where the worker was the subscriber.
    $sql .= "and not exists ( ";
    $sql .= "  select 1 ";
    $sql .= "  from field_data_field_sirius_trust_subscriber sub3 ";
    $sql .= "  join field_data_field_sirius_worker fw3 ";
    $sql .= "    on fw3.entity_type = 'node' and fw3.entity_id = sub3.entity_id ";
    $sql .= "    and fw3.bundle = 'sirius_trust_worker_benefit' and fw3.deleted = 0 ";
    $sql .= "  join field_data_field_sirius_trust_benefit fb3 ";
    $sql .= "    on fb3.entity_type = 'node' and fb3.entity_id = sub3.entity_id ";
    $sql .= "    and fb3.bundle = 'sirius_trust_worker_benefit' and fb3.deleted = 0 ";
    $sql .= "  left join field_data_field_sirius_date_start ds3 ";
    $sql .= "    on ds3.entity_type = 'node' and ds3.entity_id = sub3.entity_id ";
    $sql .= "    and ds3.bundle = 'sirius_trust_worker_benefit' and ds3.deleted = 0 ";
    $sql .= "  where sub3.entity_type = 'node' ";
    $sql .= "  and sub3.bundle = 'sirius_trust_worker_benefit' and sub3.deleted = 0 ";
    $sql .= "  and sub3.field_sirius_trust_subscriber_target_id = w.nid ";
    $sql .= "  and fw3.field_sirius_worker_target_id = w.nid ";
    $sql .= "  and fb3.field_sirius_trust_benefit_target_id = :benefit_nid ";
    $sql .= "  and (ds3.field_sirius_date_start_value < :cutoff ";
    $sql .= "    or ds3.field_sirius_date_start_value is null) ";
    $sql .= ") ";

    $sql .= "group by w.nid ";
    $sql .= "order by first_start, w.nid ";

    $stmt = sirius_sql_query($sql, array(
      ':benefit_nid' => $benefit_nid,
      ':cutoff' => $cutoff,
    ));

    // Carry first_start and election_nids through the pk so batch() doesn't
    // have to re-derive them. Fixed-width worker nid keeps the sort stable.
    $pks = array();
    while ($hr = $stmt->fetchAssoc()) {
      $pks[] = $hr['worker_nid'] . '|' . $hr['first_start'] . '|' . $hr['election_nids'];
    }

    return array_chunk($pks, 100);
  }

  public function batch($pks, $values, &$context) {
    $parameters = $values['parameters'];
    $benefit_nid = $parameters['benefit_nid'] + 0;

    $benefit_node = node_load($benefit_nid);
    $benefit_title = $benefit_node ? $benefit_node->title : '';

    $rows = array();
    foreach ($pks as $pk) {
      list ($worker_nid, $first_start, $election_nids) = explode('|', $pk, 3);
      $worker_nid += 0;

      $worker_node = node_load($worker_nid);
      if (!$worker_node) { continue; }

      $row = array();
      $row['worker_nid'] = $worker_nid;
      $row['worker_name'] = $worker_node->title;
      $row['member_id'] = $worker_node->field_sirius_id['und'][0]['value'];
      $row['benefit_nid'] = $benefit_nid;
      $row['benefit_title'] = $benefit_title;
      $row['first_start'] = $first_start;
      $row['election_nids'] = $election_nids;

      $row['phone'] = $worker_node->field_sirius_phone['und'][0]['value'];
      $row['email'] = $worker_node->field_sirius_email['und'][0]['email'];

      $row += $this->address($worker_node);

      $rows[$worker_nid] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }

  /**
   * Mailing address: worker node first, contact node as fallback.
   * Staging spot-checks showed the two agree where both are present.
   */
  public function address($worker_node) {
    $out = array(
      'address_name' => '',
      'address_street' => '',
      'address_street2' => '',
      'address_city' => '',
      'address_state' => '',
      'address_zip' => '',
      'address_country' => '',
      'address_source' => '',
    );

    $address = $worker_node->field_sirius_address['und'][0];
    $source = 'worker';

    if (!$address || !$address['thoroughfare']) {
      $contact_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];
      if ($contact_nid) {
        $contact_node = node_load($contact_nid);
        if ($contact_node && $contact_node->field_sirius_address['und'][0]['thoroughfare']) {
          $address = $contact_node->field_sirius_address['und'][0];
          $source = 'contact';
        }
      }
    }

    if (!$address || !$address['thoroughfare']) { return $out; }

    $out['address_name'] = $address['name_line'];
    $out['address_street'] = $address['thoroughfare'];
    $out['address_street2'] = $address['premise'];
    $out['address_city'] = $address['locality'];
    $out['address_state'] = $address['administrative_area'];
    $out['address_zip'] = $address['postal_code'];
    $out['address_country'] = $address['country'];
    $out['address_source'] = $source;

    return $out;
  }
}
