/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "TetherTractionTask.hpp"

#include <iostream>

#include <gazebo/msgs/vector3d.pb.h>

using namespace gazebo_usv;

TetherTractionTask::TetherTractionTask(std::string const& name)
    : TetherTractionTaskBase(name)
{
}

TetherTractionTask::~TetherTractionTask()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See TetherTractionTask.hpp for more detailed
// documentation about them.

bool TetherTractionTask::configureHook()
{
    if (! TetherTractionTaskBase::configureHook()) {
        return false;
    }

    return true;
}
bool TetherTractionTask::startHook()
{
    if (! TetherTractionTaskBase::startHook()) {
        return false;
    }

    this->node = boost::make_shared<gazebo::transport::Node>();
    this->node->Init();
    // directForcePub = node->Advertise<ConstVector3dPtr>("~/" + topicName);
    // gzmsg << "TetherTractionTask: advertising to gazebo topic ~/" + topicName << endl;

    return true;
}
void TetherTractionTask::updateHook()
{
    TetherTractionTaskBase::updateHook();

    if (!isVesselInitialized && _vessel_link.read(this->vesselAttachmentState) != RTT::NewData) {
        auto topicName = this->vesselAttachmentState.sourceFrame + "directional_force";
        directForceVesselPub = node->Advertise<gazebo::msgs::Vector3d>("~/" + topicName);

        gzmsg << "DEBUG: topic name is: " << topicName << std::endl;

        isVesselInitialized = true;
        gzmsg << "TetherTractionTask: advertising to gazebo topic ~/" + topicName << std::endl;
    }

    if (_rov_link.read(this->rovAttachmentState) != RTT::NewData) {
        bool isRovInitialized = false;
    }

    auto vessel_attachment2rov_attachment =
        this->vesselAttachmentState.getTransform() * this->rovAttachmentState.getTransform();

    auto tether_lenght = _length.value();
}
void TetherTractionTask::errorHook()
{
    TetherTractionTaskBase::errorHook();
}
void TetherTractionTask::stopHook()
{
    TetherTractionTaskBase::stopHook();
}
void TetherTractionTask::cleanupHook()
{
    TetherTractionTaskBase::cleanupHook();
}
