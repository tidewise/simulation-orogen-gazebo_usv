# frozen_string_literal: true

using_task_library "gazebo_usv"
import_types_from "base"
import_types_from "gazebo_usv"

describe OroGen.gazebo_usv.ReedSwitchSimulatorTask do
    run_live

    attr_reader :task

    before do
        @task = syskit_deploy(
            OroGen.gazebo_usv.ReedSwitchSimulatorTask.deployed_as("reed_simulator_test")
        )
        @task.properties.revolution_time = Time.at(1)
        @task.properties.trigger_time = Time.at(0.3)

        @gpio_true = gpio_state(1)
        @gpio_false = gpio_state(0)
    end

    def gpio_state(state)
        Types.linux_gpios.GPIOState.new(
            time: Time.now, states: [{ time: Time.now, data: state }]
        )
    end

    def assert_gpio_state(expected_state, gpio)
        assert_equal(expected_state, gpio.states[0].data)
    end

    it "does not return any sample when there is no gpio_retract" do
        syskit_configure_and_start(@task)
        expect_execution do
            syskit_write @task.gpio_extend_port, gpio_state(1)
        end.to do
            have_no_new_sample(task.gpio_reed_switch_port, at_least_during: 0.2)
        end
    end

    it "emits an invalid gpio size exception when the gpio size is bigger than "\
    "expected" do
        wrong_gpio = Types.linux_gpios.GPIOState.new(
            time: Time.now, states: [{ time: Time.now, data: 1 },
                                     { time: Time.now, data: 1 }]
        )
        syskit_configure_and_start(@task)
        expect_execution do
            syskit_write @task.gpio_extend_port, wrong_gpio
            syskit_write @task.gpio_retract_port, gpio_state(1)
        end.to_emit @task.invalid_gpio_size_event
    end

     it "emits an invalid gpio size exception when the gpio size is smaller than "\
    "expected" do
        wrong_gpio = Types.linux_gpios.GPIOState.new(
            time: Time.now, states: []
        )
        syskit_configure_and_start(@task)
        expect_execution do
            syskit_write @task.gpio_retract_port, wrong_gpio
            syskit_write @task.gpio_extend_port, gpio_state(1)
        end.to_emit @task.invalid_gpio_size_event
    end

    it "does not return any sample when there is no gpio_extend" do
        syskit_configure_and_start(@task)
        expect_execution do
            syskit_write @task.gpio_retract_port, gpio_state(1)
        end.to do
            have_no_new_sample(task.gpio_reed_switch_port, at_least_during: 0.2)
        end
    end

    it "throw an error when both gpio states are true" do
        syskit_configure_and_start(@task)
        expect_execution do
            syskit_write @task.gpio_extend_port, gpio_state(1)
            syskit_write @task.gpio_retract_port, gpio_state(1)
        end.to do
            emit task.exception_event
        end
    end

    it "reports reed switch ON at start" do
        syskit_configure_and_start(@task)
        output = expect_execution do
            syskit_write @task.gpio_extend_port, gpio_state(0)
            syskit_write @task.gpio_retract_port, gpio_state(0)
        end.to do
            have_one_new_sample(task.gpio_reed_switch_port)
        end
        assert_gpio_state(1, output)
    end

    it "keeps reporting the reed switch state at trigger level when there is no command "\
    "to move and timer is on the trigger range" do
        syskit_configure_and_start(@task)
        output = expect_execution do
            syskit_write @task.gpio_extend_port, gpio_state(0)
            syskit_write @task.gpio_retract_port, gpio_state(0)
        end.to do
            have_one_new_sample(task.gpio_reed_switch_port)
        end
        assert_gpio_state(1, output)

        expect_execution do
            syskit_write @task.gpio_extend_port, gpio_state(0)
            syskit_write @task.gpio_retract_port, gpio_state(0)
        end.to do
            have_no_new_sample(task.gpio_reed_switch_port, at_least_during: 1.2)
                .matching { |s| s.states[0].data == 0 }
        end
    end

    it "reports the reed switch state as true just during the trigger range for the CW "\
    "direction" do
        syskit_configure_and_start(@task)
        2.times do
            expect_execution do
                syskit_write @task.gpio_extend_port, gpio_state(1)
                syskit_write @task.gpio_retract_port, gpio_state(0)
            end.to do
                have_one_new_sample(task.gpio_reed_switch_port)
                    .matching { |s| s.states[0].data == 1 }
            end
            tic = Time.now
            expect_execution do
                syskit_write @task.gpio_extend_port, gpio_state(1)
                syskit_write @task.gpio_retract_port, gpio_state(0)
            end.to do
                have_one_new_sample(task.gpio_reed_switch_port)
                    .matching { |s| s.states[0].data == 0 }
            end
            toc = Time.now
            trigger_range_computed = toc - tic
            assert(trigger_range_computed <= 0.4)
        end
    end

    it "reports the reed switch state as false when outside the trigger range for the "\
    "CW direction" do
        syskit_configure_and_start(@task)
        2.times do
            expect_execution do
                syskit_write @task.gpio_extend_port, gpio_state(1)
                syskit_write @task.gpio_retract_port, gpio_state(0)
            end.to do
                have_one_new_sample(task.gpio_reed_switch_port)
                    .matching { |s| s.states[0].data == 0 }
            end
            tic = Time.now
            expect_execution.timeout(3.0) do
                syskit_write @task.gpio_extend_port, gpio_state(1)
                syskit_write @task.gpio_retract_port, gpio_state(0)
            end.to do
                have_one_new_sample(task.gpio_reed_switch_port)
                    .matching { |s| s.states[0].data == 1 }
            end
            toc = Time.now
            non_trigger_range_computed = toc - tic
            assert(non_trigger_range_computed <= 0.8)
        end
    end

    it "reports the reed switch state as true just during the trigger range for the CCW "\
    "direction" do
        syskit_configure_and_start(@task)
        expect_execution do
            syskit_write @task.gpio_extend_port, gpio_state(0)
            syskit_write @task.gpio_retract_port, gpio_state(1)
        end.to do
            have_one_new_sample(task.gpio_reed_switch_port)
                .matching { |s| s.states[0].data == 0 }
        end
        2.times do
            expect_execution do
                syskit_write @task.gpio_extend_port, gpio_state(0)
                syskit_write @task.gpio_retract_port, gpio_state(1)
            end.to do
                have_one_new_sample(task.gpio_reed_switch_port)
                    .matching { |s| s.states[0].data == 1 }
            end
            tic = Time.now
            expect_execution do
                syskit_write @task.gpio_extend_port, gpio_state(0)
                syskit_write @task.gpio_retract_port, gpio_state(1)
            end.to do
                have_one_new_sample(task.gpio_reed_switch_port)
                    .matching { |s| s.states[0].data == 0 }
            end
            toc = Time.now
            trigger_range_computed = toc - tic
            assert(trigger_range_computed <= 0.4)
        end
    end

    it "reports the reed switch state as false when outside the trigger range for the "\
    "CCW direction" do
        syskit_configure_and_start(@task)
        2.times do
            expect_execution do
                syskit_write @task.gpio_extend_port, gpio_state(0)
                syskit_write @task.gpio_retract_port, gpio_state(1)
            end.to do
                have_one_new_sample(task.gpio_reed_switch_port)
                    .matching { |s| s.states[0].data == 0 }
            end
            tic = Time.now
            expect_execution.timeout(3.0) do
                syskit_write @task.gpio_extend_port, gpio_state(0)
                syskit_write @task.gpio_retract_port, gpio_state(1)
            end.to do
                have_one_new_sample(task.gpio_reed_switch_port)
                    .matching { |s| s.states[0].data == 1 }
            end
            toc = Time.now
            non_trigger_range_computed = toc - tic
            assert(non_trigger_range_computed <= 0.8)
        end
    end
end
