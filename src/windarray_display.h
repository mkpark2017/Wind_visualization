#ifndef WIND_DISPLAY_H
#define WIND_DISPLAY_H

#include <boost/circular_buffer.hpp>

#include <wind_visualization/WindArray.h>
#include <rviz/message_filter_display.h>


namespace Ogre
{
  class SceneNode;
}

namespace rviz
{
  class ColorProperty;
  class FloatProperty;
  class IntProperty;
}

namespace wind_visualization
{

  class WindVisual;


  class WindDisplay: public rviz::MessageFilterDisplay<wind_visualization::Wind>
  {
    Q_OBJECT
    public:
      WindDisplay();
      virtual ~WindDisplay();

    protected:
      virtual void onInitialize();
      virtual void reset();


    private Q_SLOTS:
      void updateColorAndAlpha();
      void updateHistoryLength();

    private:
      void processMessage( const wind_visualization::Wind::ConstPtr& msg);

      boost::circular_buffer<boost::shared_ptr<WindVisual> > visuals_;

      rviz::ColorProperty* color_property_;
      rviz::FloatProperty* alpha_property_;
      rviz::IntProperty* history_length_property_;
  };

}

#endif
