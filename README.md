# ros2_ackermann_to_motor
This packa is to be used in combination with ros2_ackermann_motor_ctrl package. This converts the `ackermann_msgs::msg::AckermannDriveStamped` msgs to std_msgs/msg/String.
This package acts as a bridge between high-level autonomous navigation commands and low-level hardware control for Ackermann steering platforms. It is specifically designed to be used in combination with the **ros2_ackermann_motor_ctrl** package.

## Overview

The primary function of this package is to subscribe to standard ROS 2 Ackermann drive messages and convert them into the specific string format required by the custom `ros2_ackermann_motor_ctrl` node. 

- **Input:** `ackermann_msgs::msg::AckermannDriveStamped`
- **Output:** `std_msgs/msg/String` (formatted for serial transmission to Arduino)

## How it Works

This node performs the translation from physical units (like speed and steering angle) into the command protocol defined for the hardware controller. 

The translated string messages are published to the `/motor_command` topic, which the `ros2_ackermann_motor_ctrl` node listens to before sending them over USB serial to an Arduino Mega.

### Command Translation Mapping

The conversion follows the protocol required by the motor control package:

| Ackermann Input | Translated String Format | Description |
| :--- | :--- | :--- |
| **Speed (> 0)** | `F<speed>` | Forward movement (Speed: 0–255) |
| **Speed (< 0)** | `B<speed>` | Reverse movement (Speed: 0–255) |
| **Speed (= 0)** | `STOP` | Stops the DC motor |
| **Steering Angle** | `SERVO1<angle>` | Sets servo angles (0–180 degrees) |

## Installation

### Prerequisites

*   Ubuntu 22.04 or later.
*   ROS 2 Humble (or newer).
*   `ackermann_msgs` package installed.
*   The [ros2_ackermann_motor_ctrl](https://github.com/hermanumrao/ros2_ackermann_motor_ctrl) package.

### Building the Package

1.  Navigate to your ROS 2 workspace `src` folder:
    ```bash
    cd ~/ros2_ws/src
    ```
2.  Clone this repository:
    ```bash
    git clone https://github.com/hermanumrao/ros2_ackermann_to_motor.git
    ```
3.  Build the workspace:
    ```bash
    cd ~/ros2_ws
    colcon build --packages-select ackermann_to_motor
    ```
4.  Source the workspace:
    ```bash
    source install/setup.bash
    ```

## Usage

To start the conversion node, run:

```bash
ros2 run ackermann_to_motor ackermann_to_motor_node
```

Ensure that the **ros2_ackermann_motor_ctrl** node is also running to receive these converted strings and relay them to your hardware.

## ROS 2 Interface

*   **Subscribers:** 
    *   Topic: `/ackermann_cmd` (or configured input topic)
    *   Type: `ackermann_msgs::msg::AckermannDriveStamped`
*   **Publishers:**
    *   Topic: `/motor_command`
    *   Type: `std_msgs/msg/String`
