/* Generated from orogen/lib/orogen/templates/tasks/Task.hpp */

#ifndef GAZEBO_USV_THRUSTERTASK_TASK_HPP
#define GAZEBO_USV_THRUSTERTASK_TASK_HPP

#include "gazebo_usv/ThrusterTaskBase.hpp"

#include <gz/sim/Entity.hh>
#include <gz/sim/EntityComponentManager.hh>
#include <gz/transport.hh>
#include <gz/gazebo_usv/thrusters.pb.h>

namespace gazebo_usv {
    class ThrusterTask : public ThrusterTaskBase
    {
	friend class ThrusterTaskBase;
    public:
        typedef gz::gazebo_usv::Thrusters Thrusters;

        ThrusterTask(std::string const& name = "gazebo_usv::ThrusterTask");
        ThrusterTask(std::string const& name, RTT::ExecutionEngine* engine);
        ~ThrusterTask();

        bool configureHook();
        bool startHook();
        void updateHook();
        void errorHook();
        void stopHook();
        void cleanupHook();

        virtual void setGazebo(gz::sim::Entity const& entity,
            sdf::ElementConstPtr const& sdf,
            gz::sim::EntityComponentManager& ecm,
            gz::sim::EventManager& event_manager) override;

    private:
        gz::sim::Entity m_entity = gz::sim::kNullEntity;
        gz::sim::EntityComponentManager* m_ecm = nullptr;
        std::shared_ptr<gz::transport::Node> m_node;
        gz::transport::Node::Publisher m_publisher;
    };
}

#endif

