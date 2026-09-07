# Modern OpenGL migration plan for Ivf++

Status of branch `modern-gl` as of 2026-09-03, and the work remaining to reach a
core-profile-capable Ivf++ that still runs ObjectiveFrame unchanged on the
fixed-function path.

---

## 1. Where the branch stands

### 1.1 The architecture already in place

The branch uses a **mirror-mode** design: every legacy call site keeps its
fixed-function code and additionally writes the same information into a new
modern-side context. Drawing code then branches on whether a shader is live.

| Piece | File | Role |
| --- | --- | --- |
| `RenderContext` | `include/ivf/RenderContext.h` | Singleton: model matrix stack, `LightData[8]`, active `ShaderProgram`, global ambient, texture flag |
| `rc.h` | `include/ivf/rc.h` | Free-function facade (`rcPushMatrix`, `rcAddLight`, `rcIsShaderActive`, ...) so call sites stay terse |
| `ShaderProgram` | `src/ivf/ShaderProgram.cpp` | Compile/link plus cached uniform setters |
| `BlinnPhongShader` | `src/ivf/BlinnPhongShader.cpp` | GLSL 3.30 core, embedded as strings, mirrored in `data/shaders/` |
| `MeshBuffer` / `MeshData` | `src/ivf/MeshBuffer.cpp` | Reusable interleaved VBO/VAO container |
| `GLPrimitive::buildAndDrawVAO` | `src/ivf/GLPrimitive.cpp` | Packs the indexed coord/normal/tex/color sets into one interleaved VBO; triangulates `GL_QUADS` / `GL_QUAD_STRIP` |
| `RenderContext::drawUnlit` | `src/ivf/RenderContext.cpp` | One-shot helper geometry (cursors, grids, rulers) |

**Vertex attribute contract** (fixed — honour it everywhere):
`0 = vec3 position`, `1 = vec3 normal`, `2 = vec2 texcoord`, `3 = vec4 color`.

**Uniform contract:** `uModel`, `uView`, `uProjection`, `uNormalMatrix`,
`uMatAmbient/Diffuse/Specular/Emission/Shininess`, `uGlobalAmbient`,
`uLightCount`, `uLights[8]`, `uUseTexture`, `uTexture`, `uUnlit`,
`uUseVertexColor`.

**Dual-write sites already done:** `Camera::projectionTransform/viewTransform`
(also `rcSetProjection`/`rcSetView`), `Shape::doBeginTransform`/`doEndTransform`,
`Light::doCreateGeometry` (`glLight*` plus `rcAddLight`), `Material`
(`glMaterialfv` plus `uploadToShader`), `GlutBase`/`FltkBase::doRender`
(`rcBeginFrame`).

**Geometry converted:** `Sphere`, `Cone`, `ExtrArrow`, `SweptExtrusion`,
`SweptSolidLine`, `PathFrames`, `ExtrusionBuilder`, `ExtrusionProfile`, plus
everything routed through `GLPrimitive::buildAndDrawVAO` (`TriSet`,
`TriStripSet`, `QuadSet`, `QuadStripSet`, `PolySet`, `LineSet`, `LineStripSet`,
`PointSet`, and by inheritance `Brick`, `QuadPlane`, ...), and partial
`drawUnlit` conversions in `Cursor`, `Ruler`, `Workspace`.

### 1.2 The nine gaps

**A. The "modern" path is not core-profile-clean — it is shaders layered on a
compatibility context.** This is the single most important finding.

- 24 files in `src/ivf` still call `glPushAttrib`/`glPopAttrib`; 53 sites touch
  `GL_LIGHTING` / `GL_COLOR_MATERIAL`.
- `Shape::doBeginTransform` calls `glPushMatrix`/`glTranslated`/`glRotated`
  unconditionally and *then* mirrors into `RenderContext`.
- `Camera` still drives `gluPerspective`/`gluLookAt` and the fixed-function
  matrix stack in both modes.
- The new validation examples themselves ask for `GLFW_OPENGL_COMPAT_PROFILE`
  (`sphere_test.cpp:533`, `swept_test.cpp:1184`, `meshbuffer_test.cpp:629`).

So there are two *drawing* paths but still only one *context* profile. The goal
of the remaining work is to make the modern path survive on a real core context,
without taking the legacy path away.

**B. No explicit profile switch.** Path selection is a side effect of
`rcIsShaderActive()`. ObjectiveFrame cannot ask for the legacy path, cannot query
which path is live, and there is no single place to assert "no legacy call may
run from here on".

**C. Geometry still legacy-only.** `FaceSet`, `SimpleLineSet`, `VertexElements`,
`Mesh`, `Grid`, `ConstructionPlane`, `SelectionBox`, `WireBrick`, `Billboard`,
the gle-based `Extrusion`/`SolidLine`/`TubeExtrusion`/`SplineTube`, all of
`src/ivfgle`, all of `src/ivffont`, and `src/ivf3dui`.

**D. Picking uses `GL_SELECT`.** `BufferSelection.cpp:51-74`,
`SelectComposite.cpp:89-116` and `SelectOrtho` use `glSelectBuffer` /
`glRenderMode(GL_SELECT)` / `glPushName` and `gluPickMatrix` — all removed in
core. ObjectiveFrame depends on `Scene::pick()`, `Workspace::pick()` and
`getSelectedShapes()`, but only on that *API*, so the implementation can be
swapped underneath.

**E. Display lists conflict with the shader path.** `GLBase::compileList()` wraps
`doCreateGeometry()` in `glNewList(GL_COMPILE)`. Uniform sets execute at compile
time rather than being recorded, so a replayed list draws with stale matrices.
`m_useList` defaults to `false` (`GLBase.cpp:40`), so this is latent — but
`setUselist(true)` plus an active shader silently produces wrong output.

