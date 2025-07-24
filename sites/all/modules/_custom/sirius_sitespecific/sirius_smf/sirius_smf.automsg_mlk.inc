<?php

function sirius_smf_automsg_mlk($mode = 'test', $force = FALSE) {
	return Sirius_Smf_Automsg_Mlk::getInstance()->scan($mode, $force);
}

function sirius_smf_automsg_wb_node_updateinsert($node) {
	return Sirius_Smf_Automsg_Mlk::getInstance()->wb_node_updateinsert($node);
}

class Sirius_Smf_Automsg extends Sirius_Singleton {
  public function info($result = array()) {
  	$result['multidomain'] = TRUE;
  	$result['schedule'] = 'hourly';
  	$result['title'] = $this->get_name();
    return parent::info($result);
  }

	public function msg_info_all() {
		return array();
	}

	public function msg_info($msg_key) {
		return $this->msg_info_all()[$msg_key];
	}

	public function msg_enabled($msg_key) {
		$cron_node = $this->get_cron_node();
		$domain_nid = sirius_domain_nid();
		$cron_json = sirius_json_get($cron_node);
		return $cron_json['smf'][$domain_nid]['automsg']['msgs'][$msg_key]['enabled'] == 'Yes';
	}

	public function get_cron_node() {
		return sirius_cron_scan_get($this->get_type(), TRUE);
	}

	public function nodequeue_name() {
		return 'sirius_smf_automsg_' . $this->type;
	}

	public function nodequeue() {
		$nodequeue_name = $this->nodequeue_name();
		$nodequeue = nodequeue_load_queue_by_name($nodequeue_name);

		if (!$nodequeue) {
		  $nodequeue = new stdClass();
		  $nodequeue->api_version = 2;
		  $nodequeue->name = $nodequeue_name;
		  $nodequeue->title = $this->name;
		  $nodequeue->subqueue_title = '';
		  $nodequeue->owner = 'nodequeue';
		  $nodequeue->show_in_ui = 1;
		  $nodequeue->show_in_tab = 0;
		  $nodequeue->show_in_links = 0;
		  $nodequeue->reference = '0';
		  $nodequeue->reverse = 1;
		  $nodequeue->i18n = 0;
		  $nodequeue->roles = array();
		  $nodequeue->types = array('sirius_worker');
		  $nodequeue->size = 0;
		  $nodequeue->unique_entries = TRUE;
		  nodequeue_save($nodequeue);
		  nodequeue_add_subqueue($nodequeue, 'main');
		}

		return $nodequeue;
	}

	public function subqueue($nodequeue = NULL) {
		if (!$nodequeue) { $nodequeue = $this->nodequeue(); }
		return reset(nodequeue_load_subqueues_by_queue($nodequeue->qid));
	}

	public function nodequeue_enqueue($nid) {
		return nodequeue_subqueue_add($this->nodequeue(), $this->subqueue(), $nid);
	}

	public function nodequeue_dequeue() {
		$subqueue = $this->subqueue();
    $node = nodequeue_load_front($subqueue->sqid);
    if (!$node) { return NULL; }
    nodequeue_subqueue_remove_node($subqueue->sqid, $node->nid);
    return $node;
	}
}

class Sirius_Smf_Automsg_Mlk extends Sirius_Smf_Automsg {
	public $type = 'smf_automsg_mlk';
	public $name = 'SMF Auto-Message MLK';
	public $description = 'Automatically send welcome messages to MLK members';

  public function info($result = array()) {
    $result['callback'] = 'sirius_smf_automsg_mlk';
    return parent::info($result);
  }

