<?php

function sirius_sns_handle($json) {
	// @todo: We should handle SNS channels better. But for now, if we've got a message object, log it as an email delivery notifcation.
	if ($json->Message) {
		$message = json_decode($json->Message);
		$json->Message = $message;

		if ($message->mail->headers) {
			foreach ($message->mail->headers as $header) {
				if ($header->name == 'X-Sirius-Id') {
					$id = $header->value;
				}
			}
		}

		$status = $message->notificationType;
		$to = $message->mail->destination[0];

		if ($id) {
			$query = new EntityFieldQuery();
			$query->entityCondition('entity_type', 'node');
			$query->entityCondition('bundle', 'sirius_log');
			$query->fieldCondition('field_sirius_notes', 'value', $id);
			$query->fieldCondition('field_sirius_log_handler', 'target_id', 0, '>');
			$query->fieldCondition('field_sirius_category', 'value', 'email');
			$query->range(0,1);
			$query->propertyOrderBy('nid', 'desc');
			$stmt = $query->execute();
			if ($stmt['node']) {
				$previous_log_node = node_load(array_keys($stmt['node'])[0]);
				$handler_nid = $previous_log_node->field_sirius_log_handler['und'][0]['target_id'];
			}
		}

		sirius_log(
	    'email', 
	    $status, 
	    $to, 
	    'SNS Callback',
	    $id, 
	    $handler_nid,
	    NULL,
	    json_encode($json)
		);

	}
}
