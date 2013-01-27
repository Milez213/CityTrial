


#include "Plane.h"


extern float g_time;
extern mat4 g_proj;
extern mat4 g_view;


Plane::Plane(vec3 p, vec3 v, Mesh* rmesh, Shader* inmeshShader) {
    exp_age = 0.0f;
    pos = p;
    vel = v;

    pos.y -= 1;

    mesh = rmesh;

    stage = FLYING;

    meshShader = inmeshShader;


    s = scale(mat4(1.0f), vec3(4));
    r = rotate(mat4(1.0f), 90.0f, vec3(0, 1, 0));
}

Plane::~Plane() {
    cout << "deleted\n";
    delete smoke;
    delete explosion;
}



vec3 Plane::getPos() {
    return pos;
}

void Plane::render() {
    mat4 t = translate(mat4(1.0f), pos);

    switch (stage) {

    case FLYING:
        // render mesh
        meshShader->use();
        meshShader->setModelMatrix(t * s * r);

        mesh->render();
        // cout << "rocket render\n";
        break;

    case TRAIL:
        // render mesh
        meshShader->use();
        meshShader->setModelMatrix(t * s * r * rdown);

        mesh->render();
        
        // render smoke
        smoke->use();
        smoke->setProjMatrix(g_proj);
        smoke->setViewMatrix(g_view);
        smoke->setModelMatrix(mat4(1.0f));
        smoke->setPos(pos);
        smoke->render();

        break;

    case EXPL:
        // render smoke
        smoke->use();
        smoke->setProjMatrix(g_proj);
        smoke->setViewMatrix(g_view);
        smoke->setModelMatrix(mat4(1.0f));
        smoke->setPos(pos);
        smoke->render();


        explosion->use();
        explosion->setProjMatrix(g_proj);
        explosion->setViewMatrix(g_view);
        explosion->setModelMatrix(mat4(1.0f));
        explosion->setPos(pos);
        explosion->render();
        break;

    default:
        break;
    }
}

void Plane::update(float t, float dt) {
    switch (stage) {

    case FLYING:

        pos += vel * (ROCKET_SPEED * dt * MS_2_S);
        break;

    case TRAIL:
        smoke->update(t * MS_2_S);
        pos += vel * (ROCKET_SPEED * dt * MS_2_S);
        vel.y -= 2 * dt * MS_2_S;

        rdown = rotate(mat4(1.0f), -(float)(atan(vel.y/vel.z) * 180.0 / PI), vec3(1, 0 ,0));
        
        break;
    case EXPL:

        smoke->stop();
        smoke->update(t* MS_2_S);
        explosion->update(t * MS_2_S);

        exp_age += dt * MS_2_S;

        if (exp_age > EXP_LEN) {
            stage = DONE;
        }

        break;
    case DONE:
        break;
    default:
        break;
    }
}

void Plane::boom() {
    if (stage == TRAIL) {
        cout << "boom! (" << id << ")" << endl;
        stage = EXPL;

        explosion = new Sparks(g_time * MS_2_S, PI, 10);
        explosion->init(pos);
        explosion->stop();
    }
}

void Plane::down() {
    if (stage == FLYING) {

        stage = TRAIL;

        // TODO - make black
        smoke = new TFBParticles(g_time * MS_2_S);
        smoke->init(pos);

    }

}

bool Plane::dead() {
    return stage == DONE;
}

bool Plane::flying() {
    return stage == FLYING;
}
