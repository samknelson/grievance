<?php

function mightysearch_mschema() {

	$schema = array();
	
	//
	// Property Table
	//
	
	$schema['property'] = array();
	$schema['property']['table'] = array(
		'prompt' => 'Property',
		'bundle' => 'property',
	);
	
	//
	// Property Fields
	//
	
	$schema['property']['fields'] = array();
	$schema['property']['fields']['nid'] = array(
		'prompt' => 'ID',
		'type' => 'text',
	);
	$schema['property']['fields']['title'] = array(
		'prompt' => 'Name',
		'type' => 'text',
		'highlighted' => 1,
	);
	$schema['property']['fields']['rooms'] = array(
		'prompt' => 'Rooms',
		'type' => 'integer',
	);
	$schema['property']['fields']['union_status'] = array(
		'prompt' => 'Union Status',
		'type' => 'text',
	);
	$schema['property']['fields']['development_status'] = array(
		'prompt' => 'Development Status',
		'type' => 'text',
	);
	$schema['property']['fields']['address'] = array(
		'prompt' => 'Address',
		'type' => 'text',
		'col' => array(
			'thoroughfare' => 'Street',
			'premise' => 'Premise',
			'locality' => 'City',
			'administrative_area' => 'State',
			'postal_code' => 'Zip',
		),
	);
	
	$schema['property']['fields']['airport'] = array(
		'prompt' => 'Airport',
		'type' => 'tid',
		'vocabulary' => 'airport'
	);
	
	$schema['property']['fields']['msa'] = array(
		'prompt' => 'MSA',
		'type' => 'tid',
		'vocabulary' => 'msa'
	);

	$schema['property']['fields']['owner_of_record'] = array(
		'prompt' => 'Owner of Record',
		'type' => 'text',
	);

	$schema['property']['fields']['pin'] = array(
		'prompt' => 'PIN',
		'type' => 'text',
	);

	$schema['property']['fields']['labor_activity_index'] = array(
		'prompt' => 'Labor Activity Index',
		'type' => 'integer',
	);

	$schema['property']['fields']['phone'] = array(
		'prompt' => 'Phone',
		'type' => 'text',
	);

	$schema['property']['fields']['url'] = array(
		'prompt' => 'URL',
		'type' => 'url',
		'col' => array(
			'URL' => 'url',
		),
	);

	$schema['property']['fields']['hotel_contract_start_date'] = array(
		'prompt' => 'Hotel Contract Start Date',
		'type' => 'date',
	);

	$schema['property']['fields']['hotel_contract_end_date'] = array(
		'prompt' => 'Hotel Contract End Date',
		'type' => 'date',
	);

	$schema['property']['fields']['hotel_contract_covered'] = array(
		'prompt' => 'Hotel Contract Covered Workers',
		'type' => 'integer',
	);

	$schema['property']['fields']['rest_contract_start_date'] = array(
		'prompt' => 'Restaurant Contract Start Date',
		'type' => 'date',
	);

	$schema['property']['fields']['rest_contract_end_date'] = array(
		'prompt' => 'Restaurant Contract End Date',
		'type' => 'date',
	);

	$schema['property']['fields']['rest_contract_covered'] = array(
		'prompt' => 'Restaurant Contract Covered Workers',
		'type' => 'integer',
	);

	$schema['property']['fields']['is_casino'] = array(
		'prompt' => 'Casino?',
		'type' => 'text',
	);

	$schema['property']['fields']['csno_slots_count'] = array(
		'prompt' => 'Casino - Slots',
		'type' => 'text',
	);

	$schema['property']['fields']['csno_slots_date'] = array(
		'prompt' => 'Casino - Slots As Of',
		'type' => 'date',
	);

	$schema['property']['fields']['csno_tables_count'] = array(
		'prompt' => 'Casino - Tables',
		'type' => 'text',
	);

	$schema['property']['fields']['csno_tables_date'] = array(
		'prompt' => 'Casino - Tables As Of',
		'type' => 'date',
	);

	$schema['property']['fields']['csno_regulator'] = array(
		'prompt' => 'Casino - Regulator',
		'type' => 'text',
	);

	$schema['property']['fields']['csno_book'] = array(
		'prompt' => 'Casino - Sports Book?',
		'type' => 'text',
	);

	$schema['property']['fields']['csno_horse'] = array(
		'prompt' => 'Casino - Horse Racing?',
		'type' => 'text',
	);

	$schema['property']['fields']['csno_online'] = array(
		'prompt' => 'Casino - Online Betting?',
		'type' => 'text',
	);

	$schema['property']['fields']['csno_source'] = array(
		'prompt' => 'Casino - Source',
		'type' => 'text',
	);

	$relationship_types = array(
		'owner' => 'Owner',
		'operator' => 'Operator',
		'actor_a' => 'Actor A',
		'actor_b' => 'Actor B',
		'actor_c' => 'Actor C',
		'actor_d' => 'Actor D',
		'actor_e' => 'Actor E',
	);
	
	foreach ($relationship_types as $relationship_type => $relationship_prompt) {
		//
		// Link Table
		//
		
		$schema[$relationship_type . '_pa'] = array();
		$schema[$relationship_type . '_pa']['table'] = array(
			'prompt' => $relationship_prompt . ' Relationship',
		);
		
		//
		// Link Fields
		//
		
		$schema[$relationship_type . '_pa']['fields'] = array();
		$schema[$relationship_type . '_pa']['fields']['pension'] = array(
			'prompt' => 'Pension',
			'type' => 'text',
		);
		$schema[$relationship_type . '_pa']['fields']['percent_owned'] = array(
			'prompt' => 'Percent Owned',
			'type' => 'text',
		);
		$schema[$relationship_type . '_pa']['fields']['amount_owned'] = array(
			'prompt' => 'Amount Owned',
			'type' => 'text',
		);
		$schema[$relationship_type . '_pa']['fields']['owned_raw'] = array(
			'prompt' => 'Owned (Raw)',
			'type' => 'text',
		);
		$schema[$relationship_type . '_pa']['fields']['start_date'] = array(
			'prompt' => 'Start Date',
			'type' => 'date',
		);
		$schema[$relationship_type . '_pa']['fields']['end_date'] = array(
			'prompt' => 'End Date',
			'type' => 'date',
		);
		$schema[$relationship_type . '_pa']['fields']['sales_price'] = array(
			'prompt' => 'Sales Price',
			'type' => 'text',
		);
		$schema[$relationship_type . '_pa']['fields']['notes'] = array(
			'prompt' => 'Notes',
			'type' => 'text',
		);
		$schema[$relationship_type . '_pa']['fields']['relationship_type'] = array(
			'prompt' => 'Relationship Type',
			'type' => 'text',
		);
	
		//
		// Actor Table
		//
		
		$schema[$relationship_type] = array();
		$schema[$relationship_type]['table'] = array(
			'prompt' => $relationship_prompt,
			'bundle' => 'actor',
		);
		
		//
		// Actor Fields
		//
		
		$schema[$relationship_type]['fields'] = array();
		$schema[$relationship_type]['fields']['nid'] = array(
			'prompt' => 'ID',
			'type' => 'text',
		);
		$schema[$relationship_type]['fields']['title'] = array(
			'prompt' => 'Name',
			'type' => 'text',
		);
		$schema[$relationship_type]['fields']['phone'] = array(
			'prompt' => 'Phone',
			'type' => 'text',
		);
		$schema[$relationship_type]['fields']['actor_type'] = array(
			'prompt' => 'Actor Type',
			'type' => 'tid',
			'vocabulary' => 'actor_type'
		);
		$schema[$relationship_type]['fields']['address'] = array(
			'prompt' => 'Address',
			'type' => 'text',
			'col' => array(
				'thoroughfare' => 'Street',
				'premise' => 'Premise',
				'locality' => 'City',
				'administrative_area' => 'State',
				'postal_code' => 'Zip',
			),
		);
	}
	
	$schema['owner']['fields']['title']['highlighted'] = 1;
	$schema['operator']['fields']['title']['highlighted'] = 1;
	
	//
	// Any Actor
	//
	
	$schema['any_actor'] = array();
	$schema['any_actor']['table'] = array(
		'prompt' => 'Any Actor'
	);
	$schema['any_actor']['fields']['title'] = array(
		'prompt' => 'Name',
		'type' => 'text',
	);

	//
	// Local Table
	//
	
	$schema['local'] = array();
	$schema['local']['table'] = array(
		'prompt' => 'Local'
	);

	//
	// Local Fields
	//
	
	$schema['local']['fields'] = array();
	$schema['local']['fields']['nid'] = array(
		'prompt' => 'ID',
		'type' => 'text',
	);
	$schema['local']['fields']['title'] = array(
		'prompt' => 'Name',
		'type' => 'text',
	);
	$schema['local']['fields']['union_name'] = array(
		'prompt' => 'Union Name',
		'type' => 'integer',
	);
	$schema['local']['fields']['contact'] = array(
		'prompt' => 'Contact',
		'type' => 'text',
	);

	//
	// Brand fields
	//

	/*
	$schema['property']['fields']['brand'] = array(
		'prompt' => 'Brand',
		'type' => 'tid',
		'vocabulary' => 'brand',
	);
	*/

	$schema['brand']['table'] = array(
		'prompt' => 'Brand',
		'table_type' => 'term',
	);

	$schema['brand']['fields']['name'] = array(
		'prompt' => 'Name',
		'type' => 'text',
		'col' => '',
	);

	$schema['brand']['fields']['service_level'] = array(
		'prompt' => 'Service Level',
		'type' => 'text',
	);


	return $schema;
}

