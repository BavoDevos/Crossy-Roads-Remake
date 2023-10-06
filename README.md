# Project Overview
This project was an assignment for my Graphics Programming 2 course at HoWest-DAE. We were tasked with remaking a small game or part of a game with a focus on the graphics. The assignment was made using the "Overlord Engine" which was a unfinished c++ engine, througout the semester we were tasked with working on the render pipeline of the engine.

# Crossy Road
I chose to remake Crossy Road as I enjoy the game and thought it would be an interesting game for this specific assignment. I wanted to make the experience feel close to the real game by having random tiles spawn and slowly ramping up the difficulty of the game. For visuals I thought a cartoonish outline and a slight cell shading would work really well with the simplistic voxel style the game already had.


![ScreenShot0](https://github.com/BavoDevos/Crossy-Roads-Remake/assets/113976223/42a1b5fc-39fe-4e09-9022-7bb84c2b9216)

![ScreenShot1](https://github.com/BavoDevos/Crossy-Roads-Remake/assets/113976223/06551be8-5f6e-4025-8e5a-a2b3a8840159)

![ScreenShot2](https://github.com/BavoDevos/Crossy-Roads-Remake/assets/113976223/51f3ac75-3fb0-4cef-b928-a8074ec5dd67)

# Technical Detail
This project as stated before was done in 2 parts, the first part was implementing various technical features to the engine and the second part being the recreation of our chosen game. Here are some of the technical highlights:

* Geometry Shader : Utilize a geometry shader to enchance rendering and creating complex geometric shapes.
* Sprite Rendering : Implements sprite rendering techniques for 2D elements.
* Animation Skinning : Includes animation skinning for character movements and accurate shadows when implementing shadow mapping.
* Shadow Mapping : Incorpoates advances shadow mapping tehcniques to enchnce the visual experience and create realistic lighting and shadows.
* Post-Processing : provides post processing, in my case the outline and cell-shading was done as post processing.
* Text Rendering : implements text rendering for in-game HUD and UI elements.
* Particles : A particle system that is used for dynamic elements which add a visual flair with a billboarding effect but also the option to not have this effect.

# Conclusion
At the end I was very proud of what I had made, both the work I had done on the render pipeline as the game itself. With in the mind the time constrains of the deadline I feel the final product is in a good state, it could use some extra work to add missing features like the moving logs in the river or a wider variety of cars and behaviours maybe even the eagle mechanic. Outside these extra things I am very content with the product and perhaphs in the coming future I will further develop this project so it can be fully polished to my hearts desire. 
