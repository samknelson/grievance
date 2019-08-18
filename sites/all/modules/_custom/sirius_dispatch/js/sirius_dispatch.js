(function($) { $(document).ready(function () {

	current_conversation_nid = 0;
	current_dispatch_nid = 0;

	// Refresh immediately and every five seconds forever
	sirius_dispatch_operator_set_listeners();
	sirius_dispatch_operator_refresh();
	setInterval(sirius_dispatch_operator_refresh, 5000);

	// Listeners
	function sirius_dispatch_operator_set_listeners() {
		$('#sirius_dispatch_operator_refresh').unbind('click').click(function(event) {
			event.preventDefault(); sirius_dispatch_operator_refresh();
		});

		$('.sirius_dispatch_operator_dispatch_details_link').unbind('click').click(function(event) {
			event.preventDefault(); sirius_dispatch_operator_dispatch_details($(this).attr("data-dispatch-nid"), $(this).attr("data-conversation-nid"));
		});

		$('.sirius_dispatch_operator_dispatch_notify_sms_link').unbind('click').click(function(event) {
			event.preventDefault(); sirius_dispatch_operator_dispatch_notify($(this).attr("data-dispatch-nid"), $(this).attr("data-conversation-nid"), 'sms');
		});

		$('.sirius_dispatch_operator_dispatch_notify_voice_link').unbind('click').click(function(event) {
			event.preventDefault(); sirius_dispatch_operator_dispatch_notify($(this).attr("data-dispatch-nid"), $(this).attr("data-conversation-nid"), 'voice');
		});

		$('.sirius_dispatch_operator_dispatch_dismiss_link').unbind('click').click(function(event) {
			event.preventDefault(); sirius_dispatch_operator_dispatch_dismiss($(this).attr("data-dispatch-nid"), $(this).attr("data-conversation-nid"));
		});

		$('.sirius_dispatch_operator_dispatch_call_link').unbind('click').click(function(event) {
			event.preventDefault(); sirius_dispatch_operator_call(event);
		});
	}

	// 
	// Notify 
	//

	function sirius_dispatch_operator_dispatch_notify(dispatch_nid, conversation_nid, medium) {
		$('#sirius_dispatch_operator_status').html('... Sending notification ...');

		$.ajax({
        	'url': '/sirius/dispatch/operator/ajax/notify?dispatch_nid=' + dispatch_nid + '&medium=' + medium,
        	'type': 'GET',
			'success': function(data) {
				sirius_dispatch_operator_refresh();
        	}
        });

		$('#sirius_dispatch_operator_status').html('Notification sent (' + medium + ')');
	}

	// 
	// Details
	//

	function sirius_dispatch_operator_dispatch_details(dispatch_nid, conversation_nid) {
		console.log("Fetching details for " + dispatch_nid + " / " + conversation_nid);
		if (current_conversation_nid != conversation_nid) {
			// $('#sirius_dispatch_operator_status').html('Loading conversation');
		}
		current_conversation_nid = conversation_nid;
		current_dispatch_nid = dispatch_nid;

		$.ajax({
        	'url': '/sirius/dispatch/operator/ajax/dispatch_details?dispatch_nid=' + dispatch_nid,
        	'type': 'GET',
			'success': function(data) {
            	$('#sirius_dispatch_operator_dispatch').html(data);
				sirius_dispatch_operator_set_listeners();
				$("#sirius_dispatch_operator_dispatch_tabs").tabs();
				$('.sirius_dispatch_queue_entry').removeClass('active');
				$('.sirius_dispatch_queue_entry_' + conversation_nid).addClass('active');
        	}
        });
	}

	// 
	// Dismiss
	//

	function sirius_dispatch_operator_dispatch_dismiss() {
		current_conversation_nid = 0;
		current_dispatch_nid = 0;
		$('.sirius_dispatch_queue_entry').removeClass('active');
		$('#sirius_dispatch_operator_dispatch').html('No dispatch selected.');
	}

	//
	// Refresh the screen
	//

	function sirius_dispatch_operator_refresh() {
		console.log("Refreshing");
		$('#sirius_dispatch_operator_refresh_status').html('... Refreshing ...');

		$.ajax({
        	'url': '/sirius/dispatch/operator/ajax/queue',
        	'type': 'GET',
			'success': function(data) {
            	$('#sirius_dispatch_operator_queue').html(data);
				sirius_dispatch_operator_set_listeners();
				$('.sirius_dispatch_queue_entry').removeClass('active');
				if (current_conversation_nid) {
					$('.sirius_dispatch_queue_entry_' + current_conversation_nid).addClass('active');
				}
        	}
        });

		/*
		if (current_conversation_nid) {
        	sirius_dispatch_operator_dispatch_details(current_dispatch_nid, current_conversation_nid);
        } else {
        	sirius_dispatch_operator_dispatch_dismiss();
        }
        */


		var d = new Date();
		$('#sirius_dispatch_operator_refresh_status').html('Refreshed at ' + d.toLocaleTimeString());
    }

    //
    // Outgoing call
    //

    function sirius_dispatch_operator_call(event) {
    	phone = $(event.target).attr('data-dispatch-phone');
		if (!phone) {
			$('#sirius_dispatch_operator_status').html('No phone number');
			return; 
		}
		$('#phone-number').val(phone);
		// get the phone number to connect the call to
		var params = { 'To' : phone };
		if (device) {
		  	device.connect(params);
		}
    }

/*
 * Below is a bunch of code from Twilio, which could probably be cleaned up.
 */

var speakerDevices = document.getElementById('sirius_dispatch_operator_speaker_devices');
var ringtoneDevices = document.getElementById('sirius_dispatch_operator_ringtone_devices');
var outputVolumeBar = document.getElementById('sirius_dispatch_operator_volume_output');
var inputVolumeBar = document.getElementById('sirius_dispatch_operator_volume_input');
var volumeIndicators = document.getElementById('sirius_dispatch_operator_volume');

var device;

token = Drupal.settings.sirius_twilio.token;

// Setup Twilio.Device
device = new Twilio.Device(token);

device.on('ready',function (device) {
	log('Phone ready');
	document.getElementById('sirius_dispatch_operator_call_controls').style.display = 'block';
});

device.on('error', function (error) {
	log('Sirius Phone Error: ' + error.message);
});

device.on('connect', function (conn) {
	log('Call established');
	document.getElementById('button-call').style.display = 'none';
	document.getElementById('button-hangup').style.display = 'inline';
	bindVolumeIndicators(conn);
});

device.on('disconnect', function (conn) {
	log('Call ended');
	document.getElementById('button-call').style.display = 'inline';
	document.getElementById('button-hangup').style.display = 'none';
});

device.on('incoming', function (conn) {
	log('Incoming connection from ' + conn.parameters.From);
	// accept the incoming connection and start two-way audio
	conn.accept();
});

console.log(device);
device.audio.on('deviceChange', updateAllDevices);

// Show audio selection UI if it is supported by the browser.
if (device.audio.isSelectionSupported) {
	document.getElementById('output-selection').style.display = 'block';
}


// Bind button to make call
document.getElementById('button-call').onclick = function () {
	// get the phone number to connect the call to
	var params = {
  		To: document.getElementById('phone-number').value
	};
	console.log('Calling ' + params.To + '...');
	if (device) {
	  	device.connect(params);
	}
};

// Bind button to hangup call
document.getElementById('button-hangup').onclick = function () {
	log('Hanging up...');
	if (device) {
  		device.disconnectAll();
	}
};

document.getElementById('get-devices').onclick = function() {
	navigator.mediaDevices.getUserMedia({ audio: true }).then(updateAllDevices);
};

speakerDevices.addEventListener('change', function() {
	var selectedDevices = [].slice.call(speakerDevices.children)
  		.filter(function(node) { return node.selected; })
  		.map(function(node) { return node.getAttribute('data-id'); });

	device.audio.speakerDevices.set(selectedDevices);
});

ringtoneDevices.addEventListener('change', function() {
	var selectedDevices = [].slice.call(ringtoneDevices.children)
  		.filter(function(node) { return node.selected; })
  		.map(function(node) { return node.getAttribute('data-id'); });
	device.audio.ringtoneDevices.set(selectedDevices);
});

function bindVolumeIndicators(connection) {
	connection.volume(function(inputVolume, outputVolume) {
		var inputColor = 'red';
		if (inputVolume < .50) {
	   		inputColor = 'green';
		} else if (inputVolume < .75) {
	   		inputColor = 'yellow';
		}
		inputVolumeBar.style.width = Math.floor(inputVolume * 100) + 'px';
		inputVolumeBar.style.background = inputColor;

		var outputColor = 'red';
		if (outputVolume < .50) {
			outputColor = 'green';
		} else if (outputVolume < .75) {
			outputColor = 'yellow';
		}
		outputVolumeBar.style.width = Math.floor(outputVolume * 100) + 'px';
	  	outputVolumeBar.style.background = outputColor;
	});
}

function updateAllDevices() {
	updateDevices(speakerDevices, device.audio.speakerDevices.get());
	updateDevices(ringtoneDevices, device.audio.ringtoneDevices.get());
}

// Update the available ringtone and speaker devices
function updateDevices(selectEl, selectedDevices) {
	selectEl.innerHTML = '';
	device.audio.availableOutputDevices.forEach(function(device, id) {
	var isActive = (selectedDevices.size === 0 && id === 'default');
	selectedDevices.forEach(function(device) {
		if (device.deviceId === id) { isActive = true; }
    });

    var option = document.createElement('option');
    option.label = device.label;
    option.setAttribute('data-id', id);
    if (isActive) {
      option.setAttribute('selected', 'selected');
    }
    selectEl.appendChild(option);
  });
}

// Activity log
function log(message) {
	$('#sirius_dispatch_operator_status').html(message);
}

}); }(jQuery));
