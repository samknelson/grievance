<?php

function sirius_okta_token_refresh() {
	return Sirius_Okta::getInstance()->token_refresh();
}

/**
 * Implements hook_entity_postsave()
 * 
 * We have to do entity_postsave because user_update hasn't written to the database yet :(
 */

function sirius_okta_entity_postsave($entity) {
	// There has to be a better way to figure out if an entity is a user
	if (!$entity->uid || !$entity->name || !$entity->pass || !$entity->mail) { return; }

	$okta = Sirius_Okta::getInstance();
	if (!$okta->autosync_active()) { return array('success' => TRUE, 'msg' => 'Autosync is not active.'); }
	return $okta->user_sync($entity->uid);
}

class Sirius_Okta extends Sirius_Singleton {
	public function active() {
		return variable_get('sirius_okta_active', '') == 'Yes';
	}

	public function autosync_active() {
		return variable_get('sirius_okta_autosync_active', '') == 'Yes';
	}

	public function token_refresh() {
		return $this->connection_test();
	}

	public function connection_test() {
		sirius_minilog("Starting connection test.");
		$result = $this->query('api-tokens');
		if ($result['success']) { $result['msg'] = "Okta connection successful."; }
		return $result;
	}

	public function email_check($email) {
		// Make sure it looks like an email address
    if (!sirius_email_validate($email)) { 
      return array('success' => FALSE, 'msg' => "The header for $key, $rfc822, does not look like a valid rfc822-formatted email address.");
    }

		// Make sure it is not a dummy domain. Because we are non-domain-specific, we check all domains.
		$patterns = &drupal_static(__FUNCTION__);
		if (!isset($patterns)) {
			// Since Okta is non-domain-specific, we need to check all domains.
			$domains = sirius_domain_tree_render_as_select();
			$patterns = array('nodomain.com');
			foreach ($domains as $domain_nid => $domain_name) {
				$patterns[] = sirius_domain_variable_get('sirius_worker_dummy_domain', 'nodomain.com', $domain_nid);
			}
		}
		foreach ($patterns as $pattern) {
			if (preg_match("/$pattern/", $email)) { return array('success' => FALSE, 'msg' => "The email address, $email, matches one of the dummy domain patterns."); }
		}

		// Make sure we are allow-listed
    if (!sirius_email_whitelist_validate($email)) { 
      return array('success' => FALSE, 'msg' => "Email is globally disabled and the email address, " . $email . ", does not match any of the whitelist patterns.");
    }

		return array('success' => TRUE);
	}

	public function user_fetch($account_uid) {
		$account = user_load($account_uid);
		if (!$account) { return array('success' => FALSE, 'msg' => "Drupal user not found: $account_uid"); }
		$endpoint = '/users/' . $account->name;
		return $this->query($endpoint);
	}

	public function user_sync($account_uid) {
		$result = SiriusResult::create($this->user_fetch($account_uid));
		if ($result->success()) {
			return $this->user_update($account_uid);
		} else {
			return $this->user_create($account_uid);
		}
		return array('success' => FALSE, 'msg' => 'User update not implemented.');
	}

