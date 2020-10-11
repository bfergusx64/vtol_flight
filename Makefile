CATKIN_WS_PATH=$(PWD)/catkin_ws
BUILD_PATH=$(PWD)/build
ROS_INSTALL_PREFIX=/opt/ros/melodic
FIRMWARE_TARGET=px4_fmu-v3_default
FIRMWARE_PATH=$(CATKIN_WS_PATH)/src/px4

VTOL_BASE_PACKAGES=\
	offb

SITL_PACKAGES=\
	px4
	#mavlink_sitl_gazebo

default: sim

#deploy: firmware
#	catkin build --workspace $(CATKIN_WS_PATH) $(VTOL_BASE_PACKAGES)

sim: sitl_gazebo
	catkin build --workspace $(CATKIN_WS_PATH) $(VTOL_BASE_PACKAGES) $(SITL_PACKAGES)

sitl_gazebo:
	mkdir -p ${FIRMWARE_PATH}/Tools/sitl_gazebo/build
	cd ${FIRMWARE_PATH}/Tools/sitl_gazebo/build; cmake ..; make

#firmware:
#	make -C $(FIRMWARE_PATH) $(FIRMWARE_TARGET)

#install:
#	mkdir -p $(DESTDIR)/$(ROS_INSTALL_PREFIX)
#	DESTDIR=$(DESTDIR)/$(ROS_INSTALL_PREFIX) make -C $(CATKIN_WS_PATH)/build install

.PHONY: deploy
.PHONY: sim
.PHONY: sitl_gazebo
.PHONY: install