**F. `Camera`'s modern matrices are incomplete.** `glmProjectionMatrix()` only
implements `glm::perspective`. Orthographic (`OrthoCamera`, `SelectOrtho`), tiled
and jittered frustums (`tilePerspective`, `accPerspective`), and the asymmetric
stereo frustums are not mirrored.

**G. `drawUnlit` allocates and destroys a VAO+VBO on every call, every frame.**
Acceptable for a cursor; wasteful for `Ruler` and `Workspace` grids.

**H. Fixed-function features with no shader equivalent yet:** fog (`Fog.cpp`),
alpha test, two-sided lighting, `GL_COLOR_MATERIAL`, texture env modes (`Texture`
defaults to `GL_DECAL`), line stipple, point size.

**I. ObjectiveFrame's own constraints** — these gate the sequencing:

- It renders ImGui through `imgui_impl_opengl2` (`IvfViewWindow.cpp:12`), a
  compatibility-only backend. **This must move to `imgui_impl_opengl3` before any
  core context is possible.**
- It has its own fixed-function drawing: `FemView.cpp:4985` (`glBegin(GL_QUADS)`),
  `Area2D.cpp:81`, `IvfViewWindow.cpp:213/231/267`, `ofui/view_window.cpp:197-201`.
- It consumes `ivfgle` (`GleSpiralCylinder`), `BitmapFont`/`TextLabel`, `Texture`
  plus `SgiImage`/`PngImage`, `Fog`, `CulledScene`, `SelectOrtho`.
- It is already partly migrated: `IvfViewWindow.cpp:178` calls `rcBeginFrame()`.

---

## 2. Target architecture

### 2.1 An explicit, three-valued profile

```cpp
// include/ivf/RenderContext.h
enum class RenderProfile {
    Legacy,  // fixed function only, no shader. Compatibility context required.
    Mixed,   // shader drawing on a compatibility context. Today's behaviour. Default.
    Core     // shader only. Every legacy GL call suppressed.
};

class RenderContext {
    void          setProfile(RenderProfile p);
    RenderProfile profile() const;
    bool          legacyAllowed() const;     // profile != Core
    bool          shaderPathActive() const;  // profile != Legacy && shader && shader->isLinked()
};
```

`rcIsShaderActive()` keeps its name and its meaning but is redefined in terms of
`shaderPathActive()`, so every existing call site keeps working. `Legacy` then
becomes a requestable mode rather than "the shader happened not to link".

### 2.2 A legacy-call shim, so suppression is mechanical

Add `include/ivf/LegacyGL.h` with one inline wrapper per fixed-function call the
library uses, each a no-op when `profile() == Core`:

```cpp
inline void lgPushAttrib(GLbitfield m) { if (rcLegacyAllowed()) glPushAttrib(m); }
inline void lgPopAttrib()              { if (rcLegacyAllowed()) glPopAttrib(); }
inline void lgEnable(GLenum c)         { if (rcLegacyAllowed()) glEnable(c); }
inline void lgMaterialfv(GLenum f, GLenum p, const GLfloat* v) { ... }
// lgPushMatrix, lgTranslated, lgRotated, lgScaled
// lgBegin, lgEnd, lgVertex3dv, lgNormal3dv, lgColor4fv, lgTexCoord2dv
```

The conversion is then a near-mechanical `gl...` to `lg...` rename inside
`src/ivf`, `src/ivfgle`, `src/ivffont` and `src/ivf3dui`, and it gives one
auditable place to answer "what still needs compatibility?". Calls that *are*
legal in core (`glEnable(GL_DEPTH_TEST)`, `glLineWidth`, `glBlendFunc`,
`glPolygonMode`) stay as raw `gl...` and act as the whitelist.

Pair it with a small explicit state stack in `RenderContext` replacing the
`glPushAttrib`/`glPopAttrib` pairs that actually matter (blend enable and func,
depth mask, cull face, line width, polygon mode) using core-legal calls.

### 2.3 What the fallback guarantees

- `RenderProfile::Legacy` reproduces master's behaviour exactly. No shader is
  created, no VAO is built, `rcIsShaderActive()` is false everywhere, and the
  existing legacy branches run.
- `RenderProfile::Mixed` is the default, so nothing that builds today changes
  behaviour.
- `RenderProfile::Core` is opt-in and, once the work below is complete, produces
  zero `glGetError()` on a 3.3 core context.

---

## 3. Phased work

### Phase 0 — Baseline and guardrails — **done**

1. ✅ `modern-gl` builds clean in Debug and Release.
2. ✅ `ivf::enableDebugOutput()` — `include/ivf/GLDebug.h`, `src/ivf/GLDebug.cpp`.
   A KHR_debug / ARB_debug_output callback plus `checkGLError()`,
   `clearGLErrors()` and per-severity `debugMessageCount()`. The vendored glad
   loader is OpenGL 3.2 compatibility with no extensions, so it declares neither
   debug extension; the two entry points are resolved from the GL library at
   runtime rather than regenerating glad and touching every translation unit.
3. ✅ `examples/profile_test/` — 31 cases covering the public shape classes,
   each rendered in isolation against a fresh frame so driver complaints are
   attributed to one class. `profile_test [legacy|mixed|core] [--headless]
   [--list] [--only NAME] [--notify] [--shot DIR]`; exit 0 when every case is
   clean. Errors fail a case; Low and Medium driver messages are counted and
   shown but do not fail, because drivers report a great deal at those levels
   that is advisory rather than wrong.
