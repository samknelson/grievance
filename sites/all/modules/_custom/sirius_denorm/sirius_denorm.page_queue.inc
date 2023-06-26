<?php

function sirius_denorm_page_queue() {
	$items = sirius_denorm_queue_search(array('order' => 'denorm_id desc'));
	if (!$items) { return 'The denormalize queue is empty.'; }
	return sirius_denorm_queue_render($items, 1);
}