	public function wb_node_updateinsert($wb_node) {
		// Only interested in WB nodes
		if ($wb_node->type != 'sirius_trust_worker_benefit') { return; }

		// Only interested in our benefit
		$result = $this->benefit_lookup();
		if (!$result['success']) { return $result; }
		$benefit_node = $result['benefit_node'];
		if ($benefit_node->nid != $wb_node->field_sirius_trust_benefit['und'][0]['target_id']) { return; }

		// Only interested in active nodes. 
		if ($wb_node->field_sirius_active['und'][0]['value'] != 'Yes') { return; }

		// Only interested in "self". This is probably redundant, because we queue the subscriber anyway.
		if ($wb_node->field_sirius_contact_relation['und'][0]['target_id']) { return; }

		// Yay. Queue up the subscriber node
		$this->nodequeue_enqueue($wb_node->field_sirius_trust_subscriber['und'][0]['target_id']);
	}

	public function scan($mode, $force = FALSE) {
		// Debugging
		//$mode = 'test';
		//sirius_minilog("Because this is still under development, forcing mode = $mode.");

		// Do Not Disturb. We do this for individual messages, but let's also do it at the scan level so
		// we don't have to try each individual record.
		if (!$force) {
			$result = sirius_do_not_disturb_check('mlk');
			if (!$result['success']) { return $result; } 
		}
		
		// Look up our benefit
		$result = $this->benefit_lookup();
		if (!$result['success']) { return $result; }
		$benefit_node = $result['benefit_node'];

    $limit = 10;
    $total = $this->subqueue()->count;
    sirius_minilog("There are currently $total workers who might receive a message. Processing the first $limit.");
    if ($limit > $total) { $limit = $total; }

		$msg_info_all = $this->msg_info_all();

    for ($i=0; $i<$limit; ++$i) {
    	$worker_node = $this->nodequeue_dequeue();
    	if (!$worker_node) { break; }
    	sirius_minilog("Processing worker " . ($i+1) . " / $limit: " . $worker_node->title);

    	$requeue = FALSE;
    	foreach ($msg_info_all as $msg_key => $msg_info) {
    		sirius_minilog("... Attempting delivery of $msg_key ($msg_info[title]) to " . $worker_node->title);
    		$result = $this->deliver($msg_key, $worker_node, $mode);
    		if (!$result['success']) {
    			sirius_minilog("... ... " . $result['msg']);
    		}
    		if ($result['requeue']) {
    			sirius_minilog("... ... Adding back to the queue.");
    			$requeue = TRUE;
    		}
    	}
    	if ($requeue) { $this->nodequeue_enqueue($worker_node->nid); }
    }

		return array('success' => TRUE, 'count' => $limit);
	}

