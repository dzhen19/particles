// Bryn Mawr College, alinen, 2020
//

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

class Viewer : public Window
{
public:
  float radius = 10;
  // in degrees [0, 360]
  float azimuth = 0;
  // in degrees [-90, 90]
  float elevation = 0;

  float scaleFactor = 1;

  bool canRotate = false;

  vec3 getEyePos()
  {
    return vec3(radius * sin(radians(azimuth)) * std::max(cos(radians(elevation)), float(0.01)),
                radius * sin(radians(elevation)),
                radius * cos(radians(azimuth)) * std::max(cos(radians(elevation)), float(0.01)));
  }

  Viewer() : Window()
  {
  }

  void setup()
  {
    setWindowSize(1000, 1000);
    renderer.loadShader("simple-texture",
                        "../shaders/simple-texture.vs",
                        "../shaders/simple-texture.fs");

    Image img;
    img.load("../textures/tree.png", true);

    scaleFactor = std::max((width() / img.width()), (height() / img.height())) ;

    renderer.loadTexture("tree", img, 0);

    renderer.loadTexture("grass", "../textures/grass.png", 0);
    renderer.blendMode(agl::BLEND);
  }

  void mouseMotion(int x, int y, int dx, int dy)
  {
    if (!canRotate)
    {
      return;
    }

    float newAzimuth = azimuth + dx;
    azimuth = std::max(std::min(newAzimuth, float(360)), float(0));

    float newElevation = elevation + dy;
    elevation = std::max(std::min(newElevation, float(90)), float(-90));
  }

  void mouseDown(int button, int mods)
  {
    canRotate = true;
  }

  void mouseUp(int button, int mods)
  {
    canRotate = false;
  }

  void scroll(float dx, float dy)
  {
    float newRadius = radius += dy / 10;
    radius = std::max(newRadius, float(.01));
  }

  void draw()
  {
    renderer.beginShader("simple-texture");

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
    eyePos = getEyePos();
    renderer.lookAt(eyePos, lookPos, up);

    renderer.scale(vec3(scaleFactor));

    // draw plane
    renderer.texture("Image", "grass");
    renderer.push();
    renderer.translate(vec3(0.0, -0.5, 0));
    renderer.scale(vec3(2.0f));
    renderer.plane();
    renderer.pop();

    // draw tree
    renderer.texture("Image", "tree");
    renderer.push();
    renderer.translate(vec3(-0.5, -0.5, 0));

    // rotate tree
    vec3 n = normalize(eyePos - lookPos);
    float theta_y = atan2(n.x, n.z);
    renderer.push();
    renderer.translate(vec3(0.5, 0.5, 0));
    renderer.rotate(theta_y, vec3(0, 1, 0));
    renderer.translate(vec3(-0.5, -0.5, 0));

    // draw tree
    renderer.quad(); // vertices span from (0,0,0) to (1,1,0)

    // pop off rotate
    renderer.pop();
    // pop off translate
    renderer.pop();

    renderer.endShader();
  }

protected:
  vec3 eyePos = vec3(1, 0, 0);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
};

int main(int argc, char **argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
