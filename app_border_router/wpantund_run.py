mport subprocess
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
    #errno = process.wait()
    out, err = process.communicate()
    errno = process.returncode
    print out
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
