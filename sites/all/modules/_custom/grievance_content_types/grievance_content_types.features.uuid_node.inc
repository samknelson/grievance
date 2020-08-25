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
  'title' => 'Sirius - Help Module - Publish',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1597840084,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '09cfa66f-633f-4c73-9bd7-f61d84cd5d7f',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/help/publish',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Before doing this, remember to execute from the command line:</p>

<pre>
 cd /var/www/test.dispatch.teamsters631.com/sites/all/modules/_custom/grievance_content_types; chmod 666 *</pre>

<p>Afeter executing, remember to execute</p>

<pre>
cd /var/www/test.dispatch.teamsters631.com/sites/all/modules/_custom/grievance_content_types; chmod 664 *; git commit -m "Updated help text" .</pre>
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
  'date' => '2020-08-19 05:28:04 -0700',
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
  'title' => 'Domain User Manager',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'ds_switch' => 'default',
  'type' => 'sirius_help',
  'language' => 'en',
  'created' => 1597837548,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '5942dd12-3daf-470d-a0c4-b4ef25f468a2',
  'body' => array(),
  'field_sirius_paths' => array(
    'und' => array(
      0 => array(
        'value' => 'sirius/domain/config/usermanage',
        'format' => NULL,
      ),
    ),
  ),
  'field_sirius_summary' => array(
    'und' => array(
      0 => array(
        'value' => '<p>Use the form below to set server-wide settings about how the domain user manager should behave.&nbsp;For each role, it is possible to select whether the user manager should&nbsp;</p>

<ul>
	<li><strong>Omit</strong>&nbsp;the role, meaning that the role is not available through the user manager. Typically higher-permissioned roles such as "sirius admin" will be omitted.</li>
	<li><strong>Require</strong>&nbsp;the role, meaning that all users created through the domain user manager will automatically have the role. Typically a low-permissioned role such as "sirius user" will be required.</li>
	<li>Make the role <strong>optional</strong>, meaning that the domain administrator creating the user can optionally decide whether or not to grant that role. Typically these will be roles that a domain admin might grant to some users but not others, such as "event admin" or "bulk message admin".</li>
</ul>

<p><em>This page has security implications and should be used with caution.</em></p>
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
  'date' => '2020-08-19 04:45:48 -0700',
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
