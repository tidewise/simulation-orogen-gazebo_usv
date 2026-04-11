/* Generated from orogen/lib/orogen/templates/tasks/Task.hpp */

#ifndef GAZEBO_USV_WATERCURRENTSOURCETASK_TASK_HPP
#define GAZEBO_USV_WATERCURRENTSOURCETASK_TASK_HPP

#include "gazebo_usv/WaterCurrentSourceTaskBase.hpp"
#include <gz/transport.hh>

namespace gazebo_usv{

    /*! \class WaterCurrentSourceTask
     * \brief The task context provides and requires services. It uses an ExecutionEngine to perform its functions.
     * Essential interfaces are operations, data flow ports and properties. These interfaces have been defined using the oroGen specification.
     * In order to modify the interfaces you should (re)use oroGen and rely on the associated workflow.
     *
     * \details
     * The name of a TaskContext is primarily defined via:
     \verbatim
     deployment 'deployment_name'
         task('custom_task_name','gazebo_usv::WaterCurrentSourceTask')
     end
     \endverbatim
     *  It can be dynamically adapted when the deployment is called with a prefix argument.
     */
    class WaterCurrentSourceTask : public WaterCurrentSourceTaskBase
    {
	friend class WaterCurrentSourceTaskBase;
    protected:
        gz::sim::Entity m_entity = gz::sim::kNullEntity;
        gz::sim::EntityComponentManager* m_ecm = nullptr;
        std::shared_ptr<gz::transport::Node> m_node;
        gz::transport::Node::Publisher m_publisher;

        std::string resolveTopicName();

        void setGazebo(gz::sim::Entity const& entity,
            sdf::ElementConstPtr const& sdf,
            gz::sim::EntityComponentManager& ecm,
            gz::sim::EventManager& event_manager) override;

    public:
        /** TaskContext constructor for WaterCurrentSourceTask
         * \param name Name of the task. This name needs to be unique to make it identifiable via nameservices.
         * \param initial_state The initial TaskState of the TaskContext. Default is Stopped state.
         */
        WaterCurrentSourceTask(std::string const& name = "gazebo_usv::WaterCurrentSourceTask");

        /** Default deconstructor of WaterCurrentSourceTask
         */
        ~WaterCurrentSourceTask();

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

