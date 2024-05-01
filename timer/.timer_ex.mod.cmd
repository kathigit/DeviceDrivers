cmd_/home/pi/DeviceDrivers/timer/timer_ex.mod := printf '%s\n'   timer_ex.o | awk '!x[$$0]++ { print("/home/pi/DeviceDrivers/timer/"$$0) }' > /home/pi/DeviceDrivers/timer/timer_ex.mod
