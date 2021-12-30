<?php

function sirius_twilio_account_fetch() {
	try {
		$account_sid = sirius_domain_variable_get('sirius_twilio_account_sid', '');
		if (!$account_sid) { return array('success' => FALSE, 'msg' => 'There is no Twilio Account SID available.'); }

		$client = sirius_twilio_client_get();
		$account = $client->api->v2010->accounts($account_sid)->fetch();
		if (!$account) { return array('success' => FALSE, 'msg' => "The account with SID [$account_sid] was not found, or the access token is incorrect."); }

		return array('success' => TRUE, 'msg' => 'Account found', 'account' => $account);
	} catch (Exception $e) {
		$msg =  $e->getCode() . ' - ' . $e->getMessage();
		return array('success' => FALSE, 'msg' => "Account lookup failure: $msg");
	}
}


function sirius_twilio_app_fetch() {
	try {
		$app_sid = sirius_domain_variable_get('sirius_twilio_app_sid', '');
		if (!$app_sid) { return array('success' => FALSE, 'msg' => 'There is no Twilio App SID available.'); }

		$client = sirius_twilio_client_get();
		$app = $client->applications($app_sid)->fetch();
		if (!$app) { return array('success' => FALSE, 'msg' => "The App SID [$app_sid] was not found, or the access token is incorrect."); }

		return array('success' => TRUE, 'msg' => 'App found', 'app' => $app);
	} catch (Exception $e) {
		$msg =  $e->getCode() . ' - ' . $e->getMessage();
		return array('success' => FALSE, 'msg' => "Account lookup failure: $msg");
	}
}

function sirius_twilio_account_domains($account_sid) {
	// This is a sloppy and obnoxious way to fetch the domains associated with an account SID. But it's the best we've got.
	$sql = "select * from variable where name like :name and value like :value order by name";
	$stmt = sirius_sql_query($sql, array(':name' => '%/sirius_twilio_account_sid', ':value' => "%$account_sid%"));
	$domain_nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$value = unserialize($hr['value']);
		if ($value != $account_sid) { 
			continue;
		}
		$name = $hr['name'];
		$domain_nid = preg_replace('/\/.+/', '', $name);
		if ($domain_nid) {
			$domain_nids[] = $domain_nid;
		}
	}
	return $domain_nids;
}