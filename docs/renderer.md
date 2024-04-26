# Renderer
The Renderer is a class whose responsibility is to take
game objects and convert them into 3D graphics on the screen.
It uses Qt's wrapper of the OpenGL graphics api to do so. Because of this,
much of its design is based around restrictions presented by how OpenGL and 
graphics hardware function. For one example, OpenGL sets the coordinate system of
+Y being vertically up, so this was not a decision we got to make.


The public interface only has three methods
```c++
// queues an object to be drawn on the next frame 
void drawObject(GameObject* object);

// called when all objects desired to be drawn are queued 
void doneWithFrame();

//change what camera the renderer is using
void setCameraMode(Renderer::CameraMode mode);
```

For quick code tweaks, there's a large block of `constexpr` variables at the bottom
of the header's private region. These control various parameters the renderer
uses to draw the scene, like how dense the grass is or where
various cameras are. They all have comments explaining their function, along
with descriptive names.

### Internal Details
The rough flow of the rendering process goes like this:

On startup, the renderer reads from the assets directory, looking
specifically in models, textures, and cubemaps. It loads all valid assets
it can find in those directories, and immediately constructs GPU buffers
to store those assets, and maintains handles to them. It does this via the three
utility classes listed below, which help manage the lifecycle of GPU assets.

The reason it loads everything on startup is it greatly simplifies lifetime
management for assets. A more complex game engine would load assets on demand,
but this would necessitate much more difficult logic to implement. The tradeoff
is that this approach uses significantly more video RAM, though for the scale
of game this is, this was deemed acceptable.

Then during the rendering phase, the following loop occurs

1. Objects are queued in the frame queue by the Game
2. When the frame is complete, that is set as the 
back frame, while the front frame is emptied for further objects
3. When Qt triggers the paintGL method to draw a frame, it
   1. Updates the camera
   2. Draws the ground
   3. Loops over all the objects queued for the frame and draws them
   4. Finally, draws the skybox (this is done last, to minimize overdraw - or pixels drawn to 2+ times)

The reason it queues draw commands is out of an abundance of caution. In many
graphics APIs, drawing is done when the GPU/monitor say so, and so you may
end up with half a frame drawn if you're not careful. Because of how Qt works,
this actually may not be possible, but either way, it doesn't really hurt.

Finally, when the renderer is destroyed, it clears all of its stored buffers
of utility classes, which handles cleaning up any GPU resources.

For all of the linear algebra needs of the renderer, the third party library
GLM was used.

### Other Notes
* The `textureExists` methods were written because earlier C++ versions did not have `map.contains`
* The draw methods (drawPlayerTank, drawSkybox, etc) were split up amongst the object types for clarity
* `specialCaseAdjustment` is where you put anything special that needs tweaked for an indivual asset

## Texture
Texture is a lightweight wrapper around an OpenGL texture's handle and type.
It stores the GLint handle to the texture data, and the GLenum texture type
(i.e. whether it's a regular image texture or a cubemap). The class can't
be copied, but it can be moved with `std::move`

Regular textures need only be an image file, but cubemaps are more complicated.
They have to be a directory of six images, specifically titled:
* front
* back
* left
* right
* top
* bottom
* (optional) meta.json

The meta.json file describes how to flip the textures, if necessary. See the existing
meta.json in the bluecloud cubemap for an example.

Lastly, as with all the utility classes, its destructor cleans up the texture data on the GPU.

## Shader
Shader manages three aspects of shaders, which is compilation, uniforms, and cleanup.
As with all the renderer utility classes, it is movable, but not copyable.

The static method `fromSource` will compile a shader from passed in shader source code,
and the static method `fromFile` will compile a shader from a path to two files. If compilation
fails, an exception will be thrown with the error message from the shader compiler in it.

During runtime, it can be used to query what uniforms (or variables) that a shader has
that can be set from the CPU side. These uniforms allow controlling the behavior
of the shader while drawing things. The simplest method to use here is `setUniformIf`
which will update the value of a uniform only if that shader has that uniform.

This process uses a template to automatically deduce the type of the parameter and call
the right GL function. In the event it can't do that, it throws an `std::logic_error`,
which if this happens to you, you'll either have to change the type you're passing, or
adjust the type deduction of `Shader::setUniform`. It's done this way because this creates
a very nice interface where you have a single function you call with the name of a uniform,
and any value, and it mostly just "figures out" how to set that uniform with that value.

Similarly, textures can be bound to any sampler uniform of the shader, setting which
texture to use when drawing.

In this case, all of the shaders I wrote are stored directly in the code as strings,
though there is now sufficient infrastructure to load them from disk if desired.

Lastly, as with all the utility classes, its destructor cleans up the shader program.

## Mesh
Mesh handles loading 3D mesh files from disk and uploading the data to the GPU. It uses
the third party library Assimp to do so. From there, it interleaves the vertex data and
copies the indexes of those vertices into a set of buffers, before copying those buffers
to the GPU. This results in the following parameters stored in Mesh.

1. vbo - Vertex Buffer Object
   1. The VBO is responsible for storing the raw vertex data. Basically just a big lump of floats
      that describes things like each vertex's position in space, which direction its facing, how to
      map textures to it, etc.
2. ebo - Element Buffer Object
   1. The EBO holds the indices of the vertices to draw. In other words, when a mesh is drawn, the
      GPU will look at these indices, which are indices into the list of vertices, to construct the
      triangles that make up the mesh
3. vao - Vertex Array Object
   1. The VAO is a book keeping object. It essentially describes to the GPU what sorts of data is
      in the VBO, so it knows how to pass that data to the shader. It also associates the EBO/VBO
      together, so the end result is drawing the mesh is just binding the VAO, then drawing
4. Index Count
   1. Just the number of indices that the mesh has. Used during the draw call.

It has one method: `draw()`. It executes a draw call. The correct shader must be bound
and have its uniforms set first.

Initially, meshes did not use index-based rendering, and only stored vertex buffers. However,
it was difficult to get the triangle order correct under this approach, so meshes tended to appear
"exploded". Switching to an EBO based method fixed this issue.

Lastly, as with all the utility classes, its destructor cleans up all of its buffers.
