<?php

function sirius_twilio_page_recording_stream($recording_sid, $download = FALSE) {
	$url = sirius_twilio_recording_stream_url($recording_sid);
	$ch = curl_init($url);
	curl_setopt_array($ch,
		[
			CURLOPT_RETURNTRANSFER => true,
			CURLOPT_CONNECTTIMEOUT => 30,
		]
	);
	
	header("Content-Type: audio/mpeg");
	if ($download) { header("Content-Disposition: attachment; filename=\"$recording_sid.mp3\""); }
	
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
