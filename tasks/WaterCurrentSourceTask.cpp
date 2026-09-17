/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "WaterCurrentSourceTask.hpp"
#include "gazebo_usv/WaterCurrentSourceTaskBase.hpp"
#include <gz/msgs/vector3d.pb.h>
#include <rock_gazebo/Helpers.hpp>

using namespace gazebo_usv;

WaterCurrentSourceTask::WaterCurrentSourceTask(std::string const& name)
    : WaterCurrentSourceTaskBase(name)
{
}

WaterCurrentSourceTask::~WaterCurrentSourceTask()
{
}

void WaterCurrentSourceTask::setGazebo(gz::sim::Entity const& entity,
    sdf::ElementConstPtr const& sdf,
    gz::sim::EntityComponentManager& ecm,
    gz::sim::EventManager& event_manager) {

    BaseTask::setGazebo(entity, sdf, ecm, event_manager);

    m_entity = entity;
    m_ecm = &ecm;
}

bool WaterCurrentSourceTask::configureHook()
{
    if (! WaterCurrentSourceTaskBase::configureHook())
        return false;

    std::string const topic_namespace = _topic_namespace.get();
    m_topic_name = topic_namespace.empty() ? "/ocean_current" :
        "/model/" + topic_namespace + "/ocean_current";

    m_node = std::make_shared<gz::transport::Node>();
    gzmsg << "WaterCurrentSourceTask: advertising to gazebo topic " + m_topic_name
          << std::endl;

    m_publisher = m_node->Advertise<gz::msgs::Vector3d>(m_topic_name);
    if (!m_publisher) {
        exception(NO_TOPIC_CONNECTION);
    }
    return true;
}

bool WaterCurrentSourceTask::startHook()
{
    if (! WaterCurrentSourceTaskBase::startHook())
        return false;
    return true;
}

void WaterCurrentSourceTask::updateHook()
{
    WaterCurrentSourceTaskBase::updateHook();

    base::Vector3d water_current_target;
    if (_water_velocity_target.read(water_current_target) != RTT::NewData) {
        return;
    }

    gz::msgs::Vector3d msg;
    msg.set_x(water_current_target.x());
    msg.set_y(water_current_target.y());
    msg.set_z(water_current_target.z());
    m_publisher.Publish(msg);
}

void WaterCurrentSourceTask::errorHook()
{
    WaterCurrentSourceTaskBase::errorHook();
}
void WaterCurrentSourceTask::stopHook()
{
    gz::msgs::Vector3d msg;
    msg.set_x(0.0);
    msg.set_y(0.0);
    msg.set_z(0.0);
    m_publisher.Publish(msg);

    WaterCurrentSourceTaskBase::stopHook();
}
void WaterCurrentSourceTask::cleanupHook()
{
    WaterCurrentSourceTaskBase::cleanupHook();
}
