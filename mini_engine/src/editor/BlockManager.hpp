#pragma once

#include <vector>

#include "../essentials.hpp"
#include "../SceneObject.hpp"
#include "../scene.hpp"



struct Cammand {
    void (*func)(float);
    float arg;
};

class Motion {
public:
    vec3 input;
    vec3 output;

    vec3 position;
    vec3 direction;

    SceneObject actor;

    Motion(SceneObject actor_) {
        input = vec3(0.0);
        output = vec3(0.0);

        actor = actor_;

        position = actor.transform.position;
        direction = actor.transform.euluerAngles;
    }


    void move(float speed) {
        output = output + vec3(sin(actor.transform.rotation.x), sin(actor.transform.rotation.y), cos(actor.transform.rotation.x)) * speed;
        actor.transform.position = actor.transform.position + output;
    }

    void rotateX(float angle) {
        actor.transform.euluerAngles = actor.transform.euluerAngles + vec3(angle, 0, 0);
    }

    void rotateY(float angle) {
        actor.transform.euluerAngles = actor.transform.euluerAngles + vec3(0, angle, 0);
    }

    void rotateZ(float angle) {
        actor.transform.euluerAngles = actor.transform.euluerAngles + vec3(0, angle, 0);
    }


    void gotoActor(SceneObject target) {
        actor.transform.position = target.transform.position;
    }

    void gotoPos(const vec3& pos) {
        actor.transform.position = pos;
    }

    void glideToActor(SceneObject target, float time) {
        float start = timer;

        float x0 = actor.transform.position.x;
        float y0 = actor.transform.position.y;

        float x1 = target.transform.position.x;
        float y1 = target.transform.position.y;

        for (float i = 0.0; i < time; i += deltaTime) {
            float elapsed = timer - start;

            float t = elapsed/time;
            actor.transform.position.x = smoothstep(x0, x1, t);
            actor.transform.position.y = smoothstep(y0, y1, t);
        }
    }

    void glideToPos(const vec2& pos, float time) {
        float start = timer;

        float x0 = actor.transform.position.x;
        float y0 = actor.transform.position.y;

        for (float i = 0.0; i < time; i += deltaTime) {
            float elapsed = timer - start;

            float t = elapsed/time;
            actor.transform.position.x = smoothstep(x0, pos.x, t);
            actor.transform.position.y = smoothstep(y0, pos.y, t);
        }
    }


    void setDirection(const vec3& dir) {
        actor.transform.euluerAngles = dir;
    }

    void pointAtActor(SceneObject target) {
        actor.transform.euluerAngles = actor.transform.pointAt(target.transform.position);
    }

    void pointAtPos(const vec3& pos) {
        actor.transform.euluerAngles = actor.transform.pointAt(pos);
    }


    void changeX(float x) {
        actor.transform.position.x = actor.transform.position.x + x;
    }

    void setX(float x) {
        actor.transform.position.x = x;
    }

    void changeY(float y) {
        actor.transform.position.y = actor.transform.position.y + y;
    }

    void setY(float y) {
        actor.transform.position.x = y;
    }

    void changeZ(float z) {
        actor.transform.position.z = actor.transform.position.z + z;
    }

    void setZ(float z) {
        actor.transform.position.z = z;
    }


    // Potentially more
};



class Looks {
public:
    vec3 input;
    vec3 output;

    vec3 scale;
    float frame;

    SceneObject actor;

    Looks(SceneObject actor_) {
        input = vec3(0.0);
        output = vec3(0.0);

        frame = 1.0;

        actor = actor_;

        scale = actor.transform.scale;
    }


    void setCostume(float frame) {
        const int f = floor(frame);
        actor.mesh.frame = f;
    }

    void nextCostume() {
        actor.mesh.frame += 1;
    }

    void setBackdrop(float frame) {
        const int f = floor(frame);
        scene.backdrop.frame = f;
    }

