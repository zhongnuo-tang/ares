# This script runs a Flask web server and a serial writer in parallel processes.
# It is meant to be run on a PC with UART and in the same network as the device running ares.

from multiprocessing import Process
import time
import random
from flask import Flask, jsonify
from serial import Serial
from time import sleep
import argparse
import sys

# -------- Flask App --------
app = Flask(__name__)

@app.route("/now/", methods=["GET"])
def get_time():
    current_time = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
    return jsonify({"time": current_time})

def run_flask():
    # Disable reloader for multiprocessing compatibility
    app.run(host="0.0.0.0", port=5000, debug=True, use_reloader=False)

# -------- Serial Writer --------
def run_serial(port, baudrate):
    try:
        ser = Serial(port, baudrate)
        print(f"Opened serial port {port} with baudrate {baudrate}")
        while True:
            sleep(random.uniform(0.5, 2.0))
            ser.write(b'1234567890abcdefghijklmnopqrstuvwxyz\0')
    except Exception as e:
        print(f"Serial error: {e}", file=sys.stderr)

# -------- Main --------
if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--port', type=str, required=True, help='Serial port')
    parser.add_argument('--baudrate', type=int, default=115200, help='Baud rate')
    args = parser.parse_args()

    # Create separate processes
    flask_process = Process(target=run_flask)
    serial_process = Process(target=run_serial, args=(args.port, args.baudrate))

    # Start processes
    flask_process.start()
    serial_process.start()

    # Join processes (wait for them)
    flask_process.join()
    serial_process.join()
