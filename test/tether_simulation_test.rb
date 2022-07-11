# frozen_string_literal: true

using_task_library "gazebo_usv"
import_types_from "gazebo_usv"
import_types_from "base"

describe OroGen.gazebo_usv.TetherSimulationTask do
    run_live

    attr_reader :task

    before do
        @task = syskit_deploy(
            OroGen.gazebo_usv
                  .TetherSimulationTask
                  .deployed_as("tether_simulation_test")
        )

        @task.properties.max_tether_length = 150
        @task.properties.tether_config = \
            Types.uuv_tether_control.TetherDragAndInertiaConfig.new
        @task.properties.tether_config.zero!
        @task.properties.tether_config.saltwater_density = 1023
        @task.properties.tether_config.mass_per_unit_length = 0
        @task.properties.tether_config.diameter = 0.011
        @task.properties.tether_config.drag_coefficient = 1.2
        @task.properties.tether_config.added_mass_coefficient = 1

        @rov_pose = Types.base.samples.RigidBodyState.Invalid
        @rov_pose.position = Eigen::Vector3.new(3, 0, -4)
        @rov_pose.orientation = Eigen::Quaternion.from_angle_axis(0, Eigen::Vector3.UnitZ)
        @rov_pose.velocity = Eigen::Vector3.Zero

        @usv_pose = Types.base.samples.RigidBodyState.Invalid
        @usv_pose.position = Eigen::Vector3.new(0, 0, 0)
        @usv_pose.orientation = Eigen::Quaternion.from_angle_axis(0, Eigen::Vector3.UnitZ)
        @usv_pose.velocity = Eigen::Vector3.Zero

        @water_velocity = Eigen::Vector3.new(-5, 0, 0)
    end

    it "returns tether force " do
        syskit_configure_and_start(task)

        output = expect_execution do
            syskit_write task.rov_attachment_port, @rov_pose
            syskit_write task.usv_attachment_port, @usv_pose
            syskit_write task.water_speed_port, @water_velocity
        end.to do
            [
                have_one_new_sample(task.rov_force_port),
                have_one_new_sample(task.usv_force_port)
            ]
        end

        assert_equal(405.108, output[0].x)
        assert_equal(0, output[0].y)
        assert_equal(0, output[0].z)

        assert_equal(-405.108, output[1].x)
        assert_equal(0, output[1].y)
        assert_equal(0, output[1].z)
    end
end
