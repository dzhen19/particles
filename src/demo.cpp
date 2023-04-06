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
    int id;
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
        createParticle(300);
        renderer.setDepthTest(false);
        renderer.blendMode(agl::ADD);
    }

    void initParticle(Particle *particle)
    {
        particle->color = vec4(agl::randomUnitCube() + vec3(.3, .3, .3), 1);
        particle->size = ((double)rand() / (RAND_MAX)) * .01 + 0.02;
        particle->rot = ((double)rand() / (RAND_MAX)) + 1;
        particle->pos = randomUnitSquare() * 2.0f;
        particle->vel = randomUnitSquare() * .5f;
    }

    // recycle the first transparent confetti that we find in mParticles
    void recycleParticles(Particle particleToRecyle)
    {
        for (int i = 0; i < mParticles.size(); i++)
        {
            initParticle(&mParticles[i]);
            vec3 _newpos = particleToRecyle.pos + vec3(cos(i), sin(i), 0) * particleToRecyle.size * 1.5f;
            mParticles[i].vel = -(particleToRecyle.pos - _newpos) * 2.0f;

            mParticles[i].pos = vec3(std::min(std::max(_newpos.x, -.9f), .9f),
                                     std::min(std::max(_newpos.y, -.9f), .9f),
                                     std::min(std::max(_newpos.z, -.9f), .9f));
        }
    }

    void createParticle(int size)
    {
        renderer.loadTexture("particle", "../textures/ParticleBokeh.png", 0);
        for (int i = 0; i < size; i++)
        {
            Particle particle;
            initParticle(&particle);
            particle.id = mParticles.size() + i;
            mParticles.push_back(particle);
        }
    }

    void updateParticle()
    {
        for (int i = 0; i < mParticles.size(); i++)
        {
            bool isTransparent = mParticles[i].color[3] == 0;
            if (isTransparent)
                continue;

            vec3 _pos = mParticles[i].pos;
            Particle _i = mParticles[i];

            // check collision w/other particles
            for (int j = 0; j < mParticles.size(); j++)
            {
                // 5 second moratorium after recycle
                if (t < lastParticleCreationT + 30 * 10)
                    break;
                Particle _j = mParticles[j];
                if (_j.id == _i.id || _j.color[3] == 0)
                    continue;

                float dist = distance(_i.pos, _j.pos);
                if (dist < _i.size * .5)
                {
                    mParticles[j].vel += (_i.pos - _j.pos) * .5f;
                }

                if (dist < _i.size * .2)
                {
                    if (_i.size > _j.size && _j.color[3] > 0)
                    {
                        // absorb the other particle
                        mParticles[j].color[3] = 0;
                        mParticles[i].size += _j.size / 2;
                        mParticles[i].color = (_j.color + _i.color) / 2.0f;
                        std::cout << _j.id << " absorbed by " << _i.id << " distance: " << dist << std::endl;
                    }
                }
            }

            if (_pos.x > 1 || _pos.x < -1)
            {
                mParticles[i].vel[0] *= -1;
            }
            else if (_pos.y > 1 || _pos.y < -1)
            {
                mParticles[i].vel[1] *= -1;
            }

            mParticles[i].pos += (mParticles[i].vel) * .01f;
            mParticles[i].rot += .01;
        }
    }

    void drawParticle()
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
        renderer.beginShader("sprite");

        float aspect = ((float)width()) / height();

        vector<int> activeIDs;

        for (int i = 0; i < mParticles.size(); i++)
        {
            if (mParticles[i].color[3] > 0)
                activeIDs.push_back(mParticles[i].id);
        }
        // all cells absorbed
        if (activeIDs.size() == 1)
        {
            recycleParticles(mParticles[activeIDs[0]]);
            lastParticleCreationT = t;
            return;
        }

        drawParticle();
        updateParticle();

        renderer.lookAt(eyePos, lookPos, up);
        renderer.endShader();

        t++;
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
