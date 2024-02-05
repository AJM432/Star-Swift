![Logo](https://github.com/AJM432/AJM432.github.io/assets/49791407/6db8ea27-6acb-4613-898f-41b80ef10c0a)


# StarSwift: High-Performance Barnes-Hut Stellar Simulation

Effortlessly navigate the cosmos with StarSwift's high-performance Barnes-Hut algorithm. Enhanced with quadtrees and recursive gravity calculations, this streamlined engine accurately models thousands of stars, guaranteeing an optimized celestial experience.


![](https://img.shields.io/badge/C++-00599C?style=plastic&logo=C%2B%2B&logoColor=white)

![](https://img.shields.io/github/repo-size/AJM432/Star-Swift)
## Demo
![](https://github.com/AJM432/Solar-System-Simulator/assets/49791407/3852c4c3-14ae-4cd8-9c43-337554d73774)
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
// Determines shape of the galaxy, currently a perfect circle
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
Here are some excellent articles describing how the barnes-hut algorithm works!
 - [The Barnes-Hut Algorithm: Tom Ventimiglia & Kevin Wayne](http://arborjs.org/docs/barnes-hut)
 - [The Barnes-Hut Approximation: Efficient computation of N-body forces By Jeffrey Heer](https://jheer.github.io/barnes-hut/)


## License

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
## 🔗 Links
[![portfolio](https://img.shields.io/badge/my_portfolio-000?style=for-the-badge&logo=ko-fi&logoColor=white)](https://alvinmatthew.me/)

[![SolarSynesthesia](https://img.shields.io/badge/Solar-Synesthesia-20B2AA?style=for-the-badge)](https://github.com/AJM432/Solar-Synesthesia)

[![SolarSynesthesia](https://img.shields.io/badge/Solar%20System-Simulator-20B2AA?style=for-the-badge)](https://github.com/AJM432/Solar-System-Simulator)