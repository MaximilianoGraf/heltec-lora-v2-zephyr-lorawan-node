# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/max/zephyrproject/modules/hal/espressif/components/bootloader/subproject"
  "/home/max/zephyrproject/zephyr/samples/heltec-lorawan-node/heltec-lora-v2-zephyr-lorawan-node/build/esp-idf/build/bootloader"
  "/home/max/zephyrproject/zephyr/samples/heltec-lorawan-node/heltec-lora-v2-zephyr-lorawan-node/build/esp-idf"
  "/home/max/zephyrproject/zephyr/samples/heltec-lorawan-node/heltec-lora-v2-zephyr-lorawan-node/build/esp-idf/tmp"
  "/home/max/zephyrproject/zephyr/samples/heltec-lorawan-node/heltec-lora-v2-zephyr-lorawan-node/build/esp-idf/src/EspIdfBootloader-stamp"
  "/home/max/zephyrproject/zephyr/samples/heltec-lorawan-node/heltec-lora-v2-zephyr-lorawan-node/build/esp-idf/src"
  "/home/max/zephyrproject/zephyr/samples/heltec-lorawan-node/heltec-lora-v2-zephyr-lorawan-node/build/esp-idf/src/EspIdfBootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/max/zephyrproject/zephyr/samples/heltec-lorawan-node/heltec-lora-v2-zephyr-lorawan-node/build/esp-idf/src/EspIdfBootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/max/zephyrproject/zephyr/samples/heltec-lorawan-node/heltec-lora-v2-zephyr-lorawan-node/build/esp-idf/src/EspIdfBootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
