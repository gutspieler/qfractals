This is a C++ program using Qt to view fractals of all kinds. Currently, there are apart from the mandelbrot set a few further fractals and a preset magnetic pendulum-simulation.

My focus is not on speed but on feature-richness while retaining a good usability also for unexperienced users.

I develop in MacOSX and sometimes test in Windows 7 x64.

### Features so far ###
  * Minimalistic interface (to focus on the important stuff)
  * Antialiasing (2x2 RGSS)
  * Browsing mode (Google-Earth-alike real time zooming [way slower than e.g. fraqtive](although.md) and dragging)
  * Intuitive keyboard navigation (see below)
  * Intuitive mouse navigation (see below)
  * Scaled image (very useful for quickly looking at images at a lower resolution)
  * Arbitrary image size
  * Saving images in PNG-Format
  * Using long doubles for fractals (80 bit depth instead of 64 bits using doubles)
  * Smooth coloration
  * Multithreading
  * Platform independent (built successfully using QtCreator 2.0b in MacOSX, Linux and Windows 7 x64)
  * Tabs
  * Burning Ship-Fractal, Lambda-Fractal, Magnet1m-Fractal, IFSs
  * Magnetic Pendulum-Simulation
  * Cubic interpolation (clamped cubic splines) for colors
  * Buddhabrot of various fractals

### Usage ###

In the **Settings**-Menu you can select the navigation mode, either **Browse** or **Select**. In Browse-Mode you can zoom using the mouse wheel (very much Google-Earth-alike) in an intuitive way and drag to move the image.

In Select-Mode you can drag a frame around the area you a interested in and commit your choice by double-clicking into it. You can also move either one of the corners or the whole selection.

There are various modes:

  * Default: Opposite of selected corner is fixed, aspect ratio is kept.
  * Shift-Key: The center of the selection is fixed, aspect ratio is kept.
  * Alt-Key: The current rotation and the center is kept, the aspect ratio is modified.
  * Alt+Shift-Key: Combination of the previous two modes.

Independent of the selected mode, you can also navigate using the keyboard:

  * Zoom in and out using + and - (default factor 0.85, Alt-key pressed: 0.25)
  * Move with the cursor keys (16 pixels per default, 1 pixel with the Shift-key, 256 pixels with the Alt-key)

### Known bugs ###

  * No message if the image was not saved
  * Tabs currently have no useful name
  * No progress report when saving large files
  * In Windows the threads are pretty out of sync. (I think I fixed that one?)
  * Crash when saving images that are too large for your memory

### Supported/Implemented but not yet accessible via the interface ###

  * Changing the color palette without recalculation
  * Use of templates in the calculations (instead of long doubles every type can be used; should be a nice thing to get arbitrary precision)
  * Well, a lot of stuff, but look at settings.cpp if you want to change some parameters.

### Planned in the future ###

  * A nice parser/compiler for functions
  * Shearing in select-Mode
  * Interface for entering functions
  * Editing color palette
  * Ants simulation, Cellular simulations, ... (proposals?)
  * Various coloring methods (triangle inequality, etc...)
  * Create Julia-Set by selecting a point
  * History/Bookmark/Saving parameters as XML
  * UI-improvements (browser-alike)
  * Command line

This would be version 1.00 in my opinion.

### Planned but I don't know whether I will implement it ###

  * Full screen mode
  * Creating movies based on XML-Files (I really would love that one...)