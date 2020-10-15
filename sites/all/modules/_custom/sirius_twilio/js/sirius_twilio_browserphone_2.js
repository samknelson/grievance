(function($) { $(document).ready(function () {

	$("#sirius_browserphone_tabs").tabs();

	//
	// Settings
	// 

	token = Drupal.settings.sirius_twilio_browserphone.token;
	capability_token = token.split('|')[0];
	event_nid = Drupal.settings.sirius_twilio_browserphone.event_nid;
	default_phone = Drupal.settings.sirius_twilio_browserphone.default_phone;
	if (default_phone) { $('#sirius_browserphone_number').val(default_phone); }
	callerid_options = Drupal.settings.sirius_twilio_browserphone.callerid_options;
	omg_options = Drupal.settings.sirius_twilio_browserphone.omg_options;
	console.log(Drupal.settings.sirius_twilio_browserphone);

	var connection;

	//
	// Setup Twilio.Device
	//
	
	var device;
	device = new Twilio.Device(capability_token);
	$('#sirius_browserphone_button_hangup').hide();
	$('#sirius_browserphone_button_omg').hide();
	$('#sirius_browserphone_omg').hide();
	$('#sirius_browserphone_button_conference').hide();
	$('#sirius_browserphone_button_conference_add').hide();
	$('#sirius_browserphone_button_conference_remove').hide();

	device.on('ready',function (device) {
		sirius_browserphone_log('Sirius Phone Ready.');
	});

	device.on('error', function (error) {
		sirius_browserphone_log('Sirius Phone Error: ' + error.message);
	});

	device.on('connect', function (conn) {
		sirius_browserphone_log('Call established.');
		$('#sirius_browserphone_button_call').hide();
		$('#sirius_browserphone_button_hangup').show();
		$('#sirius_browserphone_button_omg').show();
		$('#sirius_browserphone_omg').show();
		$('#sirius_browserphone_button_conference').show();
		sirius_browserphone_volume_bind(conn);
	});

	device.on('disconnect', function (conn) {
		sirius_browserphone_log('Call ended.');
		$('#sirius_browserphone_button_call').show();
		$('#sirius_browserphone_button_hangup').hide();
		$('#sirius_browserphone_button_omg').hide();
		$('#sirius_browserphone_omg').hide();
		$('#sirius_browserphone_button_conference').hide();
		$('#sirius_browserphone_button_conference_add').hide();
		$('#sirius_browserphone_button_conference_remove').hide();
		Drupal.settings.sirius_twilio_browserphone.connection = null;
	});

	device.on('incoming', function (conn) {
		sirius_browserphone_log('Incoming connection from ' + conn.parameters.From);
		conn.accept();
	});

	device.audio.on('deviceChange', sirius_browserphone_devices_refresh);

	//
	// Mic and Speaker
	// 

	$('#sirius_browserphone_button_devices_refresh').click(function() {
		navigator.mediaDevices.getUserMedia({audio: true}).then(sirius_browserphone_devices_refresh);
		return false;
	});

	$('#sirius_browserphone_devices').change(function() {
		device.audio.speakerDevices.set($('#sirius_browserphone_devices').val());
		device.audio.ringtoneDevices.set($('#sirius_browserphone_devices').val());
	});

	// 
	// Caller IDs
	//

	for (key in callerid_options) {
		val = callerid_options[key];
		$('#sirius_browserphone_callerid').append(new Option(val, key));
	}

	// 
	// OMGs
	//

	if (omg_options) {
		for (key in omg_options) {
			val = omg_options[key];
			$('#sirius_browserphone_omg').append(new Option(val, key));
		}
	}

	//
	// Bind buttons
	// 

	$('#sirius_browserphone_button_call').click(function() {
		var params = {
	  	To: $('#sirius_browserphone_number').val(),
	  	token: token,
	  	callerid_nid: $('#sirius_browserphone_callerid').val(),
		};
		Drupal.settings.sirius_twilio_browserphone.connection = device.connect(params);
		return false;
	});

	// Bind button to hangup call
	$('#sirius_browserphone_button_hangup').click(function() {
		sirius_browserphone_log('Hanging up...');
		device.disconnectAll();
		return false;
	});

	$('#sirius_browserphone_button_omg').click(function () {
		sirius_browserphone_log('Transferring to OMG...');
		url = '/sirius/twilio/browserphone/twiml/omg?token=' + token + '&omg_key=' + $('#sirius_browserphone_omg').val() + '&phone=' + $('#sirius_browserphone_number').val();
		$.ajax({url: url}).done(function(data) { 
			sirius_browserphone_log('... ' + data);
			if (data == 'Ok.') {
				device.disconnectAll();
				// @todo: 
				//   I don't want to use eval(), and I can't be fussed with dynamically generated callbacks. So we're doing this in the most
				//   inelegant way possible.
				callback_js = Drupal.settings.sirius_twilio_browserphone.omg.callback_js;
				callback_args = Drupal.settings.sirius_twilio_browserphone.omg.callback_js_args;
				if (callback_js == 'sirius_event_phonebank_set_all_pstatus') { sirius_event_phonebank_set_all_pstatus(callback_args); }
			}
		});


		return false;
	});

	$('#sirius_browserphone_button_conference').click(function () {
		sirius_browserphone_log('Joining conference...');

		// 1. Put the outgoing leg (the child call) into the conference.
		Drupal.settings.sirius_twilio_browserphone.conference_name = sirius_browserphone_conference_name();
		twiml = '<Response><Say>Entering the conference</Say><Dial><Conference endConferenceOnExit="true">' + Drupal.settings.sirius_twilio_browserphone.conference_name + '</Conference></Dial></Response>';
		url = '/sirius/twilio/browserphone/twiml/echo?token=' + token + '&twiml=' + encodeURIComponent(twiml);
		$.ajax({url: url}).done(function(data) { 
			sirius_browserphone_log('... ' + data);
			if (data == 'Ok.') {
				sirius_browserphone_log('Callee connected');
			}
		});

		twiml = '<Response><Dial><Conference endConferenceOnExit="true">' + Drupal.settings.sirius_twilio_browserphone.conference_name + '</Conference></Dial></Response>';
		url = '/sirius/twilio/browserphone/twiml/echo?token=' + token + '&twiml=' + encodeURIComponent(twiml) + '&parent=1';
		$.ajax({url: url}).done(function(data) { 
			sirius_browserphone_log('... ' + data);
			if (data == 'Ok.') {
				// device.disconnectAll(); 
				sirius_browserphone_log('Caller connected');
			}
		});

		$('#sirius_browserphone_button_conference_add').show();
		$('#sirius_browserphone_button_conference').hide();

		return false;
	});

	$('#sirius_browserphone_button_conference_add').click(function () {
		sirius_browserphone_log("Adding participant ...");
		url = '/sirius/twilio/browserphone/twiml/conference/add?token=' + token + '&conference_name=' + Drupal.settings.sirius_twilio_browserphone.conference_name;
		$.ajax({url: url}).done(function(data) { 
			sirius_browserphone_log('... ' + data);
			if (data == 'Ok.') { 
				sirius_browserphone_log("Participant added.");
				$('#sirius_browserphone_button_conference_add').hide();
				$('#sirius_browserphone_button_conference_remove').show();
			}
		});

		return false;
	});

	$('#sirius_browserphone_button_conference_remove').click(function () {
		sirius_browserphone_log("Removing participant ...");
		twiml = '<Response><Hangup/></Response>';
		url = '/sirius/twilio/browserphone/twiml/echo?token=' + token + '&twiml=' + encodeURIComponent(twiml) + '&phone=' + encodeURIComponent(Drupal.settings.sirius_twilio_browserphone.conference_number); 
		$.ajax({url: url}).done(function(data) { 
			sirius_browserphone_log('... ' + data);
			if (data == 'Ok.') {
				sirius_browserphone_log("Participant removed.");
				$('#sirius_browserphone_button_conference_add').show();
				$('#sirius_browserphone_button_conference_remove').hide();
			}
		});

		return false;
	});


	$('#sirius_browserphone_button_keypad_1').click(function () { return sirius_browserphone_keypad('1'); });
	$('#sirius_browserphone_button_keypad_2').click(function () { return sirius_browserphone_keypad('2'); });
	$('#sirius_browserphone_button_keypad_3').click(function () { return sirius_browserphone_keypad('3'); });
	$('#sirius_browserphone_button_keypad_4').click(function () { return sirius_browserphone_keypad('4'); });
	$('#sirius_browserphone_button_keypad_5').click(function () { return sirius_browserphone_keypad('5'); });
	$('#sirius_browserphone_button_keypad_6').click(function () { return sirius_browserphone_keypad('6'); });
	$('#sirius_browserphone_button_keypad_7').click(function () { return sirius_browserphone_keypad('7'); });
	$('#sirius_browserphone_button_keypad_8').click(function () { return sirius_browserphone_keypad('8'); });
	$('#sirius_browserphone_button_keypad_9').click(function () { return sirius_browserphone_keypad('9'); });
	$('#sirius_browserphone_button_keypad_0').click(function () { return sirius_browserphone_keypad('0'); });
	$('#sirius_browserphone_button_keypad_s').click(function () { return sirius_browserphone_keypad('*'); });
	$('#sirius_browserphone_button_keypad_n').click(function () { return sirius_browserphone_keypad('#'); });

	function sirius_browserphone_keypad(s) {
		if (Drupal.settings.sirius_twilio_browserphone.connection) {
			Drupal.settings.sirius_twilio_browserphone.connection.sendDigits(s);
		}
		return false;
	};


	//
	// Utility functions
	//

	function sirius_browserphone_volume_bind(connection) {
		connection.volume(function(inputVolume, outputVolume) {
			var inputColor = 'red';
			if (inputVolume < .50) { inputColor = 'green'; } 
			else if (inputVolume < .75) { inputColor = 'yellow'; }

			var outputColor = 'red';
			if (outputVolume < .50) { outputColor = 'green'; }
			else if (outputVolume < .75) { outputColor = 'yellow'; }

			$('#sirius_browserphone_volume_input').css('width', Math.floor(inputVolume * 200) + 'px');
			$('#sirius_browserphone_volume_input').css('background-color', inputColor);

			$('#sirius_browserphone_volume_output').css('width', Math.floor(outputVolume * 200) + 'px');
			$('#sirius_browserphone_volume_output').css('background-color', outputColor);
		});
	}

	function sirius_browserphone_devices_refresh() {
		$('#sirius_browserphone_devices').empty();
		device.audio.availableOutputDevices.forEach(function(device, id) {
			$('#sirius_browserphone_devices').append(new Option(device.label, id));
	  });
	}

	function sirius_browserphone_log(message) {
	  $('#sirius_browserphone_log').append(message + '<br />');
	  console.log(message);
	}

	function sirius_browserphone_conference_name() {
		name = Drupal.settings.sirius_twilio_browserphone.user.name;
		name += Date.now();
		return name;
	}

	//
	// @todo Functions that shouldn't be in this file.
	// 

	function sirius_event_phonebank_set_all_pstatus(args) {
		pstatus_tid = args[0];
		$('#set_all_pstatus_tid').val(pstatus_tid);
		$('#set_all_pstatus_submit').click();
	}

}); } (jQuery));
