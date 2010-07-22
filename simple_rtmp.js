/**
 * Hook for rendering either the RTMP URL field or Playlist Uploader based on stream type
**/
Drupal.behaviors.simple_rtmp_stream_type_handler = function(context) {
  var stream_type_elem = $("select[name=stream_type]");
  var rtmp_url_elem = $("input[name=rtmp_url]").parent();
  var playlist_elem = $('.playlist_container');
  
  //Grabs stream type val
  var getStreamType = function() {
    return stream_type_elem.val();
  }
  
  //Handles Stream type display
  var streamTypeHandler = function() {
    var val = getStreamType();
    //Hide Playlist container if type is rtmp
    if(val == 1) {
      rtmp_url_elem.show();
      playlist_elem.hide();
    }
    //Hide RTMP URL if type is playlist
    if(val == 2) {
      playlist_elem.show();
      //Expand playlist Element
      if(playlist_elem.is('.collapsed')) {
        Drupal.toggleFieldset(playlist_elem);
      }
      rtmp_url_elem.hide();
    }
  }
  
  //Fire off inital stream type handler
  streamTypeHandler();
  
  //Attach Event Listener for stream type changes
  stream_type_elem.change(streamTypeHandler);
}

/**
 * Hook for adding the video
**/