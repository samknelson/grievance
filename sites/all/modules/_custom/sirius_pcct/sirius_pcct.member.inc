<?php 

class Sirius_Pcct_Member_Lookup extends Sirius_Service_Client {
}

class Sirius_Pcct_Member_Lookup_Dummy extends Sirius_Pcct_Member_Lookup {

	public $type = 'pcct_dummy';
	public $name = 'PCCT Dummy';
	public $description = 'Dummy PCCT member lookup service (for testing)';

  public function settings_form() { 
	  $form['intro'] = array(
	  	'#markup' => '<p>' . t('This is a test connection service, used for testing the PCCT features when a true member service is not available.') . '</p>',
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
			'success' => TRUE,
			'oop' => rand(100, 999),
			'deductible' => rand(1000, 9999),
			'is_remote' => FALSE,
		);
	}
}
