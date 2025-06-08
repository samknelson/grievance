<?php

class Sirius_Trust_Elgibility_Plugin_Reltype extends Sirius_Trust_Elgibility_Plugin {
	public $type = 'reltype';
	public $name = 'Relationship Type';
	public $description = 'Eligibility for this benefit is restricted to only certain relationship types.';

	public function subscriber_only() { return FALSE; }

	public function form($json) {
		$form['#tree'] = TRUE;
		$reltype_options = array('[self]' => t('[Self]')) + sirius_taxonomy_load('sirius_contact_relationship_types', 'tid', 'name');
		$form['reltypes'] = array(
			'#title' => 'Relationship Types', 
			'#type' => 'checkboxes',
			'#options' => $reltype_options,
			'#default_value' => sirius_form_array_default_value($json['reltypes']),
			'#description' => t('This benefit is restricted to the following relationship types.'),
		);
		return $form;
	}

	public function check($settings, $params) {
		$all_reltypes = array('[self]' => t('[Self]')) + sirius_taxonomy_load('sirius_contact_relationship_types', 'tid', 'name');

		$relationship_node = $params['relationship_node'];

		$reltypes = array();
		if (isset($settings['reltypes'])) {
			$reltypes = array_filter(array_values($settings['reltypes']));
		}
		if (!$reltypes) { return array('success' => FALSE, 'msg' => 'No relationship types are permitted. Please configure the settings for this plugin to allow at least one relationship type.'); }
		foreach ($reltypes as $reltype) { $reltype_options[$reltype] = $all_reltypes[$reltype]; }

		$is_good = FALSE;

		if (!$relationship_node) { $my_reltype = '[self]'; } else { $my_reltype = $relationship_node->field_sirius_contact_reltype['und'][0]['tid']; }

		if (!$reltype_options[$my_reltype]) {
			return array('success' => FALSE, 'msg' => 'The relationship type [' . $all_reltypes[$my_reltype] . '] was not one of the available relationship types: (' . join(',', array_values($reltype_options)) . ').');
		}

		return array(
			'success' => TRUE,
			'msg' => 'The relationship type [' . $all_reltypes[$my_reltype] . '] is one of the approved types.',
		);
	}
}