## OpenThread border router on Hamilton/RIOT

This application provides an OpenThread border router with watchdog. [OpenThread](https://github.com/openthread/openthread) is an open source implementation of [Thread](https://threadgroup.org/). [RIOT](https://github.com/RIOT-OS/RIOT) is an open source IoT operating system.

You can test various operation of Openthread border router by using [WPANCTL commands](https://github.com/openthread/wpantund/wiki/OpenThread-Simulator-Tutorial). This application is optimized for a [hamilton border router](https://hamiltoniot.com/collections/frontpage/products/hamilton-hg1-gateway).

## Quick usage with a given hamilton border router

This application is already programmed in a hamilton border router (SAMR21+Raspberry Pi). You just need to (1) plug in the provided USB and (2) power on the border router. 

## Manual setup 1: NCP, hamilton board side

If you have just a hardware and need to install the programm, you can do the following:
1. Download RIOT, OpenThread, and Wpantund source codes, run
```
../Hamilton-RIOT-Openthread-fw$ git submodule update --init --remote --recursive
```

2. Turn on JTAG for programming, run
```
../Hamilton-RIOT-Openthread-fw$ JLinkExe
J-Link> power on
```

3. Compile and flash NCP (default serial baudrate is 1 Mbps, default wireless data rate is 2 Mbps), run
```
../Hamilton-RIOT-Openthread-fw/app_border_router$ make flash
```

4. Reboot the Raspberry Pi, unplug power and plug again

5. Expected results: Hamilton board's watchdog starts working; the Hamilton board restarts if it does not receive heartbeat messages from the Raspberry Pi through serial for a while. The Hamilton board's red LED must be blinking every second, which verifies that its MCU is working.

## Manual setup 2: Wpantund, Raspberry Pi side

1. Install wpantund: On Ubuntu 16.04, run
```
sudo apt-get -y install build-essential subversion libncurses5-dev libssl-dev zlib1g-dev gawk gcc-multilib flex git-core gettext gcc binutils bzip2 python perl make unzip libz-dev tftp git shtool autogen automake libtool autotools-dev libdbus-1-dev
cd wpantund
git checkout baudrate-fix
./bootstrap.sh
./configure
make
sudo make install
```

2. Make a python script "wpantund_run.py" as follows:
   (default setting: 802.15.4 channel 26, serial baudrate 1 Mbps)
```
#!/usr/bin/env python

import subprocess
import sys
import signal
import sys
import time

def cleanup():
    command = "pkill wpantund"
    process = subprocess.Popen(command.split(), stdout=subprocess.PIPE)
    process.wait()

def check():
    command = "wpanctl get Thread:Leader:Address"
    process = subprocess.Popen(command.split(), stdout=subprocess.PIPE)
    errno = process.wait()
    if errno != 0:
        return False

    command = "wpanctl get NCP:State"
    process = subprocess.Popen(command.split(), stdout=subprocess.PIPE)
    out, err = process.communicate()
    errno = process.returncode
    if errno != 0:
        return False

    if 'NCP:State = "associated"' in out:
        return True
    else:
        return False

def run():
    command = "sudo wpantund -o Config:NCP:SocketPath /dev/ttyAMA0 -o Config:NCP:SocketBaud 1000000"
    process = subprocess.Popen(command.split(), stderr=subprocess.PIPE)
    while True:
        line = process.stderr.readline()
        if "Finished initializing NCP" in line:
            print "finished starting wpantund"
            break
        elif "Resetting and trying again..." in line:
            print "failed starting wpantund"
            break

    command = "wpanctl set Network:XPANID DEAD00BEEF00CAFE"
    process = subprocess.Popen(command.split())
    process.wait()

    command = "wpanctl set Network:Key 00112233445566778899AABBCCDDEEFF"
    process = subprocess.Popen(command.split())
    process.wait()

    command = "wpanctl config-gateway -d fd11:22::"
    process = subprocess.Popen(command.split())
    process.wait()

    command = "wpanctl form -T router -c 26 OpenThreadDemo"
    process = subprocess.Popen(command.split())
    process.wait()


def signal_handler(signal, frame):
    print "You pressed Ctrl+C! Cleaning up"
    cleanup()
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

if __name__ == "__main__":
    while True:
        cleanup()
        run()
        while check():
            print time.time(), "health check passed"
            time.sleep(1)
        print "health check failed, waiting for watchdog"
        time.sleep(10) # sleep long enough to trigger watchdog
```

3. Execute the python script, run 
```
sudo python wpantund_run.py
```

4. Expected results: Hamilton board's white LED must be blinking every second and the python script must output "health check passed" every second. These verify that the hamilton board is receiving heartbeat messages from the Raspberry Pi. The border router should automatically scan and join a Thread network around. When it joins, it starts as an end device and becomes a router later. It can be the leader of the network if it is the first node turned on. 
