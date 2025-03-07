from PyQt6.QtWidgets import QApplication, QMainWindow, QPushButton, QLabel, QComboBox, QVBoxLayout, QHBoxLayout, QGridLayout, QWidget
from PyQt6.QtCore import Qt, QThread, pyqtSignal, QTimer
from PyQt6.QtGui import QFont
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
import matplotlib.pyplot as plt
import paho.mqtt.client as mqtt
from collections import deque
from threading import Thread
import json
import sys

font = QFont("Cordia New", 16)
queue = deque()

class Window(QMainWindow):
    ax_data, ay_data, az_data, gx_data, gy_data, gz_data = [], [], [], [], [], []
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Demo 1")

        # Setting Central Widget
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        
        # Create Elements
        self.start_button = QPushButton("Start")
        self.stop_button = QPushButton("Stop")
        self.reset_button = QPushButton("Reset")
        self.close_button = QPushButton("Close")

        # Setting Button Function
        self.start_button.clicked.connect(self.start)
        self.stop_button.clicked.connect(self.stop)
        self.reset_button.clicked.connect(self.reset)
        self.close_button.clicked.connect(self.close)

        # Setting Font
        self.start_button.setFont(font)
        self.stop_button.setFont(font)
        self.reset_button.setFont(font)
        self.close_button.setFont(font)

        # Create Plot 
        self.fig, ((self.ax, self.ay, self.az), (self.gx, self.gy, self.gz)) = plt.subplots(2, 3, figsize=(15, 8),sharex=True, sharey="row")
        self.line1, = self.ax.plot([], [], color="blue")
        self.line2, = self.ay.plot([], [], color="green")
        self.line3, = self.az.plot([], [], color="red")
        self.line4, = self.gx.plot([], [], color="blue")
        self.line5, = self.gy.plot([], [], color="green")
        self.line6, = self.gz.plot([], [], color="red")

        # Adjust Plot
        self.ax.set_xlabel("Data")
        self.ay.set_xlabel("Data")
        self.az.set_xlabel("Data")
        self.gx.set_xlabel("Data")
        self.gy.set_xlabel("Data")
        self.gz.set_xlabel("Data")

        self.ax.set_ylabel("Accel X")
        self.ay.set_ylabel("Accel Y")
        self.az.set_ylabel("Accel Z")
        self.gx.set_ylabel("Gyro X")
        self.gy.set_ylabel("Gyro Y")
        self.gz.set_ylabel("Gyro Z")

        self.ax.set_xlim(0, 100)
        self.ay.set_xlim(0, 100)
        self.az.set_xlim(0, 100)
        self.gx.set_xlim(0, 100)
        self.gy.set_xlim(0, 100)
        self.gz.set_xlim(0, 100)

        self.ax.set_ylim(-1.5, 1.5)
        self.ay.set_ylim(-1.5, 1.5)
        self.az.set_ylim(-1.5, 1.5)
        self.gx.set_ylim(-200, 200)
        self.gy.set_ylim(-200, 200)
        self.gz.set_ylim(-200, 200)
        
        self.canvas = FigureCanvas(self.fig)

        # Create H Layout
        horizontal_layout = QHBoxLayout()
        horizontal_layout.addWidget(self.start_button)
        horizontal_layout.addWidget(self.stop_button)
        horizontal_layout.addWidget(self.reset_button)
        horizontal_layout.addWidget(self.close_button)

        # Create V Layout 
        vertical_layout = QVBoxLayout()
        vertical_layout.addLayout(horizontal_layout)
        vertical_layout.addWidget(self.canvas)

        # Setting Layout 
        central_widget.setLayout(vertical_layout)

        # Create Timer 
        self.timer = QTimer()
        self.timer.timeout.connect(self.plot)

        # Setup MQTT Connection
        self.broker = MQTT_Connect()

        print("Window is Ready")

    def start(self):
        task = Thread(target=self.broker.run)
        task.start()
        self.timer.start(100)
        
    def stop(self):
        self.broker.stop()
        self.timer.stop()

    def reset(self):
        self.ax_data.clear()
        self.ay_data.clear()
        self.az_data.clear()
        self.gx_data.clear()
        self.gy_data.clear()
        self.gz_data.clear()

    def on_signal(self, data:list):
        self.ax_data.append(data[0])
        self.ay_data.append(data[1])
        self.az_data.append(data[2])
        self.gx_data.append(data[3])
        self.gy_data.append(data[4])
        self.gz_data.append(data[5])

    def plot(self):
        if len(queue) > 0:
            data = queue.popleft()
            self.ax_data.append(data["accelX"])
            self.ay_data.append(data["accelY"])
            self.az_data.append(data["accelZ"])
            self.gx_data.append(data["gyroX"])
            self.gy_data.append(data["gyroY"])
            self.gz_data.append(data["gyroZ"])
            ax_plot = self.ax_data[-100:]
            ay_plot = self.ay_data[-100:]
            az_plot = self.az_data[-100:]
            gx_plot = self.gx_data[-100:]
            gy_plot = self.gy_data[-100:]
            gz_plot = self.gz_data[-100:]
            self.line1.set_data(range(len(ax_plot)), ax_plot)
            self.line2.set_data(range(len(ay_plot)), ay_plot)
            self.line3.set_data(range(len(az_plot)), az_plot)
            self.line4.set_data(range(len(gx_plot)), gx_plot)
            self.line5.set_data(range(len(gy_plot)), gy_plot)
            self.line6.set_data(range(len(gz_plot)), gz_plot)

            self.canvas.draw()

class MQTT_Connect():
    signal = pyqtSignal(list)
    def __init__(self):
        super().__init__()
        # Create Variables
        self.client = mqtt.Client(callback_api_version=mqtt.CallbackAPIVersion.VERSION2)
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message
        mqtt_server = "broker.hivemq.com"
        mqtt_port = 1883
        self.topic = "ton/server/#"
        self.client.connect(mqtt_server, mqtt_port)

    def on_connect(self, client, userdata, connect_flags, reason_code, properties):
        print("Connected", reason_code)
        self.client.subscribe(self.topic)

    def on_message(self, client, userdata, message):
        data = message.payload.decode()
        data = json.loads(data)
        queue.append(data)

    def run(self):
        print("Start")
        self.client.loop_start()

    def stop(self):
        print("Stop")
        self.client.loop_stop()

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = Window()
    window.show()
    sys.exit(app.exec())