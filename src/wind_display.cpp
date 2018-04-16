#include <stdlib.h>     /* srand, rand */
#include <iostream>

#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreSceneManager.h>

#include <tf/transform_listener.h>

#include <rviz/visualization_manager.h>
#include <rviz/properties/color_property.h>
#include <rviz/properties/float_property.h>
#include <rviz/properties/int_property.h>
#include <rviz/frame_manager.h>

#include "wind_visual.h"

#include "wind_display.h"

namespace wind_visualization
{
  WindDisplay::WindDisplay()
  {
//    color_property_ = new rviz::ColorProperty( "Color", QColor( 204, 51, 204 ),
  //                                             "Color to draw the acceleration arrows.",
    //                                           this, SLOT( updateColorAndAlpha() ));

    alpha_property_ = new rviz::FloatProperty( "Alpha", 1.0,
                                               "0 is fully transparent, 1.0 is fully opaque.",
                                               this, SLOT( updateColorAndAlpha() ));

    history_length_property_ = new rviz::IntProperty( "History Length", 1,
                                                      "Number of prior measurements to display.",
                                                      this, SLOT( updateHistoryLength() ));
    history_length_property_->setMin( 100 );
    history_length_property_->setMax( 100000 );
  }

  void WindDisplay::onInitialize()
  {
    MFDClass::onInitialize();
    updateHistoryLength();
  }

  WindDisplay::~WindDisplay()
  {
  }

  void WindDisplay::reset()
  {
    MFDClass::reset();
    visuals_.clear();
  }

/*  void WindDisplay::updateColorAndAlpha()
  {
    float alpha = alpha_property_->getFloat();
    Ogre::ColourValue color; // = color_property_->getOgreColor();

    for( size_t i = 0; i < visuals_.size(); i++ )
    {
      visuals_[i]->setColor( rand()*255, rand()*255, rand()*255, alpha );
    }
  }
*/

  void WindDisplay::updateHistoryLength()
  {
    visuals_.rset_capacity(history_length_property_->getInt());
  }

  // This is our callback to handle an incoming message.
  void WindDisplay::processMessage( const wind_visualization::Wind::ConstPtr& msg )
  {
    Ogre::Quaternion orientation;
    Ogre::Vector3 position;
    if( !context_->getFrameManager()->getTransform( msg->header.frame_id,
                                                  msg->header.stamp,
                                                  position, orientation))
    {
      ROS_DEBUG( "Error transforming from frame '%s' to frame '%s'",
                 msg->header.frame_id.c_str(), qPrintable( fixed_frame_ ));
      return;
    }

    boost::shared_ptr<WindVisual> visual;
    if( visuals_.full() )
    {
      visual = visuals_.front();
    }
    else
    {
      visual.reset(new WindVisual( context_->getSceneManager(), scene_node_ ));
    }

    visual->setMessage( msg );
    visual->setFramePosition( position );

//    visual->setFrameOrientation( orientation );

    float alpha = alpha_property_->getFloat();
//    Ogre::ColourValue color; // = color_property_->getOgreColor();
    const geometry_msgs::Vector3& color = msg->wind;

/*
    float nrm = sqrt(color.x * color.x + color.y * color.y + color.z * color.z) * 2;

    float r = (color.x/nrm + 0.5);
    float g = (color.y/nrm + 0.5);
    float b = (color.z/nrm + 0.5);
*/



    float r = 0;
    float g = 0;
    float b = 0;
/*
    if( color.x * color.x > color.y * color.y )
    {
      if (color.x >= 0)
        r = 1;
      else
      {
        r = 1;
        g = 1;
      }
    }
    else
    {
      if (color.y >= 0)
        g = 1;
      else
        b = 1;
    }
*/

    float nrm = sqrt(color.x * color.x + color.y * color.y + color.z * color.z);

    if ( nrm <= 1/4 )
      b = 0.5 + 2*nrm;
    else if( nrm <= 3/4 )
    {
      b = 1;
      g = 2*nrm - 0.5;
    }
    else if( nrm <= 5/4 )
    {
      r = 2*nrm - 1.5;
      g = 1;
      b = -2*nrm + 2.5;
    }
    else if( nrm <= 7/4 )
    {
      r = 1;
      g = -2*nrm + 3.5;
    }
    else
      r = -2*nrm + 4.5;

    visual->setColor( r, g, b, alpha );
    visuals_.push_back(visual);
  }

} // end namespace rviz_plugin_tutorials


#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(wind_visualization::WindDisplay, rviz::Display )
