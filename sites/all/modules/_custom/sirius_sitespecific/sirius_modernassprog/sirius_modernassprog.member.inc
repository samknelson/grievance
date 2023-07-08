<?php

class Sirius_Modernassprog_Member_Lookup_Jarvis extends Sirius_Pcct_Member_Lookup {

	public $type = 'pcct_jarvis';
	public $name = 'PCCT Jarvis / MAP';
	public $description = '';

  public function settings_form() { 
	  $form['intro'] = array(
	  	'#markup' => '<p>' . t('@todo: Settings for the connection (IP address, authentication keys') . '</p>',
	  );

		return $form;
	}

	public function request($args) {
		$settings = sirius_service_client_settings();

		$func = $args[0];
		if ($func != 'pcct_member_lookup') {
			return array('success' => FALSE, 'msg' => 'The PCCT service only knows how to implement the function "pcct_member_lookup".');
		}

		$member_id = $args[1];
		$member_token = $args[2];
		if (!$member_id || !$member_token) {
			return array('success' => FALSE, 'msg' => 'The pcct_lookup function requires a member ID and member token.');
		}

		return array(
			'success' => FALSE,
			'msg' => 'The Jarvis lookup is not yet implemented.',
		);
	}
}
