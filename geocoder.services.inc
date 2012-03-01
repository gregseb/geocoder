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
            'name' => 'format',
            'type' => 'string',
            'description' => 'Output Format (GPX, WKT, etc.)',
            'source' => array('param' => 'format'),
            'optional' => TRUE,
          ),
        ),
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
  $geom = geocoder($handler, $data);
  if ($format == 'default') {
    $result = $geom->out('json');
    return json_decode($result);
  }
  else {
    $result = $geom->out($format);
    $obj = new stdClass;
    $obj->result = $result;
    return $obj;
  }
}