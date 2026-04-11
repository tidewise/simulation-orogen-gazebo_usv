/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "WaterCurrentSourceTask.hpp"
#include <gazebo_usv/Components.hpp>
#include <rock_gazebo/Helpers.hpp>

#include <base-logging/Logging.hpp>
#include <gz/sim/Util.hh>

using namespace gazebo_usv;

WaterCurrentSourceTask::WaterCurrentSourceTask(std::string const& name)
    : WaterCurrentSourceTaskBase(name)
{
}

WaterCurrentSourceTask::~WaterCurrentSourceTask()
{
}

void WaterCurrentSourceTask::setGazebo(gz::sim::Entity const& entity,
    sdf::ElementConstPtr const& sdf,
    gz::sim::EntityComponentManager& ecm,
    gz::sim::EventManager& event_manager) {

    BaseTask::setGazebo(entity, sdf, ecm, event_manager);

    m_entity = entity;
    m_ecm = &ecm;
}

bool WaterCurrentSourceTask::configureHook()
{
    if (! WaterCurrentSourceTaskBase::configureHook())
        return false;
    return true;
}
bool WaterCurrentSourceTask::startHook()
{
    if (! WaterCurrentSourceTaskBase::startHook())
        return false;
    return true;
}
void WaterCurrentSourceTask::updateHook()
{
    WaterCurrentSourceTaskBase::updateHook();
}
void WaterCurrentSourceTask::errorHook()
{
    WaterCurrentSourceTaskBase::errorHook();
}
void WaterCurrentSourceTask::stopHook()
{
    WaterCurrentSourceTaskBase::stopHook();
}
void WaterCurrentSourceTask::cleanupHook()
{
    WaterCurrentSourceTaskBase::cleanupHook();
}
