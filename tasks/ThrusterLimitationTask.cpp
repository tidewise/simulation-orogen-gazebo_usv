/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "ThrusterLimitationTask.hpp"
#include <algorithm>

using namespace std;
using namespace base;
using namespace gazebo_usv;
using namespace control_base;

ThrusterLimitationTask::ThrusterLimitationTask(std::string const& name)
    : ThrusterLimitationTaskBase(name)
{
}

ThrusterLimitationTask::~ThrusterLimitationTask()
{
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See ThrusterLimitationTask.hpp for more detailed
// documentation about them.

bool ThrusterLimitationTask::configureHook()
{
    if (!ThrusterLimitationTaskBase::configureHook()) {
        return false;
    }
    m_limits = _limits.get();
    return true;
}

bool validateCommandType(JointState const& state, JointLimitRange const& limit)
{
    if(state.hasPosition() && (!limit.min.hasPosition() || !limit.max.hasPosition()))
    {
        return false;
    }
    if(state.hasSpeed() && (!limit.min.hasSpeed() || !limit.max.hasSpeed()))
    {
        return false;
    }
    if(state.hasRaw() && (!limit.min.hasRaw() || !limit.max.hasRaw()))
    {
        return false;
    }
    if(state.hasEffort() && (!limit.min.hasEffort() || !limit.max.hasEffort()))
    {
        return false;
    }
    if(state.hasAcceleration() && (!limit.min.hasAcceleration() || !limit.max.hasAcceleration()))
    {
        return false;
    }
    return true;
}

bool ThrusterLimitationTask::startHook()
{
    if (!ThrusterLimitationTaskBase::startHook()) {
        return false;
    }
    return true;
}

void ThrusterLimitationTask::validateCommand(commands::Joints const& cmd)
{
    if (cmd.elements.size() != m_limits.elements.size()) {
        return exception(INVALID_COMMAND_SIZE);
    }

    for (size_t i = 0; i < cmd.elements.size(); i++)
    {
        if(!validateCommandType(cmd.elements[i], m_limits.elements[i])){
            return exception(INVALID_COMMAND_PARAMETER);
        }
    }
}

void ThrusterLimitationTask::updateHook()
{
    ThrusterLimitationTaskBase::updateHook();

    commands::Joints cmd_in;
    if (_cmd_in.read(cmd_in) != RTT::NewData) {
        return;
    }
    validateCommand(cmd_in);

    SaturationSignal saturation_signal;
    auto [saturated, cmd_out] = m_limits.saturate(cmd_in);
    saturation_signal.value = saturated;
    saturation_signal.time = cmd_in.time;
    _saturation_signal.write(saturation_signal);
    cmd_out.time = Time::now();
    _cmd_out.write(cmd_out);
}

void ThrusterLimitationTask::errorHook()
{
    ThrusterLimitationTaskBase::errorHook();
}
void ThrusterLimitationTask::stopHook()
{
    ThrusterLimitationTaskBase::stopHook();
}
void ThrusterLimitationTask::cleanupHook()
{
    ThrusterLimitationTaskBase::cleanupHook();
}
