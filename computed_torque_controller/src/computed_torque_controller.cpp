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

#include <sys/mman.h>

#include <computed_torque_controller/computed_torque_controller.h>
#include <pluginlib/class_list_macros.h>

namespace effort_controllers
{	
	ComputedTorqueController::ComputedTorqueController(void):
		q_(0),dq_(0),v_(0),qr_(0),dqr_(0),ddqr_(0),torque_(0),fext_(0)
	{
	}
	
	ComputedTorqueController::~ComputedTorqueController(void)
	{
		sub_command_.shutdown();
	}
		
	bool ComputedTorqueController::
	        init(hardware_interface::EffortJointInterface *hw,ros::NodeHandle &n)
	{
		node_=n;
		hw_=hw;
		
		std::vector<std::string> joint_names;
		if(!node_.getParam("joints",joint_names))
		{
			ROS_ERROR("No 'joints' in controller. (namespace: %s)",
			        node_.getNamespace().c_str());
			return false;
		}
		
		nJoints_=joint_names.size();		
		
		for(int i=0; i < nJoints_;i++)
		{
			try
			{
				joints_.push_back(hw->getHandle(joint_names[i]));
			}
			catch(const hardware_interface::HardwareInterfaceException& e)
			{
				ROS_ERROR_STREAM("Exception thrown: " << e.what());
				return false;
			}
		}
		sub_command_=node_.subscribe("command",1,
		        &ComputedTorqueController::commandCB, this);
		
		std::string robot_desc_string;
		if(!node_.getParam("/robot_description",robot_desc_string))
		{
			ROS_ERROR("Could not find '/robot_description'.");
			return false;
		}
		
		if (!kdl_parser::treeFromString(robot_desc_string,tree_))
		{
			ROS_ERROR("Failed to construct KDL tree.");
			return false;
		}
		
		std::string chainRoot;
		if(!node_.getParam("chain/root",chainRoot))
		{
			ROS_ERROR("Could not find 'chain_root' parameter.");
			return false;
		}
		
		std::string chainTip;
		if(!node_.getParam("chain/tip",chainTip))
		{
			ROS_ERROR("Could not find 'chain/tip' parameter.");
			return false;
		}
		
		if (!tree_.getChain(chainRoot,chainTip,chain_)) 
		{
			ROS_ERROR("Failed to get chain from KDL tree.");
			return false;
		}
		
		KDL::Vector g;
		node_.param("gravity/x",g[0],0.0);
		node_.param("gravity/y",g[1],0.0);
		node_.param("gravity/z",g[2],-9.8);

		if((idsolver_=new KDL::ChainIdSolver_RNE(chain_,g)) == NULL)
		{
			ROS_ERROR("Failed to create ChainIDSolver_RNE.");
			return false;
		}

		q_.resize(nJoints_);
		dq_.resize(nJoints_);
		v_.resize(nJoints_);
		qr_.resize(nJoints_);
		dqr_.resize(nJoints_);
		ddqr_.resize(nJoints_);
		torque_.resize(nJoints_);

		fext_.resize(chain_.getNrOfSegments());
		
		Kp_.resize(nJoints_,nJoints_);
		Kd_.resize(nJoints_,nJoints_);
		
		std::vector<double> KpVec;
		if(!node_.getParam("Kp",KpVec))
		{
			ROS_ERROR("No 'Kp' in controller %s.",node_.getNamespace().c_str());
			return false;
		}
		Kp_=Eigen::Map<Eigen::MatrixXd>(KpVec.data(),nJoints_,nJoints_).transpose();
		
		std::vector<double> KdVec;
		if(!node_.getParam("Kd",KdVec))
		{
			ROS_ERROR("No 'Kd' in controller %s.",node_.getNamespace().c_str());
			return false;
		}
		Kd_=Eigen::Map<Eigen::MatrixXd>(KdVec.data(),nJoints_,nJoints_).transpose();
		
		return true;
	}
	
	void ComputedTorqueController::starting(const ros::Time& time)
	{
		for(unsigned int i=0;i < nJoints_;i++)
		{
			q_(i)=joints_[i].getPosition();
			dq_(i)=joints_[i].getVelocity();
		}
		qr_=q_;
		dqr_=dq_;
		SetToZero(ddqr_);
		
		struct sched_param param;
		if(!node_.getParam("priority",param.sched_priority))
		{
			ROS_WARN("No 'priority' configured for controller %s. Using highest possible priority.",node_.getNamespace().c_str());
			param.sched_priority=sched_get_priority_max(SCHED_FIFO);	
		}
		if(sched_setscheduler(0,SCHED_FIFO,&param) == -1)
		{
			ROS_WARN("Failed to set real-time scheduler.");
			return;
		}
		if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1)
			ROS_WARN("Failed to lock memory.");
	}
	
	void ComputedTorqueController::update(const ros::Time& time,
	        const ros::Duration& duration)
	{
		for(unsigned int i=0;i < nJoints_;i++)
		{
			q_(i)=joints_[i].getPosition();
			dq_(i)=joints_[i].getVelocity();
		}
		for(unsigned int i=0;i < fext_.size();i++) fext_[i].Zero();
		
		v_.data=ddqr_.data+Kp_*(qr_.data-q_.data)+Kd_*(dqr_.data-dq_.data);
		if(idsolver_->CartToJnt(q_,dq_,v_,fext_,torque_) < 0)
		        ROS_ERROR("KDL inverse dynamics solver failed.");
		
		for(unsigned int i=0;i < nJoints_;i++)
		        joints_[i].setCommand(torque_(i));
	}
	
	void ComputedTorqueController::commandCB(const trajectory_msgs::
	        JointTrajectoryPoint::ConstPtr &referencePoint)
	{
		for(unsigned int i=0;i < nJoints_;i++)
		{
			qr_(i)=referencePoint->positions[i];
			dqr_(i)=referencePoint->velocities[i];
			ddqr_(i)=referencePoint->accelerations[i];
		}
	}
}

PLUGINLIB_EXPORT_CLASS(effort_controllers::ComputedTorqueController,
        controller_interface::ControllerBase)
