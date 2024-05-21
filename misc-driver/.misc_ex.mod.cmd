cmd_/home/pi/DeviceDrivers/misc-driver/misc_ex.mod := printf '%s\n'   misc_ex.o | awk '!x[$$0]++ { print("/home/pi/DeviceDrivers/misc-driver/"$$0) }' > /home/pi/DeviceDrivers/misc-driver/misc_ex.mod
