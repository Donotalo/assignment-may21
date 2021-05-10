# assignment-may21
## Disclaimer
I've decades of exerience in C & C++, have 10 months worth of experience in Node.js. My experience in Node is writing a server using ExpressJS. This assignment is different. For this assignment I have to learn how to write a __CLI application in Node__, how to write __C++ addon for Node__ and how __Node-API works__ at the basic level. So I went through a lot of tutorials and how to guides. Finally I was able to make everything working, by modifying a hello world project for Node-API, but due to lack of experience in this area and due to the time limitation I feel like the submission isn't professional.

I've spent 5-6 hours for going through tutorial and ~5 hours for coding. The assignment is done on Windows 10 in Node.js v16.1.0.

## Implementation details
The root folder contains `main.js` file which contains the entry point of the program. It can be executed by the following command:

    node .
    
The main program accepts the following command line parameters:

    hc [<get-speed> | (<set-speed>=<number>)]

Where,

    hc = command to invoke Hei Connect interface through which communication with Hei Connect device can be established
    get-speed = to retrieve current stirrer speed in RPM
    set-speed = to set desired stirrer speed in RPM, passed in parameter
    
The file `hei_connect_interface.js` implements a class `HeiConnectInterface` which is responsible to communicate with C++ addon that directly communicates with the hardware.

The `hei_connect` folder contains the C++ addon to communicate with Hei Connect. The main code and entry point is at `./src/hei_connect.cc`. It has a `HeiConnectDevice` class that directly communicates with the hardware. It has a constructor to default initialize member variables, `GetStirrerSpeed()` to retrieve the stirrer speed and `SetStirrerSpeed()` to set the stirrer speed.

The entry point of the C++ addon is the `Init()` in `./src/hei_connect.cc` file. It calls the `Method()` function. This function contains a static variable of type `HeiConnectDevice`. All communication with the Hei Connect device occur via this variable. `Method()` function checks the parameters come from Node environment (get-speed/set-speed) and calls appropriate C++ implementation (HeiConnectDevice::GetStirrerSpeed/HeiConnectDevice::SetStirrerSpeed).

The C++ addon is built with `node-gyp`.

## Sample run
    PS E:\Documents\Programs\labforward> node . hc --help
     hc [<get-speed> <set-speed>]

    Control the Hei Connect

    Positionals:
      set-speed  Sets the rotation speed in RPM                             [number]
      get-speed  Gets currently set rotation speed in RPM

    Options:
      --help     Show help                                                 [boolean]
      --version  Show version number                                       [boolean]
    PS E:\Documents\Programs\labforward> node . hc --get-speed
    Retrieving speed...
    0
    Success
    PS E:\Documents\Programs\labforward> node . hc --set-speed=100
    Setting speed @ 100...
    Success
    PS E:\Documents\Programs\labforward> node . hc --get-speed
    Retrieving speed...
    0
    Success

## Build instruction
Assuming `Node v16.x` and `npm` are installed, following commands can be executed in powershell from the root folder of the project to build the project:

    npm install
    cd .\hei_connect\
    npm install
    node-gyp build
    cd..
    npm install .\hei_connect\

## Rooms of improvement
A notable problem with current implementation is that each time `node .` is invoked it starts the process and when the command execution is completed, it exits. I couldn't manage time to run it indefinitely. For this reason the RPM set is lost. Each time the process starts, the default RPM is loaded into memory which is 0.

Here I list everything I could do better with more time:

1. Running the app indefinitely and be able to take input from command line and process it.
2. Asynchronous communication with the devices.
3. Have a command to query all connected devices to get which are online.
4. Having an abstract class as a base class of `HeiConnectInterface`. Function prototypes of some common functionalities could be defined in the abstract class (e.g., `GetApi()`).
5. Having an abstract class as a base class of `HeiConnectDevice`. There could be prototypes of common functionalities, like there could be a `Setup()` function to setup the communication with the device (e.g., opening port, setting baud rate etc.).
6. Writing unit test for HeiConnectInterface class.
7. Writing unit test for HeiConnectDevice class.
