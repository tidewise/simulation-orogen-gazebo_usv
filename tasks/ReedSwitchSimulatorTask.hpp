#ifndef GAZEBO_USV_ReedSwitchSimulatorTask_TASK_HPP
#define GAZEBO_USV_ReedSwitchSimulatorTask_TASK_HPP

#include "base/Time.hpp"
#include "base/commands/Joints.hpp"
#include "gazebo_usv/ReedSwitchSimulatorTaskBase.hpp"
#include <optional>

namespace gazebo_usv {

    /*! \class ReedSwitchSimulatorTask
     * \brief The task context provides and requires services. It uses an ExecutionEngine
     to perform its functions.
     * Essential interfaces are operations, data flow ports and properties. These
     interfaces have been defined using the oroGen specification.
     * In order to modify the interfaces you should (re)use oroGen and rely on the
     associated workflow.
     *
     * \details
     * The name of a TaskContext is primarily defined via:
     \verbatim
     deployment 'deployment_name'
         task('custom_task_name','gazebo_usv::ReedSwitchSimulatorTask')
     end
     \endverbatim
     *  It can be dynamically adapted when the deployment is called with a prefix
     argument.
     */
    struct CommandDirection {
        int direction = 0;
        base::Time time;
    };

    class ReedSwitchSimulatorTask : public ReedSwitchSimulatorTaskBase {
        friend class ReedSwitchSimulatorTaskBase;

    private:
        std::optional<CommandDirection> m_last_command_direction;
        /**
         * The timer is like a pointer of the clock.
         * This indicates the region where the sensor is reading.
         */
        int64_t m_timer = 0;
        /**
         * The revolution time is the total time of one clock cycle
         */
        base::Time m_revolution_time;
        /**
         * The trigger time is the clock region where the sensor is at trigger level.
         * It starts from zero to trigger_time. All other regions in the clock is non
         * trigger level.
         */
        base::Time m_trigger_time;

        void updateTimeInCycle(int64_t delta);
        void updateMotorCommandDirection(linux_gpios::GPIOState const& extend,
            linux_gpios::GPIOState const& retract, base::Time const& now);
        bool sensorActive();
        bool validateGpio(linux_gpios::GPIOState const& gpio);

    public:
        /** TaskContext constructor for ReedSwitchSimulatorTask
         * \param name Name of the task. This name needs to be unique to make it
         * identifiable via nameservices. \param initial_state The initial TaskState of
         * the TaskContext. Default is Stopped state.
         */
        ReedSwitchSimulatorTask(
            std::string const& name = "gazebo_usv::ReedSwitchSimulatorTask");

        /** Default deconstructor of ReedSwitchSimulatorTask
         */
        ~ReedSwitchSimulatorTask();

        /** This hook is called by Orocos when the state machine transitions
         * from PreOperational to Stopped. If it returns false, then the
         * component will stay in PreOperational. Otherwise, it goes into
         * Stopped.
         *
         * It is meaningful only if the #needs_configuration has been specified
         * in the task context definition with (for example):
         \verbatim
         task_context "TaskName" do
           needs_configuration
           ...
         end
         \endverbatim
         */
        bool configureHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Stopped to Running. If it returns false, then the component will
         * stay in Stopped. Otherwise, it goes into Running and updateHook()
         * will be called.
         */
        bool startHook();

        /** This hook is called by Orocos when the component is in the Running
         * state, at each activity step. Here, the activity gives the "ticks"
         * when the hook should be called.
         *
         * The error(), exception() and fatal() calls, when called in this hook,
         * allow to get into the associated RunTimeError, Exception and
         * FatalError states.
         *
         * In the first case, updateHook() is still called, and recover() allows
         * you to go back into the Running state.  In the second case, the
         * errorHook() will be called instead of updateHook(). In Exception, the
         * component is stopped and recover() needs to be called before starting
         * it again. Finally, FatalError cannot be recovered.
         */
        void updateHook();

        /** This hook is called by Orocos when the component is in the
         * RunTimeError state, at each activity step. See the discussion in
         * updateHook() about triggering options.
         *
         * Call recover() to go back in the Runtime state.
         */
        void errorHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Running to Stopped after stop() has been called.
         */
        void stopHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Stopped to PreOperational, requiring the call to configureHook()
         * before calling start() again.
         */
        void cleanupHook();
    };
}

#endif