4. ✅ `--shot DIR` plus `examples/profile_test/compare_shots.py` — a **visual**
   gate alongside the error gate. `--shot` renders each case alone from a fixed
   camera to `DIR/<profile>_<case>.bmp`; the script diffs two profiles and
   reports mean absolute difference, the fraction of pixels that moved, and the
   lit-pixel count on each side.

   This second gate turned out to matter more than the first. See §3.1.

#### Baseline results (NVIDIA 591.86, GL 4.6 compat / 3.3 core)

| Profile | Errors | Renders like `legacy`? |
| --- | --- | --- |
| `legacy` | 31/31 clean | — (reference) |
| `mixed` | 31/31 clean, every case warns once | **no — see §3.1** |
| `core` | 32/32 failing (31 cases plus frame setup) | no |

**Two real bugs surfaced immediately and are fixed:**

- `Ruler::doCreateGeometry` ended with `glLineWidth(0.0)` — zero is not a legal
  line width, so every ruler drawn on the legacy path raised `GL_INVALID_VALUE`.
  Present in master. Now `glLineWidth(1.0)`.
- `Lighting::~Lighting()` called `delete` on lights it had only taken a
  reference to. `getLight()` hands out a pointer callers are expected to keep,
  so an application holding a `LightPtr` got a double free at static destruction
  — intermittent heap corruption at process exit with no useful stack. Now drops
  its reference and deletes only when nothing else holds one.

**The one warning in `mixed`** is the same message in every case: the
Blinn-Phong fragment shader declares `uTexture` and the driver sees texture unit
0 bound to object 0 with no base level, even when `uUseTexture` is false. Benign,
but it should be cleaned up when textures are addressed in Phase 6.

**The `core` failures rank the remaining work**, by driver error count:

| Case | Errors | Case | Errors |
| --- | --- | --- | --- |
| Grid | 252 | Cursor | 17 |
| Extrusion, TubeExtrusion | 119 each | (frame setup) | 13 |
| SolidLine | 62 | Arrow | 10 |
| Axis | 43 | Node | 7 |
| Composite | 14 | most converted classes | 3–5 |

Aggregated across the run, the offending calls are:

| Call | Count | Where |
| --- | --- | --- |
| `glVertex3dv` | 238 | unconverted geometry |
| `glColor3fv` / `glColor4fv` | 136 | unconverted geometry |
| `glNormal3dv` | 130 | unconverted geometry |
| `glLightfv` / `glLighti` / `glLightf` | 256 | `Light::doCreateGeometry` |
| `glPushMatrix` / `glPopMatrix` | 178 | `Shape::doBeginTransform`/`doEndTransform` |
| `glLoadName` | 75 | `Shape::doBeginTransform` |
| `glTranslated` / `glMultMatrixf` | 77 | `Shape::doBeginTransform` |

The 3–5 errors charged to every already-converted class are the `Shape`
transform pair and `glLoadName` — which is why Phase 2 starts with `Shape` and
`Light` rather than with any individual geometry class. Fixing those two files
alone should take most cases from 5 errors to 0.

**New finding — `glLoadName` is on the draw path.** `Shape::doBeginTransform`
(`Shape.cpp:213`) emits a selection name on *every* render, not only during a
picking pass. Gap D is therefore not confined to `BufferSelection` and
`SelectComposite`: the `GL_SELECT` machinery has to be suppressed in `Core` for
ordinary drawing too. Phase 2 handles it via the `lg*` shim; Phase 5 replaces
what it was for.

### 3.1 `mixed` does not render like `legacy` — the finding that reshapes the plan

A clean error count says nothing about whether the picture is right. Comparing
the two profiles pixel by pixel found four distinct defects, and one of them
invalidates the assumption the plan was built on.

**The structural one — `mixed` has no fallback.** The plan described `mixed` as
"modern where converted, legacy where not". It is not. Once `rcUseBlinnPhong()`
binds a program, *every* draw goes through that program, including the untouched
`glBegin`/`glVertex3dv` bodies of classes with no modern path. Those bodies never
call `rcUpdateShader()`, so the shader runs with whatever `uModel`, `uView` and
`uProjection` some earlier object happened to leave in it — and in compatibility
mode the driver aliases `glVertex`/`glNormal`/`glColor` onto generic attribute
slots that do not match this shader's layout.

Rendered alone, `Grid`, `Extrusion`, `TubeExtrusion` and `SolidLine` draw
**nothing at all** under `mixed`: the uniforms are still zero matrices, so every
vertex collapses to a point. Rendered after other objects they draw something
*plausible but wrong* — the grid came out yellow, a bent extrusion came out as a
short straight stub — purely from inherited state. So `mixed` is not "modern
where converted, legacy where not"; it is **modern where converted, undefined
where not**, and the result depends on scene order.

This is why Phase 1 cannot be deferred. Suppressing legacy calls in `Core` was
only half the job: the profile mechanism must also be able to **unbind the shader
around a draw that has no modern path**, so `mixed` degrades to real
fixed-function rendering instead of to undefined behaviour. Concretely, `Shape`
should bracket `doCreateGeometry()` with a "does this object have a modern path?"
query, and drop to program 0 when the answer is no.

**Three smaller defects, all fixed:**

| | Defect | Fix |
| --- | --- | --- |
| D1 | No default material. `Shape::doCreateMaterial()` uploads nothing when a shape has no material — matching legacy, where GL's material state is simply sticky. But the uniforms started zero-initialised rather than at GL's defaults, so the first unmaterialled object drew **black** and every later one took the colour of whatever preceded it. `WireBrick`, `LineStripSet` and `Arrow` were invisible. | `RenderContext::applyDefaultMaterial()` seeds `uMat*` with GL's fixed-function defaults whenever the active shader changes. |
| D2 | Per-primitive state skipped. `PointSet` set `glPointSize` and `LineSet`/`LineStripSet` set `glLineWidth` *after* `if (buildAndDrawVAO(...)) return;`, so the modern path never applied them — points drew 1px instead of 6, lines 1px instead of 2. | State is applied before the early return and restored on the way out. Both calls are core-legal. |
| D3 | Vertex-colour semantics. Legacy `glColorMaterial` defaults to `GL_AMBIENT_AND_DIFFUSE`; `uUseVertexColor` replaced diffuse only, leaving vertex-coloured geometry visibly duller. | The shader now substitutes the vertex colour for ambient as well. |

