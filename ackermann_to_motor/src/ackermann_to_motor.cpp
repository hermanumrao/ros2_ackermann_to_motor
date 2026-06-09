#include <ackermann_msgs/msg/ackermann_drive_stamped.hpp>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

#include <cmath>
#include <string>

class AckermannToMotor : public rclcpp::Node {
public:
  AckermannToMotor() : Node("ackermann_to_motor") {
    sub_ =
        this->create_subscription<ackermann_msgs::msg::AckermannDriveStamped>(
            "/drive_command", 10,
            std::bind(&AckermannToMotor::driveCallback, this,
                      std::placeholders::_1));

    pub_ = this->create_publisher<std_msgs::msg::String>("/motor_command", 10);

    RCLCPP_INFO(this->get_logger(), "Ackermann -> Motor bridge started");
  }

private:
  void publishString(const std::string &text) {
    std_msgs::msg::String msg;
    msg.data = text;

    pub_->publish(msg);

    RCLCPP_INFO(this->get_logger(), "TX -> %s", text.c_str());
  }

  void driveCallback(
      const ackermann_msgs::msg::AckermannDriveStamped::SharedPtr msg) {
    double speed = msg->drive.speed;
    double steering = msg->drive.steering_angle;

    /*
     * Steering conversion
     */

    int servo2 = static_cast<int>(90 + (steering * 57.2958 * 1.2));

    /*
     * Clamp steering
     */

    if (servo2 > 170)
      servo2 = 170;

    if (servo2 < 10)
      servo2 = 10;

    /*
     * Speed conversion
     */

    int motor_speed = static_cast<int>(std::abs(speed) * 100);

    if (motor_speed > 255)
      motor_speed = 255;

    /*
     * Motor command
     */

    if (motor_speed == 0) {
      publishString("STOP");
    } else if (speed >= 0) {
      publishString("F" + std::to_string(motor_speed));
    } else {
      publishString("B" + std::to_string(motor_speed));
    }

    /*
     * Servo1 fixed
     */

    publishString("SERVO190");

    /*
     * Servo2 steering
     */

    publishString("SERVO2" + std::to_string(servo2));

    /*
     * Debug
     */

    RCLCPP_INFO(this->get_logger(), "speed=%.2f steering=%.2f servo=%d", speed,
                steering, servo2);
  }

  rclcpp::Subscription<ackermann_msgs::msg::AckermannDriveStamped>::SharedPtr
      sub_;

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  rclcpp::spin(std::make_shared<AckermannToMotor>());

  rclcpp::shutdown();

  return 0;
}
