#!/bin/bash

rostopic pub /controller/command \
trajectory_msgs/JointTrajectoryPoint \
"[0.0, 0.0, 0.0, 0.0, 0.0]" \
"[0.0, 0.0, 0.0, 0.0, 0.0]" \
"[0.0, 0.0, 0.0, 0.0, 0.0]" \
"[0.0, 0.0, 0.0, 0.0, 0.0]" \
"[0, 0]" "-1"
