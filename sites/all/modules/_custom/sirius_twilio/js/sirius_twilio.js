(function($) { $(document).ready(function () {

var speakerDevices = document.getElementById('speaker-devices');
var ringtoneDevices = document.getElementById('ringtone-devices');
var outputVolumeBar = document.getElementById('output-volume');
var inputVolumeBar = document.getElementById('input-volume');
var volumeIndicators = document.getElementById('volume-indicators');

var device;
var CallSid

token = Drupal.settings.sirius_twilio.token;
event_nid = Drupal.settings.sirius_twilio.event_nid;

// Setup Twilio.Device
device = new Twilio.Device(token);

device.on('ready',function (device) {
	log('Sirius Phone Ready.');
	document.getElementById('call-controls').style.display = 'block';
});

device.on('error', function (error) {
	log('Sirius Phone Error: ' + error.message);
});

device.on('connect', function (conn) {
	CallSid = conn.parameters.CallSid;
	console.log(CallSid);
	console.log(conn.parameters);
	console.log(conn);
	log('Call established.');
	document.getElementById('button-call').style.display = 'none';
	document.getElementById('button-hangup').style.display = 'inline';
	volumeIndicators.style.display = 'block';
	bindVolumeIndicators(conn);
});

device.on('disconnect', function (conn) {
	log('Call ended.');
	document.getElementById('button-call').style.display = 'inline';
	document.getElementById('button-hangup').style.display = 'none';
	volumeIndicators.style.display = 'none';
});

device.on('incoming', function (conn) {
	log('Incoming connection from ' + conn.parameters.From);
	// accept the incoming connection and start two-way audio
	conn.accept();
});

device.audio.on('deviceChange', updateAllDevices);

// Show audio selection UI if it is supported by the browser.
if (device.audio.isSelectionSupported) {
	document.getElementById('output-selection').style.display = 'block';
}


// Bind button to make call
document.getElementById('button-call').onclick = function (event) {
	// get the phone number to connect the call to
	var params = {
  	To: document.getElementById('phone-number').value
	};
	console.log('Calling ' + params.To + '...');
	if (device) {
		device.connect(params);
	}
	return false;
};

// Bind button to hangup call
document.getElementById('button-hangup').onclick = function () {
	log('Hanging up...');
	if (device) {
		device.disconnectAll();
	}
	return false;
};

document.getElementById('button-omg').onclick = function () {
	log('Transferring...');
	url = '/sirius/event/phonebank/omg/transfer/' + event_nid + '/' + CallSid;
	$.ajax({url: url}).done(function(data) { 
		// device.disconnectAll(); 
		console.log(data);
	});
	return false;
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
		inputVolumeBar.style.width = Math.floor(inputVolume * 300) + 'px';
		inputVolumeBar.style.background = inputColor;

		var outputColor = 'red';
		if (outputVolume < .50) {
			outputColor = 'green';
		} else if (outputVolume < .75) {
			outputColor = 'yellow';
		}
		outputVolumeBar.style.width = Math.floor(outputVolume * 300) + 'px';
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
  var logDiv = document.getElementById('log');
  logDiv.innerHTML += '<p>&gt;&nbsp;' + message + '</p>';
  logDiv.scrollTop = logDiv.scrollHeight;
}

}); }(jQuery));
