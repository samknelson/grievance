<?php

function sirius_event_term_tab_widgets($event_type_term) {
	$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);

	$html .= sirius_event_term_tab_widgets_quicknav($event_type_json, 'taxonomy/term/' . $event_type_term->tid . '/sirius_event_type_widgets');

	$rule_type = sirius_node_tab_arg(0);
	$delta = sirius_node_tab_arg(1);
	$widget = sirius_node_tab_arg(2);

	if ($rule_type && $delta && $widget) {
		$html .= drupal_render(drupal_get_form('sirius_event_term_tab_widgets_form', $event_type_term, $rule_type, $delta, $widget));
	} else {
		$html .= "Some widgets can have additional configuration options. Please select the widget from the list above.";
	}

	return $html;
}

function sirius_event_term_tab_widgets_form($form, &$form_state, $event_type_term, $rule_type, $delta, $widget) {
	$form_state['event_type_tid'] = $event_type_term->tid;
	$form_state['rule_type'] = $rule_type;
	$form_state['delta'] = $delta;
	$form_state['widget'] = $widget;

	$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);

	$form['#tree'] = TRUE;
	$form['settings']['name'] = array(
		'#type' => 'textfield',
		'#title' => 'Widget Name',
		'#default_value' => $event_type_json['widget_settings'][$rule_type][$delta][$widget]['name'],
	);

	$form['settings']['confirmation_text'] = array(
		'#type' => 'textarea',
		'#title' => 'Confirmation Text',
		'#default_value' => $event_type_json['widget_settings'][$rule_type][$delta][$widget]['confirmation_text'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_term_tab_widgets_form_submit($form, &$form_state) {
	$event_type_term = taxonomy_term_load($form_state['event_type_tid']);
	$rule_type = $form_state['rule_type'];
	$delta = $form_state['delta'];
	$widget = $form_state['widget'];

	$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);
	$event_type_json['widget_settings'][$rule_type][$delta][$widget] = $form_state['values']['settings'];
	sirius_jsonfield_util_term_json_set($event_type_term, $event_type_json);
	taxonomy_term_save($event_type_term);
}

function sirius_event_term_tab_widgets_quicknav($event_type_json, $base_path) {
	$links = array();
	$link = array();
	$link['title'] = 'All Widgets';
	$link['path'] = $base_path;
	$links[] = $link;

	foreach ($event_type_json['register'] as $rules) {
		foreach ($rules as $delta => $rule) {
			foreach ($rule['widget'] as $widget) {
				if ($widget == 'user_self') {
					$link = array();
					$link['title'] = 'Register / ' . $rule['name'] . ' / ' . $widget;
					$link['path'] = $base_path . "/register/$delta/$widget";
					$links[] = $link;
				}
			}
		}
	}

	return sirius_quicknav($links);
}