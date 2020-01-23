(function($) { $(document).ready(function () {
    foo = Drupal.settings.sirius_worker_skills.foo;
    console.log("Boo");
    console.log(foo);
    $('.sirius_job_nid_set').click(function(event) {
            event.preventDefault();
            console.log(event.target);
            job_nid = $(event.target).data('job-nid');
            $('#sirius_job_nid').val(job_nid);
            $('.sirius_accordion_label').removeClass('accordion_highlighted');
            $(event.target).parent('.sirius_accordion_body').prev('.sirius_accordion_label').addClass('accordion_highlighted');
    });
}); }(jQuery));
