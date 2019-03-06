#include <ros/ros.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Pose.h>
#include <tf/transform_broadcaster.h>

#include <wind_visualization/Wind.h>

#include <OGRE/OgreVector3.h>

#include <iostream>

wind_visualization::Wind wind_data;

class wind_data_class
{
  public:
    explicit wind_data_class(ros::NodeHandle nh) : m_nh(nh)
    {
      m_sub = m_nh.subscribe ("/firefly/ground_truth/pose", 1, &wind_data_class::windGenerate, this);
    }

  private:
    ros::NodeHandle m_nh;
    ros::Subscriber m_sub;

  void windGenerate(const geometry_msgs::PosePtr& msg);
};


void wind_data_class::windGenerate(const geometry_msgs::PosePtr& msg)
{
  geometry_msgs::Point _P = msg->position;
//  geometry_msgs::Quaternion _O = msg -> orientation;

//  Ogre::Vector3 pp(_P.x, _P.y, _P.z);

//  float r = _O.w;
//  float i = -_O.x;
//  float j = -_O.y;
//  float k = -_O.z;

//  wind_data.wind.x = 2*(r*_P.z*j + i*_P.z*k - r*_P.y*k + i*_P.y*j) + _P.x*(r*r + i*i - j*j - k*k);
//  wind_data.wind.y = 2*(r*_P.x*k + i*_P.x*j - r*_P.z*i + j*_P.z*k) + _P.y*(r*r - i*i + j*j - k*k);
//  wind_data.wind.z = 2*(r*_P.y*i - r*_P.x*j + i*_P.x*k + j*_P.y*k) + _P.z*(r*r - i*i - j*j + k*k);
  wind_data.wind.x = _P.x/10+0.1;
  wind_data.wind.y = _P.y/10;
  wind_data.wind.z = _P.z/30-0.3;

  if (_P.z <= 7)
  {
    wind_data.wind.x = 0;
    wind_data.wind.y = 0;
    wind_data.wind.z = 0;
  }

  float norm = sqrt(wind_data.wind.x * wind_data.wind.x + wind_data.wind.y * wind_data.wind.y + wind_data.wind.z * wind_data.wind.z);

  if ( norm > 2)
  {
    wind_data.wind.x = 2*wind_data.wind.x/norm;
    wind_data.wind.y = 2*wind_data.wind.y/norm;
    wind_data.wind.z = 2*wind_data.wind.z/norm;
  }

//  wind_data.position.x = _P.x;
//  wind_data.position.y = _P.y;
//  wind_data.position.z = _P.z;

//  wind_data.orientation.x = _O.x;
//  wind_data.orientation.y = _O.y;
//  wind_data.orientation.z = _O.z;
//  wind_data.orientation.w = _O.w;

}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "Wind_test");

  ros::NodeHandle nh;

  ros::Rate r(1);
  ros::Publisher m_pub = nh.advertise <wind_visualization::Wind> ("wind_data", 10);
  wind_data_class wind(nh);

  while(nh.ok())
  {
      ros::spinOnce();			//check the incoming messages
      wind_data.header.frame_id = "firefly1/base_link";
      wind_data.header.stamp = ros::Time::now();

      m_pub.publish(wind_data);

      r.sleep();
  }

}
