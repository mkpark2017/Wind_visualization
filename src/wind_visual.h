#ifndef WIND_VISUAL_H
#define WIND_VISUAL_H

#include <wind_visualization/Wind.h>

namespace Ogre
{
  class Vector3;
  class Quaternion;
}

namespace rviz
{
  class Arrow;
}

namespace wind_visualization
{
  class WindVisual
  {
    public:
      WindVisual( Ogre::SceneManager* scene_manager, Ogre::SceneNode* parent_node );

      virtual ~WindVisual();
      void setMessage( const wind_visualization::Wind::ConstPtr& msg );

      void setFramePosition( const Ogre::Vector3& position );
      void setFrameOrientation( const Ogre::Quaternion& orientation );

      void setColor( float r, float g, float b, float a );

    private:

      boost::shared_ptr<rviz::Arrow> wind_velocity_arrow_;


      Ogre::SceneNode* frame_node_;

      Ogre::SceneManager* scene_manager_;
  };
} // end namespace

#endif // WIND_VISUAL_H
