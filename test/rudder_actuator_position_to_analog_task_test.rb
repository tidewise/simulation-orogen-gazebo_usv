# frozen_string_literal: true

using_task_library "gazebo_usv"

describe OroGen.gazebo_usv.RudderActuatorPositionToAnalogTask do
    run_live

    attr_reader :task

    before do
        @task = syskit_deploy(
            OroGen.gazebo_usv.RudderActuatorPositionToAnalogTask
                  .deployed_as("task_under_test")
        )
    end

    it "mirrors the position on the analog output with the default configuration" do
        syskit_configure_and_start(@task)

        position = make_joints(position: 0.1)
        analog = expect_execution { syskit_write @task.position_samples_port, position }
                   .to { have_one_new_sample task.analog_out_port }

        assert_equal position.time, analog[0].time
        assert_in_delta 0.1, analog[0].data
    end

    it "converts the position using the provided min/max values" do
        @task.properties.analog_min = 200
        @task.properties.analog_max = 400
        @task.properties.position_min = -1
        @task.properties.position_max = 1

        syskit_configure_and_start(@task)

        position = make_joints(position: -0.5)
        analog =
            expect_execution do
                syskit_write @task.position_samples_port, position
            end.to { have_one_new_sample task.analog_out_port }

        assert_in_delta 250.0, analog[0].data
    end

    it "emits INVALID_POSITION_SAMPLE if input sample mode is not POSITION" do
        syskit_configure_and_start(@task)

        sample = make_joints(speed: -0.5)
        expect_execution do
            syskit_write @task.position_samples_port, sample
        end.to do
            emit task.invalid_position_sample_event
        end
    end

    def make_joints(fields, now: Time.now)
        joint_state = Types.base.JointState.new(
            fields
        )

        joints = Types.base.samples.Joints.new(
            time: now,
            elements: [joint_state]
        )
        joints
    end
end
