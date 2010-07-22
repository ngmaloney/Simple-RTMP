<?php

/** 
 * Implementation of hook_node_info()
**/
function simple_rtmp_node_info() {
  return array(
    'simple_rtmp' => array(
      'name' => t('Simple RTMP Video'),
      'module' => 'simple_rtmp',
      'description' => t("This is a simple rtmp node used for displaying rtmp videos."),
      'has_title' => TRUE,
      'title_label' => t('Video Title'),
      'has_body' => TRUE,
      'body_label' => t('Video Body'),
    )
  );
}

/** 
 * Implementation of hook_form
**/
function simple_rtmp_form(&$node) {
  //Add JS for effects
  $js_path = drupal_get_path('module','simple_rtmp') . '/simple_rtmp.js';
  drupal_add_js($js_path);
  
  $type = node_get_types('type', $node);  
  $form = array('#attributes' => array('enctype' => 'multipart/form-data'));

  if ($type->has_title) {
    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => check_plain($type->title_label),
      '#required' => TRUE,
      '#default_value' => $node->title,
      '#weight' => -5,
    );
  }
  
  if ($type->has_body) {
    $form['body_field'] = node_body_field($node, $type->body_label, $type->min_word_count);
  }

  $form['stream_type'] = array(
     '#type' => 'select',
     '#title' => t('Stream Type'),
     '#default_value' => isset($node->stream_type) ? $node->stream_type : 0,
     '#options' => array(
       1 => 'rtmp',
       2 => 'playlist',
      ),
      '#description' => t('The type of RTMP stream. Current types are rtmp or playlist.'),
  );
  
  $form['rtmp_url'] = array(
    '#type' => 'textfield',
    '#title' => t('RTMP URL'),
    '#default_value' => isset($node->rtmp_url) ? $node->rtmp_url : NULL,
    '#size' => 60, 
    '#maxlength' => 128, 
    '#required' => FALSE,
  );
  
  //Playlist File
  $form['playlist'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Playlist Settings'), 
    '#weight' => 3, 
    '#collapsible' => TRUE, 
    '#collapsed' => TRUE,
    '#attributes' => array('class' => 'playlist_container'),
  );
  
  if($node->playlist_file) {
    //Display link to file with change and delete links
    $form['playlist']['playlist_file'] = array(
      '#type' => 'markup',
      '#value' => "<span>Playlist File: " . _simple_rtmp_get_file_link($node->playlist_file, 'playlist_link') . "</span>",
    );
    
    //Display delete link
    $form['playlist']['playlist_file_delete'] = array(
      '#type' => 'checkbox',
      '#title' => 'Delete playlist file',
    );
  }
  
  $form['playlist']['playlist_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Playlist Size'),
    '#default_value' => isset($node->playlist_size) ? $node->playlist_size : 320,
    '#size' => 5, 
    '#maxlength' => 5, 
    '#required' => FALSE,
  );
  
  $form['playlist']['playlist_orientation'] = array(
    '#type' => 'select',
    '#title' => t('Playlist Orientation'),
    '#default_value' => isset($node->playlist_orientation) ? $node->playlist_orientation : 'right',
    '#options' => array(
      'right' => 'Right',
      'bottom' => 'Bottom'
    ),
    '#description' => t('The position of the playlist in relation to the video. Either on the right side or below.')    
  );
  
  $form['playlist']['playlist_file_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload a playlist file'),
    '#size' => 40,
  );
  
  //Video Icon File
  $form['icon'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Icon Settings'), 
    '#weight' => 4, 
    '#collapsible' => TRUE, 
    '#collapsed' => TRUE,
    '#attributes' => array('class' => 'icon_container'),    
  );
  
  if($node->icon_file) {
    $form['icon']['icon_file'] = array(
      '#type' => 'markup',
      '#value' => "<span>Icon File: " . _simple_rtmp_get_file_link($node->icon_file, 'icon_link') . "</span>",
    );
    
    $form['icon']['icon_file_delete'] = array(
      '#type' => 'checkbox',
      '#title' => 'Delete icon file',
    );
  }
  
  $form['icon']['icon_file_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload a video icon'),
    '#size' => 40,
  );
  
  //Video Attributes
  $form['video_attributes'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Video Settings'), 
    '#weight' => 5, 
    '#collapsible' => TRUE, 
    '#collapsed' => TRUE,
  );
  
  $form['video_attributes']['video_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Video Width'),
    '#default_value' => isset($node->video_width) ? $node->video_width : NULL,
    '#size' => 5, 
    '#maxlength' => 5, 
    '#required' => FALSE,
  );
  
  $form['video_attributes']['video_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Video Width'),
    '#default_value' => isset($node->video_height) ? $node->video_height : NULL,
    '#size' => 5, 
    '#maxlength' => 5, 
    '#required' => FALSE,
  );
  
  $form['video_attributes']['video_autoplay'] = array(
    '#type' => 'select',
    '#title' => t('Video Auto Play'),
    '#default_value' => isset($node->video_height) ? $node->video_autoplay : 0,
    '#options' => array(
      0 => 'no',
      1 => 'yes',
     ),
  );
  
  //Specify Custom validation hook
  $form['#validate'] = array('simple_rtmp_node_form_validate');
  return $form;
}

