#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int32.h>
#include <rmw_microros/rmw_microros.h>
#include "pico/stdlib.h"
#include "pico_uart_transports.h"

const uint LED_PIN = 25; 

rcl_publisher_t publisher;
rcl_subscription_t subscriber;
std_msgs__msg__Int32 send_msg;
std_msgs__msg__Int32 recv_msg;

// Callback: Khi PC gửi lệnh xuống
void sub_callback(const void * msgin) {
    const std_msgs__msg__Int32 * msg = (const std_msgs__msg__Int32 *)msgin;
    // data = 1 -> Bật đèn, ngược lại -> Tắt
    gpio_put(LED_PIN, (msg->data == 1) ? 1 : 0);
}

// Callback: Gửi dữ liệu lên PC (10Hz)
void timer_callback(rcl_timer_t * timer, int64_t last_call_time) {
    if (timer != NULL) {
        rcl_publish(&publisher, &send_msg, NULL);
        send_msg.data++;
    }
}

int main() {
    // Cấu hình Transport sử dụng hàm Custom Hardware UART
    rmw_uros_set_custom_transport(
        true,
        NULL,
        pico_serial_transport_open,
        pico_serial_transport_close,
        pico_serial_transport_write,
        pico_serial_transport_read
    );

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    rcl_timer_t timer;
    rcl_node_t node;
    rcl_allocator_t allocator;
    rclc_support_t support;
    rclc_executor_t executor;

    allocator = rcl_get_default_allocator();

    // Ping Agent: Chờ kết nối với máy tính
    while (rmw_uros_ping_agent(100, 1) != RCL_RET_OK) {
        gpio_put(LED_PIN, !gpio_get(LED_PIN)); // Nháy đèn chờ
        sleep_ms(100);
    }
    gpio_put(LED_PIN, 0); // Kết nối xong -> Tắt đèn

    // Khởi tạo Node, Publisher, Subscriber
    rclc_support_init(&support, 0, NULL, &allocator);
    rclc_node_init_default(&node, "pico_node", "", &support);

    rclc_publisher_init_default(&publisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32), "pico_data");
    rclc_subscription_init_default(&subscriber, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32), "led_cmd");
    
    rclc_timer_init_default(&timer, &support, RCL_MS_TO_NS(100), timer_callback);

    rclc_executor_init(&executor, &support.context, 2, &allocator);
    rclc_executor_add_timer(&executor, &timer);
    rclc_executor_add_subscription(&executor, &subscriber, &recv_msg, &sub_callback, ON_NEW_DATA);

    send_msg.data = 0;
    while (true) {
        rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
    }
    return 0;
}