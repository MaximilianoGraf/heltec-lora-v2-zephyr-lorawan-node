# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/max/zephyrproject/modules/hal/espressif/components/partition_table"
  "/home/max/zephyrproject/zephyr/samples/heltec-lorawan-node/heltec-lora-v2-zephyr-lorawan-node/build/esp-idf/build"
  "/home/max/zephyrproject/zephyr/samples/heltec-lorawan-node/heltec-lora-v2-zephyr-lorawan-node/build/zephyr/soc/xtensa/esp32/EspPartitionTable-prefix"
  "/home/max/zephyrproject/zephyr/samples/heltec-lorawan-node/heltec-lora-v2-zephyr-lorawan-node/build/zephyr/soc/xtensa/esp32/EspPartitionTable-prefix/tmp"
  "/home/max/zephyrproject/zephyr/samples/heltec-lorawan-node/heltec-lora-v2-zephyr-lorawan-node/build/zephyr/soc/xtensa/esp32/EspPartitionTable-prefix/src/EspPartitionTable-stamp"
  "/home/max/zephyrproject/zephyr/samples/heltec-lorawan-node/heltec-lora-v2-zephyr-lorawan-node/build/zephyr/soc/xtensa/esp32/EspPartitionTable-prefix/src"
  "/home/max/zephyrproject/zephyr/samples/heltec-lorawan-node/heltec-lora-v2-zephyr-lorawan-node/build/zephyr/soc/xtensa/esp32/EspPartitionTable-prefix/src/EspPartitionTable-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/max/zephyrproject/zephyr/samples/heltec-lorawan-node/heltec-lora-v2-zephyr-lorawan-node/build/zephyr/soc/xtensa/esp32/EspPartitionTable-prefix/src/EspPartitionTable-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/max/zephyrproject/zephyr/samples/heltec-lorawan-node/heltec-lora-v2-zephyr-lorawan-node/build/zephyr/soc/xtensa/esp32/EspPartitionTable-prefix/src/EspPartitionTable-stamp${cfgdir}") # cfgdir has leading slash
endif()
