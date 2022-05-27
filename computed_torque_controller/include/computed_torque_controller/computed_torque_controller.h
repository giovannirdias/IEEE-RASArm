/******************************************************************************
	               ROS computed_torque_controller Package
                           Computed Torque  Controller
          Copyright (C) 2013..2017 Walter Fetter Lages <w.fetter@ieee.org>

        This program is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful, but
        WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
        General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program.  If not, see
        <http://www.gnu.org/licenses/>.
        
*******************************************************************************/

#ifndef COMPUTED_TORQUE_CONTROLLER_COMPUTED_TORQUE_CONTROLLER
#define COMPUTED_TORQUE_CONTROLLER_COMPUTED_TORQUE_CONTROLLER

#include <cstddef>
#include <vector>
#include <string>

#include <ros/node_handle.h>
#include <hardware_interface/joint_command_interface.h>
#include <controller_interface/controller.h>
#include <trajectory_msgs/JointTrajectoryPoint.h>

#include <Eigen/Core>

#include <kdl/frames.hpp>
#include <kdl_parser/kdl_parser.hpp>
#include <kdl/chainidsolver_recursive_newton_euler.hpp>

namespace effort_controllers
{
	class ComputedTorqueController: public controller_interface::
		Controller<hardware_interface::EffortJointInterface>
	{
		ros::NodeHandle node_;

		hardware_interface::EffortJointInterface *hw_;
		std::vector<hardware_interface::JointHandle> joints_;
		int nJoints_;

		ros::Subscriber sub_command_;
		
		KDL::Tree tree_;
		KDL::Chain chain_;	
		KDL::ChainIdSolver_RNE *idsolver_;
				
		KDL::JntArray q_;
		KDL::JntArray dq_;
		KDL::JntArray v_;
			
		KDL::JntArray qr_;
		KDL::JntArray dqr_;
		KDL::JntArray ddqr_;
			
		KDL::JntArray torque_;
		
		KDL::Wrenches fext_;
			
		Eigen::MatrixXd Kp_;
		Eigen::MatrixXd Kd_;
		
		void commandCB(const trajectory_msgs::JointTrajectoryPoint::
		ConstPtr &referencePoint);
		
		public:
		ComputedTorqueController(void);
		~ComputedTorqueController(void);
		
		bool init(hardware_interface::EffortJointInterface *hw,
			ros::NodeHandle &n);
		void starting(const ros::Time& time);
		void update(const ros::Time& time,const ros::Duration& duration);
	};
}
#endif
