#include "WaveSourceTask.hpp"

#include <rock_gazebo/Helpers.hpp>

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

    rock_gazebo::GazeboSync sync(*this);

    // Set gazebo topic to advertise
    m_node.reset(new gz::transport::Node());

    m_publisher =
        m_node->Advertise<gz::gazebo_usv::Wave>("/" + m_model_name + "/wave_amplitude");
    gzmsg << "WaveSourceTask: advertising to gazebo topic /" + m_model_name +
                 "/wave_amplitude"
          << endl;

    return true;
}

void WaveSourceTask::setGazebo(gz::sim::Entity const& entity,
    sdf::ElementConstPtr const& sdf,
    gz::sim::EntityComponentManager& ecm,
    gz::sim::EventManager& event_manager) {

    throw std::logic_error("not implemented error");
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
