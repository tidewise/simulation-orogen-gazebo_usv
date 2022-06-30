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
    {
        return false;
    }
    // Defining the max tether length
    mMaxTetherLength = _max_tether_length.get();
    mConfig = _tether_config.get();

    return true;
}

bool TetherSimulationTask::startHook()
{
    if (! TetherSimulationTaskBase::startHook())
    {
        return false;
    }

    mTether = TetherDragAndInertia(mConfig);

    return true;
}

void TetherSimulationTask::updateHook()
{

    base::Vector3d water_velocity;
    if (_water_speed.read(water_velocity) == RTT::NoData)
    {
        return;
    }

    base::samples::RigidBodyState rov_pose;
    if (_rov_attachment.read(rov_pose) == RTT::NoData)
    {
        return;
    }

    base::samples::RigidBodyState usv_pose;
    if (_usv_attachment.read(usv_pose) != RTT::NewData)
    {
        return;
    }

    // Calculating the current tether length, since it was defined as 'tether_length = 1.2 * rov2usv_distance'
    auto tether_length = 1.2 * (rov_pose.position - usv_pose.position).norm();

    // Setting the state of the tether
    TetherDragAndInertiaState state;
    state = mTether.computeTetherState(mConfig, usv_pose, rov_pose, water_velocity, tether_length);

    // Calculating the drag effect reflected on the tether
    base::Vector3d tether_drag_effect;
    tether_drag_effect = mTether.computeTetherDrag(mConfig, state);

    // Calculating the tether force in the rov and the usv (at this time, we are just considering the drag effect)
    base::Vector3d rov_total_force;
    base::Vector3d usv_total_force;
    rov_total_force = - tether_drag_effect/2;
    usv_total_force = tether_drag_effect/2;

    // Writing on the output ports
    _rov_force.write(rov_total_force);
    _usv_force.write(usv_total_force);

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
