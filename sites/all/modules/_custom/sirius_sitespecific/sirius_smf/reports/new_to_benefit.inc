<?php

/**
 * New To Benefit
 *
 * Subscribers whose FIRST-EVER actual coverage under a selected benefit
 * begins within a selected range of months (inclusive). Each subscriber
 * has exactly one first month, so a multi-month range is the disjoint
 * union of the single-month results.
 *
 * Source of truth is the worker-month-benefit record (WMB): the record of
 * coverage actually granted. Elections are NOT used. An election's start
 * date is the enrollment date, which routinely precedes coverage by one or
 * more months under hours-based eligibility, so an election-based test
 * both mislabels start dates and drops people who enrolled early.
 *
 * Rules
 *   - Subscribers only: the WMB's covered person must be its subscriber.
 *     Prior coverage as someone else's dependent is not prior history.
 *   - "First-ever" = the MIN start date across all of the subscriber's own
 *     WMBs for the benefit. If that falls in the selected month, they are
 *     new that month. No separate "never before" test is needed.
 *   - One-day stub WMBs (start = end) are ignored. The election save writes
 *     these, backdated to the 1st of the current month, in the same second
 *     as the election; they are not real coverage and would otherwise make
 *     a genuinely new subscriber look like they had prior history.
 *   - Unpublished WMBs are ignored.
 *
 * Horizon: WMBs exist only as far ahead as the benefit scan has run
 * (about one month). A month beyond that returns nothing yet.
 *
 * pks():   one grouped query, HAVING on the first start date.
 * batch(): node_load per worker for name, member ID, contact, address.
 */

class Sirius_Smf_Report_New_To_Benefit extends Report_Cache_Report_Generator {
  public $type = 'sirius-smf-new-to-benefit';