After those fixes `PointSet` and `LineSet` are **pixel-identical** between the
two profiles, and `QuadSet+colors` dropped from a mean absolute difference of
7.53 to 1.28.

**What is still different, and expected to be:**

- `Sphere`, `Cone`, `Cylinder`, `QuadSet+colors` — 3–4% of pixels move. Legacy
  shades per vertex (Gouraud); the shader shades per fragment. This is a
  deliberate improvement, not a regression, but it means pixel equality is the
  wrong acceptance criterion for lit surfaces. Gate on lit-pixel **coverage**
  and on mean absolute difference, not on exact equality.
- `WireBrick` / `SelectionBox` — `glPolygonMode(GL_LINE)` over quads that
  `buildAndDrawVAO` has triangulated, so the triangulation diagonals show as
  extra edges. Phase 4 needs an edge-list path for wireframe quads.
- `ExtrArrow` — proportions differ from `Arrow`; needs checking in Phase 4.

### Phase 1 — Make the switch explicit — **done**

1. ✅ `RenderProfile { Legacy, Mixed, Core }` on `RenderContext`, with
   `setProfile`/`profile`/`legacyAllowed`/`shaderPathActive`/`needsLegacyDraw`,
   and `rcSetProfile`/`rcProfile`/`rcLegacyAllowed`/`rcUnuseShader` in `rc.h`.
2. ✅ `rcIsShaderActive()` now reports `shaderPathActive()`, so `Legacy` turns
   the whole library back into its fixed-function self without unlinking
   anything.
3. ✅ `GlutBase`/`FltkBase::setRenderProfile()` and `renderProfile()`;
   `disableBlinnPhongShader()` also selects `Legacy`.
4. ✅ **Fix E:** `GLBase::useDisplayList()` returns false whenever the shader
   path is active. Display lists are documented as a legacy-profile feature.
5. ✅ `include/ivf/LegacyGL.h` — the `lg*` shim. Nothing calls it yet; Phase 2
   converts the library file by file. Calls that stay legal in core deliberately
   have no wrapper, so the header doubles as the whitelist's complement.
6. ✅ **The fallback.** `GLBase::hasModernPath()` is virtual, defaults to false,
   and is overridden true on the converted classes, on containers that only
   render children, and on the state objects (`Material`, `Texture`, `View`).
   `GLBase::renderImmediate()` brackets an object that answers false with
   `glUseProgram(0)` … `rcUseShader()`, so its fixed-function code reaches the
   fixed-function pipeline.

   Defaulting to false is deliberate: a missed override costs performance, not
   correctness — the object simply renders the old way.

#### What this took beyond the sketch

Three things the design did not anticipate, each found by the gates:

- **`Material` derives from `GLBase`.** With the default of false it got
  bracketed like geometry, so `Material::doCreateMaterial()` skipped its
  uniform upload and every lit object rendered with the default material. State
  objects have to answer true. `Texture` is the same case.
- **Unbinding the program is not enough.** `rcIsShaderActive()` still reported
  true inside the bracket, so `Material` uploaded uniforms with no program bound
  — 140 `GL_INVALID_OPERATION`s per run. `RenderContext::beginLegacyDraw()` /
  `endLegacyDraw()` make `shaderPathActive()` report false for the duration,
  which is simply the truth for the object being drawn.
- **`beginFrame()` re-bound the shader every frame**, and `drawUnlit()` checked
  only whether a shader was linked, never the profile. Between them, selecting
  `Legacy` did almost nothing once a shader existed. Both now respect the
  profile, and `beginFrame()` actively unbinds when the profile does not want a
  shader — which is what makes the switch work on its own, without the caller
  remembering to unbind.

#### Results after Phase 1

`profile_test` gained a fourth mode, `legacy-shader`: build and link the shader,
then select `RenderProfile::Legacy` anyway. That is exactly ObjectiveFrame's
position, and it must be indistinguishable from never having built one.

| Comparison | Result |
| --- | --- |
| `legacy` vs `legacy-shader` | **all 31 cases pixel-identical** (max channel difference 0) |
| `legacy` vs `mixed` | 0 coverage problems; residual differences are shading-model only |
| error gate, `legacy` / `legacy-shader` / `mixed` | 31/31 clean |
| error gate, `core` | 32/32 failing, unchanged — that is Phase 2's job |

The four classes that used to draw **nothing** under `mixed` — `Grid`,
`Extrusion`, `TubeExtrusion`, `SolidLine` — are now pixel-identical to legacy.

Two conversion bugs were fixed on the way:

- `ExtrArrow` swept all five gaps between its six control points. `glePolyCone()`
  treats the first and last as phantom endpoints that only set the angle of the
  end cuts, so the modern path was drawing two segments of geometry gle never
  draws, and the arrow came out a unit too long at each end.
- `WireBrick` (and `SelectionBox`) now answer `hasModernPath() == false` on
  purpose. The inherited path triangulates the quads, and under
  `glPolygonMode(GL_LINE)` every diagonal showed as an extra edge. Saying "no
  modern path" is the honest answer until `GLPrimitive` grows an edge-list path
  for wireframe quads; Phase 4 flips it back.

