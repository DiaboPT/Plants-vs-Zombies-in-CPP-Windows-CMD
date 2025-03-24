# Plants vs. Zombies in C++ (Windows CMD)

This is a command-line implementation of a **Plants vs. Zombies**-style game written in **C++**. It features an object-oriented design with an **ECS-style architecture** and **fixed time step** for updates. Rendering is done using **std::cout**, and input handling uses **getch()**.

## Game Overview

### Boards

#### Plants Board (size: `5x2`)
```
+---+ +---+---+---+
| C | | S | P | W | <- Plants
+---+ +---+---+---+
| n | | 1 | 2 | 1 | <- Cost
+---+ +---+---+---+
```
- `C` = Currency (Starts with 2 C) **(Yellow)**
- `S` = Sunflower (Generates 1 C every 9 frames) **(Yellow)**
- `P` = Peashooter (Shoots 1 projectile per frame, 6 HP) **(Green)**
- `W` = Wall-nut (75 HP) **(Brown)**
- Selection is highlighted **(Blue)** (`vector2(KEY_Q, KEY_E)`).

#### Game Board (size: `9x5`)
```
+---+---+---+---+---+---+---+---+
| S | P | * |   | * | Z |   |   |
+---+---+---+---+---+---+---+---+
| S |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+
| S |   |   |   | W | Z |   |   |
+---+---+---+---+---+---+---+---+
| S |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+
| S |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+
```
- `*` = Projectile (Moves 1 cell per frame, deals 1 damage) **(Green)**
- `Z` = Zombie (Moves 1 cell every 3 frames, deals 1 damage per frame, 10 HP) **(Red)**
- Selection is highlighted **(Blue)** (`vector4(KEY_A, KEY_D, KEY_S, KEY_W)`).
- Players can place selected plants if they have enough currency.

## Development Plans & Considerations

### Suggestions & Improvements
- **Zombie behavior variety** - More zombie types (fast, tanky, etc.).
- **More plant variety** - Snow Peas (slows zombies down by increasing move cooldown).
- **Plant cooldowns** - Delay before reusing the same plant.
- **Balancing economy** - Sunflowers currently generate sun every 9 frames; might adjust to 6.
- **Projectile-Zombie timing** - Ensures projectiles always hit zombies.

### Technical Considerations
- **Rendering** - Layer-based system (static and updatable layers) using `std::cout`, `sleep()`, and `clear()`.
- **Input Handling** - `getch()`.
- **Collision Detection** - A projectile is removed when it lands on the same or further cell as a zombie and damages it.

## Architecture

### Game Loop
- **Fixed time step** (Potentially **60 FPS**, but can reduce to **10 FPS** if flickering occurs).
- **Separate update and render phases** (`Start()` and `Update()` functions).

### Object-Oriented Approach
- `GameObject` base class with `Transform` component.
- ASCII Art style planned for the future.
- **Component-based** approach for enhancing objects.

### ECS-Style Design
- **Entities** are built like a puzzle, defined by their components:
  - **Plants:** `TransformComponent`, `HealthComponent`, `ProjectileInstantiateComponent` (for shooting plants).
  - **Zombies:** `TransformComponent`, `MoveComponent`, `HealthComponent`.
  - **Zombotany:** `TransformComponent`, `MoveComponent`, `HealthComponent`, `ProjectileInstantiateComponent` (A zombie that shoots like a Peashooter).

### Component Storage
- **ECS-style centralized storage**, rather than storing components in each GameObject.
- Entities are linked to components via **ID-based mapping**.

## Future Features
- ASCII Art visuals.
- Additional plants and zombies.
- More advanced collision and AI.
- Improved UI and gameplay mechanics.

---
This project is currently in development! Feedback and contributions are welcome.

