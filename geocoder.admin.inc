<?php

/**
 * @file
 * Settings form.
 */

/**
 * Module settings page.
 */
function geocoder_admin_settings($form, &$form_state) {
  $geocoder_settings= variable_get("geocoder_settings", array());

  include_once(drupal_get_path('module', 'geocoder') . '/geocoder.drush.inc');
  $form['geocoder_backfill'] = array(
    '#type' => 'fieldset',
    '#title' => t('Maintenance'),
  );

  $form['geocoder_backfill']['backfill'] = array(
    '#type' => 'submit',
    '#value' => t('Recalculate all geocoded values'),
    '#submit' => array('geocoder_drush_backfill'),
  );

  $form['geocoder_apikey_yahoo'] = array(
    '#type' => 'textfield',
    '#title' => t('Yahoo PlaceFinder API Key'),
    '#description' => t('You can obtain a Yahoo PlaceFinder consumer key at') . ' ' . 'http://developer.yahoo.com/geo/placefinder/',
    '#default_value' => empty($geocoder_settings['geocoder_apikey_yahoo']) ? '' : $geocoder_settings['geocoder_apikey_yahoo'],
    '#required' => FALSE,
  );

  $form['geocoder_apikey_yandex'] = array(
    '#type' => 'textfield',
    '#title' => t('Yandex Maps API Key'),
    '#description' => t('You can obtain a Yandex API Key at ') . 'http://api.yandex.ru/maps/getkey.xml',
    '#default_value' => empty($geocoder_settings['geocoder_apikey_yandex']) ? '' : $geocoder_settings['geocoder_apikey_yandex'],
    '#required' => FALSE,
  );

  $form['#submit'][] = 'geocoder_admin_settings_submit';
  return system_settings_form($form);
}

function geocoder_admin_settings_validate($form_id, $form_values) {
  if (!empty($form_values['values']['geocoder_apikey_yahoo']) && preg_match("/[^A-Za-z0-9\\-]/", trim($form_values['values']['geocoder_apikey_yahoo']))) {
    form_set_error('geocoder_apikey_yahoo', t('Yahoo API keys are alpha numeric and dashes only.'));
  }
}

function geocoder_admin_settings_submit($form, &$form_state) {
  $geocoder_settings= variable_get("geocoder_settings", array());
  $geocoder_settings['geocoder_apikey_yahoo'] = trim($form_state['values']['geocoder_apikey_yahoo']);
  $geocoder_settings['geocoder_apikey_yandex'] = trim($form_state['values']['geocoder_apikey_yandex']);
  variable_set("geocoder_settings", $geocoder_settings);
}