	public function msg_info_all() {
		return array(
			'en01' => array(
				'title' => 'English 01',
				'description' => 'Triggered upon enrollment in MLK (English speaker)',
				'lang' => 'en',
				'msg' => array(
					'body' => "Welcome to MLK Care! Your health is our priority. Access benefits & support tailored for you. Questions? Call (213) 456-2012 and leave a message, or go to https://mlkcare.org/. Stay tuned for more info regarding your MLK Benefits - MLK Care Team",
				),
				'sms_attach' => array('url' => array('https://sirius.benefits11.org/sites/all/modules/_custom/sirius_sitespecific/sirius_smf/images/mlk_welcome_text_graphic.jpg')),
			),
			'en02' => array(
				'title' => 'English 02',
				'description' => 'Triggered 1 month after the first message, if still enrolled in MLK (English speaker)',
				'lang' => 'en',
				'msg' => array(
					'body' => 'MLK Care members: Get your yearly primary care check-up and you will receive a check for $250! Up to $500 with your spouse! Call to make an appointment: (424) 529-6755. Processing time for checks is around 3 months. Call (213) 456-2012 with questions or if you haven’t received your check yet. - MLK Care Team',
				),
				'prior' => 'en01',
			),
			'en03' => array(
				'title' => 'English 03',
				'lang' => 'en',
				'msg' => array(
					'body' => 'MLK Care members: Are you looking for an Urgent Care Center? Click here https://mlkcare.org/wp-content/uploads/2024/04/MLK-Care-Locations-Eng-Flyer.pdf for a list of Urgent Care centers in network or call (213) 456-2012 for assistance. FYI, the LA County Urgent Care Clinic at MLK is NOT in network. – MLK Care Team',
				),
				'prior' => 'en02',
			),
			'en04' => array(
				'title' => 'English 04',
				'lang' => 'en',
				'msg' => array(
					'body' => 'MLK Care members: Make your primary care doctor appointment today! Your primary care centers are located at 2251 W. Rosecrans, and 135 E. Compton Blvd. in Compton. Call to make an appointment: (424) 529-6755',
				),
				'prior' => 'en03',
			),
			'en05' => array(
				'title' => 'English 05',
				'lang' => 'en',
				'msg' => array(
					'body' => 'MLK Members: Need medication? Take advantage of your Express Scripts card and use it at an in-network pharmacy. Find your closest pharmacy here or call us at (213) 456-2012. Most pharmacies are in network, except for Walgreen’s. - MLK Care Team',
				),
				'prior' => 'en04',
			),
			'en06' => array(
				'title' => 'English 06',
				'lang' => 'en',
				'msg' => array(
					'body' => 'MLK Care members: Your vision is important. Now you can get your eye exam scheduled with VSP. No ID card required. Simply dial (800) 877-7195 or click here. - MLK Care Team    https://www.vsp.com/eye-doctor?sortByPremier=true',
				),
				'prior' => 'en05',
			),
			'en07' => array(
				'title' => 'English 07',
				'lang' => 'en',
				'msg' => array(
					'body' => 'MLK Care members: Introducing Carelon, a new option for all of your Mental Health and Substance Use Disorder care. In person, telehealth, and text. Call (888) 479-6606. No referral required! - MLK Care Team',
				),
				'prior' => 'en06',
			),
			'en08' => array(
				'title' => 'English 08',
				'lang' => 'en',
				'msg' => array(
					'body' => 'MLK Care members: Physical therapy available online, at your convenience! Hinge Health can help conquer back and joint pain, recover from an injury, and reduce stiffness in achy joints all at no cost. Enroll today at hinge.health/smunitehere-enroll',
				),
				'prior' => 'en07',
			),
			'en09' => array(
				'title' => 'English 09',
				'lang' => 'en',
				'msg' => array(
					'body' => 'Get personalized exercise therapy for pregnancy and postpartum, pelvic muscle strengthening and more, at no cost with Hinge Health. Enroll today at hinge.health/smunitehere-enrollwph',
				),
				'prior' => 'en08',
			),
			'en10' => array(
				'title' => 'English 10',
				'lang' => 'en',
				'msg' => array(
					'body' => 'Need help with stress, family problems, depression, finances? Carelon can help! Go to their website today! https://www.carelonwellbeing.com/santamonicauhh Video: https://vimeo.com/875233503/efbeaba9c3?share=copy',
				),
				'prior' => 'en09',
				'last_msg' => TRUE,
			),

			'es01' => array(
				'title' => 'Spanish 01',
				'description' => 'Triggered upon enrollment in MLK (Spanish speaker)',
				'lang' => 'es',
				'msg' => array(
					'body' => "Bienvenido/a a MLK Care! Su salud es nuestra prioridad. Acceda a beneficios y apoyo personalizados para usted. ¿Preguntas? Llame al (213) 456-2012 y deje un mensaje, o vaya a https://mlkcare.org/. Manténgase al tanto para más información sobre sus Beneficios MLK. - Equipo de MLK Care",
				),
				'sms_attach' => array('url' => array('https://sirius.benefits11.org/sites/all/modules/_custom/sirius_sitespecific/sirius_smf/images/mlk_bienvenido_text_graphic.jpg')),
			),
			'es02' => array(
				'title' => 'Spanish 02',
				'description' => 'Triggered 1 month after the first message, if still enrolled in MLK (Spanish speaker)',
				'lang' => 'es',
				'msg' => array(
					'body' => 'Miembros de MLK Care: ¡Realice su revisión médica anual de atención primaria y recibirá un cheque de $250! ¡Hasta $500 con su cónyuge! Llame para hacer una cita; (424) 529-6755. El tiempo de procesamiento para los cheques es de alrededor de 3 meses. Llame al (213) 456-2012 si tiene preguntas o si aún no ha recibido su cheque. - Equipo de MLK Care',
				),
				'prior' => 'es01',
			),
			'es03' => array(
				'title' => 'Spanish 03',
				'lang' => 'es',
				'msg' => array(
					'body' => 'Miembros de MLK Care: ¿Está buscando un Centro de Atención de Urgencia? Presione aqui para ver una lista de centros de atención de urgencia en la red o llame al (213) 456-2012 para recibir ayuda. Atencion, la Clínica de Atención de Urgencia del Condado de Los Ángeles en MLK NO está en la red. - Equipo de MLK Care',
				),
				'prior' => 'es02',
			),
			'es04' => array(
				'title' => 'Spanish 04',
				'lang' => 'es',
				'msg' => array(
					'body' => 'Miembros de MLK Care: ¡Haga su cita con el médico de atención primaria hoy! Sus centros de atención primaria están ubicados en 2251 W. Rosecrans y 135 E. Compton Blvd. en Compton. Llame para programar una cita: (424) 529-6755',
				),
				'prior' => 'en03',
			),
			'es05' => array(
				'title' => 'Spanish 05',
				'lang' => 'es',
				'msg' => array(
					'body' => 'Miembros de MLK: ¿Necesitan medicina? Aprovechen su tarjeta de Express Scripts y úsenla en una farmacia dentro de la red de farmacias. Encuentre aquí su farmacia más cercana o llámenos al (213) 456-2012. La mayoría de las farmacias están dentro de la red, excepto Walgreen\'s.- Equipo de MLK Care',
				),
				'prior' => 'es04',
			),
			'es06' => array(
				'title' => 'Spanish 06',
				'lang' => 'es',
				'msg' => array(
					'body' => 'Miembros de MLK Care: Su visión es importante. Ahora puede programar su examen de la vista con VSP. No se requiere tarjeta de identificación. Simplemente marque al (800) 877-7195 o haga clic aqui.- Equipo de MLK Care https://www.vsp.com/eye-doctor?sortByPremier=true',
				),
				'prior' => 'es05',
			),
			'es07' => array(
				'title' => 'Spanish 07',
				'lang' => 'es',
				'msg' => array(
					'body' => 'Miembros de MLK Care: Les presentamos Carelon, una nueva opción para toda su atención en Salud Mental y Trastornos por Uso de Sustancias. Ofrecemos servicios en persona, telemedicina y mensajes de texto. Llamen al (888) 479-6606. ¡No se requiere referencia! - Equipo de MLK Care',
				),
				'prior' => 'es06',
			),
			'es08' => array(
				'title' => 'Spanish 08',
				'lang' => 'es',
				'msg' => array(
					'body' => 'Miembros de MLK Care: Fisioterapia disponible en línea, a su conveniencia. Hinge Health puede ayudar a superar el dolor de espalda y articulaciones, recuperarse de una lesión y reducir la rigidez en las articulaciones doloridas, todo sin costo alguno. Inscríbete hoy en hinge.health/smunitehere-enroll',
				),
				'prior' => 'es07',
			),
			'es09' => array(
				'title' => 'Spanish 09',
				'lang' => 'es',
				'msg' => array(
					'body' => 'Obtén ejercicios terapéuticos personalizados para el embarazo y posparto, fortalecimiento muscular pélvico y más, sin costo alguno con Hinge Health. Inscríbete hoy en hinge.health/smunitehere-enrollwph',
				),
				'prior' => 'es08',
			),
			'es10' => array(
				'title' => 'Spanish 10',
				'lang' => 'es',
				'msg' => array(
					'body' => '¿Necesita ayuda con el estrés, problemas familiares, depresión, finanzas? ¡Carelon puede ayudar! ¡Vaya a su sitio web hoy! https://www.carelonwellbeing.com/santamonicauhh Video: https://vimeo.com/875233503/efbeaba9c3?share=copy',
				),
				'prior' => 'es09',
				'last_msg' => TRUE,
			),
		);
	}

