#
#Created on 2023-10-22 by Jacob B. (TheTepidCoder)
#Purpose: To render animated 3D character models for use in 2D isometric games/animations.
#Methodology: I clamp the camera to a bezier circle; set it as the parent of the circle;
#and rotate the circle. This causes the camera to rotate along a circular path. Finally, I
#set the camera to track an invisble object situated in the center of the character to capture.
#For each rotation, each key frame of the animation will be rendered to a file.
#
#Required setup steps:
#-Modify the call to render_animated_isometric_character to include the full directory
#path to which you'd like to export the rendered frames.
#E.g. str_render_directory_path = 'c:/blender_output/some_project_name/'
#-Create an invisible cube object to place inside of the character that you want to render.
#(In its viewport, uncheck its render state.)
#-Create a bezier circle.
#-Clamp the camera to the bezier circle's Z axis through a new constraint.
#-Add a TrackTo constraint to the camera on the unrendered object.
#(Should be -Z and Y in the settings for the TrackTo constraint in the camera on the cube.)
#-Move the bezier circle above the character model so that the camera looks down on it
#while following the circle (because of the clamp).
#-Scale the circle to be wide enough that the camera is looking upon the character at
#as close to a 45 degree angle as possible.
#
#Notes:
#I find it easier to situate the character object and bezier circle at (0, 0, 0) and
#transform the circle's Z coordinate to be above the model to capture.
#Additional notes:
#This script will automatically determine which frames are key frames and export only those
#ones.
#All exported frames will be in the format:
#[path]/[blend_project_name].blend_[angle].[three_digit_frame_index]
#
#Attribution notes:
#I took the camera pathing/tracking setup method from
# https://opengameart.org/forumtopic/how-to-make-a-2d-isometric-spritesheet-from-a-3d-model-using-blender
#and made my own script to do the same thing (albeit with some automation with the frame range and
#filtration specifically for key frames) in blender version 3.6.5.
#So, I want to give a huge thanks to the user, VenetianBears! If not for his/her write up to push me
#in the correct direction (even with hotkey changes and Python script API call deprecations),
#it would have taken me much longer to understand the required process and to get this script working.
#
# This script is licensed under GPLv3 because it could be considered to be derivative of Blender for
# calling its API functions via Python.
#
# While I require no personal attribution from anyone who wishes to use and/or redistribute this
# script, I'd greatly appreciate it if you were to give back any alterations that you make to the
# source code so that any other users can enjoy it! Maybe fork the project on Github or something,
# if you make any changes. I'd love to know, if/how you've improved/updated the script.
#
# With regard to contributions, in particular, I think that it'd be cool to make it so that
# the bezier curve creation, the camera positioning & parenthood, and the ClampTo and TrackTo
# settings were automated such that the camera is always at such a hight so as to view the
# character to be rendered at a 45 degree angle with the character sprite filling as much of
# the rendered frame as possible to obviate the need for any latent scaling of the images.
#
# Also, it'd be great to automate the position(s) of lamps around the model in 45 degree
# increments along the bezier circle for consistent lighting. From what I've read recently,
# it seems like some renderer options in Blender may allow for global illumination, which is
# nice, if true. Still, it'd be nice to have an easy and consistent method to mitigate human
# error.
#

import bpy
from math import radians, ceil
from os.path import join
import time

#Get an unsorted, unique array (set) of zero-based frame indices which hold key frames in the
#current animation.
def get_key_frame_index_array (  ):
  set_key_frames = set (  )
  for obj_action in bpy.data.actions:
    for obj_channel in obj_action.fcurves:
      for obj_key in obj_channel.keyframe_points:
        if obj_key.select_control_point:
          set_key_frames .add ( ceil ( obj_key.co.x ) )
  return set_key_frames

#Rotate the camera which actually captures the scene when rendering.
def rotate_camera ( obj_camera, f_angle_in_degrees ):
  obj_camera.rotation_euler.z = radians ( f_angle_in_degrees )
  #update the scene to use the new camera position.
  bpy.context.view_layer.update (  )
  #A hack to force the scene to redraw before the Python script completes.
  bpy.ops.wm.redraw_timer(type='DRAW_WIN_SWAP', iterations=1)

