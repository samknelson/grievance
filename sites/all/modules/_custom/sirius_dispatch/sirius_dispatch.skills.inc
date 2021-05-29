<?php

function sirius_dispatch_skill_options() {
	$skill_options = &drupal_static(__FUNCTION__);
  if (!isset($skill_options)) {
  	$skill_options = array();
		$skill_terms = sirius_taxonomy_load('sirius_skill', 'tid', 'full');
		foreach ($skill_terms as $tid => $term) {
			$json = sirius_json_get($term);
			if ($json['dispatch']['job']['available'] == 'No') { continue; }
			$skill_options[$tid] = $term->name;
		}
	}

	return $skill_options;
}