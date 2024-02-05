![Logo](https://github.com/AJM432/AJM432.github.io/assets/49791407/6db8ea27-6acb-4613-898f-41b80ef10c0a)


# StarSwift: High-Performance Barnes-Hut Stellar Simulation

Effortlessly navigate the cosmos with StarSwift's high-performance Barnes-Hut algorithm. Enhanced with quadtrees and recursive gravity calculations, this streamlined engine accurately models thousands of stars, guaranteeing an optimized celestial experience.


![](https://img.shields.io/badge/C++-00599C?style=plastic&logo=C%2B%2B&logoColor=white)

## Demo
![](https://github.com/AJM432/Solar-System-Simulator/assets/49791407/3852c4c3-14ae-4cd8-9c43-337554d73774)

## System Parameters
![](https://github.com/AJM432/Star-Swift/assets/49791407/d1494e0a-d3f8-4caa-b7ed-201b5dae4e5b)
### General
- `Pause/Start`
	- This button allows users to toggle the simulation update cycle to allow them to view a particular system state.

- `Reset Galaxy`
	- Reset the system state to the initial galaxy configuration when the program began. Use this when the system becomes unstable.

- `Gravitational Strength`
	- Determines the mass of a star. Each star is assumed to have the same mass. Increasing this value increases the force of the interaction between neighboring stars creating local star clusters.

- `Max Star Velocity`
	- Determines the maximum velocity a star can travel preventing the system from becoming unstable due to large gravitational forces. Also allows users to reset the system state without needing to reset the entire program.

- `Theta Threshold`
	- This parameter is the core of the Barnes-hut algorithm. Theta determines the accuracy of the simulation. Setting $\theta = 0$ reduces the simulation to a naive n-body simulation of time complexity $O(n^2)$. Increasing this value gradually reduces the time complexity to $O(n)$ by sacrificing accuracy for speed. Increase this parameter to increase simulation speed.

- `Collision Softening`
	- Dampens the force between two stars thereby reducing the chance that a star is launched outside the system. We calculate acceleration by $a_{star} = \frac{m_{star}}{r^2+d_{soft}}$ where $d_{soft}$ is the softening parameter. This helps prevent velocities from becoming infinite when stars get too close to one another.

- `Galaxy Color`
	Change the base color of the galaxy.
	
- `Color Modes`
	- Change the method that determines the color of each star. Choose between three modes (Radial, Solid, Velocity).
	
		- **Radial**: Stars near the center of mass of the galaxy are white whereas stars far from the center of mass are closer to the `galaxy color`. This gives the illusion that the center of mass is at a higher temperature.
		- **Solid**: All stars are the same color as the galaxy color.
		- **Velocity**: Stars travelling at a higher velocity are white whereas stars travelling at low velocities are closer to the `galaxy color`.

### Vector Display
- `Show Velocity Vectors`
	- Enabling this option allows users to view the velocity vectors of all stars in the system represented by a white line.

- `Show Gravity Vectors`
	- Enabling this option allows users to view the gravity/acceleration vectors of all stars in the system represented by a white line.

### Stats
- `System Energy`
	- Shows the energy components of the system. Ideally, the kinetic and gravitational potential energies will always have equal magnitude but opposite sign or $U = -K$ assuming the system had very little total energy in the beginning. As the user changes the `Gravitational Strength` or `Max Star Velocity` they artificially introduce energy into the system and thus $U \neq -K$ in those scenarios.

## Features

- View stellar dynamics in real-time
- Tunable system parameters for experimentation
- View system variables such as velocity and acceleration in real-time
- View kinetic energy graph during system evolution
- Fullscreen mode

## Usage/Examples
Manually change starting parameters to observe different initial galaxy configurations.
```cpp
// Determine galaxy size and shape
const int NUM_STARS = 5000;
const int RADIUS = 300;
...
// Determines the shape of the galaxy, currently a perfect circle
std::uniform_real_distribution<double> dist_pos_y(-y_variance+height_middle, y_variance+height_middle);
```


## Run Locally
`Ensure you have g++ and make installed`

Clone the project

```bash
  git clone https://github.com/AJM432/Star-Swift.github.io.git
```

Go to the project directory

```bash
  cd Star-Swift
```

Build the project

```bash
  make
```

Run StarSwift

```bash
  ./StarSwift
```


## Tech Stack
**Graphics and Window Handling**: SDL2 (Simple DirectMedia Layer)

**GUI Framework**: ImGui (Immediate mode GUI library for C++)

**Plotting**: ImPlot (Extension for ImGui for real-time plotting)

**Programming Language**: C++
## Feedback

If you have any feedback, please reach out to me at alvinjosematthew@gmail.com
## Acknowledgements
Here are some excellent articles describing how the Barnes-Hut algorithm works!
 - [The Barnes-Hut Algorithm: Tom Ventimiglia & Kevin Wayne](http://arborjs.org/docs/barnes-hut)
 - [The Barnes-Hut Approximation: Efficient computation of N-body forces By Jeffrey Heer](https://jheer.github.io/barnes-hut/)


## License

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
## 🔗 Links
[![portfolio](https://img.shields.io/badge/my_portfolio-000?style=for-the-badge&logo=ko-fi&logoColor=white)](https://alvinmatthew.me/)

[![SolarSynesthesia](https://img.shields.io/badge/Solar-Synesthesia-20B2AA?style=for-the-badge)](https://github.com/AJM432/Solar-Synesthesia)

[![SolarSynesthesia](https://img.shields.io/badge/Solar%20System-Simulator-20B2AA?style=for-the-badge)](https://github.com/AJM432/Solar-System-Simulator)