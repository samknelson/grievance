<?php

/**
 * Wrapper for forms, because we can't call drupal_get_form() on a class method
 */

function sirius_worker_rating_worker_form($form, &$form_state, $worker_node) {
	return Sirius_Worker_Rating::getInstance()->worker_form($form, $form_state, $worker_node);
}

function sirius_worker_rating_worker_form_validate($form, &$form_state) {
	return Sirius_Worker_Rating::getInstance()->worker_form_validate($form, $form_state);
}

function sirius_worker_rating_worker_form_submit($form, &$form_state) {
	return Sirius_Worker_Rating::getInstance()->worker_form_submit($form, $form_state);
}

function sirius_worker_rating_form($form, &$form_state, $worker_node) {
	return Sirius_Worker_Rating::getInstance()->form($form, $form_state, $worker_node);
}

function sirius_worker_rating_form_validate($form, &$form_state) {
	return Sirius_Worker_Rating::getInstance()->form_validate($form, $form_state);
}

function sirius_worker_rating_form_submit($form, &$form_state) {
	return Sirius_Worker_Rating::getInstance()->form_submit($form, $form_state);
}

function sirius_worker_rating_cron_reminders($mode = 'test', $force = FALSE) {
	return Sirius_Worker_Rating::getInstance()->cron_reminders($mode, $force);
}

class Sirius_Worker_Rating extends Sirius_Singleton {
	public $type = 'rating';
	public $name = 'Rating';
	public $description = '';

	public function status_options() {
		return array(
		);
	}

	public function status_label($status) {
		$label = $this->status_options()[$status];
		if (!$label) { $label = $status; }
		return $label;
	}

	public function types($options) {
		if ($options['indent']) {
			return sirius_taxonomy_options('sirius_worker_rating_type');
		} else {
			return sirius_taxonomy_load('sirius_worker_rating_type', 'tid', 'name');
		}
	}

	public function ratings_get($worker_node) {
		$json = sirius_json_get($worker_node);
		$ratings = $json['worker_ratings']['ratings'];
		if (!$ratings) { $ratings = array(); }
		return $ratings;
	}

	public function ratings_set($worker_node, $ratings) {
		$json = sirius_json_get($worker_node);
		$json['worker_ratings']['ratings'] = $ratings;
		sirius_json_set($worker_node, $json);
	}

	public function worker_form($form, &$form_state, $worker_node, $options = array()) {
		if (!$worker_node) { return sirius_form_error("Missing worker node."); }

		drupal_add_css(drupal_get_path('module', 'sirius') . '/css/form.css');
		drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');

		$form['#tree'] = TRUE;
		$form_state['worker_nid'] = $worker_node->nid;

		$types = $this->types(array('indent' => TRUE));
		if (!$types) { return sirius_form_error('Please create at least one rating type.'); }

		$ratings = $this->ratings_get($worker_node);

		foreach ($types as $tid => $type) {
			$form['ratings'][$tid] = array(
				'#type' => 'textfield',
				'#size' => 2,
				'#title' => $type,
				'#prefix' => '<div class="sirius_form_inline sirius_form_inline_longlabel">',
				'#suffix' => '</div>',
				'#default_value' => $ratings[$tid],
			);
		}

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);


		return $form;

		$form['intro'] = array('#markup' => sirius_domain_variable_get('sirius_worker_rating_intro'));

		$rating_node = $this->worker_active_record($worker_node->nid);

		if ($rating_node) {
			$status = $this->status($rating_node);
			$form['action'] = array('#type' => 'value', '#value' => 'cancel');

			$html = '<p>' . '<strong><span class="sirius_text_red">' . t('You are currently marked as "Absent".') . '</span></strong></p>';

			$class = 'sirius_callout_basic ';
			if ($status == 'active') { $class .= 'sirius_callout_red '; }
			$class .= "sirius_worker_rating sirius_worker_rating_$status";

			$html .= "<div class=\"$class\">";
			$html .= sirius_fakerender_field('Record ID:', $rating_node->uuid);
			$html .= sirius_fakerender_field('Status:', $this->status_label($status));
			$html .= sirius_fakerender_field('Absent at:', date('l, F j, Y g:i A T', $rating_node->created));
			$rating_json = sirius_json_get($rating_node);
			$ts = $rating_json['worker_rating']['cancel']['ts'];
			if ($ts) {
				$html .= sirius_fakerender_field('Returned at:', date('l, F j, Y g:i A T', $ts));
			}
			$html .= '<p>' . sirius_text_to_html($rating_node->field_sirius_summary['und'][0]['value']) . '</p>';
			$html .= '</div>';

			$form['cancel']['intro'] = array('#markup' => $html);

			$form['cancel']['confirm'] = array(
				'#type' => 'checkbox',
				'#title' => t('Remove me from the "Absent" list.'),
				'#required' => TRUE,
			);

			$form['cancel']['submit'] = array(
				'#type' => 'submit',
				'#value' => t('Remove me from the "Absent" list'),
			);

		} else {
			$form['action'] = array('#type' => 'value', '#value' => 'activate');

			$html = '<p>' . '<strong><span class="sirius_text_green">' . t('You are not on the "Absent" list.') . '</span></strong></p>';
			$form['activate']['intro'] = array('#markup' => $html);

			$form['activate']['comments'] = array(
				'#type' => 'textarea',
				'#title' => t('Comments'),
				'#required' => TRUE,
			);

			$form['activate']['confirm'] = array(
				'#type' => 'checkbox',
				'#title' => t('Place me on the "Absent" list.'),
				'#required' => TRUE,
			);

			$form['activate']['submit'] = array(
				'#type' => 'submit',
				'#value' => t('Mark me absent now'),
			);
		}

		return $form;
	}

	public function worker_form_validate($form, &$form_state) {
		foreach ($form_state['values']['ratings'] as $tid => $value) {
			if ($value === '' || $value === null) { continue; }
			if (is_numeric($value) && (int)$value == $value && $value >= 0 && $value <= 10) { continue; }
			form_set_error("ratings][$tid", 'Please enter a number between 0 and 10.');
		}
	}

	public function worker_form_submit($form, &$form_state) {
		$worker_node = node_load($form_state['worker_nid']);
		$ratings = $form_state['values']['ratings'];
		$this->ratings_set($worker_node, $ratings);
		node_save($worker_node);
		drupal_set_message("Ratings saved.");
	}
}
