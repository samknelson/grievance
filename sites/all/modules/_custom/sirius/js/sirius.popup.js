(function ($) {

Drupal.behaviors.sirius = {
  attach: function (context) {
    $('.sirius_popup_trigger').click(function(event) {
      event.preventDefault();
      var left = Math.floor(($(window).width() - 600) / 2);
      var top = Math.floor(($(window).height() - 400) / 2);

      var overlay = $(this).parent().find('.sirius_popup_overlay');
      var announcement = $(this).parent().find('.sirius_popup_wrap');
      var close = $(this).parent().find('.sirius_popup_close');

      overlay.show();
      announcement.show().css({
        'width': 600,
        'height': 'auto',
        'position': 'fixed',
        'top': '10%',
        'left': '50%',
        'transform': 'translate(-50%, 0)',
      });

      // close announcement by keyboard or mouse
      close.click(function() {
        overlay.fadeOut();
        announcement.fadeOut();
      });
      overlay.click(function() {
        overlay.fadeOut();
        announcement.fadeOut();
      });
      $(document).keyup(function(e) {
        if (e.keyCode == 27) {
          overlay.fadeOut();
          announcement.fadeOut();
        }
      });
    });
  }
};

})(jQuery);