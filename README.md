# 3D Terrain Generator

Welcome to the documentation for **3D Terrain Generator**, a graphical tool developed to generate, analyze, and sculpt large-scale 3D terrains.

This software was created as a semester project for the third year of the **Computer Engineering** bachelor's degree at **SUPSI DTI** (Department of Innovative Technologies), developed by **Giona Valsecchi** and **Pietro Brusadelli** ([@PietroBrusa](https://github.com/PietroBrusa)).

---

## Tech Stack and Behind the Scenes

The project was built with a focus on performance and direct hardware control:

- **Graphics Engine:** Proprietary. We use our own custom graphics engine, developed by the team for a previous computer graphics course, which guarantees total control over rendering and 3D mesh manipulation.
- **User Interface (UI):** Built with **ImGui** (with _docking_ support) for a fluid and customizable window-based interface.
- **Core Libraries:**
  - `Reputeless/PerlinNoise` (C++17/20) for the mathematical procedural generation of heightmaps.
  - `syoyo/tinyexr` for professional saving of high dynamic range images.
  - `Native File Dialog (NFD)` for native interfacing with the OS file system.

---

## Terrain Generation (Setup)

The **Texture Setup** window is the starting point. The system uses a **Perlin Noise** based algorithm to procedurally calculate elevation and apply it to the initial geometry.

### World Parameters (Mesh Parameters)

- **World Size (m):** Total physical size of the terrain in 3D space, from small plots (256m) to large-scale maps (8192m).
- **Style (Quad Size):** Distance between vertices (vertex spacing). Forces powers of 2 for perfect LOD scaling (from _1.0m_ for Ultra HD, up to _16.0m_ for a Low Poly look).
- **Height Scale:** Vertical multiplier that scales the overall height of the generated terrain.

### Noise Parameters (Heightmap Parameters)

- **Map Detail:** Pixel density of the heightmap relative to the physical dimensions of the world.
- **Frequency:** Determines the noise frequency (higher values create tighter, denser hills).
- **Octaves:** Number of overlapping noise layers; adds microscopic details, making rocks more jagged.
- **Seed:** Seed for random generation. Using the same seed will reproduce the exact same topographic map.

---

## Navigation and Camera Control

The navigation system allows you to easily move around the workspace to analyze every angle of the mesh.

| Input                    | Action            | Description                                                                 |
| :----------------------- | :---------------- | :-------------------------------------------------------------------------- |
| **Left Click** (Drag)    | **Orbit**         | Rotates the view around the origin. _(Only if the Pointer tool is active)._ |
| **Middle Click** (Wheel) | **Orbit**         | Universal shortcut to rotate the view. _(Always active)._                   |
| **Right Click** (Drag)   | **Panning**       | Moves the view horizontally and vertically in space.                        |
| **Mouse Wheel** (Scroll) | **Zoom In / Out** | Moves the perspective closer or further away.                               |

---

## Editing Tools (Brushes)

Brushes allow you to directly manipulate the terrain geometry. They share three fundamental parameters:

- **Radius:** In **meters (m)**. Defines the size of the area of influence.
- **Strength:** In **meters per second (m/s)**. Determines the intensity and speed of the modification. The effect is continuous as long as the mouse is held down.
- **Falloff:** In **percentage (%)**. Indicates the percentage of the radius within which the effect is at 100% before smoothly degrading towards the edges.

### Brush Types

| Brush         | Function   | Visual Effect                                                   |
| :------------ | :--------- | :-------------------------------------------------------------- |
| **Sculpting** | **Raise**  | Extrudes the terrain upwards to create mountains and hills.     |
| **Erosion**   | **Lower**  | Carves into the terrain downwards to create valleys and basins. |
| **Smoothing** | **Smooth** | Softens roughness and steep slopes for smoother transitions.    |

---

## Viewport Modes

Four rendering modes are available to analyze the topology, slopes, and aesthetics of the mesh:

| Mode               | Description and Utility                                                                                             |
| :----------------- | :------------------------------------------------------------------------------------------------------------------ |
| **Wireframe**      | Shows the polygonal skeleton of the mesh for technical density checking.                                            |
| **Solid**          | Monochromatic display with basic shading to quickly evaluate volumes.                                               |
| **Realistic**      | Procedural coloring based on normals: green texture for plains, gray rock texture for steep walls (slope > 70-80%). |
| **Altitude Color** | Thermal gradient based on elevation: **Blue** (valleys/low), **Green** (plains/medium), **Red** (peaks/high).       |

---

## Export and Save

The tool is not an end in itself: data can be exported in standard formats for integration into external software (such as Blender, Maya, Unreal Engine, Unity).

- **Export Mesh (`.obj`):** Saves the 3D geometry in the universal Wavefront OBJ format.
- **Export Texture (`.exr`):** Saves the heightmap in ultra-high dynamic range (floating-point) OpenEXR format, ensuring no data loss and avoiding the "stepping" effect typical of 8-bit image formats.