#Call this with the directory path (ending with a '/') where you want
#the animation's key frames to be rendered.
def render_animated_isometric_character (
  str_render_directory_path : str
):


  #The position of the camera relative to the scene that it's capturing it.
  #E.G. the camera is "behind" the scene @ index 0.
  ary_camera_relative_position_names = [
   "behind", "behind_right", "right", "front_right",
   "front", "front_left", "left", "behind_left"
  ]
  #This presumes that the camera is looking at a scene with nothing but a character.
  #This will describe the direction in which the character would be walking, from the
  #perspective of a user viewing the scene that is being captured by the camera, given
  #its facing direction.
  ary_character_direction_names = [
   "up", "up_right", "right", "down_right",
   "down", "down_left", "left", "up_left"
  ]

  #Alias the scene object.
  obj_scene = bpy.context.scene

  #front, left, right, back, and 45 degrees between each of them = 4*4 = 8.
  i_number_of_angles = 8
  #Evenly divide a full 360 degree (2pi radian) rotation into the 8 directions that we need.
  f_angle_delta_per_iteration = 360 / i_number_of_angles

  #If you named the bezier circle to which you've clamped the camera,
  #differently, update 'BezierCircle' to match the name of your object.
  obj_camera = bpy.data.objects [ 'BezierCircle' ]

  #Figure out which animation frame indices are key frames so that we can
  #save just those to files during the rendering process.
  ary_key_frames = get_key_frame_index_array (  )

  #Just for debugging, show the key frame indices that we've found so that we can
  #tell whether any frames are missing/extra, if the rendered animation doesn't look
  #correct from any of the perspectives.
  print ( "Key frame indices: " + str ( ary_key_frames ) )


  #For each angle that we need to rotate.
  for i_angle_index in range ( i_number_of_angles ):

      #print ( "Rendering frames for \"" + ary_camera_relative_position_names [ i_angle_index ] + "\" direction." )
      print ( "Direction of character from viewer's perspective: " + str ( ary_character_direction_names [ i_angle_index ] ) )

      #Note: We're setting the angle of the bezier circle to which the camera
      #should be clamped on the Z axis. This should cause the camera to rotate
      #with it thereby following its path.
      f_current_angle = i_angle_index * f_angle_delta_per_iteration
      print ( "Current angle: " + str ( f_current_angle ) )
      rotate_camera ( obj_camera, f_current_angle )

      #The actual index may jump around, since we're rendering only the key frames,
      #so the file name used for each rendered frame will include the key frame's
      #index for this particular perspective from the camera.
      i_effective_frame_index = 0

      #Render each of the key frames in the list that we gathered.
      for i_frame_index in ary_key_frames:

        #Choose the frame to render by its index.
        obj_scene .frame_set ( i_frame_index )

        #Set the file path to use for rendering to the user-defined directory
        #appended with the file name consisting of the project name, angle index,
        #frame number, and file extension.
        obj_scene.render.filepath = "{directory_path}{project}_{angle}.{frame_number}{extension}" .format (
          directory_path = str_render_directory_path,
          project = bpy.path.basename ( bpy.data.filepath ),
          angle = i_angle_index,
          frame_number = str ( i_effective_frame_index ) .zfill ( 3 ), #three digit base-10 frame identifier.
          extension = obj_scene .render .file_extension
        )

        #Render the image to the file path specified hereinabove.
        bpy .ops .render .render ( write_still = True )

        #Track how many key frames we've handled.
        i_effective_frame_index += 1

    #For each key frame to render in the current perspective

  #For each angle from which we want to view the scene
#End of render_animated_isometric_character subroutine


#Camera initial test for bezier circle and camera pairing with hidden object tracking.
#This is useful before any animation key frames are setup for exporting.
def test_camera (  ):
  obj_camera = bpy.data.objects [ 'BezierCircle' ]
  for i in range ( 0, 8 ):
    rotate_camera ( obj_camera, 45*i )
    time .sleep ( 0.5 )
  raise RuntimeError ( "Ending early." )


test_camera (  ) #uncomment to test.

#Render the key frames of our character's animations from 8 perspectives
#(up, down, left, right, and 45 degrees between each).
render_animated_isometric_character ( "c:/path/to/your/output/directory/" )


"""
Copyright (C) 2023 Jacob B. (TheTepidCoder)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <https://www.gnu.org/licenses/>.

    Contact information:
      https://x.com/TheTepidCoder
"""
