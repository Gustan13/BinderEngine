# BinderEngine
The BinderEngine is an open source 3D game engine created for a specific video game currently in development.
It currently has basic phong shading support, skeletal animation (without position interpolation) and texturing.

The engine will focus on optimizing collisions using tree data structures and interior worlds to explore.
The base engine will also eventually have a simple editor to change the layout of the game being made and future similiar games.

## Structure
Unlike most engines, the BinderEngine will not be entierly structured as a tree of nodes, dynamic objects and static objects will be stored in two different vectors and the map geometry
in a tree. Models, animations and maps are loaded in the initialization process of the engine, sacrificing memory in order to optimize processing time during gameplay. 

### Preloaded Assets
The engine preloads several types of assets during initialization. Models, animations and maps are stored in tables that can be quickly accessed without much computational power.

### Maps
The maps will hold two structures: the geometry that will be shown to the players and the invisible collision geometry. The second structure will be a set of triangles that interacts with eliptical geometry that represents the game's objects. This interaction will be implemented as defined in this paper: (http://www.peroxide.dk/papers/collision/collision.pdf).

### Objects
The objects that will be present in the game are classes that will contain the eliptical geometry that interacts with the map and with other objects, implementing the algorithm described in the paper above, and point to previously loaded models and animations in the aforementioned tables to define their visuals. It is the programmer's job to create subclasses in order to properly define the desired object's behaviour.

## Dependencies
The engine uses OpenGL for rendering and GPU interactions, GLM for mathematical formulas and Assimp for model handling.
