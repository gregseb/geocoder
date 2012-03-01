<?php
/**
 * Implements geocoder_services_resources().
 */
function geocoder_services_resources() {
  return array(
    'geocoder' => array(
      'retrieve' => array(
        'help' => 'Geocode data',
        'file' => array('type' => 'inc', 'module' => 'geocoder', 'name' => 'geocoder.services'),
        'callback' => 'geocoder_services_geocode',
        'access callback' => 'geocoder_services_access',
        'access arguments' => array('geocode'),
        'args' => array(
          array(
            'name' => 'handler',
            'type' => 'string',
            'description' => 'The geocoder handler to use - google, gpx, kml etc.',
            'source' => array('path' => '0'),
            'optional' => FALSE,
          ),
          array(
            'name' => 'data',
            'type' => 'string',
            'description' => 'Value to geocode',
            'source' => array('param' => 'data'),
            'optional' => FALSE,
          ),
          array(
            'name' => 'output',
            'type' => 'string',
            'description' => 'Output Format (GPX, WKT, etc.)',
            'source' => array('param' => 'output'),
            'optional' => TRUE,
          ),
        ),
      ),
      'index' => array(
        'help' => 'List Geocoder Capabilities',
        'file' => array('type' => 'inc', 'module' => 'geocoder', 'name' => 'geocoder.services'),
        'callback' => 'geocoder_services_capabilities',
        'access callback' => 'geocoder_services_access',
        'access arguments' => array('capabilitie'),
      ),
    ),
  );
}

//@@TODO: Implement this
function geocoder_services_access($action = 'geocode') {
  return TRUE;
}

/**
 * Callback for geocoding service
 *
 * @param int $id
 * @param object $data
 * @return object
 */
function geocoder_services_geocode($handler, $data, $format = 'default') {
  geocoder_load_geophp();
  if (!in_array($handler, array_keys(geocoder_services_handlers()))) {
    drupal_set_message(t('Could not find handler @handler',array('@handler' => $handler)),'error');
    drupal_not_found();
    exit();
  }
  if (($format && $format != 'default') && !in_array($format, array_keys(geoPHP::getAdapterMap()))) {
    drupal_set_message(t('Could not find format @format',array('@format' => $format)),'error');
    drupal_not_found();
    exit();
  }
  
  $geom = geocoder($handler, $data);
  if (!$format || $format == 'default') {
    $result = $geom->out('json');
    return json_decode($result);
  }
  else {
    return $geom->out($format);
  }
}

function geocoder_services_capabilities() {
  geocoder_load_geophp();
  $object = new stdClass();
  $object->handlers = geocoder_services_handlers();
  $object->output = geoPHP::getAdapterMap();
  return $object;
}

function geocoder_services_handlers() {
  $handlers = array();
  foreach (geocoder_handler_info() as $hid => $handler) {
    if (!isset($handler['safe']) || $handler['safe']) {
      $handlers[$hid] = $handler['description'];
    }
  }
  return $handlers;
}