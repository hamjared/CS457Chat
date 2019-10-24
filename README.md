# CS457Chat

## Build Instructions:

Change Directory to CS457 and then:
```
    make all
```

## Running Instructions:
Run the below commands from the project root directory. 

To start the server run the command below.
 
In order for the correct IP address of the server to print, the device must have its IP defined in /etc/hosts
```
$ ./chat 
```

To start the client:
```
    $ ./chat -p [Port Number] -s [IPv4 Address]
```

Maximum message length is 140 characters. 

## Thanks to
I referenced the following websites while developing this code:
  - https://www.geeksforgeeks.org/socket-programming-cc/
  - https://beej.us/guide/bgnet/
  - http://man7.org/linux/man-pages/man2/socket.2.html
  - https://linux.die.net/man/2/setsockopt