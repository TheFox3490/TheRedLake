/*
Copyright Shilnikov Nikita 2018.
This file is a component of the source code of RedMinecraft
which is wrote in C++ and SFML + OpenGL.

The list of all source files:
    1) main.cpp
    2) render.hpp
    3) optz.hpp << This File.
    4) worldgen.hpp

All rights reserved.
*/

#ifndef OPTZ_HPP_INCLUDED
#define OPTZ_HPP_INCLUDED

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/glu.h>

GLfloat frustum[6][4];

void ExtractFrustum() {
  GLfloat proj[16];
  GLfloat modl[16];
  GLfloat clip[16];
  GLfloat t;

  /// Узнаем текущую матрицу PROJECTION ///
  glGetFloatv(GL_PROJECTION_MATRIX, proj);
  /// Узнаем текущую матрицу MODELVIEW ///
  glGetFloatv(GL_MODELVIEW_MATRIX, modl);

  /// Комбинируем матрицы(перемножаем) ///
  clip[0] = modl[0] * proj[0] + modl[1] * proj[4] + modl[2] * proj[8] +
            modl[3] * proj[12];
  clip[1] = modl[0] * proj[1] + modl[1] * proj[5] + modl[2] * proj[9] +
            modl[3] * proj[13];
  clip[2] = modl[0] * proj[2] + modl[1] * proj[6] + modl[2] * proj[10] +
            modl[3] * proj[14];
  clip[3] = modl[0] * proj[3] + modl[1] * proj[7] + modl[2] * proj[11] +
            modl[3] * proj[15];
  clip[4] = modl[4] * proj[0] + modl[5] * proj[4] + modl[6] * proj[8] +
            modl[7] * proj[12];
  clip[5] = modl[4] * proj[1] + modl[5] * proj[5] + modl[6] * proj[9] +
            modl[7] * proj[13];
  clip[6] = modl[4] * proj[2] + modl[5] * proj[6] + modl[6] * proj[10] +
            modl[7] * proj[14];
  clip[7] = modl[4] * proj[3] + modl[5] * proj[7] + modl[6] * proj[11] +
            modl[7] * proj[15];
  clip[8] = modl[8] * proj[0] + modl[9] * proj[4] + modl[10] * proj[8] +
            modl[11] * proj[12];
  clip[9] = modl[8] * proj[1] + modl[9] * proj[5] + modl[10] * proj[9] +
            modl[11] * proj[13];
  clip[10] = modl[8] * proj[2] + modl[9] * proj[6] + modl[10] * proj[10] +
             modl[11] * proj[14];
  clip[11] = modl[8] * proj[3] + modl[9] * proj[7] + modl[10] * proj[11] +
             modl[11] * proj[15];
  clip[12] = modl[12] * proj[0] + modl[13] * proj[4] + modl[14] * proj[8] +
             modl[15] * proj[12];
  clip[13] = modl[12] * proj[1] + modl[13] * proj[5] + modl[14] * proj[9] +
             modl[15] * proj[13];
  clip[14] = modl[12] * proj[2] + modl[13] * proj[6] + modl[14] * proj[10] +
             modl[15] * proj[14];
  clip[15] = modl[12] * proj[3] + modl[13] * proj[7] + modl[14] * proj[11] +
             modl[15] * proj[15];

  /// Находим A, B, C, D для ПРАВОЙ плоскости ///
  frustum[0][0] = clip[3] - clip[0];
  frustum[0][1] = clip[7] - clip[4];
  frustum[0][2] = clip[11] - clip[8];
  frustum[0][3] = clip[15] - clip[12];

  /// Приводим уравнение плоскости к нормальному виду ///
  t = sqrt(frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] +
           frustum[0][2] * frustum[0][2]);
  frustum[0][0] /= t;
  frustum[0][1] /= t;
  frustum[0][2] /= t;
  frustum[0][3] /= t;

  /// Находим A, B, C, D для ЛЕВОЙ плоскости ///
  frustum[1][0] = clip[3] + clip[0];
  frustum[1][1] = clip[7] + clip[4];
  frustum[1][2] = clip[11] + clip[8];
  frustum[1][3] = clip[15] + clip[12];

  /// Приводим уравнение плоскости к нормальному виду ///
  t = sqrt(frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] +
           frustum[1][2] * frustum[1][2]);
  frustum[1][0] /= t;
  frustum[1][1] /= t;
  frustum[1][2] /= t;
  frustum[1][3] /= t;

  ///Находим A, B, C, D для НИЖНЕЙ плоскости ///
  frustum[2][0] = clip[3] + clip[1];
  frustum[2][1] = clip[7] + clip[5];
  frustum[2][2] = clip[11] + clip[9];
  frustum[2][3] = clip[15] + clip[13];

  /// Приводим уравнение плоскости к нормальному ///
  t = sqrt(frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] +
           frustum[2][2] * frustum[2][2]);
  frustum[2][0] /= t;
  frustum[2][1] /= t;
  frustum[2][2] /= t;
  frustum[2][3] /= t;

  /// ВЕРХНЯЯ плоскость ///
  frustum[3][0] = clip[3] - clip[1];
  frustum[3][1] = clip[7] - clip[5];
  frustum[3][2] = clip[11] - clip[9];
  frustum[3][3] = clip[15] - clip[13];

  /// Нормальный вид ///
  t = sqrt(frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] +
           frustum[3][2] * frustum[3][2]);
  frustum[3][0] /= t;
  frustum[3][1] /= t;
  frustum[3][2] /= t;
  frustum[3][3] /= t;

  /// ЗАДНЯЯ плоскость ///
  frustum[4][0] = clip[3] - clip[2];
  frustum[4][1] = clip[7] - clip[6];
  frustum[4][2] = clip[11] - clip[10];
  frustum[4][3] = clip[15] - clip[14];

  /// Нормальный вид ///
  t = sqrt(frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] +
           frustum[4][2] * frustum[4][2]);
  frustum[4][0] /= t;
  frustum[4][1] /= t;
  frustum[4][2] /= t;
  frustum[4][3] /= t;

  /// ПЕРЕДНЯЯ плоскость ///
  frustum[5][0] = clip[3] + clip[2];
  frustum[5][1] = clip[7] + clip[6];
  frustum[5][2] = clip[11] + clip[10];
  frustum[5][3] = clip[15] + clip[14];

  /// Нормальный вид ///
  t = sqrt(frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] +
           frustum[5][2] * frustum[5][2]);
  frustum[5][0] /= t;
  frustum[5][1] /= t;
  frustum[5][2] /= t;
  frustum[5][3] /= t;
}

