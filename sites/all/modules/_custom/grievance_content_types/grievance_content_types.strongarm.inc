<?php

/**
 * @file
 * grievance_content_types.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function grievance_content_types_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_grievance';
  $strongarm->value = '[node:field_grievance_shop] - [node:field_grievance_first_name] [node:field_grievance_last_name]';
  $export['ant_pattern_grievance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_grievance_announcement';
  $strongarm->value = '';
  $export['ant_pattern_grievance_announcement'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_grievance_attachment';
  $strongarm->value = '[node:field_grievance] - [node:field_grievance_document_type]';
  $export['ant_pattern_grievance_attachment'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_grievance_company';
  $strongarm->value = '';
  $export['ant_pattern_grievance_company'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_grievance_contract';
  $strongarm->value = '';
  $export['ant_pattern_grievance_contract'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_grievance_contract_clause';
  $strongarm->value = '';
  $export['ant_pattern_grievance_contract_clause'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_grievance_contract_rule';
  $strongarm->value = '[node:field_grievance_contract_tplt] - [node:field_grievance_status]';
  $export['ant_pattern_grievance_contract_rule'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_grievance_contract_section';
  $strongarm->value = '';
  $export['ant_pattern_grievance_contract_section'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_grievance_contract_template';
  $strongarm->value = '';
  $export['ant_pattern_grievance_contract_template'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_grievance_field_overrides';
  $strongarm->value = '[node:field_grievance_entity_type] - [node:field_grievance_bundle] - [node:field_grievance_field_name]';
  $export['ant_pattern_grievance_field_overrides'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_grievance_holiday';
  $strongarm->value = '';
  $export['ant_pattern_grievance_holiday'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_grievance_log';
  $strongarm->value = '';
  $export['ant_pattern_grievance_log'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_grievance_news';
  $strongarm->value = '';
  $export['ant_pattern_grievance_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_grievance_settlement';
  $strongarm->value = '[node:field_grievance] - Settlement';
  $export['ant_pattern_grievance_settlement'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_grievance_shop';
  $strongarm->value = '';
  $export['ant_pattern_grievance_shop'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_grievance_shop_contact';
  $strongarm->value = '[node:field_grievance_co_name]';
  $export['ant_pattern_grievance_shop_contact'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_member';
  $strongarm->value = '';
  $export['ant_pattern_member'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_sirius_bulk';
  $strongarm->value = '';
  $export['ant_pattern_sirius_bulk'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_sirius_dispatch';
  $strongarm->value = '[node:field_sirius_worker] - Dispatch #[node:nid]';
  $export['ant_pattern_sirius_dispatch'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_sirius_dispatch_facility';
  $strongarm->value = '';
  $export['ant_pattern_sirius_dispatch_facility'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_sirius_dispatch_facility_hall';
  $strongarm->value = '';
  $export['ant_pattern_sirius_dispatch_facility_hall'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_sirius_dispatch_job';
  $strongarm->value = '[node:field_grievance_shop] - [node:field_sirius_dispatch_job_type] - [node:nid]';
  $export['ant_pattern_sirius_dispatch_job'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_sirius_dispatch_job_group';
  $strongarm->value = '';
  $export['ant_pattern_sirius_dispatch_job_group'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_sirius_field_overrides';
  $strongarm->value = '';
  $export['ant_pattern_sirius_field_overrides'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_sirius_help';
  $strongarm->value = '';
  $export['ant_pattern_sirius_help'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_sirius_log';
  $strongarm->value = '';
  $export['ant_pattern_sirius_log'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_sirius_news';
  $strongarm->value = '';
  $export['ant_pattern_sirius_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_sirius_skill';
  $strongarm->value = '';
  $export['ant_pattern_sirius_skill'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_sirius_twilio_conversation';
  $strongarm->value = '[node:field_sirius_phone] - [node:created]';
  $export['ant_pattern_sirius_twilio_conversation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_sirius_twilio_voice_conversation';
  $strongarm->value = '[node:field_sirius_phone] - [node:created]';
  $export['ant_pattern_sirius_twilio_voice_conversation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_sirius_worker';
  $strongarm->value = '[node:field_sirius_name]';
  $export['ant_pattern_sirius_worker'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_sirius_worker_skill';
  $strongarm->value = '';
  $export['ant_pattern_sirius_worker_skill'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_grievance';
  $strongarm->value = 0;
  $export['ant_php_grievance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_grievance_announcement';
  $strongarm->value = 0;
  $export['ant_php_grievance_announcement'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_grievance_attachment';
  $strongarm->value = 0;
  $export['ant_php_grievance_attachment'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_grievance_company';
  $strongarm->value = 0;
  $export['ant_php_grievance_company'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_grievance_contract';
  $strongarm->value = 0;
  $export['ant_php_grievance_contract'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_grievance_contract_clause';
  $strongarm->value = 0;
  $export['ant_php_grievance_contract_clause'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_grievance_contract_rule';
  $strongarm->value = 0;
  $export['ant_php_grievance_contract_rule'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_grievance_contract_section';
  $strongarm->value = 0;
  $export['ant_php_grievance_contract_section'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_grievance_contract_template';
  $strongarm->value = 0;
  $export['ant_php_grievance_contract_template'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_grievance_field_overrides';
  $strongarm->value = 0;
  $export['ant_php_grievance_field_overrides'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_grievance_holiday';
  $strongarm->value = 0;
  $export['ant_php_grievance_holiday'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_grievance_log';
  $strongarm->value = 0;
  $export['ant_php_grievance_log'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_grievance_news';
  $strongarm->value = 0;
  $export['ant_php_grievance_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_grievance_settlement';
  $strongarm->value = 0;
  $export['ant_php_grievance_settlement'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_grievance_shop';
  $strongarm->value = 0;
  $export['ant_php_grievance_shop'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_grievance_shop_contact';
  $strongarm->value = 0;
  $export['ant_php_grievance_shop_contact'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_member';
  $strongarm->value = 0;
  $export['ant_php_member'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_sirius_bulk';
  $strongarm->value = 0;
  $export['ant_php_sirius_bulk'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_sirius_dispatch';
  $strongarm->value = 0;
  $export['ant_php_sirius_dispatch'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_sirius_dispatch_facility';
  $strongarm->value = 0;
  $export['ant_php_sirius_dispatch_facility'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_sirius_dispatch_facility_hall';
  $strongarm->value = 0;
  $export['ant_php_sirius_dispatch_facility_hall'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_sirius_dispatch_job';
  $strongarm->value = 0;
  $export['ant_php_sirius_dispatch_job'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_sirius_dispatch_job_group';
  $strongarm->value = 0;
  $export['ant_php_sirius_dispatch_job_group'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_sirius_field_overrides';
  $strongarm->value = 0;
  $export['ant_php_sirius_field_overrides'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_sirius_help';
  $strongarm->value = 0;
  $export['ant_php_sirius_help'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_sirius_log';
  $strongarm->value = 0;
  $export['ant_php_sirius_log'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_sirius_news';
  $strongarm->value = 0;
  $export['ant_php_sirius_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_sirius_skill';
  $strongarm->value = 0;
  $export['ant_php_sirius_skill'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_sirius_twilio_conversation';
  $strongarm->value = 0;
  $export['ant_php_sirius_twilio_conversation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_sirius_twilio_voice_conversation';
  $strongarm->value = 0;
  $export['ant_php_sirius_twilio_voice_conversation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_sirius_worker';
  $strongarm->value = 0;
  $export['ant_php_sirius_worker'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_sirius_worker_skill';
  $strongarm->value = 0;
  $export['ant_php_sirius_worker_skill'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_sirius_bulk';
  $strongarm->value = '0';
  $export['ant_sirius_bulk'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_sirius_dispatch';
  $strongarm->value = '1';
  $export['ant_sirius_dispatch'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_sirius_dispatch_facility';
  $strongarm->value = '0';
  $export['ant_sirius_dispatch_facility'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_sirius_dispatch_facility_hall';
  $strongarm->value = '0';
  $export['ant_sirius_dispatch_facility_hall'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_sirius_dispatch_job';
  $strongarm->value = '1';
  $export['ant_sirius_dispatch_job'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_sirius_dispatch_job_group';
  $strongarm->value = '0';
  $export['ant_sirius_dispatch_job_group'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_sirius_field_overrides';
  $strongarm->value = '0';
  $export['ant_sirius_field_overrides'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_sirius_help';
  $strongarm->value = '0';
  $export['ant_sirius_help'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_sirius_log';
  $strongarm->value = '0';
  $export['ant_sirius_log'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_sirius_news';
  $strongarm->value = '0';
  $export['ant_sirius_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_sirius_skill';
  $strongarm->value = '0';
  $export['ant_sirius_skill'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_sirius_twilio_conversation';
  $strongarm->value = '1';
  $export['ant_sirius_twilio_conversation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_sirius_twilio_voice_conversation';
  $strongarm->value = '1';
  $export['ant_sirius_twilio_voice_conversation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_sirius_worker';
  $strongarm->value = '1';
  $export['ant_sirius_worker'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_sirius_worker_skill';
  $strongarm->value = '0';
  $export['ant_sirius_worker_skill'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_grievance';
  $strongarm->value = 0;
  $export['comment_anonymous_grievance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_grievance_basic_page';
  $strongarm->value = 0;
  $export['comment_anonymous_grievance_basic_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_grievance_shop';
  $strongarm->value = 0;
  $export['comment_anonymous_grievance_shop'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_grievance';
  $strongarm->value = 0;
  $export['comment_default_mode_grievance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_grievance_basic_page';
  $strongarm->value = 1;
  $export['comment_default_mode_grievance_basic_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_grievance_shop';
  $strongarm->value = 1;
  $export['comment_default_mode_grievance_shop'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_grievance';
  $strongarm->value = '50';
  $export['comment_default_per_page_grievance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_grievance_basic_page';
  $strongarm->value = '50';
  $export['comment_default_per_page_grievance_basic_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_grievance_shop';
  $strongarm->value = '50';
  $export['comment_default_per_page_grievance_shop'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_grievance';
  $strongarm->value = 1;
  $export['comment_form_location_grievance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_grievance_basic_page';
  $strongarm->value = 1;
  $export['comment_form_location_grievance_basic_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_grievance_shop';
  $strongarm->value = 1;
  $export['comment_form_location_grievance_shop'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_grievance';
  $strongarm->value = '2';
  $export['comment_grievance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_grievance_basic_page';
  $strongarm->value = '1';
  $export['comment_grievance_basic_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_grievance_shop';
  $strongarm->value = '1';
  $export['comment_grievance_shop'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_grievance';
  $strongarm->value = '1';
  $export['comment_preview_grievance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_grievance_basic_page';
  $strongarm->value = '1';
  $export['comment_preview_grievance_basic_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_grievance_shop';
  $strongarm->value = '1';
  $export['comment_preview_grievance_shop'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_grievance';
  $strongarm->value = 1;
  $export['comment_subject_field_grievance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_grievance_basic_page';
  $strongarm->value = 1;
  $export['comment_subject_field_grievance_basic_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_grievance_shop';
  $strongarm->value = 1;
  $export['comment_subject_field_grievance_shop'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'diff_enable_revisions_page_node_grievance';
  $strongarm->value = 0;
  $export['diff_enable_revisions_page_node_grievance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'diff_enable_revisions_page_node_sirius_letterhead';
  $strongarm->value = 0;
  $export['diff_enable_revisions_page_node_sirius_letterhead'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'diff_show_preview_changes_node_grievance';
  $strongarm->value = 0;
  $export['diff_show_preview_changes_node_grievance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'diff_show_preview_changes_node_sirius_letterhead';
  $strongarm->value = 0;
  $export['diff_show_preview_changes_node_sirius_letterhead'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'diff_view_mode_preview_node_grievance';
  $strongarm->value = 'full';
  $export['diff_view_mode_preview_node_grievance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'diff_view_mode_preview_node_sirius_letterhead';
  $strongarm->value = 'full';
  $export['diff_view_mode_preview_node_sirius_letterhead'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__grievance';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => TRUE,
      ),
      'revision' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform' => array(
        'custom_settings' => TRUE,
      ),
      'grievance_arbitrationform' => array(
        'custom_settings' => TRUE,
      ),
      'grievance_gform_a' => array(
        'custom_settings' => TRUE,
      ),
      'grievance_form_226' => array(
        'custom_settings' => TRUE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '29',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__grievance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__grievance_attachment';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_arbitrationform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform_a' => array(
        'custom_settings' => FALSE,
      ),
      'revision' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_form_226' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '1',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__grievance_attachment'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__grievance_basic_page';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '-5',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__grievance_basic_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__grievance_chapter';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_arbitrationform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_form_226' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform_a' => array(
        'custom_settings' => FALSE,
      ),
      'revision' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__grievance_chapter'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__grievance_contract';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__grievance_contract'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__grievance_contract_clause';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_arbitrationform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform_a' => array(
        'custom_settings' => FALSE,
      ),
      'revision' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_form_226' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '2',
        ),
        'path' => array(
          'weight' => '30',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__grievance_contract_clause'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__grievance_contract_rule';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__grievance_contract_rule'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__grievance_contract_section';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_arbitrationform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_form_226' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform_a' => array(
        'custom_settings' => FALSE,
      ),
      'revision' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '2',
        ),
        'path' => array(
          'weight' => '30',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__grievance_contract_section'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__grievance_contract_template';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_arbitrationform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform_a' => array(
        'custom_settings' => FALSE,
      ),
      'revision' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_form_226' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__grievance_contract_template'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__grievance_field_overrides';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_arbitrationform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform_a' => array(
        'custom_settings' => FALSE,
      ),
      'revision' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__grievance_field_overrides'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__grievance_holiday';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '-5',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__grievance_holiday'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__grievance_letter_template';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_arbitrationform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_form_226' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform_a' => array(
        'custom_settings' => FALSE,
      ),
      'revision' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__grievance_letter_template'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__grievance_log';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_arbitrationform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform' => array(
        'custom_settings' => FALSE,
      ),
      'revision' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform_a' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_form_226' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__grievance_log'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__grievance_news';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '-5',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__grievance_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__grievance_settlement';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_arbitrationform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform_a' => array(
        'custom_settings' => FALSE,
      ),
      'revision' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_form_226' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__grievance_settlement'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__grievance_shop';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'revision' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_arbitrationform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform_a' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_form_226' => array(
        'custom_settings' => FALSE,
      ),
      'sirius_worker' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
        'path' => array(
          'weight' => '3',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__grievance_shop'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__grievance_shop_contact';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_arbitrationform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform_a' => array(
        'custom_settings' => FALSE,
      ),
      'revision' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_form_226' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '5',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__grievance_shop_contact'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__sirius_bulk';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'sirius_worker' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_arbitrationform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_form_226' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform_a' => array(
        'custom_settings' => FALSE,
      ),
      'revision' => array(
        'custom_settings' => FALSE,
      ),
      'diff_standard' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '2',
        ),
        'path' => array(
          'weight' => '3',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__sirius_bulk'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__sirius_dispatch';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_arbitrationform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_form_226' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform_a' => array(
        'custom_settings' => FALSE,
      ),
      'revision' => array(
        'custom_settings' => FALSE,
      ),
      'sirius_worker' => array(
        'custom_settings' => TRUE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
        'path' => array(
          'weight' => '1',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__sirius_dispatch'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__sirius_dispatch_facility';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_arbitrationform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_form_226' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform_a' => array(
        'custom_settings' => FALSE,
      ),
      'revision' => array(
        'custom_settings' => FALSE,
      ),
      'sirius_worker' => array(
        'custom_settings' => TRUE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
        'path' => array(
          'weight' => '5',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__sirius_dispatch_facility'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__sirius_dispatch_facility_hall';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_arbitrationform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_form_226' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform_a' => array(
        'custom_settings' => FALSE,
      ),
      'revision' => array(
        'custom_settings' => FALSE,
      ),
      'sirius_worker' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
        'path' => array(
          'weight' => '4',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__sirius_dispatch_facility_hall'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__sirius_dispatch_job';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_arbitrationform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_form_226' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform_a' => array(
        'custom_settings' => FALSE,
      ),
      'revision' => array(
        'custom_settings' => FALSE,
      ),
      'sirius_worker' => array(
        'custom_settings' => TRUE,
      ),
      'diff_standard' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
        'path' => array(
          'weight' => '7',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__sirius_dispatch_job'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__sirius_dispatch_job_group';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'sirius_worker' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_arbitrationform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_form_226' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform_a' => array(
        'custom_settings' => FALSE,
      ),
      'revision' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
        'path' => array(
          'weight' => '1',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__sirius_dispatch_job_group'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__sirius_help';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_arbitrationform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_form_226' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform_a' => array(
        'custom_settings' => FALSE,
      ),
      'revision' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__sirius_help'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__sirius_letterhead';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__sirius_letterhead'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__sirius_log';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_arbitrationform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_form_226' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform_a' => array(
        'custom_settings' => FALSE,
      ),
      'revision' => array(
        'custom_settings' => FALSE,
      ),
      'sirius_worker' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
        'path' => array(
          'weight' => '1',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__sirius_log'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__sirius_news';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'sirius_worker' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_arbitrationform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_form_226' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform_a' => array(
        'custom_settings' => FALSE,
      ),
      'revision' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
        'path' => array(
          'weight' => '1',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__sirius_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__sirius_worker';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_arbitrationform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_form_226' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform' => array(
        'custom_settings' => FALSE,
      ),
      'grievance_gform_a' => array(
        'custom_settings' => FALSE,
      ),
      'revision' => array(
        'custom_settings' => FALSE,
      ),
      'sirius_worker' => array(
        'custom_settings' => TRUE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
        'path' => array(
          'weight' => '1',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__sirius_worker'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_extended_grievance';
  $strongarm->value = 1;
  $export['i18n_node_extended_grievance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_extended_grievance_attachment';
  $strongarm->value = 1;
  $export['i18n_node_extended_grievance_attachment'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_extended_grievance_chapter';
  $strongarm->value = 1;
  $export['i18n_node_extended_grievance_chapter'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_extended_grievance_contract_clause';
  $strongarm->value = 1;
  $export['i18n_node_extended_grievance_contract_clause'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_extended_grievance_contract_rule';
  $strongarm->value = 1;
  $export['i18n_node_extended_grievance_contract_rule'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_extended_grievance_contract_section';
  $strongarm->value = 1;
  $export['i18n_node_extended_grievance_contract_section'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_extended_grievance_field_overrides';
  $strongarm->value = 1;
  $export['i18n_node_extended_grievance_field_overrides'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_extended_grievance_holiday';
  $strongarm->value = 1;
  $export['i18n_node_extended_grievance_holiday'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_extended_grievance_letter_template';
  $strongarm->value = 1;
  $export['i18n_node_extended_grievance_letter_template'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_extended_grievance_log';
  $strongarm->value = 1;
  $export['i18n_node_extended_grievance_log'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_extended_grievance_news';
  $strongarm->value = 1;
  $export['i18n_node_extended_grievance_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_extended_grievance_settlement';
  $strongarm->value = 1;
  $export['i18n_node_extended_grievance_settlement'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_extended_grievance_shop_contact';
  $strongarm->value = 1;
  $export['i18n_node_extended_grievance_shop_contact'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_options_grievance';
  $strongarm->value = array();
  $export['i18n_node_options_grievance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_options_grievance_attachment';
  $strongarm->value = array();
  $export['i18n_node_options_grievance_attachment'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_options_grievance_chapter';
  $strongarm->value = array();
  $export['i18n_node_options_grievance_chapter'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_options_grievance_contract_clause';
  $strongarm->value = array();
  $export['i18n_node_options_grievance_contract_clause'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_options_grievance_contract_rule';
  $strongarm->value = array();
  $export['i18n_node_options_grievance_contract_rule'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_options_grievance_contract_section';
  $strongarm->value = array();
  $export['i18n_node_options_grievance_contract_section'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_options_grievance_field_overrides';
  $strongarm->value = array();
  $export['i18n_node_options_grievance_field_overrides'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_options_grievance_holiday';
  $strongarm->value = array();
  $export['i18n_node_options_grievance_holiday'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_options_grievance_letter_template';
  $strongarm->value = array();
  $export['i18n_node_options_grievance_letter_template'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_options_grievance_log';
  $strongarm->value = array();
  $export['i18n_node_options_grievance_log'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_options_grievance_news';
  $strongarm->value = array();
  $export['i18n_node_options_grievance_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_options_grievance_settlement';
  $strongarm->value = array();
  $export['i18n_node_options_grievance_settlement'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_options_grievance_shop_contact';
  $strongarm->value = array();
  $export['i18n_node_options_grievance_shop_contact'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_grievance';
  $strongarm->value = '0';
  $export['language_content_type_grievance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_grievance_attachment';
  $strongarm->value = '0';
  $export['language_content_type_grievance_attachment'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_grievance_chapter';
  $strongarm->value = '0';
  $export['language_content_type_grievance_chapter'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_grievance_company';
  $strongarm->value = '0';
  $export['language_content_type_grievance_company'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_grievance_contract';
  $strongarm->value = '0';
  $export['language_content_type_grievance_contract'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_grievance_contract_clause';
  $strongarm->value = '0';
  $export['language_content_type_grievance_contract_clause'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_grievance_contract_rule';
  $strongarm->value = '0';
  $export['language_content_type_grievance_contract_rule'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_grievance_contract_section';
  $strongarm->value = '0';
  $export['language_content_type_grievance_contract_section'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_grievance_contract_template';
  $strongarm->value = '0';
  $export['language_content_type_grievance_contract_template'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_grievance_field_overrides';
  $strongarm->value = '0';
  $export['language_content_type_grievance_field_overrides'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_grievance_holiday';
  $strongarm->value = '0';
  $export['language_content_type_grievance_holiday'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_grievance_letter_template';
  $strongarm->value = '0';
  $export['language_content_type_grievance_letter_template'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_grievance_log';
  $strongarm->value = '0';
  $export['language_content_type_grievance_log'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_grievance_news';
  $strongarm->value = '0';
  $export['language_content_type_grievance_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_grievance_settlement';
  $strongarm->value = '0';
  $export['language_content_type_grievance_settlement'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_grievance_shop_contact';
  $strongarm->value = '0';
  $export['language_content_type_grievance_shop_contact'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_sirius_bulk';
  $strongarm->value = '0';
  $export['language_content_type_sirius_bulk'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_sirius_dispatch';
  $strongarm->value = '0';
  $export['language_content_type_sirius_dispatch'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_sirius_dispatch_facility';
  $strongarm->value = '0';
  $export['language_content_type_sirius_dispatch_facility'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_sirius_dispatch_facility_hall';
  $strongarm->value = '0';
  $export['language_content_type_sirius_dispatch_facility_hall'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_sirius_dispatch_job';
  $strongarm->value = '0';
  $export['language_content_type_sirius_dispatch_job'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_sirius_dispatch_job_group';
  $strongarm->value = '0';
  $export['language_content_type_sirius_dispatch_job_group'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_sirius_help';
  $strongarm->value = '0';
  $export['language_content_type_sirius_help'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_sirius_letterhead';
  $strongarm->value = '0';
  $export['language_content_type_sirius_letterhead'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_sirius_log';
  $strongarm->value = '0';
  $export['language_content_type_sirius_log'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_sirius_news';
  $strongarm->value = '0';
  $export['language_content_type_sirius_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_sirius_worker';
  $strongarm->value = '0';
  $export['language_content_type_sirius_worker'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_grievance';
  $strongarm->value = array();
  $export['menu_options_grievance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_grievance_attachment';
  $strongarm->value = array();
  $export['menu_options_grievance_attachment'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_grievance_basic_page';
  $strongarm->value = array();
  $export['menu_options_grievance_basic_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_grievance_chapter';
  $strongarm->value = array();
  $export['menu_options_grievance_chapter'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_grievance_company';
  $strongarm->value = array();
  $export['menu_options_grievance_company'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_grievance_contract';
  $strongarm->value = array();
  $export['menu_options_grievance_contract'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_grievance_contract_clause';
  $strongarm->value = array();
  $export['menu_options_grievance_contract_clause'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_grievance_contract_rule';
  $strongarm->value = array();
  $export['menu_options_grievance_contract_rule'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_grievance_contract_section';
  $strongarm->value = array();
  $export['menu_options_grievance_contract_section'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_grievance_contract_template';
  $strongarm->value = array();
  $export['menu_options_grievance_contract_template'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_grievance_field_overrides';
  $strongarm->value = array();
  $export['menu_options_grievance_field_overrides'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_grievance_holiday';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_grievance_holiday'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_grievance_letter_template';
  $strongarm->value = array();
  $export['menu_options_grievance_letter_template'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_grievance_log';
  $strongarm->value = array();
  $export['menu_options_grievance_log'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_grievance_news';
  $strongarm->value = array();
  $export['menu_options_grievance_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_grievance_settlement';
  $strongarm->value = array();
  $export['menu_options_grievance_settlement'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_grievance_shop';
  $strongarm->value = array();
  $export['menu_options_grievance_shop'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_grievance_shop_contact';
  $strongarm->value = array();
  $export['menu_options_grievance_shop_contact'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_sirius_bulk';
  $strongarm->value = array();
  $export['menu_options_sirius_bulk'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_sirius_dispatch';
  $strongarm->value = array();
  $export['menu_options_sirius_dispatch'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_sirius_dispatch_facility';
  $strongarm->value = array();
  $export['menu_options_sirius_dispatch_facility'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_sirius_dispatch_facility_hall';
  $strongarm->value = array();
  $export['menu_options_sirius_dispatch_facility_hall'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_sirius_dispatch_job';
  $strongarm->value = array();
  $export['menu_options_sirius_dispatch_job'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_sirius_dispatch_job_group';
  $strongarm->value = array();
  $export['menu_options_sirius_dispatch_job_group'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_sirius_help';
  $strongarm->value = array();
  $export['menu_options_sirius_help'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_sirius_letterhead';
  $strongarm->value = array();
  $export['menu_options_sirius_letterhead'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_sirius_log';
  $strongarm->value = array();
  $export['menu_options_sirius_log'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_sirius_news';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_sirius_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_sirius_worker';
  $strongarm->value = array();
  $export['menu_options_sirius_worker'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_grievance';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_grievance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_grievance_attachment';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_grievance_attachment'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_grievance_basic_page';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_grievance_basic_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_grievance_chapter';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_grievance_chapter'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_grievance_company';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_grievance_company'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_grievance_contract';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_grievance_contract'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_grievance_contract_clause';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_grievance_contract_clause'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_grievance_contract_rule';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_grievance_contract_rule'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_grievance_contract_section';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_grievance_contract_section'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_grievance_contract_template';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_grievance_contract_template'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_grievance_field_overrides';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_grievance_field_overrides'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_grievance_holiday';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_grievance_holiday'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_grievance_letter_template';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_grievance_letter_template'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_grievance_log';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_grievance_log'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_grievance_news';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_grievance_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_grievance_settlement';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_grievance_settlement'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_grievance_shop';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_grievance_shop'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_grievance_shop_contact';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_grievance_shop_contact'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_sirius_bulk';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_sirius_bulk'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_sirius_dispatch';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_sirius_dispatch'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_sirius_dispatch_facility';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_sirius_dispatch_facility'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_sirius_dispatch_facility_hall';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_sirius_dispatch_facility_hall'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_sirius_dispatch_job';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_sirius_dispatch_job'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_sirius_dispatch_job_group';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_sirius_dispatch_job_group'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_sirius_help';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_sirius_help'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_sirius_letterhead';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_sirius_letterhead'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_sirius_log';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_sirius_log'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_sirius_news';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_sirius_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_sirius_worker';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_sirius_worker'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_grievance';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_grievance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_grievance_attachment';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_grievance_attachment'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_grievance_basic_page';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_grievance_basic_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_grievance_chapter';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_grievance_chapter'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_grievance_company';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_grievance_company'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_grievance_contract';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_grievance_contract'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_grievance_contract_clause';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_grievance_contract_clause'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_grievance_contract_rule';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_grievance_contract_rule'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_grievance_contract_section';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_grievance_contract_section'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_grievance_contract_template';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_grievance_contract_template'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_grievance_field_overrides';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_grievance_field_overrides'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_grievance_holiday';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_grievance_holiday'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_grievance_letter_template';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_grievance_letter_template'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_grievance_log';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_grievance_log'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_grievance_news';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_grievance_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_grievance_settlement';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_grievance_settlement'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_grievance_shop';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_grievance_shop'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_grievance_shop_contact';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_grievance_shop_contact'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_sirius_bulk';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_sirius_bulk'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_sirius_dispatch';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_sirius_dispatch'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_sirius_dispatch_facility';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_sirius_dispatch_facility'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_sirius_dispatch_facility_hall';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_sirius_dispatch_facility_hall'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_sirius_dispatch_job';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_sirius_dispatch_job'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_sirius_dispatch_job_group';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_sirius_dispatch_job_group'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_sirius_help';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_sirius_help'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_sirius_letterhead';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_sirius_letterhead'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_sirius_log';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_sirius_log'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_sirius_news';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_sirius_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_sirius_worker';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_sirius_worker'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_grievance';
  $strongarm->value = '0';
  $export['node_preview_grievance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_grievance_attachment';
  $strongarm->value = '0';
  $export['node_preview_grievance_attachment'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_grievance_basic_page';
  $strongarm->value = '1';
  $export['node_preview_grievance_basic_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_grievance_chapter';
  $strongarm->value = '0';
  $export['node_preview_grievance_chapter'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_grievance_company';
  $strongarm->value = '0';
  $export['node_preview_grievance_company'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_grievance_contract';
  $strongarm->value = '0';
  $export['node_preview_grievance_contract'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_grievance_contract_clause';
  $strongarm->value = '0';
  $export['node_preview_grievance_contract_clause'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_grievance_contract_rule';
  $strongarm->value = '0';
  $export['node_preview_grievance_contract_rule'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_grievance_contract_section';
  $strongarm->value = '0';
  $export['node_preview_grievance_contract_section'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_grievance_contract_template';
  $strongarm->value = '0';
  $export['node_preview_grievance_contract_template'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_grievance_field_overrides';
  $strongarm->value = '0';
  $export['node_preview_grievance_field_overrides'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_grievance_holiday';
  $strongarm->value = '1';
  $export['node_preview_grievance_holiday'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_grievance_letter_template';
  $strongarm->value = '0';
  $export['node_preview_grievance_letter_template'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_grievance_log';
  $strongarm->value = '0';
  $export['node_preview_grievance_log'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_grievance_news';
  $strongarm->value = '0';
  $export['node_preview_grievance_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_grievance_settlement';
  $strongarm->value = '1';
  $export['node_preview_grievance_settlement'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_grievance_shop';
  $strongarm->value = '0';
  $export['node_preview_grievance_shop'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_grievance_shop_contact';
  $strongarm->value = '0';
  $export['node_preview_grievance_shop_contact'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_sirius_bulk';
  $strongarm->value = '0';
  $export['node_preview_sirius_bulk'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_sirius_dispatch';
  $strongarm->value = '0';
  $export['node_preview_sirius_dispatch'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_sirius_dispatch_facility';
  $strongarm->value = '0';
  $export['node_preview_sirius_dispatch_facility'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_sirius_dispatch_facility_hall';
  $strongarm->value = '0';
  $export['node_preview_sirius_dispatch_facility_hall'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_sirius_dispatch_job';
  $strongarm->value = '0';
  $export['node_preview_sirius_dispatch_job'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_sirius_dispatch_job_group';
  $strongarm->value = '0';
  $export['node_preview_sirius_dispatch_job_group'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_sirius_help';
  $strongarm->value = '0';
  $export['node_preview_sirius_help'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_sirius_letterhead';
  $strongarm->value = '0';
  $export['node_preview_sirius_letterhead'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_sirius_log';
  $strongarm->value = '0';
  $export['node_preview_sirius_log'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_sirius_news';
  $strongarm->value = '1';
  $export['node_preview_sirius_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_sirius_worker';
  $strongarm->value = '0';
  $export['node_preview_sirius_worker'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_grievance';
  $strongarm->value = 0;
  $export['node_submitted_grievance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_grievance_attachment';
  $strongarm->value = 0;
  $export['node_submitted_grievance_attachment'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_grievance_basic_page';
  $strongarm->value = 0;
  $export['node_submitted_grievance_basic_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_grievance_chapter';
  $strongarm->value = 0;
  $export['node_submitted_grievance_chapter'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_grievance_company';
  $strongarm->value = 0;
  $export['node_submitted_grievance_company'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_grievance_contract';
  $strongarm->value = 0;
  $export['node_submitted_grievance_contract'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_grievance_contract_clause';
  $strongarm->value = 0;
  $export['node_submitted_grievance_contract_clause'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_grievance_contract_rule';
  $strongarm->value = 0;
  $export['node_submitted_grievance_contract_rule'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_grievance_contract_section';
  $strongarm->value = 0;
  $export['node_submitted_grievance_contract_section'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_grievance_contract_template';
  $strongarm->value = 0;
  $export['node_submitted_grievance_contract_template'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_grievance_field_overrides';
  $strongarm->value = 0;
  $export['node_submitted_grievance_field_overrides'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_grievance_holiday';
  $strongarm->value = 1;
  $export['node_submitted_grievance_holiday'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_grievance_letter_template';
  $strongarm->value = 0;
  $export['node_submitted_grievance_letter_template'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_grievance_log';
  $strongarm->value = 0;
  $export['node_submitted_grievance_log'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_grievance_news';
  $strongarm->value = 0;
  $export['node_submitted_grievance_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_grievance_settlement';
  $strongarm->value = 0;
  $export['node_submitted_grievance_settlement'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_grievance_shop';
  $strongarm->value = 0;
  $export['node_submitted_grievance_shop'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_grievance_shop_contact';
  $strongarm->value = 0;
  $export['node_submitted_grievance_shop_contact'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_sirius_bulk';
  $strongarm->value = 0;
  $export['node_submitted_sirius_bulk'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_sirius_dispatch';
  $strongarm->value = 0;
  $export['node_submitted_sirius_dispatch'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_sirius_dispatch_facility';
  $strongarm->value = 0;
  $export['node_submitted_sirius_dispatch_facility'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_sirius_dispatch_facility_hall';
  $strongarm->value = 0;
  $export['node_submitted_sirius_dispatch_facility_hall'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_sirius_dispatch_job';
  $strongarm->value = 0;
  $export['node_submitted_sirius_dispatch_job'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_sirius_dispatch_job_group';
  $strongarm->value = 0;
  $export['node_submitted_sirius_dispatch_job_group'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_sirius_help';
  $strongarm->value = 0;
  $export['node_submitted_sirius_help'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_sirius_letterhead';
  $strongarm->value = 0;
  $export['node_submitted_sirius_letterhead'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_sirius_log';
  $strongarm->value = 0;
  $export['node_submitted_sirius_log'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_sirius_news';
  $strongarm->value = 1;
  $export['node_submitted_sirius_news'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_sirius_worker';
  $strongarm->value = 0;
  $export['node_submitted_sirius_worker'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_grievance_attachment_pattern';
  $strongarm->value = 'grievance/node/[node:content-type]/[node:nid]';
  $export['pathauto_node_grievance_attachment_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_grievance_basic_page_pattern';
  $strongarm->value = 'grievance/node/[node:content-type]/[node:nid]';
  $export['pathauto_node_grievance_basic_page_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_grievance_chapter_pattern';
  $strongarm->value = 'grievance/node/[node:content-type]/[node:nid]';
  $export['pathauto_node_grievance_chapter_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_grievance_company_pattern';
  $strongarm->value = 'grievance/node/[node:content-type]/[node:nid]';
  $export['pathauto_node_grievance_company_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_grievance_contract_clause_pattern';
  $strongarm->value = 'grievance/node/[node:content-type]/[node:nid]';
  $export['pathauto_node_grievance_contract_clause_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_grievance_contract_pattern';
  $strongarm->value = 'grievance/node/[node:content-type]/[node:nid]';
  $export['pathauto_node_grievance_contract_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_grievance_contract_rule_pattern';
  $strongarm->value = 'grievance/node/[node:content-type]/[node:nid]';
  $export['pathauto_node_grievance_contract_rule_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_grievance_contract_section_pattern';
  $strongarm->value = 'grievance/node/[node:content-type]/[node:nid]';
  $export['pathauto_node_grievance_contract_section_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_grievance_contract_template_pattern';
  $strongarm->value = 'grievance/node/[node:content-type]/[node:nid]';
  $export['pathauto_node_grievance_contract_template_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_grievance_field_overrides_pattern';
  $strongarm->value = 'grievance/node/[node:content-type]/[node:nid]';
  $export['pathauto_node_grievance_field_overrides_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_grievance_holiday_pattern';
  $strongarm->value = 'grievance/node/[node:content-type]/[node:nid]';
  $export['pathauto_node_grievance_holiday_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_grievance_letter_template_pattern';
  $strongarm->value = 'grievance/node/[node:content-type]/[node:nid]';
  $export['pathauto_node_grievance_letter_template_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_grievance_log_pattern';
  $strongarm->value = 'grievance/node/[node:content-type]/[node:nid]';
  $export['pathauto_node_grievance_log_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_grievance_news_pattern';
  $strongarm->value = 'grievance/node/[node:content-type]/[node:nid]';
  $export['pathauto_node_grievance_news_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_grievance_pattern';
  $strongarm->value = 'grievance/node/[node:content-type]/[node:nid]';
  $export['pathauto_node_grievance_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_grievance_settlement_pattern';
  $strongarm->value = 'grievance/node/[node:content-type]/[node:nid]';
  $export['pathauto_node_grievance_settlement_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_grievance_shop_contact_pattern';
  $strongarm->value = 'grievance/node/[node:content-type]/[node:nid]';
  $export['pathauto_node_grievance_shop_contact_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_grievance_shop_pattern';
  $strongarm->value = 'grievance/node/[node:content-type]/[node:nid]';
  $export['pathauto_node_grievance_shop_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_sirius_dispatch_facility_hall_pattern';
  $strongarm->value = '[node:content-type]/[node:nid]';
  $export['pathauto_node_sirius_dispatch_facility_hall_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_sirius_dispatch_facility_pattern';
  $strongarm->value = '[node:content-type]/[node:nid]';
  $export['pathauto_node_sirius_dispatch_facility_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_sirius_dispatch_job_pattern';
  $strongarm->value = '[node:content-type]/[node:nid]';
  $export['pathauto_node_sirius_dispatch_job_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_sirius_dispatch_pattern';
  $strongarm->value = '[node:content-type]/[node:nid]';
  $export['pathauto_node_sirius_dispatch_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_sirius_help_pattern';
  $strongarm->value = '[node:content-type]/[node:nid]';
  $export['pathauto_node_sirius_help_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_sirius_worker_pattern';
  $strongarm->value = '[node:content-type]/[node:nid]';
  $export['pathauto_node_sirius_worker_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_alert_types_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_alert_types_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_assignee_notes_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_assignee_notes_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_broughtby_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_broughtby_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_category_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_category_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_contact_types_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_contact_types_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_contract_clause_tags_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_contract_clause_tags_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_contract_section_tags_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_contract_section_tags_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_contract_template_tags_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_contract_template_tags_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_delivery_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_delivery_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_department_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_department_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_document_types_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_document_types_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_job_classification_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_job_classification_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_log_tags_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_log_tags_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_log_types_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_log_types_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_outcome_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_outcome_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_remedies_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_remedies_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_resproc_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_resproc_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_settlement_tags_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_settlement_tags_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_shift_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_shift_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_status_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_status_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_tags_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_tags_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_types_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_types_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_grievance_work_status_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_grievance_work_status_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_sirius_dispatch_job_type_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_sirius_dispatch_job_type_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_sirius_industry_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_sirius_industry_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_sirius_member_status_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_sirius_member_status_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_sirius_skill_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_sirius_skill_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_sirius_work_status_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_sirius_work_status_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'theme_at_sirius_settings';
  $strongarm->value = array(
    'toggle_logo' => 1,
    'toggle_name' => 1,
    'toggle_favicon' => 1,
    'toggle_main_menu' => 1,
    'toggle_secondary_menu' => 1,
    'default_logo' => 1,
    'logo_path' => '',
    'logo_upload' => '',
    'default_favicon' => 0,
    'favicon_path' => 'sites/all/themes/at_sirius/images/favicon.png',
    'favicon_upload' => '',
    'bigscreen_layout' => 'three_col_grail',
    'bigscreen_sidebar_unit' => '%',
    'bigscreen_sidebar_first' => '20',
    'bigscreen_sidebar_second' => '20',
    'bigscreen_page_unit' => '%',
    'bigscreen_page_width' => '90',
    'bigscreen_set_max_width' => 1,
    'bigscreen_max_width_unit' => 'px',
    'bigscreen_max_width' => '1330',
    'bigscreen_media_query' => 'only screen and (min-width:1025px)',
    'tablet_landscape_layout' => 'two_sidebars_right_stack',
    'tablet_landscape_sidebar_unit' => '%',
    'tablet_landscape_sidebar_first' => '20',
    'tablet_landscape_sidebar_second' => '20',
    'tablet_landscape_page_unit' => '%',
    'tablet_landscape_page_width' => '95',
    'tablet_landscape_media_query' => 'only screen and (min-width:769px) and (max-width:1024px)',
    'tablet_portrait_layout' => 'two_sidebars_right_stack',
    'tablet_portrait_sidebar_unit' => '%',
    'tablet_portrait_sidebar_first' => '50',
    'tablet_portrait_sidebar_second' => '50',
    'tablet_portrait_page_unit' => '%',
    'tablet_portrait_page_width' => '100',
    'tablet_portrait_media_query' => 'only screen and (min-width:581px) and (max-width:768px)',
    'smalltouch_landscape_layout' => 'one_col_stack',
    'smalltouch_landscape_sidebar_unit' => '%',
    'smalltouch_landscape_sidebar_first' => '50',
    'smalltouch_landscape_sidebar_second' => '50',
    'smalltouch_landscape_media_query' => 'only screen and (min-width:321px) and (max-width:580px)',
    'smalltouch_landscape_page_width' => '100',
    'smalltouch_landscape_page_unit' => '%',
    'smalltouch_portrait_media_query' => 'only screen and (max-width:320px)',
    'smalltouch_portrait_page_width' => '100',
    'smalltouch_portrait_page_unit' => '%',
    'smalltouch_portrait_sidebar_first' => '100',
    'smalltouch_portrait_sidebar_second' => '100',
    'smalltouch_portrait_sidebar_unit' => '%',
    'smalltouch_portrait_layout' => 'one_col_stack',
    'bigscreen_three_50_25_25' => 'three-50-25-25',
    'bigscreen_three_25_25_50' => 'three-25-25-50',
    'bigscreen_three_3x33' => 'three-3x33',
    'bigscreen_three_25_50_25' => 'three-25-50-25',
    'bigscreen_four_4x25' => 'four-4x25',
    'bigscreen_two_33_66' => 'two-33-66',
    'bigscreen_two_brick' => 'two-brick',
    'bigscreen_two_50' => 'two-50',
    'bigscreen_two_66_33' => 'two-66-33',
    'bigscreen_five_5x20' => 'five-5x20',
    'bigscreen_six_6x16' => 'six-6x16',
    'bigscreen_three_inset_right' => 'three-inset-right',
    'bigscreen_three_inset_left' => 'three-inset-left',
    'tablet_landscape_three_50_25_25' => 'three-50-25-25',
    'tablet_landscape_three_25_25_50' => 'three-25-25-50',
    'tablet_landscape_three_3x33' => 'three-3x33',
    'tablet_landscape_three_25_50_25' => 'three-25-50-25',
    'tablet_landscape_four_4x25' => 'four-4x25',
    'tablet_landscape_two_33_66' => 'two-33-66',
    'tablet_landscape_two_brick' => 'two-brick',
    'tablet_landscape_two_50' => 'two-50',
    'tablet_landscape_two_66_33' => 'two-66-33',
    'tablet_landscape_five_5x20' => 'five-5x20-2x3-grid',
    'tablet_landscape_six_6x16' => 'six-6x16-3x2-grid',
    'tablet_landscape_three_inset_right' => 'three-inset-right',
    'tablet_landscape_three_inset_left' => 'three-inset-left',
    'tablet_portrait_three_50_25_25' => 'three-50-25-25-stack-top',
    'tablet_portrait_three_25_25_50' => 'three-25-25-50-stack-top',
    'tablet_portrait_three_3x33' => 'three-3x33-stack-top',
    'tablet_portrait_three_25_50_25' => 'three-25-50-25-stack-top',
    'tablet_portrait_four_4x25' => 'four-4x25-2x2-grid',
    'tablet_portrait_two_33_66' => 'two-33-66',
    'tablet_portrait_two_brick' => 'two-brick',
    'tablet_portrait_two_50' => 'two-50',
    'tablet_portrait_two_66_33' => 'two-66-33',
    'tablet_portrait_five_5x20' => 'five-5x20-1x2x2-grid',
    'tablet_portrait_six_6x16' => 'six-6x16-2x3-grid',
    'tablet_portrait_three_inset_right' => 'three-inset-right-wrap',
    'tablet_portrait_three_inset_left' => 'three-inset-left-wrap',
    'smalltouch_landscape_three_50_25_25' => 'three-50-25-25-stack',
    'smalltouch_landscape_three_25_25_50' => 'three-25-25-50-stack',
    'smalltouch_landscape_three_3x33' => 'three-3x33-stack',
    'smalltouch_landscape_three_25_50_25' => 'three-25-50-25-stack',
    'smalltouch_landscape_four_4x25' => 'four-4x25-stack',
    'smalltouch_landscape_two_33_66' => 'two-33-66-stack',
    'smalltouch_landscape_two_brick' => 'two-brick-stack',
    'smalltouch_landscape_two_50' => 'two-50-stack',
    'smalltouch_landscape_two_66_33' => 'two-66-33-stack',
    'smalltouch_landscape_five_5x20' => 'five-5x20-stack',
    'smalltouch_landscape_six_6x16' => 'six-6x16-stack',
    'smalltouch_landscape_three_inset_right' => 'three-inset-right-stack',
    'smalltouch_landscape_three_inset_left' => 'three-inset-left-stack',
    'global_default_layout_toggle' => 0,
    'global_default_layout' => 'smalltouch-portrait',
    'disable_responsive_styles' => 0,
    'global_files_path' => 'public_files',
    'custom_files_path' => '',
    'combine_css_files' => 0,
    'combine_js_files' => 0,
    'enable_custom_media_queries' => 0,
    'load_html5js' => 1,
    'load_matchmediajs' => 0,
    'load_onmediaqueryjs' => 0,
    'load_ltie8css' => 0,
    'load_respondjs' => 0,
    'load_scalefixjs' => 0,
    'adaptivetheme_meta_viewport' => 'width=device-width, initial-scale=1.0, user-scalable=yes',
    'adaptivetheme_meta_mobileoptimized' => 'width',
    'adaptivetheme_meta_handheldfriendly' => '1',
    'adaptivetheme_meta_apple_mobile_web_app_capable' => 'yes',
    'adaptivetheme_meta_ie_document_mode' => '',
    'adaptivetheme_meta_clear_type' => 0,
    'expose_regions' => 0,
    'show_window_size' => 0,
    'atcore_version_test' => 0,
    'load_all_panels' => 0,
    'load_all_panels_no_sidebars' => 0,
    'enable_extensions' => 1,
    'enable_font_settings' => 0,
    'enable_heading_settings' => 0,
    'enable_image_settings' => 0,
    'enable_apple_touch_icons' => 0,
    'enable_exclude_css' => 0,
    'enable_custom_css' => 0,
    'enable_context_regions' => 0,
    'enable_menu_toggle' => 0,
    'enable_float_region_blocks' => 0,
    'enable_markup_overides' => 1,
    'at-settings__active_tab' => 'edit-bigscreen',
    'global_gutter_width' => '36',
    'page_full_width_wrappers' => 0,
    'page_content_type_suggestions' => 0,
    'menu_item_span_elements' => 0,
    'breadcrumb_display' => 0,
    'breadcrumb_home' => 1,
    'breadcrumb_label' => 0,
    'breadcrumb_title' => 0,
    'breadcrumb_separator' => ' &#187; ',
    'login_block_remove_links' => 0,
    'login_block_remove_openid' => 0,
    'horizontal_login_block' => 0,
    'user_block_user_name' => 0,
    'frontpage_remove_title' => 0,
    'unset_block_system_main_front' => 0,
    'feed_icons_hide' => 0,
    'comments_hide_title' => 0,
    'unset_menu_titles' => 0,
    'unset_local_tasks' => 0,
    'adv_search_extra_fieldsets' => 0,
    'skip_link_target' => '#main-content',
    'rel_author' => 0,
    'logo_title' => 0,
    'extra_page_classes' => 1,
    'extra_article_classes' => 1,
    'extra_comment_classes' => 1,
    'extra_block_classes' => 1,
    'extra_menu_classes' => 1,
    'extra_item_list_classes' => 1,
    'attribution_toggle' => 0,
    'at__active_tab' => 'edit-modify-output',
    'at_core' => '7.x-3.x',
    'jquery_update_jquery_version' => '',
    'favicon_mimetype' => 'image/png',
  );
  $export['theme_at_sirius_settings'] = $strongarm;

  return $export;
}
