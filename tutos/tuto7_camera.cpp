
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
            sizeof(float) * 3 ,  // stride: 6 floats per vertex (3 for position and 3 for normal)
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





// utilitaire. creation d'une grille / repere.
Mesh make_grid(const int n = 10)
{
    Mesh grid = Mesh(GL_LINES);

    // grille
    grid.color(White());
    for (int x = 0; x < n; x++)
    {
        float px = float(x) - float(n) / 2 + .5f;
        grid.vertex(Point(px, 0, -float(n) / 2 + .5f));
        grid.vertex(Point(px, 0, float(n) / 2 - .5f));
    }

    for (int z = 0; z < n; z++)
    {
        float pz = float(z) - float(n) / 2 + .5f;
        grid.vertex(Point(-float(n) / 2 + .5f, 0, pz));
        grid.vertex(Point(float(n) / 2 - .5f, 0, pz));
    }

    // axes XYZ
    grid.color(Red());
    grid.vertex(Point(0, .1, 0));
    grid.vertex(Point(1, .1, 0));

    grid.color(Green());
    grid.vertex(Point(0, .1, 0));
    grid.vertex(Point(0, 1, 0));

    grid.color(Blue());
    grid.vertex(Point(0, .1, 0));
    grid.vertex(Point(0, .1, 1));

    glLineWidth(2);

    return grid;
}


class TP : public AppCamera
{
public:
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    TP() : AppCamera(1024, 640) {}

    // creation des objets de l'application
    int init()
    {
        // decrire un repere / grille
        m_repere = make_grid(10);

        // charge un objet
        //m_cube= read_mesh("data/cube.obj");

        // un autre objet
        m_objet = Mesh(GL_TRIANGLES);


        // Charger un robot
        // m_robot= read_mesh("data/robot.obj");
        Mesh mesh = read_mesh("data/robot.obj");
        m_robot.create(mesh);


        {
            // ajouter des triplets de sommet == des triangles dans objet...
        }

        // Part I added for coloring the robot

        m_program = read_program("shaders/cartoon.glsl");
        program_print_errors(m_program);
        //

        glClearColor(0.2f, 0.2f, 0.2f, 1.f);        // couleur par defaut de la fenetre

        glClearDepth(1.f);                          // profondeur par defaut
        glDepthFunc(GL_LESS);                       // ztest, conserver l'intersection la plus proche de la camera
        glEnable(GL_DEPTH_TEST);                    // activer le ztest

        return 0;   // pas d'erreur, sinon renvoyer -1
    }

    // destruction des objets de l'application
    int quit()
    {
        m_objet.release();
        m_robot.release();
        m_repere.release();
        release_program(m_program);
        return 0;   // pas d'erreur
    }

    // dessiner une nouvelle image
    int render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // dessine le repere, place au centre du monde, pour le point de vue de la camera
        draw(m_repere, /* model */ Identity(), camera());

        // dessine un cube, lui aussi place au centre du monde
            // draw(m_cube, /* model */ Identity(), camera());

            //////////////////////
            //draw(m_robot, /* model */ Identity(), camera());
            /////////////

        // dessine le meme cube, a un autre endroit.
            // il faut modifier la matrice model, qui sert a ca : placer un objet dans le monde, ailleurs qu'a l'origine.
            // par exemple, pour dessiner un 2ieme cube a la verticale au dessus du premier cube :
            // la transformation est une translation le long du vecteur Y= (0, 1, 0), si on veut placer le cube plus haut, il suffit d'utiliser une valeur > 1
        Transform t = Translation(0, 2, 0);

        //

    // comment dessiner m_objet ??

    // et sans le superposer au cube deja dessine ?

        // continuer, afficher une nouvelle image
        // tant que la fenetre est ouverte...


        // Part I added for coloring the robot it was [ draw(m_robot, /* model */ Identity(), camera()); ] The coloring is uniform
        Transform model = RotationX(0);
        Transform view = m_camera.view();
        Transform projection = m_camera.projection(window_width(), window_height(), 45);
        Transform mvp = projection * view * model;
        int location;
        glUseProgram(m_program);
        location = glGetUniformLocation(m_program, "mvpMatrix");
        glUniformMatrix4fv(location, 1, GL_TRUE, mvp.data());
        location = glGetUniformLocation(m_program, "color");
        glUniform4f(location, 0.5, 0.5, 0, 1);

        glBindVertexArray(m_robot.vao);
        glDrawArrays(GL_TRIANGLES, 0, m_robot.vertex_count);

        return 1;

    }

protected:
    Mesh m_objet;
    // Mesh m_cube;
    Mesh m_repere;
    // m_robot is now buffers and not a mesh (Part I added for coloring the robot)
    Buffers m_robot;
    //
    GLuint m_program;
    std::vector<TriangleGroup> m_groups;
};


int main(int argc, char** argv)
{
    // il ne reste plus qu'a creer un objet application et la lancer

    TP tp;
    tp.run();
    return 0;
}
