# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/Espressif/esp-idf/components/bootloader/subproject"
  "D:/ESE/AyP/medidor_volumen/build/bootloader"
  "D:/ESE/AyP/medidor_volumen/build/bootloader-prefix"
  "D:/ESE/AyP/medidor_volumen/build/bootloader-prefix/tmp"
  "D:/ESE/AyP/medidor_volumen/build/bootloader-prefix/src/bootloader-stamp"
  "D:/ESE/AyP/medidor_volumen/build/bootloader-prefix/src"
  "D:/ESE/AyP/medidor_volumen/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/ESE/AyP/medidor_volumen/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/ESE/AyP/medidor_volumen/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
