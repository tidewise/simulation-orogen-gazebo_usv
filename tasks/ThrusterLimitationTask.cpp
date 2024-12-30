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
        infinity.elements.push_back(range);
        infinity.names.push_back("thruster::left");
        infinity.names.push_back("thruster::right");
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

void ThrusterLimitationTask::updateHook()
{
    ThrusterLimitationTaskBase::updateHook();

    commands::Joints cmd_in;
    if (_cmd_in.read(cmd_in) != RTT::NewData) {
        return;
    }
    validateEffortCommand(cmd_in);

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
