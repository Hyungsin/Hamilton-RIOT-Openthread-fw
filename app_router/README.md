## OpenThread router on Hamilton/RIOT

This application provides an OpenThread router. [OpenThread](https://github.com/openthread/openthread) is an open source implementation of [Thread](https://threadgroup.org/). [RIOT](https://github.com/RIOT-OS/RIOT) is an open source IoT operating system.

You can test various operation of Openthread router by using [CLI commands](https://github.com/openthread/wpantund/wiki/OpenThread-Simulator-Tutorial). This application is optimized for a [hamilton border router](https://hamiltoniot.com/collections/frontpage/products/hamilton-hg1-gateway).

## Manual setup with a given hamilton border router

1. Download RIOT, OpenThread, and Wpantund source codes, run
```
../Hamilton-RIOT-Openthread-fw$ git submodule update --init --remote --recursive
```

2. Turn on JTAG for programming, run
```
../Hamilton-RIOT-Openthread-fw$ JLinkExe
J-Link> power on
```

3. Compile and flash Openthread router (default serial baudrate is 115.2 kbps, default wireless data rate is 2 Mbps), run
```
../Hamilton-RIOT-Openthread-fw/app_router$ make flash
```

4. Turn on CLI window, run
```
../Hamilton-RIOT-Openthread-fw/app_router$ make BOARD=samr21-xpro PROGRAMMER=edbg PORT_LINUX=/dev/ttyAMA0 term
```

5. Expected results: The router should automatically scan and join a Thread network around. When it joins, it starts as an end device and becomes a router later. It can be the leader of the network if it is the first node turned on. Put various [CLI commands](https://github.com/openthread/wpantund/wiki/OpenThread-Simulator-Tutorial) to test Openthread router operation
