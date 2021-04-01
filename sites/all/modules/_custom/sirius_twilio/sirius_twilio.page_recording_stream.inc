<?php

function sirius_twilio_page_recording_stream($rec_sid, $download = FALSE) {
	// @todo: I don't understand access control at Twilio. They seem to post their recordings publicly, at a hard-to-guess but insecure URL.
	//        Oh well, if they're willing to put them there, I'm willing to proxy them, especially since my page is more secure than theirs.
	$url = 'https://api.twilio.com/2010-04-01/Accounts/';
	$url .= sirius_domain_variable_get('sirius_twilio_account_sid');
	$url .= '/Recordings/'; 
	$url .= $rec_sid;
	$url .= '.mp3';

	$ch = curl_init($url);
	curl_setopt_array($ch,
		[
			CURLOPT_RETURNTRANSFER => true,
			CURLOPT_CONNECTTIMEOUT => 30,
		]
	);
	
	header("Content-Type: audio/mpeg");
	if ($download) { header("Content-Disposition: attachment; filename=\"$rec_sid.mp3\""); }
	
	// Properly speaking, I ought to echo whatever headers Twilio sends. But this seemed to cause any number of problems. So I'm just setting my own.
	/*
	curl_setopt($ch, CURLOPT_HEADERFUNCTION, function($curl, $header) {
		header($header);
		return strlen($header);
	});
	*/

	curl_setopt($ch, CURLOPT_WRITEFUNCTION, function($curl, $body) {
		echo $body;
		return strlen($body);
	});
	
	$response = curl_exec($ch);
	curl_close($ch);

	exit;
}
