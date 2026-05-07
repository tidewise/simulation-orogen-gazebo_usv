/* Generated from orogen/lib/orogen/templates/tasks/Task.hpp */

#ifndef GAZEBO_USV_RUDDERACTUATORPOSITIONTOANALOGTASK_TASK_HPP
#define GAZEBO_USV_RUDDERACTUATORPOSITIONTOANALOGTASK_TASK_HPP

#include "gazebo_usv/RudderActuatorPositionToAnalogTaskBase.hpp"

namespace gazebo_usv {
    class RudderActuatorPositionToAnalogTask
        : public RudderActuatorPositionToAnalogTaskBase {
        friend class RudderActuatorPositionToAnalogTaskBase;

    protected:
        float m_input_min;
        float m_input_max;
        float m_position_min;
        float m_position_max;

        base::samples::Joints m_position;
        std::vector<raw_io::Analog> m_analog_input;

    public:
        RudderActuatorPositionToAnalogTask(
            std::string const& name = "gazebo_usv::RudderActuatorPositionToAnalogTask");
        ~RudderActuatorPositionToAnalogTask();
        bool configureHook();
        bool startHook();
        void updateHook();
        void errorHook();
        void stopHook();
        void cleanupHook();
    };
}

#endif
