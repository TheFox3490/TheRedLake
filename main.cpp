/*
Copyright Shilnikov Nikita 2018.
This file is a component of the source code of RedMinecraft
which is wrote in C++ and SFML + OpenGL.
//
The list of all source files:
    1) main.cpp << This File.
    2) render.hpp
    3) optz.hpp
    4) worldgen.hpp

All rights reserved.
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/glu.h>
#include <math.h>
#include <time.h>

#include "render.hpp"
#include "optz.hpp"
#include "worldgen.hpp"

using namespace sf;

float angleX, angleY;

class Player {
public:
  bool onGround;
  float x, y, z;
  float dx, dy, dz;
  float w, h, d;
  float speed;

  Player(float x0, float y0, float z0) {
    x = x0;
    y = y0;
    z = z0;
    dx = 0;
    dy = 0;
    dz = 0;
    w = 5;
    h = 20;
    d = 5;
    speed = 5;
    onGround = 0;
  }

  void update(float time) {
    if (!onGround)
      dy -= 1.5 * time;
    onGround = 0;

    x += dx * time;
    collision(dx, 0, 0);
    y += dy * time;
    collision(0, dy, 0);
    z += dz * time;
    collision(0, 0, dz);

    dx = dz = 0;
  }

  void collision(float Dx, float Dy, float Dz) {
    for (int X = (x - w) / size; X < (x + w) / size; X++)
      for (int Y = (y - h) / size; Y < (y + h) / size; Y++)
        for (int Z = (z - d) / size; Z < (z + d) / size; Z++)
          if (check(X, Y, Z)) {
            if (Dx > 0)
              x = X * size - w;
            if (Dx < 0)
              x = X * size + size + w;
            if (Dy > 0) {
              y = Y * size - h;
              dy = 0;
            }
            if (Dy < 0) {
              y = Y * size + size + h;
              onGround = 1;
              dy = 0;
            }
            if (Dz > 0)
              z = Z * size - d;
            if (Dz < 0)
              z = Z * size + size + d;
          }
  }

  void keyboard() {
    if (Keyboard::isKeyPressed(Keyboard::Space))
      if (onGround) {
        onGround = 0;
        dy = 12;
      };

    if (Keyboard::isKeyPressed(Keyboard::W)) {
      dx = -sin(angleX / 180 * PI) * speed;
      dz = -cos(angleX / 180 * PI) * speed;
    }

    if (Keyboard::isKeyPressed(Keyboard::S)) {
      dx = sin(angleX / 180 * PI) * speed;
      dz = cos(angleX / 180 * PI) * speed;
    }

    if (Keyboard::isKeyPressed(Keyboard::D)) {
      dx = sin((angleX + 90) / 180 * PI) * speed;
      dz = cos((angleX + 90) / 180 * PI) * speed;
    }

    if (Keyboard::isKeyPressed(Keyboard::A)) {
      dx = sin((angleX - 90) / 180 * PI) * speed;
      dz = cos((angleX - 90) / 180 * PI) * speed;
    }
  }
};

int main() {
  ///Архиважные настройки окна. Если хоть чуть-чуть изменить, то работать не
  ///будет!!!///
  ContextSettings contextSettings;
  contextSettings.depthBits = 24;
  contextSettings.sRgbCapable = 0;
  RenderWindow window(VideoMode(800, 600), "SFML OpenGL Minecraft C++",
                      Style::Close, contextSettings);
  window.setVerticalSyncEnabled(1);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glDepthMask(GL_TRUE);
  glClearDepth(1.f);
  glDisable(GL_LIGHTING);
  glViewport(0, 0, window.getSize().x, window.getSize().y);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  GLfloat ratio = static_cast<float>(window.getSize().x) / window.getSize().y;
  glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 2000.f);

  window.setMouseCursorVisible(0);

  ///Карта///
  srand(time(NULL));
  setSeed(1 + rand() % 2048);

  uint_least16_t WORLDSIZE = 256;
  uint_least8_t HeightMap[300][300];

  for (int i = 0; i < WORLDSIZE; i++)
    for (int j = 0; j < WORLDSIZE; j++) {
      HeightMap[i][j] =
          roundf((perlin2d(i, j, 0.1, 2) +
                  (perlin2d(i, j, 0.1, 2) *
                    perlin2d(i, j, 0.1, 2))) * 10) + 1;
    }

  for (int i = 0; i < WORLDSIZE; i++)
    for (int j = 0; j < WORLDSIZE; j++)
      for (int h = 1; h < 25; h++) {
        for (int l = h - 1; l > 0; l--) {
          if (HeightMap[i][j] == h) {
            mass[i][h][j] = 1;
          }
        }
      }

  std::cout << "World SEED = " << getSeed() << "\n";

  ///Текстуры///
  Texture t;
  t.loadFromFile("resources/cursor.png");
  Sprite s(t);
  s.setOrigin(8, 8);
  s.setPosition(window.getSize().x / 2, window.getSize().y / 2);

  GLuint boxTextureAtlas = LoadTexture("resources/atlas.png", 0);
  GLuint SkyboxTextureAtlas = LoadTexture("resources/skybox.png", 0);

  Clock clock;
  bool mLeft = 0, mRight = 0;
  Player p(WORLDSIZE / 2 * size, 400, WORLDSIZE / 2 * size);

  ///Основной Цикл///
  while (window.isOpen()) {
    float time = clock.getElapsedTime().asMilliseconds();
    clock.restart();
    time = time / 50;
    if (time > 3)
      time = 3;

    Event event;
    while (window.pollEvent(event)) {
      if (event.type == Event::Closed)
        window.close();

      if ((event.type == Event::KeyPressed) &&
          (event.key.code == Keyboard::Escape))
        window.close();

      if (event.type == Event::MouseButtonPressed) {
        if (event.key.code == Mouse::Right)
          mRight = 1;
        if (event.key.code == Mouse::Left)
          mLeft = 1;
      }
    }

    glClear(GL_DEPTH_BUFFER_BIT);

    p.keyboard();
    p.update(time);

    ///Работа С Мышью///
    int xt = window.getSize().x / 2;
    int yt = window.getSize().y / 2;

    /// 4 — Чувствительность Мышки///
    angleX += (xt - Mouse::getPosition(window).x) / 4;
    angleY += (yt - Mouse::getPosition(window).y) / 4;

    if (angleY < -89.0) {
      angleY = -89.0;
    }
    if (angleY > 89.0) {
      angleY = 89.0;
    }

    Mouse::setPosition(sf::Vector2i(xt, yt), window);

    if (mRight || mLeft) {
      float x = p.x;
      float y = p.y + p.h / 2;
      float z = p.z;

      int X, Y, Z, oldX, oldY, oldZ;
      int dist = 0;
      /// 120 - Это Радиус, В Котором Мы Можем Ломать Блоки///
      while (dist < 120) {
        dist++;

        x += -sin(angleX / 180 * PI);
        X = x / size;
        y += tan(angleY / 180 * PI);
        Y = y / size;
        z += -cos(angleX / 180 * PI);
        Z = z / size;

        if (check(X, Y, Z))
          if (mLeft) {
            mass[X][Y][Z] = 0;
            break;
          } else {
            mass[oldX][oldY][oldZ] = 1;
            break;
          }

        oldX = X;
        oldY = Y;
        oldZ = Z;
      }
    }

    mLeft = mRight = 0;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(p.x, p.y + p.h / 2, p.z, p.x - sin(angleX / 180 * PI),
              p.y + p.h / 2 + (tan(angleY / 180 * PI)),
              p.z - cos(angleX / 180 * PI), 0, 1, 0);

    ///Рисуем Коробки///
    int R = 45;
    int X = p.x / size;
    int Y = p.y / size;
    int Z = p.z / size;

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    ExtractFrustum();

    for (int x = X - R; x < X + R; x++)
      for (int y = Y - R; y < Y + R; y++)
        for (int z = Z - R; z < Z + R; z++) {
          if (!check(x, y, z))
            continue;
          int cordX = size * x + sizeDiv2;
          int cordY = size * y + sizeDiv2;
          int cordZ = size * z + sizeDiv2;

          if (CubeInFrustum(sizeDiv2, cordX, cordY, cordZ))
            createBoxVertexArray(sizeDiv2, cordX, cordY, cordZ, boxTextureAtlas);
        }

    createSkyboxVertexArray(1000, p.x, p.y, p.z, SkyboxTextureAtlas);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    window.pushGLStates();
    window.draw(s); ///Рисуем Курсор///
    window.popGLStates();

    window.display();
  }
  return 0;
}
