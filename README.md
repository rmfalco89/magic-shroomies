# magic-shroomies
[WIP]

In mushroom cultivation, a FC (Fruiting Chamber) is a controlled environment in which temperature and relative humidity are kept constant to values specific to the type of mushroom.

This project is a firmare for the Arduino framework to control
- a temperature and relative humidity sensor
- an heat lamp
- a fan that gently pushes air onto the heat lamp for better uniform dissipation of the heat in the chamber
- a humidifier
- a FAE (Fresh-Air-Exchange) fan
- a momentary switch to signal the presence of ice in the chamber and use the fan to move the air internally to the chamber and bring the temp down
- an oled display

The heat lamp and fan are activated so that the temperature is brought gently up avoiding burning the magic creatures.

THe fresh air exchange fan is used to push new air in the chamber every 4/5 hours, and also to help spread the humidity uniformly in the environment.

The temp sensor can be configured to perform multiple readings and average out the results
