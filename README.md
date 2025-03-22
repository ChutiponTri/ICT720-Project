# ICT720-project-2025
Repo for demo idea, and code for ICT720 course 2025. 

# Group Onii-Chan
## WheelSense: Development of Automation System for Wheelchair User 
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
1. Chutipon Trirattananurak ID: 6722040786 (chutipon.trir@gmail.com)
2. Natthapol Sangkool ID: 6722040679 (petch.grenger@gmail.com)
3. Kazuma Yoshida ID: Institute of Science Tokyo (kazu0706tokyotech@gmail.com)

## Stakeholders
### Primary Stakeholders:
- Wheelchair Users – Benefit from automated control and activity monitoring.
- Elderly Individuals – Gain enhanced accessibility and safety at home.
- Caregivers – Monitor the user’s activity and receive alerts in case of emergencies.
### Secondary Stakeholders:
- Medical Professionals – Can analyze movement data for health insights.
- Home Automation Developers – Can improve and expand the system’s functionalities.

## User Stories
1. As a **<ins>wheelchair user</ins>**, I want my **motion data to be tracked and stored** on a server so that I can monitor my daily activity and caregivers can check for unusual patterns.
2. As a **<ins>wheelchair user</ins>**, I want **smart appliances (lights, fan, TV, etc.) to turn on automatically** when I enter a room so that I don’t have to manually control them.
3. As a **<ins>wheelchair user</ins>**, I want the **system to turn off appliances automatically** when I leave a room so that I can save energy and reduce fire hazards.
4. As a **<ins>caregiver</ins>**, I want to **receive an alert if the wheelchair user** has been inactive for too long so that I can check on their well-being.
5. As a **<ins>wheelchair user</ins>**, I want to be **able to manually turn appliances on/off** via a smartphone app so that I have control when needed.

## Hardwares
<img src="https://raw.githubusercontent.com/ChutiponTri/ict720-project/refs/heads/main/images/m5capsule.png" alt="alt text" width="400">

M5Stack Capsule Kit w/ M5StampS3 ( M5Stack Capsule Kit w/ M5StampS3)
- [Documentation](https://docs.m5stack.com/en/core/M5Capsule)
- [6-Axis IMU BMI270](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/core/MPU-6886-000193%2Bv1.1_GHIC_en.pdf)

---

<img src="https://raw.githubusercontent.com/ChutiponTri/ict720-project/refs/heads/main/images/tsim.png" alt="alt text" width="400">

T-SIMCAM ESP32-S3 CAM Development Board WiFi Bluetooth 5.0 Wireless Module
- [Documentation](https://lilygo.cc/products/t-simcam?srsltid=AfmBOoqbCqzsFoeuB6VMgMevYe_R6TFA3whzR2Cuaq5tDpFaRyuv9rE3)
- [Github Examples](https://github.com/Xinyuan-LilyGO/LilyGo-Camera-Series?spm=a2g0o.detail.1000023.11.135d19533BcRsR)

---

<img src="https://raw.githubusercontent.com/ChutiponTri/ict720-project/refs/heads/main/images/pi400.jpg" alt="alt text" width="400">

Raspberry Pi 400
- [Documentation](https://www.raspberrypi.com/products/raspberry-pi-400/)
- [Getting Started](https://www.raspberrypi.com/documentation/computers/getting-started.html)

---

<img src="https://raw.githubusercontent.com/ChutiponTri/ict720-project/refs/heads/main/images/Tapo.jpg" alt="alt text" width="400">

TP-Link Smart Home
- [About Tapo](https://www.tapo.com/th/)
- [Tapo P100](https://www.tp-link.com/th/home-networking/smart-plug/tapo-p100/)

## System Architecture
<img src="https://raw.githubusercontent.com/ChutiponTri/ict720-project/refs/heads/main/images/system_architecture.png" alt="alt text" width="600">

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
<img src="https://raw.githubusercontent.com/ChutiponTri/ict720-project/refs/heads/main/images/software_arch.png" alt="alt text" width="600">

## State Diagram
### 1. M5Capsule State Diagram
<img src="https://raw.githubusercontent.com/ChutiponTri/ict720-project/refs/heads/main/images/m5capsule_state_diagram.png" alt="alt text" width="400">

### 2. T-SIMCAM State Diagram
<img src="https://raw.githubusercontent.com/ChutiponTri/ict720-project/refs/heads/main/images/t_simcam_state_diagram.png" alt="alt text" width="400">

### 3. Raspberry Pi State Diagram
<img src="https://raw.githubusercontent.com/ChutiponTri/ict720-project/refs/heads/main/images/raspberry_pi_state_diagram.png" alt="alt text" width="400">

## Sequence Diagram
<img src="https://raw.githubusercontent.com/ChutiponTri/ict720-project/refs/heads/main/images/sequence_diagram.png" alt="alt text" width="600">

## Data Flow Process
<img src="https://raw.githubusercontent.com/ChutiponTri/ict720-project/refs/heads/main/images/data_flow.png" alt="alt text" width="600">

## Data Modeling
<img src="https://raw.githubusercontent.com/ChutiponTri/ict720-project/refs/heads/main/images/Data%20Modeling.PNG" alt="alt text" width="600">

## Benefits of the System
- Enhanced Accessibility – Provides independent appliance control for wheelchair users.
- Improved Safety – Alerts caregivers if the user is inactive for a long period.
- Energy Efficiency – Reduces electricity consumption by turning off unused appliances.
- Caregiver Assistance – Allows remote monitoring to ensure user well-being.
- Data-Driven Insights – Tracks long-term activity patterns to help in health analysis.

## Future Enhancements and AI Integration
1. AI-Based Motion Pattern Analysis (Machine Learning - ML)
    - Use ML models (LSTM, CNN, or Random Forest) to analyze motion data.
    - Detect changes in movement behavior that may indicate fatigue or health risks.
2. AI-Based Fall Detection (Computer Vision + Sensor Fusion)
    - Use motion sensors and camera data to detect falls.
    - Send an emergency alert to caregivers if a fall is detected.
3. AI for Smart Appliance Optimization (Reinforcement Learning - RL)
    - Use Reinforcement Learning to predict and optimize appliance activation based on user behavior.
4. AI-Powered Voice Assistant (NLP Integration)
    - Implement voice commands to control appliances (e.g., "Turn on the lights").
    - Use chatbots for caregivers to ask for real-time user activity updates.
5. Edge AI for Proximity Detection Optimization
    - Train an AI model to enhance Bluetooth-based proximity detection, reducing false activations.

<img src="https://github.com/ChutiponTri/ict720-project/blob/main/images/wheelchair.png" alt="alt text" width="600">

## Conclusion
<p>&nbsp;&nbsp;&nbsp;&nbsp;This project aims to enhance the quality of life for wheelchair users by integrating motion tracking, proximity-based automation, and smart home control. By leveraging AI in future iterations, the system can provide predictive analytics, fall detection, and adaptive automation, further improving safety, accessibility, and energy efficiency.</p>
