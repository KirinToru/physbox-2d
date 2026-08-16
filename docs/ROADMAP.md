# Technical Roadmap

> Outline of the technical milestones for the 2D Physics Sandbox overhaul.

---

## Phase 1: Core Physics & Tools Foundation

- [x] **Engine Update**: Migrate to C++23, CMake 4.4.2, and SFML 3.1.0.
- [x] **Physics Engine Integration**: Integrate Box2D v3 into the build system and game loop.
- [x] **Physics World Setup**: Initialize the Box2D physics world, define gravity, and handle world stepping.
- [x] **Debug Rendering**: Implement Box2D debug drawing to visualize rigid bodies, shapes, and joints in the engine.

## Phase 2: Mouse Controls & Object Interaction

- [x] **Mouse Input System**: Handle mouse clicks, drag, and drop within the game window.
- [x] **Physics Picking**: Implement Raycasting or AABB queries via Box2D to select objects on screen.
- [x] **Object Spawning**: Create the core "Gun" mechanic that allows players to shoot/spawn objects where they click.
- [x] **Object Manipulation**: Allow players to grab, move, and throw physics objects using the mouse.

## Phase 3: Sandbox Elements & Wind Mechanics

- [x] **Dynamic Objects**: Implement various sandbox objects (e.g., squares, circles/balls, rectangles) with correct mass, friction, and restitution.
- [x] **Wind System**: Implement a global or localized wind force system. 
- [x] **Wind & Player Interaction**: Apply forces to the player character or change max speed depending on if they move with or against the wind.
- [x] **Wind & Object Interaction**: Apply wind forces to physics objects based on their properties, making balls roll and lightweight objects fly.

## Phase 4: Garry's Mod Style Spawn Menu

- [x] **UI Overlay System**: Create an in-game UI system capable of rendering a spawn menu.
- [x] **Spawn Menu Interface**: Implement a categorized menu showing available objects to spawn.
- [x] **Object Selection**: Allow the user to click an object in the menu to equip it to their "Spawner Gun".
- [x] **Tool Modes**: (Optional) Add different tools to the menu (e.g., Delete tool, Connect/Joint tool).

## Phase 5: Polish & Optimization

- [x] **Visual Polish**: Add sprites and textures to the physics bodies.
- [x] **UI Polish (HL2 Style)**: Implement Source-engine style menus (Pause, Main Menu), popup windows, and developer console integration.
- [x] **Input & Coordinate Handling**: Ensure UI correctly handles mouse mapping across different window scales and fullscreen modes.
- [ ] **Particle Effects**: Add visual cues for the wind (e.g., wind lines or blowing leaves).
- [ ] **Performance Profiling**: Ensure physics simulation handles large amounts of spawned blocks smoothly.

## Phase 6: Audio & Props
- [x] **UI Sounds**: Add hover and click sounds for UI buttons in Menu and Pause screens.
- [ ] **Game Sounds**: Add background music, and collision/interaction sounds for objects.
- [x] **New Objects**: Add specific sound profiles to objects (bouncy ball, wooden crate, steel triangle) and add a new Light Star object.

## Phase 7: Gravity Gun & Maps
- [ ] **Gravity Gun Beam**: Add a visual bending beam from the player to the held object, and a blue glowing outline around the selected object.
- [ ] **Map System (TMX)**: Reintroduce the map selection list and TMX map format loading.
