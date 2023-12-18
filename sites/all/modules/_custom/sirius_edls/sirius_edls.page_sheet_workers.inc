<?php

function sirius_edls_node_tab_sheet_workers_access($sheet_node) {
  return sirius_access_instance('edls')->access('sheet_edit_workers', array('sheet_node' => $sheet_node));
}

function sirius_edls_node_tab_sheet_workers_form($form, &$form_state, $sheet_node) {
	$employer_nid = $sheet_node->field_grievance_shop['und'][0]['target_id'];
	$employer_node = node_load($employer_nid);

	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/form.css');

	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/table.css');
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/table.js', array('type' => 'file'));

	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/ux.js', array('type' => 'file'));

	drupal_add_js(drupal_get_path('module', 'sirius_edls') . '/js/sheet_workers.js', array('type' => 'file'));
	drupal_add_css(drupal_get_path('module', 'sirius_edls') . '/css/sheet_workers.css');

	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/sirius.popup.js');
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/sirius.popup.css');

	$js_settings = array();
	$js_settings['employer_nid'] = $employer_nid;
	$js_settings['sheet_nid'] = $sheet_node->nid;
	$js_settings['date'] = $sheet_node->field_sirius_date_start['und'][0]['value'];

	$classification_options = sirius_edls_worker_classification_options();
	$js_settings['classification_options'] = $classification_options;

	// js_settings for associative arrays don't preserve key order. Ick.
	$ms_list = sirius_edls_ms_list($employer_node, 'name');
	$js_settings['ms_name_lookup'] = $ms_list;
	$js_settings['ms_keys'] = array_values($ms_list);
	$js_settings['ms_name_lookup_short'] = sirius_edls_ms_list($employer_node, 'short');

	drupal_add_js(array('sirius_edls_sheet_workers' => $js_settings), 'setting');

	$form_state['sheet_nid'] = $sheet_node->nid;
	$form['#tree'] = TRUE;

	$crews = sirius_edls_crews($sheet_node);
	if (!$crews) { return sirius_form_error('Please create at least one crew before assigning workers.'); }

	$form['wrapper_open'] = array('#markup' => '<table class="sirius_table sirius_table_align"><tr>');

	$form['col1_open'] = array('#markup' => '<td width="60%" valign="top" style="vertical-align: top;">');
	$html = '<div id="sirius_worker_edls_sheet_stats">';
	$html .= t('Assigned Workers');
	$html .= '<span class="sirius_worker_edls_sheet_stats_inner">';
	$html .= '<span id="sirius_edls_sheet_stats_assigned">0</span> / ';
	$html .= '<span id="sirius_edls_sheet_stats_count">0</span> ';
	$html .= '</span>';
	$html .= '</div>';
	$form['col1_stats'] = array('#markup' => $html);

	foreach ($crews as $crew_delta => $crew) {
		$uuid = $crew['uuid'];
		$form['assigned'][$crew_delta]['open'] = array('#markup' => 
			'<div class="sirius_edls_crew" data-uuid="' . $uuid . '" id="sirius_edls_crew_' . $uuid . '">' .
			'<div class="sirius_edls_crew_head">' . 
			sirius_edls_crew_title($crew) . 
			'<span class="sirius_edls_crew_stats">' . 
			'<span class="sirius_edls_crew_stats_assigned">0</span> / ' .
			'<span class="sirius_edls_crew_stats_count">0</span> ' .
			'</span>' .
			'</div>' .
			'<table class="sirius_table sirius_table_basic sirius_table_form">'
		);

		for ($position = 0; $position < $crew['count']; ++$position) {
			$html = "<tr class=\"sirius_edls_assignment_wrapper\" id=\"crew_$crew[uuid]_position_$position\">";

			$html .= '<td width="10%">';
			$html .= '<span class="sirius_edls_ms"></span>';
			$html .= '</td>';

			$html .= '<td width="90%">';
			$html .= '<span class="sirius_edls_ctrl_unassign sirius_edls_ctrl_unassign_disabled">';
			$html .= '<i class="far fa-times-circle"></i>';
			$html .= '</span>';
			$html .= '<span class="sirius_edls_crew_worker_name"></span>';

			$html .= '<span class="sirius_edls_extra_wrap">';

			$html .= '<span class="sirius_edls_extra"></span>';
			$html .= '<a href="#" class="sirius_popup_trigger sirius_edls_hidden">';
			$html .= '<i class="far fa-edit sirius_popup_trigger"></i>';
			$html .= '</a>';

			$html .= '<div style="display:none" class="sirius_popup_overlay"></div>';
			$html .= '<div style="display:none" class="sirius_popup_wrap">';
			$html .= '<div class="sirius_popup_contents">';

			$html .= '<strong>' . t('Type') . '</strong>' . '<br />';
			$html .= '<select class="sirius_edls_extra_classification">';
			$html .= '<option value="">-- Select --</option>';
			foreach ($classification_options as $key => $value) {
				$html .= "<option value=\"$key\">$value</option>";
			}
			$html .= '</select>';
			$html .= '<br /><br />';

			$html .= '<strong>' . t('Time') . '</strong>' . '<br />';
			$html .= '<input type="time" class="sirius_edls_extra_time">';
			$html .= '<br /><br />';

			$html .= '<strong>' . t('Truck #') . '</strong>' . '<br />';
			$html .= '<input type="text" class="sirius_edls_extra_truck" size="5">';
			$html .= '<br /><br />';

			$html .= '<a href="#" class="sirius_edls_set_extra_submit tool-button">';
			$html .= t('Submit');
			$html .= '</a>';

			$html .= '</div>'; // sirius_popup_contents
			$html .= '<div class="sirius_popup_close"></div>';

			$html .= '</div>'; // sirius_popup_wrap

			$html .= '</span>'; // sirius_edls_extra_wrap

			$html .= '</td>';

			$html .= '</tr>';
			$form['assigned'][$crew_delta]['workers'][$position]['close'] = array('#markup' => $html);
		}
		$form['assigned'][$crew_delta]['close'] = array('#markup' => '</table></div>');
	}

	$html  = '<div class="sirius_edls_notes_wrap">';

	$html .= '<h2>Notes</h2>';
	$html .= '<div id="sirius_edls_notes_inner">';
	// $html .= '[Notes go here]';
	$html .= '</div>'; // sirius_edls_notes_inner
	$html .= '<a href="#" class="sirius_popup_trigger">';
	$html .= '<i class="far fa-edit sirius_popup_trigger"></i>';
	$html .= '</a>';

	$html .= '<div style="display:none" class="sirius_popup_overlay"></div>';
	$html .= '<div style="display:none" class="sirius_popup_wrap">';
	$html .= '<div class="sirius_popup_contents">';

	$html .= '<strong>' . t('Notes') . '</strong>' . '<br />';
	$html .= '<textarea id="sirius_edls_notes_edit" rows="10">';
	$html .= '</textarea>';

	$html .= '<a href="#" class="sirius_edls_set_notes_submit tool-button">';
	$html .= t('Submit');
	$html .= '</a>';

	$html .= '</div>'; // sirius_popup_contents
	$html .= '<div class="sirius_popup_close"></div>';

	$html .= '</div>'; // sirius_popup_wrap

	$html .= '</div>'; // sirius_edls_extra_wrap

	$form['notes'] = array('#markup' => $html);

	$form['col1_close'] = array('#markup' => '</td>');

	$form['col2_open'] = array('#markup' => '<td width="40%" valign="top" style="vertical-align: top;"><div id="sirius_edls_sheet_avail_head">' . t('Available Workers') . '<span class="sirius_edls_worker_stats_inner">0</span></div>');

	$form['available']['filters'] = sirius_worker_form_filters(array('ms_list' => $ms_list));
	$form['available']['workers'] = array('#markup' => sirius_worker_render_list(array('ms_list' => $ms_list)));

	$form['col2_close'] = array('#markup' => '</td>');
	$form['wrapper_close'] = array('#markup' => '</tr></table>');

	return $form;
}