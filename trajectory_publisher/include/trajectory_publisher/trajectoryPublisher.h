//  May/2018, ETHZ, Jaeyoung Lim, jalim@ethz.ch

#ifndef TRAJECTORYPUBLISHER_H
#define TRAJECTORYPUBLISHER_H

#include <stdio.h>
#include <cstdlib>
#include <string>
#include <sstream>
#include <Eigen/Dense>

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int32.h>
#include <geometry_msgs/TwistStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <std_srvs/SetBool.h>
#include <nav_msgs/Path.h>
#include <mav_planning_msgs/PolynomialTrajectory4D.h>

#include "trajectory_publisher/trajectory.h"
#include "trajectory_publisher/polynomialtrajectory.h"
#include "trajectory_publisher/shapetrajectory.h"

using namespace std;
using namespace Eigen;
class trajectoryPublisher
{
private:
  ros::NodeHandle nh_;
  ros::NodeHandle nh_private_;
  ros::Publisher trajectoryPub_;
  ros::Publisher referencePub_;
  std::vector<ros::Publisher> primitivePub_;
  ros::Subscriber motionselectorSub_;
  ros::Subscriber mavposeSub_;
  ros::Subscriber mavtwistSub_;
  ros::ServiceServer trajtriggerServ_;
  ros::Timer trajloop_timer_;
  ros::Timer refloop_timer_;
  ros::Time start_time_, curr_time_;

  nav_msgs::Path refTrajectory_;
  nav_msgs::Path primTrajectory_;
  geometry_msgs::TwistStamped refState_;

  int trajectory_type_;
  Eigen::Vector3d p_targ, v_targ;
  Eigen::Vector3d p_mav_, v_mav_;
  Eigen::Vector3d shape_origin_, shape_axis_;
  double shape_omega_ = 0;
  double theta_ = 0.0;
  double controlUpdate_dt_;
  double primitive_duration_;
  double trigger_time_;
  double init_pos_x_, init_pos_y_, init_pos_z_;
  double max_jerk_;
  int num_primitives_;
  int motion_selector_;

  std::vector<std::shared_ptr<trajectory>> motionPrimitives_;
  std::vector<Eigen::Vector3d> inputs_;


public:
  trajectoryPublisher(const ros::NodeHandle& nh, const ros::NodeHandle& nh_private);
  void updateReference();
  void pubrefTrajectory(int selector);
  void pubprimitiveTrajectory();
  void pubrefState();
  void initializePrimitives(int type);
  void updatePrimitives();
  void loopCallback(const ros::TimerEvent& event);
  void refCallback(const ros::TimerEvent& event);
  bool triggerCallback(std_srvs::SetBool::Request &req, std_srvs::SetBool::Response &res);
  void trajectoryCallback(const mav_planning_msgs::PolynomialTrajectory4D& segments_message);
  void motionselectorCallback(const std_msgs::Int32& selector);
  void mavposeCallback(const geometry_msgs::PoseStamped& msg);
  void mavtwistCallback(const geometry_msgs::TwistStamped& msg);

  };


#endif
