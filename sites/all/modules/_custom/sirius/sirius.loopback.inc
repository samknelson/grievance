<?php


/**
 * Utility function to determine if this is a self-request -- i.e. a call from the current server.
 * 
 * Because of public / private issues and HTTP_X_FORWARDED_FOR, this is harder than it sounds.
 */

function sirius_loopback_url() {
	global $is_https;
	if ($is_https ||
		(isset($_SERVER['HTTP_X_FORWARDED_PROTO']) && $_SERVER['HTTP_X_FORWARDED_PROTO'] == 'https')) {
		$url = 'https://';
	} else { 
		$url = 'http://';
	}
	$url .= $_SERVER['SERVER_NAME'];
	return $url;
}

function sirius_loopback_access() {
  if (user_access('sirius admin')) { return TRUE; }

  $remote_ip = sirius_loopback_remote_ip();
  if (!$remote_ip) { return FALSE; }

  if ($remote_ip == $_SERVER['SERVER_ADDR']) { return TRUE; }

  global $base_url;
  $url = $base_url . '/sirius/loopback/remote-ip';
  $server_ip = file_get_contents($url);
  if ($server_ip == $remote_ip) { return TRUE; }

  return FALSE;
}

function sirius_loopback_remote_ip() {
  $remote_ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
  if (!$remote_ip) $remote_ip = $_SERVER['REMOTE_ADDR'];
  return $remote_ip;
}

function sirius_loopback_remote_ip_print() {
  print sirius_loopback_remote_ip(); 
  exit();
}

