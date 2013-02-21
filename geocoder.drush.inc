<?php

/**
 * Implementation of hook_drush_command().
 */
function geocoder_drush_command() {
  $items = array();

  // the key in the $items array is the name of the command.
  $items['geocoder-backfill'] = array(
    'callback' => 'geocoder_drush_backfill',
    'description' => "Geocodes all nodes that have a geocoder widget but no geodata.",
  );

  return $items;
}

function geocoder_drush_backfill() {
  $all_entity_info = entity_get_info();
  foreach ($all_entity_info as $entity_type => $entity_info) {
    dpm($entity_info);
    $save_callback = $entity_info['save callback'];
    if ($entity_info['fieldable']) {
      foreach ($entity_info['bundles'] as $bundle_name => $bundle_info) {
        foreach (field_info_instances($entity_type, $bundle_name) as $field_name => $field_instance) {
          dpm($field_instance);
          $field_info = field_info_field($field_name);
          if ($field_instance['widget']['type'] === 'geocoder') {
            $entity_load = $entity_info['load hook'];

            $query = db_select($entity_info['base table'], 't')
            ->fields('t', array($entity_info['entity keys']['id']));

            $results = $query->execute();

            // A select query on the users table will get a first result with id = 0
            //if ($entity_type == 'user') $id = $results->fetchField();
            //dpm($id);

            while (($id = $results->fetchField()) !== FALSE) {
            //for ($id = $results->fetchField(); $id !== FALSE; $id = $results->fetchField()) {
              if ($id == 0 ) continue;
              dpm($id);
              dpm($entity_type);
              $entity = $entity_load($id);
              dpm($entity);
              $langcode = field_language($entity_type, $entity, $field_name);

              // Bypass unchanged source field lock
              $entity->original = NULL;

              $items = field_get_items($entity_type, $entity, $field_name, $langcode);

              // The save_callback for the user entity type as stored in $entity_info is incorrect
              // so it must be checked for so that the correct callback can be used.
              if ($entity_type != 'user') {
                // Check for values and if there are no values, reload the entity
                if (($field_info['type'] == 'geofield') && (!empty($save_callback))) {
                  if (empty($items['wkt'])) $save_callback($entity);
                }
                if (($field_info['type'] == 'location') && (!empty($save_callback))) {
                  if (empty($items['latitude'])) $save_callback($entity);
                }
                if (($field_info['type'] == 'geolocation') && (!empty($save_callback))) {
                  if (empty($items['lat'])) $save_callback($entity);
                }
              } else {
                dpm($id);
                dpm($entity);
                // Check for values and if there are no values, reload the user
                if ($field_info['type'] == 'geofield') {
                  if (empty($items['wkt'])) user_save($entity);
                }
                if ($field_info['type'] == 'location') {
                  if (empty($items['latitude'])) user_save($entity);
                }
                if ($field_info['type'] == 'geolocation') {
                  if (empty($items['lat'])) user_save($entity);
                }
                dpm('poker');
              }
            }
          }
        }
      }
    }
  }
}