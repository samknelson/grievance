/**
 * @file
 * Primary JavaScript file for the Pop-up announcement module.
 */

(function ($) {

Drupal.behaviors.sirius_news_popup = {
  attach: function (context) {
    if (Drupal.settings && Drupal.settings.sirius_news_popup) {
      function sirius_news_popup_create() {

        var left = Math.floor(($(window).width() - Drupal.settings.sirius_news_popup.width) / 2);
        var top = Math.floor(($(window).height() - Drupal.settings.sirius_news_popup.height) / 2);

        var overlay = $('#popup-announcement-overlay', context);
        var announcement = $('#popup-announcement-wrap', context);
        var close = $('#popup-announcement-close', context);

        overlay.show();
        $('BODY').append(announcement);
        announcement.show().css({
          'width': Drupal.settings.sirius_news_popup.width,
          'height': Drupal.settings.sirius_news_popup.height,
          'left': left,
          'top': top
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

      }

      sirius_news_popup_create();
    }
  }
};

})(jQuery);