(function ($) {

  Drupal.behaviors.initGeocoder = {
    attach: function (context, settings) {
      geocoder_admin_field_selected();
    }
  };
})(jQuery);

function geocoder_admin_field_selected() {
  var $ = jQuery;
  
  var field = $('#edit-instance-widget-settings-geocoder-field').val();
  var field_type = Drupal.settings.geocoder_widget_settings.types[field];
  var valid_handlers = Drupal.settings.geocoder_widget_settings.handlers[field_type];

  // Filter the options list to ones that are valid for this field
  $('#edit-instance-widget-settings-geocoder-handler option').each(function() {
    handler_type = $(this).val();
    if (geocoder_admin_handler_in_array(handler_type,valid_handlers)) {
      $(this).attr('disabled',false);
      $(this).show();
    }
    else {
      $(this).attr('disabled','disabled');
      $(this).hide();
    }
  });

  // If the currently selected handler is not valid, set it to the first valid handler
  if (!geocoder_admin_handler_in_array($('#edit-instance-widget-settings-geocoder-handler').val(),valid_handlers)) {
    $('#edit-instance-widget-settings-geocoder-handler').val(valid_handlers[0]);
  }
}

function geocoder_admin_handler_in_array(needle, haystack) {
  var length = haystack.length;
  for(var i = 0; i < length; i++) {
    if(haystack[i] == needle) return true;
  }
  return false;
}