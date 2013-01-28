


#include "Rocket.h"


extern float g_time;
extern mat4 g_proj;
extern mat4 g_view;


Rocket::Rocket(vec3 p, vec3 v, Mesh* rmesh, Shader* inmeshShader) {
    exp_age = 0.0f;
    pos = p;
    vel = v;

    pos.y -= 1;

    pos -= v;

    mesh = rmesh;
    flame = new TFBParticles(g_time * MS_2_S);
    flame->init(pos);
    flame->id = id;

    stage = ROCKET_TRAIL;

    meshShader = inmeshShader;
}

Rocket::~Rocket() {
    cout << "deleted\n";
    delete flame;
    delete explosion;
}


vec3 Rocket::getPos() {
    return pos;
}

void Rocket::render() {
    mat4 t = translate(mat4(1.0f), pos);

    if (stage == ROCKET_TRAIL) {
        // render mesh
        mat4 s = scale(mat4(1.0f), vec3(0.1));
        // TODO - rotate
        // mat4 r = rotate(mat4(1.0f), );
        meshShader->use();
        meshShader->setModelMatrix(t * s);

        mesh->render();
        // cout << "rocket render\n";
    }

    // render flame
    flame->use();
    flame->setProjMatrix(g_proj);
    flame->setViewMatrix(g_view);
    flame->setModelMatrix(mat4(1.0f));
    flame->setPos(pos);
    flame->render();

    
    if (stage == ROCKET_EXPL) {
        explosion->use();
        explosion->setProjMatrix(g_proj);
        explosion->setViewMatrix(g_view);
        explosion->setModelMatrix(mat4(1.0f));
        explosion->setPos(pos);
        explosion->render();
    }

    // cout << "posx: " << pos.x << endl;
}

void Rocket::update(float t, float dt) {
    switch (stage) {
    case ROCKET_TRAIL:
        flame->update(t * MS_2_S);
        pos += vel * (ROCKET_SPEED * dt * MS_2_S);

        // accel
        // vel.y -= 1 * dt * MS_2_S;
        
        /*
        if (pos.y < -1) {
            vel.y *= -1;
            vel.y *= 0.9;
        }
        */
        
        break;
    case ROCKET_EXPL:

        explosion->update(t * MS_2_S);

        exp_age += dt * MS_2_S;

        if (exp_age > EXP_LEN) {
            stage = ROCKET_DONE;
        }

        break;
    case ROCKET_DONE:
        break;
    default:
        break;
    }
}

void Rocket::boom() {
    if (stage == ROCKET_TRAIL) {
        cout << "boom! (" << id << ")" << endl;
        stage = ROCKET_EXPL;
        flame->stop();

        explosion = new Sparks(g_time * MS_2_S, PI, 4);
        explosion->init(pos);
        explosion->stop();
    }
}

bool Rocket::dead() {
    return stage == ROCKET_DONE;
}

bool Rocket::flying() {
    return stage == ROCKET_TRAIL;
}
