(function($) {
	$(document).ready(function () {

 		$("#sirius_browserphone_tabs").tabs();

		//
		// Settings
		// 

		token = Drupal.settings.sirius_twilio_browserphone.token;
		event_nid = Drupal.settings.sirius_twilio_browserphone.event_nid;
		default_phone = Drupal.settings.sirius_twilio_browserphone.default_phone;
		if (default_phone) { $('#sirius_browserphone_number').val(default_phone); }
		callerid_options = Drupal.settings.sirius_twilio_browserphone.callerid_options;
		omg_options = Drupal.settings.sirius_twilio_browserphone.omg_options;

		//
		// Setup Twilio.Device
		//
		
		var device;
		device = new Twilio.Device(token);
		$('#sirius_browserphone_button_hangup').hide();
		$('#sirius_browserphone_button_omg').hide();
		$('#sirius_browserphone_omg').hide();

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
			$('#sirius_browserphone_volume').show();
			sirius_browserphone_volume_bind(conn);
		});

		device.on('disconnect', function (conn) {
			sirius_browserphone_log('Call ended.');
			$('#sirius_browserphone_button_call').show();
			$('#sirius_browserphone_button_hangup').hide();
			$('#sirius_browserphone_button_omg').hide();
			$('#sirius_browserphone_omg').hide();
			$('#sirius_browserphone_volume').hide();
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
		  	callerid_nid: $('#sirius_browserphone_callerid').val()
			};
			device.connect(params);
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
			url = '/sirius/twilio/browserphone/twiml/omg?token=' + token + '&omg_key=' + $('#sirius_browserphone_omg').val();
			$.ajax({url: url}).done(function(data) { 
				sirius_browserphone_log('... ' + data);
				if (data == 'Ok.') {
					device.disconnectAll(); 
				}
			});
			return false;
		});

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

				$('#sirius_browserphone_volume_input').css('width', Math.floor(inputVolume * 300) + 'px');
				$('#sirius_browserphone_volume_input').css('background-color', inputColor);

				$('#sirius_browserphone_volume_output').css('width', Math.floor(outputVolume * 300) + 'px');
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
		}

	});
}(jQuery));
