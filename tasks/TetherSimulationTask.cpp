/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "TetherSimulationTask.hpp"

using namespace gazebo_usv;

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
    return true;
}
bool TetherSimulationTask::startHook()
{
    if (! TetherSimulationTaskBase::startHook())
        return false;
    return true;
}
void TetherSimulationTask::updateHook()
{
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
