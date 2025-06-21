<?php

class Sirius_Worker_Report_Geo extends Report_Cache_Report_Generator {
  public $type = 'sirius-worker-geo';

  public function info($result = array()) {
    $result = array(
      'name' => t('Geo Search'),
      'access' => 'sirius worker view',
      'module' => 'sirius_worker',
      'file' => 'sirius_worker.report_geo.inc',
      'dashboard' => TRUE,
      'operations' => array(
        'contact' => array(
          'field_contact_nid' => 'contact_nid',
        ),
      ),
      'preview' => array(
        /*
        'default_display_fields' => array(
          'worker_title',
          'distance',
        ),
        */
        'preview_links' => TRUE,
      ),
    );

    return parent::info($result);
  }

  public function fields() {
    return array(
      'contact_nid' => 'Contact NID',
      'worker_nid' => 'Worker NID',
      'distance' => 'Distance in Miles',
      'worker_title' => 'Name',
      'worker_ssn' => 'SSN',
      'worker_address_thoroughfare' => 'Address',
      'worker_address_premise' => 'Address 2',
      'worker_address_locality' => 'City',
      'worker_address_administrative_area' => 'State',
      'worker_address_postal_code' => 'Zip',
    );
  }

  public function preview_links($row) {
    return array(
      'node/' . $row['worker_nid'] => 'Worker',
    );
  }

  public function form() {
    $form['addr'] = array(
      '#type' => 'textfield',
      '#title' => t('Address'),
    );

    $form['radius'] = array(
      '#type' => 'textfield',
      '#title' => t('Radius (in miles)'),
      '#size' => 4,
    );

    return $form;
  }

  public function pks(&$form_state) {
    $domain_nid = sirius_domain_nid();
    $sql_args = array();

    $addr = $form_state['values']['parameters']['addr'];
    $result = sirius_addr_geocode_impl($addr);
    if (!$result['success']) {
      drupal_set_message("Geocode failed for [$addr]: " . $result['msg'], 'warning');
      return array();
    } else {
      $form_state['values']['parameters']['lat'] = $result['point']->coords[1];
      $form_state['values']['parameters']['lon'] = $result['point']->coords[0];
    }

    $sql = "select node.nid as worker_nid, ";
    $sql .= "contact_node.nid as contact_nid, ";
    $sql .= "field_sirius_address_geo_lat as lat, ";
    $sql .= "field_sirius_address_geo_lon as lon ";
    $sql .= " from node ";

    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";

    $sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
    $sql .= "left join node contact_node on contact_node.nid = field_sirius_contact_target_id ";
    $sql .= "left join field_data_field_sirius_address_geo on field_data_field_sirius_address_geo.entity_type = 'node' and field_data_field_sirius_address_geo.entity_id = contact_node.nid ";
    $sql .= "where node.type = 'sirius_worker' ";
    if ($domain_nid) { 
      $sql .= "and field_sirius_domain_target_id = :domain_nid ";
      $sql_args[':domain_nid'] = $domain_nid;
    } else {
      $sql .= "and field_sirius_domain_target_id is NULL ";
    }
    $sql .= "and field_sirius_address_geo_lat is not null ";
    $sql .= "and field_sirius_address_geo_lon is not null ";
    // $sql .= "and node.nid = 2542724 ";

    $sql .= "order by node.nid ";
    // $sql .= "limit 500 ";

    $stmt = sirius_sql_query($sql, $sql_args);
    $records = array();
    while ($hr = $stmt->fetchAssoc()) {
      $records[] = $hr;
    }
    return array_chunk($records, 100, TRUE);
  }

  public function batch($records, $values, &$context) {
    $lat = $values['parameters']['lat'];
    $lon = $values['parameters']['lon'];
    $radius = $values['parameters']['radius'];

    $rows = array('is_multiple' => TRUE, 'rows' => array());
    $worker_nids_with_multiple = array();
    foreach ($records as $record) {
      $distance = round(sirius_addr_latlon_distance($lat, $lon, $record['lat'], $record['lon']), 4);
      if ($distance > $radius) { continue; }

      $worker_node = node_load($record['worker_nid']);

      $row = array();
      $row['worker_nid'] = $record['worker_nid'];
      $row['contact_nid'] = $record['contact_nid'];
      $row['distance'] = $distance;
      $row['worker_title'] = sirius_node_title($row['worker_nid']);
      $row['worker_ssn'] = $worker_node->field_sirius_ssn['und'][0]['value'];
      foreach (array('thoroughfare', 'premise', 'locality', 'administrative_area', 'postal_code') as $key) {
        $row["worker_address_$key"] = $worker_node->field_sirius_address['und'][0][$key];
      }

      $rows['rows'][$record['worker_nid']] = $row;

      $context['results']['dashboard']['count']++;
    }

    return $rows;
  }

  public function render_parameters($parameters) {
    $parameters = $parameters['parameters'];
    $options = array();
    $html = sirius_jsonfield_util_render_all_values_new($this->form(), $parameters, $options);
    return $html;
  }
}
