#include "WaveSourceTask.hpp"
#include <gazebo_usv/Components.hpp>
#include <rock_gazebo/Helpers.hpp>

#include <base-logging/Logging.hpp>
#include <gz/sim/Util.hh>

using namespace std;
using namespace gazebo_usv;

WaveSourceTask::WaveSourceTask(std::string const& name)
    : WaveSourceTaskBase(name)
{
}

WaveSourceTask::~WaveSourceTask()
{
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See WaveSourceTask.hpp for more detailed
// documentation about them.

bool WaveSourceTask::configureHook()
{
    if (!WaveSourceTaskBase::configureHook())
        return false;

    // Set gazebo topic to advertise
    auto topic_name = resolveTopicName();

    rock_gazebo::GazeboSync sync(*this);
    m_node = std::make_shared<gz::transport::Node>();
    gzmsg << "WaveSourceTask: advertising to gazebo topic " + topic_name
          << endl;
    m_publisher = m_node->Advertise<gz::gazebo_usv::Wave>(topic_name);

    return true;
}

std::string WaveSourceTask::resolveTopicName() {
    base::Time deadline = base::Time::now() + base::Time::fromSeconds(5);
    while (base::Time::now() < deadline) {
        {
            rock_gazebo::GazeboSync sync(*this);

            optional<string> topic_name =
                m_ecm->ComponentData<gazebo_usv::WaveParametersTopic>(m_entity);
            if (topic_name.has_value()) {
                return *topic_name;
            }
        }

        this_thread::sleep_for(chrono::milliseconds(100));
    }

    LOG_ERROR_S << "could not find topic name for WaveSourceTask attached to "
                << gz::sim::scopedName(m_entity, *m_ecm, "::", false);
    throw std::logic_error("no wave plugin attached to given model");
}

void WaveSourceTask::setGazebo(gz::sim::Entity const& entity,
    sdf::ElementConstPtr const& sdf,
    gz::sim::EntityComponentManager& ecm,
    gz::sim::EventManager& event_manager) {

    BaseTask::setGazebo(entity, sdf, ecm, event_manager);

    m_entity = entity;
    m_ecm = &ecm;
}

bool WaveSourceTask::startHook()
{
    if (!WaveSourceTaskBase::startHook())
        return false;
    return true;
}
void WaveSourceTask::updateHook()
{
    WaveAttributes wave_attributes;
    if (_wave_attributes.read(wave_attributes) != RTT::NewData) {
        return;
    }

    gz::gazebo_usv::Wave wave;
    *wave.mutable_amplitude() = rock_gazebo::eigen2Proto(wave_attributes.wave_amplitude);
    *wave.mutable_frequency() = rock_gazebo::eigen2Proto(wave_attributes.wave_frequency);
    wave.set_roll_amplitude(wave_attributes.roll_amplitude);
    wave.set_roll_frequency(wave_attributes.roll_frequency);
    m_publisher.Publish(wave);

    WaveSourceTaskBase::updateHook();
}
void WaveSourceTask::errorHook()
{
    WaveSourceTaskBase::errorHook();
}
void WaveSourceTask::stopHook()
{
    gz::gazebo_usv::Wave wave;
    *wave.mutable_amplitude() = rock_gazebo::eigen2Proto(Eigen::Vector3d::Zero());
    *wave.mutable_frequency() = rock_gazebo::eigen2Proto(Eigen::Vector3d::Zero());
    wave.set_roll_amplitude(0);
    wave.set_roll_frequency(0);
    m_publisher.Publish(wave);
    WaveSourceTaskBase::stopHook();
}
void WaveSourceTask::cleanupHook()
{
    WaveSourceTaskBase::cleanupHook();
}