/** 
 * Custom validation hook
 *
 * This hook needs to use custom validation because the default node hook_validate doesn't seem to support 
 * File uploads. See: http://www.imedstudios.com/labs/node/22
**/
function simple_rtmp_node_form_validate($form, &$form_state) {
  //Add JS for effects
  $js_path = drupal_get_path('module','simple_rtmp') . '/simple_rtmp.js';
  drupal_add_js($js_path);
  
  //Validate stream type
  if($form['stream_type']) {
    if($form['stream_type']['#value'] != 1 && $form['stream_type']['#value'] != 2) {
      form_set_error('stream_type', t('Invalid stream type specified.'));
    }    
  }
  
  //Validate rtmp_url
  if($form['rtmp_url']["#value"]) {
    $stream = strtolower($form['rtmp_url']['#value']);
    if(substr($stream,0,4) != 'rtmp' && substr($stream,0,4) != 'http') {
      form_set_error('rtmp_url', t('Invalid url specified for RTMP Stream URL.'));
    }
  }  
  
  //Handle conditional playlist type validation
  
  //Ensure an rtmp url is specified if stream type is rtmp
  if($form['stream_type']['#value'] == 1 && empty($form['rtmp_url']["#value"])) {
    form_set_error('rtmp_url', t('RTMP URL required for specified stream type.'));
  }
  
  //Handle uploading of playlist file
  if(!empty($_FILES['files']['tmp_name']['playlist_file_upload'])) {
    $validators = array(
      'file_validate_extensions' => array('xml')
    );
    
    //Handle path creation
    $path = SIMPLE_RTMP_DIR . '/playlists';
    if(!file_exists($path)) {
      mkdir($path,0775,TRUE);
    }    
    $playlist_file = file_save_upload('playlist_file_upload', $validators, $path);
    
    if (!$playlist_file)
      form_set_error('playlist_file_upload', 'Invalid playlist file uploaded.');
    else {
      $form_state['values']['playlist_file'] = $playlist_file;
    }
  }
  
  //Handle uploading of icon file
  if(!empty($_FILES['files']['tmp_name']['icon_file_upload'])) {
    $validators = array(
      'file_validate_is_image' => array(),
    );
    
    //Handle path creation
    $path = SIMPLE_RTMP_DIR . '/icons';
    if(!file_exists($path)) {
      mkdir($path,0775,TRUE);
    }    
    
    $icon_file = file_save_upload('icon_file_upload', $validators, $path);
    
    if (!$icon_file)
      form_set_error('icon_file_upload', 'Invalid icon file uploaded.');
    else {
      $form_state['values']['icon_file'] = $icon_file;
    }
  }
}

/**
 * Implementation of hook_view()
**/
function simple_rtmp_view($node, $teaser = FALSE, $page = FALSE) {
  $node = node_prepare($node, $teaser);
  return $node;
}

