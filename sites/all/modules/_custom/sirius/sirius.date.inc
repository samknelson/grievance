<?php

function sirius_date_ts($node, $field) {
	return strtotime($node->{$field}['und'][0]['value'] . ' ' . $node->{$field}['und'][0]['timezone_db']);
}

function sirius_date_date($node, $field, $format = 'Y-m-d H:i:s') {
	return date($format, sirius_date_ts($node, $field));
}