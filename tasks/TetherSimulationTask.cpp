/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "TetherSimulationTask.hpp"

using namespace std;
using namespace base;
using namespace gazebo_usv;
using namespace uuv_tether_control;

TetherSimulationTask::TetherSimulationTask(std::string const& name)
    : TetherSimulationTaskBase(name)
{
}

TetherSimulationTask::~TetherSimulationTask()
{
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See TetherSimulationTask.hpp for more detailed
// documentation about them.

bool TetherSimulationTask::configureHook()
{
    if (! TetherSimulationTaskBase::configureHook())
        return false;
    mMaxTetherLength = _max_tether_length.get();
    mConfig = _tether_config.get();

    return true;
}

bool TetherSimulationTask::startHook()
{
    if (! TetherSimulationTaskBase::startHook())
        return false;
    mTether = TetherDragAndInertia(mConfig);

    return true;
}

void TetherSimulationTask::updateHook()
{

    base::Vector3d water_velocity;
    if (_water_speed.read(water_velocity) == RTT::NoData)
        return;

    base::samples::RigidBodyState rov_pose;
    if (_rov2world_pose.read(rov_pose) == RTT::NoData)
	return;

    base::samples::RigidBodyState usv_pose;
    if (_usv2world_pose.read(usv_pose) != RTT::NewData)
 	return;

    auto tether_length = 1.2 * (rov_pose.position - usv_pose.position).norm();

    base::Vector3d tether_force;
    base::Acceleration acceleration;

    acceleration.setZero();
    tether_force = mTether.evaluate(rov_pose.time, tether_length, usv_pose, rov_pose, acceleration, water_velocity);

//Output Port
     base::Vector3d rov_force;
     base::Vector3d usv_force;

     rov_force = - tether_force;
     usv_force = tether_force;
    _rov_force.write(rov_force);
    _usv_force.write(usv_force);


    TetherSimulationTaskBase::updateHook();
}

void TetherSimulationTask::errorHook()
{
    TetherSimulationTaskBase::errorHook();
}
void TetherSimulationTask::stopHook()
{
    TetherSimulationTaskBase::stopHook();
}
void TetherSimulationTask::cleanupHook()
{
    TetherSimulationTaskBase::cleanupHook();
}
