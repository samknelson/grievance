<?php

/**
 * @file
 * grievance_content_types.services.inc
 */

/**
 * Implements hook_default_services_endpoint().
 */
function grievance_content_types_default_services_endpoint() {
  $export = array();

  $endpoint = new stdClass();
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 3;
  $endpoint->name = 'sirius_service';
  $endpoint->server = 'rest_server';
  $endpoint->path = 'sirius_service';
  $endpoint->authentication = array(
    'sirius_service' => 'sirius_service',
  );
  $endpoint->server_settings = array(
    'formatters' => array(
      'json' => TRUE,
      'bencode' => FALSE,
      'jsonp' => FALSE,
      'php' => FALSE,
      'xml' => FALSE,
    ),
    'parsers' => array(
      'application/json' => TRUE,
      'application/x-www-form-urlencoded' => FALSE,
      'application/xml' => FALSE,
      'multipart/form-data' => FALSE,
      'text/xml' => FALSE,
    ),
  );
  $endpoint->resources = array(
    'sirius' => array(
      'alias' => 'sirius',
      'actions' => array(
        'generic' => array(
          'enabled' => '1',
        ),
      ),
    ),
  );
  $endpoint->debug = 0;
  $export['sirius_service'] = $endpoint;

  return $export;
}
