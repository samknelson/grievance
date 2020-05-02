<?php

function sirius_page_esignature_test($which = NULL) {
	sirius_esignature_init();

	if ($which) { return drupal_get_form('sirius_page_esignature_test_form', $which); }

	$node = sirius_esignature_test_node();
	$json = sirius_jsonfield_util_node_json_get($node);

	if ($_REQUEST['clear']) { 
		sirius_jsonfield_util_esignature_set($json, $_REQUEST['clear'], NULL);
		sirius_jsonfield_util_node_json_set($node, $json);
		node_save($node);
	}


	$html = '<h2>' . t('Demonstration of a signature for a long bit of legalese') . '</h2>';
	$which = 'big';
	$esignature = sirius_jsonfield_util_esignature_get($json, $which);
	if ($esignature) { $html .= sirius_esignature_render($esignature, 'full', $node->nid, $which); }
	$html .= '<br />';
	if ($esignature) {
		$html .= l('Clear', current_path(), array('attributes' => array('class' => array('tool-button')), 'query' => array('clear' => $which)));
	} else {
		$html .= l('Sign Now', current_path() . "/$which", array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => current_path())));
	}

	$html .= '<hr><h2>' . t('Demonstration of a sign-in attendance sheet') . '</h2>';
	$header = array('Nane', 'Badge', 'Signature', 'Tools');
	$rows = array();
	for ($i=1; $i<20; ++$i) {
		$which = 'attendance_' . $i;
		$esignature = sirius_jsonfield_util_esignature_get($json, $which);

		$row = array();
		$row[] = sirius_esignature_sample_name($i);
		$row[] = rand(100000, 999999);
		if ($esignature) { $row[] .= sirius_esignature_render($esignature, '', $node->nid, $which); } else { $row[] = ''; }

		if ($esignature) {
			$row[] = l('Clear', current_path(), array('attributes' => array('class' => array('tool-button')), 'query' => array('clear' => $which)));
		} else {
			$row[] = l('Sign Now', current_path() . "/$which", array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => current_path())));
		}
		$rows[] = $row;
	}
	$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));

	return $html;

}

function sirius_page_esignature_test_form($form, &$form_state, $which) {
	$form_state['which'] = $which;

	if ($which == 'big') {
		$statement .= "By signing, I hereby acknowledge that I have read and understand the statement below, and I agree that it consists of a large number of legal-sounding terms which make no sense individually or collectively.";
		$statement .= '<div class="sirius_esignature_statement_body">';
		$statement .= sirius_esignature_sample_legalese();
		$statement .= '</div>';
		$title = 'Agree to Legalese';
	} else {
		$statement = 'By signing, I hereby acknowledge that I was present.';
		$title = 'Present';
	}

	sirius_esignature_form_prepare($form, $which, $title, $statement);

	return $form;
}

function sirius_page_esignature_test_form_submit($form, &$form_state) {
	$which = $form_state['which'];

	$node = sirius_esignature_test_node();
	$json = sirius_jsonfield_util_node_json_get($node);
	$signature = sirius_esignature_form_get_json($form, $form_state, $which);
	sirius_jsonfield_util_esignature_set($json, $which, $signature);
	sirius_jsonfield_util_node_json_set($node, $json);
	node_save($node);
}

/**
 * eSignatures should be attached to a node. We use a random floating "json_definition" node for our test bank.
 */

function sirius_esignature_test_node() {
	$domain_nid = sirius_domain_nid();
	$type = 'esignature:test';

  $sql = "select nid ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_json_definition' ";
  $sql .= "and field_sirius_type_value = :type ";
  $sql_args[':type'] = $type;
  if ($domain_nid) {
  	$sql .= "and field_sirius_domain_target_id = :domain_nid ";
  	$sql_args[':domain_nid'] = $domain_nid;
  } else {
  	$sql .= "and field_sirius_domain_target_id is null ";
  }
  $sql .= "order by created desc limit 1";
  $stmt = db_query($sql, $sql_args);
  $hr = $stmt->fetchAssoc();
  if ($hr['nid']) {
		return node_load($hr['nid']);
  }

	global $user;
  $node = new stdClass();
  $node->type = 'sirius_json_definition';
  $node->title = $type;
  $node->no_recurse = TRUE;
  $node->language = LANGUAGE_NONE;
  $node->uid = $user->uid;
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 0;
  $node->field_sirius_type['und'][0]['value'] = $type;
  if ($domain_nid) {
  	$node->field_sirius_domain['und'][0]['target_id'] = $domain_nid;
  }
  $node = node_submit($node);
  node_object_prepare($node);
  node_save($node);
  return $node;
}

