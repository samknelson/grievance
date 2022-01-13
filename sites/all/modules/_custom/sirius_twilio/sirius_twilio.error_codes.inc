<?php

function sirius_twilio_render_error($error_code) {
	$html = $error_code;
	$html .= " [<a href=\"https://www.twilio.com/docs/api/errors/$error_code\" target=\"_blank\">Info</a>]";
	return $html;
}
