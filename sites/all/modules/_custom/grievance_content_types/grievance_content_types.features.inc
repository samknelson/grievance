<?php

/**
 * @file
 * grievance_content_types.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function grievance_content_types_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "ds" && $api == "ds") {
    return array("version" => "1");
  }
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "field_validation" && $api == "default_field_validation_rules") {
    return array("version" => "2");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function grievance_content_types_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_flag_default_flags().
 */
function grievance_content_types_flag_default_flags() {
  $flags = array();
  // Exported flag: "Bookmarks".
  $flags['bookmarks'] = array(
    'entity_type' => 'node',
    'title' => 'Bookmarks',
    'global' => 0,
    'types' => array(),
    'flag_short' => '<span class="unicode_checkbox">&#9744;</span> Bookmark',
    'flag_long' => 'Add a bookmark to this item.',
    'flag_message' => '',
    'unflag_short' => '<span class="unicode_checkbox">&#9745;</span> Bookmark',
    'unflag_long' => 'Remove the bookmark for this item.',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'weight' => 0,
    'show_in_links' => array(
      'full' => 0,
      'teaser' => 0,
      'rss' => 0,
      'diff_standard' => 0,
      'token' => 0,
      'sirius_worker' => 0,
      'grievance_arbitrationform' => 0,
      'grievance_form_226' => 0,
      'grievance_gform' => 0,
      'grievance_gform_a' => 0,
      'revision' => 0,
    ),
    'show_as_field' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => 0,
    'i18n' => 0,
    'module' => 'grievance_content_types',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  return $flags;

}

/**
 * Implements hook_image_default_styles().
 */
function grievance_content_types_image_default_styles() {
  $styles = array();

  // Exported image style: sirius_signature_preview.
  $styles['sirius_signature_preview'] = array(
    'label' => 'Sirius Signature Preview',
    'effects' => array(
      2 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 450,
          'height' => 90,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function grievance_content_types_node_info() {
  $items = array(
    'grievance' => array(
      'name' => t('Grievance'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_attachment' => array(
      'name' => t('Grievance Attachment'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_basic_page' => array(
      'name' => t('Grievance Basic Page'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_chapter' => array(
      'name' => t('Grievance Chapter'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Chapter Name'),
      'help' => '',
    ),
    'grievance_company' => array(
      'name' => t('Grievance Company'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Company Name'),
      'help' => '',
    ),
    'grievance_contract' => array(
      'name' => t('Grievance Contract'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_contract_clause' => array(
      'name' => t('Grievance Contract Section'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Brief Statement'),
      'help' => '',
    ),
    'grievance_contract_rule' => array(
      'name' => t('Grievance Contract Rule'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_contract_section' => array(
      'name' => t('Grievance Contract Article'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_contract_template' => array(
      'name' => t('Grievance Contract Template'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_field_overrides' => array(
      'name' => t('Grievance Field Overrides'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_holiday' => array(
      'name' => t('Grievance Holiday'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_irset' => array(
      'name' => t('Grievance Information Request Set'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_letter_template' => array(
      'name' => t('Grievance Letter Template'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_log' => array(
      'name' => t('Grievance Log'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_settlement' => array(
      'name' => t('Grievance Settlement'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'grievance_shop' => array(
      'name' => t('Grievance Shop'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Shop Name'),
      'help' => '',
    ),
    'grievance_shop_contact' => array(
      'name' => t('Grievance Shop Contact'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_bulk' => array(
      'name' => t('Sirius Bulk'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_callerid' => array(
      'name' => t('Sirius Caller ID'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_contact' => array(
      'name' => t('Sirius Contact'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_contact_relationship' => array(
      'name' => t('Sirius Contact Relationship'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_dispatch' => array(
      'name' => t('Sirius Dispatch'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_dispatch_facility' => array(
      'name' => t('Sirius Dispatch Facility'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_dispatch_facility_hall' => array(
      'name' => t('Sirius Dispatch Facility Hall'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_dispatch_job' => array(
      'name' => t('Sirius Dispatch Job'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_dispatch_job_group' => array(
      'name' => t('Sirius Dispatch Job Group'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_domain' => array(
      'name' => t('Sirius Domain'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_edls_sheet' => array(
      'name' => t('Sirius EDLS Sheet'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_employee' => array(
      'name' => t('Sirius Employee'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_employer_payperiod' => array(
      'name' => t('Sirius Employer Payperiod'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_event' => array(
      'name' => t('Sirius Event'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_event_participant' => array(
      'name' => t('Sirius Event Participant'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_feed' => array(
      'name' => t('Sirius Feed'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_help' => array(
      'name' => t('Sirius Help'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_json_definition' => array(
      'name' => t('Sirius JSON Definition'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_ledger_account' => array(
      'name' => t('Sirius Ledger Account'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_letterhead' => array(
      'name' => t('Sirius Letterhead'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_log' => array(
      'name' => t('Sirius Log'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_news' => array(
      'name' => t('Sirius News'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_payment' => array(
      'name' => t('Sirius Payment'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_payperiod' => array(
      'name' => t('Sirius Payperiod'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_phonenumber' => array(
      'name' => t('Sirius Phonenumber'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_term_proxy' => array(
      'name' => t('Sirius Term Proxy'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_trust_benefit' => array(
      'name' => t('Sirius Trust Benefit'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_trust_provider' => array(
      'name' => t('Sirius Trust Provider'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_trust_service' => array(
      'name' => t('Sirius Trust Service'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_trust_worker_benefit' => array(
      'name' => t('Sirius Trust Worker Benefit'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_trust_worker_election' => array(
      'name' => t('Sirius Trust Worker Election'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'sirius_worker' => array(
      'name' => t('Sirius Worker'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
