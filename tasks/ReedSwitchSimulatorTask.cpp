/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "ReedSwitchSimulatorTask.hpp"
#include <base-logging/Logging.hpp>

using namespace std;
using namespace base;
using namespace gazebo_usv;
using namespace linux_gpios;

ReedSwitchSimulatorTask::ReedSwitchSimulatorTask(std::string const& name)
    : ReedSwitchSimulatorTaskBase(name)
{
}

ReedSwitchSimulatorTask::~ReedSwitchSimulatorTask()
{
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See ReedSwitchSimulatorTask.hpp for more detailed
// documentation about them.

bool ReedSwitchSimulatorTask::validateGpio(linux_gpios::GPIOState const& gpio)
{
    if (gpio.states.size() != 1) {
        LOG_FATAL_S << "received a gpio state of size " << gpio.states.size();
        exception(INVALID_GPIO_SIZE);
        return false;
    }
    return true;
}

bool ReedSwitchSimulatorTask::configureHook()
{
    if (!ReedSwitchSimulatorTaskBase::configureHook()) {
        return false;
    }
    m_revolution_time = _revolution_time.get();
    m_trigger_time = _trigger_time.get();
    return true;
}

bool ReedSwitchSimulatorTask::startHook()
{
    if (!ReedSwitchSimulatorTaskBase::startHook()) {
        return false;
    }
    m_timer = 0;
    return true;
}

void ReedSwitchSimulatorTask::updateTimeInCycle(int64_t delta)
{
    int64_t updated_timer = m_timer + delta;
    if (updated_timer < 0) {
        int16_t number_of_revolutions =
            static_cast<int16_t>(abs(delta / m_revolution_time.microseconds)) + 1;
        updated_timer += number_of_revolutions * m_revolution_time.microseconds;
    }
    // Normalize the updated_timer in a time inside one revolution.
    m_timer = updated_timer % m_revolution_time.microseconds;
}

CommandDirection motorCommand(GPIOState const& extend,
    GPIOState const& retract,
    Time const& now)
{
    bool extend_state = extend.states[0].data;
    bool retract_state = retract.states[0].data;
    if (!extend_state && !retract_state) {
        return CommandDirection{0, now};
    }
    else if (extend_state && !retract_state) {
        return CommandDirection{1, now};
    }
    else if (!extend_state && retract_state) {
        return CommandDirection{-1, now};
    }
    else {
        throw std::runtime_error("Invalid command direction - Both gpio states should "
                                 "not be true at the same time.");
    }
}

void ReedSwitchSimulatorTask::updateMotorCommandDirection(GPIOState const& extend,
    GPIOState const& retract,
    Time const& now)
{
    CommandDirection command_direction = motorCommand(extend, retract, now);
    if (m_last_command_direction.has_value()) {
        Time delta_time = now - m_last_command_direction->time;
        updateTimeInCycle(delta_time.microseconds * m_last_command_direction->direction);
    }
    m_last_command_direction = command_direction;
}

GPIOState gpio(bool flag, Time const& now)
{
    GPIOState gpio;
    gpio.time = now;
    gpio.states.resize(1);
    gpio.states[0].time = now;
    gpio.states[0].data = flag;
    return gpio;
}

bool ReedSwitchSimulatorTask::sensorActive()
{
    return m_timer <= m_trigger_time.microseconds;
}

void ReedSwitchSimulatorTask::updateHook()
{
    ReedSwitchSimulatorTaskBase::updateHook();

    GPIOState gpio_retract;
    if (_gpio_retract.read(gpio_retract) == RTT::NoData) {
        return;
    }
    GPIOState gpio_extend;
    if (_gpio_extend.read(gpio_extend) == RTT::NoData) {
        return;
    }
    if (!validateGpio(gpio_retract) || !validateGpio(gpio_extend)) {
        return;
    }

    Time now = Time::now();
    updateMotorCommandDirection(gpio_extend, gpio_retract, now);

    GPIOState gpio_reed_switch = gpio(sensorActive(), now);
    _gpio_reed_switch.write(gpio_reed_switch);
}

void ReedSwitchSimulatorTask::errorHook()
{
    ReedSwitchSimulatorTaskBase::errorHook();
}
void ReedSwitchSimulatorTask::stopHook()
{
    ReedSwitchSimulatorTaskBase::stopHook();
    m_last_command_direction = {};
    m_timer = 0;
}
void ReedSwitchSimulatorTask::cleanupHook()
{
    ReedSwitchSimulatorTaskBase::cleanupHook();
}
