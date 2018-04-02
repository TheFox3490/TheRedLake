/*
Copyright Shilnikov Nikita 2018.
This file is a component of the source code of RedMinecraft
which is wrote in C++ and SFML + OpenGL.

The list of all source files:
    1) main.cpp
    2) render.hpp << This File.
    3) optz.hpp
    4) worldgen.hpp

All rights reserved.
*/

#ifndef RENDER_HPP_INCLUDED
#define RENDER_HPP_INCLUDED

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/glu.h>

#define GL_CLAMP_TO_EDGE 0x812F

const GLfloat PI = 3.14;
bool mass[1000][1000][1000];
GLfloat size = 20.f;
GLfloat sizeDiv2 = size / 2;

GLuint LoadTexture(sf::String name, bool textureFiltring) {
  sf::Image image;
  if (!image.loadFromFile(name))
    return EXIT_FAILURE;

  image.flipVertically();

  GLuint texture = 0;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.getSize().x,
                    image.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE,
                    image.getPixelsPtr());

  if (textureFiltring) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
  } else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  return texture;
}

void createBox(GLuint skybox[], GLfloat size) {
  glBindTexture(GL_TEXTURE_2D, skybox[0]);
  glBegin(GL_QUADS);
  /// front///
  glTexCoord2f(0, 0);
  glVertex3f(-size, -size, -size);
  glTexCoord2f(1, 0);
  glVertex3f(size, -size, -size);
  glTexCoord2f(1, 1);
  glVertex3f(size, size, -size);
  glTexCoord2f(0, 1);
  glVertex3f(-size, size, -size);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, skybox[1]);
  glBegin(GL_QUADS);
  /// back///
  glTexCoord2f(0, 0);
  glVertex3f(size, -size, size);
  glTexCoord2f(1, 0);
  glVertex3f(-size, -size, size);
  glTexCoord2f(1, 1);
  glVertex3f(-size, size, size);
  glTexCoord2f(0, 1);
  glVertex3f(size, size, size);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, skybox[2]);
  glBegin(GL_QUADS);
  /// left///
  glTexCoord2f(0, 0);
  glVertex3f(-size, -size, size);
  glTexCoord2f(1, 0);
  glVertex3f(-size, -size, -size);
  glTexCoord2f(1, 1);
  glVertex3f(-size, size, -size);
  glTexCoord2f(0, 1);
  glVertex3f(-size, size, size);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, skybox[3]);
  glBegin(GL_QUADS);
  /// right///
  glTexCoord2f(0, 0);
  glVertex3f(size, -size, -size);
  glTexCoord2f(1, 0);
  glVertex3f(size, -size, size);
  glTexCoord2f(1, 1);
  glVertex3f(size, size, size);
  glTexCoord2f(0, 1);
  glVertex3f(size, size, -size);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, skybox[4]);
  glBegin(GL_QUADS);
  /// bottom///
  glTexCoord2f(0, 0);
  glVertex3f(-size, -size, size);
  glTexCoord2f(1, 0);
  glVertex3f(size, -size, size);
  glTexCoord2f(1, 1);
  glVertex3f(size, -size, -size);
  glTexCoord2f(0, 1);
  glVertex3f(-size, -size, -size);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, skybox[5]);
  glBegin(GL_QUADS);
  /// top///
  glTexCoord2f(0, 0);
  glVertex3f(-size, size, -size);
  glTexCoord2f(1, 0);
  glVertex3f(size, size, -size);
  glTexCoord2f(1, 1);
  glVertex3f(size, size, size);
  glTexCoord2f(0, 1);
  glVertex3f(-size, size, size);
  glEnd();
}

