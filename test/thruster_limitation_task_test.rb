# frozen_string_literal: true

using_task_library "gazebo_usv"
import_types_from "base"
import_types_from "gazebo_usv"

describe OroGen.gazebo_usv.ThrusterLimitationTask do
    run_live

    attr_reader :task

    before do
        @task = syskit_deploy(
            OroGen
            .gazebo_usv
            .ThrusterLimitationTask
            .deployed_as("thruster_limitation_test")
        )

        @task.properties.limits = Types.base.JointLimits.new(
            elements: [{
                min: Types.base.JointState.Effort(-1000),
                max: Types.base.JointState.Effort(1000)
            }, {
                min: Types.base.JointState.Effort(-1000),
                max: Types.base.JointState.Effort(1000)
            }],
            names: %w[port stbd]
        )
        @max_cmd = effort_joint(1000, 1000)
        @min_cmd = effort_joint(-1000, -1000)
        @cmd = effort_joint(500, 500)
        @cmd_above_the_max_limit = effort_joint(1100, 1100)
        @cmd_below_the_min_limit = effort_joint(-1100, -1100)

        @saturated_signal = Types.control_base.SaturationSignal.new(value: true)
        @not_saturated_signal = Types.control_base.SaturationSignal.new(value: false)
    end

    it "does not return any saturation signal when there is no input command" do
        syskit_configure_and_start(@task)
        expect_execution.to do
            have_no_new_sample(task.saturation_signal_port, at_least_during: 0.2)
        end
    end

    it "does not return any output command when there is no input command" do
        syskit_configure_and_start(@task)
        expect_execution.to do
            have_no_new_sample(task.cmd_out_port, at_least_during: 0.2)
        end
    end

    it "forwards the input command when it is inside the limited range" do
        syskit_configure_and_start(@task)
        output = expect_execution do
            syskit_write @task.cmd_in_port, @cmd
        end.to do
            have_one_new_sample(task.cmd_out_port)
        end

        assert_equal(@cmd.elements[0].effort, output.elements[0].effort)
    end

    it "limits the input command when it is bigger than the max limit range" do
        syskit_configure_and_start(@task)
        output = expect_execution do
            syskit_write @task.cmd_in_port, @cmd_above_the_max_limit
        end.to do
            have_one_new_sample(task.cmd_out_port)
        end

        assert_equal(@max_cmd.elements[0].effort, output.elements[0].effort)
    end

    it "limits the input command when it is smaller than the min limit range" do
        syskit_configure_and_start(@task)
        output = expect_execution do
            syskit_write @task.cmd_in_port, @cmd_below_the_min_limit
        end.to do
            have_one_new_sample(task.cmd_out_port)
        end

        assert_equal(@min_cmd.elements[0].effort, output.elements[0].effort)
    end

    it "returns a saturated signal when the input command is out of the limited range" do
        syskit_configure_and_start(@task)
        output = expect_execution do
            syskit_write @task.cmd_in_port, @cmd_above_the_max_limit
        end.to do
            have_one_new_sample(task.saturation_signal_port)
        end
        assert_equal(@saturated_signal, output)

        output2 = expect_execution do
            syskit_write @task.cmd_in_port, @cmd_below_the_min_limit
        end.to do
            have_one_new_sample(task.saturation_signal_port)
        end
        assert_equal(@saturated_signal, output2)
    end

    it "returns a no saturated signal when the input command within the limited range" do
        syskit_configure_and_start(@task)
        output = expect_execution do
            syskit_write @task.cmd_in_port, @cmd
        end.to do
            have_one_new_sample(task.saturation_signal_port)
        end

        assert_equal(@not_saturated_signal, output)
    end

    def effort_joint(port, stbd)
        Types.base.samples.Joints.new(
            elements: [{ effort: port }, { effort: stbd }],
            names: %w[port stbd]
        )
    end
end
