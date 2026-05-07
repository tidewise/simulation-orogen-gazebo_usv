/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "RudderActuatorPositionToAnalogTask.hpp"

using namespace gazebo_usv;

RudderActuatorPositionToAnalogTask::RudderActuatorPositionToAnalogTask(
    std::string const& name)
    : RudderActuatorPositionToAnalogTaskBase(name)
{
}

RudderActuatorPositionToAnalogTask::~RudderActuatorPositionToAnalogTask()
{
}

bool RudderActuatorPositionToAnalogTask::configureHook()
{
    if (!RudderActuatorPositionToAnalogTaskBase::configureHook())
        return false;

    m_input_min = _input_min.get();
    m_input_max = _input_max.get();
    m_position_min = _position_min.get();
    m_position_max = _position_max.get();
    m_analog_input.resize(1);

    return true;
}
bool RudderActuatorPositionToAnalogTask::startHook()
{
    if (!RudderActuatorPositionToAnalogTaskBase::startHook())
        return false;
    return true;
}
void RudderActuatorPositionToAnalogTask::updateHook()
{
    RudderActuatorPositionToAnalogTaskBase::updateHook();
    while (_position_samples.read(m_position) == RTT::NewData) {
        if (m_position.elements.empty() || !m_position.elements[0].hasPosition()) {
            return exception(INVALID_POSITION_SAMPLE);
        }

        float position = m_position.elements[0].position;

        float analog_value = m_input_min + (position - m_position_min) *
                                               (m_input_max - m_input_min) /
                                               (m_position_max - m_position_min);

        m_analog_input[0].time = m_position.time;
        m_analog_input[0].data = analog_value;

        _analog_input.write(m_analog_input);
    }
}
void RudderActuatorPositionToAnalogTask::errorHook()
{
    RudderActuatorPositionToAnalogTaskBase::errorHook();
}
void RudderActuatorPositionToAnalogTask::stopHook()
{
    RudderActuatorPositionToAnalogTaskBase::stopHook();
}
void RudderActuatorPositionToAnalogTask::cleanupHook()
{
    RudderActuatorPositionToAnalogTaskBase::cleanupHook();
}
