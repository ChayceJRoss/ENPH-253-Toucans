# ENPH-253 - Team Toucans Software

## What is ENPH 253 - Introduction to Instrument Design?

UBC Engineering Physics is an intense, five-year undergraduate engineering / physics / math combined major tailored for students who want to work at the new leading edge of science and engineering, whether in industry or academia. Part of this program is a series of project-based courses to educate students in the practice of engineering and applied physics. Introduction to Instrument Design is one of these courses, taken by our second-year (sophomore) students. Its goal is to train students in the practical aspects of intelligent instrument design and construction, including electrical and mechanical design, prototyping, microcontrollers, sensors, actuators, motion control, and control theory.

To offset the intensity and difficulty of the course, the engineering content to be tackled by the students has been framed in the context of an autonomous robot competition. Each year students spend six weeks in a series of labs and lectures to learn some basic elements of electromechanical design, and then form teams of four to embark on seven weeks of full-time robot design and construction. Time commitment during this phase is at the students’ discretion but often reaches 50 – 70 hrs/week. The course culminates in a publicly attended robot competition that also serves as the final exam. Past competitions have included one-on-one hockey, volleyball, and search and rescue challenges.

## 2021 Summer Competition Details

Here are the [Official Competition Rules](https://docs.google.com/document/d/e/2PACX-1vSLlSZZVygysL-lW5NMPqw43kuUsevNV7X7EGTOROZqbiwdKwnDBDWqIHAD17FZLw/pub).

In summary, the design challenge was to simulate a Mars Rover sample return mission. We designed and built a fully autonomous robot that lowers itself from a skycrane, and proceeds to retrieve as many empty soda cans as possible within a time limit. 

The entire competition was recorded on this public [Youtube video](https://www.youtube.com/watch?v=xkh0X_zH3i8&ab_channel=enphtwofivethree). Team Toucan placed 3rd in the competition, out of 16 teams. 

## Robot Software Architecture

We used a Finite State Machine architecture for the robot, which specified 4 distinct states that the robot could operate in: `INITIALIZE`, `SEARCH`, `STORE_CAN`, `STOP_DROP_ROLL`. The robot employs various sensors to sense its surroundings and jump between states, including two reflectance sensors positioned at the front of the robot that enables it to follow a path of electrical tape while driving. 
A flowchart of the high level strategy can be seen below.

![Strategy](https://user-images.githubusercontent.com/61093495/139575876-dfb8e927-fe88-4b35-8aa1-666841d9b04a.png)

## How to Run
The software is designed to run on BluePill F103C8 boards (STM32F103C8T6 with 64k flash memory). We used [PlatformIO](https://platformio.org/), a microcontroller build system, to edit and upload the software onto the boards. It is easiest to use its IDE on VS Code. A ST-Link V2 was used to upload the code from computer to the BluePill. 

