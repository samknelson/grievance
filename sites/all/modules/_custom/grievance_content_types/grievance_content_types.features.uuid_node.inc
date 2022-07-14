<?php

/**
 * @file
 * grievance_content_types.features.uuid_node.inc
 */

/**
 * Implements hook_uuid_features_default_content().
 */
function grievance_content_types_uuid_features_default_content() {
  $nodes = array();

  $nodes[] = array(
  'title' => 'Ledger - Monthly Tester',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1603491130,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '0145e4ef-1e80-481b-95ce-fed3310064ae',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/config/ledger/test-monthly',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use the form below to test the monthly charge configuration. Computations can be run by entering the node ID of a specific worker, or by executing an entire "batch" of workers.&nbsp;</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2020-10-23 15:12:10 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Trust - Policy - Benefits',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1645116551,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '02c9d7ef-e574-42f2-9556-816d5502326a',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_trust_policy_benefits',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use the tools below to indicate which benefits are offered under a given policy.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2022-02-17 08:49:11 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Twilio - Call - Recording',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1623775460,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '09babc30-3dfb-4b06-8254-89b59d8aba95',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>This page allows the recording for a call to be "pulled" from the telephony provider.</p>

<p>If this call record is associated with a record at the telephony provider, and if the call came in to a recorded line, then the recording may be "pulled". This process will create a separate recording record, and link it to the call record.</p>

<p>Note that recordings stored at the telephony provider may be deleted periodically. Recordings which have been "pulled" and which are stored locally are never deleted.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_twilio_call/recording',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>This page allows the recording for a call to be "pulled" from the telephony provider.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-06-15 09:44:20 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Event - Sections',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1610544682,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '0e25a22e-21af-4c89-b5c6-759bce95d767',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'taxonomy/term/%/sirius_event_type_section',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>A single event may have multiple "sections", which participants are required to sign up for indendently. For example, a lab class might require students to sign up for a specific lab day or time; a multi-day volunteer activity might have volunteers signing up only for specific dates.</p>

<p>Use the form below to indicate whether this event type supports sections which can be individual signed up for, and which particpant roles have access to sign up for or administer sections.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-01-13 05:31:22 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Dispatch - Prefs - EBA',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1657027020,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '0e25b013-c51e-474f-9487-ec8ac39990a0',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_worker_dispatches/eba',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>If you are currently employed but have room in your schedule, use the tool below to indicate which days you would be available for additional work.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2022-07-05 06:17:00 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Twilio - Recordings - Local',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1623420264,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '138724d3-0c8a-4292-9b58-7f84f5090359',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/twilio/local',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Local...</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-06-11 07:04:24 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Event - Phonebank - Followup',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1599918831,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '15035dda-1ae6-4460-b5f8-8901b0e09e75',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_event_participant_definition_followup',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use the form below to add a follow-up text message to a phonebank. The caller can decide whether to send the follow-up message. For example, if a caller is interested in a particular candidate, a follow-up message might include a link to the candidate\'s website.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2020-09-12 06:53:51 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Payments',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => '',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1604084380,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '1beb03fd-a14a-4ab0-888b-144dba3dfd4f',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/ledger/payments',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>You can add, view or edit payments from this page. &nbsp;Please note a payment must be created/added to show up on this page. &nbsp;Once added, a payment must be allocated to individual workers\' accounts, and then posted (status = received, cleared, or cancelled).</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2020-10-30 11:59:40 -0700',
  'user_uuid' => '4840e21c-6ce8-4fb7-84cb-fa178e20b95b',
);
  $nodes[] = array(
  'title' => 'Signatures - Settings',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1613912129,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '1c934ec4-ea6f-4c64-9935-6d1d4561c609',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use this form to specify which types of signatures should be required. For example, a certification might require a signature from both the student and the instructor; an authorization form might require a certification from the instructor, their supervisor, and the fund administrator.</p>

<ul>
	<li><strong>Title&nbsp;</strong>is the title of the signature, i.e. "Instructor", "Supervisor", etc.</li>
	<li><strong>Code </strong>is a brief code used to identify this signature type. It must be all lowercase with no spaces, and is used in letter templates.</li>
	<li><strong>Name</strong> and <strong>organization </strong>are the default name and organization of the signers. These are generally shown only when a blank signature is printed to be physically signed.</li>
	<li><strong>Role </strong>is the user role with permission to sign this entity.</li>
</ul>

<p>&nbsp;</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/esignature/admin',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use this form to define the types of signatures which should be required.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-02-21 04:55:29 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Ledger - Delinquency Letters',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1608413468,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '1d1d2bc5-7c14-470d-b4f0-36ba3d83dbae',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/ledger/delinquency',
        'format' => NULL,
      ),
      1 => array(
        'value' => 'sirius/ledger/delinquency/%',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>This tool will generate a delinquency letter for any active matching entity where the account balance is positive (money is owed).</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2020-12-19 13:31:08 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Machine Name',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1614342465,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '1f270cea-3d60-4df7-858d-98a8428edb74',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_json_definition_machine_name',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use the form below to set the machine name for this entity. The machine name can be used to reference the entity in letter templates.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-02-26 04:27:45 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'BAC1 - Report - IU Hours',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1629752717,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '23390fbd-591a-4406-8633-a995c41a6537',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/report-cache/sirius-bac1-iu-hours/form',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>The classification is a custom field defined <a href="/sirius/contact/definition">here.</a> The rates for each classification are defined <a href="/bac1/iu-classification-rates">here.</a></p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-08-23 14:05:17 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Ledger - A/R - Search',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1627298416,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '299fd3bc-a807-4e5c-8005-decaf6d0535d',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use the form below to search the A/R ledger. The most recent 1,000 entries will be displayed.</p>

<p><strong>Account</strong></p>

<p>Select the account. Leave blank to search all accounts.</p>

<p><strong>Start and End Date</strong></p>

<p>Enter a minimum and maximum date for the search.</p>

<p><strong>Minimum and Maximum Amount</strong></p>

<p>The search can be limited to a minimum and maximum amount.&nbsp; Use a minimum of 0 to search for charges only, and a maximum of 0 to search for payments only.</p>

<p><strong>Memo</strong></p>

<p>Enter a string to search the "memo" field of the entry. Substrings are permitted.</p>

<p><strong>Searching by Worker, Contact, or Employer</strong></p>

<p>For searches specific to a worker, contact, or employer, visit the A/R tab for that entity.</p>

<p><strong>Display</strong></p>

<p>Results may be displayed on the screen or exported to a spreadsheet in CSV format.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/ledger/ar/search',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use the form below to search the A/R ledger.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-07-26 04:20:16 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Custom Contact Fields - Display',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1604925205,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '33f0199a-d1f0-4716-9b57-a72d1b9a79d4',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/contact/definition/display',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use this page to indicate which custom contact fields should be shown in various locations -- for example, on the phone bank or on a dispatch.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2020-11-09 04:33:25 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Mailattach',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1612788133,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '36cd607a-6331-4a08-8978-81a1b1d79cc2',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>The Sirius "mailattach" system allows users to add attachments by sending emails to a specific address. The body of the email, as well as any attachments, are added to the requested record.</p>

<p><strong>Usage</strong></p>

<p>Your site administrator will configure an email mailbox to use fo the mailattach system. Contact your administrator to find out what address has been configured. To use the system, simply send an email to this address. Your email will be attached to the record that you specify, and you will receive an email response indicating that your attachment has been accepted.</p>

<p><strong>Sender</strong></p>

<p>The email address used to send the message must be associated with a Sirius user with appropriate permissions. If you send the message from an email address that is not associated with a Sirius user, or is associated with a user without adequate permissions, your email will not be accepted, and you will receive a response indicating the reason.</p>

<p><strong>Subject</strong></p>

<p>The subject of the email must be formatted specifically, so that mailattach knows which record to attach the message to. Here are some examples:</p>

<ul>
	<li>Subject: <strong>Worker 12345</strong><br />
	(Attach the message to the record for Worker with ID 12345)<br />
	&nbsp;</li>
	<li>Subject: <strong>Worker 12345: Personal documents</strong><br />
	(Attach the message to the record for Worker with ID 12345, and add the additional subject "Personal documents" to the log entry.)<br />
	&nbsp;</li>
	<li>Subject: <strong>Grievance 20200229: Company information request response</strong><br />
	(Attach the mesage to grievance with ID 20200229, with the additional subject specified)<br />
	&nbsp;</li>
	<li>Subject:&nbsp;<strong>20200229</strong><br />
	Subject: <strong>20200229: Grievant\'s initial documents</strong><br />
	(The site administrator will specify a "default" record type. In this case, if mailattach just sees an ID number, it assumes it\'s a grievance.)<br />
	&nbsp;</li>
	<li>Subject:&nbsp;<strong>Worker 2125551212: Disciplinary report</strong><br />
	Subject:&nbsp;<strong>Worker 001-22-1234: Disciplinary report</strong><br />
	Subject:&nbsp;<strong>Worker sam@siriussoftware.com: Disciplinary report</strong><br />
	(You can attach documents to a worker record by phone, email, or social security number.&nbsp;<em>Important: we do not recommend sending emails with social security numbers.</em>)</li>
</ul>

<p>&nbsp;</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/mailattach',
        'format' => NULL,
      ),
      1 => array(
        'value' => 'sirius/mailattach/test',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use these pages to manage the Sirius "mailattach" system.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-02-08 04:42:13 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Feeds - Configuration',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1600083154,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '37e61062-61a6-4225-83e7-439c15d9d49f',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/config/feed',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Set the batch size for each feed importer. Batch size should be set so as to be as large as possible while remaining&nbsp;comfortably within the server\'s limits (memory and processing time). Limits are set for the entire site, and cannot be set per-domain.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2020-09-14 04:32:34 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Dispatch - Prefs - ASI',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1657026640,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '3923d9b5-6283-4f47-9842-291b1d5bb712',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_worker_dispatches/asi',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use this form to indicate whether you would like to be automatically signed in to the out-of-work list whenever a dispatch ends.</p>

<p>By selecting "Yes, When a dispatch ends I should be automatically signed in,"&nbsp;I acknowledge that:&nbsp;</p>

<ol>
	<li>When an employer lays off multiple employees simultaneously, laid-off employees’ names will be added to the out-of-work list in alphabetical order.</li>
	<li>Upon being successfully signed in to the out-of-work list, I will receive a notification based upon the preferred manner of notification that I selected.&nbsp;</li>
	<li>I am responsible for completing my scheduled shift regardless what time of day I receive the notification that my name was added to the out-of-work list. &nbsp;Failure to do so may result in disciplinary action by the employer.</li>
	<li>The Union will not be liable if my employer does not sign me in to the out-of-work list in a timely fashion or at all, or if the system errs and my name is not automatically added to the out-of-work list. &nbsp;It is my responsibility to verify that I have been successfully added to the out-of-work list.&nbsp;</li>
</ol>

<p>By selecting "No, when a dispatch ends I will sign in to the out of work list manually" I acknowledge that:</p>

<ol>
	<li>My name will only be added to the out-of-work list when I sign myself in either (A) through the web interface, (B) through the telephone sign-in system; or (C) in person at the Union Hall.</li>
	<li>Upon being successfully signed in to the out-of-work list, I will receive a notification based upon the preferred manner of notification that I selected.</li>
	<li>The Union will not be liable if the system errs and my name is not added to the out-of-work list. &nbsp;It is my responsibility to verify that I have been successfully added to the out-of-work list.&nbsp;</li>
</ol>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2022-07-05 06:10:40 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Dispatch - Auto Sign-In - Disclaimer',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1555591759,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '39610c00-57d9-4816-85fc-3e62a0aed3b9',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'field|sirius_worker_dispatch_preferences|asi',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>By selecting "Yes, When a dispatch ends I should be automatically signed in,"&nbsp;I acknowledge that:&nbsp;</p>

<ol>
	<li>When an employer lays off multiple employees simultaneously, laid-off employees’ names will be added to the out-of-work list in alphabetical order.</li>
	<li>Upon being successfully signed in to the out-of-work list, I will receive a notification based upon the preferred manner of notification that I selected.&nbsp;</li>
	<li>I am responsible for completing my scheduled shift regardless what time of day I receive the notification that my name was added to the out-of-work list. &nbsp;Failure to do so may result in disciplinary action by the employer.</li>
	<li>The Union will not be liable if my employer does not sign me in to the out-of-work list in a timely fashion or at all, or if the system errs and my name is not automatically added to the out-of-work list. &nbsp;It is my responsibility to verify that I have been successfully added to the out-of-work list.&nbsp;</li>
</ol>

<p>By selecting "No, when a dispatch ends I will sign in to the out of work list manually" I acknowledge that:</p>

<ol>
	<li>My name will only be added to the out-of-work list when I sign myself in either (A) through the web interface, (B) through the telephone sign-in system; or (C) in person at the Union Hall.</li>
	<li>Upon being successfully signed in to the out-of-work list, I will receive a notification based upon the preferred manner of notification that I selected.</li>
	<li>The Union will not be liable if the system errs and my name is not added to the out-of-work list. &nbsp;It is my responsibility to verify that I have been successfully added to the out-of-work list.&nbsp;</li>
</ol>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2019-04-18 05:49:19 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Duplicate Member Reports',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1575911886,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '3d910445-cfb4-4512-9417-a1eea339b543',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>&nbsp;</p>

<ul>
</ul>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/report-cache/sirius-worker-duplicate-id/form',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Choose an ID type to check for duplicates on:</p>

<ul>
	<li>ID = Dispatch/Badge ID</li>
	<li>ID2 = Titan/External ID</li>
	<li>ID3 = Apprentice/Training Center ID</li>
	<li>SSN = Social Security Number [Primary Unique ID]</li>
</ul>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2019-12-09 09:18:06 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Disclaimer',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1610195690,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '3dc4f868-0e7e-40f6-aab4-3166be7135a8',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>"Disclaimers" are legal statements which must be accepted by all event participants. Examples might include</p>

<ul>
	<li>an agreement to engage in acceptable behavior and treat others with respect</li>
	<li>a waiver of liability</li>
	<li>an agreement to assume reasonable charges in the event of negligence</li>
	<li>a specific and timely agreement, such as to wear a mask and practice social distancing during a pandemic</li>
</ul>

<p>For users with appropriate privileges, the available disclaimers can be managed <a href="/sirius/disclaimer/list">here.</a></p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'taxonomy/term/%/sirius_event_type_disclaimer',
        'format' => NULL,
      ),
      1 => array(
        'value' => 'node/%/sirius_event_disclaimer/settings',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use these tools to manage "Disclaimers" for an event or event type.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-01-09 04:34:50 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Bulk Message - Body - SMS',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1622469964,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '40f9e76f-3b69-4a80-8785-d3e97715fdeb',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>The cost of an SMS message is dependent on the number of "segments" required. A segment count is displayed below the "SMS Body" field. Generally speaking, the best practice is to try to keep the message to just one segment.&nbsp;</p>

<p><strong>Why does it start with 28 characters?</strong></p>

<p>Often, Sirius is configured to automatically append an "unsubscribe" message to each outgoing SMS. If this is the case, the length of that message is included in the segment count; this explains why even an empty message might have a nonzero number of characters.</p>

<p>(Including an unsubscribe message helps improve deliverability, and protects the reputation of the sending phone number.)</p>

<p><strong>Link Shorteners</strong></p>

<p>In the past, it was common practice to send SMS messages with links which had been "shortened" with a service such as TinyUrl or Bitly. However, more recently, <strong>carrier policies forbid the use of shared public URL shorteners</strong>. For more information, please see <a href="https://support.twilio.com/hc/en-us/articles/1260804572090-How-can-I-send-shortened-URLs-links-in-my-messages-" target="_blank">the relevant Help Center page at Twilio</a>.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_bulk_body_sms',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Enter the text of the SMS message.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-05-31 07:06:04 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Custom Field Sets - Event Type',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1613343525,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '4724429e-b5d1-4299-9324-b397bd7f423b',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use these pages to assign custom fields sets and values to event types. For example, for classes, an organization might want to keep tracker of the grant name, the location, the required materials, and so forth.</p>

<p>When a new event is created, it will inherit the custom field settings from the event type.&nbsp;</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'taxonomy/term/%/sirius_event_type_customfield/settings',
        'format' => NULL,
      ),
      1 => array(
        'value' => 'taxonomy/term/%/sirius_event_type_customfield/edit',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Custom field sets <a href="/sirius/customfield/definition/list">can be configured here.</a></p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-02-14 14:58:45 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Twilio - Call',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1623775329,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '48d73be1-cac4-4f08-8183-bf19ff5524ac',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>This page shows information about a call which was made into the Sirius system.</p>

<p><strong>SID</strong></p>

<p>The "SID" is the identifier of the call used by&nbsp;the telephony provider.</p>

<p><strong>From, To</strong></p>

<p>"From" is the phone number that originated the call, and "To" is the receiving phone number. In the case of incoming calls, the "To" number is typically the public-facing phone number, sometimes associated with a particular operator. The call may also have been forwarded to an office number, but the forwarded number is never the "To" nubmer.</p>

<p><strong>Date</strong></p>

<p>The date and time when the call started</p>

<p><strong>Log</strong></p>

<p>The log shows the interactions that the caller had with the phone tree.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_twilio_call',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>This page shows information about a call which was made into the Sirius system.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-06-15 09:42:09 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Event - Intake - Application - Manage',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1619616198,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '4bcae4d6-0f75-4152-80e2-9a2b405a725b',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Accepting an application performs the following actions.</p>

<ul>
	<li>A new Worker record is created, using the name, phone number, and email address provided
	<ul>
		<li>The worker is given the specified work status, if any</li>
		<li>The worker is given the specified member sttaus, if any</li>
		<li>The worker is given the specified apprentice level, if any</li>
		<li>The intake form is attached to the worker record as a log entry</li>
	</ul>
	</li>
	<li>A new Participant record is created, associated the new Worker with the current Event.</li>
	<li>Any attachments are added as Materials associated with the Worker</li>
	<li>A User may be created, allowing the Worker to sign in to the system.</li>
	<li>A text message and email notification can be sent to the newly-created Worker.</li>
</ul>

<p>&nbsp;</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_event_intake_application_manage',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use the form below to accept, reject, or re-open an application.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-04-28 06:23:18 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Bulk Message - Body - Email',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1622469874,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '51ebac7c-d4cf-4cb2-8126-8249370986bb',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Email messages may be entered in HTML format. However, be careful to test your message carefully, since not all email clients can render all HTML tags.&nbsp;</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_bulk_body_email',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Enter the subject and the body of the bulk email.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-05-31 07:04:34 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Feeds',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => '',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1604084683,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '5208faa4-3bac-4616-9bc9-d78ab8061c0c',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/feeds',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<ul>
	<li>Load demographics -- upload new people</li>
	<li>Load Member status&nbsp;</li>
	<li>Load the worker work status</li>
	<li>Load payments / allocations</li>
</ul>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2020-10-30 12:04:43 -0700',
  'user_uuid' => '4840e21c-6ce8-4fb7-84cb-fa178e20b95b',
);
  $nodes[] = array(
  'title' => 'Phone Number - Administrative Block',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1640947935,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '527b6e8b-a7da-4413-a154-25bf66bde833',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>An administrative block may be applied to a phone number, for example, if the owner of the number contacts the site administrator and requests to stop receiving messages. Administrative blocks can be used to prevent outgiong text messages (SMS) or voice messages. Administrative blocks have no effect on incoming messages.</p>

<p>Note that administrative blocks are <em>in addition </em>to other forms of blocks. For example, if a user opts out of messaging by sending a "STOP" text message, a user block is created. These blocks can only be cleared by the user (for example, by sending a "START" text message.) Site administrators cannot set or clear user blocks.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_phonenumber_manual_block',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use this form to set an administrative block for a phone number.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-12-31 02:52:15 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Do Not Disturb',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1603881856,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '54161002-1de7-4736-8546-f79153ddf6a7',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/admin/do_not_disturb',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use the form below to create "Do Not Disturb" schedules. For each day, indicate:</p>

<ul>
	<li>Whether or not messaging is permitted on that day</li>
	<li>The time that messging can start, and</li>
	<li>The time that messaging must end</li>
</ul>

<p>Remember to use military time, i.e. 7:00 PM should be entered as "19:00". Multiple schedules are permitted, for example, if text messages and voice calling should have different schedules, or if there should be different schedules for urgent and non-urgent communications.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2020-10-28 03:44:16 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Sirius Employer Job List',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1542646989,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '57bee834-6878-48c8-a79f-373911579bc3',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/employer/dispatch/job/list',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Welcome to the Sirius Dispatch System.&nbsp; Your list of job requests is below and can be filtered by call number, type, status&nbsp;and facility.</p>

<p>Create a new job using the link in the menu above&nbsp;or&nbsp;by clicking&nbsp;<a href="/sirius/employer/dispatch/job/create">here</a>.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2018-11-19 09:03:09 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Twilio - Recordings - Remote',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1623420249,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '590d277a-3a4b-4883-9b43-cc6d03ec763d',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>This page lists remote recordings stored at the telephony provider. Some of these recordings may have been "pulled" to the local repository.</p>

<p>The "Filters" tool to may be used to search for a specific recording. At the bottom of the page, there are options to delete selected recordings from the remote telephony provider, or to pull selected recordings and create local copies.</p>

<p><strong>Columns</strong></p>

<ul>
	<li>From, To: The phone numbers associated with the call that this recording is connected to.&nbsp;</li>
	<li>Date: The date that the recording started</li>
	<li>Duration: The length of the recording. This may not be the same as the duration of the call.</li>
	<li>Play: Play or download the recording directly from the remote telephony provider.</li>
	<li>Local?: Indicates whether this recording has been "pulled" so that a local copy exists.</li>
</ul>

<p>&nbsp;</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/twilio/recordings',
        'format' => NULL,
      ),
      1 => array(
        'value' => 'sirius/twilio/recordings/remote',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>This page lists remote recordings stored at the telephony provider.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-06-11 07:04:09 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Job Attachments',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1607109170,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '5ba130a8-dc9a-416c-a4bc-10b946417983',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_dispatch_job_attachments',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>This page is for the employer or admins to upload documentation for workers.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2020-12-04 11:12:50 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Office - Operators',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1623776385,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '5bb36063-9680-4b3e-85af-11af320d177c',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>"Operators" are destinations for incoming calls. A user calling the main number may be routed to any selected operator.</p>

<p><strong>Name</strong></p>

<p>The name of the operator. Operators may be offices, individuals, or functions. For example, some common operators might include "Finance Department", "Dispatch Office", or "Training Center".</p>

<p><strong>Status</strong></p>

<p>Indicates whether the operator is "open". Open operators can receive incoming calls. Closed offices respond to incoming calls with a simple message, and then disconnect.</p>

<p><strong>Incoming (Public) Phone</strong></p>

<p>The phone number that is used for incoming calls. Many operators might share the same incoming number, in which case the caller is requested to select which operator they want.</p>

<p><strong>Outgoing (Private) Phone</strong></p>

<p>When an incoming call is forwarded to an operator, it is done by dialing the private, or "outgoing", phone number. Many operators might share the same private number.</p>

<p><strong>Tools</strong></p>

<p>Use the tools to manage the the settings for the operator, the status (open or closed), or to see the list of incoming calls.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/office/operator/list',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use this page to manage the list of operators.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-06-15 09:59:45 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Service - Server',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1649772149,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '5cc97242-3cc1-42e6-bf17-74f216cc4efd',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Configure this domain as a server. It will allow incoming connections from clients.</p>

<p><strong>Server Active:</strong>&nbsp;Allow incoming connections?</p>

<p><strong>Server IP Allowlist:&nbsp;</strong>Incoming connections are permitted from these IP addresses. Generally this should contain just the IP address of the remote client instance.</p>

<p><strong>Account ID:</strong>&nbsp;The account ID to use when accessing this server.</p>

<p><strong>Access Token:&nbsp;</strong>The access token to use when accessing this server.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/service/config/server',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Configure this domain as a server.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2022-04-12 07:02:29 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Dispatch - Prefs - Notifications',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1657026525,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '5eec031f-f807-4e52-b152-694458e954c9',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p><strong>Primary Dispatch Medium</strong></p>

<p>This is your primary notification medium for new dispatches.</p>

<p><strong>Secondary Dispatch Medium</strong></p>

<p>This is your secondary notification medium. New dispatches will be sent to&nbsp;<strong>both</strong>&nbsp;your primary and secondary media.</p>

<p><strong>Other Notifications Medium</strong></p>

<p>All other notifications (skill expiration, layoff notifications, etc.) will be sent by your "other" notifications medium.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_worker_dispatches/notifications',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Indicate how you would like to receive notifications about new dispatches - by text message, email, or automated voice call (if available).</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2022-07-05 06:08:45 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Dispatch - SMS',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1541503962,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '6033352e-6425-43ca-8500-9278cec13552',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_dispatch_sms',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>The form below shows the SMS conversation regarding this dispatch.&nbsp; You can check for updates here or send a new SMS.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'path' => array(
    'alias' => 'sirius/node/sirius-help/8958',
  ),
  'date' => '2018-11-06 03:32:42 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Dispatch - Config - VLRI',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1622660941,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '607717d3-ec77-4f9f-8ce7-e7df61a8ea00',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>A VLRI, or "Voluntary Layoff with a request to convert into an Involuntary Layoff", is a special dispatch status.</p>

<p>A worker on a dispatch may select a VLRI. They are immediately laid off, making them available to be dispatched to another job. However, their layoff is considered "voluntary" until it is resolved by the employer, who must either accept or decline the request. Accepting the request converts the dispatch into an involuntary layoff.</p>

<ul>
	<li><strong>Enable: </strong>Set to "Yes" to enable this dispatch status.<br />
	&nbsp;</li>
	<li><strong>Window: </strong>Employers are expected to resolve VLRI\'s within this window. The window is in seconds, so it\'s a good idea to enter a fairly large number, i.e. 43,200 seconds = 60*60*12, or 12 hours.<br />
	&nbsp;</li>
	<li><strong>Limit:</strong> Emloyers who fail to resolve VLRI\'s in a timely manner will be blocked from the dispatch system, and unable to create new jobs. Use this field to enter the number of unresolved VLRI\'s (older than the window) before the Employer\'s access to the dispatch system is blcoked.<br />
	&nbsp;</li>
	<li><strong>Notify Worker:</strong>&nbsp;By default, workers are notified when their VLRI request is approved or rejected. This flag allows the notification to be suppressed.</li>
</ul>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/dispatch/config/vlri',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use this page to configure the "Voluntary Layoff, Request that to convert to Involuntary Layoff".</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-06-02 12:09:01 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Event - Export / Import',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1608915814,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '656d6073-ca1f-43a2-85fe-a0d0e994e4c0',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_event_clone/exim',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use this tool to export or import the settings for an event. Settings do not include participants, title, or schedule information.</p>

<p>Remember that settings should only be imported into an event of the same type; in other words, don\'t import the settings for an event of type "class" into an event of type "phonebank".</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2020-12-25 09:03:34 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Dispatch - Create a Job',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1604083463,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '69c2f019-d2c8-4ba4-a5bf-054d932d104d',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/dispatch/job/create',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Select the Employer, Request Type (call by name - individual people, or Open call - call by seniority and availability date), and Industry (this will trigger a choice of&nbsp;Member Statuses to select to fill the job).</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2020-10-30 11:44:23 -0700',
  'user_uuid' => '4840e21c-6ce8-4fb7-84cb-fa178e20b95b',
);
  $nodes[] = array(
  'title' => 'Phone - Answering Machine Detection',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1603450987,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '6d4be677-bef5-4d10-944c-f489a957da48',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/config/phone/twilio/amd',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use the form below to set tuning values for the Answering Machine Detection (AMD) parameters. For an explanation of each parameter, see&nbsp;</p>

<p><a href="https://www.twilio.com/docs/voice/answering-machine-detection" target="_blank">https://www.twilio.com/docs/voice/answering-machine-detection</a>.</p>

<p>These settings are domain-specific.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2020-10-23 04:03:07 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Report - Auto-Genrate',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1644756539,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '70ccc350-6493-4caa-aad8-842c2fa55636',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Reports with auto-generation enabled are run once a day. The most recent 30 auto-generated reports are retained.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/reports/%/auto',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use this form to enable auto-generation for this report.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2022-02-13 04:48:59 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Fastload',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1621426936,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '76325793-5702-4387-a3ad-b3e1e7ab596e',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>In Drupal, loading large numbers of records ("nodes") can be a slow process. The Sirius FastLoad tool is designed to speed this process up. It operates by pre-generating and saving large numbers of nodes. These "stub" nodes can then be quickly reserved and updated.</p>

<p><strong>Content Types</strong></p>

<p>The FastLoad system pre-generates nodes of type "Contact" and "Log". For Contact nodes, the available fields include Name, Phone, Email, and JSON.</p>

<p><strong>Domains</strong></p>

<p>FastLoad is a server-wide tool. It is configured once, and the pool of available FastLoad nodes is shared across all domains.&nbsp;</p>

<p><strong>Configuration</strong></p>

<p>On the configuration page, FastLoad can be enabled or disabled for the server. It is also necessary to set two important variables:&nbsp;</p>

<p>- The&nbsp;"Buffer Size". This is the number of available FastLoad nodes which the system attempts to maintain. The buffer size should generally be larger than the largest likely upload; in other words, if a typical upload might inlude 100,000 contacts, a typical buffer size might be 250,000 or 500,000.&nbsp;</p>

<p>- The "Batch Size". This is the number of FastLoad nodes which are created in each "batch" run. Batches typically run once an hour, so a good estimate might be to set the batch size at 10% of the buffer size.</p>

<p><strong>Status</strong></p>

<p>The status page shows, for each content type, the current number of "available" FastLoad nodes, as well as the number which have been used historically. If the generation process is complete, then the number of available nodes should equal the buffer size. If many FastLoad nodes have recently been used, the buffer may be depleted, but it should fill back up over time.</p>

<p><strong>Generate</strong></p>

<p>FastLoad node generation happens automatically in the background. It is also possible to run a generation batch manually, from the "generate" page.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/fastload',
        'format' => NULL,
      ),
      1 => array(
        'value' => 'sirius/fastload/config',
        'format' => NULL,
      ),
      2 => array(
        'value' => 'sirius/fastload/generate',
        'format' => NULL,
      ),
      3 => array(
        'value' => 'sirius/fastload/status',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use these pages to manage the Sirius FastLoad system.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-05-19 05:22:16 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Create a payment',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => '',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1604084052,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '77ba9aad-a696-44e2-abe7-9e7fa2d2989b',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/ledger/payment/create',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Please be sure to select whether the payment is from an individual worker or from an employer, Select which account the payment is to,&nbsp;Select the payment type and enter the amount (payments to the account should be entered as a negative number - payments are negative numbers,&nbsp;balances owed are positive numbers.).&nbsp;Once a payment is entered, then you must allocate the payment to a specific worker\'s account, and then post the payment.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2020-10-30 11:54:12 -0700',
  'user_uuid' => '4840e21c-6ce8-4fb7-84cb-fa178e20b95b',
);
  $nodes[] = array(
  'title' => 'Service - Client - Test',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1650719038,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '7800e1f0-f1c7-4ede-a434-0138d4289f1f',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>This form can be used to verify that connection settings are correct when configuring this domain as a "client". There are two test options.</p>

<ul>
	<li>The&nbsp;<strong>client connection test</strong>&nbsp;will attempt to run a "ping" function on the remote server. It will fail if the client is not active or if the remote server fails to respond appropriately.</li>
	<li>The <strong>proxy connection test</strong>&nbsp;will attempt to run the same "ping" function using a proxy call. If the domain is not configured as a proxy, then it will run the function locally. This means that a successful test doesn\'t necessarily indicate that a proxy is active; the response must be examined to determine whether the call was made locally or remotely.</li>
</ul>

<p>&nbsp;</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/service/config/client/test',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use the form below to test service connectivity settings.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2022-04-23 06:03:58 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Employer User List',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1543598885,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '78073ba7-016f-4e23-b6e4-da4ebe7961bb',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/employer_users',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>This is the list of all users associated with your shop.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2018-11-30 09:28:05 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Phone Number - Scan',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1640948582,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '7ad725b4-9de3-4b2b-a0bf-d4b74a6b2ca4',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>"Scanning" a phone number fetches carrier information from the service provider. There is a small fee associated with each scan.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_phonenumber_scan',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use the form below to scan a phone number.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-12-31 03:03:02 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'IP Restrictions',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1650112292,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '7b2fde97-c290-4854-bf08-fd9cf71e6f87',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use the tool below to configure IP restrictions for this server.</p>

<p>IP restrictions are applied globally, across the entire Drupal instance. They are not specific to a domain.</p>

<p>Please use this tool with caution, as it is possible to lock yourself out of the system.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/iprestrict',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use this tool to configure IP restrictions for this server.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2022-04-16 05:31:32 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Mapping',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1607252268,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '7db5a52f-7c87-4c83-817f-48c888416958',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>There are a number of steps required to configure geocoding and mapping for Sirius.&nbsp;</p>

<ol>
	<li>Create a Google Cloud project, at&nbsp;<a href="https://console.cloud.google.com/" target="_blank">https://console.cloud.google.com/</a>

	<ul>
		<li>Enable billing</li>
		<li>Enable at least the APIs "Geocoding API" and "Maps JavaScript API"</li>
		<li>Create at least one API key.</li>
	</ul>
	<a href="https://console.cloud.google.com/" target="_blank"> ​</a></li>
	<li>Within Sirius, go to the <a href="/admin/config/content/geocoder">Geocoder&nbsp;configuration page</a> and enter the API.</li>
	<li>Within Sirius, go to the <a href="/admin/config/content/geofield_map">Mapping configuration page</a> and enter the API.</li>
	<li>Go to the "components" page of any domain that should have mapping enabled, and enable the mapping component.</li>
	<li>Within that domain, create a new contact or edit an existing one. Ensure that when viewing the contact, a map of their address appears.</li>
</ol>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'admin/config/content/geofield_map',
        'format' => NULL,
      ),
      1 => array(
        'value' => 'admin/config/content/geocoder',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use these pages to configure the mapping interface for Sirius.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2020-12-06 02:57:48 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Dispatch - Out of Work List - Journeyman',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1542641030,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '80b41fe9-f0c7-4a9c-b22d-5a4b71ec61e5',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/employer/worker/list/271',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>This is the list of currently unemployed Journeyman workers.&nbsp; If there are not enough Journeyman workers available to fill your job request, please contact Dispatch to request access to the extraboard lists.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2018-11-19 07:23:50 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Flood',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1653679948,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '86359528-b19e-401b-925d-d49fab0e8f0e',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>"Floods" are used to detect and block attempts to access services too frequently. For example, to prevent users from attempting to guess passwords, we may limit the number of login attempts from a given IP address.</p>

<p>Each type of flood can have a&nbsp;<strong>threshold</strong>, which is the number of events before the service is blocked, and a&nbsp;<strong>window</strong>, which is a number of seconds before access to the service is blocked. For example, we might limit login attempts to a threshold of 50 attempts within a window of 60*60 = 3600 seconds, or one hour.</p>

<p><strong>Unblock</strong></p>

<p>The "Unblock" page can be used to manually clear a block. For example, if a user is prevented from logging in, an administrator can clear their block and allow them to try again. (Note that blocks clear automatically after their window has expired, so in general it should not be necessary to manually clear blocks.)</p>

<p><strong>Settings</strong></p>

<p>Use the settings page to set the threshold and window for each type of block.</p>

<p><strong>Test</strong></p>

<p>The test page can be used to manually create blocks, as a way to test the settings or ensure that the system is working as expected.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/flood',
        'format' => NULL,
      ),
      1 => array(
        'value' => 'sirius/flood/unblock',
        'format' => NULL,
      ),
      2 => array(
        'value' => 'sirius/flood/settings',
        'format' => NULL,
      ),
      3 => array(
        'value' => 'sirius/flood/test',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>View and unblock flood events.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2022-05-27 12:32:28 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Dispatch - Prefs - CBN',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1657026961,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '866f7845-3949-4806-aa28-c50acbccfb4d',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_worker_dispatches/cbn',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use the tool below to indicate whether you would like to receive open calls only, or only call-by-name jobs requesting that specifically request you.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2022-07-05 06:16:01 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Training Center - Evaluations',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1626264762,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '8cd25af6-0157-4965-a80a-b5586df50372',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>This page shows the most recently submitted 100 evaluations of each status: Draft, Accepted, and Rejected.</p>

<p>Each evaluation is displayed. Draft evaluations may be individually marked as "accepted" or "rejected".</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/training/evaluations/queue',
        'format' => NULL,
      ),
      1 => array(
        'value' => 'sirius/training/evaluations/queue/draft',
        'format' => NULL,
      ),
      2 => array(
        'value' => 'sirius/training/evaluations/queue/accepted',
        'format' => NULL,
      ),
      3 => array(
        'value' => 'sirius/training/evaluations/queue/rejected',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>The most recent 100 evaluations are shown below.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-07-14 05:12:42 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Phone - Network Test',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1602160552,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '901c4891-59ae-470b-ac0f-5943a902dbb3',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/twilio/network-test',
        'format' => NULL,
      ),
      1 => array(
        'value' => 'node/%/sirius_event_phonebank/network_test',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>This tool&nbsp;uses a network test suite provided by Twilio to check the capability of your computer to perform a Voice over Internet (VoIP) call.</p>

<p>Use the button below to load the test tool on this page. The test will take about a minute. You may be asked to allow the page to use your microphone and / or camera.&nbsp;</p>

<p>(Alternatively, you can use the tool on the Twilio site at&nbsp;<a href="https://networktest.twilio.com">https://networktest.twilio.com</a>.)</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2020-10-08 05:35:52 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Training Center - Levels - Denormalize / Resync',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1622808216,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '93f7b12f-5566-4b04-9b9a-fefbf2d73876',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>The level settings for each worker are stored in structured data. (Technically, they\'re stored in a "JSON" field attached to the worker record.) When the worker\'s levels need to be rendered or modified, this structured data is decoded.</p>

<p>Unfortunately, many standard reports and queries are unable to retrieve this type of structured data. In particular, Drupal Views cannot fetch these values directly. This makes it challenging to add the worker\'s levels to a standard search form.</p>

<p>The solution is to "denormalize" the level -- storing a copy of it in a standard, easy-to-access Drupal field.</p>

<p>This process of denormalization happens automatically whenever a worker\'s level is set. However, occasionally the internal representation and the denormalized copy might get out of sync; for example, if the worker is updated directly within the database, or if there is a software change. In that case, the denormalized copies of the data need to be recomputed. This utility is designed to do that.</p>

<p>The system begins with the most recent worker (highest NID) and re-denormalizes a "batch". Batch sizes should be selected so that the system doesn\'t run out of memory and so that the process finishes in a reasonable amount of time. It will probably be necessary to run the utility several times until all workers are recomputed.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/config/training/level-denorm-resync',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use this form to re-denormalize levels for all workers.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-06-04 05:03:36 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Bulk Message - Throttle',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1627042317,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '94947d9f-af1b-4d3c-9c22-e6b1258e77ec',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p><strong>Batch Size</strong></p>

<p>A batch size can be specified for this bulk message. If this batch size is greater than the system-wide batch size for the medium, it is ignored.</p>

<p>Batches are run:</p>

<ul>
	<li>No more often than the server\'s "cron" is set to run , AND&nbsp;</li>
	<li>No more often than the the system-wide interval between batches for this medium.</li>
</ul>

<p>Administrative users can run batches manually and examine the results using the <a href="/sirius/bulk/config/poll">bulk message poll</a> tool.</p>

<p><strong>Line Throttle</strong></p>

<p>For voice calls, the "line throttle" limits the number of phone calls that can be active at any given time. If a batch is being processed and the line throttle is exceeded, then the batch is terminated and no more calls are initiated. A throttle can be restricted by the "from" number (i.e. to only throttle calls made from a particular Caller ID) or by "to" number (i.e. to throttle calls forwarded to a particular conference number.</p>

<p>It\'s important to keep batch sizes and delays in mind when setting the line throttle. For example, suppose that the goal is to limit the number of calls to a call center to 10 at a time. If the batch size is 100, then the system will initiate up to 100 calls within a few seconds; the first callers won\'t have had a chance to connect to the call center by the time that the last calls are initiated, so there may be 100 calls active at any given moment. Therefore, it\'s important to make sure that the batch size is not significantly greater than the line throttle.</p>

<p><em>Important: Use of a line throttle is computationally expensive, because it requires a query to the telephony provider before EACH CALL is initiated. </em>Using the line throttle will dramatically reduce the maximum number of calls/hour that can be delivered. The line throttle should only be used when it\'s necessary to reduce the call rate far below what the system is capable of.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_bulk_send/throttle',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use the form below to "throttle" this message, or limit the delivery rate. Settings here are in addition to the <a href="/sirius/bulk/config">system-wide settings</a>.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-07-23 05:11:57 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Config - Drop-Down List Management',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1638200241,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '962380f5-1892-4c99-ba50-d40cf70e5d75',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>The tools below can be used to manage drop-down lists on the site. Many of the lists are domain-specific; others are global across the entire system.</p>

<p>For each list, we show:</p>

<ul>
	<li>The name of the list (e.g. "Grievance Status" or "Sirius Event Type")</li>
	<li>A brief description of the list</li>
	<li>Tools to edit the list, or re-arrange the order of the elements on the list</li>
</ul>

<p>&nbsp;</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/taxonomy',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use the tools below to manage drop-down lists.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-11-29 07:37:21 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Grievance Arbitration Form Settings',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1570477757,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '96be6bc7-324e-4dfd-93ab-c8bc4870d3b6',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'admin/config/grievance/arbitrationform',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>This page is for configuration of the "Grievance Form" for the site.&nbsp; This is now deprecated, and most sites should be using the "Letters" system instead.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2019-10-07 12:49:17 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Event - Clone',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1608916733,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '978f7de2-ac03-4b87-9bb5-554ab1ecd799',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_event_clone',
        'format' => NULL,
      ),
      1 => array(
        'value' => 'node/%/sirius_event_clone/clone',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use this tool to create a new event as a copy of this event.&nbsp;</p>

<p>The new event will have the same type, schedule, and settings as the previous event. Participants and materials are NOT copied to the new event.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2020-12-25 09:18:53 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Member Statuses',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1551990182,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '98a61713-67db-435c-ac34-4cfe38c15ff9',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>This page is for managing Member Satuses, which control which lists workers appear on, and which roles can dispatch those workers.</p>

<ul>
	<li><strong>User Roles</strong>&nbsp;are the roles in the system that automatically get applied to a worker\'s user based on which member status they have.&nbsp; (i.e. workers with the "Journeyman" Member Status should be given the role "worker journeyman").</li>
	<li><strong>Dispatch Roles</strong>&nbsp;indicate who can <a href="/sirius/dispatch/job/create" target="_blank">create Jobs</a>&nbsp;that will include workers of the corresponding status.&nbsp; This means that to "Open up the Skilled Extraboard list to employers", you would check off the "employer" role under the "Dispatch Roles" for the Skilled Extra-Board Member Status.</li>
	<li><strong>Job Types</strong>&nbsp;indicates which job types a worker on the associated list can be dispatched to.</li>
</ul>

<p>You can set the skilled and unskilled rates for each member status.&nbsp; The skilled rate will be used if a Job specifies a skill requirement, otherwise the unskilled rate will be used.&nbsp; (They rates can be the same, in the case of Journeymen).</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/admin/member-status',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>This page is for managing Member Satuses, which control which lists workers appear on, and which roles can dispatch those workers.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2019-03-07 12:23:02 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Dispatch - Prefs - HFE',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1657026886,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '9bea50c5-f275-40b1-baf3-ee8ff9031ccd',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_worker_dispatches/hfe',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>If you are waiting for a call from a specific employer, you can put a hold on your dispatches using this tool. For the specified length of time, you will not receive calls from any employers&nbsp;<strong>except</strong>&nbsp;the one that you specify.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2022-07-05 06:14:46 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Sirius Industry List',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1569946780,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'a5b14e62-ac4d-43d0-a6df-133a497c3f9c',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/taxonomy/sirius_industry',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>This is the list of all industries configured in your system.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2019-10-01 09:19:40 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Event - Intake - Application - Manage - Member Status',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1620574244,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'a8a90d7f-0f3a-4993-9053-baa30ee2b7e1',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_event_intake_application_manage/ms',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>This form may be used to add new member status / industries to the worker, based on their intake application shown on the right. To remove member status / industries, or to access other options, please go to the worker\'s "Industries" tab.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-05-09 08:30:44 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Labor Projection - Edit',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1630420702,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'a91ac14f-d4fa-47e8-9d57-919376f83630',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use the form below to enter the labor projection for the month.</p>

<p><strong>Groups&nbsp;or Shows</strong></p>

<p>If groups or shows have been selected, enter the number of anticipated workers for each group / show for each day.</p>

<p><strong>Total</strong></p>

<p>Enter the total number of anticipated workers for each day. If groups or shows are used, it is not necessary for the total number of workers to be equal to the sum of the workers at each show.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/employer_labproj_edit',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use the form below to enter the labor projection for the month.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-08-31 07:38:22 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Twilio - Call - Worker',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1623775600,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'a9d8a51f-dc22-4326-9d3c-331090c171fd',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use this page to associate a call or a recording with a worker record. When the call or recording is linked to the worker record, then it will appear in the Worker\'s log.</p>

<p>If the call came from a number that is associated with a worker, then that worker is suggested as the default link.</p>

<p>Note that linking a call automatically also links any associated recording, and linking any recording also links any associated call.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_twilio_call/worker',
        'format' => NULL,
      ),
      1 => array(
        'value' => 'node/%/sirius_twilio_recording/worker',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use this page to associate a call or a recording with a worker record.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-06-15 09:46:40 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Dispatch - Configuration - Display',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1604925280,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'ab560b72-e059-4d57-a8c0-a23489fa53d9',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/dispatch/config/view',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use this form to configure the way that dispatches are displayed. Additional settings can be found under <a href="/sirius/contact/definition/display">Contact Fields</a>.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2020-11-09 04:34:40 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Phone Number - Info',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1640907819,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'ad9a4f3c-ab1c-44cb-ba77-7dc592cc30d9',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p><strong>SMS / Voice Possible</strong></p>

<p>These flags indicate whether the system is permitted to send SMS or Voice messages to the phone number. It is not possible to set these flags directly; they are calculated from the other settings shown below.</p>

<p><strong>Last Scan</strong></p>

<p>This section indicates the results of the last "scan" of the number. Numbers are generally scanned at most once a month. (It is possible to disable scans in the <a href="/sirius/config/phone/twilio">Twilio settings</a>; scans reduce the number of texts sent to invalid numbers, but there is a small fee each time a number is scanned.)&nbsp;If a scan returns no carrier, then both SMS and Voice are both disabled for the number. If the scan returns no mobile carrier, then SMS is disabled.</p>

<p><strong>Last Outgoing SMS</strong></p>

<p>This indicates the last outgoing SMS message sent by the system to this number. Prior messages are recorded in the log.</p>

<p><strong>Last Incoming SMS</strong></p>

<p>This indicates the last incoming SMS sent to the system by this number. Prior messages are stored in the log.</p>

<p><strong>User Block</strong></p>

<p>This indicates whether the owner of this number has blocked text messages sent by this system. Blocks are created when the user sends a message which consists of one of the strings \'STOP\', \'STOPALL\', \'UNSUBSCRIBE\', \'CANCEL\', \'END\', \'QUIT\', or \'SIRIUSSTOP\'. (Case is unimportant, but the message can\'t contain additional text; so for example "Stop!" will work, but "Please Stop" will not.)</p>

<p>Once the user has sent one of these messages, then SMS messaging to this number is disabled until the user sends a subsequent message which consists of one of the strings&nbsp;\'START\', \'YES\', or \'SIRIUSSTART\'.</p>

<p>User blocks cannot be created or removed by administrators. Only the owner of the phone number can create or remove user blocks.</p>

<p><strong>Manual Block</strong></p>

<p>This tool can be used by administrators to manually disable text or voice messaging to the number. For example, if the user contacts the site administrator and asks to stop receiving messages, the administrator might put a manual block on the number.</p>

<p><strong>Contacts and Workers</strong></p>

<p>If the phone number is associated with any contact or worker records, they will be displayed at the bottom of the page.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_phonenumber_info',
        'format' => NULL,
      ),
      1 => array(
        'value' => 'node/%|sirius_phonenumber',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use this page to view information about a phone number.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-12-30 15:43:39 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Domain - Components - Root',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1655556507,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'b0ce1e82-a853-4e67-b7b0-67143919709a',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Select which components are enabled for the root domain.&nbsp;</p>

<p><strong>Root Domain, Not Global Setting</strong></p>

<p>Settings here will not impact settings for subdomains. For example, if the "Grievance" components is disabled at the root, it may still be enabled for a domain or subdomain.</p>

<p><strong>Legacy</strong></p>

<p>Most new Sirius sites should have their content in domain. This tool is primarily intended to support legacy sites, where the content is in no domain.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/domain/components/root',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Select which components are enabled for the root domain. This will not affect enabled for other domains.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2022-06-18 05:48:27 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Office - Operator - Settings',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1623238832,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'b1984832-95c7-472e-a827-0e28215a92fc',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p><strong>Incoming Phone Number</strong></p>

<p>This is the public-facing phone number for the office.</p>

<ul>
	<li>This must be a phone number that is fully hosted at Twilio (not a caller ID) and able to accept incoming voice calls.</li>
	<li>The number must be set so that incoming voice calls to this number use a "Webhook" pointing to https://[domain name]/sirius/twilio/incoming/voice</li>
	<li>Calls to this number will appear on the "Incoming Calls" tab for the operator.&nbsp;</li>
	<li>Multiple operators may share the same incoming phone number.</li>
</ul>

<p><strong>Outgoing Phone&nbsp;Number</strong></p>

<p>This is a private phone number for forwarding calls to the office.</p>

<ul>
	<li>This number in general should&nbsp;<strong>not</strong>&nbsp;be hosted at Twilio. It should be an external number.</li>
	<li>If an incoming call is forwared to the operator, this is the number that will be dialed.</li>
</ul>

<p><strong>Bypass Phonetree</strong></p>

<p>If set to "Yes", calls to this number will bypass the dispatch phonetree. Callers will not be asked to enter their dispatch ID and will not have an opportunity to hear their current dispatches. Instead, immediately upon calling, they will be connected with the office.</p>

<p><strong>Recording Calls</strong></p>

<p>A call which is forwarded to the operator may, optionally, be recorded.</p>

<ul>
	<li>Set Record = "Active" to indicate that these calls should be recorded.</li>
	<li>For recorded calls, select a suitable "recording mode", i.e. "record-from-answer-dual".&nbsp;</li>
	<li>See <a href="/sirius/twilio/recordings">the main recordings page</a>&nbsp;for a list of recordings and to set recording retention policies.</li>
	<li>For more information about audio recording, consult the Twilio documentation.</li>
</ul>

<p><em>The user of this system is responsible for complying with all applicable laws regarding call recording.</em></p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_operator_settings',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use this page to define the settings for this operator.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-06-09 04:40:32 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Bulk Message - Body - Voice',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1622454492,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'b361fd53-8ddd-44c7-902e-f2158c2d3aca',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p><strong>Caller ID</strong></p>

<p>For outgoing voice messages, a caller ID can be selected. Caller IDs must be configured and validated. The valid caller IDs can be managed <a href="/sirius/config/phone/callerid">here.</a></p>

<p><strong>Answering Machine Detection</strong></p>

<p>It is possible to specify different messages to be delivered if an answering machine does and does not pick up. Answering machine detection settings can be configured <a href="/sirius/config/phone/twilio/amd">here.</a></p>

<p><strong>Message Content</strong></p>

<p>The message may be provided either as an uploaded file, text, or a "Twilio Flow".</p>

<ul>
	<li>If the message is uploaded, a file should be provided in WAV or MP3 format.&nbsp;You may want to use a service such as <a href="https://online-voice-recorder.com/" target="_blank">online-voice-recorder.com</a>. You may upload separate messages for human and answering machine recipients.<br />
	&nbsp;</li>
	<li>If text-to-speech is selected, then text can be entered which will be converted into an audio and played for the recipient. Separate messages are available for human and answering machine recipients.<br />
	&nbsp;</li>
	<li>If a "Twilio Flow" is selected, then a Flow must be created using Twilio Studio. The Flow should start with a "Rest API" trigger, and the first action should be "Make Outgoing Call". It may be useful to add a widget to split based on the "AnsweredBy" field, so that the Flow can provide different responses to human and answering machine recipients.</li>
</ul>

<p><strong>Twilio Flow Variables</strong></p>

<p>If control is passed to a Twilio Flow, then contact variables can be included in the Flow. This might be used, for example, to play a different audio file for each contact, or to use text-to-speech to address the call recipients individually by name.</p>

<p>To access the variable in the Flow, use the following synext:</p>

<table border="0" cellpadding="1" cellspacing="1">
	<tbody>
		<tr>
			<td><strong>Sirius Variable Name</strong></td>
			<td><strong>Twilio Flow Variable Name</strong></td>
		</tr>
		<tr>
			<td>:sirius_contact:core:nid</td>
			<td>{{flow.data.sirius_contact__core__nid}}</td>
		</tr>
		<tr>
			<td>:sirius_contact:core:field_sirius_phone:value</td>
			<td>{{flow.data.sirius_contact__core__field_sirius_phone__value}}</td>
		</tr>
		<tr>
			<td>&nbsp;:sirius_contact:default:custom_fieldset:custom_variable_name</td>
			<td>{{flow.data.sirius_contact__default__custom_fieldset__custom_variable_name}}</td>
		</tr>
	</tbody>
</table>

<p>&nbsp;</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_bulk_body_voice',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use this tool to specify the voice message that should be played when this bulk message is delivered.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-05-31 02:48:12 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Worker - Member Status Notifications',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1601383094,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'b55c0593-d616-4780-bb0c-0ff356dfe8bc',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'taxonomy/term/%/sirius_member_status_tos',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Enter the email addresses which should receive a notification when a worker adds or removes this member status.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2020-09-29 05:38:14 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Grievance Core',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1570479121,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'be50e9e6-96ba-4bbe-8784-6431af1a9b7c',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'admin/config/grievance/core',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>This page covers a range of configuration options for the Grievance Module.&nbsp; For assistance with these settings, please contact your Sirius deployment team.</p>

<ul>
	<li>Status Types: configure special statuses within the system, e.g. what is the first status within all grievances, or statuses used for certain reporting.</li>
	<li>Member Notification: configure the base member notification from the system.</li>
	<li>Alerts: configure who should be alerted about timelines when.</li>
	<li>Managers: select users should be in roles that get assigned to all grievances in the system as managers, watchers or assignees.</li>
	<li>Messaging: set the footer message for the site.</li>
	<li>Loopback IP: set a priviledged IP for certain callback functions.</li>
	<li>Hide Fields: list which fields are hidden in the system (because they don\'t apply for a given site).</li>
	<li>From Email: set the email address for emails sent from the system.</li>
	<li>My Grievances: select which view is used for the "My Grievances" tab and overwrite column headers</li>
	<li>Grievance IDs: configure the pattern used for naming grievances</li>
	<li>Emailed Attachments: settings for emailing attachments into the system</li>
</ul>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2019-10-07 13:12:01 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Dispatch - Three Strikes',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1628250867,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'beba50c6-f898-4d53-8cca-488c6da427b3',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>The "three strikes" tool signs out workers who decline or ignore dispatches on successive days.</p>

<p>Each day, the tool scans all workers. If the worker has declined or ignored a dispatch on each of the three preceding days, the worker is signed out of the dispatch system. Under some configurations, they also have a sign-in block applied, preventing them from signing back in until the block has been cleared by an administrator.</p>

<p><strong>Tester</strong></p>

<p>The tester can be used to run the three-strikes scan and view detailed output. The scan can be run in "live" mode, or in "test" mode. In "test" mode, no changes are actually made; the system indicates how many workers would be affected by a three-strikes scan, but does not actually change their sign-in status.</p>

<p>The tester can be run on a single worker, or on all workers.</p>

<p><strong>Settings</strong></p>

<p>On the settings page, administrators can turn on or off automatic three-strikes scans. When scans are active, the time of day that they should run can be set, and the notifications that will be sent to workers can be specified.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/dispatch/threestrikes',
        'format' => NULL,
      ),
      1 => array(
        'value' => 'sirius/dispatch/threestrikes/config',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>The "Three Strikes" tool signs out workers who decline or ignore dispatches on successive days.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-08-06 04:54:27 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Labor Projection - Status',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1630416763,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'c3e86e0b-b543-49ca-ad56-93c7ef79591e',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p><strong>Status</strong></p>

<p>Use this tool to set the status for the labor projection. Incomplete labor projections should have their status set to "draft". Complete labor projections should be set to "submitted".</p>

<p><strong>Comments</strong></p>

<p>Use this field to indicate any comments or special information about this labor projection.</p>

<p><strong>Job Groups / Shows</strong></p>

<p>If your labor projection includes job groups or shows, you must select which ones will be included. Projections can only be submitted for the groups or shows that are selected here.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/employer_labproj_status',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use this tool to set the labor projection status, and select the job groups or shows that will be included.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-08-31 06:32:43 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Bulk Messaging - Configuration',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1603452352,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'c5446725-4709-4e42-a96e-ad303917c8d7',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/bulk/config',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use the form below to configure settings for the bulk-message system. These settings are not domain-specific; they apply globally to all domains on the site.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2020-10-23 04:25:52 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Config - Employer User Manager',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1638204158,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'c9163e11-2cd2-42b2-a5f9-f085fe6c6569',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>The Employer User Manager is the tool that employers use to manage their own users. Users connected with an employer (if they have the correct permissions) can add additional users, remove users, or reset users\' passwords. To access the employer user manager, go to the <a href="/sirius/admin/shops">employer list page</a> and click on "Users" for any employer.</p>

<p>This page is used to configure the tool. For each of the user roles on the system, select whether that role should be:</p>

<ul>
	<li><strong>Required:</strong> EVERY user created through the employer user manager must have this role. For example, a role such as "Employer User" might be required.</li>
	<li><strong>Optional:</strong> Users created through the tool may optionally have this role. For example, there might be a role such as "Employer Dispatch Manager", which would be granted only to certain employer users.</li>
	<li><strong>Omit:</strong> NO user created through this tool will have this role. For example, administrative roles such as "Sirius Admin" would probably be omitted.</li>
</ul>

<p>&nbsp;</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/employer/config/usermanage',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Configure roles for the employer user manager.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-11-29 08:42:38 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Ledger - Account A/R',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1607425129,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'c9492c91-e2c0-4771-8c1a-fdb90e1c63d6',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_ledger_account_ar',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>The table below inclues all the A/R entries associated with this account. Only the most recent 1,000 entries are shown.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2020-12-08 02:58:49 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Phaxio',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1617023281,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'ca70e4b5-edd0-40c6-82fd-e58ff606e319',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>The Phaxio service (<a href="https://www.phaxio.com" target="_blank">www.phaxio.com</a>) provides fax services for Sirius. This is used in the Letters system. If the Phaxio service is configured, then letters may be send by fax.</p>

<p>The settings (API Key, Secret key, and the callback token) may be found on the Phaxio account page.</p>

<p>The API Host should generally be set to&nbsp;<strong>https://api.phaxio.com/v1/</strong></p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'admin/config/grievance/phaxio',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Phaxio provides fax services for Sirius software.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-03-29 06:08:01 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Dispatch - Prefs - DNC2',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1657026735,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'cc3f0b78-70e5-4f1c-9c9a-0695f2f43950',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_worker_dispatches/dnc2',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>If you would prefer <strong>NOT </strong>to receive dispatches from certain employers, select them below.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2022-07-05 06:12:15 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Email - Settings',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1633959834,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'cdcb4b41-962c-441f-8deb-61ecce6adb19',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p><strong>Settings</strong></p>

<p>Use this page to configure settings for outgoing emails for the current domain. If any values on this page are left blank, the global settings will be used.</p>

<ul>
	<li><strong>Username:</strong>&nbsp;The user name for the SMTP service. With SendGrid / Twilio, this should generally be "apikey".</li>
	<li><strong>Password:</strong>&nbsp;The SMTP password. With SendGrid / Twilio, this should generally be the API key configured for this site or domain.</li>
	<li><strong>Host, Backup Host:</strong>&nbsp;The SMTP host to use. With SendGrid / Twilio, this should be "smtp.sendgrid.net". The backup host is not required.</li>
	<li><strong>Port:&nbsp;</strong>The port expected by the SMTP provider. With SendGrid / Twilio, this should be 587</li>
	<li><strong>From Address:</strong>&nbsp;The email address that ALL messages from this site or domain originate from.&nbsp;<em>This must be a properly configured sender at the SMTP provider.</em></li>
	<li><strong>From Name:</strong>&nbsp;The name that will appear as the sender of all messages from this site or domain.&nbsp;</li>
</ul>

<p><strong>Test</strong></p>

<p>Use the "test" page to validate the email configuration by sending a test message.</p>

<p><strong>Global Settings</strong></p>

<p><em>The "Global Settings" page is only available to site administrators (users with the "sirius admin" permission.)</em></p>

<p>If any variable is not set for a given domain, the corresponding global settings are used. Global settings also have additonal options, including:</p>

<ul>
	<li><strong>Global Disable</strong><strong>:&nbsp;</strong>If "Global Disable" is on, then ALL emails will fail unless the recipients appear on the whitelist</li>
</ul>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/config/email',
        'format' => NULL,
      ),
      1 => array(
        'value' => 'sirius/config/email/config',
        'format' => NULL,
      ),
      2 => array(
        'value' => 'sirius/config/email/test',
        'format' => NULL,
      ),
      3 => array(
        'value' => 'sirius/config/email/global',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use these tools to set up email for the domain or the site.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-10-11 06:43:54 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Dispatch - Configuration - Stub Jobs',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1599302955,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'cddbdd66-1025-4fd8-9cc7-cce96a5a6309',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/dispatch/job/stub',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>A "stub" job is a job that is created at an employer as an administrative tool to allow dispatching workers without the need to create an a real job. For example, if legacy dispatch data needs to be imported in a situation where good historical job data doesn\'t exist, it might be appropriate to create a "stub" job at each employer, as a placeholder for historic dispatches. Similarly, if an employer has a single ongoing contract with workers coming and going periodically, it might be appropriate to give the employer a single ongoing "stub" job.</p>

<p>This tool can be used to manage stub jobs. Select "create" to create or update stub jobs at selected employers. (There will never be more than one stub job for a given employer and industry.) Select "download" to download the list of stub jobs; this can be useful, for example, if it is necesary to dispatch workers in bulk to stub jobs via a feed.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2020-09-05 03:49:15 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Ledger - Payperiod Test',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1599564124,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'd9034b35-4f3c-45c4-bd41-bb0fa72755b5',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_payperiod_ledger_test',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use the form below to test ledger entries for this payperiod. The payperiod node will be saved, and a log of ledger computations will be displayed.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2020-09-08 04:22:04 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Service - Client',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1649772002,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'db849e14-403e-4995-b000-35f1d54ee550',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Configure this domain as a client. The authoritative server is hosted elsewhere, and service requests will be made to that server.</p>

<p><strong>Client Active:</strong>&nbsp;Is this domain a client for a remote server?</p>

<p><strong>Remote Host:</strong>&nbsp;The domain name of the remote server.&nbsp;</p>

<p><strong>Account ID:</strong> The account ID to use when accessing the remote server.</p>

<p><strong>Access Token: </strong>The access token to use when accessing the remote server.</p>

<hr />
<p>Note that a <a href="/admin/structure/services">Drupal service must also be configured</a>.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/service/config/client',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Configure this domain as a client.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2022-04-12 07:00:02 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Special Contact Types',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1570477945,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'dc6b1584-43e4-4a3f-a722-7b7bcb8a7c55',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'admin/config/grievance/contacts',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>This page is for configuration of special contact types, e.g. "Shop Stewards" are what contact type.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2019-10-07 12:52:25 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Events - Catalogs',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1643378560,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'e9d99dcc-24d2-408a-842b-c6c154edd39d',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>A "catalog" is a collection of event types. They are typically used to display a set of course offerings, with each event types representing courses.&nbsp;</p>

<p>Catalogs can be restricted by role, or they can be made public so that anonymous users can view them.</p>

<p>A typical domain will usually only have a single catalog, but sometimes it might be appropriate to divide classes into multiple catalogs, e.g. breaking them up by industry or by population served.</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/event/catalog/list',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Catalogs are collections of event types, typically courses. They can be public.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2022-01-28 06:02:40 -0800',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'Twilio - Recording',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1623346779,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'f95f6f62-df09-42d2-9e4c-dc932721e865',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>This page shows the local, or downloaded, copy of a call recording.</p>

<p><strong>Recording Information</strong></p>

<ul>
	<li>SID: The unique (remote) identifier for the recording record at the telelphony provider.</li>
	<li>Date: The date that the recording started. This may not be identical to the date that the call started. For example, recording might not begin until a call is forwarded to an operator.</li>
	<li>Duration: The duration of the recording.</li>
</ul>

<p><strong>Call Information</strong></p>

<ul>
	<li>SID: The unique (remote) identifier for the call instance at the telephony provider.</li>
	<li>From, To: The phone numbers associated with the call. Note that for certain call instances (conference calls or forwarded calls, for example) these numbers might not be available.</li>
	<li>Date: The date that the call started.</li>
</ul>

<p>&nbsp;</p>
',
        'summary' => '',
        'format' => 'full_html',
        'safe_summary' => '',
      ),
    ),
  ),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'node/%/sirius_twilio_recording',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>This page shows the local (downloaded) copy of a call recording.</p>
',
        'format' => 'full_html',
      ),
    ),
  ),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title',
      ),
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
  ),
  'date' => '2021-06-10 10:39:39 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  return $nodes;
}
