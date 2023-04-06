# particles

Implements particle demos

<img width="925" alt="Screen Shot 2023-04-06 at 3 17 49 PM" src="https://user-images.githubusercontent.com/55254786/230474364-d4172b75-b650-42aa-8a77-6e1405e12bc0.png">

## Demo of basic features

Basic features: 
- sparkel-trail: trail of sparkles follows a revolving sprite
- explosion: one single explosion sprite is animated 
- billboard-axis: textured quad of a tree will face camera upon rotation. 

## Unique Particle System: Cannabilistic Cellular Automata 

DEMO VIDEO: https://www.kapwing.com/videos/642e3c66bac9230018d7d7a0

A cellular automaton (CA) is a collection of cells such that each cell changes state as a function of time, according to a defined set of rules driven by the states of neighboring cells. This demo uses the agl library to simulate cellular automata that can cannabilize and grow (similar to agar.io). 

Rules: 
1. When particles hit the border of the screen, they will go the other way. 
2. When two particles are fairly close to one another, the bigger particle will "pull" the smaller particle in. 
3. When two particles are very close to one another, the bigger particle will absorb the smaller particle and gain size proportional to the smaller particle's size. 
4. When there is only one particle remaining, that particle will burst into smaller particles. 


## How to build

*Windows*

Open git bash to the directory containing this repository.

```
particles $ mkdir build
particles $ cd build
particles/build $ cmake ..
particles/build $ start particles.sln
```

Your solution file should contain four projects.
To run from the git bash command shell, 

```
particles/build $ ../bin/Debug/billboard-axis.exe
particles/build $ ../bin/Debug/explosion.exe
particles/build $ ../bin/Debug/sparkle-trail.exe
particles/build $ ../bin/Debug/sparkles.exe
```

*macOS*

Open terminal to the directory containing this repository.

```
particles $ mkdir build
particles $ cd build
particles/build $ cmake ..
particles/build $ make
```

To run each program from build, you would type

```
particles/build $ ../bin/billboard-axis
particles/build $ ../bin/sparkles
particles/build $ ../bin/sparkle-trail
particles/build $ ../bin/explosion
```
