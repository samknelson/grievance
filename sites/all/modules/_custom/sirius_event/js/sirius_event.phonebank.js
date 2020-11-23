(function ($) {
	Drupal.behaviors.esignature = {
		attach: function (context) {
			$('#page-banner').hide();
			$('#header').hide();
			$('.banner-menu-outer-wrapper').hide();
			$('.secondary-tasks-wrapper').hide();
			$('.primary-local-tasks-wrapper').hide();
			$('#page-footer').hide();
			$('body').addClass('sirius_fullpage');

			function enable_submit() {
				if ($('#submit_wrap input').hasClass('disable_always')) { 
					$('#submit_wrap input').prop('disabled', true);
					return;
				}

				pstatus_tid = $('#pstatus_wrap input[type=radio][name=pstatus]:checked').val();
				if ($('#submit_wrap input').hasClass('disable_if_pstatus_' + pstatus_tid)) {
					$('#submit_wrap input').prop('disabled', true);
					return;
				}

				$('#submit_wrap input').prop('disabled', false);
			}

			enable_submit();
			$('#pstatus_wrap').once('react-to-pstatus', function() {
				$('#pstatus_wrap input[type=radio][name=pstatus]').on('change', function() {
					enable_submit();
				});
			});

			


			$('#sirius_ckstyles_script_instructions_toggle').once('react-to-toggle', function() {
				$('#sirius_ckstyles_script_instructions_toggle').on('click', function(event) {
					event.preventDefault();
					instructions_toggle();
				});
			});

			function instructions_toggle() {
				if ($.cookie("sirius_ckstyles_script_instructions_hide") == 1) {
					$.cookie("sirius_ckstyles_script_instructions_hide", 0);
				} else {
					$.cookie("sirius_ckstyles_script_instructions_hide", 1);
				}
				instructions_handle();
			}

			function instructions_handle() {
				if ($('.sirius_ckstyles_script_instructions').length < 1) {
					$('#sirius_ckstyles_script_instructions_toggle').hide();
				} else {
					if ($.cookie("sirius_ckstyles_script_instructions_hide") == 1) {
						$('#sirius_ckstyles_script_instructions_toggle').html('[Show Instructions]');
						$('.sirius_ckstyles_script_instructions').hide();
					} else {
						$('#sirius_ckstyles_script_instructions_toggle').html('[Hide Instructions]');
						$('.sirius_ckstyles_script_instructions').show();
					}
				}
			}

			instructions_handle();

			/*
			participant_nid = Drupal.settings.sirius_event_phonebank.participant_nid;
			console.log("Phone bank for " + participant_nid);

			// sirius_event_phonebank_survey
			$('#sirius_event_phonebank_survey select').change(function(event) {
				sirius_event_phonebank_submit_ajax();
			});
			$('#sirius_event_phonebank_survey input').blur(function(event) {
				sirius_event_phonebank_submit_ajax();
			});
			$('#sirius_event_phonebank_survey textarea').blur(function(event) {
				sirius_event_phonebank_submit_ajax();
			});

			function sirius_event_phonebank_submit_ajax() {
				url = '/sirius/event/phonebank_submit_ajax';
				values = $( "input, textarea, select").serialize();
				console.log(values);
				$.ajax({
				  type: "POST",
				  url: url,
				  data: values,
				}).done(function( data ) {
					console.log("Ajax request returned: " + data);
			  });
			}
			*/

			/*
			function sirius_quicksave_msg(status, msg) {
				if (status != 'ready')  { $('#sirius_quicksave_msg').removeClass('sirius_quicksave_ready'); }
				if (status != 'active') { $('#sirius_quicksave_msg').removeClass('sirius_quicksave_active'); }
				if (status != 'error')  { $('#sirius_quicksave_msg').removeClass('sirius_quicksave_error'); }
				$('#sirius_quicksave_msg').addClass('sirius_quicksave_' + status);
				$('#sirius_quicksave_msg').html(msg);
			}

			$('.sirius_quicksave').change(function(e) {
				$('#sirius_quicksave_msg').addClass('sirius_quicksave_active');
				$('#sirius_quicksave_msg').removeClass('sirius_quicksave_ready');
				$('#sirius_quicksave_msg').html('Saving...');

				url = '/sirius/ajax/phonebank-quicksave';
				post_data = {};
				post_data['key'] = $('#sirius_quicksave_key').val();
				post_data['json_id'] = $(this).attr('id');
				post_data['json_value'] = $(this).serializeArray();

				// console.log($(this));
				console.log(post_data);
				return;


				$.ajax({
					'url': url,
					'type': 'POST',
					'data': post_data,
		  		'dataType': 'json',
		  		'error': function(jqXHR, textStatus, errorThrown) {
						sirius_quicksave_msg('error', 'Error: ' + textStatus + ': ' + errorThrown);
		  		},
					'success': function(data) {
						console.log(data);
						var d = new Date();
						if (data['success']) {
							sirius_quicksave_msg('ready', 'Saved: ' + data['msg'] + ' (' + d.toLocaleTimeString() + ')');
						} else {
							sirius_quicksave_msg('error', 'Error: ' + data['msg'] + ' (' + d.toLocaleTimeString() + ')');
						}
					}
				});
			});

			sirius_quicksave_msg('ready', 'Ready...');
			*/
		}
	};
})(jQuery);
