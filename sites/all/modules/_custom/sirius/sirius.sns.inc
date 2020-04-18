<?php


// @todo: composer and autoload :(
require_once DRUPAL_ROOT . '/sites/all/libraries/aws-sns-validator/src/Message.php';
require_once DRUPAL_ROOT . '/sites/all/libraries/aws-sns-validator/src/MessageValidator.php';
require_once DRUPAL_ROOT . '/sites/all/libraries/aws-sns-validator/src/Exception/InvalidSnsMessageException.php';

use Aws\Sns\Message;
use Aws\Sns\MessageValidator;

function sirius_sns_config() {
  $form["sirius_sns_topics"] = array(
    '#type' => 'textarea',
    '#title' => 'Amazon SES Topics',
    '#default_value' => variable_get("sirius_sns_topics", ''),
    '#description' => t('Enter the permitted AWS SNS topic ARMs, one per line.'),
  );

  return system_settings_form($form);
}

function sirius_sns_incoming() {
  // Construct our JSON.
  $json = json_decode(file_get_contents("php://input"));

  // The payload is attached as a raw string. Expand it as well.
  if ($json && $json->Message) { $json->Message = json_decode($json->Message); }

  // Next we're going to verify that this is a safe message to process.
  $is_safe = FALSE;

  // Admins can access
  if (user_access('sirius sns admin')) { $is_safe = TRUE; }

  // Subscription confirmations can access
  if ($json->Type == "SubscriptionConfirmation") { $is_safe = TRUE; }

  // Validate the topic
  if (!$is_safe) {
    $topic = $json->TopicArn;
    if (!$topic) { 
      watchdog('sirius_sns', 'No topic found: <pre>' . print_r($json, 1) . '</pre>');
      return "No topic found. Message not processed.";
    }
    $allowed_topics = preg_split('/\s+/', variable_get('sirius_sns_topics', ''));
    if (!in_array($topic, $allowed_topics)) { 
      watchdog('sirius_sns', "Invalid topic $topic: <pre>" . print_r($json, 1) . '</pre>');
      return "Invalid topic $topic. Message not processed.";
    }
  }

  // Check the signature.
  if (!$is_safe) {
    $validatorMessage = Message::fromRawPostData();
    $validator = new MessageValidator();
    if (!$validator->isValid($validatorMessage)) {
      watchdog('sirius_sns', "Invalid message: <pre>" . print_r($json, 1) . '</pre>');
      return "Invalid message. Message not processed.";
    }
  }

  // We're good. We can proceed to handle this message.
  sirius_sns_handle($json);
  return 'Ok';
}

function sirius_sns_handle($json) {
  // Handle mail callback
	if ($json->Message->mail) {
    $type = $json->Message->eventType;
    if ($json->Message->mail->headers) {
      foreach ($json->Message->mail->headers as $header) {
        if ($header->name ==  'X-Sirius-Id') { $id = $header->value; }
      }
    }
    sirius_email_callback($id, $type, $json);
    return TRUE;
	}

  // Handle subscription callback
  if ($json->Type == "SubscriptionConfirmation") {
    sirius_sns_handle_subscription($json);
    return TRUE;
  }

  sirius_log('sns', 'unknown', 'Unknown json callback', '', '', array(), array(), $json);  
}

function sirius_sns_handle_subscription($json) {
  $url = $json->SubscribeURL;
  $curl_handle = curl_init();
  curl_setopt($curl_handle, CURLOPT_URL, $url);
  curl_setopt($curl_handle, CURLOPT_CONNECTTIMEOUT, 2);
  curl_exec($curl_handle);
  curl_close($curl_handle);

  sirius_log('sns', 'subscription', 'Subscription confirmation for ' . $url, '', '', array(), array(), $json);
}
