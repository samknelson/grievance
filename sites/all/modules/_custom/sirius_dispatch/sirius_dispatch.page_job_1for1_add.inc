<?php 

function sirius_node_tab_dispatch_job_1for1_add_access($job_node) {
	// 1for1 plugin must be active
	if (!sirius_dispatch_elig_plugin_active($job_node, '1for1')) { return FALSE; }

	// Must have "manage" access to the job
	if (!sirius_node_tab_dispatch_job_manage_access($job_node)) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_dispatch_job_1for1_add_form($form, &$form_state, $job_node) {
  $form_state['job_nid'] = $job_node->nid;

  $form['#tree'] = TRUE;

  $form['worker_id'] = array(
    '#type' => 'textfield',
    '#size' => 8,
    '#title' => 'Badge ID',
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function sirius_node_tab_dispatch_job_1for1_add_form_submit($form, &$form_state) {
  $job_node = node_load($form_state['job_nid']);
  $plugin = sirius_dispatch_elig_instance('1for1');
  $worker_id = $form_state['values']['worker_id'];
  $result = $plugin->for1_add_by_id($job_node, $worker_id);
  if (!$result['success']) {
    drupal_set_message($result['msg'], 'error');
    return;
  }

  drupal_set_message($result['msg']);
}

















function sirius_node_tab_dispatch_job_1for1_add_form_too_complicated($form, &$form_state, $job_node) {
  form_load_include($form_state, 'inc', 'sirius_dispatch', 'sirius_dispatch.page_job_1for1_add');
  form_load_include($form_state, 'inc', 'sirius_event', 'sirius_event.register_worker_byname');
  drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/event.css', 'file');

	$form_state['job_nid'] = $job_node->nid;

	$plugin_key = '1for1';
	sirius_dispatch_elig_plugin_load($plugin_key);
	$replaced = sirius_dispatch_1for1_data($job_node);

  $form['table_open'] = array('#markup' => '<table class="columns_table">');
  $form['col1_open'] = array('#markup' => '<tr><td style="width: 66%; vertical-align: top;">');

  $ajax_params = $form_state['triggering_element']['#sirius_ajax'];

  $form['workers']['prefix'] = array('#markup' => '<div id="workers">');
  $form['workers'][0] = array(
    '#type' => 'textfield',
    '#autocomplete_path' => 'sirius/ajax/bullpen-quicksearch',
    '#ajax' => array(
      'callback' => 'sirius_event_register_worker_byname_form_ajax_preview',
      'wrapper' => 'sirius_worker_preview',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#sirius_ajax' => array(
      'delta' => $i,
    ),
    '#title' => t('Select a Worker'),
    '#description' => t('Enter or scan the worker\'s dispatch ID, SSN, or name.'),
  );
  $form['workers']['suffix'] = array('#markup' => '</div>');

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  $form['col1_close'] = array('#markup' => '</td>');

  $form['col2_open'] = array('#markup' => '<td style="width: 33%; vertical-align: top;">');
  $worker_str = $form_state['input']['workers'][0];
  $form['worker_preview'] = array(
    '#type' => 'markup',
    '#markup' => sirius_event_register_worker_byname_render_preview($worker_str),
  );
  $form['col2_close'] = array('#markup' => '</td></tr></table>');

  $form['#tree'] = TRUE;
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}