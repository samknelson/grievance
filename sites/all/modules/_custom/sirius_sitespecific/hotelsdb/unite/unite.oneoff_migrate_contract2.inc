<?php

sirius_require('sirius', 'Sirius/Batchrunner.inc');
sirius_require('sirius', 'Sirius/Batchrunner_Node.inc');

// drush --user=1 php-eval "sirius_require('unite', 'unite.oneoff_migrate_contract2.inc'); Sirius_Unite_Batchrunner_Oneoff_Contract_Migrate2::getInstance()->cl('test');"

class Sirius_Unite_Batchrunner_Oneoff_Contract_Migrate2 extends Sirius_Batchrunner_Node {
	public $type = 'Sirius_Unite_Batchrunner_Oneoff_Contract_Migrate2';
	public $name = 'Sirius_Unite_Batchrunner_Oneoff_Contract_Migrate2';

	public function content_type_impl(&$context = array()) { return 'property'; }

	public function grand_total_pending(&$context = array()) {
		$content_type = $this->content_type_impl($context);
		if (!$content_type) { return 0; }

		$last_pk = $this->last_pk($context);
		$sql = "select count(*) as c from node ";
		$sql .= "left join field_data_field_union_other on field_data_field_union_other.entity_type = 'node' and field_data_field_union_other.entity_id = node.nid ";
		$sql .= "where type = :content_type and nid > :last_pk and field_union_other_value is not null ";
		$stmt = sirius_sql_query($sql, array(':content_type' => $content_type, ':last_pk' => $last_pk));
		$hr = $stmt->fetchAssoc();
		return $hr['c'];
	}

	public function get_next_batch(&$context = array()) {
		// return array(33566);
		// return array(34135);
		$batch_size = $this->batch_size($context);
		$last_pk = $this->last_pk($context);
		$content_type = $this->content_type_impl($context);
		if (!$content_type) { return array(); }

		$sql = "select nid from node where type = :content_type and nid > :last_pk order by nid limit $batch_size";
		$sql = "select nid from node ";
		$sql .= "left join field_data_field_union_other on field_data_field_union_other.entity_type = 'node' and field_data_field_union_other.entity_id = node.nid ";
		$sql .= "where type = :content_type and nid > :last_pk and field_union_other_value is not null limit $batch_size";
		$stmt = sirius_sql_query($sql, array(':content_type' => $content_type, ':last_pk' => $last_pk));

		$pks = array();
		while ($hr = $stmt->fetchAssoc()) {
			$pks[] = $hr['nid'];
		}
		return $pks;
	}

	public function process_record_impl($property_node, &$context = array()) {
		$other_union_name = $property_node->field_union_other['und'][0]['value'];
		if (!$other_union_name) {
			sirius_minilog("No 'other union'");
			return;
		}

		$mode = $this->mode($context);
		$property_nid = $property_node->nid;
		sirius_minilog("Working on: " . $other_union_name);


		$lookups = &drupal_static(__FUNCTION__);
		if (!isset($lookups['union_tid'])) {
			$terms = sirius_taxonomy_load('actor_type', 'tid', 'name');
			$terms = array_flip($terms);
			$lookups['union_tid'] = $terms['Union'];
			if (!$lookups['union_tid']) { die("Terms for Union not found."); }
			sirius_minilog("Found local TID $lookups[union_local_tid]");
		}

		// //////////////////////////
		// Get or create the actor
		// //////////////////////////

		$other_union_name_canon = unite_actor_canonicalize_title($other_union_name);
		if (!$other_union_name_canon) { 
			sirius_minilog("'Other union' canonical name is empty");
			return;
		}
		sirius_minilog("Looking up: $other_union_name_canon");

		$sql = "select * from field_data_field_title_canon where entity_type = 'node' and bundle = 'actor' and field_title_canon_value = :title_canon ";
		$stmt = db_query($sql, array(':title_canon' => $other_union_name_canon));
		$hr = $stmt->fetchAssoc();
		$hr2 = $stmt->fetchAssoc();
		if ($hr2) { die("Actor $other_union_name_canon found twice"); }

		if ($hr) {
			$actor_nid = $hr['entity_id'];
		} else {
			sirius_minilog("Creating a new actor.");

      $actor_node = new stdClass();
      $actor_node->type = "actor";
      $actor_node->language = LANGUAGE_NONE;
      $actor_node->uid = 1;
      node_object_prepare($actor_node);
      $actor_node->title = $other_union_name;
      $actor_node->field_actor_type['und'][0]['tid'] = $lookups['union_tid'];
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

		// Union Status is "Non-Union"= Represented “N” and ignore any data for Contract Start/End Dates
    if ($union_status == 'N') {
    	$relationship_node->field_ur_rep['und'][0]['value'] = '';
    }

		// Union Status is "Non-UNITE HERE Union"= Represented “Y” AND if Contract Start Date and/or End Date have data= add Contract “Y” and attach Start/End dates to this relationship
		if ($union_status == 'NUH') {
    	$relationship_node->field_ur_rep['und'][0]['value'] = 'Y';
    	if ($start_date || $end_date) {
    		$relationship_node->field_ur_contract['und'][0]['value'] = 'Y';
	    	if ($start_date) { $relationship_node->field_ur_contract_start['und'][0]['value'] = $start_date; }
  	  	if ($end_date) { $relationship_node->field_ur_contract_end['und'][0]['value'] = $end_date; }
  	  }
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
