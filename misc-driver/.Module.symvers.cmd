cmd_/home/pi/DeviceDrivers/misc-driver/Module.symvers :=  sed 's/ko$$/o/'  /home/pi/DeviceDrivers/misc-driver/modules.order | scripts/mod/modpost -m -a    -o /home/pi/DeviceDrivers/misc-driver/Module.symvers -e -i Module.symvers -T - 
