#pragma once

#include <glad/gl.h>
#include "vertex.hpp"

namespace our {

    #define ATTRIB_LOC_POSITION 0
    #define ATTRIB_LOC_COLOR    1
    #define ATTRIB_LOC_TEXCOORD 2
    #define ATTRIB_LOC_NORMAL   3

    class Mesh {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements 
        GLsizei elementCount;
    public:

        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering 
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& elements)
        {
            //TODO: Done (Req 2) Write this function
            // remember to store the number of elements in "elementCount" since you will need it for drawing
            // For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc

            // VAO
            glGenVertexArrays(1, &VAO); // Generate vertex array 
            glBindVertexArray(VAO); // Bind it

            // VBO
            glGenBuffers(1, &VBO); // Generate vertex buffer
            glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind it
            glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW); // Copy the data to the buffer

            //EBO
            glGenBuffers(1, &EBO); // Generate element buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Bind it
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size()*sizeof(unsigned int), elements.data(), GL_STATIC_DRAW); // Copy the data to the buffer

            // Position attribute
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);

            // Color Attribute
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)(offsetof(Vertex, color)));
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);

            // Texture Attribute
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, tex_coord)));
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);

            // Normal Attribute
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);

            glBindVertexArray(0); // UnBind VAO 

            elementCount = (GLsizei) elements.size(); // Store the number of elements for drawing
        }

        // this function should render the mesh
        void draw() 
        {
            //TODO: Done (Req 2) Write this function

            glBindVertexArray(VAO); // Bind vertex array
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, 0); // Draw the elements
            glBindVertexArray(0); // UnBind vertex array
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh(){
            //TODO: Done (Req 2) Write this function

            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
            glDeleteVertexArrays(1, &VAO);
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}