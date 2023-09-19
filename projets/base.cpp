
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

        robot_mesh = read_mesh("data/robot.obj");

        m_groups = robot_mesh.groups();

        m_program = read_program("shaders/uniform.glsl");
        program_print_errors(m_program);

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
        release_program(m_program);
        return 0;   // pas d'erreur
    }

    // dessiner une nouvelle image
    int render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const Materials& materials = robot_mesh.materials();

        Transform model = RotationX(0);
        Transform view = m_camera.view();
        Transform projection = m_camera.projection(window_width(), window_height(), 45);
        Transform mvp = projection * view * model;
        int location;
        for (unsigned i = 0; i < m_groups.size(); i++)
        {
            const TriangleGroup& group = m_groups[i];
            Color color;
            // recuperer la couleur de la matiere du group
            color = materials.material(group.index).diffuse;
            // parametrer le shader pour dessiner avec la couleur
            glUseProgram(m_program);
            location = glGetUniformLocation(m_program, "mvpMatrix");
            glUniformMatrix4fv(location, 1, GL_TRUE, mvp.data());
            location = glGetUniformLocation(m_program, "color");
            glUniform3f(location, color.r, color.g, color.b);
            // dessiner les triangles du groupe
            glBindVertexArray(m_robot.vao);
            glDrawArrays(GL_TRIANGLES, group.first, group.n);
        }

        return 1;

    }

protected:
    Buffers m_robot;
    Mesh robot_mesh;
    GLuint m_program;
    std::vector<TriangleGroup> m_groups;
};


int main(int argc, char** argv)
{
    TP tp;
    tp.run();
    return 0;
}
