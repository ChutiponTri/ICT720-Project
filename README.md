# ICT720-project-2025
Repo for demo idea, and code for ICT720 course 2025. 

# Group Onii-Chan
## Development of Automation System for Wheelchair User 
<p>&nbsp;&nbsp;&nbsp;&nbsp;People with mobility impairments, especially wheelchair users, often face challenges in controlling home appliances and monitoring their daily activity. This project aims to create a smart environment that enhances accessibility, convenience, and safety using M5Capsule, T-SIM camera, Raspberry Pi, and Tapo smart plug.</p>

<p>&nbsp;&nbsp;&nbsp;&nbsp;The system works by tracking the wheelchair user’s motion and detecting their proximity to automate appliance control and provide activity monitoring. When the user moves, the M5Capsule transmits motion data via Wi-Fi to a server, allowing caregivers to monitor their daily movement patterns. Additionally, a T-SIM camera detects the wheelchair’s Bluetooth signal and sends commands to a Raspberry Pi, which controls a Tapo smart plug to turn on or off appliances like lights, fans, or TVs.</p>

## Objectives
<p>&nbsp;&nbsp;&nbsp;&nbsp;The primary goal of this project is to develop a smart motion tracking and environment control system for wheelchair users, leveraging M5Capsule, T-SIM camera, Raspberry Pi, and Tapo smart plug. The system aims to enhance accessibility, safety, and energy efficiency. The specific objectives are:</p>

1. To track wheelchair user motion in real time
    - Utilize M5Capsule to collect motion data and transmit it via Wi-Fi to a server.
    - Store and analyze movement patterns to detect daily activity trends.
2. To enable proximity-based automation of appliances
    - Use a T-SIM camera to detect M5Capsule’s Bluetooth signal when the user enters or leaves a room.
    - Send commands to a Raspberry Pi, which will control a Tapo smart plug to turn appliances on or off automatically.
3. To improve safety through inactivity detection
    - Monitor the user’s movement and send alerts to caregivers if inactivity is detected for an extended period.
4. To optimize energy efficiency in smart homes
    - Ensure that lights, fans, or other devices only operate when needed, reducing power consumption.
5. To provide remote monitoring and manual control
    - Develop an interface for caregivers or users to view motion data and manually control appliances if necessary.

## Our Members
1. Chutipon Trirattananurak
2. Natthapol Sangkool
3. Kazuma Yoshida

## Stakeholders
1. Wheelchair User
2. Elderly
3. Caregiver

## User Stories
1. As a **<ins>wheelchair user</ins>**, I want my **motion data to be tracked and stored** on a server so that I can monitor my daily activity and caregivers can check for unusual patterns.
2. As a **<ins>wheelchair user</ins>**, I want **smart appliances (lights, fan, TV, etc.) to turn on automatically** when I enter a room so that I don’t have to manually control them.
3. As a **<ins>wheelchair user</ins>**, I want the **system to turn off appliances automatically** when I leave a room so that I can save energy and reduce fire hazards.
4. As a **<ins>caregiver</ins>**, I want to **receive an alert if the wheelchair user** has been inactive for too long so that I can check on their well-being.
5. As a **<ins>wheelchair user</ins>**, I want to be **able to manually turn appliances on/off** via a smartphone app so that I have control when needed.

## Hardwares
<img src="https://raw.githubusercontent.com/ChutiponTri/ict720-project/refs/heads/main/m5capsule.png" alt="alt text" width="400">

M5Stack Capsule Kit w/ M5StampS3 ( M5Stack Capsule Kit w/ M5StampS3)
- [Documentation](https://docs.m5stack.com/en/core/M5Capsule)
- [6-Axis IMU BMI270](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/core/MPU-6886-000193%2Bv1.1_GHIC_en.pdf)

---

<img src="https://raw.githubusercontent.com/ChutiponTri/ict720-project/refs/heads/main/tsim.png" alt="alt text" width="400">

T-SIMCAM ESP32-S3 CAM Development Board WiFi Bluetooth 5.0 Wireless Module
- [Documentation](https://lilygo.cc/products/t-simcam?srsltid=AfmBOoqbCqzsFoeuB6VMgMevYe_R6TFA3whzR2Cuaq5tDpFaRyuv9rE3)
- [Github Examples](https://github.com/Xinyuan-LilyGO/LilyGo-Camera-Series?spm=a2g0o.detail.1000023.11.135d19533BcRsR)

---

<img src="https://raw.githubusercontent.com/ChutiponTri/ict720-project/refs/heads/main/raspi%203.png" alt="alt text" width="400">

Raspberry Pi 3 Model B+
- [Documentation](https://www.raspberrypi.com/products/raspberry-pi-3-model-b-plus/)
- [Getting Started](https://www.raspberrypi.com/documentation/computers/getting-started.html)

---

<img src="https://raw.githubusercontent.com/ChutiponTri/ict720-project/refs/heads/main/Tapo.jpg" alt="alt text" width="400">

TP-Link Smart Home
- [About Tapo](https://www.tapo.com/th/)
- [Tapo P100](https://www.tp-link.com/th/home-networking/smart-plug/tapo-p100/)

## System Architecture
<img src="https://raw.githubusercontent.com/ChutiponTri/ict720-project/refs/heads/main/system_architecture.png" alt="alt text" width="600">
1. Motion Tracking & Activity Monitoring
    - M5Capsule: Attaches to the wheelchair and collects motion data.
    - Wi-Fi Transmission: Sends motion data to a cloud server.
    - Data Storage & Analysis: Tracks movement patterns and detects inactivity.

2. Proximity-Based Automation
    - T-SIM camera: Detects the wheelchair's Bluetooth signal when entering/leaving a aroom.
    - Raspberry Pi: Receives signals and processes automation commands.
    - Tapo Smart Plug: Controls home appliances (light, fan, TV, etc.).

3. Safety & Energy Efficiency
    - Inactivity Alerts: If no movement is detected for an extended period, an alert sent to caregivers.
    - Smart Power Management: Appliances turn off automatically when the user exits a room.
  
4. Remote Monitoring & Manual Control
    - Cloud Dashboard/App: Allows caregivers and users to view motion data.
    - Manual Appliances Control: Users can turn appliances on/off through a smartphone app.

## Software Architecture
<img src="https://raw.githubusercontent.com/ChutiponTri/ict720-project/refs/heads/main/software_architecture.png" alt="alt text" width="600">
## State Diagram

## Sequence Diagram

## Data Flow Process

## Data Modeling
