
//! \file tuto7_camera.cpp reprise de tuto7.cpp mais en derivant AppCamera, avec gestion automatique d'une camera.


#include "wavefront.h"
#include "texture.h"

#include "orbiter.h"
#include "draw.h"
#include "app_camera.h"        // classe Application a deriver
#include "program.h"
#include "texture.h"


GLuint program;


class TP : public AppCamera
{
public:
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    TP() : AppCamera(1024, 640) {}

    // creation des objets de l'application
    int init()
    {

        m_robot = read_mesh("bistro-small/export.obj");
        // recuperer les dimensions de l'objet
        Point pmin, pmax;
        m_robot.bounds(pmin, pmax);

        // regler la camera pour observer l'objet
        camera().lookat(pmin, pmax);

        glClearColor(0.2f, 0.2f, 0.2f, 1.f);        // couleur par defaut de la fenetre
        glClearDepth(1.f);                          // profondeur par defaut
        glDepthFunc(GL_LESS);                       // ztest, conserver l'intersection la plus proche de la camera
        glEnable(GL_DEPTH_TEST);                    // activer le ztest

        return 0;   // pas d'erreur, sinon renvoyer -1
    }

    // destruction des objets de l'application
    int quit()
    {
        m_robot.release();
        return 0;   // pas d'erreur
    }

    // dessiner une nouvelle image
    int render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        draw(m_robot, /* model */ Identity(), camera());
        return 1;
    }

protected:
    Mesh m_robot;
};


int main(int argc, char** argv)
{
    TP tp;
    tp.run();
    return 0;
}
