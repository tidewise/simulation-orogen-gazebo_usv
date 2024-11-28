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
        // Initialize speed limits as infinity
        JointLimits infinity;
        JointLimitRange range;
        range.Speed(-base::infinity<float>(), base::infinity<float>());
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

bool ThrusterLimitationTask::checkSpeedSaturation(commands::Joints const& cmd)
{
    return cmd.elements[0].speed >= m_limits.elements[0].max.speed ||
           cmd.elements[0].speed <= m_limits.elements[0].min.speed;
}

void ThrusterLimitationTask::validateSpeedCommand(commands::Joints const& cmd)
{
    if (cmd.elements.size() != 1) {
        return exception(INVALID_COMMAND_SIZE);
    }

    auto joint = cmd.elements.at(0);
    if (!joint.isSpeed()) {
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
    validateSpeedCommand(cmd_in);

    SaturationSignal saturation_signal;
    saturation_signal.value = checkSpeedSaturation(cmd_in);
    saturation_signal.time = cmd_in.time;
    _saturation_signal.write(saturation_signal);

    commands::Joints cmd_out;
    cmd_out.elements.resize(1);
    cmd_out.elements[0].speed = clamp(cmd_in.elements[0].speed,
        m_limits.elements[0].min.speed,
        m_limits.elements[0].max.speed);

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
