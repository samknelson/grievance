<?php

function sirius_edls_page_worker_list() {
	$workers = sirius_edls_worker_list(2457288, '1970-01-01', array());
	dpm($workers);
	return 'Dev...';
}