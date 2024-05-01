cmd_/home/pi/DeviceDrivers/waitq/waitq_ex.mod := printf '%s\n'   waitq_ex.o | awk '!x[$$0]++ { print("/home/pi/DeviceDrivers/waitq/"$$0) }' > /home/pi/DeviceDrivers/waitq/waitq_ex.mod
