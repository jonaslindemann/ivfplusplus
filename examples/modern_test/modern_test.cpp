// ------------------------------------------------------------
//
// modern_test — end-to-end validation of the modern OpenGL pipeline
//
// Validates:
//   - RenderContext::useBlinnPhong() (one-call shader setup)
//   - Camera (glm projection/view fed into RenderContext)
//   - Light   (LightData fed into RenderContext)
//   - Sphere  (VAO/VBO modern path, falls back to gluSphere without shader)
//   - Material::uploadToShader (uMat* uniforms)
//
// Mouse drag: orbit the camera around the sphere.
//
// ------------------------------------------------------------

#include <ivfglfw/GlfwApplication.h>
#include <ivfglfw/GlfwWindow.h>

#include <ivf/Camera.h>
#include <ivf/Lighting.h>
#include <ivf/Material.h>
#include <ivf/Sphere.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

using namespace ivf;

// ============================================================

IvfSmartPointer(ModernTestWindow);

class ModernTestWindow : public GlfwWindow {
private:
    CameraPtr   m_camera;
    LightPtr    m_light;
    MaterialPtr m_material;
    SpherePtr   m_sphere;

    // Mouse orbit state
    bool   m_mouseDown = false;
    int    m_lastX = 0, m_lastY = 0;
    double m_angleH   = 30.0;
    double m_angleV   = 20.0;
    double m_distance =  4.0;

    void updateCameraPosition()
    {
        double radH = glm::radians(m_angleH);
        double radV = glm::radians(m_angleV);
        m_camera->setPosition(
            m_distance * cos(radV) * sin(radH),
            m_distance * sin(radV),
            m_distance * cos(radV) * cos(radH));
    }

public:
    ModernTestWindow(int X, int Y, int W, int H)
        : GlfwWindow(X, Y, W, H) {}

    static ModernTestWindowPtr create(int X, int Y, int W, int H)
    {
        return ModernTestWindowPtr(new ModernTestWindow(X, Y, W, H));
    }

    void onInit(int width, int height) override
    {
        // ---- Camera ----
        m_camera = Camera::create();
        m_camera->setTarget(0.0, 0.0, 0.0);
        updateCameraPosition();

        // ---- Light ----
        m_light = Lighting::getInstance()->getLight(0);
        m_light->setLightPosition(3.0f, 5.0f, 3.0f, 1.0f);
        m_light->setAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
        m_light->setDiffuseColor(1.0f, 1.0f, 0.9f, 1.0f);
        m_light->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
        m_light->enable();

        // ---- Material ----
        m_material = Material::create();
        m_material->setAmbientColor( 0.15f, 0.05f, 0.05f, 1.0f);
        m_material->setDiffuseColor( 0.8f,  0.2f,  0.2f,  1.0f);
        m_material->setSpecularColor(1.0f,  1.0f,  1.0f,  1.0f);
        m_material->setEmissionColor(0.0f,  0.0f,  0.0f,  1.0f);
        m_material->setShininess(64.0f);

        // ---- Sphere ----
        m_sphere = Sphere::create();
        m_sphere->setRadius(1.0);
        m_sphere->setSlices(32);
        m_sphere->setStacks(32);
        m_sphere->setMaterial((Material*)m_material);

        // ---- Activate modern shader ----
        enableBlinnPhongShader(0.05f, 0.05f, 0.05f);

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    }

    void onResize(int width, int height) override
    {
        m_camera->setPerspective(45.0, 0.1, 200.0);
        m_camera->setViewPort(width, height);
        m_camera->initialize();
    }

    void onRender() override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_camera->render();
        m_light->render();

        // Sphere::doCreateGeometry detects the active shader and draws via VAO.
        // The material was assigned to the sphere so doCreateMaterial uploads it.
        m_sphere->render();
    }

    void onMouseDown(int x, int y) override { m_mouseDown = true;  m_lastX = x; m_lastY = y; }
    void onMouseUp  (int x, int y) override { m_mouseDown = false; }

    void onMouseMove(int x, int y) override
    {
        if (!m_mouseDown) return;
        m_angleH += (x - m_lastX) * 0.5;
        m_angleV -= (y - m_lastY) * 0.5;
        m_angleV  = glm::clamp(m_angleV, -89.0, 89.0);
        m_lastX = x; m_lastY = y;
        updateCameraPosition();
        redraw();
    }
};

// ============================================================
int main(int argc, char** argv)
{
    auto app = GlfwApplication::getInstance(&argc, argv);
    app->setDisplayMode(IVF_DOUBLE | IVF_RGB | IVF_DEPTH);

    auto window = ModernTestWindow::create(0, 0, 640, 640);
    window->setWindowTitle("Ivf++ modern pipeline test — drag to orbit");
    window->show();

    app->run();
    return 0;
}