function sirius_loopback_sample_html($letterhead_node = NULL, $paragraph_count = 14) {
	if ($paragraph_count > 1000) { $paragraph_count = 1000; }

	$paragraphs = array();

	$paragraphs[] = 'Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Commodo nulla facilisi nullam vehicula ipsum a. Consectetur a erat nam at lectus. Blandit volutpat maecenas volutpat blandit aliquam etiam erat velit scelerisque. Nullam vehicula ipsum a arcu cursus vitae. Ac feugiat sed lectus vestibulum. Suspendisse faucibus interdum posuere lorem. Euismod quis viverra nibh cras pulvinar. Vel facilisis volutpat est velit egestas dui. Sapien et ligula ullamcorper malesuada proin. Blandit turpis cursus in hac habitasse. Aenean vel elit scelerisque mauris pellentesque. Amet cursus sit amet dictum. Nec nam aliquam sem et tortor consequat id porta nibh. Commodo quis imperdiet massa tincidunt nunc pulvinar. In egestas erat imperdiet sed euismod nisi porta lorem mollis. Ante metus dictum at tempor commodo ullamcorper a lacus vestibulum. Sodales ut eu sem integer vitae justo eget.';

	$paragraphs[] = 'Id neque aliquam vestibulum morbi blandit. Diam volutpat commodo sed egestas egestas. Aliquet sagittis id consectetur purus. Etiam non quam lacus suspendisse faucibus interdum posuere lorem. Congue eu consequat ac felis donec et odio pellentesque diam. Malesuada fames ac turpis egestas maecenas pharetra convallis. Eleifend donec pretium vulputate sapien nec sagittis aliquam malesuada. Ultrices tincidunt arcu non sodales neque sodales. Ipsum dolor sit amet consectetur adipiscing elit duis tristique sollicitudin. A pellentesque sit amet porttitor eget. Risus quis varius quam quisque id. Dictum varius duis at consectetur lorem donec massa. Amet dictum sit amet justo donec. Ultrices neque ornare aenean euismod elementum nisi. Vulputate sapien nec sagittis aliquam malesuada bibendum arcu. Pellentesque habitant morbi tristique senectus et netus et malesuada fames.';

	$paragraphs[] = 'Sed id semper risus in hendrerit gravida rutrum quisque non. Massa massa ultricies mi quis hendrerit dolor magna. Iaculis eu non diam phasellus vestibulum lorem. Aliquam nulla facilisi cras fermentum odio eu feugiat pretium. Ut ornare lectus sit amet est placerat in. Dolor sed viverra ipsum nunc aliquet bibendum. Diam phasellus vestibulum lorem sed risus ultricies tristique. Urna neque viverra justo nec ultrices. Laoreet id donec ultrices tincidunt arcu non sodales neque sodales. Phasellus faucibus scelerisque eleifend donec pretium. Risus at ultrices mi tempus imperdiet nulla malesuada pellentesque elit.';

	$paragraphs[] = 'Tincidunt id aliquet risus feugiat in ante metus. Vulputate dignissim suspendisse in est ante. Enim eu turpis egestas pretium aenean pharetra magna ac placerat. Enim eu turpis egestas pretium aenean pharetra magna ac. Aliquet nibh praesent tristique magna. Nascetur ridiculus mus mauris vitae ultricies leo integer. Sit amet commodo nulla facilisi nullam vehicula ipsum. Eu sem integer vitae justo eget. Nam libero justo laoreet sit amet cursus. Lacinia at quis risus sed vulputate odio ut enim. Porttitor leo a diam sollicitudin tempor id eu nisl nunc. Nam libero justo laoreet sit. Diam volutpat commodo sed egestas egestas fringilla phasellus faucibus. Vulputate odio ut enim blandit. Amet massa vitae tortor condimentum.';

	$paragraphs[] = 'Adipiscing at in tellus integer feugiat scelerisque. Est ante in nibh mauris. Donec adipiscing tristique risus nec feugiat in fermentum posuere. Amet purus gravida quis blandit turpis cursus in. Tincidunt praesent semper feugiat nibh sed pulvinar. Nullam vehicula ipsum a arcu cursus vitae congue mauris. Egestas purus viverra accumsan in nisl nisi scelerisque eu. Faucibus et molestie ac feugiat. Volutpat blandit aliquam etiam erat velit scelerisque in dictum. In nibh mauris cursus mattis.';

	$paragraphs[] = 'Id neque aliquam vestibulum morbi blandit cursus risus at ultrices. In eu mi bibendum neque egestas congue. Scelerisque felis imperdiet proin fermentum leo. Arcu ac tortor dignissim convallis aenean et tortor at. Mattis vulputate enim nulla aliquet porttitor lacus luctus. Cursus euismod quis viverra nibh cras pulvinar mattis nunc. Sed viverra tellus in hac habitasse platea dictumst vestibulum rhoncus. Egestas sed sed risus pretium quam. At augue eget arcu dictum. Viverra accumsan in nisl nisi scelerisque. Nulla facilisi morbi tempus iaculis urna id volutpat lacus laoreet. Aenean pharetra magna ac placerat vestibulum lectus mauris ultrices eros. In metus vulputate eu scelerisque felis. Enim blandit volutpat maecenas volutpat blandit aliquam etiam. Risus commodo viverra maecenas accumsan lacus vel facilisis. Volutpat odio facilisis mauris sit amet massa vitae tortor.';

	$paragraphs[] = 'Ipsum dolor sit amet consectetur. Adipiscing bibendum est ultricies integer quis. Nisl nunc mi ipsum faucibus vitae aliquet nec ullamcorper sit. Elit eget gravida cum sociis natoque penatibus et magnis. Rhoncus urna neque viverra justo. Lorem mollis aliquam ut porttitor leo a diam sollicitudin. Purus non enim praesent elementum facilisis leo vel. Interdum posuere lorem ipsum dolor sit amet. Varius morbi enim nunc faucibus a pellentesque. Massa massa ultricies mi quis hendrerit. Mauris pharetra et ultrices neque ornare aenean. In nulla posuere sollicitudin aliquam ultrices sagittis orci. Aenean pharetra magna ac placerat vestibulum.';

	$paragraphs[] = 'Quam id leo in vitae turpis. Elementum sagittis vitae et leo duis. At in tellus integer feugiat scelerisque varius morbi. Viverra vitae congue eu consequat ac felis donec et. Velit sed ullamcorper morbi tincidunt ornare massa eget egestas. Blandit libero volutpat sed cras ornare. Fames ac turpis egestas integer eget aliquet nibh praesent. Urna et pharetra pharetra massa massa ultricies. Pellentesque habitant morbi tristique senectus et netus et malesuada. Egestas diam in arcu cursus euismod quis. Vulputate sapien nec sagittis aliquam malesuada bibendum arcu vitae elementum. Ut placerat orci nulla pellentesque dignissim enim sit amet.';

	$paragraphs[] = 'Massa enim nec dui nunc mattis. Sit amet porttitor eget dolor morbi non arcu risus. Ac orci phasellus egestas tellus rutrum tellus. Fermentum dui faucibus in ornare quam viverra orci. Amet venenatis urna cursus eget nunc scelerisque. Proin sagittis nisl rhoncus mattis rhoncus. Integer enim neque volutpat ac tincidunt vitae semper quis lectus. Pretium nibh ipsum consequat nisl vel pretium lectus. Velit aliquet sagittis id consectetur purus ut faucibus pulvinar. Eros in cursus turpis massa tincidunt. Nisl suscipit adipiscing bibendum est ultricies integer. Tellus cras adipiscing enim eu turpis egestas. Nulla facilisi etiam dignissim diam. Vitae aliquet nec ullamcorper sit amet risus nullam eget. Eget est lorem ipsum dolor sit amet consectetur adipiscing.';

	$paragraphs[] = 'Hac habitasse platea dictumst quisque sagittis purus. Lacus laoreet non curabitur gravida arcu ac tortor dignissim convallis. Vestibulum rhoncus est pellentesque elit ullamcorper. Interdum velit laoreet id donec ultrices tincidunt arcu non sodales. Turpis in eu mi bibendum neque. Amet volutpat consequat mauris nunc congue nisi vitae. Nulla facilisi cras fermentum odio eu feugiat. Bibendum enim facilisis gravida neque convallis a cras. Pulvinar elementum integer enim neque volutpat ac tincidunt vitae semper. Posuere sollicitudin aliquam ultrices sagittis orci a scelerisque. Convallis posuere morbi leo urna molestie at. Habitant morbi tristique senectus et netus et. Purus semper eget duis at tellus at. Morbi non arcu risus quis. Imperdiet sed euismod nisi porta lorem mollis. Pellentesque sit amet porttitor eget dolor. Tincidunt dui ut ornare lectus. Velit scelerisque in dictum non consectetur a erat. Varius morbi enim nunc faucibus a pellentesque sit.';

	$html = '<h1>Sample HTML Content</h1>';
	for ($i=0; $i<$paragraph_count; ++$i) {
		if ($i % 5 == 0) {
			$html .= "<h2>Paragraph #" . ($i+1) . "</h2>";
		}
		$html .= '<p>';
		$html .= $paragraphs[$i % 10];
		$html .= '</p>';
	}
	return $html;
}

function sirius_loopback_echo() {
	grievance_classes_array('loopback');

	$body = $_REQUEST['body'];
	if (!$body) { $body = 'No body.'; }
	return $body;
}