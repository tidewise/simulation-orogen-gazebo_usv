/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "BaseTask.hpp"

using namespace gazebo_usv;

BaseTask::BaseTask(std::string const& name)
    : BaseTaskBase(name)
{
}

BaseTask::BaseTask(std::string const& name, RTT::ExecutionEngine* engine)
    : BaseTaskBase(name, engine)
{
}

BaseTask::~BaseTask()
{
}

void BaseTask::setGazeboWorld(WorldPtr _world)
{
    world = _world;
}

std::string BaseTask::getWorldName() const
{
    return world->Name();
}

base::Time BaseTask::getSimTime() const
{
    gazebo::common::Time sim_time = world->SimTime();
    return base::Time::fromSeconds(sim_time.sec) +
        base::Time::fromMicroseconds(sim_time.nsec / 1000);
}

base::Time BaseTask::getCurrentTime(gazebo::msgs::Time sim_timestamp) const
{
    return getCurrentTime(base::Time::fromSeconds(sim_timestamp.sec())+
            base::Time::fromMicroseconds(sim_timestamp.nsec() / 1000));
}

base::Time BaseTask::getCurrentTime(base::Time sim_timestamp) const
{
    if (_use_sim_time)
        return sim_timestamp;
    else
        return base::Time::now() - (getSimTime() - sim_timestamp);
}

base::Time BaseTask::getCurrentTime() const
{
    if (_use_sim_time)
        return getSimTime();
    else
        return base::Time::now();
}


/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See BaseTask.hpp for more detailed
// documentation about them.

bool BaseTask::configureHook()
{
    if (! BaseTaskBase::configureHook())
        return false;
    return true;
}
bool BaseTask::startHook()
{
    if (! BaseTaskBase::startHook())
        return false;
    return true;
}
void BaseTask::updateHook()
{
    BaseTaskBase::updateHook();
}
void BaseTask::errorHook()
{
    BaseTaskBase::errorHook();
}
void BaseTask::stopHook()
{
    BaseTaskBase::stopHook();
}
void BaseTask::cleanupHook()
{
    BaseTaskBase::cleanupHook();
}