	public function user_postfields($account_uid, $create = FALSE) {
		$account = user_load($account_uid);
		if (!$account) { return array('success' => FALSE, 'msg' => "Drupal user not found: $account_uid"); }
		$rid = variable_get('sirius_okta_role', '');
		if (!$rid) { return array('success' => FALSE, 'msg' => "No role has been set to sync to okta."); }
		if (!user_has_role($rid, $account)) { return array('success' => FALSE, 'msg' => 'The user does not have the Okta role.'); }

		$result = $this->email_check($account->mail);
		if (!$result['success']) { return $result; }

		$worker_node = sirius_worker_get($account_uid);

		$postfields = array();
		$postfields['profile'] = array();
		$postfields['profile']['login'] = $account->name;
		$postfields['profile']['email'] = $account->mail;

		if ($worker_node) {
			$postfields['profile']['firstName'] = $worker_node->field_sirius_name['und'][0]['given'];
			$postfields['profile']['lastName'] = $worker_node->field_sirius_name['und'][0]['family'];
			$postfields['profile']['mobilePhone'] = $worker_node->field_sirius_phone['und'][0]['value'];
		}

		if (!$postfields['profile']['firstName'] || !$postfields['profile']['lastName']) {
			$parts = preg_split('/\s+/', $account->name);
			$postfields['profile']['lastName'] = array_pop($parts);
			$postfields['profile']['firstName'] = join(' ', $parts);
		}

		if (!$postfields['profile']['mobilePhone']) {
			$postfields['profile']['mobilePhone'] = $account->field_grievance_phone['und'][0]['value'];
		}

		if (!$postfields['profile']['firstName']) { $postfields['profile']['firstName'] = "Noname"; }
		if (!$postfields['profile']['lastName']) { $postfields['profile']['lastName'] = "Noname" . $account_uid; }

		if ($create) {
			for ($i=0; $i<5; ++$i) {
				$rid = variable_get("sirius_okta_rolepush_${i}_from");
				if (!$rid) { continue; }
				if (!user_has_role($rid, $account)) { continue; }

				$userType = variable_get("sirius_okta_rolepush_${i}_to_type");
				if ($userType) { $postfields['type']['id'] = $userType; }

				$group = variable_get("sirius_okta_rolepush_${i}_to_group");
				if ($group) { $postfields['groupIds'][] = $group; }
			}
		}

		return array('success' => TRUE, 'postfields' => $postfields);
	}

	public function user_create($account_uid) {
		$result = $this->user_postfields($account_uid, TRUE);
		if (!$result['success']) { return $result; }

		$endpoint = '/users';
		return $this->query($endpoint, $result['postfields']);
	}

	public function user_update($account_uid) {
		$result = $this->user_postfields($account_uid);
		if (!$result['success']) { return $result; }

		$endpoint = '/users/' . $result['postfields']['profile']['login'];
		return $this->query($endpoint, $result['postfields']);
	}









	public function query($endpoint, $postfields = array()) {
		if (!$this->active()) { return array('success' => FALSE, 'msg' => 'Okta is not active.'); }

		if ($postfields) { $request_type = 'POST'; } else { $request_type = 'GET'; }

		$domain = variable_get('sirius_okta_domain');
		if (!$domain) { return array('success' => FALSE, 'msg' => 'No Okta domain specified.'); }

		$token = variable_get('sirius_okta_token');
		if (!$token) { return array('success' => FALSE, 'msg' => 'No Okta token specified.'); }

		$url = 'https://' . $domain . '/api/v1/' . $endpoint;

		$ch = curl_init();

		$header = array();
		$header[] = 'Authorization: SSWS ' . $token;
		if ($postfields) { $header[] = 'Content-Type: application/json'; }

  	curl_setopt($ch, CURLOPT_HTTPHEADER, $header);
		curl_setopt($ch, CURLOPT_URL, $url);
		// curl_setopt($ch, CURLOPT_BINARYTRANSFER, true);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
		curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 30);
  	curl_setopt($ch, CURLOPT_CUSTOMREQUEST, $request_type);
  	if ($postfields) { curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($postfields)); }

		$result = curl_exec($ch);
		$result_json = json_decode($result, TRUE);

		$curl_info = curl_getinfo($ch);
		if ($curl_info['http_code'] != 200) {
			$msg = "Request failed with error code: " . $curl_info['http_code'];
			if ($result_json['errorSummary']) { $msg .= ' -- ' . $result_json['errorSummary']; }
			return array('success' => FALSE, 'msg' => $msg, 'data' => $result_json, 'http_code' => $curl_info['http_code']);
		}

		curl_close($ch);

		if (!$result_json) {
			return array('success' => FALSE, 'msg' => "Request returned no result: " . curl_error($ch));
		}


		return array('success' => TRUE, 'msg' => 'Okta connection successful.', 'data' => $result_json);
	}
}
