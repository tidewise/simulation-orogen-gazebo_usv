#include "WindSourceTask.hpp"
#include <gazebo_usv/Components.hpp>
#include <rock_gazebo/Helpers.hpp>

#include <base-logging/Logging.hpp>
#include <gz/sim/Util.hh>


using namespace std;
using namespace gazebo_usv;

WindSourceTask::WindSourceTask(std::string const &name)
    : WindSourceTaskBase(name)
{
}

WindSourceTask::~WindSourceTask()
{
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See WindSourceTask.hpp for more detailed
// documentation about them.

bool WindSourceTask::configureHook()
{
    if (!WindSourceTaskBase::configureHook())
        return false;

    // Set gazebo topic to advertise
    auto topic_name = resolveTopicName();

    rock_gazebo::GazeboSync sync(*this);
    m_node = std::make_shared<gz::transport::Node>();
    gzmsg << "WaveSourceTask: advertising to gazebo topic " + topic_name
          << endl;
    m_publisher = m_node->Advertise<gz::msgs::Vector3d>(topic_name);

    return true;
}

std::string WindSourceTask::resolveTopicName() {
    base::Time deadline = base::Time::now() + base::Time::fromSeconds(5);
    while (base::Time::now() < deadline) {
        {
            rock_gazebo::GazeboSync sync(*this);

            optional<string> topic_name =
                m_ecm->ComponentData<gazebo_usv::WindSpeedTopic>(m_entity);
            if (topic_name.has_value()) {
                return *topic_name;
            }
        }

        this_thread::sleep_for(chrono::milliseconds(100));
    }

    LOG_ERROR_S << "could not find topic name for WindSourceTask attached to "
                << gz::sim::scopedName(m_entity, *m_ecm, "::", false);
    throw std::logic_error("no wind plugin attached to given model");
}

void WindSourceTask::setGazebo(gz::sim::Entity const& entity,
    sdf::ElementConstPtr const& sdf,
    gz::sim::EntityComponentManager& ecm,
    gz::sim::EventManager& event_manager) {

    BaseTask::setGazebo(entity, sdf, ecm, event_manager);

    m_entity = entity;
    m_ecm = &ecm;
}

bool WindSourceTask::startHook()
{
    if (!WindSourceTaskBase::startHook())
        return false;
    return true;
}
void WindSourceTask::updateHook()
{
    base::Vector3d wind_velocity;
    if (_wind_velocity.read(wind_velocity) != RTT::NewData)
        return;

    base::samples::RigidBodyState wind_pose;
    wind_pose.velocity = wind_velocity;
    _wind_pose.write(wind_pose);

    gz::msgs::Vector3d wind_vel_msg;
    wind_vel_msg.set_x(wind_velocity.x());
    wind_vel_msg.set_y(wind_velocity.y());
    wind_vel_msg.set_z(wind_velocity.z());
    m_publisher.Publish(wind_vel_msg);
    WindSourceTaskBase::updateHook();
}
void WindSourceTask::errorHook()
{
    WindSourceTaskBase::errorHook();
}
void WindSourceTask::stopHook()
{
    base::samples::RigidBodyState wind_pose;
    wind_pose.velocity = Eigen::Vector3d::Zero();
    _wind_pose.write(wind_pose);

    gz::msgs::Vector3d wind_vel_msg;
    wind_vel_msg.set_x(0);
    wind_vel_msg.set_y(0);
    wind_vel_msg.set_z(0);
    m_publisher.Publish(wind_vel_msg);
    WindSourceTaskBase::stopHook();
}
void WindSourceTask::cleanupHook()
{
    WindSourceTaskBase::cleanupHook();
}
