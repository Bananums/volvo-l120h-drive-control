# volvo-l120h-drive-control


### Uart validation

#Validate the need for the first line. Should now have to register the group to user.
```shell
sudo usermod -aG uucp $USER 
sudo chmod 666 /dev/ttyUSB1
stty -F /dev/ttyUSB1 115200 raw -echo
echo -n "YourData" > /dev/ttyUSB1   
```

