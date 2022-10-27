#include "WaveSourceTask.hpp"
#include <regex>

using namespace std;
using namespace gazebo;
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
    m_node = transport::NodePtr(new transport::Node());
    m_node->Init();

    m_wave_amplitude_publisher =
        m_node->Advertise<gazebo::msgs::Vector3d>("/" + m_model_name + "/wave_amplitude");
    gzmsg << "WaveSourceTask: advertising to gazebo topic /" + m_model_name +
                 "/wave_amplitude"
          << endl;

    m_wave_frequency_publisher =
        m_node->Advertise<gazebo::msgs::Vector3d>("/" + m_model_name + "/wave_frequency");
    gzmsg << "WaveSourceTask: advertising to gazebo topic /" + m_model_name +
                 "/wave_frequency"
          << endl;

    m_roll_publisher =
        m_node->Advertise<gazebo::msgs::Vector2d>("/" + m_model_name + "/roll_vector");
    gzmsg << "WaveSourceTask: advertising to gazebo topic /" + m_model_name +
                 "/roll_vector"
          << endl;
    return true;
}

void WaveSourceTask::setGazeboModel(std::string const& pluginName, ModelPtr model)
{
    m_model_name = getNamespaceFromPluginName(pluginName);
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
    base::Vector3d wave_amplitude;
    base::Vector3d wave_frequency;
    double roll_amplitude;
    double roll_frequency;

    bool new_wave_attributes = _wave_attributes.read(wave_attributes) != RTT::NewData;
    if (new_wave_attributes)
        return;
    wave_amplitude = wave_attributes.wave_amplitude;
    wave_frequency = wave_attributes.wave_frequency;
    roll_amplitude = wave_attributes.roll_amplitude;
    roll_frequency = wave_attributes.roll_frequency;

    gazebo::msgs::Vector3d wave_amp_msg;
    wave_amp_msg.set_x(wave_amplitude.x());
    wave_amp_msg.set_y(wave_amplitude.y());
    wave_amp_msg.set_z(wave_amplitude.z());
    m_wave_amplitude_publisher->Publish(wave_amp_msg);

    gazebo::msgs::Vector3d wave_freq_msg;
    wave_freq_msg.set_x(wave_frequency.x());
    wave_freq_msg.set_y(wave_frequency.y());
    wave_freq_msg.set_z(wave_frequency.z());
    m_wave_frequency_publisher->Publish(wave_freq_msg);

    gazebo::msgs::Vector2d roll_msg;
    roll_msg.set_x(roll_amplitude);
    roll_msg.set_y(roll_frequency);
    m_roll_publisher->Publish(roll_msg);

    WaveSourceTaskBase::updateHook();
}
void WaveSourceTask::errorHook()
{
    WaveSourceTaskBase::errorHook();
}
void WaveSourceTask::stopHook()
{
    gazebo::msgs::Vector3d wave_amp_msg;
    wave_amp_msg.set_x(0);
    wave_amp_msg.set_y(0);
    wave_amp_msg.set_z(0);
    m_wave_amplitude_publisher->Publish(wave_amp_msg);

    gazebo::msgs::Vector3d wave_freq_msg;
    wave_freq_msg.set_x(0);
    wave_freq_msg.set_y(0);
    wave_freq_msg.set_z(0);
    m_wave_frequency_publisher->Publish(wave_freq_msg);

    gazebo::msgs::Vector2d roll_msg;
    roll_msg.set_x(0);
    roll_msg.set_y(0);
    m_roll_publisher->Publish(roll_msg);
    WaveSourceTaskBase::stopHook();
}
void WaveSourceTask::cleanupHook()
{
    WaveSourceTaskBase::cleanupHook();
}
