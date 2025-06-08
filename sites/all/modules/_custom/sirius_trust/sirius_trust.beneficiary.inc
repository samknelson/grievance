<?php

class Sirius_Trust_Beneficiary extends Sirius_Singleton {
	public function form($form, &$form_state, $entity_node) {
		$form_state['entity_nid'] = $entity_node->nid;

		$form['#tree'] = TRUE;
		foreach (array('primary' => 'Primary'/*, 'secondary' => 'Secondary'*/) as $key => $title) {
			/*
			$form['beneficiaries'][$key] = array(
				'#type' => 'fieldset',
				'#title' => $title .= " Beneficiaries",
			);
			*/

			$form['beneficiaries'][$key]['intro'] = array('#markup' => '<table><tr><th>Name</th><th>SSN</th><th>Phone</th><th>Address</th><th>%</th></tr>');
			for ($i=0; $i<8; ++$i) {
				$form['beneficiaries'][$key][$i]['intro'] = array('#markup' => '<tr>');
				$form['beneficiaries'][$key][$i]['name'] = array(
					'#type' => 'textfield', 
					'#field_prefix' => '<td>', 
					'#field_suffix' => '</td>', 
					'#default_value' => $data['beneficiaries'][$key][$i]['name'],
					'#size' => 20
				);
				$form['beneficiaries'][$key][$i]['ssn'] = array(
					'#type' => 'textfield', 
					'#field_prefix' => '<td>', 
					'#field_suffix' => '</td>', 
					'#default_value' => $data['beneficiaries'][$key][$i]['ssn'],
					'#size' => 12
				);
				$form['beneficiaries'][$key][$i]['phone'] = array(
					'#type' => 'textfield', 
					'#field_prefix' => '<td>', 
					'#field_suffix' => '</td>', 
					'#default_value' => $data['beneficiaries'][$key][$i]['phone'],
					'#size' => 12
				);
				$form['beneficiaries'][$key][$i]['address'] = array(
					'#type' => 'textfield', 
					'#field_prefix' => '<td>', 
					'#field_suffix' => '</td>', 
					'#default_value' => $data['beneficiaries'][$key][$i]['address'],
					'#size' => 30
				);
				$form['beneficiaries'][$key][$i]['pct'] = array(
					'#type' => 'textfield', 
					'#field_prefix' => '<td>', 
					'#field_suffix' => '</td>', 
					'#default_value' => $data['beneficiaries'][$key][$i]['pct'],
					'#size' => 2,
				);
				$form['beneficiaries'][$key][$i]['outro'] = array('#markup' => '</tr>');
			}
			$form['beneficiaries'][$key]['outro'] = array('#markup' => '</table>');
		}

		$entity_json = sirius_json_get($entity_node);
		sirius_jsonfield_util_set_defaults($form['beneficiaries'], $entity_json['beneficiaries']);
		return $form;
	}

	public function form_validate($form, &$form_state) {
		foreach (array('primary' => 'Primary'/*, 'secondary' => 'Secondary'*/) as $key => $title) {
			$key_empty = TRUE;
			$total = 0;
			for ($i=0; $i<8; ++$i) {
				$row_empty = TRUE;
				foreach (array('name', 'ssn', 'phone', 'address', 'pct') as $subkey) {
					if ($form_state['values']['beneficiaries'][$key][$i][$subkey]) { 
						$row_empty = FALSE;
						$key_empty = FALSE;
					}
				}
				if ($row_empty) { continue; }

				if (!$form_state['values']['beneficiaries'][$key][$i]['name']) { form_set_error("beneficiaries][$key][$i][name", "Please enter a name."); }

				if (!$form_state['values']['beneficiaries'][$key][$i]['ssn']) { form_set_error("beneficiaries][$key][$i][ssn", "Please enter an SSN"); }
				else if (!sirius_ssn_validate($form_state['values']['beneficiaries'][$key][$i]['ssn'])) { form_set_error("beneficiaries][$key][$i][ssn", "Please enter a valid SSN"); }

				if (!$form_state['values']['beneficiaries'][$key][$i]['phone']) { form_set_error("beneficiaries][$key][$i][phone", "Please enter a phone number"); }
				else if (!sirius_phone_validate($form_state['values']['beneficiaries'][$key][$i]['phone'])) { form_set_error("beneficiaries][$key][$i][phone", "Please enter a valid phone number."); }

				if (!$form_state['values']['beneficiaries'][$key][$i]['address']) { form_set_error("beneficiaries][$key][$i][address", "Please enter an address"); }

				$pct = $form_state['values']['beneficiaries'][$key][$i]['pct'];
				if (!is_numeric($pct) || ($pct < 1) || $pct > 100) { form_set_error("beneficiaries][$key][$i][pct", "Please enter a valid percentage."); }
				else { $total += $pct; }
			}

			if (!$key_empty) {
				if ($total != 100) {
					form_set_error("beneficiaries][$key][0][pct", "Percentages must add up to 100.");
				}
			}
		}
	}

	public function form_submit($form, &$form_state) {
		$entity_node = node_load($form_state['entity_nid']);
		$entity_json = sirius_json_get($entity_node);
		$entity_json['beneficiaries'] = $form_state['values']['beneficiaries'];
		sirius_json_set($entity_node, $entity_json);
		node_save($entity_node);
	}

	public function render($beneficiaries) {
		if (!$beneficiaries) { return t('No beneficiaries found.'); }

		$header = array();
		$header[] = t('Name');
		$header[] = t('SSN');
		$header[] = t('Phone');
		$header[] = t('Address');
		$header[] = t('%');

		$rows = array();
		foreach (array('primary' => 'Primary'/*, 'secondary' => 'Secondary'*/) as $key => $title) {
			foreach ($beneficiaries[$key] as $beneficiary) {
				if (!$beneficiary) { continue; }
				$row = array();
				$row[] = $beneficiary['name'];
				$row[] = $beneficiary['ssn'];
				$row[] = $beneficiary['phone'];
				$row[] = $beneficiary['address'];
				$row[] = $beneficiary['pct'];
				if (!array_filter($row)) { continue; }
				$rows[] = $row;
			}
		}

		return sirius_table_render($header, $rows);
			
	}
}

