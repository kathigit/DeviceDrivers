cmd_/home/pi/DeviceDrivers/gpio/char_gpio.mod := printf '%s\n'   char_gpio.o | awk '!x[$$0]++ { print("/home/pi/DeviceDrivers/gpio/"$$0) }' > /home/pi/DeviceDrivers/gpio/char_gpio.mod
