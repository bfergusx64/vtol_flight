/**


File to launch and control the VTOL


 */




//include libraries
#include "ros/ros.h"
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <mavros_msgs/CommandTOL.h>
#include <iostream>
#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Odometry.h>
#include "std_msgs/Float64.h"
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <mavros_msgs/PositionTarget.h>
#include <unistd.h>
#include <geometry_msgs/Pose2D.h>
#include <mavros_msgs/CommandTOL.h>
#include <time.h>
#include <cmath>
#include <math.h>
#include <ros/duration.h>



using namespace std;


//Set global variables
mavros_msgs::State current_state;
//nav_msgs::Odometry current_pose;
geometry_msgs::PoseStamped current_pose;
geometry_msgs::PoseStamped pose;
std_msgs::Float64 current_heading;



//function that sets the current state
void state_cb(const mavros_msgs::State::ConstPtr& msg){
    current_state = *msg;
}

//get current position of drone
void pose_cb(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
  current_pose = *msg;
  ROS_INFO("x: %f y: %f z: %f", current_pose.pose.position.x, current_pose.pose.position.y, current_pose.pose.position.z);
  // ROS_INFO("y: %f", current_pose.pose.position.y);
  // ROS_INFO("z: %f", current_pose.pose.position.z);
}


//get compass heading
void heading_cb(const std_msgs::Float64::ConstPtr& msg)
{
  current_heading = *msg;
  //ROS_INFO("current heading: %f", current_heading.data);
}


//function that controls yaw, pitch, and roll to keep the vtol pointing the correct direction
void setHeading(float heading)
{
  heading = -heading + 90;
  float yaw = heading*(M_PI/180);
  float pitch = 0;
  float roll = 0;

  float cy = cos(yaw * 0.5);
  float sy = sin(yaw * 0.5);
  float cr = cos(roll * 0.5);
  float sr = sin(roll * 0.5);
  float cp = cos(pitch * 0.5);
  float sp = sin(pitch * 0.5);

  float qw = cy * cr * cp + sy * sr * sp;
  float qx = cy * sr * cp - sy * cr * sp;
  float qy = cy * cr * sp + sy * sr * cp;
  float qz = sy * cr * cp - cy * sr * sp;

  pose.pose.orientation.w = qw;
  pose.pose.orientation.x = qx;
  pose.pose.orientation.y = qy;
  pose.pose.orientation.z = qz;

}


//function that abstracts away setting flight points x, y, and z
void setDestination(float x, float y, float z)
{
  float deg2rad = (M_PI/180);
  float X = x*cos(deg2rad) - y*sin(deg2rad);
  float Y = x*sin(deg2rad) + y*cos(deg2rad);
  float Z = z;
  pose.pose.position.x = X;
  pose.pose.position.y = Y;
  pose.pose.position.z = Z;
  ROS_INFO("Destination set to x: %f y: %f z %f", X, Y, Z);
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "offb_node");
    ros::NodeHandle nh;

    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State> ("mavros/state", 10, state_cb);
    ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped> ("mavros/setpoint_position/local", 10);
    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool> ("mavros/cmd/arming");
    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode> ("mavros/set_mode");
    ros::Publisher set_vel_pub = nh.advertise<mavros_msgs::PositionTarget>("mavros/setpoint_raw/local", 10);
    ros::Subscriber currentPos = nh.subscribe<geometry_msgs::PoseStamped>("/mavros/global_position/pose", 10, pose_cb);
    ros::Subscriber currentHeading = nh.subscribe<std_msgs::Float64>("/mavros/global_position/compass_hdg", 10, heading_cb);

    //the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(20.0);

    // wait for FCU connection
    while(ros::ok() && !current_state.connected){
        ros::spinOnce();
        rate.sleep();
    }

    pose.pose.position.x = 0;
    pose.pose.position.y = 0;
    pose.pose.position.z = 2;

    //send a few setpoints before starting
    for(int i = 100; ros::ok() && i > 0; --i){
        local_pos_pub.publish(pose);
        ros::spinOnce();
        rate.sleep();
    }

    mavros_msgs::SetMode offb_set_mode;
    offb_set_mode.request.custom_mode = "OFFBOARD";

    mavros_msgs::CommandBool arm_cmd;
    arm_cmd.request.value = true;





    ros::Time last_request = ros::Time::now();

    while(ros::ok()){
        if( current_state.mode != "OFFBOARD" &&
            (ros::Time::now() - last_request > ros::Duration(5.0))){
            if( set_mode_client.call(offb_set_mode) &&
                offb_set_mode.response.mode_sent){
                ROS_INFO("Offboard enabled");
            }
            last_request = ros::Time::now();
        } else {
            if( !current_state.armed &&
                (ros::Time::now() - last_request > ros::Duration(5.0))){
                if( arming_client.call(arm_cmd) &&
                    arm_cmd.response.success){
                    ROS_INFO("Vehicle armed");
                }
                last_request = ros::Time::now();
            }
        }


        local_pos_pub.publish(pose);



        setDestination(10,10,10);

        local_pos_pub.publish(pose);







        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}
