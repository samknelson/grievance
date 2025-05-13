<?php

class Sirius_Employer_Report_Contact_Bytype extends Report_Cache_Report_Generator {
  public $type = 'sirius-employer-contact-bytype';

  public function info($result = array()) {
    $result = array(
      'name' => t('Employer Contact - By Type'),
      'access' => 'sirius employer admin',
      'module' => 'sirius_employer',
      'file' => 'sirius_employer.report_contact_bytype.inc',
      'dashboard' => FALSE,
      'preview' => array(
        'preview_links' => TRUE,
        'default_display_fields' => array(
          'employer_title',
          'contact_type_name',
          'contact_title',
          'contact_phone',
          'contact_email',
        ),
      ),
    );

    return parent::info($result);
  }

  public function form() {
    return array();
  }

  public function fields() {
    $fields = array();
    $fields['pk'] = 'PK';
    $fields['contact_nid'] = 'Contact NID';
    $fields['employer_nid'] = 'Employer NID';
    $fields['employer_title'] = 'Employer Name';
    $fields['contact_type_tid'] = 'Contact Type TID';
    $fields['contact_type_name'] = 'Contact Type';
    $fields['contact_title'] = 'Contact Title';
    $fields['contact_phone'] = 'Contact Phone';
    $fields['contact_email'] = 'Contact Email';
    // foreach (sirius_addr_cols() as $col => $name) {
    // $fields["worker_address_$col"] = "Worker $name";
    // }

    return $fields;
  }

  public function preview_links($row) {
    $links = array();
    $links['node/' . $row['employer_nid'] . '/contact'] = array('name' => 'Employer');
    if ($row['contact_nid']) { $links['node/' . $row['contact_nid']] = array('name' => 'Contact'); }
    return $links;
  }

  public function pks(&$form_state) {
    $employers = sirius_employer_list();
    return array_chunk(array_keys($employers), 100, TRUE);
  }

  public function batch($employer_nids, $values, &$context) {
    $domain_nid = sirius_domain_nid();

    $contact_types = sirius_taxonomy_load('grievance_contact_types', 'tid', 'name');

    $rows = array('is_multiple' => TRUE, 'rows' => array());
    foreach ($employer_nids as $employer_nid) {

      foreach ($contact_types as $contact_type_tid => $contact_type_name) {

        $sql_args = array();
        $sql = "select node.nid ";
        $sql .= "from node ";
        $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
        $sql .= "left join field_data_field_grievance_contact_types on field_data_field_grievance_contact_types.entity_type = 'node' and field_data_field_grievance_contact_types.entity_id = node.nid ";
        $sql .= "left join field_data_field_grievance_shops on field_data_field_grievance_shops.entity_type = 'node' and field_data_field_grievance_shops.entity_id = node.nid ";
        $sql .= "where node.type = 'grievance_shop_contact' ";

        $sql .= "and field_grievance_shops_target_id = :employer_nid ";
        $sql_args[':employer_nid'] = $employer_nid;

        $sql .= "and field_grievance_contact_types_tid = :contact_type_tid ";
        $sql_args[':contact_type_tid'] = $contact_type_tid;

        if ($domain_nid) { 
          $sql .= "and field_sirius_domain_target_id = :domain_nid ";
          $sql_args[':domain_nid'] = $domain_nid;
        } else {
          $sql .= "and field_sirius_domain_target_id is NULL ";
        }

        $sql .= "order by node.title ";

        $stmt = sirius_sql_query($sql, $sql_args);
        $contact_nids = array();
        while ($hr = $stmt->fetchAssoc()) { $contact_nids[] = $hr['nid']; }
        $contact_nodes = node_load_multiple($contact_nids);

        if ($contact_nodes) {
          foreach ($contact_nodes as $contact_nid => $contact_node) {
            $row = array();
            $row['pk'] = "$employer_nid|$contact_type_tid|$contact_nid";
            $row['contact_nid'] = $contact_nid;
            $row['employer_nid'] = $employer_nid;
            $row['employer_title'] = sirius_node_title($employer_nid);
            $row['contact_title'] = $contact_node->title;
            $row['contact_phone'] = $contact_node->field_grievance_co_phone['und'][0]['value'];
            $row['contact_email'] = $contact_node->field_grievance_co_email['und'][0]['email'];
            $row['contact_type_tid'] = $contact_type_tid;
            $row['contact_type_name'] = $contact_type_name;
            $rows['rows'][$row['pk']] = $row;
          }
        } else {
          $row = array();
          $row['pk'] = "$employer_nid|$contact_type_tid|";
          $row['employer_nid'] = $employer_nid;
          $row['employer_title'] = sirius_node_title($employer_nid);
          $row['contact_type_tid'] = $contact_type_tid;
          $row['contact_type_name'] = $contact_type_name;
          $rows['rows'][$row['pk']] = $row;
        }
      }
    }

    return $rows;
  }

  public function render_parameters($parameters) {
    return '';
    /*
    $parameters = $parameters['parameters'];
    $html .= sirius_fakerender_field('Contact Type:', sirius_term_title($parameters['contact_type']));
    return $html;
    */
  }
}
