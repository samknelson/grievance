<?php

/************************************************************************************************
 * Step 2: Body
 ************************************************************************************************/

function sirius_letter_wizard_body($info, $data, $letter_node, $current_step) { return drupal_get_form('sirius_letter_wizard_body_form', $info, $data, $letter_node, $current_step); }

function sirius_letter_wizard_body_form($form, &$form_state, $info, $data, $letter_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['letter_nid'] = $letter_node->nid;
	$form_state['current_step'] = $current_step;

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);
	$steps = sirius_wizard_steps($info, $data, $entity_node);

	if ($data['format'] == 'sms') {
		// Add a link shortener
		// No longer recommended, carriers block messages with shortened links.
		/*
	  drupal_add_js(drupal_get_path('module', 'sirius') . '/js/sirius.popup.js');
	  drupal_add_css(drupal_get_path('module', 'sirius') . '/css/sirius.popup.css');
	  $html .= '<br /><a href="#" class="sirius_popup_trigger tool-button">' . t('Link Shortener') . '</a>';
	  $html .= '<div style="display:none" class="sirius_popup_overlay"></div>';
	  $html .= '<div style="display:none" class="sirius_popup_wrap">';
	  $html .= '<div class="sirius_popup_contents">';
	  $shorten_block = block_load('shorten', 'shorten');
		$shorten_build = module_invoke('shorten', 'block_view', 'shorten');
		drupal_alter(array('block_view', "block_view_shorten_shorten"), $shorten_build, $shorten_block);
		$html .= $shorten_build['content'];
	  $html .= '</div>';
	  $html .= '<div class="sirius_popup_close"></div>';
	  $html .= '</div>';
	  $form['#prefix'] = $html;
	  */
	}

	if ($data['format'] == 'email') {
		$form['subject'] = array(
			'#type' => 'textfield',
			'#title' => t('Subject'),
			'#default_value' => $data['subject'],
		);
	}

	if (in_array($data['format'], array('email', 'postal'))) {
		$form['body'] = array(
			'#type' => 'text_format',
			'#title' => t('Body'),
			'#default_value' => $data['body'],
			'#rows' => 20,
			'#format' => 'sirius_form_editor',
			'#allowed_formats' => array('full_html'),
		);
	}

	if ($data['format'] == 'sms') {
		$form['body'] = array(
			'#type' => 'textarea',
			'#title' => 'Body',
			'#default_value' => $data['body'],
			'#rows' => 10,
			'#id' => 'sms_body',
			'#description' => sirius_sms_segment_count('sms_body'),
		);
	}

	if (in_array($data['format'], array('email', 'pdf'))) {
		$form['pdf_body'] = array(
			'#type' => 'text_format',
			'#title' => 'PDF Body',
			'#default_value' => $data['pdf_body'],
			'#rows' => 20,
			'#format' => 'sirius_form_editor',
			'#allowed_formats' => array('full_html'),
		);

		if (user_access('sirius letterhead use') || user_access('grievance letter variable letterhead')) {
			$options = array(NULL => t('No Letterhead'));
			$letterheads = sirius_letterhead_list();
			foreach ($letterheads as $nid => $title) {
				$options[$nid] = $title;
			}
		  $form['pdf_letterhead'] = array(
		    '#type' => 'select',
		    '#options' => $options,
		    '#title' => t('Letterhead'),
				'#default_value' => $data['pdf_letterhead'],
				'#format' => 'sirius_form_editor',
				'#allowed_formats' => array('full_html'),
		  );
		}
	}

	if ($data['format'] == 'email') {

		$other_material_nids = array();

		$query = new EntityFieldQuery();
	  $query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'grievance_attachment');
	  $query->fieldCondition('field_grievance', 'target_id', $entity_node->nid);
	  $query->propertyOrderBy('nid');
	  $stmt = $query->execute();
	  if ($stmt['node']) { $other_material_nids += array_keys($stmt['node']); }

	  $other_material_nodes = node_load_multiple($other_material_nids);
	  $other_material_options = array();
	  foreach ($other_material_nodes as $other_material_nid => $other_material_node) {
	  	if (!$other_material_node->field_grievance_attachments['und'][0]['fid']) { continue; }
	  	foreach ($other_material_node->field_grievance_attachments['und'] as $delta => $file) { 
	  		$name = $other_material_node->title;
	  		$name .= ': '; 
	  		if ($file['origname']) { $name .= $file['origname']; } else { $name .= $file['filename']; }
	  		if ($file['filesize']) { $name .= ' (' . format_size($file['filesize']) . ')'; }
		  	$other_material_options["$other_material_nid|$delta"] = $name;
		  }
	  }
	  if ($other_material_options) {
	  	$form['upload_others'] = array(
	  		'#title' => t('Attach these materials'),
	  		'#type' => 'checkboxes',
	  		'#options' => $other_material_options,
	  	);
	  	if ($data['upload_others']) { $form['upload_others']['#default_value'] = $data['upload_others']; }
	  }

		$renderable = field_view_field('node', $letter_node, 'field_sirius_attachments', 'full');
		$renderable['#title'] = 'Currently Uploads';
		$form['uploads_current'] = array('#markup' => drupal_render($renderable));

		for ($i=0; $i<3; ++$i) {
			$form["upload_$i"] = array(
				'#type' => 'file',
				'#title' => t('Upload #@i', array('@i' => ($i+1))),
			);
		}
	}

	if ($data['format'] == 'postal') {
		if (!sirius_domain_component_enabled('postal')) { return sirius_form_error('Postal module is not enabled.'); }
		$instance = sirius_postal_client_instance();
		if (!$instance) { return sirius_form_error('No postal client has been configured.'); }
		$form['postal_options'] = array(
			'#type' => 'fieldset',
			'#title' => t('Postal Options'),
			'#tree' => TRUE,
		);
		$form['postal_options'] += $instance->mail_options_form();
		sirius_jsonfield_util_set_defaults($form['postal_options'], $data['postal_options']);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_letter_wizard_body_form_validate($form, &$form_state) {
	$info = field_info_instance('node', 'field_sirius_attachments', 'sirius_log');
	$extensions = $info['settings']['file_extensions'];
	for ($delta = 0; $delta < 3; ++$delta) {
	  $file = file_save_upload("upload_$delta", array(
	    'file_validate_extensions' => array($extensions),
	  ));
	  if ($file) {
	    $name = rand();
	    if ($file = file_move($file, 'private://attach_' . rand())) {
	      $form_state['storage']["attach_$delta"] = $file;
	    }
	    else {
	      form_set_error("attach_$delta", t("Failed to write the uploaded file to the site's file folder."));
	    }
	  }
	}
}

function sirius_letter_wizard_body_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$letter_node = node_load($form_state['letter_nid']);
	$current_step = $form_state['current_step'];

	$files = array();
	for ($delta = 0; $delta < 3; ++$delta) {
	  $file = $form_state['storage']["attach_$delta"];
	  if ($file) {
  		$file = (array) $file;
  		$file['display'] = 1;
			unset($form_state['storage']["attach_$delta"]);
  		unset($file['destination']);
  		unset($file['uri']);
  		$files[] = $file;
  	}
  }
  if ($files) { $letter_node->field_sirius_attachments['und'] = $files; }

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);

	$data['subject'] = $form_state['values']['subject'];
	$data['body'] = sirius_richtext_handle_input($form_state['values']['body']);
  $data['pdf_body'] = $form_state['values']['pdf_body']['value'];
  $data['pdf_letterhead'] = $form_state['values']['pdf_letterhead'];
  $data['upload_others'] = $form_state['values']['upload_others'];
  $data['postal_options'] = $form_state['values']['postal_options'];

	sirius_letter_set_title($letter_node);
	sirius_wizard_submit($info, $data, $letter_node, $current_step, "Letter body saved.");
}
