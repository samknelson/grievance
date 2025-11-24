<?php

sirius_require('sirius', 'Sirius/Batchrunner.inc');
sirius_require('sirius', 'Sirius/Batchrunner_Node.inc');

// drush --user=1 php-eval "sirius_require('unite', 'unite.oneoff_migrate_contract.inc'); Sirius_Unite_Batchrunner_Oneoff_Contract_Migrate::getInstance()->cl('test');"

class Sirius_Unite_Batchrunner_Oneoff_Contract_Migrate extends Sirius_Batchrunner_Node {
	public $type = 'Sirius_Unite_Batchrunner_Oneoff_Contract_Migrate';
	public $name = 'Sirius_Unite_Batchrunner_Oneoff_Contract_Migrate';

	/*
	public function get_next_batch(&$context = array()) {
		return array(45598);
	}
	*/

	public function content_type_impl(&$context = array()) { return 'property'; }

	public function process_record_impl($property_node, &$context = array()) {
		$local_nid = $property_node->field_local['und'][0]['target_id'];
		if (!$local_nid) {
			sirius_minilog("No local.");
			return;
		}

		$mode = $this->mode($context);
		$property_nid = $property_node->nid;

		$lookups = &drupal_static(__FUNCTION__);
		if (!isset($lookups['unite_here_nid'])) {
			// Get the IU's NID
			$sql = "select * from field_data_field_title_canon where entity_type = 'node' and bundle = 'actor' and field_title_canon_value = 'UNITEHEREIU' ";
			$stmt = db_query($sql, array());
			$hr = $stmt->fetchAssoc();
			$hr2 = $stmt->fetchAssoc();
			if (!$hr) { die("UNITE HERE actor not found."); }
			if ($hr2) { die("UNITE HERE actor found twice."); }
			$lookups['unite_here_nid'] = $hr['entity_id'];
			sirius_minilog("Found UNITE HERE IU $lookups[unite_here_nid]");
		}

		if (!isset($lookups['union_tid'])) {
			$terms = sirius_taxonomy_load('actor_type', 'tid', 'name');
			$terms = array_flip($terms);
			$lookups['union_tid'] = $terms['Union'];
			$lookups['union_local_tid'] = $terms['Union Local'];
			if (!$lookups['union_tid'] || !$lookups['union_local_tid']) { die("Terms for Union, Local not found."); }
			sirius_minilog("Found local TID $lookups[union_local_tid]");
		}

		$local_node = node_load($local_nid);
		if (!$local_node) {
			sirius_minilog("Local not found: $local_nid", 'warning');
			return;
		}
		$local_title = $local_node->title;
		$local_title_canon = unite_actor_canonicalize_title($local_title);

		// //////////////////////////
		// Get or create the actor
		// //////////////////////////

		sirius_minilog("Looking up local: $local_title_canon");
		if (!$local_title_canon) {
			sirius_minilog("The local has no title. Skipping.", 'warning');
			return;
		}

		$sql = "select * from field_data_field_title_canon where entity_type = 'node' and bundle = 'actor' and field_title_canon_value = :title_canon ";
		$stmt = db_query($sql, array(':title_canon' => $local_title_canon));
		$hr = $stmt->fetchAssoc();
		$hr2 = $stmt->fetchAssoc();
		if ($hr2) { die("Actor $local_title_canon found twice"); }

		if ($hr) {
			$actor_nid = $hr['entity_id'];
		} else {
			sirius_minilog("Creating a new actor.");

      $actor_node = new stdClass();
      $actor_node->type = "actor";
      $actor_node->language = LANGUAGE_NONE;
      $actor_node->uid = 1;
      node_object_prepare($actor_node);
      $actor_node->title = $local_title;
      $actor_node->field_actor_type['und'][0]['tid'] = $lookups['union_local_tid'];
      $actor_node->field_actors['und'][0]['target_id'] = $lookups['unite_here_nid'];
      $actor_node = node_submit($actor_node);

      if ($mode == 'live') {
      	node_save($actor_node);
      	sirius_minilog("Created actor: " . $actor_node->title . " (" . $actor_node->nid . ')');
      } else {
      	sirius_minilog("[skipping save because mode = $mode]");
      }
      $actor_nid = $actor_node->nid;
		}

		sirius_minilog("Using actor $actor_nid");

		// //////////////////////////
		// Get or create the relationship
		// //////////////////////////

		$relationship_type = 'Union';
		if (!$property_nid || !$actor_nid || !$relationship_type) { die("Missing required field."); }
		sirius_minilog("Looking up relationship ($property_nid, $actor_nid, $relationship_type)");

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'property_actor');
    $query->fieldCondition('field_property', 'target_id', $property_nid);
    $query->fieldCondition('field_actor', 'target_id', $actor_nid);
    $query->fieldCondition('field_relationship_type', 'value', $relationship_type);
    $stmt = $query->execute();
    if ($stmt['node']) {
    	sirius_minilog("Relationship already exists.");
    	$relationship_node = node_load(reset(array_keys($stmt['node'])));
    } else {
    	sirius_minilog("Relationship not found. Creating.");

	    $relationship_node = new stdClass();
	    $relationship_node->type = 'property_actor';
	    $relationship_node->language = LANGUAGE_NONE;
	    $relationship_node->uid = 1;
	    $relationship_node->status = 1;
	    $relationship_node->promote = 0;
	    $relationship_node->comment = 0;
	  }

