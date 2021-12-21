#include "WindSourceTask.hpp"

using namespace std;
using namespace gazebo;
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
    mNode = transport::NodePtr(new transport::Node());
    mNode->Init();

    mWindVelocityPublisher = mNode->Advertise<gazebo::msgs::Vector3d>("~/" + mModelName + "/wind_velocity");
    gzmsg << "WindSourceTask: advertising to gazebo topic ~/" + mModelName + "/wind_velocity" << endl;
    return true;
}

void WindSourceTask::setGazeboModel(std::string const &pluginName, ModelPtr model)
{
    string worldName = model->GetWorld()->Name();

    string taskName = "gazebo:" + worldName + ":" + model->GetName() + ":" + pluginName;
    provides()->setName(taskName);
    _name.set(taskName);

    mModelName = model->GetName();
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
    
    gazebo::msgs::Vector3d wind_vel_msg;
    wind_vel_msg.set_x(wind_velocity.x());
    wind_vel_msg.set_y(wind_velocity.y());
    wind_vel_msg.set_z(wind_velocity.z());
    mWindVelocityPublisher->Publish(wind_vel_msg);
    
    WindSourceTaskBase::updateHook();
}
void WindSourceTask::errorHook()
{
    WindSourceTaskBase::errorHook();
}
void WindSourceTask::stopHook()
{
    WindSourceTaskBase::stopHook();
}
void WindSourceTask::cleanupHook()
{
    WindSourceTaskBase::cleanupHook();
}
