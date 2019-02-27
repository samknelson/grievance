<?php

function sirius_dispatch_message_text($medium, $key, $tokens) {
	global $base_url;

	if ($medium == 'sms' && $key == 'dispatch_notify') {
		$text = "Teamsters 631 is announcing a job dispatch.\n";
		$text .= "Employer: [job_node:field_grievance_shop]\n";
		$text .= "Call type: [job_node:field_sirius_dispatch_job_type]\n";
		$text .= "Report date: [job_node:field_sirius_datetime]\n";
		$text .= "Link: $base_url/d/[dispatch_node:nid]/[dispatch_node:field_sirius_aat]\n";
		$text .= "\n";
		$text .= "To accept this job, reply:\n";
		$text .= "[dispatch_node:nid] accept\n";
		$text .= "\n";
		$text .= "To decline this job, reply:\n";
		$text .= "[dispatch_node:nid] decline\n";
	} 

	if ($medium == 'email' && $key == 'dispatch_notify_subject') {
		$text = "Teamsters 631 job dispatch announcement: [job_node:field_grievance_shop]";
	}

	if ($medium == 'email' && $key == 'dispatch_notify_body') {
		$text = "Teamsters 631 is announcing a job dispatch.<br /><br />";
		$text .= "Employer: <strong>[job_node:field_grievance_shop]</strong><br />";
		$text .= "Call type: <strong>[job_node:field_sirius_dispatch_job_type]</strong><br />";
		$text .= "Report date: <strong>[job_node:field_sirius_datetime]</strong><br /><br />";
		$text .= "To view the full dispatch and accept or decline, please follow the link below:<br /><br />";
		$url = "$base_url/d/[dispatch_node:nid]/[dispatch_node:field_sirius_aat]";
		$text .= "<a href=\"$url\">$url</a>";
	} 

	else if ($medium == 'sms' && $key == 'dispatch_notify_details') {
		$text .= "Employer: [job_node:field_grievance_shop]\n";
		$text .= "Call type: [job_node:field_sirius_dispatch_job_type]\n";
		$text .= "Facility: [job_node:field_sirius_dispatch_facility]\n";
		$text .= "Hall: [job_node:field_sirius_dispatch_hall]\n";
		$text .= "Report date: [job_node:field_sirius_datetime]\n";
		$text .= "Link: $base_url/d/[dispatch_node:nid]/[dispatch_node:field_sirius_aat]\n";
		$text .= "[sirius:dispatch:job:body_text]\n";
		$text .= "\n";
		$text .= "To accept this job, reply:\n";
		$text .= "[dispatch_node:nid] accept\n";
		$text .= "\n";
		$text .= "To decline this job, reply:\n";
		$text .= "[dispatch_node:nid] decline\n";
	} 

	else if ($medium == 'sms' && $key == 'dispatch_notify_unknown') {
		$text = "I'm sorry, I don't understand that response.\n";
		$text .= "\n";
		$text .= "To accept this job, reply:\n";
		$text .= "[dispatch_node:nid] accept\n";
		$text .= "\n";
		$text .= "To decline this job, reply:\n";
		$text .= "[dispatch_node:nid] accept\n";
	} 

	else if ($medium == 'sms' && $key == 'dispatch_accept_confirm') {
		$text = "The dispatch has been accepted. Thank you.";
	} 

	else if ($medium == 'sms' && $key == 'dispatch_decline_confirm') {
		$text = "The dispatch has been declined. Thank you.";
	} 

	else if ($medium == 'sms' && $key == 'dispatch_error') {
		$text = "We were unable to process your request: [sirius:dispatch:err]\n";
	} 

	else if ($medium == 'voice' && $key == 'dispatch_notify') {
		$text = "Teamsters 6 3 1 is announcing a job from [job_node:field_grievance_shop]. ";
		$text .= "The call type is [job_node:field_sirius_dispatch_job_type]. ";
		$text .= "The report date is [job_node:field_sirius_datetime]. ";
		$text .= "Press 1 for more details. ";
		$text .= "Press 2 to accept. ";
		$text .= "Press 3 to decline. ";
		$text .= "Press 8 to return to the previous menu. ";
		$text .= "Press 9 to be connected with the operator. ";
		$text .= "Press star to hear this message again.";
	}

	else if ($medium == 'voice' && $key == 'dispatch_notify_details') {
		$text .= "The employer is [job_node:field_grievance_shop]. ";
		$text .= "The facility [job_node:field_sirius_dispatch_facility]. ";
		$text .= "[sirius:dispatch:job:body_text] ";
		$text .= "Press 1 for more details. ";
		$text .= "Press 2 to accept. ";
		$text .= "Press 3 to decline. ";
		$text .= "Press 8 to return to the previous menu. ";
		$text .= "Press 9 to be connected with the operator. ";
		$text .= "Press star to hear this message again.";
	}

	else if ($medium == 'voice' && $key == 'dispatch_notify_unknown') {
		$text = "I'm sorry, I don't understand that response. ";
		$text .= "Press 1 for more details. ";
		$text .= "Press 2 to accept. ";
		$text .= "Press 3 to decline. ";
		$text .= "Press 8 to return to the previous menu. ";
		$text .= "Press 9 to be connected with the operator. ";
		$text .= "Press star to hear this message again.";
	} 

	else if ($medium == 'voice' && $key == 'dispatch_callback_requested') {
		$text = "A dipatch operator will call you shortly. ";
	}

	else if ($medium == 'voice' && $key == 'dispatch_forward_to_operator') {
		$text = "Your call is being forwarded to the office.";
	}

	else if ($medium == 'voice' && $key == 'dispatch_accept_confirm') {
		$text = "The dispatch has been accepted. Thank you.";
	} 

	else if ($medium == 'voice' && $key == 'dispatch_decline_confirm') {
		$text = "The dispatch has been declined. Thank you.";
	} 

	else if ($medium == 'voice' && $key == 'dispatch_error') {
		$text = "We were unable to process your request. [dispatch_node:field_sirius_message]";
	} 


	if (!$text) {
		$tokens['sirius']['message']['medium'] = $medium;
		$tokens['sirius']['message']['key'] = $key;
		$text = "Error: message text not set for medium = [sirius:message:medium], key = [sirius:message:key]";
	}

	return sirius_text_render($text, $tokens);
}
