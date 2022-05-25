(function ($) {

Drupal.behaviors.sirius_tooltip = {
  attach: function (context) {
    $('.sirius-tooltip').tooltip({
      content: function () {
        return $(this).children('.sirius-tooltip-content').html();
      },
      position: { 
        my: "left+15 center", 
        at: "right center"
      },
      tooltipClass: 'sirius-tooltip-content-active', // Deprecated now
      classes: {
        'ui-tooltip': 'sirius-tooltip-content-active', // should work when we upgrade
      }
    });
  }
};

})(jQuery);