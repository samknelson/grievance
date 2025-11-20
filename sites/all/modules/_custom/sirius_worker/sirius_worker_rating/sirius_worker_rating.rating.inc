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

	public function rating_get($worker_nid, $rating_type_tid) {
		// @todo: Caching!!!!
		$worker_node = node_load($worker_nid);
		$ratings = $this->ratings_get($worker_node);
		return $ratings[$rating_type_tid];
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
	}

	public function worker_form_validate($form, &$form_state) {
		foreach ($form_state['values']['ratings'] as $tid => $value) {
			if ($value === '' || $value === null) { continue; }
			if (is_numeric($value) && (int)$value == $value && $value >= 0 && $value <= 4) { continue; }
			form_set_error("ratings][$tid", 'Please enter a number between 0 and 4.');
		}
	}

	public function worker_form_submit($form, &$form_state) {
		$worker_node = node_load($form_state['worker_nid']);
		$ratings = $form_state['values']['ratings'];
		$this->ratings_set($worker_node, $ratings);
		node_save($worker_node);
		drupal_set_message("Ratings saved.");
	}

	public function user_default_type_set($type_tid) {
		global $user;
		$user_full = user_load($user->uid);
		$json = sirius_json_get($user_full);
		$json['worker']['rating']['default'] = $type_tid;
		sirius_json_set($user_full, $json);
		user_save($user_full);
	}

	public function user_default_type() {
		global $user;
		$user_full = user_load($user->uid);
		$json = sirius_json_get($user_full);
		return $json['worker']['rating']['default'];
	}

	public function render_stars($rating_val) {
		drupal_add_css(drupal_get_path('module', 'sirius') . '/css/form.css');
		drupal_add_css(drupal_get_path('module', 'sirius_worker_rating') . '/css/rating.css');

		$html = '<span class="sirius_worker_rating_stars">';

		for ($i=0; $i<4; ++$i) {
			if ($i < $rating_val) {
				$html .= '<i class="fas fa-star"></i>';
			} else {
				$html .= '<i class="far fa-star"></i>';
			}
		}
		$html .= '</span>';
		return $html;
	}

	public function cache_sync($worker_node) {
		$sql = "delete from sirius_worker_rating_cache where worker_nid = :worker_nid ";
		$stmt = sirius_sql_query($sql, array(':worker_nid' => $worker_node->nid));

		$ratings = $this->ratings_get($worker_node);
		foreach ($ratings as $rating_type_tid => $value) {
			$sql = "insert into sirius_worker_rating_cache ";
			$sql .= "(worker_nid, rating_type_tid, value) ";
			$sql .= " values (:worker_nid, :rating_type_tid, :value) ";
			$stmt = sirius_sql_query($sql, array(':worker_nid' => $worker_node->nid, ':rating_type_tid' => $rating_type_tid, ':value' => $value));
		}
		return array('success' => TRUE);
	}

}
