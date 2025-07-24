<?php

function sirius_postal_lob_page_body($uuid) {
  $client = Sirius_Postal_Client_Lob::getInstance();
  $body = $client->cache_get($uuid);

  if (!$body) {
    watchdog('sirius_postal_lob', "No body found for UUID $uuid");
    http_response_code(404);
    exit();
  }

  print $body;
  exit();
}