<?php

/**
 * @file
 * grievance_content_types.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function grievance_content_types_taxonomy_default_vocabularies() {
  return array(
    'grievance_alert_types' => array(
      'name' => 'Grievance Alert Types',
      'machine_name' => 'grievance_alert_types',
      'description' => 'What action is needed to move a grievance from one step to another.  e.g "Hold Step 1 Meeting" or "Refer to Arbitration"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'grievance_assignee_notes' => array(
      'name' => 'Grievance Assignee Notes',
      'machine_name' => 'grievance_assignee_notes',
      'description' => 'Directions/notes to the temporary grievance assignee. e.g. "Please Translate"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'grievance_broughtby' => array(
      'name' => 'Grievance Brought By',
      'machine_name' => 'grievance_broughtby',
      'description' => 'What entity is filing the grievance.  e.g. "Martian Invaders Local 51".  Not used for grievance sites that have only one entity.',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'grievance_category' => array(
      'name' => 'Grievance Category',
      'machine_name' => 'grievance_category',
      'description' => 'The type of grievance.  e.g. "Contract" or "Discipline"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'grievance_contact_types' => array(
      'name' => 'Grievance Contact Types',
      'machine_name' => 'grievance_contact_types',
      'description' => 'The type of contact.  e.g. "General Manager" or "Human Resources"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'grievance_contract_clause_tags' => array(
      'name' => 'Grievance Contract Section Tags',
      'machine_name' => 'grievance_contract_clause_tags',
      'description' => 'Tags for describing sections, e.g. "Step 1 Procedure"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'grievance_contract_section_tags' => array(
      'name' => 'Grievance Contract Article Tags',
      'machine_name' => 'grievance_contract_section_tags',
      'description' => 'Tags for describing Articles, e.g. "Union Recognition" or "Grievance Procedure"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'grievance_contract_template_tags' => array(
      'name' => 'Grievance Contract Template Tags',
      'machine_name' => 'grievance_contract_template_tags',
      'description' => 'Tags for describing contract templates,  e.g. "Food Service" or "Statewide Contract"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'grievance_delivery' => array(
      'name' => 'Grievance Delivery',
      'machine_name' => 'grievance_delivery',
      'description' => 'How the letter will be delivered. e.g. "Via e-mail" or "Via fax"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'grievance_department' => array(
      'name' => 'Grievance Department',
      'machine_name' => 'grievance_department',
      'description' => 'The department a worker or grievant belongs to.  e.g. "Housekeeping" or "Drivers"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'grievance_document_types' => array(
      'name' => 'Grievance Document Types',
      'machine_name' => 'grievance_document_types',
      'description' => 'Attachment Categories & Information Requests',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'grievance_job_classification' => array(
      'name' => 'Grievance Job Classification',
      'machine_name' => 'grievance_job_classification',
      'description' => 'Specific job classifications. e.g. "Server" or "Driver"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'grievance_log_tags' => array(
      'name' => 'Grievance Log Tags',
      'machine_name' => 'grievance_log_tags',
      'description' => 'Tags for describing or flagging log entries.  e.g. "Grievant Complaint"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'grievance_log_types' => array(
      'name' => 'Grievance Log Types',
      'machine_name' => 'grievance_log_types',
      'description' => 'The type of log entries recorded for a grievance.  e.g. "Communication - Company - Notification"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'grievance_outcome' => array(
      'name' => 'Grievance Outcome',
      'machine_name' => 'grievance_outcome',
      'description' => 'The final outcome of the grievance.  e.g. "Won" or "Declined to pursue"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'grievance_remedies' => array(
      'name' => 'Grievance Remedies',
      'machine_name' => 'grievance_remedies',
      'description' => 'The list of default remedies.  e.g. "Make whole"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'grievance_resproc' => array(
      'name' => 'Grievance Resolution Procedures',
      'machine_name' => 'grievance_resproc',
      'description' => 'List of  procedures to resolve a grievance.  e.g. "Mediation" or "Formal Arbitration"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'grievance_settlement_tags' => array(
      'name' => 'Grievance Settlement Tags',
      'machine_name' => 'grievance_settlement_tags',
      'description' => 'Tags for describing settlements.  e.g. "Wage Reimbursement" or "Benefit Contribution"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'grievance_shift' => array(
      'name' => 'Grievance Shift',
      'machine_name' => 'grievance_shift',
      'description' => 'The shift a worker or grievant is normally scheduled.  e.g. "Graveyard" or "AM Shift"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'grievance_status' => array(
      'name' => 'Grievance Status',
      'machine_name' => 'grievance_status',
      'description' => 'List the statuses that a grievance can have: "Step 1", "No Further Action", "Arbitration Requested"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'grievance_tags' => array(
      'name' => 'Grievance Tags',
      'machine_name' => 'grievance_tags',
      'description' => 'Tags for describing grievances.  e.g. "High Priority" or "Precedent Setting"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'grievance_types' => array(
      'name' => 'Grievance Types',
      'machine_name' => 'grievance_types',
      'description' => 'The grievance type or complaint.  e.g. "Attendance Policy" or "Health and Safety"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'grievance_work_status' => array(
      'name' => 'Grievance Work Status',
      'machine_name' => 'grievance_work_status',
      'description' => 'The work status for the grievant.  e.g. "Full Time" or "Per Shift"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'sirius_contact_tags' => array(
      'name' => 'Sirius Contact Tags',
      'machine_name' => 'sirius_contact_tags',
      'description' => 'Tags for contacts, e.g. "Likely Voter" or "New Import"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'sirius_dispatch_job_tags' => array(
      'name' => 'Sirius Dispatch Job Tags',
      'machine_name' => 'sirius_dispatch_job_tags',
      'description' => 'Tags for describing jobs.  e.g. "As soon as possible" or "High Priority"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'sirius_dispatch_job_type' => array(
      'name' => 'Sirius Dispatch Job Type',
      'machine_name' => 'sirius_dispatch_job_type',
      'description' => 'List of job types in the system.  e.g. "OPEN" or "CBN"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'sirius_dispatch_sib' => array(
      'name' => 'Sirius Dispatch Sign-in Status',
      'machine_name' => 'sirius_dispatch_sib',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'sirius_gender' => array(
      'name' => 'Sirius Gender',
      'machine_name' => 'sirius_gender',
      'description' => 'List the names to be shown in the gender dropdown menus on this site, e.g. "Female", "Nonbinary", "Male"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'sirius_industry' => array(
      'name' => 'Sirius Industry',
      'machine_name' => 'sirius_industry',
      'description' => 'List of industries, e.g. "Convention" or "Construction"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'sirius_member_status' => array(
      'name' => 'Sirius Member Status',
      'machine_name' => 'sirius_member_status',
      'description' => 'List of member statuses, e.g. "Journeyman" or "Apprentice"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'sirius_skill' => array(
      'name' => 'Sirius Skill',
      'machine_name' => 'sirius_skill',
      'description' => 'The skills a worker has, and how long they last.',
      'hierarchy' => 1,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'sirius_work_status' => array(
      'name' => 'Sirius Work Status',
      'machine_name' => 'sirius_work_status',
      'description' => 'List of work statuses, e.g. "Active Member" or "New Applicant"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'sirius_worker_dispatch_status' => array(
      'name' => 'Sirius Worker Dispatch Status',
      'machine_name' => 'sirius_worker_dispatch_status',
      'description' => 'List of statuses a dispatch can have, e.g. "Requested" or "Notified" or "Accepted"',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
  );
}
