## Dependencies
ROS Desktop Full  
PX4 Firmware  
MAVROS  
Make sure mavros and mavlink folders are in src  

## Compiling & Running
Make sure all dependencies are installed  
Create a workspace  
Fork project so files are in your workspace  

cd your-workspace  
catkin build offb  
source ~/catkin_ws/px4/Tools/setup_gazebo.bash ~/catkin_ws/px4 ~/catkin_ws/px4/build/px4_sitl_default  
source devel/setup.bash  
./launch-offb.sh  

##  To Do  
~~Smooth out flight (i.e. change theta as x y and z changes)~~  
Add obstacle avoidance (https://github.com/PX4/avoidance)  
~~Modify gazebo world file to be similar to IARC course (i.e. add the two goal posts we have to fly around and the swaying mast)~~  
~~Create folder and file for gazebo world and upload to github (right now it runs using a world file that is part of the PX4 Firmware)~~  
~~After takeoff provide functions for the VTOL to fly around the two goal posts for a set length~~  
~~Abstract away low level flight details~~  
Start in air object manipulation