What still differs between `legacy` and `mixed`, and should:
`QuadSet+colors` 14.5% of pixels (mean 1.28), `Cone` 3.7%, `Sphere` 3.5` — all
per-vertex versus per-fragment shading. `ExtrArrow` 1.7%, because its modern path
draws unlit; worth revisiting in Phase 4.

*Exit criterion met: both gates pass for `legacy`, `legacy-shader` and `mixed`,
with every case rendered in isolation.*

### Phase 2 — Core-clean the shared path — **done**

1. ✅ `Shape::doBeginTransform`/`doEndTransform` — matrix calls, `glLoadName`,
   `GL_NORMALIZE` and the texture enables all through `lg*`.
2. ✅ `Material::doCreateMaterial` — the `glMaterialfv` calls, and the
   `glIsEnabled(GL_LIGHTING)` gate itself, which raises `GL_INVALID_ENUM` in core
   and so had to be checked behind `rcLegacyAllowed()` rather than inside.
3. ✅ `Light` / `Lighting` — `glLight*`, `glLightModel*`, `GL_LIGHTING`.
4. ✅ `GLPrimitive` subclasses — the `glPushAttrib`/`GL_COLOR_MATERIAL` preamble
   and the whole `glBegin` body, across `TriSet`, `TriStripSet`, `QuadSet`,
   `QuadStripSet`, `PolySet`, `LineSet`, `LineStripSet`, `PointSet`, `FaceSet`,
   `SimpleLineSet`, `Mesh`, `Brick`, `SelectionBox`.
5. ✅ `Blending`, `BlendState`, `CullState`, `CompositeState`, plus `Cursor`,
   `Ruler`, `Workspace`, `Cone`, `Billboard`, `Extrusion`, `TextLabel`,
   `Texture`, `SceneBase`, and the `GL_SELECT` users (`SelectComposite`,
   `BufferSelection`, `ShapeSelection`, `SelectOrtho`) — the last of which now
   go quiet in core rather than erroring, pending Phase 5.
6. ✅ **Fix G:** `drawUnlit` keeps one VAO and buffer and orphans-then-refills
   instead of generating and deleting a pair per call per frame.

Also added: in `Core`, `GLBase::renderImmediate()` skips `doCreateGeometry()`
for an object with no modern path (`rcCanDrawGeometry()`). Its geometry code is
all `lg*` no-ops by then, so calling it achieves nothing; skipping says so
plainly and keeps the report about coverage rather than about a flood of errors
from calls that were going to be ignored anyway.

### Phase 3 — Camera completeness — **done**

`glmProjectionMatrix()` now mirrors everything `projectionTransform()` can do,
not just the plain perspective case:

- Tiled rendering (`tilePerspective`) and accumulation-buffer jitter
  (`accPerspective`/`accFrustum`), both via `glm::frustum` — jitter reads the
  viewport to convert its pixel offset, exactly as `accFrustum()` does.
- Stereo, which shears the frustum sideways by half an eye separation scaled by
  the near/far ratio. Previously only the eye *position* was mirrored, so the
  shader and fixed-function paths disagreed on the frustum shape.
- `glmPickMatrix()` added as groundwork for Phase 5.
- `gluPerspective` / `gluLookAt` / `glFrustum` / `glOrtho` / `gluPickMatrix` /
  `glMatrixMode` / `glLoadIdentity` routed through `lg*` in `Camera`, `View` and
  `SelectOrtho`.

`glm::ortho` turned out not to be needed: `OrthoCamera` is an empty stub that
never overrides `projectionTransform()`. The only real orthographic projection in
the library is in `SelectOrtho`, which belongs to the picking work in Phase 5.

#### Results after Phases 2 and 3

| Profile | Errors | Draws like `legacy`? |
| --- | --- | --- |
| `legacy` | 31/31 clean | reference |
| `legacy-shader` | 31/31 clean | **pixel-identical** |
| `mixed` | 31/31 clean | 0 coverage problems |
| `core` | **31/31 clean** — was 32/32 failing | 23/31 cases draw correctly |

The library now runs on a genuine OpenGL 3.3 core forward-compatible context
with **zero GL errors**. Three files — `Shape`, `Material`, `Light` — accounted
for three quarters of that on their own, exactly as the Phase 0 measurement
predicted.

What `core` still cannot draw, and why:

| Case | Reason |
| --- | --- |
| `Extrusion`, `TubeExtrusion`, `SolidLine` | gle-based; no modern path yet (Phase 4.1) |
| `Grid` | its `LineSet`s carry per-index line widths, which force the legacy branch |
| `WireBrick`, `SelectionBox` | deliberately marked legacy-only until wireframe quads get an edge-list path |
| `LineSet`, `LineStripSet` | draw, but thinner — see below |

**A core-profile limitation worth knowing:** a forward-compatible core context
accepts only `glLineWidth(1.0)` and raises `GL_INVALID_VALUE` for anything wider.
`lgLineWidth()` clamps rather than suppresses, so wide lines still draw, just
thin — which is why `LineSet` covers 488 pixels in legacy and 244 in core. Real
wide lines in core have to be built from triangles; that belongs with the other
shader-side feature parity in Phase 7.

### Phase 4 — Remaining geometry — **mostly done**

Done so far:

- ✅ **`Extrusion`, `TubeExtrusion`, `SolidLine`** — retargeted onto
  `SweptExtrusion` rather than reimplementing the sweep. `Extrusion` keeps a
  `SweptExtrusion`, feeds it the same section and spine on demand, and calls its
  new `drawGeometry()` / `drawSelectGeometry()`. `setSpineScale()` turned out to
  be the only public writer of the gle section transform, and it only ever writes
  a diagonal, so the mapping between the two APIs is complete.
- ✅ **`WireBrick` / `SelectionBox`** — `buildAndDrawVAO()` gained a wireframe
  mode that emits real face edges. Triangulating and then setting
  `glPolygonMode(GL_LINE)` drew every triangulation diagonal, so a box came out
  with its faces crossed out.
- ✅ **`Grid`** — its `LineSet`s carry per-index line widths, which used to force
  the whole class onto the legacy path. `buildAndDrawVAO()` now records the
  vertex range of each coordinate index set and issues one draw call per set, so
  each can carry its own width.
- ✅ **`SimpleLineSet`** — drawn as unlit line segments through `drawUnlit()`.
- ⬜ **`FaceSet`, `Mesh`, `VertexElements`** — measured and still unconverted.
  They render correctly in `mixed` (the fallback handles them) and draw nothing
  in `core`. Each keeps its own vertex data rather than using `GLPrimitive`, so
  each needs its own `MeshBuffer` path.
- ⬜ **`src/ivfgle`**, **`src/ivf3dui`** — not started.

#### Two real bugs found on the way

**`SweptExtrusion` pinched at every mitred corner.** `swept_test` reported IoU
1.0000 on every case, but every one of those cases used a *straight* spine — the
entire join-style path was dead code under test. Adding a bent-spine case
immediately showed IoU 0.934.

The cause: gle mitres by sliding each contour point along the segment axis until
it meets the bisecting plane (`ex_angle.c`, the `INNERSECT` calls), which keeps
the cross-section measured perpendicular to each leg exactly equal to the
profile. `PathFrames` instead placed the profile *in* the bisecting plane
unchanged, which foreshortens it by cos(a) and pinches the tube. `PathFrame`
gained a `mitre` matrix — a pure stretch by 1/cos(a) along the one profile axis
lying in the plane of the bend — applied in `ExtrusionBuilder::placePoint()` and
`worldNormal()`. Both bent cases now read **IoU 1.0000**, and every straight case
is unchanged.

Worth noting how it was nearly missed twice: computing the stretch inside
`buildPolylineStations()` produced byte-identical output, because `normal` and
`binormal` are not assigned until `assignFrameOrientations()` runs and the
tangent is not normalised until after that either. It has to be a post-pass.

**`SimpleLineSet` could wedge the GL context.** It opened
`glBegin(GL_LINE_STRIP)` and only closed it on meeting a `-1` terminator in the
index array. An index array without one — which nothing documents as required —
left the primitive open, and from that point every call in the frame failed with
"invalid from the current immediate mode state". In `mixed` it went on to crash
with an access violation. Present in master. It now tracks whether a strip is
open and closes it at the end.

#### Results

| Comparison | Result |
| --- | --- |
| error gate, all four profiles | 35/35 clean |
| `legacy` vs `legacy-shader` | pixel-identical |
| `legacy` vs `mixed` | **0 coverage problems across all 35 cases** |
| `legacy` vs `core` | 5 coverage problems, down from 8 |
| `swept_test` | 92 checks, 0 failures (was 84 checks; the 8 new ones are the bent-spine cases) |

All extrusion classes now cover exactly the same pixels as the gle originals
(1860 for the bent extrusions, 1976 for the solid lines) in every profile.

The five remaining `core` gaps are `FaceSet`, `Mesh` and `VertexElements`, which
have no modern path yet, plus `LineSet` and `LineStripSet`, which draw but thin
because of the core wide-line limit — Phase 7.

### Phase 4 — original plan *(for reference)*

Ordered by ObjectiveFrame's dependency list, so its cutover unblocks earliest:

1. **ObjectiveFrame-critical:** `Extrusion` / `SolidLine` / `TubeExtrusion` /
   `SplineTube` — retarget onto the gle-free `SweptExtrusion` /
   `ExtrusionBuilder` machinery already on this branch (check the recorded gle
   convention notes before touching normals and cap orientation). Then
   `Billboard`, `SelectionBox`, `Grid`, `ConstructionPlane`, `QuadPlane`.
2. **`src/ivfgle`:** the wrappers ObjectiveFrame uses (`GleSpiralCylinder` first)
   re-expressed on `PathFrames` plus `ExtrusionProfile`; the rest follow, and the
   vendored `src/gle` C library becomes deletable.
3. **Rest of `src/ivf`:** `FaceSet`, `SimpleLineSet`, `VertexElements`, `Mesh`,
   `WireBrick`, and finish the partial `Cursor` / `Ruler` / `Workspace`
   conversions — they still fall through to `glBegin` on some branches.
4. **`src/ivf3dui`:** `UIButton`, `UISwitch` and friends.

Each class: modern branch first, `if (...) return;` guard, legacy body untouched
below it. Add the class to `profile_test`.

### Phase 5 — Picking without `GL_SELECT` — **done**

- ✅ `PickShader` (`include/ivf/PickShader.h`) — a flat-colour program that writes
  `uPickColor` unchanged. No lighting, no texturing, no vertex colour: the value
  is an identifier travelling through a colour channel, not a colour.
- ✅ `RenderContext` gained `usePickShader()`, `setPickMode()`, `setPickName()`
  and `decodePickName()`. Names are encoded into 24 bits with zero reserved for
  background, so the value written is `name + 1`.
- ✅ `Shape::doBeginTransform()` reports the object name through `setPickName()`
  alongside its existing `glLoadName()`. Driving both from the same place is what
  makes the two implementations agree — including the inherited case, where a
  child with `setUseName(false)` keeps whatever its parent last set.
- ✅ `BufferSelection::pick()` switches to `pickColorId()` whenever the shader
  path is active: render the scene into an offscreen FBO, read back a 4×4 region
  around the cursor, decode the ids, and take the nearest by depth. It produces
  the same three answers as the `GL_SELECT` path — the shape list, the nearest
  shape, and a count.
- ✅ `profile_test --pick` sweeps a grid of sample points over a small scene and
  prints the shape picked at each as a character map.

**Deviation from the plan:** the replacement lives inside `BufferSelection`
rather than in a sibling `ColorPickSelection`. `SceneBase` holds a
`BufferSelectionPtr` and `Scene`/`Workspace` delegate straight to it, so putting
both implementations behind the same `pick()` means ObjectiveFrame gets working
picking in every profile with no plumbing change at all. `GL_SELECT` is still
used, unchanged, whenever the profile is `Legacy`.

#### The pick test checks against the rendered image, not against GL_SELECT

Comparing the two implementations to each other would only show they agree. The
test instead reads back what was actually drawn and asks whether every visible
sample picks something and no background sample does.

| Profile | visible but not picked | picked but not visible |
| --- | --- | --- |
| `legacy` (GL_SELECT) | **103** | 23 |
| `mixed` (colour id) | **0** | 15 |
| `core` (colour id) | **0** | 15 |

Colour-id picking turns out to be strictly *more* accurate than what it replaces:
every visible sample picks a shape. The `GL_SELECT` path missed 103 of 800
samples that plainly had geometry on them. The small "picked but not visible"
figure is the 4×4 region deliberately reaching slightly past a silhouette edge,
which is the click tolerance both implementations are meant to have.

**One more core violation found:** `Light::enable()` called `glEnable(GL_LIGHTn)`
directly — the earlier conversion of `Light.cpp` had only covered `glLight*`. It
escaped the error gate because it runs once during setup, before the per-case
measurement begins, so its error was drained rather than attributed. Now routed
through `lgEnableLegacy()`.

### Phase 5 — original plan *(for reference)*

Fix D. Keep `Scene::pick(x,y)`, `getSelectedShape()` and `getSelectedShapes()`
identical in signature and semantics; replace the implementation:

- New `ColorPickSelection` (sibling of `BufferSelection`): render the scene to an
  offscreen FBO with a flat-colour shader, one unique RGBA id per shape, then
  `glReadPixels` a 1x1 region (or a small rect, for multi-select).
- `SelectComposite` / `SelectOrtho` gain an id-assignment pass replacing
  `glPushName` / `glPopName`.
- `BufferSelection` stays and is used when `profile() == Legacy`, so behaviour is
  preserved exactly for anyone on the old path.
- Watch the depth-precision and `GL_MULTISAMPLE` pitfalls: the pick FBO must be
  single-sampled and must not dither.

### Phases 6 and 7 — Text, textures and fixed-function parity — **mostly done**

The two phases collapsed into one piece of work. `TextLabel` already builds
`QuadPlane` children carrying texture coordinates, so text was never a geometry
problem: what it needed was the texture environment and the alpha test, both of
which are Phase 7 items. Doing them together avoided writing a text-specific
path that the general one would then replace.

**The fragment shader gained the remaining fixed-function stages**, each mirrored
into `RenderContext` by the class that owns the legacy call:

| Feature | Uniforms | Fed by |
| --- | --- | --- |
| Texture environment | `uTextureMode`, `uTextureEnvColor` | `Texture::syncToRenderContext()` |
| `GL_TEXTURE` matrix | `uTextureMatrix` (mat3) | same |
| Fog | `uFogMode`, `uFogColor`, `uFogDensity`, `uFogStart`, `uFogEnd` | `Fog::syncToRenderContext()` |
| Two-sided lighting | `uTwoSided` | `Lighting::setTwoSide()` |
| Alpha test | `uAlphaTestFunc`, `uAlphaTestRef` → `discard` | `TextLabel::doPreGeometry()` |

Alpha handling had to move: the shader assigned `color.a = diffuseColor.a`
*after* texturing, which threw away the texture's alpha — the exact channel a
glyph atlas carries its shape in. Alpha now comes from the material first and
texturing modifies it, which is the order GL uses.

**Wide lines in core.** A forward-compatible core context accepts only
`glLineWidth(1.0)`, so wide lines are expanded into screen-facing quads. The
expansion happens in the vertex shader, because a line width is a screen
measurement and expanding before projection would give a width that shrank with
distance. No new vertex attributes were needed: a wide-line vertex reuses the
existing layout, with `aNormal` carrying the segment's other endpoint and
`aTexCoord` carrying (side, half width). Lines draw unlit, so both slots are free.

The half width is per index set, not per draw. Taking the widest of them — which
is how the decision to expand at all is made — drew `Grid`'s one-pixel rules four
pixels wide and put its coverage 60% over legacy.

**Two more pre-existing bugs:**

- `Fog::setDensity()` wrote `GL_FOG_END` rather than `GL_FOG_DENSITY`. Setting a
  density therefore did nothing to the exponential fog modes and quietly moved
  the far plane of the linear one instead. `Fog` also read its whole state back
  out of GL, with queries that are illegal in core; it now keeps its own.
- The Blinn-Phong shader declares a sampler, and drivers validate it whether or
  not the branch reading it is taken — NVIDIA warned about an incomplete texture
  on **every draw**. `RenderContext` now keeps a 1×1 white texture bound to unit
  0. That single change took the harness from "32 of 35 cases clean but warned"
  to **zero warnings in every profile**.

#### Results after Phases 6 and 7

| Profile | Errors | Warnings | Coverage vs `legacy` |
| --- | --- | --- | --- |
| `legacy` | 0/36 | 0 | reference |
| `legacy-shader` | 0/36 | 0 | pixel-identical |
| `mixed` | 0/36 | 0 | **0 coverage problems** |
| `core` | 0/36 | 0 | 3 problems, down from 5 |

`LineSet` (488 → 514 px), `LineStripSet` (745 → 841) and `Grid` (5300 → 5804)
now draw at the right width in core; all three were coverage failures before.

#### Still open

- **`FaceSet`, `Mesh`, `VertexElements`** draw nothing in core — Phase 4's
  remaining conversions, unchanged by this work.
- **`TexturedPlane` differs 12.5% between `legacy` and the shader path.** The
  texture itself is right — same coverage, same UVs, same checker — but legacy
  renders it at full texture brightness while the shader path modulates it with
  the material. The two disagree about the texture environment or about what
  lighting contributes, and it needs one more look before texturing can be called
  done.
- `src/ivffont`'s `RasterFont`, `PixmapFont` and `BitmapFont` still use
  `glBitmap`/`glDrawPixels`, which have no core equivalent. `TextLabel` does not
  depend on them, so this is lower priority than it looked.

### Phase 6 — original plan *(for reference)*

- `BitmapFont` / `TextLabel`: already quad-based — route through `MeshBuffer` and
  add a `uUnlit` plus `uUseTexture` text material.
- `src/ivffont/RasterFont`, `PixmapFont`, `BitmapFont`: `glBitmap` and
  `glDrawPixels` have no core equivalent. Replace with a texture-atlas glyph
  renderer; keep the raster implementations behind `RenderProfile::Legacy`.
- `Texture`: `GL_DECAL` / `GL_MODULATE` / `GL_BLEND` env modes become a
  `uTextureMode` uniform in the fragment shader; the texture matrix
  (`m_texScaleX/Y`, `m_texRotate`, `m_texTransX/Y`) becomes a `uTextureMatrix`
  `mat3`.

### Phase 7 — Fixed-function feature parity in the shader *(small to medium)*

Fix H, driven by what ObjectiveFrame actually uses:

- **Fog** (used by ObjectiveFrame): `uFogMode` / `uFogColor` / `uFogDensity` /
  `uFogStart` / `uFogEnd`, implementing `GL_LINEAR`, `GL_EXP`, `GL_EXP2`.
- `GL_COLOR_MATERIAL`: `uColorMaterialMode`.
- Two-sided lighting: `uTwoSided` plus a `gl_FrontFacing` normal flip.
- Alpha test: `uAlphaTestFunc` / `uAlphaTestRef` with `discard`.

### Phase 8 — ObjectiveFrame cutover *(coordinated, in the ObjectiveFrame repo)*

Nothing here is blocked by Ivf++ except the last step.

1. Swap `imgui_impl_opengl2` for `imgui_impl_opengl3` (`IvfViewWindow.cpp:12`,
   `src/ofui/`). **Do this first — it is independent and it is the hard blocker.**
2. Convert its own fixed-function drawing (`FemView.cpp:4985`, `Area2D.cpp:81`,
   `IvfViewWindow.cpp:213/231/267`, `ofui/view_window.cpp:197-201`) to
   `rcDrawUnlit` or a `MeshBuffer`.
3. Move `GleSpiralCylinder` onto whatever Phase 4.2 produces.
4. Run against `RenderProfile::Mixed` — this should be a no-op change.
5. Flip the GLFW hints to `GLFW_OPENGL_CORE_PROFILE` plus forward-compatible, set
   `RenderProfile::Core`, and clear whatever `enableDebugOutput()` reports.

### Phase 9 — Deprecate *(after ObjectiveFrame ships on Core)*

Mark the legacy path deprecated but keep it compiling. Optionally gate it behind a
CMake option `IVF_ENABLE_LEGACY_GL` (default `ON`) so downstream users get a
compile-time signal before anything is removed. Delete `src/gle`, `OldLight`,
`OldScene`, `OldGrid` and `OldLightModel` once nothing references them.

---

## 4. Sequencing summary

| Phase | Depends on | Parallel-safe? |
| --- | --- | --- |
| 0 Baseline and `profile_test` — **done** | — | yes |
| 1 `RenderProfile`, fallback, display-list fix - **done** | 0 | - |
| 2 Core-clean shared path - **done** | 1 | - |
| 3 Camera completeness - **done** | 1 | - |
| 4 Remaining geometry - **mostly done** | 2 | yes, per class |
| 5 Colour picking - **done** | 2, 3 | - |
| 6 Text and textures - **mostly done** | 2 | - |
| 7 Shader feature parity - **done** | 2 | - |
| 8.1 ImGui GL3 backend (ObjectiveFrame) | — | **start now, independent** |
| 8.2-8.4 ObjectiveFrame port | 4 | — |
| 8.5 ObjectiveFrame on Core | 5, 6, 7 | — |
| 9 Deprecate | 8 | — |

## 5. Invariants to hold throughout

1. **Never delete a legacy branch.** Guard it; do not remove it, until Phase 9.
2. **The modern branch goes first and returns early**
   (`if (buildAndDrawVAO(...)) return;`) so the legacy body stays reachable.
3. **Every mutator that changes geometry calls `markVAODirty()`** (and
   `markListDirty()` for the legacy list). A missed call shows up as stale
   geometry only in `Mixed` and `Core`, which makes it easy to blame the wrong
   thing.
4. **Attribute locations 0/1/2/3 are contract.** Custom shaders must honour them.
5. **`profile_test` gets a new case with every converted class**, and passes in
   all three profiles before the change is committed.
6. **Both gates, always.** A clean error count proves nothing about the picture —
   the classes that drew nothing at all under `mixed` reported zero errors. Run
   `compare_shots.py` as well, and run the cases **in isolation** (`--only`):
   drawn back to back, an unconverted class inherits the previous object's
   uniforms and can look almost right.
