(function ($) {
	Drupal.behaviors.sirius_edls_sheet_workers = {
		attach: function (context) {
			var settings = Drupal.settings.sirius_edls_sheet_workers;

			function sheet_search() {
				if (!settings.sheet_nid) { return; }
				flash('Loading sheet ...', 'info');

				$.ajax({
					'url': '/sirius/edls/ajax/sheet_assignments',
					'data': {
						'sheet_nid': settings.sheet_nid,
					},
					'type': 'GET',
					'dataType': 'json',
					'complete': function(jqXHR, textStatus) {
						result = handle_ajax_response(jqXHR, textStatus, 'info');
						if (!result) { return; }

						assignments = result['data']['assignments'];
						crews = result['data']['crews'];

						for (crew_uuid in crews) {
							count = result['data']['crews'][crew_uuid]['count'];
							assigned = result['data']['crews'][crew_uuid]['assigned'];
							$('.sirius_edls_crew[data-uuid=\'' + crew_uuid + '\'] .sirius_edls_crew_stats_assigned').html(assigned);
							$('.sirius_edls_crew[data-uuid=\'' + crew_uuid + '\'] .sirius_edls_crew_stats_count').html(count);

							for (position = 0; position < count; ++position) {
								var assignment_div = $('#crew_' + crew_uuid + '_position_' + position);

								if (assignments[crew_uuid]) {
									assignment = assignments[crew_uuid][position];
								} else {
									assignment = null;
								}

								if (assignment) { 
									worker_id = assignment['worker_id'];
									worker_name = assignment['worker_name'];
									worker_nameid = worker_id + ' - ' + worker_name;
									extra = extra_parse(assignment);
									ms_name = settings.ms_name_lookup_short[assignment['worker_ms']];
									if (!ms_name) { ms_name = ''; }

									assignment_div.find('.sirius_edls_crew_worker_name').html(worker_nameid);
									assignment_div.attr('data-id', worker_id);
									assignment_div.find('.sirius_edls_extra_wrap a').removeClass('sirius_edls_hidden');
									assignment_div.find('.sirius_edls_extra').html(extra_render(extra));
									assignment_div.find('.sirius_edls_extra_time').val(extra['time']);
									// assignment_div.find('.sirius_edls_extra_truck').val(extra['truck']);
									assignment_div.find('.sirius_edls_extra_note').val(extra['note']);
									assignment_div.find('.sirius_edls_extra_classification').val(extra['classification']);
									assignment_div.find('.sirius_edls_ctrl_unassign').removeClass('sirius_edls_ctrl_unassign_disabled');
									assignment_div.find('.sirius_edls_ms').html(ms_name);
								} else {
									assignment_div.find('.sirius_edls_crew_worker_name').html('');
									assignment_div.attr('data-id', '');
									assignment_div.find('.sirius_edls_extra_wrap a').addClass('sirius_edls_hidden');
									assignment_div.find('.sirius_edls_extra').html('');
									assignment_div.find('.sirius_edls_extra_time').val('');
									// assignment_div.find('.sirius_edls_extra_truck').val('');
									assignment_div.find('.sirius_edls_extra_note').val('');
									assignment_div.find('.sirius_edls_extra_classification').val('');
									assignment_div.find('.sirius_edls_ctrl_unassign').addClass('sirius_edls_ctrl_unassign_disabled');
									assignment_div.find('.sirius_edls_ms').html('');
								}
							}
						}
						
						$('#sirius_edls_notes_inner').html(result['data']['notes_render']);
						$('#sirius_edls_notes_edit').val(result['data']['notes']);

						$('#sirius_edls_sheet_stats_assigned').html(result['data']['stats']['assigned']);
						$('#sirius_edls_sheet_stats_count').html(result['data']['stats']['count']);
					}
				});
			}

			function worker_search() {
				flash('Loading workers ...', 'info');
				console.log(settings);
				$('#sirius_edls_worker_search').prop('disabled', true);

				$.ajax({
					'url': '/sirius/edls/ajax/worker_list',
					'data': {
						'employer_nid': settings.employer_nid,
						'date': settings.date,
						'filters': {
							'nameid': $('#sirius_edls_worker_filter_nameid').val(),
							'ms': $('#sirius_edls_worker_filter_ms').val(),
							'has_assignment_curr': $('#sirius_edls_worker_filter_has_assignment_curr').val(),
							'has_assignment_next': $('#sirius_edls_worker_filter_has_assignment_next').val(),
							'prev_department': $('#sirius_edls_worker_filter_prev_department').val(),
							'curr_sheet_nid': settings.sheet_nid,
						},
					},
					'type': 'GET',
					'dataType': 'json',
					'complete': function(jqXHR, textStatus) {
						$('#sirius_edls_worker_search').prop('disabled', false);

						result = handle_ajax_response(jqXHR, textStatus);
						if (!result) { return; }

						workers = result['data']['workers'];

						// Clear all workers
						$('#sirius_edls_workers .sirius_edls_workers_ms').html('');
						$('#sirius_edls_workers .sirius_edls_workers_ms_wrap').addClass('sirius_edls_hidden');

						$('#sirius_edls_sheet_avail_head .sirius_edls_worker_stats_inner').html(workers.length);

						if (!workers.length) { return; }

						for (i=0; i<workers.length; ++i) {
							worker = workers[i];

							var worker_div = $('<div />');

							var c = 'sirius_edls_worker_wrapper';

							if (worker['curr_sheet_nid'] && (worker['curr_sheet_nid'] == settings.sheet_nid)) {
								c += ' sirius_edls_worker_assigned';
							} else if (worker['curr_sheet_nid']) {
								c += ' sirius_edls_worker_unavailable';
							}

							html = '<div class="' + c + '" id="sirius_edls_worker_' + worker['worker_id'] + '" data-id="' + worker['worker_id'] + '">';

							html += '<span class="sirius_edls_worker_indicator">';

							var c = 'sirius_edls_worker_indicator_dot'; 
							if (worker['prev_sheet_nid']) { c += ' sirius_edls_worker_indicator_dot_filled'; }
							html += '<span class="' + c + '"></span>';

							var c = 'sirius_edls_worker_indicator_dot'; 
							if (worker['curr_sheet_nid']) {
								status = worker['curr_status'];
								if (status == 'lock') { c += ' sirius_edls_worker_indicator_dot_filled'; }
								else if (status == 'reserved') { c += ' sirius_edls_worker_indicator_dot_reserved'; }
								else { c += ' sirius_edls_worker_indicator_dot_draft'; }
							}
							html += '<span class="' + c + '"></span>';

							var c = 'sirius_edls_worker_indicator_dot'; 
							if (worker['next_sheet_nid']) {
								status = worker['next_status'];
								if (status == 'lock') { c += ' sirius_edls_worker_indicator_dot_filled'; }
								else if (status == 'reserved') { c += ' sirius_edls_worker_indicator_dot_reserved'; }
								else { c += ' sirius_edls_worker_indicator_dot_draft'; }
							}
							html += '<span class="' + c + '"></span>';

							/*
							html += '<span class="sirius_edls_worker_indicator_' + status + '">';
							if (worker['next_sheet_nid']) { html += '&#9899;'; } else { html += '&#9898;'; }
							html += '</span>';
							*/

							html += '</span>';

							if (settings['show_worker_link']) {
								html += '<span class="sirius_edls_worker_link">';
								html += '<a href="/sirius/edls/worker/' + worker['worker_id'] + '">[' + 'view' + ']</a>&nbsp;';
								html += '</span>';
							}

							html += '<span class="sirius_edls_worker_name">';
							html += worker['worker_id'] + ' - ' + worker['worker_name'];
							html += '</span>';

							html += '</div>';
							worker_div.html(html);

							var wrapper = $('#sirius_edls_workers .sirius_edls_workers_ms_wrap[data-ms=\'' + worker['worker_ms'] + '\'] .sirius_edls_workers_ms');
							worker_div.appendTo(wrapper);

							$('#sirius_edls_workers .sirius_edls_workers_ms_wrap[data-ms=\'' + worker['worker_ms'] + '\']').removeClass('sirius_edls_hidden');
						}

						if (settings.sheet_nid) {
							$('.sirius_edls_worker_wrapper .sirius_edls_worker_name').click(function() {
								worker_id = $(this).parents('.sirius_edls_worker_wrapper').attr('data-id');
								crew_uuid = get_selected_crew();
								assign(worker_id, crew_uuid);
								return false;
							});
						}

						$('.sirius_edls_worker_wrapper .sirius_edls_worker_indicator').click(function() {
							worker_id = $(this).parents('.sirius_edls_worker_wrapper').attr('data-id');
							popup_worker_details(worker_id);
							return false;
						});

						sheet_search();
					}
				});
			}

			function assign(worker_id, crew_uuid) {
				worker_nameid = $('#sirius_edls_worker_' + worker_id + ' .sirius_edls_worker_name').html();
				flash('Assigning: ' + worker_nameid + '...', 'info');

				$.ajax({
					'url': '/sirius/edls/ajax/assign',
					'data': {
						'sheet_nid': settings.sheet_nid,
						'worker_id': worker_id,
						'crew_uuid': crew_uuid,
					},
					'type': 'GET',
					'dataType': 'json',
					'complete': function(jqXHR, textStatus) {
						result = handle_ajax_response(jqXHR, textStatus);
						sheet_search();
						if (!result) { return; }
						$('#sirius_edls_worker_' + worker_id).addClass('sirius_edls_worker_assigned');
					}
				});
			}

			function unassign(worker_id) {
				flash('Unassigning ' + worker_id + '...', 'info');

				$.ajax({
					'url': '/sirius/edls/ajax/unassign',
					'data': {
						'sheet_nid': settings.sheet_nid,
						'worker_id': worker_id,
					},
					'type': 'GET',
					'dataType': 'json',
					'complete': function(jqXHR, textStatus) {
						result = handle_ajax_response(jqXHR, textStatus);
						sheet_search();
						if (!result) { return; }
						$('#sirius_edls_worker_' + worker_id).removeClass('sirius_edls_worker_assigned');
					}
				});
			}

			function assignment_set_extra(worker_id) {
				flash('Updating worker ' + worker_id + '...', 'info');
				$('.sirius_popup_overlay').fadeOut();
				$('.sirius_popup_wrap').fadeOut();

				var assignment_div = $('.sirius_edls_assignment_wrapper[data-id=\'' + worker_id + '\']');

				$.ajax({
					'url': '/sirius/edls/ajax/assignment_set_extra',
					'data': {
						'sheet_nid': settings.sheet_nid,
						'worker_id': worker_id,
						'extra': {
							'time': assignment_div.find('.sirius_edls_extra_time').val(),
							'classification': assignment_div.find('.sirius_edls_extra_classification').val(),
							// 'truck': assignment_div.find('.sirius_edls_extra_truck').val(),
							'note': assignment_div.find('.sirius_edls_extra_note').val(),
						}
					},
					'type': 'GET',
					'dataType': 'json',
					'complete': function(jqXHR, textStatus) {
						handle_ajax_response(jqXHR, textStatus);
						sheet_search();
					}
				});
			}

			function set_notes() {
				flash('Sending notes...', 'info');
				$('.sirius_popup_overlay').fadeOut();
				$('.sirius_popup_wrap').fadeOut();

				$.ajax({
					'url': '/sirius/edls/ajax/sheet_set_notes',
					'data': {
						'sheet_nid': settings.sheet_nid,
						'notes': $('#sirius_edls_notes_edit').val(),
					},
					'type': 'POST',
					'dataType': 'json',
					'complete': function(jqXHR, textStatus) {
						handle_ajax_response(jqXHR, textStatus);
						sheet_search();
					}
				});
			}

			function popup_worker_details(worker_id) {
				flash('Loading worker ' + worker_id + '...', 'info');

				$.ajax({
					'url': '/sirius/edls/ajax/worker_lookup',
					'data': {
						'employer_nid': settings.employer_nid,
						'worker_id': worker_id,
						'date': settings.date,
					},
					'type': 'GET',
					'dataType': 'json',
					'complete': function(jqXHR, textStatus) {
						result = handle_ajax_response(jqXHR, textStatus);
						if (!result) { return; }

			      var left = Math.floor(($(window).width() - 600) / 2);
			      var top = Math.floor(($(window).height() - 400) / 2);

			      var overlay = $('#popup_worker_details_overlay');
			      var wrap = $('#popup_worker_details_wrap');
			      var close = $('#popup_worker_details_close');
			      var content = $('#popup_worker_details_content');

			      content.html(result['render']);

			      overlay.show();
			      wrap.show().css({
			        'width': 800,
			        'height': 'auto',
			        'position': 'fixed',
			        'top': '10%',
			        'left': '50%',
			        'transform': 'translate(-50%, 0)',
			      });

			      // close announcement by keyboard or mouse
			      close.click(function() {
			        overlay.fadeOut();
			        wrap.fadeOut();
			      });
			      overlay.click(function() {
			        overlay.fadeOut();
			        wrap.fadeOut();
			      });
			      $(document).keyup(function(e) {
			        if (e.keyCode == 27) {
			          overlay.fadeOut();
			          wrap.fadeOut();
			        }
			      });
					}
				});
		  }

			function get_selected_crew() {
				return $('.sirius_edls_crew_selected').attr('data-uuid');
			}

			function extra_render(extra) {
				if (!extra) { return ''; }
				render = '';
				if (extra['classification']) { 
					render += settings['classification_options'][extra['classification']];
				}
				if (extra['time']) {
					if (render) { render += ' '; }
					render += extra['time'];
				}
				// if (extra['truck']) {
				// if (render) { render += ' '; }
				// render += 'Trk #' + extra['truck'];
				// }
				if (extra['note']) {
					if (render) { render += ' '; }
					render += extra['note'];
				}
				return render;
			}

			function extra_parse(assignment) {
				// if (!assignment) { return {'time':'', 'classification':'', 'truck':''}; }
				if (!assignment) { return {'time':'', 'classification':'', 'note':''}; }

				extra = assignment['assignment_extra'];
				// if (!extra) { return {'time':'', 'classification':'', 'truck':''}; }
				if (!extra) { return {'time':'', 'classification':'', 'note':''}; }
				
				if (!('time' in extra)) { extra['time'] = ''; }
				if (!('classification' in extra)) { extra['classification'] = ''; }
				// if (!('truck' in extra)) { extra['truck'] = ''; }
				if (!('note' in extra)) { extra['note'] = ''; }
				return extra;
			}

			function handle_ajax_response(jqXHR, textStatus, priority = 'success') {
				if (textStatus == 'error') {
					flash('Connection failed: ' + jqXHR['statusText'], 'error');
					return false;
				} else if (!jqXHR['responseJSON']) {
					flash('Error: ' + jqXHR['responseText'], 'error');
					return false;
				} else if (!jqXHR['responseJSON']['success']) {
					flash('Error: ' + jqXHR['responseJSON']['msg'], 'error');
					return false;
				} else {
					flash(jqXHR['responseJSON']['msg'], priority);
					return jqXHR['responseJSON'];
				}
			}

			function flash(msg, priority) {
				return Drupal.behaviors.sirius_ux.flash(msg, priority);
			}

			//
			// Run once on load...
			//

			// Search for workers on load
			worker_search();

			// Search for workers every X seconds
  		interval = 300;
			setInterval(worker_search, interval * 1000);

			// Select the first crew by default
			$('.sirius_edls_crew').first().addClass('sirius_edls_crew_selected');

			//
			// Click handlers
			//

			// Search for workers on click
			$('#sirius_edls_worker_search').click(function () {
				worker_search();
				return false;
			});

			// Update on click
			$('.sirius_edls_set_extra_submit').click(function() {
				worker_id = $(this).parents('.sirius_edls_assignment_wrapper').attr('data-id');
				assignment_set_extra(worker_id);
				return false;
			});

			// Unassign on click
			$('.sirius_edls_ctrl_unassign').click(function() {
				worker_id = $(this).parents('.sirius_edls_assignment_wrapper').attr('data-id');
				if (!worker_id) { return; }
				unassign(worker_id);
				return false;
			});

			// Set notes
			$('.sirius_edls_set_notes_submit').click(function() {
				set_notes();
				return false;
			});

			// Select a crew
			$('.sirius_edls_crew').click(function() {
				$('.sirius_edls_crew').removeClass('sirius_edls_crew_selected');
				$(this).addClass('sirius_edls_crew_selected');
				return false;
			});
		}
	};
})(jQuery);
