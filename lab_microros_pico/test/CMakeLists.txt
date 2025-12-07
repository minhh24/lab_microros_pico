cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_BOARD pico CACHE STRING "Board type")

# --- Sửa đường dẫn này theo máy của sinh viên ---
set(PICO_SDK_PATH "/home/pc12/micro_ros_ws/src/pico-sdk")
set(MICROROS_PATH "/home/pc12/micro_ros_ws/src/micro_ros_raspberrypi_pico_sdk/libmicroros")

include(pico_sdk_import.cmake)
project(test C CXX ASM)
pico_sdk_init()

# Cấu hình Micro-ROS
include_directories(${MICROROS_PATH}/include)
link_directories(${MICROROS_PATH})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Thêm file mã nguồn
add_executable(test test.c pico_uart_transport.c)

# --- QUAN TRỌNG: Cấu hình ngõ ra ---
pico_enable_stdio_usb(test 0)   # Tắt USB (để không xung đột)
pico_enable_stdio_uart(test 1)  # Bật UART (để dùng GP0/GP1)

# Link thư viện
target_link_libraries(test
        pico_stdlib
        microros
        hardware_pio
        hardware_clocks
        hardware_uart   # Thư viện điều khiển UART phần cứng
        )

pico_add_extra_outputs(test)