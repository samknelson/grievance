<?php

function sirius_trust_node_tab_worker_benefits_form_access($worker_node) {
  return Sirius_Trust_Access::getInstance()->access('worker_benefits_landing', array('worker_node' => $worker_node));
}

function sirius_trust_node_tab_worker_benefits_form($form, &$form_state, $worker_node) {
	$is_staff = Sirius_Trust_Access::getInstance()->access('staff', array('worker_node' => $worker_node));

	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/table.js', 'file');

  $all_benefit_types = sirius_taxonomy_load('sirius_trust_benefit_type', 'tid', 'name');
  $show_inactive = $form_state['values']['filters']['show_inactive'];
  if (isset($form_state['values']['filters']['benefit_types'])) {
  	$benefit_types = array_filter(array_values($form_state['values']['filters']['benefit_types']));
  } else {
  	$benefit_types = array_filter(array_keys($all_benefit_types));
  }

	$active_benefits_received = sirius_trust_fetch_worker_benefits($worker_node, 'direct', array('active' => 'Yes', 'benefit_types' => $benefit_types));
	$future_benefits_received = sirius_trust_fetch_worker_benefits($worker_node, 'direct', array('benefit_types' => $benefit_types, 'start_date_range' => array('start' => date('Y-m-d', strtotime('+1 day')), 'end' => date('Y-m-d', strtotime('+100 years')))));
	$all_benefits_received = sirius_trust_fetch_worker_benefits($worker_node, 'direct', array('benefit_types' => $benefit_types));
	
	$active_benefits_provided = sirius_trust_fetch_worker_benefits($worker_node, 'dependent', array('active' => 'Yes', 'benefit_types' => $benefit_types));
	$future_benefits_provided = sirius_trust_fetch_worker_benefits($worker_node, 'dependent', array('benefit_types' => $benefit_types, 'start_date_range' => array('start' => date('Y-m-d', strtotime('+1 day')), 'end' => date('Y-m-d', strtotime('+100 years')))));
	$all_benefits_provided = sirius_trust_fetch_worker_benefits($worker_node, 'dependent', array('benefit_types' => $benefit_types));

	$form['#tree'] = TRUE;

	if ($is_staff) {
	  $form['filters'] = array(
	  	'#type' => 'fieldset',
	  	'#title' => 'filters',
	  	'#collapsible' => TRUE,
	  	'#collapsed' => TRUE,
	  );

	  $form['filters']['benefit_types'] = array(
	  	'#type' => 'checkboxes',
	  	'#checkall' => TRUE,
	  	'#title' => t('Benefit Types'),
	  	'#options' => $all_benefit_types,
	  	'#default_value' => array_keys($all_benefit_types),
	  );

	  $form['filters']['show_inactive'] = array(
	  	'#type' => 'checkbox',
	  	'#title' => t('Show Inactive Benefits'),
	  );

	  $form['filters']['submit'] = array(
	  	'#type' => 'submit',
	  	'#value' => t('Submit'),
	  );
	}

	$render_options = array();
	if (!$is_staff) {
		$render_options['hide_notes'] = TRUE;
		$render_options['hide_active'] = TRUE;
		$render_options['hide_name'] = TRUE;
	}

	$html .= '<h2>' . t('Benefits Received') . '</h2>';
	if ($show_inactive) {
		$html .= sirius_trust_render_worker_benefits($worker_node, 'direct', $all_benefits_received, $render_options);
	} else {
		$html .= sirius_trust_render_worker_benefits($worker_node, 'direct', $active_benefits_received+$future_benefits_received, $render_options);
		$count_inactive = count($all_benefits_received) - count($active_benefits_received);

		if ($is_staff) {
			if ($count_inactive > 0) {
				$html .= '<p>' . t('(Not showing <strong>@count_inactive</strong> inactive records.)', array('@count_inactive' => $count_inactive)) . '</p>';
				$show_button = TRUE;
			}
		}
	}

	if ($is_staff) {
		$render_options['hide_header'] = TRUE;
		$html .= '<h2>' . t('Dependent Benefits') . '</h2>';
		if ($show_inactive) {
			$html .= sirius_trust_render_worker_benefits($worker_node, 'direct', $all_benefits_provided, $render_options);
		} else {
			$html .= sirius_trust_render_worker_benefits($worker_node, 'direct', $active_benefits_provided, $render_options);
			$count_inactive = count($all_benefits_provided) - count($active_benefits_provided);
			if ($count_inactive > 0) {
				$html .= '<p>' . t('(Not showing <strong>@count_inactive</strong> inactive records.)', array('@count_inactive' => $count_inactive)) . '</p>';
				$show_button = TRUE;
			}
		}
	}

	$form['results'] = array('#markup' => $html);
	return $form;
}

function sirius_trust_node_tab_worker_benefits_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}
