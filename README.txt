=====
Simple RTMP

-----
The Simple RTMP module creates a new node type called Simple RTMP. It was developed to be a simple way for displaying RTMP videos on a Drupal site. It supports both direct RTMP streams and XML playlists.

It uses some older releases of the JW Player and SWFObject because they work. Going forward newer versions will be integrated. Because of the included dependencies of the JW Player and swfobject, this module (to the best of my knowledge) cannot be an official Drupal module. 

-----
Simple RTMP Node Structure

stream_type: The type of RTMP stream. Current values are rtmp or playlist.
  - rtmp: Direct RTMP stream from a URL
  - playlist: Stream generated via an xml play list
  
rtmp_url: Stream URL. Only applicable if stream type is rtmp.

playlist_path: The path of the rtmp xml playlist file. Only applicable if stream type is playlist

icon_path: Path of video icon. 

video_width: Video display width.

video_height: Video display height.

video_auto_play: Set to auto play video on page load.