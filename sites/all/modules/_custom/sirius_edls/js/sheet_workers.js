(function ($) {
	Drupal.behaviors.sirius_edls_sheet_workers = {
		attach: function (context) {
			var settings = Drupal.settings.sirius_edls_sheet_workers;

			function sheet_search() {
				flash('Loading sheet ...', 'info');

				$.ajax({
					'url': '/sirius/edls/ajax/sheet/assignments',
					'data': {
						'sheet_nid': settings.sheet_nid,
					},
					'type': 'GET',
					'dataType': 'json',
					'error': function(jqXHR, textStatus, errorThrown) {
						flash('Connection failed: ' + textStatus + ': ' + errorThrown, 'error');
					},
					'success': function(result) {
						if (!result['success']) {
							flash('Error: ' + result['msg'], 'error');
							return;
						}

						assignments = result['data']['assignments'];
						for (crew_uuid in assignments) {
							for (position in assignments[crew_uuid]) {
								worker_id = assignments[crew_uuid][position]['worker_id'];
								worker_name = assignments[crew_uuid][position]['worker_name'];
								worker_nameid = worker_id + ' - ' + worker_name;
								extra = extra_parse(assignments[crew_uuid][position]);

								var assignment_div = $('#crew_' + crew_uuid + '_position_' + position);
								assignment_div.find('.sirius_edls_crew_worker_name').html(worker_nameid);
								assignment_div.attr('data-id', worker_id);
								assignment_div.find('.sirius_edls_extra_wrap a').removeClass('sirius_edls_hidden');
								assignment_div.find('.sirius_edls_extra').html(extra_render(extra));
								assignment_div.find('.sirius_edls_extra_time').val(extra['time']);
								assignment_div.find('.sirius_edls_extra_classification').val(extra['classification']);
							}
						}

						flash('Sheet refreshed at: ' + new Date().toLocaleTimeString(), 'success');
					}
				});
			}

			function worker_search() {
				flash('Loading workers ...', 'info');

				$.ajax({
					'url': '/sirius/edls/ajax/worker/list',
					'data': {
						'employer_nid': settings.employer_nid,
						'date': settings.date,
						'filters': {
							'nameid': $('#sirius_edls_worker_filter_nameid').val(),
							'ms': $('#sirius_edls_worker_filter_ms').val(),
						},
					},
					'type': 'GET',
					'dataType': 'json',
					'error': function(jqXHR, textStatus, errorThrown) {
						flash('Connection failed: ' + textStatus + ': ' + errorThrown, 'error');
					},
					'success': function(result) {
						if (!result['success']) {
							flash('Error: ' + result['msg'], 'error');
							return;
						}

						workers = result['data']['workers'];

						// Clear all workers
						$('#sirius_edls_workers .sirius_edls_workers_ms').html('');
						$('#sirius_edls_workers .sirius_edls_workers_ms_wrap').addClass('sirius_edls_hidden');

						if (!workers.length) {
							flash('No workers found.', 'warning');
							return;
						}

						for (i=0; i<workers.length; ++i) {
							worker = workers[i];

							var worker_div = $('<div />');

							var c = 'sirius_edls_worker_wrapper';
							if (worker['sheet_nid'] == settings.sheet_nid) {
								c += ' sirius_edls_worker_assigned';
							} else if (worker['sheet_nid']) {
								c += ' sirius_edls_worker_unavailable';
							}

							html = '<div class="' + c + '" id="sirius_edls_worker_' + worker['worker_id'] + '" data-id="' + worker['worker_id'] + '">';

							/*
							html += '<a href="#" class="sirius_popup_trigger">';
							html += '<i class="fas fa-info-circle"></i>';
							html += '</a>';
							*/

							html += '<span class="sirius_edls_worker_name">';
							html += worker['worker_id'] + ' - ' + worker['worker_name'];
							html += '</span>';

							html += '</div>';
							worker_div.html(html);

							var wrapper = $('#sirius_edls_workers .sirius_edls_workers_ms_wrap[data-ms=\'' + worker['worker_ms'] + '\'] .sirius_edls_workers_ms');
							worker_div.appendTo(wrapper);

							$('#sirius_edls_workers .sirius_edls_workers_ms_wrap[data-ms=\'' + worker['worker_ms'] + '\']').removeClass('sirius_edls_hidden');
						}

						$('.sirius_edls_worker_wrapper .sirius_edls_worker_name').click(function() {
							worker_id = $(this).parents('.sirius_edls_worker_wrapper').attr('data-id');
							crew_uuid = get_selected_crew();
							assign(worker_id, crew_uuid);
							return false;
						});

						sheet_search();

						flash('List refreshed at: ' + new Date().toLocaleTimeString(), 'success');
					}
				});
			}

			function get_selected_crew() {
				return $('.sirius_edls_crew_selected').attr('data-uuid');
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
					'error': function(jqXHR, textStatus, errorThrown) {
						flash('Connection failed: ' + textStatus + ': ' + errorThrown, 'error');
					},
					'success': function(result) {
						if (!result['success']) {
							flash('Error: ' + result['msg'], 'error');
							return;
						}

						position = result['position'];
						extra = extra_parse(result['assignment']);

						var assignment_div = $('#crew_' + crew_uuid + '_position_' + position);
						var worker_div = $('#sirius_edls_worker_' + worker_id);

						assignment_div.find('.sirius_edls_crew_worker_name').html(worker_nameid);
						assignment_div.attr('data-id', worker_id);
						assignment_div.find('.sirius_edls_extra_wrap a').removeClass('sirius_edls_hidden');
						assignment_div.find('.sirius_edls_extra').html(extra_render(extra));
						assignment_div.find('.sirius_edls_extra_time').val(extra['time']);
						assignment_div.find('.sirius_edls_extra_classification').val(extra['classification']);

						worker_div.addClass('sirius_edls_worker_assigned');

						flash('Assigned: ' + worker_nameid + ' to position #' + (position+1), 'success');
						return;
					}
				});
			}

			function unassign(worker_id) {
				flash('Removing ' + worker_id + '...', 'info');

				$.ajax({
					'url': '/sirius/edls/ajax/unassign',
					'data': {
						'sheet_nid': settings.sheet_nid,
						'worker_id': worker_id,
					},
					'type': 'GET',
					'dataType': 'json',
					'error': function(jqXHR, textStatus, errorThrown) {
						flash('Connection failed: ' + textStatus + ': ' + errorThrown, 'error');
					},
					'success': function(result) {
						if (!result['success']) {
							flash('Error: ' + result['msg'], 'error');
							return;
						}

						position = result['position'];

						var assignment_div = $('.sirius_edls_assignment_wrapper[data-id=\'' + worker_id + '\']');
						var worker_div = $('#sirius_edls_worker_' + worker_id);

						worker_nameid = assignment_div.find('.sirius_edls_crew_worker_name').html();
						assignment_div.find('.sirius_edls_crew_worker_name').html('');
						assignment_div.attr('data-id', '');
						assignment_div.find('.sirius_edls_extra_wrap a').addClass('sirius_edls_hidden');
						assignment_div.find('.sirius_edls_extra').html('');
						assignment_div.find('.sirius_edls_extra_time').val('');
						assignment_div.find('.sirius_edls_extra_classification').val('');

						worker_div.removeClass('sirius_edls_worker_assigned');

						flash('Removed: ' + worker_nameid, 'success');
						return;
					}
				});
			}

			function assignment_set_extra(worker_id) {
				flash('Sending ' + worker_id + '...', 'info');
				var assignment_div = $('.sirius_edls_assignment_wrapper[data-id=\'' + worker_id + '\']');

				$.ajax({
					'url': '/sirius/edls/ajax/assignment_set_extra',
					'data': {
						'sheet_nid': settings.sheet_nid,
						'worker_id': worker_id,
						'extra': {
							'time': assignment_div.find('.sirius_edls_extra_time').val(),
							'classification': assignment_div.find('.sirius_edls_extra_classification').val(),
						}
					},
					'type': 'GET',
					'dataType': 'json',
					'error': function(jqXHR, textStatus, errorThrown) {
						flash('Connection failed: ' + textStatus + ': ' + errorThrown, 'error');
					},
					'success': function(result) {
						if (!result['success']) {
							flash('Error: ' + result['msg'], 'error');
							return;
						}

						var assignment_div = $('.sirius_edls_assignment_wrapper[data-id=\'' + worker_id + '\']');
						worker_nameid = assignment_div.find('.sirius_edls_crew_worker_name').html();

						assignment_div.find('.sirius_edls_extra').html(extra_render(result['extra']));

						// This should really be handled by popup.js, but I'm being lazy and copy-paste
						$('.sirius_popup_overlay').fadeOut();
      			$('.sirius_popup_wrap').fadeOut();

						flash('Updated: ' + worker_nameid, 'success');
						return;
					}
				});
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
				return render;
			}

			function extra_parse(assignment) {
				if (!assignment) { return {'time':'', 'classification':''}; }

				extra = assignment['assignment_extra'];
				if (!extra) { return {'time':'', 'classification':''}; }
				
				extra = JSON.parse(extra);
				if (!extra) { return {'time':'', 'classification':''}; }

				if (!('time' in extra)) { extra['time'] = ''; }
				if (!('classification' in extra)) { extra['classification'] = ''; }
				return extra;
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
			$('.sirius_edls_crew_worker_name').click(function() {
				worker_id = $(this).parents('.sirius_edls_assignment_wrapper').attr('data-id');
				unassign(worker_id);
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