void createBoxVertexArray(const GLfloat REALSIZE, GLfloat x, GLfloat y, GLfloat z,
                          GLuint texture) {
  glBindTexture(GL_TEXTURE_2D, texture);
  ///все вершины в одном массиве///
  const GLfloat vertices[72] = {
      x - REALSIZE, y + REALSIZE, z - REALSIZE, x - REALSIZE,
      y + REALSIZE, z + REALSIZE, x + REALSIZE, y + REALSIZE,
      z + REALSIZE, x + REALSIZE, y + REALSIZE, z - REALSIZE,

      x - REALSIZE, y - REALSIZE, z + REALSIZE, x + REALSIZE,
      y - REALSIZE, z + REALSIZE, x + REALSIZE, y + REALSIZE,
      z + REALSIZE, x - REALSIZE, y + REALSIZE, z + REALSIZE,

      x - REALSIZE, y - REALSIZE, z - REALSIZE, x - REALSIZE,
      y + REALSIZE, z - REALSIZE, x + REALSIZE, y + REALSIZE,
      z - REALSIZE, x + REALSIZE, y - REALSIZE, z - REALSIZE,

      x - REALSIZE, y - REALSIZE, z - REALSIZE, x + REALSIZE,
      y - REALSIZE, z - REALSIZE, x + REALSIZE, y - REALSIZE,
      z + REALSIZE, x - REALSIZE, y - REALSIZE, z + REALSIZE,

      x + REALSIZE, y - REALSIZE, z - REALSIZE, x + REALSIZE,
      y + REALSIZE, z - REALSIZE, x + REALSIZE, y + REALSIZE,
      z + REALSIZE, x + REALSIZE, y - REALSIZE, z + REALSIZE,

      x - REALSIZE, y - REALSIZE, z - REALSIZE, x - REALSIZE,
      y - REALSIZE, z + REALSIZE, x - REALSIZE, y + REALSIZE,
      z + REALSIZE, x - REALSIZE, y + REALSIZE, z - REALSIZE};

  ///все текстурные координаты в одном массиве///
  static const GLfloat texCoords[48] = {///верхняя грань///
                                      0.25, 1.0, 0.25, 0.0, 0.5, 0.0, 0.5, 1.0,

                                      ///боковые грани///
                                      0.0, 0.0, 0.25, 0.0, 0.25, 1.0, 0.0, 1.0,

                                      0.25, 0.0, 0.25, 1.0, 0.0, 1.0, 0.0, 0.0,

                                      ///нижняя грань///
                                      0.75, 1.0, 0.5, 1.0, 0.5, 0.0, 0.75, 0.0,

                                      ///боковые грани///
                                      0.25, 0.0, 0.25, 1.0, 0.0, 1.0, 0.0, 0.0,

                                      0.0, 0.0, 0.25, 0.0, 0.25, 1.0, 0.0, 1.0};

  static const uint_least16_t numOfVerts = 24;

  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
  glDrawArrays(GL_QUADS, 0, numOfVerts);
}

void createSkyboxVertexArray(const GLfloat REALSIZE, GLfloat x, GLfloat y, GLfloat z,
                          GLuint texture) {
  glBindTexture(GL_TEXTURE_2D, texture);
  ///все вершины в одном массиве///
  const GLfloat vertices[72] = {
      x - REALSIZE, y + REALSIZE, z - REALSIZE, x - REALSIZE,
      y + REALSIZE, z + REALSIZE, x + REALSIZE, y + REALSIZE,
      z + REALSIZE, x + REALSIZE, y + REALSIZE, z - REALSIZE,

      x - REALSIZE, y - REALSIZE, z + REALSIZE, x + REALSIZE,
      y - REALSIZE, z + REALSIZE, x + REALSIZE, y + REALSIZE,
      z + REALSIZE, x - REALSIZE, y + REALSIZE, z + REALSIZE,

      x - REALSIZE, y - REALSIZE, z - REALSIZE, x - REALSIZE,
      y + REALSIZE, z - REALSIZE, x + REALSIZE, y + REALSIZE,
      z - REALSIZE, x + REALSIZE, y - REALSIZE, z - REALSIZE,

      x - REALSIZE, y - REALSIZE, z - REALSIZE, x + REALSIZE,
      y - REALSIZE, z - REALSIZE, x + REALSIZE, y - REALSIZE,
      z + REALSIZE, x - REALSIZE, y - REALSIZE, z + REALSIZE,

      x + REALSIZE, y - REALSIZE, z - REALSIZE, x + REALSIZE,
      y + REALSIZE, z - REALSIZE, x + REALSIZE, y + REALSIZE,
      z + REALSIZE, x + REALSIZE, y - REALSIZE, z + REALSIZE,

      x - REALSIZE, y - REALSIZE, z - REALSIZE, x - REALSIZE,
      y - REALSIZE, z + REALSIZE, x - REALSIZE, y + REALSIZE,
      z + REALSIZE, x - REALSIZE, y + REALSIZE, z - REALSIZE};

  ///все текстурные координаты в одном массиве///
  static const GLfloat texCoords[48] = {///верхняя грань///
                                      0.3334, 0.0, 0.1667, 0.0, 0.1667, 1.0,  0.3334, 1.0,

                                      ///боковые грани///
                                      0.8335, 0.0, 0.6668, 0.0, 0.6668, 1.0, 0.8335, 1.0,

                                      0.8335, 0.0, 0.8335, 1.0, 1.0, 1.0, 1.0, 0.0,

                                      ///нижняя грань///
                                      0.1667, 1.0, 0.1667, 0.0, 0.0, 0.0, 0.0, 1.0,

                                      ///боковые грани///
                                      0.3334, 0.0, 0.3334, 1.0, 0.5001, 1.0, 0.5001, 0.0,

                                      0.6668, 0.0, 0.5001, 0.0, 0.5001, 1.0, 0.6668, 1.0};

  static const uint_least16_t numOfVerts = 24;

  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
  glDrawArrays(GL_QUADS, 0, numOfVerts);
}

bool check(unsigned int x, unsigned int y, unsigned int z) {
  if ((x >= 1000) || (y >= 1000) || (z >= 1000))
    return 0;

  return mass[x][y][z];
}

#endif
