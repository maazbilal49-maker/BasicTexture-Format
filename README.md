# 🎨 BTEX (BasicTexture Format)

A lightweight, human-readable, text-based texture format built from scratch using C and SDL3. 

Instead of relying on heavy image-loading libraries like `SDL_image`, `libpng`, or `libjpeg`, **BTEX** allows you to write retro pixel art directly in your favorite text editor (like Neovim) and parse it into hardware-accelerated SDL3 textures in microseconds.

---

## 🚀 Key Features

- **Human-Readable:** Edit your game assets directly in plain text.
- **Git-Friendly:** No more "Binary files differ" warnings. Track changes to your art pixel-by-pixel inside Git diffs.
- **Ultra-Lightweight:** Powered by a zero-dependency, $O(1)$ Direct Lookup Table (LUT) parser in C.
- **Pixel-Perfect Scaling:** Native integration with SDL3's nearest-neighbor filtering for crisp, retro aesthetics.

---

## 📋 The BTEX Specification

A `.btex` file starts with a single header line defining the dimensions, followed by rows of characters mapping to a color palette.

### Format Structure

```text
[WIDTH] [HEIGHT]
[ROW_1_PIXELS]
[ROW_2_PIXELS]
...
