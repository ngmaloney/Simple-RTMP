/**
 * Helper function for adding a swf object video to a site
 * @param string container the DOM ID of the video container
 * @param string rtmp stream url (ex: )
 * @param obj params (see line 20 for specs)
 *
 * Author: Nicholas G. Maloney (nmaloney@bentley.edu)
 * Version: 1.0
**/ 
simpleRtmpVideo = function(container, rtmp, dparams) {
  this.player_instance;
  this.rtmp_array;
  this.rtmp_file;
  this.rtmp_path;
  this.params = {};
  this.player_instance;
  
  //Constants
  var PLAYLIST_WIDTH = 320; //Fixed width for video playlist
  var VIDEO_WIDTH = 360; //Default Video Width
  var VIDEO_HEIGHT = 280; //Default Vido Height
  
  //Define default params
  this.params.stream_type = 'rtmp';
  this.params.video_width = VIDEO_WIDTH;
  this.params.video_height = VIDEO_HEIGHT;
  this.params.autostart = false;
  this.params.flash_version = 9;
  this.params.player_name = 'simple_rtmp';
  this.params.playlist_orientation = 'right';
  this.params.playlist_size = PLAYLIST_WIDTH;
  
  /** Manually parse params to validate values 
  for(var param in dparams) {
    if(dparams.hasOwnProperty(param)) {
      params[param] = dparams[param];
    }
  }
  **/
    
  //Helper function for parsing param args
  params_parser = function(dparams) {
    //Set Player Path
    if(typeof(dparams.player_path) != 'undefined' && dparams.player_path != '') {
      this.params.player_path = dparams.player_path;
    }
    
    //Set Player Type
    if(typeof(dparams.stream_type) != 'undefined' && dparams.stream_type != '') {      
      this.params.stream_type = dparams.stream_type;
    }
    
    //Set Player Width
    if(typeof(dparams.video_width) != 'undefined' && dparams.video_width != 0) {
      this.params.video_width = dparams.video_width;
    }
    
    //Set Player Height
    if(typeof(dparams.video_height) != 'undefined' && dparams.video_height != 0) {
      this.params.video_height = dparams.video_height;
    }
    
    //Set Player Autostart
    if(typeof(dparams.autostart) != 'undefined') {
      if(parseInt(dparams.autostart) == 1) {
        this.params.autostart = true;  
      }
      else {
        this.params.autostart = false;  
      }      
    }
    
    //Set Playlist Orientation
    if(typeof(dparams.playlist_orientation) != 'undefined' && dparams.playlist_orientation != '') {
      this.params.playlist_orientation = dparams.playlist_orientation;
    }
    //Set Playlist Size
    if(typeof(dparams.playlist_size) != 'undefined' && dparams.playlist_size != 0) {
      this.params.playlist_size = dparams.playlist_size;
    }    
  }
  
  /**
   * Hook for adding a standard RTMP stream
  **/
  add_rtmp = function() {
    var params = this.params; //redeclare so I don't have to keep typing this.params
    this.player_instance = new SWFObject(params.player_path, params.player_name, params.video_width, params.video_height, params.flash_version, "#FFFFFF");
    this.player_instance.addParam("allowfullscreen", true);
    this.player_instance.addParam("allowscriptaccess", "always");
    this.player_instance.addParam("wmode", "opaque");
    this.player_instance.addVariable("width", params.video_width);
    this.player_instance.addVariable("height", params.video_height);
    this.player_instance.addVariable("displayheight", params.video_height);
    this.player_instance.addVariable("file", this.rtmp_path);
    this.player_instance.addVariable("id", this.rtmp_file);
    this.player_instance.addVariable("autostart", params.autostart);
    this.player_instance.write(this.container);
    return this.player_instance;
  }
  
  /**
   * Hook for adding a playlist based RTMP stream
  **/
  add_playlist = function() {
    var params = this.params; //redeclare so I don't have to keep typing this.params
    //These dimensions are for the player + playlist
    //var width = parseInt(params.video_width) + PLAYLIST_WIDTH; //Add width of playlist
    //var height = parseInt(params.video_height) + 20;
    var width = this._get_player_width();
    var height = this._get_player_height();
    this.player_instance = new SWFObject(params.player_path, params.player_name, width, height, params.flash_version, "#FFFFFF");
    this.player_instance.addParam("allowfullscreen", true);
    this.player_instance.addParam("allowscriptaccess", "always");
    this.player_instance.addParam("wmode", "opaque");
    this.player_instance.addVariable("width", width); //TODO: Make this height player_height + X
    this.player_instance.addVariable("height", height);
    this.player_instance.addVariable("displaywidth", params.video_width);
    this.player_instance.addVariable("displayheight", params.video_height);
    this.player_instance.addVariable("file", this.rtmp_path);
    this.player_instance.addVariable("id", this.rtmp_file);
    this.player_instance.addVariable("autostart", params.autostart);
    this.player_instance.addVariable('backcolor','0xEAEAEA');
    this.player_instance.addVariable('frontcolor','0x232323');
    this.player_instance.addVariable('lightcolor','0x636363');
    this.player_instance.write(this.container);
    return this.player_instance;    
  }
  
  /**
   * Helper function for getting player width
  **/
  _get_player_width = function() {
    if(this.params.stream_type == 'playlist') {
      if(this.params.playlist_orientation == 'right') {
        return parseInt(this.params.video_width) + parseInt(this.params.playlist_size);
      }
      else {
        return this.params.video_width;
      }
    }
    else {
     return this.params.video_width; 
    }
  }
  
  /**
   * Helper function for getting player height
  **/
  _get_player_height = function() {
    if(this.params.stream_type == 'playlist') {
      if(this.params.playlist_orientation == 'bottom') {
        return parseInt(this.params.video_height) + parseInt(this.params.playlist_size);
      }
      else {
        return this.params.video_height;
      }
    }
    else {
      return this.params.video_height;
    }
  }
  
  /**
   * Init Hook
  **/
  init = function(container, rtmp, dparams) {
    //parse RTMP info
    this.container = container;
    this.rtmp_array = rtmp.split("/"); 
    this.rtmp_file = self.rtmp_array[self.rtmp_array.length - 1];
    this.rtmp_path = rtmp.replace(self.rtmp_file,"");
    this.params_parser(dparams);
    if(this.params.stream_type == 'rtmp') {
     return this.add_rtmp();
    }
    if(this.params.stream_type == 'playlist') {
      this.rtmp_path = rtmp;
      return this.add_playlist();
    }
  }
  
  //Fire off hooks
  this.init(container, rtmp, dparams);
}