bool CubeInFrustum(float REALSIZE, GLfloat x, GLfloat y, GLfloat z) {
  int p;
  for (p = 0; p < 6; p++) {
    if (frustum[p][0] * (x - REALSIZE) + frustum[p][1] * (y - REALSIZE) +
            frustum[p][2] * (z - REALSIZE) + frustum[p][3] > 0)
      continue;
    if (frustum[p][0] * (x + REALSIZE) + frustum[p][1] * (y - REALSIZE) +
            frustum[p][2] * (z - REALSIZE) + frustum[p][3] > 0)
      continue;
    if (frustum[p][0] * (x - REALSIZE) + frustum[p][1] * (y + REALSIZE) +
            frustum[p][2] * (z - REALSIZE) + frustum[p][3] > 0)
      continue;
    if (frustum[p][0] * (x + REALSIZE) + frustum[p][1] * (y + REALSIZE) +
            frustum[p][2] * (z - REALSIZE) + frustum[p][3] > 0)
      continue;
    if (frustum[p][0] * (x - REALSIZE) + frustum[p][1] * (y - REALSIZE) +
            frustum[p][2] * (z + REALSIZE) + frustum[p][3] > 0)
      continue;
    if (frustum[p][0] * (x + REALSIZE) + frustum[p][1] * (y - REALSIZE) +
            frustum[p][2] * (z + REALSIZE) + frustum[p][3] > 0)
      continue;
    if (frustum[p][0] * (x - REALSIZE) + frustum[p][1] * (y + REALSIZE) +
            frustum[p][2] * (z + REALSIZE) + frustum[p][3] > 0)
      continue;
    if (frustum[p][0] * (x + REALSIZE) + frustum[p][1] * (y + REALSIZE) +
            frustum[p][2] * (z + REALSIZE) + frustum[p][3] > 0)
      continue;
    return 0;
  }
  return 1;
}

bool SphereInFrustum(GLfloat radius, GLfloat x, GLfloat y, GLfloat z) {
  int p;
  for (p = 0; p < 6; p++)
    if (frustum[p][0] * x + frustum[p][1] * y + frustum[p][2] * z +
            frustum[p][3] <= -radius)
                return 0;
  return 1;
}

int getFPS(const GLfloat time) {
     return round(1000.0f / time);
}

#endif
