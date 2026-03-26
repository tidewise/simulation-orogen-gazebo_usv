/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "ThrusterTask.hpp"
#include <gazebo_usv/Components.hpp>
#include <rock_gazebo/Helpers.hpp>

#include <base-logging/Logging.hpp>
#include <gz/sim/Util.hh>

using namespace std;
using namespace gazebo_usv;

ThrusterTask::ThrusterTask(string const& name)
    : ThrusterTaskBase(name)
{
}

ThrusterTask::ThrusterTask(string const& name, RTT::ExecutionEngine* engine)
    : ThrusterTaskBase(name, engine)
{
}

ThrusterTask::~ThrusterTask()
{
}

bool ThrusterTask::configureHook()
{
    if (!ThrusterTaskBase::configureHook())
        return false;

    // Initialize communication node and subscribe to gazebo topic
    {
        rock_gazebo::GazeboSync sync(*this);
        m_node = std::make_shared<gz::transport::Node>();
    }

    base::Time deadline = base::Time::now() + base::Time::fromSeconds(5);
    while (base::Time::now() < deadline) {
        {
            rock_gazebo::GazeboSync sync(*this);

            optional<string> topic_name =
                m_ecm->ComponentData<gazebo_usv::ThrustersTopic>(m_entity);
            if (topic_name.has_value()) {
                gzmsg << "ThrusterTask: advertising to gazebo topic " + *topic_name
                      << endl;
                m_publisher = m_node->Advertise<gz::gazebo_usv::Thrusters>(*topic_name);
                break;
            }
        }

        this_thread::sleep_for(chrono::milliseconds(100));
    }

    if (!m_publisher.Valid()) {
        LOG_ERROR_S << "could not find topic name for ThrusterTask attached to "
                    << gz::sim::scopedName(m_entity, *m_ecm, "::", false);
        return false;
    }

    return true;
}

bool ThrusterTask::startHook()
{
    if (!ThrusterTaskBase::startHook())
        return false;

    return true;
}

void ThrusterTask::updateHook()
{
    ThrusterTaskBase::updateHook();

    // Read Rock input port and update the message
    base::samples::Joints joints_cmd;
    if (_thrusters_cmd.read(joints_cmd) != RTT::NewData)
        return;

    if (joints_cmd.names.empty()) {
        exception(NO_JOINT_NAMES);
        return;
    }

    gz::gazebo_usv::Thrusters msg;
    for (auto const& jointName : joints_cmd.names) {
        base::JointState jointState = joints_cmd.getElementByName(jointName);
        gz::gazebo_usv::Thruster* thruster = msg.add_thrusters();
        thruster->set_name(jointName);
        if (jointState.isEffort()) {
            thruster->set_effort(jointState.effort);
        }
    }

    // Write in gazebo topic
    if (m_publisher.HasConnections()) {
        m_publisher.Publish(msg);
    }
    else {
        gzmsg << "ThrusterTask: publisher has no connections. Going into exception"
              << endl;
        exception(NO_TOPIC_CONNECTION);
    }
    joints_cmd.time = base::Time::now();
    _joint_samples.write(joints_cmd);
}

void ThrusterTask::errorHook()
{
    ThrusterTaskBase::errorHook();
}

void ThrusterTask::stopHook()
{
    ThrusterTaskBase::stopHook();
}

void ThrusterTask::cleanupHook()
{
    ThrusterTaskBase::cleanupHook();
    m_node.reset();
}

void ThrusterTask::setGazebo(gz::sim::Entity const& entity,
    sdf::ElementConstPtr const& sdf,
    gz::sim::EntityComponentManager& ecm,
    gz::sim::EventManager& event_manager)
{
    BaseTask::setGazebo(entity, sdf, ecm, event_manager);

    m_entity = entity;
    m_ecm = &ecm;
}
