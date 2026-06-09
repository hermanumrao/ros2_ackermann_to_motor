from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():

    ackermann_bridge = Node(
        package="ackermann_to_motor",
        executable="ackermann_to_motor",
        name="ackermann_to_motor",
        output="screen",
    )

    arduino_motor = Node(
        package="arduino_motor",
        executable="motor_node",
        name="arduino_motor",
        output="screen",
    )

    return LaunchDescription([ackermann_bridge, arduino_motor])
