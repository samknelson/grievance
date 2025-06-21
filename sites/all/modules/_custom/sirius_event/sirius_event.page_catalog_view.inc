<?php 

function sirius_event_node_tab_catalog_view_access($catalog_node) {
	// Only log entries of the correct type
	if ($catalog_node->field_sirius_type['und'][0]['value'] != 'sirius_event:catalog') { return FALSE; }

	// Admins can access
	// if (user_access('sirius event admin')) { return TRUE; }

	// Public access?
	$catalog_json = sirius_json_get($catalog_node);
	if ($catalog_json['settings']['access']['public'] == 'Yes') { return TRUE; }

	// Do we have one of the required roles?
	global $user;
	$user_full = user_load($user->uid);
	$catalog_roles = $catalog_json['settings']['access']['roles'];
	$my_roles = $user_full->roles;
	if ($catalog_roles && $my_roles) { 
		if (array_intersect(array_filter(array_values($catalog_roles)), array_filter(array_keys($my_roles)))) {
			return TRUE;
		}
	}

	// Oh well.
	return FALSE;
}

function sirius_event_node_tab_catalog_view($catalog_node) {
	return drupal_get_form('sirius_event_node_tab_catalog_view_form', $catalog_node);
}

function sirius_event_node_tab_catalog_view_form($form, &$form_state, $catalog_node) {
	$form_state['catalog_nid'] = $catalog_node->nid;
	$form['#tree'] = TRUE;

	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/catalog.css', 'file');
  drupal_add_library('system', 'ui.accordion');
  drupal_add_js('jQuery(document).ready(function () { jQuery("#sirius_accordion").accordion({active: false, collapsible: true, autoehight: true, heightStyle: "content"}); });', 'inline');

	$catalog_json = sirius_json_get($catalog_node);

	if ($catalog_json['settings']['markup']['intro']) {
		$form['intro'] = array('#markup' => '<div class="sirius_event_catalog_intro">' . $catalog_json['settings']['markup']['intro'] . '</div>');
	}

	// Fetch all the custom field sets that we might need to reference
	$keys = array('inline_custom_fields', 'body_custom_fields', 'filter_custom_fields');
	$cfs_nodes = $cfs_json = $cf_forms = array();
	$cf_paths = array();
	foreach ($keys as $key) {
		if (!$catalog_json['settings']['course_display'][$key]) { continue; }
		$full_paths = array_filter(array_values($catalog_json['settings']['course_display'][$key]));
		foreach ($full_paths as $full_path) {
			list ($cfs_nid, $ignore) = explode(':', $full_path, 2);
			$path = preg_replace("/^$cfs_nid/", '', $full_path);
			$cf_paths[$key][$cfs_nid][] = $path;
			if (!$cfs_nodes[$cfs_nid]) {
				$cfs_nodes[$cfs_nid] = node_load($cfs_nid);
				$cfs_json[$cfs_nid] = sirius_json_get($cfs_nodes[$cfs_nid]);
				$cfs_forms[$cfs_nid] = sirius_jsonfield_util_form_get($cfs_json[$cfs_nid], 'default');
			}
		}
	}

	if ($catalog_json['settings']['filters']['show_filters'] == 'Yes') {
		$title = 'Filters';
		$form['filters'] = array(
			'#type' => 'fieldset',
			'#title' => 'Filters',
			'#collapsible' => TRUE,
			'#collapsed' => TRUE,
		);

		$form['filters']['title'] = array(
			'#title' => t('Course Name'),
			'#type' => 'textfield',
			'#description' => t('Enter all or part of the course name.'),
		);

		if ($cf_paths['filter_custom_fields']) {
			foreach ($cf_paths['filter_custom_fields'] as $cfs_nid => $paths) {
				foreach ($paths as $path) {
					$form_element = sirius_jsonfield_util_get_from_path($cfs_forms[$cfs_nid], $path);

					// Rewrite our form element so it looks like a filter.
					if ($form_element['#type'] == 'checkboxes') {
						$form_element['#type'] = 'select';
						$form_element['#options'] = array(NULL => t('-- Select --')) + $form_element['#options'];
					}

					$form['filters']['custom'][$cfs_nid][$path] = $form_element;
				}
			}
		}

		$form['filters']['submit'] = array(
			'#value' => t('Filter'),
			'#type' => 'submit',
		);
	}


	if ($catalog_json['settings']['courses']['event_types']) {
		$my_contact_nid = sirius_contact_nid_get_from_user();

		$all_event_types = sirius_taxonomy_load('sirius_event_type', 'tid', 'full');
		$event_type_tids  = array_filter(array_values($catalog_json['settings']['courses']['event_types']));

		$html = '<div id="sirius_accordion">';
		$no_courses_found = TRUE;
		foreach ($event_type_tids as $event_type_tid) {
			$event_type_term = $all_event_types[$event_type_tid];
			$event_type_json = sirius_json_get($event_type_term);

			$filters = $form_state['input']['filters'];

			if (trim($filters['title'])) {
				if (stripos($event_type_term->name, trim($filters['title'])) === FALSE) { continue; }
			} 

			if ($filters['custom']) {
				$match = TRUE;
				foreach ($filters['custom'] as $cfs_nid => $paths) {
					$cfs_results_json = sirius_jsonfield_util_results_get($event_type_json, 'customfield_' . $cfs_nid);
					foreach ($paths as $path => $filter_value) {
						if (!$filter_value) { continue; }
						$cf_value = sirius_jsonfield_util_get_from_path($cfs_results_json, $path);
						if (is_array($cf_value) && !in_array($filter_value, array_filter(array_values($cf_value)))) { $match = FALSE; break; }
						if (!is_array($cf_value) && (strtoupper($filter_value) != strtoupper($cf_value))) { $match = FALSE; break; }
					}
				}
				if (!$match) { continue; }
			}
			$no_courses_found = FALSE;

			$accordion_title = $event_type_term->name;
			$accordion_body = $event_type_term->description;

			$accordion_title_extras = array();
			if ($cf_paths['inline_custom_fields']) {
				foreach ($cf_paths['inline_custom_fields'] as $cfs_nid => $paths) {
					$cfs_results_json = sirius_jsonfield_util_results_get($event_type_json, 'customfield_' . $cfs_nid);
					foreach ($paths as $path) {
						$cf_value = sirius_jsonfield_util_get_from_path($cfs_results_json, $path);
						$form_element = sirius_jsonfield_util_get_from_path($cfs_forms[$cfs_nid], $path);
						$render = sirius_jsonfield_util_render_value($form_element, $cf_value, array('raw' => TRUE, 'separator' => ', '));
						if ($render) { $accordion_title_extras[] = $render; }
					}
				}
			}
			if ($accordion_title_extras) {
				$accordion_title .= '<span class="sirius_accordion_label_extra">' . join(' / ', $accordion_title_extras) . '</span>';
			}

			if ($cf_paths['body_custom_fields']) {
				foreach ($cf_paths['body_custom_fields'] as $cfs_nid => $paths) {
					$cfs_results_json = sirius_jsonfield_util_results_get($event_type_json, 'customfield_' . $cfs_nid);
					foreach ($paths as $path) {
						$cf_value = sirius_jsonfield_util_get_from_path($cfs_results_json, $path);
						$form_element = sirius_jsonfield_util_get_from_path($cfs_forms[$cfs_nid], $path);
						$accordion_body .= sirius_jsonfield_util_render_value($form_element, $cf_value, array());
					}
				}
			}


			$event_nids = sirius_event_upcoming_by_type($event_type_tid);
			$event_nodes = node_load_multiple($event_nids);

			$event_nodes = array_filter($event_nodes, 
				function($event_node, $event_nid) {
    			return sirius_event_listed($event_node);
				}, 
				ARRAY_FILTER_USE_BOTH
			);

			if ($event_nodes) {
				$accordion_body .= '<div class="sirius_event_catalog_upcoming">';
				$accordion_body .= '<strong>Upcoming Events:</strong><br/>';
				foreach ($event_nodes as $event_nid => $event_node) {
					$accordion_body .= sirius_event_render_teaser($event_node);
				}
				$accordion_body .= '</div>';
			} else {
				$accordion_body .= '<div class="sirius_event_catalog_upcoming sirius_event_catalog_upcoming_empty">';
				$accordion_body .= 'There are no upcoming events of this type.';
				$accordion_body .= '</div>';
			}

			$interest_enabled = sirius_event_interestlist_enabled_byterm($event_type_term);
			$interestlist_path = "taxonomy/term/$event_type_tid/sirius_event_type_interestlist";
			if ($interest_enabled && $my_contact_nid && drupal_valid_path($interestlist_path)) {
				$on_interestlist = sirius_event_interestlist_lookup($event_type_term, $my_contact_nid);
				$accordion_body .= '<div class="sirius_event_catalog_intereslist_wrapper">';
				$accordion_body .= '<strong>Interest List:</strong><br/>';
				if ($on_interestlist) {
					$accordion_body .= t('You are on the interest list.');
					$accordion_body .= l('Remove me', $interestlist_path, array('attributes' => array('class' => 'tool-button'), 'query' => array('destination' => current_path())));
				} else {
					$accordion_body .= t('An interest list is available.');
					$accordion_body .= l('Add me', $interestlist_path, array('attributes' => array('class' => 'tool-button'), 'query' => array('destination' => current_path())));
				}
				$accordion_body .= '</div>';
			}


			$html .= '<div class="sirius_accordion_label">';
			$html .= $accordion_title;
			$html .= '</div>';
			$html .= '<div class="sirius_accordion_body">';
			$html .= $accordion_body;
			$html .= '</div>';
		}
		$html .= '</div>';
		$form['body'] = array('#markup' => $html);
	}

	if ($no_courses_found) {
		$form['body'] = array('#markup' => '<div class="sirius_event_catalog_no_courses_found">' . t('There are no matching entries') . '</div>');
	}

	if ($catalog_json['settings']['markup']['outro']) {
		$form['outro'] = array('#markup' => '<div class="sirius_event_catalog_outro">' . $catalog_json['settings']['markup']['outro'] . '</div>');
	}

	return $form;
}

function sirius_event_node_tab_catalog_view_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}