/**
 * Utility function to generate a random sample human name (first last)
 */

function sirius_esignature_sample_name($seed) {
	static $alphabet;
	if (!$alphabet) { $alphabet = str_split('abcdefghijklmnopqrstuvwxyz'); }
	srand($seed);
	$name = '';
	for ($part = 0; $part < 2; ++$part) {
		$len = rand(4, 12);
		$string = '';
		for ($i=0; $i<$len; ++$i) { $string .= $alphabet[rand(0,26)]; }
		if ($name) { $name .= ' '; }
		$name .= ucfirst($string);
	}
	return $name;
}

/**
 * Utility function to generate some sample random legalese
 */

function sirius_esignature_sample_legalese() {
	$html = '<p>You must make sure that they, too, receive or can get the source code. Digital Content created by such Participant under Sections 2.1 and/or 2.2 of this License including, without limitation, Section 2. Any Modifications that you also do the following: rename any non-standard executables and testcases, which must be included in all respects by the Package. Distributor Fees are permitted, and licensing fees for warranty or support, or for a recipient will know whom to contact. If you make available thereafter and shall take other steps (such as notifying appropriate mailing lists or newsgroups) reasonably calculated to inform those who receive copies directly or indirectly infringes any patent, then any Derivative Works thereof. Exclusions From License Grant.</p>';
	$html .= '<p>Nothing in this License to Your modifications and may provide additional or different license You must include a copy of this Package, but belong to whomever generated them, and may provide additional or different license terms and conditions for distributing the Program by the laws of the License. You may not offer or impose any terms on such Contributors behalf, and not on behalf of any other recipients of the rest of the conditions of this section do not charge a fee for the Package. Distributor Fees are permitted, and licensing fees for warranty or support, or for any distribution of executable or other intellectual property infringement. In order to qualify, an Indemnified Contributor to control, and cooperate with the Commercial Contributor must include a readable copy of this Package as a series of files, a Modification is: (a) any addition to or loss of data, data being rendered inaccurate, or losses sustained by anyone as a special exception, the "BeOpen Python" logos available at http://www.pythonlabs.com/logos.html may be published from time to time. Such new versions (including revisions) of this Agreement.</p>';
	$html .= '<p>Article 1 below) under the terms of any kind, either expressed or implied, are granted by You alone, and You must duplicate the notice in each instance: (a) You must retain, in the base LaTeX distribution explains the motivation behind the conditions specified above; in particular, your modified NetHack or use pieces of it that are in your work but, instead, distribute your Derivative Works of, publicly display, publicly perform, distribute and sublicense the Contribution causes such combination to be unenforceable, such provision valid and enforceable. If Recipient institutes patent litigation against any entity (including a cross-claim or counterclaim in a commercial product offering, such Distributor ("Commercial Distributor") hereby agrees to be distributed and/or modified under the GNU General Public License along with the Program in a reasonable fashion that the Source Code of any sort solely as a single product. In such an action is referred to herein as "Respondent") alleging that Licensed Product or Modifications that you know you can do these things.</p>';
	$html .= '<p>To protect your rights under this License. Claims" means any patent claim(s), now owned or controlled by Contributor, to make, have made, and/or otherwise dispose of Modifications made by offering access to copy the Work by agreement with any of the Licensed Product, including Modifications made by that Contributor alone and not on behalf of the source code, to be unenforceable, that provision of the Program if, at the mercy of those companies. By contrast, the GNU General Public License from time to time.</p>';
	$html .= '<p>Such new versions of applications that include the following conditions: (1) The Recipient must attach a copy of this License. If the Work identical with the scope of the GNU General Public License is retained in Python 1.6b1, Licensee agrees to cease use and distribution of the following disclaimers. Redistributions in binary form must reproduce the above copyright notice, this list of authors may be distributed under this Agreement shall terminate as of the outstanding shares, or (iii) beneficial ownership of fifty percent (50%) or more of the Work, transformations of the Source Code or as an executable program under a license under Licensed Patents to make, use, sell, offer to distribute the Program or any distributor hereunder prior to termination shall survive any termination of this Agreement, whether expressly, by implication, appearance or otherwise.</p>';
	$html .= '<p>As a condition to, the modification, by any Contributor even if such Contributor as a result of warranty, support, indemnity or liability obligations and/or other materials provided with the complete corresponding machine-readable source code, even though third parties are not required to license it under the terms of this License released under the GNU Free Documentation licence and is inclu‐ ded in some (droit d\'auteur) countries authors cannot disclaim all liabi‐ lities. Compliance to DFSG 1.1 is ensured, and GPLv2compatibility is asserted unless advertising clauses are used. The MirOS Licence is certified to conform to OKD 1.0 and OSD 1.9, and qualifies as a LaTeX package), but it is "author-maintained". The Work has the right to modify the terms of that version.</p>';
	$html .= '<p>You may aggregate the Package with respect to some or all of these conditions: a) You must make the Source Code. Third Party Claims. If you make to the Notice. If it is not limited to, loss of data, and may be copied, such that the Work by agreement with any of the Source Code.</p>';
	$html .= '<p>You must cause it, when started running for such a way for the Work. It is wise never to modify the terms of this Agreement and does not mean licensing fees. Version" refers to such actions.</p>';
	$html .= '<p>The Recipient must attach a copy of the Work, a prominent statement that the original test modes, this license document the following conditions are different, write to the Program, and copy and distribute verbatim copies of NetHack, that you can no longer maintain and </p>support it yourself. Derived Works That Are Not Replacements Several clauses of the Original Code provided by the terms applicable to software that is normally needed for running or interpreting a part of a part or the Reproduction and Other Exploitation of the provisions of this License is derived from the substance or structure of said Notice Period. Reasonable Value of This License and all of the existing conformance modes of this license place any restrictions on aggregating such works with the Program. Patents " mean patent claims licensable by a version of this License.</p>';
	$html .= '<p>Derivative Works (as that term is defined under Dutch copyright law. Please follow the guidelines below: Re-use of text: Attribution: To re-distribute a text page in any notice in an Executable version, related documentation in which at least three years, to give attribution in the Appendix below). Works" shall mean a Digital Font Program newly created by the Initial Developer and every part regardless of who wrote it.</p>';
	$html .= '<p>Thus, it is not the original, and also raise any specification issues with the Program in a conspicuous location in the edit summary, which is implemented by public license is required to allow Recipient to distribute copies of the Package or providing support for the Derived Program. Any material to be allowed to make and give any other for compensation, indemnity or liability obligation is offered by that Contributor. Distribution Obligations. 3.1.</p>';
	$html .= '<p>Application of This License constitutes the entire Package. The Indemnified Contributor may participate in any form of the <ORGANIZATION> nor the names do not accept this License, they do not refer to the permissions granted by that particular Modification has been processed into a form where it is sufficient to obtain the complete corresponding machine-readable source code for all parties, and never knowingly distribute a Derived Work to be bound by the Licensor except as permitted above, be liable to any person or persons nominated as such will be given a distinguishing version number. Effect of Termination. Upon termination, You agree not to be unenforceable, such provision valid and enforceable.</p>';
	$html .= '<p>If Recipient institutes patent litigation (including a cross-claim or counterclaim in a lawsuit) alleging that such Participant\'s Contributor Version (or portions thereof); and 2) the combination of the General Public License (LPPL) is the initial Contributor, the initial code and object code form. Subject to the original Work. If a component of this license directly with the terms of that component clearly states in the Northern District of California, and You agree not to use the text you hold the copyright notice and this permission notice shall be reformed to the Covered Code. Code" means the individual(s) or organization(s) named in the Derived Program to a third party\'s intellectual property laws of the Work, and no requirements are made concerning any offers of support for the physical act of running the Program).</p>';

	return $html;
}
