<?php

/************************************************************************************************
 * Step 4: Review
 ************************************************************************************************/

function sirius_letter_wizard_review($info, $data, $letter_node, $current_step) { 
	$info = sirius_wizard_info($wizard_name);

	if ($data['subject']) { $html = sirius_fakerender_field("Subject:", $data['subject']); }

	if ($data['recipients']) {
		foreach ($data['recipients'] as $recipient) {
			if ($recipient['disposition'] == 'skip') { continue; }
			if ($data['format'] == 'email') {
				$html .= sirius_fakerender_field($recipient['disposition'] . ':', htmlspecialchars(sirius_email_rfc822_format($recipient['name'], $recipient['email'])));
			} else if ($data['format'] == 'sms') {
				$html .= sirius_fakerender_field($recipient['disposition'] . ':', $recipient['name'] . ' [' . $recipient['phone'] . ']');
			}
		}
	}

	$renderable = field_view_field('node', $letter_node, 'field_sirius_attachments', 'full');
	$renderable['#title'] = 'Attachment';
	$html .= drupal_render($renderable);

	if ($data['upload_others']) {
		$keys = array_filter(array_values($data['upload_others']));
		if ($keys) {
			foreach ($keys as $key) {
				list ($other_material_nid, $delta) = explode('|', $key);
				$other_material_node = node_load($other_material_nid);
				$file = $other_material_node->field_grievance_attachments['und'][$delta];
	  		$name = $other_material_node->title;
	  		$name .= ': '; 
	  		if ($file['origname']) { $name .= $file['origname']; } else { $name .= $file['filename']; }
	  		if ($file['filesize']) { $name .= ' (' . format_size($file['filesize']) . ')'; }

	  		$url = '';
	  		if ($file['uri']) {
	  			$url = file_create_url($file['uri']);
	  		}
				$html .= sirius_fakerender_field("Attachment:", "<a href=\"$url\">$name</a>");
			}
		}
	}

	if (strlen($data['body']) > 20) {
		$html .= '<hr><h2>Body</h2><div style="padding: 20px; margin: 20px; border: 1px solid #cccccc;">';
		$html .= $data['body'];
		$html .= '</div>';
	}

	if (strlen($data['pdf_body']) > 20) {
		$html .= '<hr><h2>PDF Body</h2><div style="padding: 20px; margin: 20px; border: 1px solid #cccccc;">';
		$html .= $data['pdf_body'];
		$html .= '</div>';
	}

	return array('markup' => array('#markup' => $html));
}
