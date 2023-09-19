
//! \file tuto7_camera.cpp reprise de tuto7.cpp mais en derivant AppCamera, avec gestion automatique d'une camera.


#include "wavefront.h"
#include "texture.h"

#include "orbiter.h"
#include "draw.h"
#include "app_camera.h"        // classe Application a deriver
#include "program.h"



GLuint program;
struct Buffers
{
    GLuint vao;
    GLuint vertex_buffer;
    GLuint normal_Buffer;

    int vertex_count;

    Buffers() : vao(0), vertex_buffer(0), normal_Buffer(0), vertex_count(0) {}

    void create(const Mesh& mesh)
    {
        if (!mesh.vertex_buffer_size() || !mesh.normal_buffer_size()) return;

        // cree et configure le vertex array object: conserve la description des attributs de sommets
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);


        // Create and initialize the combined vertex and normal buffer

        glGenBuffers(1, &vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        std::size_t totalBufferSize = mesh.vertex_buffer_size() + mesh.normal_buffer_size();
        // Allocate memory for the buffer, but don't fill it yet
        glBufferData(GL_ARRAY_BUFFER, totalBufferSize, nullptr, GL_DYNAMIC_DRAW);

        // Copy both position and normal data to the buffer


        glBufferSubData(GL_ARRAY_BUFFER, 0, mesh.vertex_buffer_size(), mesh.vertex_buffer());
        glBufferSubData(GL_ARRAY_BUFFER, mesh.vertex_buffer_size(), mesh.normal_buffer_size(), mesh.normal_buffer());


        // attribut 0, position des sommets, declare dans le vertex shader : in vec3 position;
        glVertexAttribPointer(0,
            3, GL_FLOAT, GL_FALSE,
            sizeof(float) * 3,  // stride: 6 floats per vertex (3 for position and 3 for normal)
            0                   // offset: 0 (data starts at the beginning of the buffer)
        );
        glEnableVertexAttribArray(0);

        // Attribute 1: Vertex Normals (in vec3 normal; in the vertex shader)
        glVertexAttribPointer(2,
            3, GL_FLOAT, GL_FALSE,
            sizeof(float) * 3,  // stride: 6 floats per vertex (3 for position and 3 for normal)
            0 // offset: 3 floats (skip the position data)
        );
        glEnableVertexAttribArray(2);
        // attention : le vertex array selectionne est un parametre implicite
        // attention : le buffer selectionne sur GL_ARRAY_BUFFER est un parametre implicite
        // 
        // conserve le nombre de sommets
        vertex_count = mesh.vertex_count();
    }

    void release()
    {
        glDeleteBuffers(1, &vertex_buffer);
        glDeleteBuffers(1, &normal_Buffer);
        glDeleteVertexArrays(1, &vao);
    }
};



class TP : public AppCamera
{
public:
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    TP() : AppCamera(1024, 640) {}

    // creation des objets de l'application
    int init()
    {

        robot_mesh = read_mesh("data/robot.obj");
        m_robot.create(robot_mesh);

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