    void nextBackdrop() {
        scene.backdrop.frame += 1;
    }


    void changeScaleX(float x) {
        actor.transform.scale.x = actor.transform.scale.x + x;
    }

    void setScaleX(float x) {
        actor.transform.scale.x = x;
    }

    void changeScaleY(float y) {
        actor.transform.scale.y = actor.transform.scale.y + y;
    }

    void setScaleY(float y) {
        actor.transform.scale.y = y;
    }

    void changeScaleZ(float z) {
        actor.transform.scale.z = actor.transform.scale.z + z;
    }

    void setScaleZ(float z) {
        actor.transform.scale.z = z;
    }


    void changeEffect(float x, const std::string& effect) {
        //
    }

    void setEffect(float x, const std::string& effect) {
        //
    }

    void clearEffects() {
        //
    }


    void show() {
        actor.mesh.visible = true;
    }

    void hide() {
        actor.mesh.visible = false;
    }


    void setLayer(float x) {
        int layer = floor(x);
        actor.mesh.layer = layer;
    }

    void frontLayer() {
        actor.mesh.layer = 1;
    }

    void backLayer() {
        actor.mesh.layer = -1;
    }

    void changeLayer(float x) {
        int layer = floor(x);
        actor.mesh.layer += layer;
    }
};



class Sound {
// later

};



class Events {
public:
    vec3 input;
    vec3 output;

    SceneObject actor;

    Events(SceneObject actor_) {
        input = vec3(0.0);
        output = vec3(0.0);

        actor = actor_;
    }



    void start(std::vector<void>& commands) {
        for(int i = 0; i < commands.size(); i++) {
            commands[i].func(commands[i].arg);
        }
    }

    void whenPressed(char k, std::vector<void>& commands) {
        if (actor.Input.detectKey(k)) {
            for(int i = 0; i < commands.size(); i++) {
                commands[i].func(commands[i].arg);
            }
        }
    }

    void whenClicked() {
        // Need to add
    }


};



class Controls {
public:
    vec3 input;
    vec3 output;

    SceneObject actor;

    Controls(SceneObject actor_) {
        input = vec3(0.0);
        output = vec3(0.0);

        actor = actor_;
    }



    void wait(float time) {
        float start = timer;

        for (float i = 0; i < time; i += deltaTime) {
            float elapsed = timer - start;
            float t = timer/time;
        }
    }



    void repeat(float count, std::vector<void>& commands) {
        for (float i = 0; i < count; i++) {
            for(int j = 0; j < commands.size(); j++) {
                commands[j].func(commands[j].arg);
            }
        }
    }

    void forever(std::vector<void>& commands) {
        while (true) {
            for(int i = 0; i < commands.size(); i++) {
                commands[i].func(commands[i].arg);
            }
        }
    }



    void ifThen(bool in, std::vector<void>& commands) {
        if (in) {
            for(int i = 0; i < commands.size(); i++) {
                commands[i].func(commands[i].arg);
            }
        }
    }

    void ifThenElse(bool in, std::vector<void>& commands1, std::vector<void>& commands2) {
        if (in) {
            for(int i = 0; i < commands1.size(); i++) {
                commands1[i].func(commands1[i].arg);
            }
        } else {
            for(int i = 0; i < commands2.size(); i++) {
                commands2[i].func(commands2[i].arg);
            }
        }
    }

    void waitUntil(bool in) {
        float start = timer;
        while (in) {
            float elapsed = timer - start;
        }
    }

    void repeatUntil(bool in, std::vector<void>& commands) {
        while (!in) {
            for(int i = 0; i < commands.size(); i++) {
                commands[i].func(commands[i].arg);
            }
        }
    }

    void whileTrue(bool in, std::vector<void>& commands) {
        while (in) {
            for(int i = 0; i < commands.size(); i++) {
                commands[i].func(commands[i].arg);
            }
        }
    }



    void stop(std::string& type) {
        //
    }





};