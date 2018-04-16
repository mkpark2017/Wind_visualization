#!/usr/bin/env python

import roslib; roslib.load_manifest( 'wind_visualization' )
from wind_visualization.msg import Wind
import rospy
from math import cos, sin
import tf
from random import *

topic = 'test_wind'
publisher = rospy.Publisher( topic, Wind, queue_size=10 )

rospy.init_node( 'test_wind' )

br = tf.TransformBroadcaster()
rate = rospy.Rate(1)
radius = 5
angle = 0

dist = 3
while not rospy.is_shutdown():

    wind = Wind()
    wind.header.frame_id = "firefly/base_link"
    wind.header.stamp = rospy.Time.now()

    wind.wind.x = 2*random()-0.25;
    wind.wind.y = random()-0.25;
    wind.wind.z = random()-0.25;

    publisher.publish( wind )

   # br.sendTransform((radius * cos(angle), radius * sin(angle), 0),
    #                 tf.transformations.quaternion_from_euler(0, 0, angle),
     #                rospy.Time.now(),
      #               "base_link",
       #              "map")
   # angle += .01
    rate.sleep()

