################################################################################
#Yume Engine MIT License (MIT)

# Copyright (c) 2015 arkenthera
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# File : CMakeLists.txt
# Date : 8.27.2015
# Comments :
################################################################################

configure_file(${YUME_TEMPLATES_DIR}/CMakeBuildSettings.h.in ${CMAKE_SOURCE_DIR}/Engine/Source/Runtime/Core/YumeBuildSettings.h @ONLY)
install(FILES ${CMAKE_SOURCE_DIR}/Engine/Source/Runtime/Core/YumeBuildSettings.h DESTINATION ${CMAKE_SOURCE_DIR}/Source/Engine/Runtime/Core)