	public function unique_id($worker_nid, $msg_key) {
		return join('|', array(
			$worker_nid,
			'sms_automsg',
			$this->get_type(),
			$msg_key
		));
	}

	public function benefit_lookup() {
		$benefit_id = 'M';
		$benefit_node = sirius_trust_benefit_get_by_id($benefit_id);
		if (!$benefit_node) { return array('success' => FALSE, 'msg' => "There is no benefit with ID $benefit_id."); }
		if (!preg_match('/mlk/i', $benefit_node->title)) {
			return array('success' => FALSE, 'msg' => "I found a benefit with ID $benefit_id, but its name is " . $benefit_node->title . ". I had expected the name to include the string 'mlk'. Just to be on the safe side, I'm not going to send this message until you check with the programmer.");
		}

		return array('success' => TRUE, 'benefit_node' => $benefit_node);
	}

	public function deliver_from_worker_id($msg_key, $worker_id, $mode, $options = array()) {
		// Look up the worker
		if (!$worker_id) { return array('success' => FALSE, 'msg' => "Missing worker_id.", 'requeue' => FALSE); }
		$worker_node = sirius_worker_get_by_id($worker_id);
		if (!$worker_node) { return array('success' => FALSE, 'msg' => "Worker $worker_id not found.", 'requeue' => FALSE); }
		return $this->deliver($msg_key, $worker_node, $mode, $options);
	}

