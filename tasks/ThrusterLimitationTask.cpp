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
    if (m_limits.elements.empty()) {
        // Initialize effort limits as infinity
        JointLimits infinity;
        JointLimitRange range;
        range.Effort(-base::infinity<float>(), base::infinity<float>());
        infinity.elements.push_back(range);
        m_limits = infinity;
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

bool ThrusterLimitationTask::checkEffortSaturation(commands::Joints const& cmd)
{
    return cmd.elements[0].effort >= m_limits.elements[0].max.effort ||
           cmd.elements[0].effort <= m_limits.elements[0].min.effort ||
           cmd.elements[1].effort >= m_limits.elements[0].max.effort ||
           cmd.elements[1].effort <= m_limits.elements[0].min.effort;
}

void ThrusterLimitationTask::validateEffortCommand(commands::Joints const& cmd)
{
    if (cmd.elements.size() != 2) {
        return exception(INVALID_COMMAND_SIZE);
    }
    std::vector<base::JointState> joints;
    joints.resize(2);
    joints = cmd.elements;
    if (!joints[0].isEffort() || !joints[1].isEffort()) {
        return exception(INVALID_COMMAND_PARAMETER);
    }
}

commands::Joints ThrusterLimitationTask::saturateCommand(commands::Joints const& cmd)
{
    commands::Joints cmd_out;
    cmd_out.names.resize(2);
    cmd_out.elements.resize(2);
    for (size_t i = 0; i < 2; i++) {
        cmd_out.names[i] = cmd.names[i];
        cmd_out.elements[i] = cmd.elements[i];
        cmd_out.elements[i].effort = clamp(cmd.elements[i].effort,
            m_limits.elements[i].min.effort,
            m_limits.elements[i].max.effort);
    }
    return cmd_out;
}

void ThrusterLimitationTask::updateHook()
{
    ThrusterLimitationTaskBase::updateHook();

    commands::Joints cmd_in;
    if (_cmd_in.read(cmd_in) != RTT::NewData) {
        return;
    }
    validateEffortCommand(cmd_in);

    SaturationSignal saturation_signal;
    saturation_signal.value = checkEffortSaturation(cmd_in);
    saturation_signal.time = cmd_in.time;
    _saturation_signal.write(saturation_signal);

    commands::Joints cmd_out = saturateCommand(cmd_in);
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
