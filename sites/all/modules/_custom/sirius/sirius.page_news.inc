<?php

function sirius_page_news() {
	global $user;
	return views_embed_view('sirius_news', 'block_1', join('+', array_keys($user->roles)));
}
