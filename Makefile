CATKIN_WS_PATH=$(PWD)/catkin_ws
BUILD_PATH=$(PWD)/build
ROS_INSTALL_PREFIX=/opt/ros/melodic
FIRMWARE_TARGET=px4_fmu-v3_default
FIRMWARE_PATH=$(CATKIN_WS_PATH)/src/Firmware

VTOL_BASE_PACKAGES=\
	offb

SITL_PACKAGES=\
	px4
	#mavlink_sitl_gazebo

default: sim

#deploy: firmware
#	catkin build --workspace $(CATKIN_WS_PATH) $(VTOL_BASE_PACKAGES)

sim:
	catkin build --workspace $(CATKIN_WS_PATH) $(VTOL_BASE_PACKAGES) $(SITL_PACKAGES)

#firmware:
#	make -C $(FIRMWARE_PATH) $(FIRMWARE_TARGET)

install:
	mkdir -p $(DESTDIR)/$(ROS_INSTALL_PREFIX)
	DESTDIR=$(DESTDIR)/$(ROS_INSTALL_PREFIX) make -C $(CATKIN_WS_PATH)/build install

.PHONY: deploy
.PHONY: sim
.PHONY: sitl_gazebo
.PHONY: install
