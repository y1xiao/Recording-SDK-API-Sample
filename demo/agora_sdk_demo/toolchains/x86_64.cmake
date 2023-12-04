# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)
 
set(DIR /usr/bin/)
set(CROSS "")
 
set (CMAKE_C_COMPILER ${DIR}${CROSS}gcc CACHE STRING "Set the cross-compiler tool CC" FORCE)
set (CMAKE_CXX_COMPILER ${DIR}${CROSS}g++ CACHE STRING "Set the cross-compiler tool CXX" FORCE)
set (CMAKE_LINKER ${DIR}${CROSS}ld CACHE STRING "Set the cross-compiler tool LD" FORCE)
set (CMAKE_AR ${DIR}${CROSS}ar CACHE STRING "Set the cross-compiler tool AR" FORCE)
set (CMAKE_NM ${DIR}${CROSS}nm CACHE STRING "Set the cross-compiler tool NM" FORCE)
set (CMAKE_OBJCOPY ${DIR}${CROSS}objcopy CACHE STRING "Set the cross-compiler tool OBJCOPY" FORCE)
set (CMAKE_OBJDUMP ${DIR}${CROSS}objdump CACHE STRING "Set the cross-compiler tool OBJDUMP" FORCE)
set (CMAKE_RANLIB ${DIR}${CROSS}ranlib CACHE STRING "Set the cross-compiler tool RANLIB" FORCE)
set (CMAKE_STRIP ${DIR}${CROSS}strip CACHE STRING "Set the cross-compiler tool RANLIB" FORCE)
 
# Target environment
#set (CMAKE_FIND_ROOT_PATH ${ELDK_DIR}/arm-linux-gnueabihf)
 
# Don't search for programs in the host environment
#set (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
 
# Search for the libraries and headers in the target environment
#set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
