<?php

function grievance_contract_template_node_tab_holidays($contract_template_node) {
  drupal_set_title($contract_template_node->title . ': ' . t('Holidays'));

	return drupal_get_form('grievance_holiday_form', $contract_template_node);
}

function grievance_holiday_form($form, &$form_state, $contract_template_node) {
	$form_state['contract_template_node'] = $contract_template_node;

  // Get all the holidays
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_holiday');
  $query->propertyOrderBy('title');
  $stmt = $query->execute();
  $holiday_nodes = array();
  if ($stmt['node']) {
    $holiday_nids = array_keys($stmt['node']);
    $holiday_nodes = node_load_multiple($holiday_nids);
  }

  $options = array();
  foreach ($holiday_nodes as $holiday_nid => $holiday_node) {
  	$options[$holiday_nid] = $holiday_node->title;
  }

	$default_value = unserialize($contract_template_node->field_grievance_holidays['und'][0]['value']);
	if (!$default_value) { $default_value = array(); }

  $form['holidays'] = array(
 		'#type' => 'checkboxes',
  	'#options' => $options,
  	'#title' => t('Select the holidays that are observed under this conrtract.'),
  	'#default_value' => $default_value,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );
	
	return $form;
}

function grievance_holiday_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$contract_template_node = $form_state['contract_template_node'];

	$holiday_nids = array();
	foreach ($form_state['values']['holidays'] as $ignore => $value) {
		if ($value) {
			$holiday_nids[] = $value;
		}
	}

	$contract_template_node->field_grievance_holidays['und'][0]['value'] = serialize($holiday_nids);

	drupal_set_message(t('Holidays for :title have been saved', array(':title' => $contract_template_node->title)));

	node_save($contract_template_node);
}

function grievance_holidays_date($days, $days_type, $contract_nid, $from_ts = NULL) {
	$contract_node = node_load($contract_nid);

	if ($days_type != 'Business Days') {
		return date('Y-m-d', strtotime("+$days days", $from_ts));
	}

	$holiday_nids = unserialize($contract_node->field_grievance_holidays['und'][0]['value']);
	if (!$holiday_nids) { $holiday_nids = array(); }

  // Get all the holidays
  $holiday_nodes = node_load_multiple($holiday_nids);
  $holidays = array();
  foreach ($holiday_nodes as $holiday_node) {
    if ($holiday_node->field_grievance_annual['und'][0]['value'] == 'No') {
      $holidays[date('Y-m-d', strtotime($holiday_node->field_grievance_date['und'][0]['value']))] = TRUE;
    } else {
      for ($year = date('Y', $from_ts)-20; $year < date('Y', $from_ts)+20; ++$year) {
        $holidays[$year . '-' . date('m-d', strtotime($holiday_node->field_grievance_date['und'][0]['value']))] = TRUE;
      }
    }
  }

  // Walk forward skipping non-business days
  $probe = date('Y-m-d', $from_ts);
  $probe_ts = strtotime($probe);
  $count = 0;
  while ($count < $days) {
    $probe = date('Y-m-d', strtotime('+1 day', $probe_ts));
    $probe_ts = strtotime($probe);

    $is_business_day = TRUE;
    if (date('D', $probe_ts) == 'Sat' || date('D', $probe_ts) == 'Sun') {
      $is_business_day = FALSE;
    } else if ($holidays[$probe]) {
      $is_business_day = FALSE;
    }

    if ($is_business_day) {
      ++$count;
    }
  }

  return $probe;
}