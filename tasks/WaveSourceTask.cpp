#include "WaveSourceTask.hpp"
#include <regex>

using namespace std;
using namespace gazebo;
using namespace gazebo_usv;

WaveSourceTask::WaveSourceTask(std::string const &name)
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

    m_wave_amplitude_publisher = m_node->Advertise<gazebo::msgs::Vector3d>("/" + m_model_name + "/wave_amplitude");
    gzmsg << "WaveSourceTask: advertising to gazebo topic /" + m_model_name + "/wave_amplitude" << endl;

    m_wave_frequency_publisher = m_node->Advertise<gazebo::msgs::Vector3d>("/" + m_model_name + "/wave_frequency");
    gzmsg << "WaveSourceTask: advertising to gazebo topic /" + m_model_name + "/wave_frequency" << endl;
        
    return true;
}

void WaveSourceTask::setGazeboModel(std::string const &pluginName, ModelPtr model)
{
    m_model_name = std::regex_replace(pluginName, std::regex("__"), "/");
}

void WaveSourceTask::setGazeboPluginTaskName( std::string const& pluginTaskName )
{
    provides()->setName(pluginTaskName);
    _name.set(pluginTaskName);
}


bool WaveSourceTask::startHook()
{
    if (!WaveSourceTaskBase::startHook())
        return false;
    return true;
}
void WaveSourceTask::updateHook()
{
    base::Vector3d wave_amplitude;
    base::Vector3d wave_frequency;
    
    bool new_wave_amplitude = _wave_amplitude.read(wave_amplitude) != RTT::NewData;
    bool new_wave_frequency = _wave_frequency.read(wave_frequency) != RTT::NewData;
    if (new_wave_amplitude && new_wave_frequency)
        return;

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

    WaveSourceTaskBase::stopHook();
}
void WaveSourceTask::cleanupHook()
{
    WaveSourceTaskBase::cleanupHook();
}
