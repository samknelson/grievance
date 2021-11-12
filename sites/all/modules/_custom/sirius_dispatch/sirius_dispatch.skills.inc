<?php

function sirius_dispatch_skill_options($industry_tid = NULL) {
	$skill_options = &drupal_static(__FUNCTION__);
  if (!isset($skill_options)) {
  	$skill_options = array();
		$skill_terms = sirius_taxonomy_load('sirius_skill', 'tid', 'full');
		foreach ($skill_terms as $tid => $term) {
			$industry_tids = sirius_fieldvals($term, 'field_sirius_industry', 'tid');
			if ($industry_tids && !in_array($industry_tid, $industry_tids)) { continue; }

			$json = sirius_json_get($term);
			if ($json['dispatch']['job']['available'] == 'No') { continue; }

			$skill_options[$tid] = $term->name;
		}
	}

	return $skill_options;
}