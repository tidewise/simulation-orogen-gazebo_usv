/* Generated from orogen/lib/orogen/templates/tasks/Task.hpp */

#ifndef GAZEBO_USV_TETHERTRACTIONTASK_TASK_HPP
#define GAZEBO_USV_TETHERTRACTIONTASK_TASK_HPP

#include "gazebo_usv/TetherTractionTaskBase.hpp"

#include <gazebo/transport/Node.hh>
#include <gazebo/transport/Publisher.hh>


namespace gazebo_usv {

    /*! \class TetherTractionTask
     * \brief The task context provides and requires services. It uses an ExecutionEngine to perform its functions.
     * Essential interfaces are operations, data flow ports and properties. These interfaces have been defined using the oroGen specification.
     * In order to modify the interfaces you should (re)use oroGen and rely on the associated workflow.
     * 
     * \details
     * The name of a TaskContext is primarily defined via:
     \verbatim
     deployment 'deployment_name'
         task('custom_task_name','gazebo_usv::TetherTractionTask')
     end
     \endverbatim
     *  It can be dynamically adapted when the deployment is called with a prefix argument.
     */
    class TetherTractionTask : public TetherTractionTaskBase
    {
	friend class TetherTractionTaskBase;
    protected:



    public:
        /** TaskContext constructor for TetherTractionTask
         * \param name Name of the task. This name needs to be unique to make it identifiable via nameservices.
         * \param initial_state The initial TaskState of the TaskContext. Default is Stopped state.
         */
        TetherTractionTask(std::string const& name = "gazebo_usv::TetherTractionTask");

        /** Default deconstructor of TetherTractionTask
         */
	    ~TetherTractionTask();

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
        gazebo::transport::NodePtr node;
        gazebo::transport::PublisherPtr directForceVesselPub;
        gazebo::transport::PublisherPtr directForceRovPub;

        base::samples::RigidBodyState vesselAttachmentState;
        base::samples::RigidBodyState rovAttachmentState;

        bool isVesselInitialized = false;
        bool isRovInitialized = false;
    };
}

#endif

