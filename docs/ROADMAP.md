# Technical Roadmap

> Outline of the technical milestones for the 2D Physics Sandbox overhaul.

---

## Phase 1: Core Physics & Tools Foundation

- [ ] **Engine Update**: Migrate to C++23, CMake 4.4.2, and SFML 3.1.0.
- [ ] **Physics Engine Integration**: Integrate Box2D v3 into the build system and game loop.
- [ ] **Physics World Setup**: Initialize the Box2D physics world, define gravity, and handle world stepping.
- [ ] **Debug Rendering**: Implement Box2D debug drawing to visualize rigid bodies, shapes, and joints in the engine.

## Phase 2: Mouse Controls & Object Interaction

- [ ] **Mouse Input System**: Handle mouse clicks, drag, and drop within the game window.
- [ ] **Physics Picking**: Implement Raycasting or AABB queries via Box2D to select objects on screen.
- [ ] **Object Spawning**: Create the core "Gun" mechanic that allows players to shoot/spawn objects where they click.
- [ ] **Object Manipulation**: Allow players to grab, move, and throw physics objects using the mouse.

## Phase 3: Sandbox Elements & Wind Mechanics

- [ ] **Dynamic Objects**: Implement various sandbox objects (e.g., squares, circles/balls, rectangles) with correct mass, friction, and restitution.
- [ ] **Wind System**: Implement a global or localized wind force system. 
- [ ] **Wind & Player Interaction**: Apply forces to the player character or change max speed depending on if they move with or against the wind.
- [ ] **Wind & Object Interaction**: Apply wind forces to physics objects based on their properties, making balls roll and lightweight objects fly.

## Phase 4: Garry's Mod Style Spawn Menu

- [ ] **UI Overlay System**: Create an in-game UI system capable of rendering a spawn menu.
- [ ] **Spawn Menu Interface**: Implement a categorized menu showing available objects to spawn.
- [ ] **Object Selection**: Allow the user to click an object in the menu to equip it to their "Spawner Gun".
- [ ] **Tool Modes**: (Optional) Add different tools to the menu (e.g., Delete tool, Connect/Joint tool).

## Phase 5: Polish & Optimization

- [ ] **Visual Polish**: Add sprites and textures to the physics bodies.
- [ ] **Particle Effects**: Add visual cues for the wind (e.g., wind lines or blowing leaves).
- [ ] **Performance Profiling**: Ensure physics simulation handles large amounts of spawned blocks smoothly.
