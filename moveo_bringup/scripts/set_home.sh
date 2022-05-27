#!/bin/bash

rosservice call /gazebo/set_model_configuration moveo joint \
"['joint_1','joint_2','joint_3','joint_4','joint_5']" \
"[0.0, 0.5, 0.0, 0.5, 0.0]"

rostopic pub /controller/command \
trajectory_msgs/JointTrajectoryPoint \
"[0.0, 0.5, 0.5, 0.0, 0.5]" \
"[0.0, 0.0, 0.0, 0.0, 0.0]" \
"[0.0, 0.0, 0.0, 0.0, 0.0]" \
"[0.0, 0.0, 0.0, 0.0, 0.0]" \
"[0, 0]" "-1"