/**
 * Implementation of hook_insert()
**/
function simple_rtmp_insert($node) {
  
  //Handle playlist and icon files
  $playlist_fid = NULL;
  $icon_fid = NULL;
  if(!empty($node->playlist_file->fid)) {
   $playlist_fid = $node->playlist_file->fid;
   file_set_status($node->playlist_file, FILE_STATUS_PERMANENT); 
  }
  if(!empty($node->icon_file->fid)) {
    $icon_fid = $node->icon_file->fid;
    file_set_status($node->icon_file, FILE_STATUS_PERMANENT); 
  }
  
  //DB Insert
  db_query("INSERT INTO {simple_rtmp} 
    (vid, nid, stream_type, rtmp_url, playlist_file, icon_file, video_width, video_height, video_autoplay, playlist_size, playlist_orientation)
    VALUES (%d, %d, %d, '%s', '%s', '%s', %d, %d, %d, %d, '%s')",
    $node->vid, $node->nid, $node->stream_type, $node->rtmp_url, $playlist_fid, $icon_fid, $node->video_width, $node->video_height, $node->video_autoplay, $node->playlist_size, $node->playlist_orientation
  );
}


/**
 * Implementation of hook_update()
**/
function simple_rtmp_update($node) {
  
  //Handle playlist and icon files
  $og_node = node_load($node->nid); //Grab node's current db vals
  $playlist_fid = $og_node->playlist_file;
  $icon_fid = $og_node->icon_file;
  
  //Set status of playlist file and delete the old one
  if(!empty($node->playlist_file->fid)) {
   $playlist_fid = $node->playlist_file->fid;
   file_set_status($node->playlist_file, FILE_STATUS_PERMANENT); 
   if($og_node && ($og_node->playlist_file != $playlist_fid)) {
     _simple_rtmp_delete_file($og_node->playlist_file);
   } 
  }
  
  //Set status of the icon file and delete the old one
  if(!empty($node->icon_file->fid)) {
    $icon_fid = $node->icon_file->fid;
    file_set_status($node->icon_file, FILE_STATUS_PERMANENT); 
     if($og_node && ($og_node->icon_file != $icon_fid)) {
       _simple_rtmp_delete_file($og_node->icon_file);
     }
  }
  
  //Handle deletions of playlist file
  if($node->playlist_file_delete == 1) {
    _simple_rtmp_delete_file($og_node->playlist_file);
    $playlist_fid = NULL;
  }
  
  //Handle deletions of playlist file
  if($node->icon_file_delete == 1) {
    _simple_rtmp_delete_file($og_node->icon_file);
    $icon_fid = NULL;
  }
  
  if($node->revision) {
    simple_rtmp_insert($node);
  }
  else {
    db_query("UPDATE {simple_rtmp} 
      SET stream_type = %d,
          rtmp_url = '%s',
          playlist_file = '%s',
          icon_file = '%s',
          video_width = %d,
          video_height = %d,
          video_autoplay = %d,
          playlist_size = %d,
          playlist_orientation = '%s'
      WHERE vid = %d",
      $node->stream_type, $node->rtmp_url, $playlist_fid, $icon_fid, $node->video_width, $node->video_height, $node->video_autoplay, $node->playlist_size, $node->playlist_orientation, $node->vid
    );
  }
}

/**
 * Implementation of hook_delete()
**/
function simple_rtmp_delete($node) {
  //Delete Playlist File
  _simple_rtmp_delete_file($node->playlist_file);
  //Delete Icon File
  _simple_rtmp_delete_file($node->icon_file);
  //Delete Node
  node_delete($node->nid);
  //Delete simple_rtmp row
  db_query('DELETE FROM {simple_rtmp} WHERE nid = %d', $node->nid);
}

/**
 * Implementation of hook_load()
**/
function simple_rtmp_load($node) {
    return db_fetch_object(db_query('SELECT * FROM {simple_rtmp} WHERE vid = %d', $node->vid));
}

/** 
 * Implementation of hook_nodeapi
**/
function simple_rtmp_nodeapi(&$node) {
  switch ($op) {
     case 'delete revision':
       // Notice that we're matching a single revision based on the node's vid.
       db_query('DELETE FROM {simple_rtmp} WHERE vid = %d', $node->vid);
       break;
   }
}