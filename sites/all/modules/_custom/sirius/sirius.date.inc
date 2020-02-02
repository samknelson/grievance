<?php

function sirius_date_ts($node, $field, $delta = 0, $col = 'value') {
	if (!$node->{$field}['und'][$delta]['value']) { return 0; }
	return strtotime($node->{$field}['und'][$delta][$col] . ' ' . $node->{$field}['und'][$delta]['timezone_db']);
}

function sirius_date_date($node, $field, $format = 'Y-m-d H:i:s') {
	return date($format, sirius_date_ts($node, $field));
}