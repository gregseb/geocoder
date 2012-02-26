<?php
// $Id$

/**
 * @file
 * Plugin to provide a google geocoder.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Google Geocoder"),
  'description' => t('Geocodes via google geocoder'),
  'callback' => 'geocoder_google',
  'field_types' => array('text','text_long','addressfield','text_with_summary','computed'),
  'field_callback' => 'geocoder_google_field',
);

/**
 * Process Markup
 */
function geocoder_google($address, $return = 'point') {
  geocoder_load_geophp();
  return geoPHP::load($address,'google_geocode',$return);
}

function geocoder_google_field($field, $field_item) {
  if ($field['type'] == 'text' || $field['type'] == 'text_long' || $field['type'] == 'text_with_summary' || $field['type'] == 'computed') {
    return geocoder_google($field_item['value']);
  }
  if ($field['type'] == 'addressfield') {
    $address = geocoder_widget_parse_addressfield($field_item);
    return geocoder_google($address);
  }
}
