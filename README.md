# COST-EFFECTIVE-INTEGRATED-DISTANCE-AND-SOUND-LEVEL-MONITOR
The main objective of this project is to create a low-cost integrated device for the measurement of sound levels at a given distance from vehicles, that is, to help the Motor
Vehicle Department (MVD) enforce rules against modified exhaust systems.
At the moment, measurements of sound levels are mostly carried out at random distances, most of which are too close to the exhaust pipe, and hence produce sound readings
that are highly exaggerated. Not only does this lead to erroneous evaluations but also
unnecessarily incurs fines on the owners of the vehicles. The suggested solution therefore
tackles this crucial problem by making sure sound levels are properly measured from a
fixed distance of 7.5 meters, hence encouraging adherence to noise rules

According to legal norms, exhaust noise needs to be measured from a uniform field-free distance of 7.5
meters. But in real life, officers measure sound at a much shorter distance because there
are no reliable tools used for measuring distance, leading to incorrect readings and high
fines. To mitigate this, the system incorporates a TF Luna LiDAR sensor for accurate
distance measurement and a MAX4466 analog microphone for sound intensity estimation.
The measurements are calculated by employing an ESP32 microcontroller and are shown
real-time on a 0.96-inch OLED display, enabling the user to see both parameters at the
same time.

The MAX4466 sensor detects changes in environmental noise and converts such changes
into electrical signals, which the ESP32 interprets to determine levels of noise. At the
same time, the TF Luna LiDAR detects object distances based on time-of-flight infrared
pulses. The ESP32 interprets data from both sensors and displays it in real-time, thereby
allowing users to receive real-time information about environmental conditions
