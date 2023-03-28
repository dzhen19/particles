// Bryn Mawr College, alinen, 2020
//

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

struct Particle
{
  glm::vec3 pos;
  glm::vec3 vel;
  glm::vec4 color;
  float rot;
  float size;
};

class Viewer : public Window
{
public:
  Viewer() : Window()
  {
  }

  void setup()
  {
    setWindowSize(1000, 1000);
    createConfetti(1);
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
  }

  void initConfetti(Particle *particle)
  {
    particle->color = vec4(agl::randomUnitCube(), 1);
    particle->size = 0.25;
    particle->rot = ((double)rand() / (RAND_MAX)) + 1;
    particle->pos = position;
    particle->vel = agl::randomUnitCube();
  }

  // recycle the first transparent confetti that we find in mParticles
  void recycleConfetti()
  {
    for (int i = 0; i < mParticles.size(); i++)
    {
      float alpha = mParticles[i].color[3];
      if (alpha <= 0)
        initConfetti(&mParticles[i]);
      return;
    }
  }

  void createConfetti(int size)
  {
    if (mParticles.size() > MAX_PARTICLES)
    {
      recycleConfetti();
      return;
    }

    renderer.loadTexture("particle", "../textures/star4.png", 0);
    for (int i = 0; i < size; i++)
    {
      Particle particle;
      initConfetti(&particle);
      mParticles.push_back(particle);
    }
  }

  void updateConfetti()
  {
    for (int i = 0; i < mParticles.size(); i++)
    {
      vec4 prevColor = mParticles[i].color;
      // only update particle if it is not transparent
      if (prevColor[3] > 0)
      {
        mParticles[i].pos += (mParticles[i].vel) * .01f;
        mParticles[i].size += .005;
        mParticles[i].rot += .01;
        mParticles[i].color = vec4(prevColor[0], prevColor[1], prevColor[2], prevColor[3] - .01);
      }
    }
  }

  void drawConfetti()
  {
    renderer.texture("image", "particle");
    for (int i = 0; i < mParticles.size(); i++)
    {
      Particle particle = mParticles[i];
      renderer.sprite(particle.pos, particle.color, particle.size, particle.rot);
    }
  }

  void mouseMotion(int x, int y, int dx, int dy)
  {
  }

  void mouseDown(int button, int mods)
  {
  }

  void mouseUp(int button, int mods)
  {
  }

  void scroll(float dx, float dy)
  {
    eyePos.z += dy;
  }

  void keyUp(int key, int mods)
  {
  }

  void draw()
  {
    t = elapsedTime();

    // create a particle every half second
    if (t > lastParticleCreationT + .05)
    {
      createConfetti(1);
      lastParticleCreationT = t;
    }

    renderer.beginShader("sprite");
    position = vec3(cos(t), sin(t), 0);

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

    renderer.lookAt(eyePos, lookPos, up);
    renderer.sprite(position, vec4(1.0f), 0.25f);
    updateConfetti();
    drawConfetti();
    renderer.endShader();
  }

protected:
  float t = 0;
  float lastParticleCreationT = t;
  int MAX_PARTICLES = 500;
  vec3 eyePos = vec3(0, 0, 3);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  vec3 position = vec3(1, 0, 0);

  std::vector<Particle> mParticles;
};

int main(int argc, char **argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
