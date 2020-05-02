<?php

/**
 * @todo: Currently all fax functionality lives in grievance.phaxio.inc. Need to abstract out as we do for SMS and other services.
 */

function sirius_fax_enabled() {
	return variable_get('grievance_phaxio_api_key', '');
}