	public function deliver($msg_key, $worker_node, $mode, $options = array()) {
		// Get the message info
		$msg_info = $this->msg_info($msg_key);
		if (!$msg_info) { 
			return array(
				'success' => FALSE, 
				'msg' => "Invalid message key ($msg_key)", 
				'requeue' => FALSE,
			);
		}

		$options['msg_params']['handler_nids'][] = $worker_node->nid;

		// Generate a unique ID, and don't send twice
		$unique_id = $this->unique_id($worker_node->nid, $msg_key);
		$options['msg_params']['X-Sirius-Id'] = $unique_id;
		if ($this->already_sent($unique_id)) {
			if ($options['force']) {
				unset($options['msg_params']['X-Sirius-Id']);
			} else {
				return array(
					'success' => FALSE, 
					'msg' => "Already sent this message to this worker ($unique_id)",
					'requeue' => FALSE,
				);
			}
		}

		// Look up the contact
		$contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);
		if (!$contact_node) {
			return array(
				'success' => FALSE, 
				'msg' => 'Worker ' . $worker_node->title . ' does not have a contact.', 
				'requeue' => FALSE
			);
		}

		// Look up the phone
		$phone = $contact_node->field_sirius_phone['und'][0]['value'];
		if (!$phone) {
			return array(
				'success' => FALSE, 
				'msg' => "Worker " . $worker_node->title . " does not have a phone number.",
				'requeue' => FALSE
			);
		}

		// Check the message language
		if (!$options['force']) {
			$lang = sirius_contact_lang_primary($contact_node);
			if ($msg_info['lang'] != $lang) {
				return array(
					'success' => FALSE, 
					'msg' => "The worker's primary language is [ $lang ], but this message has the langauge [ $msg_info[lang] ].",
					'requeue' => FALSE,
				);
			}
		}

		// Check the benefit 
		$result = $this->benefit_lookup();
		if (!$result['success']) { return $result; }
		$benefit_node = $result['benefit_node'];

		// Make sure that the worker currently has MLK
		$search_params = array();
		$search_params['worker_nid'] = $worker_node->nid;
		$search_params['active'] = 'Yes';
		$search_params['benefit_nids'] = array($benefit_node->nid);
		$search_params['nids_only'] = TRUE;
		$wb_nids = sirius_trust_worker_benefits_search($search_params);
		if (!$wb_nids) {
			return array(
				'success' => FALSE, 
				'requeue' => FALSE,
				'msg' => 'Worker ' . $worker_node->title . ' does not currently have the MLK benefit.'
			);
		}
		// $wb_nid = reset($wb_nids);
		// $wb_node = node_load($wb_nid);

