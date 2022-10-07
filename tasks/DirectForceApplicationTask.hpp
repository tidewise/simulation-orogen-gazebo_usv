/* Generated from orogen/lib/orogen/templates/tasks/Task.hpp */

#ifndef GAZEBO_USV_DIRECTFORCEAPPLICATIONTASK_TASK_HPP
#define GAZEBO_USV_DIRECTFORCEAPPLICATIONTASK_TASK_HPP

#include "gazebo_usv/DirectForceApplicationTaskBase.hpp"

#include <gazebo/physics/PhysicsTypes.hh>
#include <gazebo/transport/Node.hh>
#include <gazebo/transport/Publisher.hh>

namespace gazebo_usv {

    /*! \class DirectForceApplicationTask
     * \brief The task context provides and requires services. It uses an ExecutionEngine to perform its functions.
     * Essential interfaces are operations, data flow ports and properties. These interfaces have been defined using the oroGen specification.
     * In order to modify the interfaces you should (re)use oroGen and rely on the associated workflow.
     * 
     * \details
     * The name of a TaskContext is primarily defined via:
     \verbatim
     deployment 'deployment_name'
         task('custom_task_name','gazebo_usv::DirectForceApplicationTask')
     end
     \endverbatim
     *  It can be dynamically adapted when the deployment is called with a prefix argument.
     */
    class DirectForceApplicationTask : public DirectForceApplicationTaskBase
    {
	friend class DirectForceApplicationTaskBase;
    protected:
        std::string mModelName;



    public:
        /** TaskContext constructor for DirectForceApplicationTask
         * \param name Name of the task. This name needs to be unique to make it identifiable via nameservices.
         * \param initial_state The initial TaskState of the TaskContext. Default is Stopped state.
         */
        DirectForceApplicationTask(std::string const& name = "gazebo_usv::DirectForceApplicationTask");

        /** Default deconstructor of DirectForceApplicationTask
         */
	    ~DirectForceApplicationTask();

        void setGazeboModel(std::string const& pluginName, gazebo::physics::ModelPtr model) override;
        void setGazeboPluginTaskName( std::string const& pluginTaskName );


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
        bool startHook();
        void updateHook();
        void errorHook();
        void stopHook();
        void cleanupHook();

    private:
        gazebo::transport::NodePtr mNode;
        gazebo::transport::PublisherPtr mForcePub;
    };
}

#endif

