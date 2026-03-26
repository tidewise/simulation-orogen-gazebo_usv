/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "DirectForceApplicationTask.hpp"

#include <string>
#include <rock_gazebo/Helpers.hpp>

using namespace gazebo_usv;

DirectForceApplicationTask::DirectForceApplicationTask(std::string const& name)
    : DirectForceApplicationTaskBase(name)
{
}

DirectForceApplicationTask::~DirectForceApplicationTask()
{
}

void DirectForceApplicationTask::setGazebo(gz::sim::Entity const& entity,
    sdf::ElementConstPtr const& sdf,
    gz::sim::EntityComponentManager& ecm,
    gz::sim::EventManager& event_manager) {
    throw std::runtime_error("not implemented");
}


/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See DirectForceApplicationTask.hpp for more detailed
// documentation about them.

bool DirectForceApplicationTask::configureHook()
{
    if (! DirectForceApplicationTaskBase::configureHook()) {
        return false;
    }

    rock_gazebo::GazeboSync sync(*this);

    this->m_node = std::make_shared<gz::transport::Node>();
    auto topic_name = "";
    m_publisher = m_node->Advertise<gz::msgs::Vector3d>(topic_name);

    return true;
}
bool DirectForceApplicationTask::startHook()
{
    if (! DirectForceApplicationTaskBase::startHook()) {
        return false;
    }

    return true;
}
void DirectForceApplicationTask::updateHook()
{
    DirectForceApplicationTaskBase::updateHook();

    base::Vector3d force_cmd;
    if (_force.read(force_cmd) != RTT::NewData) {
        return;
    }

    gz::msgs::Vector3d msg;
    msg.set_x(force_cmd.x());
    msg.set_y(force_cmd.y());
    msg.set_z(force_cmd.z());

    m_publisher.Publish(msg);
}
void DirectForceApplicationTask::errorHook()
{
    DirectForceApplicationTaskBase::errorHook();
}
void DirectForceApplicationTask::stopHook()
{
    DirectForceApplicationTaskBase::stopHook();
}
void DirectForceApplicationTask::cleanupHook()
{
    DirectForceApplicationTaskBase::cleanupHook();
    m_node.reset();
}