    $relationship_node->field_property['und'][0]['target_id'] = $property_nid;
    $relationship_node->field_actor['und'][0]['target_id'] = $actor_nid;
    $relationship_node->field_relationship_type['und'][0]['value'] = $relationship_type;

    $union_status = $property_node->field_union_status['und'][0]['value'];
    $other_union = $property_node->field_union_other['und'][0]['value'];
    $other_union = preg_replace('/\?/', '', $other_union);
    $other_union = trim($other_union);
    $start_date = $property_node->field_hotel_contract_start_date['und'][0]['value'];
    $end_date = $property_node->field_hotel_contract_end_date['und'][0]['value'];

  	unset($relationship_node->field_start_date['und']);
  	unset($relationship_node->field_end_date['und']);

  	unset($relationship_node->field_ur_contract['und']);
  	unset($relationship_node->field_ur_contract_start['und']);
  	unset($relationship_node->field_ur_contract_end['und']);

  	unset($relationship_node->field_ur_rep['und']);
  	unset($relationship_node->field_ur_rep_start['und']);
  	unset($relationship_node->field_ur_rep_end['und']);

  	unset($relationship_node->field_ur_orgag['und']);
  	unset($relationship_node->field_ur_orgag_start['und']);
  	unset($relationship_node->field_ur_orgag_end['und']);


		// Add Union relation with Jurisdiction "Y"
    $relationship_node->field_ur_juris['und'][0]['value'] = 'Y';

		// Union Status is "Union"= On same relation, add Represented "Y" AND if Contract Start Date and/or End Date have data= add Contract “Y” and attach Start/End dates to this relationship
    if ($union_status == 'U') {
    	$relationship_node->field_ur_rep['und'][0]['value'] = 'Y';

    	if ($start_date || $end_date) {
    		$relationship_node->field_ur_contract['und'][0]['value'] = 'Y';
	    	if ($start_date) { $relationship_node->field_ur_contract_start['und'][0]['value'] = $start_date; }
  	  	if ($end_date) { $relationship_node->field_ur_contract_end['und'][0]['value'] = $end_date; }
  	  }
    }

		// Union Status is "Non-Union"= On same relation, add Represented "N", Contract “N”, and ignore any data for Contract Start/End Dates
    if ($union_status == 'N') {
    	$relationship_node->field_ur_rep['und'][0]['value'] = 'N';
    	$relationship_node->field_ur_contract['und'][0]['value'] = 'N';
    }

		// Union Status is "CCNA"= On same relation, add Organizing agreement "Y" and ignore any data for Contract Start/End Dates
    if ($union_status == 'CCNA') {
    	$relationship_node->field_ur_orgag['und'][0]['value'] = 'Y';
    }

		// Union Status is "Non-UNITE HERE Union"= On same relation, add Represented "N" and ignore any data for Contract Start/End Dates
		if ($union_status == 'NUH') {
    	$relationship_node->field_ur_rep['und'][0]['value'] = 'N';
		}

    $relationship_node = node_submit($relationship_node);

    if ($mode == 'live') {
    	node_save($relationship_node);
    	sirius_minilog("Created relationship: " . $relationship_node->title . " (" . $relationship_node->nid . ')');
    } else {
    	sirius_minilog("[skipping save because mode = $mode]");
    }
	}
}
