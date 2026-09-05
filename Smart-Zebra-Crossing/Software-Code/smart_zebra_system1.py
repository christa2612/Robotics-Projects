import socket
import cv2
import time
from ultralytics import YOLO

# 🔌 SOCKET SETUP
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(("172.20.10.4", 5000))

# Load YOLO Model
model = YOLO("yolov8n.pt")

# Open Camera
cap = cv2.VideoCapture(0)

# ROI (Zebra Crossing Area)
roi_y1 = 250

# 🔥 CONTROL MESSAGE SPEED
last_send_time = 0
send_interval = 1  # seconds

print("🚀 Smart Zebra System Started (Headless Mode)")

while True:
    ret, frame = cap.read()
    if not ret:
        print("❌ Camera not working")
        break

    frame = cv2.resize(frame, (640, 480))

    # Run YOLO
    results = model(frame, verbose=False)

    vehicles = 0
    pedestrian_count = 0
    emergency_detected = False

    for box in results[0].boxes:
        cls = int(box.cls[0])
        label = model.names[cls]
        conf = float(box.conf[0])

        if conf > 0.3:   # lower threshold for better detection

            # 🚗 Count vehicles
            if label in ["car", "truck", "bus", "motorbike"]:
                vehicles += 1

            # 🚑 Emergency detection (you can refine later)
            if label in ["truck", "bus"]:
                emergency_detected = True

            # 🚶 Count pedestrians in ROI
            if label == "person" and box.xyxy[0][3] > roi_y1:
                pedestrian_count += 1

    # 🚦 DECISION LOGIC
    if emergency_detected:
        decision = "EMERGENCY"
        signal_text = "🚑 EMERGENCY → VEHICLE GREEN"

    else:
        if pedestrian_count > vehicles:
            decision = "PERSON"
            signal_text = f"🚶 PEDESTRIAN PRIORITY ({pedestrian_count}>{vehicles})"
        else:
            decision = "VEHICLE"
            signal_text = f"🚗 VEHICLE PRIORITY ({vehicles}>={pedestrian_count})"

    # 🔁 Send data every 1 sec
    if time.time() - last_send_time > send_interval:
        client.sendall((decision + "\n").encode())
        print(f"SENT: {decision} | 🚶 {pedestrian_count} | 🚗 {vehicles}")
        last_send_time = time.time()

# Cleanup
cap.release()
client.close()
