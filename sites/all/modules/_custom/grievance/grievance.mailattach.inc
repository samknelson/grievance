<?php

function grievance_mailattach_fetch() {
	$mbox = imap_open("{uhexch00.unitehere.org:993/imap/ssl}INBOX", "grievance1-incoming@unitehere.org", "Local1!");
	if (!$mbox) { die("can't connect: " . imap_last_error()); }
	$MC = imap_check($mbox);
	$result = imap_fetch_overview($mbox,"1:{$MC->Nmsgs}",0);
	foreach ($result as $overview) {
		print_r($result);
  	echo "#{$overview->msgno} ({$overview->date}) - From: {$overview->from} {$overview->subject}\n";
	}
	imap_close($mbox);
}

