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
  'title' => 'sirius/config/ledger/test-monthly',
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
  'title' => 'node/%/sirius_event_participant_definition_followup',
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
  'title' => 'sirius/contact/definition/display',
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
  'title' => 'sirius/config/feed',
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
  'title' => 'ASI Disclaimer',
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
  'title' => 'sirius/admin/do_not_disturb',
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
  'title' => 'Dispatch - Dispatch - SMS',
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
  'title' => 'Create a Job - begin here',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => '',
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
  'title' => 'sirius/config/phone/twilio/amd',
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
  'title' => 'sirius/twilio/network-test',
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
  'title' => 'sirius/dispatch/config/view',
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
  'title' => 'node/%/sirius_bulk_body',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1603968551,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => 'ad4183ab-d3a5-41f5-834b-55c51a255b9b',
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p><strong>SMS (Text) Messages</strong></p>

<p>The cost of an SMS message is dependent on the number of "segments" required. A segment count is displayed below the "SMS Body" field. Generally speaking, the best practice is to try to keep the message to just one segment.</p>

<p><strong>Voice (Robocall) Messages</strong></p>

<p>For robocalls, the message can be entered in text or as an uploaded audio file. If both are provided, the uploaded audio file will be used.&nbsp;</p>

<p><strong>Email Messages</strong></p>

<p>Email messages may be entered in HTML format. However, be careful to test your message carefully, since not all email clients can render all HTML tags.</p>
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
        'value' => 'node/%/sirius_bulk_body',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Enter the subject and / or body of the message.</p>
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
  'date' => '2020-10-29 03:49:11 -0700',
  'user_uuid' => '88058974-df68-466a-bc2d-adc1b80e4cd5',
);
  $nodes[] = array(
  'title' => 'taxonomy/term/%/sirius_member_status_tos',
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
  'title' => 'sirius/bulk/config',
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
  'title' => 'sirius/dispatch/job/stub',
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
  'title' => 'node/%/sirius_payperiod_ledger_test',
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
  return $nodes;
}
