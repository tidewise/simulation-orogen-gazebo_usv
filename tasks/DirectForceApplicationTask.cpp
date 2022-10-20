/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "DirectForceApplicationTask.hpp"

#include <string>

#include <gazebo/physics/Model.hh>

using namespace gazebo_usv;

DirectForceApplicationTask::DirectForceApplicationTask(std::string const& name)
    : DirectForceApplicationTaskBase(name)
{
    this->m_node = boost::make_shared<gazebo::transport::Node>();
    this->m_node->Init();
}

DirectForceApplicationTask::~DirectForceApplicationTask()
{
}

void DirectForceApplicationTask::setGazeboModel(std::string const& plugin_name, gazebo::physics::ModelPtr model) 
{
    m_model_name = getNamespaceFromPluginName(plugin_name);
}


/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See DirectForceApplicationTask.hpp for more detailed
// documentation about them.

bool DirectForceApplicationTask::configureHook()
{
    if (! DirectForceApplicationTaskBase::configureHook()) {
        return false;
    }

    auto topic_name = m_model_name + "/" + _link_name.get() + "/gazebo_usv_force";
    m_force_pub = m_node->Advertise<gazebo::msgs::Vector3d>("/" + topic_name);

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

    gazebo::msgs::Vector3d msg;
    msg.set_x(force_cmd.x());
    msg.set_y(force_cmd.y());
    msg.set_z(force_cmd.z());

    m_force_pub->Publish(msg);
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

    m_force_pub->Fini();
}
