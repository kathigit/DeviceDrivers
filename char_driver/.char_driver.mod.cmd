cmd_/home/pi/DeviceDrivers/char_driver/char_driver.mod := printf '%s\n'   char_driver.o | awk '!x[$$0]++ { print("/home/pi/DeviceDrivers/char_driver/"$$0) }' > /home/pi/DeviceDrivers/char_driver/char_driver.mod
