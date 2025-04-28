# Raytracer Shader System

This document explains how to use the shader system in the raytracer project.

## Overview

Shaders allow you to modify the appearance of rendered objects by applying post-processing effects to the calculated colors. The raytracer supports multiple shader types and allows combining them using a decorator pattern.

## Basic Usage

Add a `shaders` section to your scene configuration file:

```cfg
shaders = (
  {
    type = "basic";  # The most basic shader that doesn't modify colors
  }
);
```

## Available Shader Types

### Basic Shader

The simplest shader that doesn't modify colors.

```cfg
{
  type = "basic";
}
```

### Phong Shader

Adds specular highlights to surfaces.

```cfg
{
  type = "phong";
  specularStrength = 0.5;  # Strength of the specular highlight (0.0-1.0)
  shininess = 32.0;        # Size of the highlight (higher = smaller, sharper)
}
```

### Toon Shader

Creates a cel-shaded cartoon effect by quantizing colors and adding outlines.

```cfg
{
  type = "toon";
  levels = 4;           # Number of color levels (higher = smoother)
  edgeThreshold = 0.2;  # Threshold for outline detection (lower = thicker outlines)
}
```

### Fresnel Shader

Creates a rim-lighting effect that highlights the edges of objects.

```cfg
{
  type = "fresnel";
  fresnelPower = 2.0;  # Controls the width of the rim effect
  rimColor : {         # Color for the highlighted edges
    r = 1.0;
    g = 0.8;
    b = 0.2;
  };
}
```

## Combining Shaders (Decorator Pattern)

You can combine multiple shader effects using the decorator pattern. For example, to add a fresnel effect on top of a phong shader:

```cfg
{
  type = "fresnel-decorator";
  fresnelPower = 2.0;
  rimColor : {
    r = 0.2;
    g = 0.6;
    b = 1.0;
  };
  baseShader : {
    type = "phong";
    specularStrength = 0.6;
    shininess = 32.0;
  };
}
```

## Order of Shader Application

When multiple shaders are specified in the `shaders` list, they are applied in the order they appear. Each shader takes the output of the previous shader as its input.

## Example Scene

See `scenes/shader_showcase.cfg` for a complete example demonstrating different shader types.