  public function info($result = array()) {
    $result = array(
      'name' => t('New To Benefit'),
      'description' => t('Subscribers whose first-ever coverage under a benefit begins in a selected month.'),
      'access' => 'sirius trust staff',
      'preview' => array(
        'default_display_fields' => array(
          'worker_nid',
          'member_id',
          'worker_name',
          'benefit_title',
          'coverage_month',
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
    $fields['worker_nid']      = 'Worker NID';
    $fields['member_id']       = 'Member ID';
    $fields['worker_name']     = 'Worker';
    $fields['benefit_nid']     = 'Benefit NID';
    $fields['benefit_title']   = 'Benefit';
    $fields['first_start']     = 'Coverage Start';
    $fields['coverage_month']  = 'Coverage Month';
    $fields['address_name']    = 'Address Name';
    $fields['address_street']  = 'Street';
    $fields['address_street2'] = 'Street 2';
    $fields['address_city']    = 'City';
    $fields['address_state']   = 'State';
    $fields['address_zip']     = 'ZIP';
    $fields['address_country'] = 'Country';
    $fields['address_source']  = 'Address Source';
    $fields['phone']           = 'Phone';
    $fields['email']           = 'Email';
    return $fields;
  }

  public function preview_links($row) {
    $links = array();
    if ($row['worker_nid']) { $links['node/' . $row['worker_nid']] = 'Worker'; }
    return $links;
  }

  public function form() {
    $options = $this->month_options();
    $next = date('Y-m', strtotime('first day of next month'));

    $form['benefit_nid'] = array(
      '#type' => 'select',
      '#title' => t('Benefit'),
      '#options' => $this->benefit_options(),
      '#required' => TRUE,
      '#empty_option' => t('- Select -'),
    );

    $form['start_month'] = array(
      '#type' => 'select',
      '#title' => t('Start Month'),
      '#options' => $options,
      '#default_value' => $next,
      '#required' => TRUE,
    );

    $form['end_month'] = array(
      '#type' => 'select',
      '#title' => t('End Month'),
      '#description' => t('Reports subscribers whose first-ever coverage under this benefit begins in any month from Start through End, inclusive. Coverage is only on file about one month ahead.'),
      '#options' => $options,
      '#default_value' => $next,
      '#required' => TRUE,
    );

    return $form;
  }

  /**
   * 'YYYY-MM' => 'Month YYYY', January five years back through two months
   * ahead. UI range only; widen if older history is ever needed.
   */
  public function month_options() {
    $options = array();
    $ts = mktime(0, 0, 0, 1, 1, date('Y') - 5);
    $last = strtotime('+2 months', strtotime(date('Y-m-01')));
    while ($ts <= $last) {
      $options[date('Y-m', $ts)] = date('F Y', $ts);
      $ts = strtotime('+1 month', $ts);
    }
    return $options;
  }

  public function form_validate($form, &$form_state) {
    $parameters = $form_state['values']['parameters'];

    $start = $parameters['start_month'];
    $end = $parameters['end_month'];

    if (!preg_match('/^\d{4}-\d{2}$/', $start)) {
      form_set_error("parameters][start_month", 'Please select a start month.');
    }
    if (!preg_match('/^\d{4}-\d{2}$/', $end)) {
      form_set_error("parameters][end_month", 'Please select an end month.');
    }
    if ($start && $end && strcmp($end, $start) < 0) {
      form_set_error("parameters][end_month", 'End month cannot be before start month.');
    }
  }

  /**
   * Benefit nid => title, built from the benefit nodes so it never goes stale.
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

    $benefit_nid = intval($parameters['benefit_nid']);
    // Snap to whole months: 1st at 00:00:00 through last day at 23:59:59.
    $start_ts = strtotime($parameters['start_month'] . '-01');
    $end_ts = strtotime($parameters['end_month'] . '-01');
    $month_start = date('Y-m-d 00:00:00', $start_ts);
    $month_end = date('Y-m-t 23:59:59', $end_ts);

    $sql = "select sub.field_sirius_trust_subscriber_target_id as worker_nid, ";
    $sql .= "min(ds.field_sirius_date_start_value) as first_start ";
    $sql .= "from field_data_field_sirius_trust_benefit fb ";
    $sql .= "join node m ";
    $sql .= "  on m.nid = fb.entity_id and m.type = 'sirius_trust_worker_benefit' and m.status = 1 ";
    $sql .= "join field_data_field_sirius_trust_subscriber sub ";
    $sql .= "  on sub.entity_type = 'node' and sub.entity_id = fb.entity_id ";
    $sql .= "  and sub.bundle = 'sirius_trust_worker_benefit' and sub.deleted = 0 ";

    // Subscribers only: the covered person must be the subscriber.
    $sql .= "join field_data_field_sirius_worker fw ";
    $sql .= "  on fw.entity_type = 'node' and fw.entity_id = fb.entity_id ";
    $sql .= "  and fw.bundle = 'sirius_trust_worker_benefit' and fw.deleted = 0 ";
    $sql .= "  and fw.field_sirius_worker_target_id = sub.field_sirius_trust_subscriber_target_id ";

    $sql .= "join field_data_field_sirius_date_start ds ";
    $sql .= "  on ds.entity_type = 'node' and ds.entity_id = fb.entity_id ";
    $sql .= "  and ds.bundle = 'sirius_trust_worker_benefit' and ds.deleted = 0 ";
    $sql .= "left join field_data_field_sirius_date_end de ";
    $sql .= "  on de.entity_type = 'node' and de.entity_id = fb.entity_id ";
    $sql .= "  and de.bundle = 'sirius_trust_worker_benefit' and de.deleted = 0 ";

    $sql .= "where fb.entity_type = 'node' ";
    $sql .= "and fb.bundle = 'sirius_trust_worker_benefit' ";
    $sql .= "and fb.deleted = 0 ";
    $sql .= "and fb.field_sirius_trust_benefit_target_id = :benefit_nid ";

    // Ignore the one-day stubs written by the election save.
    $sql .= "and (de.field_sirius_date_end_value is null ";
    $sql .= "  or de.field_sirius_date_end_value <> ds.field_sirius_date_start_value) ";

    $sql .= "group by sub.field_sirius_trust_subscriber_target_id ";
    $sql .= "having first_start between :month_start and :month_end ";
    $sql .= "order by first_start, worker_nid ";

    $stmt = sirius_sql_query($sql, array(
      ':benefit_nid' => $benefit_nid,
      ':month_start' => $month_start,
      ':month_end' => $month_end,
    ));

    $pks = array();
    while ($hr = $stmt->fetchAssoc()) {
      $pks[] = $hr['worker_nid'] . '|' . $hr['first_start'];
    }

    return array_chunk($pks, 100);
  }

  public function batch($pks, $values, &$context) {
    $parameters = $values['parameters'];
    $benefit_nid = intval($parameters['benefit_nid']);

    $benefit_node = node_load($benefit_nid);
    $benefit_title = $benefit_node ? $benefit_node->title : '';

    $rows = array();
    foreach ($pks as $pk) {
      list ($worker_nid, $first_start) = explode('|', $pk, 2);
      $worker_nid = intval($worker_nid);

      $worker_node = node_load($worker_nid);
      if (!$worker_node) { continue; }

      $row = array();
      $row['worker_nid'] = $worker_nid;
      $row['worker_name'] = $worker_node->title;
      $row['member_id'] = $worker_node->field_sirius_id['und'][0]['value'];
      $row['benefit_nid'] = $benefit_nid;
      $row['benefit_title'] = $benefit_title;
      $row['first_start'] = $first_start;
      $row['coverage_month'] = substr($first_start, 0, 7);

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
