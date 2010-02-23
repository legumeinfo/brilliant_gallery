<?php

function testext($filename) {
  if (substr(strtolower($filename), -4, 4) == ".jpg" or substr(strtolower($filename), -5, 5) == ".jpeg" or substr(strtolower($filename), -4, 4) == ".gif" or substr(strtolower($filename), -4, 4) == ".png") {
    return true;
  }
  // Otherwise return false.
  return false;
}


function load_dir_to_array($absolpath, $imagewidth, $fullresolutionmaxwidth, $brilliant_gallery_sort, $imagecrop, $maximumnumbertoshow = '') {
  #watchdog('Brilliant Gal','imgw: '.$imagewidth.'/'.$imagecrop);
  # Load Directory Into Array
  $poct              = -1;
  $retval_dimensions = array();
  #$handle            = @opendir($absolpath);
  $actualpath        = realpath(file_directory_path()) . $absolpath;
  #watchdog('Brilliant Gal','absol3: '.$actualpath);
  $handle            = @opendir($actualpath);
  $imagemaxh         = 0;

  // Load the directory into an array first.
  $filearray = array();
  while ($file = @readdir($handle)) {
    $testending = strtolower(substr($file, -4, 4));
    if (!testext($file) and strtolower($testending) <> '.mpg' and strtolower($testending) <> '.swf' and strtolower($testending) <> '.mov' and strtolower($testending) <> '.avi') {
      continue;
    }
    $filearray[] = $file;
  }
  @closedir($handle);

  if(empty($filearray)){
    watchdog('Brilliant Gal', 'No displayable images in '.$absolpath.'! Elapsed time: '.(time()-$timenow).' seconds.');
  } else {
    foreach($filearray as $file){
      # Index of a real image or something we are going to display.
      $poct += 1;
      $retval_dimensions[$poct]['file'] = $file; 
      $retval_dimensions[$poct]['imgcrop'] = $imagecrop;

      # Is image horizontally or vertically oriented?
      #$temp = getimagesize($absolpath .'/'. $file);
      $temp = getimagesize($actualpath .'/'. $file);
      if ($temp === false) {
        continue;
      }
      if ((($temp[0] - $temp[1]) >= 0) OR $maximumnumbertoshow == 1) {
        // This is a horizontal image.
        // Treat single images just as horizontal images (no need to fit their height to the height of horizontals)! The specified width of an image is authoritative for both horizontal images and also vertical if they show alone.
        if ($imagecrop == 'yes'){
          $retval_dimensions[$poct]['imgw'] = $imagewidth;
          $retval_dimensions[$poct]['imgh'] = $imagewidth;
        }
        else {
          $retval_dimensions[$poct]['imgw'] = $imagewidth;
          $retval_dimensions[$poct]['imgh'] = round(($temp[1] / $temp[0]) * $imagewidth);
          #watchdog('Brilliant Gal','1imgw: '.$imagewidth.'/'.$imagecrop);
        }
        $retval_dimensions[$poct]['imgwbig'] = $fullresolutionmaxwidth;
        $retval_dimensions[$poct]['imghbig'] = round(($temp[1] / $temp[0]) * $fullresolutionmaxwidth);
      }
      else {
        // This is a vertical image
        if ($imagecrop == 'yes'){
          $retval_dimensions[$poct]['imgw'] = round(($temp[0] / $temp[1]) * $imagewidth);
          $retval_dimensions[$poct]['imgh'] = $imagewidth;
        }
        else {
          $retval_dimensions[$poct]['imgw'] = round(($temp[0] / $temp[1]) * ($temp[0] / $temp[1]) * $imagewidth);
          $retval_dimensions[$poct]['imgh'] = round(($temp[0] / $temp[1]) * $imagewidth);
          #watchdog('Brilliant Gal','2imgw: '.$imagewidth.'/'.$imagecrop);
          #watchdog('Brilliant Gal','2imgw: '.$temp[0].'-'.$temp[1]);
        }
        $retval_dimensions[$poct]['imgwbig'] = round(($temp[0] / $temp[1]) * ($temp[0] / $temp[1]) * $fullresolutionmaxwidth);
        $retval_dimensions[$poct]['imghbig'] = round(($temp[0] / $temp[1]) * $fullresolutionmaxwidth);
      }
      // In $imagemaxh collect the maximum vertical size of the gallery.
      if ($imagemaxh < $retval_dimensions[$poct]['imgh']) {
        $imagemaxh = $retval_dimensions[$poct]['imgh'];
      }
    }
  }

  if ($brilliant_gallery_sort == '1') {
    @sort($retval_dimensions);
  }
  else {
    shuffle($retval_dimensions);
  }
  return array($retval_dimensions, $imagemaxh);
}