		// Make sure we've sent the preceding message
		if (!$options['force']) {
			$prior_msg_key = $msg_info['prior'];

			if ($prior_msg_key) {
				$prior_unique_id = $this->unique_id($worker_node->nid, $prior_msg_key);
				$prior_record = $this->already_sent($prior_unique_id);
				if (!$prior_record) {
					return array(
						'success' => FALSE, 
						'msg' => "Unable to send message [$msg_key] because we haven't yet sent [$prior_msg_key].",
						'requeue' => TRUE,
					);
				}

				$cutoff = strtotime('-1 week');
				if ($prior_record['created'] > $cutoff) {
					return array(
						'success' => FALSE, 
						'msg' => "The previous message ($prior_msg_key) was sent on " . date('m/d/Y H:i:s T', $prior_record['created']) . ", only " . format_interval(time() - $prior_record['created'], 2) . " ago. We cannot send message $msg_key until 1 week after message $prior_msg_key.",
						'requeue' => TRUE,
					);
				}
			}
		}

		return $this->deliver_raw($msg_key, $phone, $mode, $options);
	}

	public function deliver_raw($msg_key, $phone, $mode, $options = array()) {
		if (!$options['force']) {
			$result = sirius_do_not_disturb_check('mlk');
			if (!$result['success']) {
				$result['requeue'] = TRUE;
				return $result;
			}
		}

		if (!sirius_phone_validate($phone)) {
			return array(
				'success' => FALSE, 
				'msg' => "Invalid phone number ($phone).",
				'requeue' => FALSE
			);
		}

		$msg_info = $this->msg_info($msg_key);
		if (!$msg_info) {
			return array(
				'success' => FALSE, 
				'msg' => "Invalid message key ($msg_key)",
				'requeue' => FALSE,
			);
		}

		if (!$options['force']) {
			if (!$this->msg_enabled($msg_key)) {
				return array(
					'success' => FALSE, 
					'msg' => "Message is disabled: $info[title] ($msg_key)",
					'requeue' => FALSE,
				);
			}
		}

		$params = $options['msg_params'];
		$params['to'] = $phone;
		$params['sms'] = $msg_info['msg']['body'];
		if ($msg_info['sms_attach']) { $params['sms_attach'] = $msg_info['sms_attach']; }
		$params['automsg'] = array(
			'type' => $this->type,
			'key' => $msg_key,
		);

		if ($mode == 'live') {
			return sirius_sms_send($params);
		} else {
			return array(
				'success' => TRUE, 
				'msg' => "[Skipping delivery because mode = $mode]",
				'requeue' => FALSE,
			);
		}
	}

	public function already_sent($id) {
		// @todo: when we have the new Comm framework, we'd really prefer to do this:
		// return Sirius_Comm_Sms::get_instance()->already_sent($id);
		// But we don't have the comm framework, and we want the actual prior delivery record itself, so we copy and paste.

		if (!$id) { return FALSE; }

    // SQL for speed
    $sql = "select * from node ";
    $sql .= "join field_data_field_sirius_notes on field_data_field_sirius_notes.entity_type = 'node' and field_data_field_sirius_notes.entity_id = node.nid ";
    $sql .= "join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
    $sql .= "join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
    $sql .= "where node.type = 'sirius_log' ";
    $sql .= "and field_sirius_notes_value = :id ";
    $sql .= "and field_sirius_category_value = :type ";
    $sql .= "and field_sirius_type_value in ('sending', 'sent', 'failed') ";
    $sql .= "order by created desc limit 1 ";
    $stmt = db_query($sql, array(':id' => $id, ':type' => 'sms'));
    return $stmt->fetchAssoc();
  }
}
