#include "WaveSourceTask.hpp"

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
    mNode = transport::NodePtr(new transport::Node());
    mNode->Init();

    mWaveVelocityPublisher = mNode->Advertise<gazebo::msgs::Vector3d>("/" + mModelName + "/wave_velocity");
    gzmsg << "WaveSourceTask: advertising to gazebo topic ~/" + mModelName + "/wave_velocity" << endl;

    mWaveFrequencyPublisher = mNode->Advertise<gazebo::msgs::Vector3d>("/" + mModelName + "/wave_frequency");
    gzmsg << "WaveSourceTask: advertising to gazebo topic ~/" + mModelName + "/wave_frequency" << endl;
        
    cout << "Task";
    cout << mModelName;
    return true;
}

void WaveSourceTask::setGazeboModel(std::string const &pluginName, ModelPtr model)
{
    string worldName = model->GetWorld()->Name();

    string taskName = std::regex_replace(pluginName, std::regex("__"), "::");
    provides()->setName(taskName);
    _name.set(taskName);

    mModelName = std::regex_replace(pluginName, std::regex("__"), "/");
}

bool WaveSourceTask::startHook()
{
    if (!WaveSourceTaskBase::startHook())
        return false;
    return true;
}
void WaveSourceTask::updateHook()
{
    base::Vector3d wave_velocity;
    base::Vector3d wave_frequency;
    
    bool new_wave_velocity = _wave_velocity.read(wave_velocity) != RTT::NewData;
    bool new_wave_frequency = _wave_frequency.read(wave_frequency) != RTT::NewData;
    if (new_wave_velocity && new_wave_frequency)
        return;

    gazebo::msgs::Vector3d wave_vel_msg;
    wave_vel_msg.set_x(wave_velocity.x());
    wave_vel_msg.set_y(wave_velocity.y());
    wave_vel_msg.set_z(wave_velocity.z());
    mWaveVelocityPublisher->Publish(wave_vel_msg);

    gazebo::msgs::Vector3d wave_per_msg;
    wave_per_msg.set_x(wave_frequency.x());
    wave_per_msg.set_y(wave_frequency.y());
    wave_per_msg.set_z(wave_frequency.z());
    mWaveFrequencyPublisher->Publish(wave_per_msg);
    WaveSourceTaskBase::updateHook();
}
void WaveSourceTask::errorHook()
{
    WaveSourceTaskBase::errorHook();
}
void WaveSourceTask::stopHook()
{
    gazebo::msgs::Vector3d wave_vel_msg;
    wave_vel_msg.set_x(0);
    wave_vel_msg.set_y(0);
    wave_vel_msg.set_z(0);
    mWaveVelocityPublisher->Publish(wave_vel_msg);

    gazebo::msgs::Vector3d wave_per_msg;
    wave_per_msg.set_x(0);
    wave_per_msg.set_y(0);
    wave_per_msg.set_z(0);
    mWaveFrequencyPublisher->Publish(wave_per_msg);

    WaveSourceTaskBase::stopHook();
}
void WaveSourceTask::cleanupHook()
{
    WaveSourceTaskBase::cleanupHook();
}
