


#include "Bunnie.h"


extern float g_time;
extern mat4 g_proj;
extern mat4 g_view;

float randFloat() {
    return ((float) rand() / RAND_MAX);
}


Bunnie::Bunnie(vec3 p, vec3 v, Mesh* rmesh, Shader* inmeshShader) {
    pos = p;
    vel = v;

    mesh = rmesh;

    stage = MOVING;
    meshShader = inmeshShader;

    // add(vec3(-10 + 20 * randFloat(), 5 + 5 * randFloat(), -30), vec3(0, 0, 1));
    s = scale(mat4(1.0f), vec3(2));
    r = rotate(mat4(1.0f), randFloat() * 360, vec3(0, 1, 0));
}


// Does the bunnie collide with a sphere at pos, radius r?
bool Bunnie::collide(vec3 pos, float r) {
    if ( ((this->pos.x-pos.x) * (this->pos.x-pos.x) +
                (this->pos.y-pos.y) * (this->pos.y-pos.y) +
                (this->pos.z-pos.z) * (this->pos.z-pos.z)) <= r * r ) {
        return true;
    }
    return false;
}

Bunnie::~Bunnie() {
    cout << "deleted\n";
}

void Bunnie::stop() {
    // cout << "bunnie stop\n";
    stage = STILL;
}


vec3 Bunnie::getPos() {
    return pos;
}

void Bunnie::render() {
    mat4 t = translate(mat4(1.0f), pos);

    switch (stage) {

    case MOVING:
        break;

    case STILL:
        // TODO change material?

        break;

    case DONE:
        return;
        break;

    default:

        break;
    }
    // render mesh
    meshShader->use();
    meshShader->setModelMatrix(t * s * r);

    mesh->render();
}

void Bunnie::update(float t, float dt) {
    switch (stage) {

    case MOVING:
        pos += vel * (BUNNIE_SPEED * dt * MS_2_S);
        break;

    case STILL:
        break;

    case DONE:
        break;

    default:
        break;
    }
}


/*
void Bunnie::stop() {
    if (stage == FLYING) {
        stage = TRAIL;
        // TODO - make black
        smoke = new TFBParticles(g_time * MS_2_S);
        smoke->init(pos);
    }
}
*/

