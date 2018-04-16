#include <OGRE/OgreVector3.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreSceneManager.h>

#include <rviz/ogre_helpers/arrow.h>

#include "wind_visual.h"

namespace wind_visualization
{
  WindVisual::WindVisual( Ogre::SceneManager* scene_manager, Ogre::SceneNode* parent_node )
  {
    scene_manager_ = scene_manager;

    frame_node_ = parent_node ->createChildSceneNode();

    wind_velocity_arrow_.reset(new rviz::Arrow( scene_manager_, frame_node_ ));
  }

  WindVisual::~WindVisual()
  {
    scene_manager_->destroySceneNode( frame_node_ );
  }

  void WindVisual::setMessage( const wind_visualization::Wind::ConstPtr& msg )
  {
    const geometry_msgs::Vector3& a = msg->wind;

    Ogre::Vector3 acc( a.x, a.y, a.z );
    float length = acc.length();
    Ogre::Vector3 scale( length, length, length );
    wind_velocity_arrow_->setScale( scale );
    wind_velocity_arrow_->setDirection( acc );
  }

  void WindVisual::setFramePosition( const Ogre::Vector3& position )
  {
    frame_node_->setPosition( position );
  }

  void WindVisual::setFrameOrientation( const Ogre::Quaternion& orientation )
  {
    frame_node_->setOrientation( orientation );
  }

  void WindVisual::setColor( float r, float g, float b, float a )
  {
    wind_velocity_arrow_->setColor( r, g, b, a );
  }

}
