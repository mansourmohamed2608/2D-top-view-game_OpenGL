
#include <stdio.h>
#include <math.h>
#include <random>

#include <glut.h>
#include <cstdlib>
#include <cmath>
#include <string>
#include <ctime>

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 600;
const float PLAYER_SIZE = 30.0f;
const float TARGET_SIZE = 20.0f;
const float OBSTACLE_SIZE = 20.0f;
const float GOAL_SIZE = 20.0f;
const float POWERUP_SIZE = 20.0f;
float PLAYER_SPEED = 2.0f;
const int NUM_TARGETS = 9;
const int NUM_OBSTACLES = 10;
float targetPositions[NUM_TARGETS][2];
float obstaclePositions[NUM_OBSTACLES][2];

const int NUM_PowerUps1 = 2;
float PowerUpsPositions1[NUM_PowerUps1][2];
const int NUM_PowerUps2 = 2;
float PowerUpsPositions2[NUM_PowerUps2][2];

float powerupX2, powerupY2;
float powerupX1, powerupY1;
float playerX = WINDOW_WIDTH / 2;
float playerY = WINDOW_HEIGHT / 2;
float targetX, targetY;
float obstacleX, obstacleY;
float playerHealth = 100;
float infoheight = WINDOW_HEIGHT - 55;
float infowidth = WINDOW_WIDTH - 10;
bool gameOver = false;
int playerScore = 0;
int countdown = 30;
bool gameStarted = false;
float goalX = 50;
float goalY = 50;

bool isPowerUpActive1 = false;

time_t powerUpActivationTime;
int powerUpDuration1 = 5;
int powerUpDuration2 = 5;
int score = 10;
bool playerWon = false;
bool isPowerUpActive2 = false;
time_t powerUp2ActivationTime;

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> postX(0, WINDOW_WIDTH - static_cast<int>(TARGET_SIZE));
    std::uniform_int_distribution<int> postY(0, infoheight - static_cast<int>(TARGET_SIZE));
    std::uniform_int_distribution<int> posoX(0, WINDOW_WIDTH - static_cast<int>(OBSTACLE_SIZE));
    std::uniform_int_distribution<int> posoY(0, infoheight - static_cast<int>(OBSTACLE_SIZE));
    std::uniform_int_distribution<int> posp1X(0, WINDOW_WIDTH - static_cast<int>(POWERUP_SIZE));
    std::uniform_int_distribution<int> posp1Y(0, infoheight - static_cast<int>(POWERUP_SIZE));
    std::uniform_int_distribution<int> posp2X(0, WINDOW_WIDTH - static_cast<int>(POWERUP_SIZE));
    std::uniform_int_distribution<int> posp2Y(0, infoheight - static_cast<int>(POWERUP_SIZE));

    float playerLeft = playerX;
    float playerRight = playerX + PLAYER_SIZE;
    float playerTop = playerY + PLAYER_SIZE;
    float playerBottom = playerY;


    for (int i = 0; i < NUM_TARGETS; ++i) {

        do {
            targetPositions[i][0] = postX(gen);
            targetPositions[i][1] = postY(gen);
        } while (
            targetPositions[i][0] < playerRight &&
            targetPositions[i][0] + TARGET_SIZE > playerLeft &&
            targetPositions[i][1] < playerTop &&
            targetPositions[i][1] + TARGET_SIZE > playerBottom
            );
    }


    for (int i = 0; i < NUM_OBSTACLES; ++i) {

        do {
            obstaclePositions[i][0] = posoX(gen);
            obstaclePositions[i][1] = posoY(gen);
        } while (
            obstaclePositions[i][0] < playerRight &&
            obstaclePositions[i][0] + OBSTACLE_SIZE > playerLeft &&
            obstaclePositions[i][1] < playerTop &&
            obstaclePositions[i][1] + OBSTACLE_SIZE > playerBottom
            );
    }
    for (int i = 0; i < NUM_PowerUps1; ++i) {

        do {
            PowerUpsPositions1[i][0] = posp1X(gen);
            PowerUpsPositions1[i][1] = posp1Y(gen);
        } while (
            PowerUpsPositions1[i][0] < playerRight &&
            PowerUpsPositions1[i][0] + POWERUP_SIZE > playerLeft &&
            PowerUpsPositions1[i][1] < playerTop &&
            PowerUpsPositions1[i][1] + POWERUP_SIZE > playerBottom
            );
    }
    for (int i = 0; i < NUM_PowerUps2; ++i) {

        do {
            PowerUpsPositions2[i][0] = posp2X(gen);
            PowerUpsPositions2[i][1] = posp2Y(gen);
        } while (
            PowerUpsPositions2[i][0] < playerRight &&
            PowerUpsPositions2[i][0] + POWERUP_SIZE > playerLeft &&
            PowerUpsPositions2[i][1] < playerTop &&
            PowerUpsPositions2[i][1] + POWERUP_SIZE > playerBottom
            );
    }
}
void activatePowerUp() {
    isPowerUpActive1 = true;
    powerUpActivationTime = time(nullptr);
}
void checkPowerUpDuration() {
    if (isPowerUpActive1) {
        time_t currentTime = time(nullptr);

        if (currentTime - powerUpActivationTime >= powerUpDuration1) {
            isPowerUpActive1 = false;

        }
    }
}
void activatePowerUp2() {
    isPowerUpActive2 = true;
    powerUp2ActivationTime = time(nullptr);
}
void checkPowerUpDuration2() {
    if (isPowerUpActive2) {
        time_t currentTime = time(nullptr);
        if (currentTime - powerUp2ActivationTime >= powerUpDuration2) {
            isPowerUpActive2 = false;

        }
    }
}

void drawCircle(float x, float y, float radius, int segments) {

    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * acos(-1) * static_cast<float>(i) / static_cast<float>(segments);
        float xPos = x + radius * std::cos(angle);
        float yPos = y + radius * std::sin(angle);
        glVertex2f(xPos, yPos);
    }
    glEnd();
}
void drawSceneBoundaries() {
    glLineWidth(5.0f);


    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(0, 0);
    glVertex2f(0, WINDOW_HEIGHT);
    glEnd();


    glBegin(GL_LINES);
    glVertex2f(WINDOW_WIDTH, 0);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnd();


    glBegin(GL_LINES);
    glVertex2f(0, WINDOW_HEIGHT);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(0, 0);
    glVertex2f(WINDOW_WIDTH, 0);
    glEnd();

    glLineWidth(1.0f);


    float cornerRadius = 30.0f;
    int numSegments = 30;

    glColor3f(1.0f, 0.0f, 0.0f);
    drawCircle(0, 0, cornerRadius, numSegments);
    drawCircle(WINDOW_WIDTH, 0, cornerRadius, numSegments);
    drawCircle(0, WINDOW_HEIGHT, cornerRadius, numSegments);
    drawCircle(WINDOW_WIDTH, WINDOW_HEIGHT, cornerRadius, numSegments);
}




void drawCylinder(float x, float y, float radius, float height, int segments) {

    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * acos(-1) * static_cast<float>(i) / static_cast<float>(segments);
        float xPos = x + radius * std::cos(angle);
        float yPos = y + radius * std::sin(angle);

        float nextXPos = x + radius * std::cos(angle + 2.0f * acos(-1) / static_cast<float>(segments));
        float nextYPos = y + radius * std::sin(angle + 2.0f * acos(-1) / static_cast<float>(segments));


        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= segments; ++j) {
            float segmentAngle = 2.0f * acos(-1) * static_cast<float>(j) / static_cast<float>(segments);
            float circleX = xPos + radius * std::cos(segmentAngle);
            float circleY = yPos + radius * std::sin(segmentAngle);
            glVertex2f(circleX, circleY);

            float nextCircleX = nextXPos + radius * std::cos(segmentAngle);
            float nextCircleY = nextYPos + radius * std::sin(segmentAngle);
            glVertex2f(nextCircleX, nextCircleY);
        }
        glEnd();
    }
}
void drawSquare(float x, float y, float size) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + size, y);
    glVertex2f(x + size, y + size);
    glVertex2f(x, y + size);
    glEnd();
}
void drawHuman(float x, float y) {

    drawCircle(x, y + PLAYER_SIZE, PLAYER_SIZE / 2, 30);


    glBegin(GL_TRIANGLES);
    glVertex2f(x - PLAYER_SIZE / 2, y);
    glVertex2f(x + PLAYER_SIZE / 2, y);
    glVertex2f(x, y + PLAYER_SIZE);
    glEnd();


    drawCylinder(x - PLAYER_SIZE / 3, y, PLAYER_SIZE / 10, PLAYER_SIZE / 2, 20);
    drawCylinder(x + PLAYER_SIZE / 3, y, PLAYER_SIZE / 10, PLAYER_SIZE / 2, 20);


    drawSquare(x - PLAYER_SIZE, y + PLAYER_SIZE / 2, PLAYER_SIZE / 4);
    drawSquare(x + PLAYER_SIZE, y + PLAYER_SIZE / 2, PLAYER_SIZE / 4);


    glColor3f(1.0f, 1.0f, 1.0f);


    drawCircle(x - PLAYER_SIZE / 4, y + PLAYER_SIZE + PLAYER_SIZE / 4, PLAYER_SIZE / 10, 20);
    drawCircle(x + PLAYER_SIZE / 4, y + PLAYER_SIZE + PLAYER_SIZE / 4, PLAYER_SIZE / 10, 20);


    glColor3f(0.0f, 0.0f, 0.0f);
}







void drawHealthBar() {

    glColor3f(0.0f, 0.5f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(20, WINDOW_HEIGHT - 20);
    glVertex2f(20 + playerHealth * 2, WINDOW_HEIGHT - 20);
    glVertex2f(20 + playerHealth * 2, WINDOW_HEIGHT - 40);
    glVertex2f(20, WINDOW_HEIGHT - 40);
    glEnd();


    glLineWidth(2.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(20, WINDOW_HEIGHT - 20);
    glVertex2f(20 + playerHealth * 2, WINDOW_HEIGHT - 20);
    glVertex2f(20 + playerHealth * 2, WINDOW_HEIGHT - 40);
    glVertex2f(20, WINDOW_HEIGHT - 40);
    glEnd();
    glLineWidth(1.0f);
}
void drawScore() {

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(WINDOW_WIDTH / 2 - 55, WINDOW_HEIGHT - 51);
    glVertex2f(WINDOW_WIDTH / 2 + 60, WINDOW_HEIGHT - 51);
    glVertex2f(WINDOW_WIDTH / 2 + 60, WINDOW_HEIGHT);
    glVertex2f(WINDOW_WIDTH / 2 - 55, WINDOW_HEIGHT);
    glEnd();
    glLineWidth(1.0f);




    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(WINDOW_WIDTH / 2 - 40, WINDOW_HEIGHT - 40);

    std::string scoreString = "Score: " + std::to_string(playerScore);
    for (char c : scoreString) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
}



void drawObstacle(float x, float y, float size) {

    glColor3f(0.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y + size);
    glVertex2f(x - size / 2, y - size / 2);
    glVertex2f(x + size / 2, y - size / 2);
    glEnd();


    float circleRadius = size / 2.5;
    drawCircle(x, y + size + circleRadius, circleRadius, 30);

}
void drawTarget(float x, float y, float size, float rotationAngle) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(rotationAngle, 0, 0, 1);


    float squareSize = size / 1.8;
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-squareSize / 2, -size - squareSize / 2);
    glVertex2f(squareSize / 2, -size - squareSize / 2);
    glVertex2f(squareSize / 2, -size + squareSize / 2);
    glVertex2f(-squareSize / 2, -size + squareSize / 2);
    glEnd();


    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0, 0);
    glVertex2f(-size / 2, -size);
    glVertex2f(size / 2, -size);
    glEnd();

    float circleRadius = size / 2.5;
    drawCircle(0, circleRadius, circleRadius, 30);
    glPopMatrix();
}




void drawGoal(float x, float y, float size, float rotationAngle) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(rotationAngle, 0, 0, 1);


    float squareSize = size / 1.8;
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-squareSize / 2, -size - squareSize / 2);
    glVertex2f(squareSize / 2, -size - squareSize / 2);
    glVertex2f(squareSize / 2, -size + squareSize / 2);
    glVertex2f(-squareSize / 2, -size + squareSize / 2);
    glEnd();


    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-squareSize / 2, -size - squareSize / 2);
    glVertex2f(squareSize / 2, -size - squareSize / 2);
    glVertex2f(squareSize / 2, -size + squareSize / 2);
    glVertex2f(-squareSize / 2, -size + squareSize / 2);
    glEnd();

    float circleRadius = size / 2.5;
    glColor3f(0.0f, 1.0f, 0.0f);
    drawCircle(0, circleRadius, circleRadius, 30);


    float pinkCircleRadius = size / 2.0;
    glColor3f(1.0f, 0.41f, 0.71f);
    drawCircle(0, circleRadius * 2.5, pinkCircleRadius, 30);


    glColor3f(0.5f, 0.0f, 0.5f);
    glBegin(GL_QUADS);
    float purpleSquareSize = size / 2.2;
    glVertex2f(-purpleSquareSize / 2, circleRadius * 4.5 - purpleSquareSize / 2);
    glVertex2f(purpleSquareSize / 2, circleRadius * 4.5 - purpleSquareSize / 2);
    glVertex2f(purpleSquareSize / 2, circleRadius * 4.5 + purpleSquareSize / 2);
    glVertex2f(-purpleSquareSize / 2, circleRadius * 4.5 + purpleSquareSize / 2);
    glEnd();

    glPopMatrix();
}

void drawPowerUp1(float x, float y, float size, float rotationAngle) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(rotationAngle, 0, 0, 1);

    float squareSize = size / 1.8;
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-squareSize / 2, -size - squareSize / 2);
    glVertex2f(squareSize / 2, -size - squareSize / 2);
    glVertex2f(squareSize / 2, -size + squareSize / 2);
    glVertex2f(-squareSize / 2, -size + squareSize / 2);
    glEnd();


    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-squareSize / 2, -size / 2 - squareSize / 2);
    glVertex2f(squareSize / 2, -size / 2 - squareSize / 2);
    glVertex2f(squareSize / 2, -size / 2 + squareSize / 2);
    glVertex2f(-squareSize / 2, -size / 2 + squareSize / 2);
    glEnd();


    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-squareSize / 2, -size / 2 - squareSize / 2);
    glVertex2f(squareSize / 2, -size / 2 - squareSize / 2);
    glVertex2f(squareSize / 2, -size / 2 + squareSize / 2);
    glVertex2f(-squareSize / 2, -size / 2 + squareSize / 2);
    glEnd();


    float circleRadius = size / 2.5;
    glColor3f(0.0f, 1.0f, 0.0f);
    drawCircle(0, circleRadius, circleRadius, 30);


    glColor3f(1.0f, 0.41f, 0.71f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0, -size * 2.2);
    glVertex2f(-size, -size);
    glVertex2f(size, -size);
    glEnd();

    glPopMatrix();
}


void drawPowerUp2(float x, float y, float size, float rotationAngle) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(rotationAngle, 0, 0, 1);


    float greenScale = 1.5;


    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0, -size * 2.2);
    glVertex2f(-size, -size);
    glVertex2f(size, -size);
    glEnd();


    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0, -size * 1.2);
    glVertex2f(-size, 0);
    glVertex2f(size, 0);
    glEnd();


    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0, 0.8 * size * greenScale * 2);
    glVertex2f(-0.8 * size * greenScale, size * greenScale * 2);
    glVertex2f(0.8 * size * greenScale, size * greenScale * 2);
    glEnd();


    glColor3f(0.68f, 0.85f, 0.90f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0, size * 1.8);
    glVertex2f(-size * 0.9, size * 0.9);
    glVertex2f(size * 0.9, size * 0.9);
    glEnd();

    glPopMatrix();
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawSceneBoundaries();

    float rotationAngle = 1.0f;
    static float targetRotation = 0.0f;
    static float goalRotation = 0.0f;
    static float powerup1Rotation = 0.0f;
    static float powerup2Rotation = 0.0f;
    targetRotation += rotationAngle;
    goalRotation += rotationAngle;
    powerup1Rotation += rotationAngle;
    powerup2Rotation += rotationAngle;


    for (int i = 0; i < NUM_TARGETS; ++i) {
        drawTarget(targetPositions[i][0], targetPositions[i][1], TARGET_SIZE, targetRotation);
    }


    for (int i = 0; i < NUM_OBSTACLES; ++i) {
        drawObstacle(obstaclePositions[i][0], obstaclePositions[i][1], OBSTACLE_SIZE);
    }
    for (int i = 0; i < NUM_PowerUps1; ++i) {
        drawPowerUp1(PowerUpsPositions1[i][0], PowerUpsPositions1[i][1], POWERUP_SIZE, powerup1Rotation);
    }
    for (int i = 0; i < NUM_PowerUps2; ++i) {
        drawPowerUp2(PowerUpsPositions2[i][0], PowerUpsPositions2[i][1], POWERUP_SIZE, powerup2Rotation);
    }





    drawGoal(goalX, goalY, GOAL_SIZE, goalRotation);

    drawHuman(playerX, playerY);
    if (playerHealth > 0) {
        drawHealthBar();
    }

    drawScore();
    if (isPowerUpActive1) {
        PLAYER_SPEED = 20.0f;
    }
    if (!isPowerUpActive1) {
        PLAYER_SPEED = 2.0f;
    }
    if (isPowerUpActive2) {
        score = 20;
    }
    if (!isPowerUpActive2) {
        score = 10;
    }
    if (gameOver) {
        glColor3f(1.0f, 0.0f, 0.0f);


        int textWidth = 80;
        int textHeight = 80;
        int xPos = (WINDOW_WIDTH - textWidth) / 2;
        int yPos = WINDOW_HEIGHT / 2 - textHeight / 2;

        const char* gameOverText = "G am e O ver";
        for (int i = 0; i < strlen(gameOverText); ++i) {
            glRasterPos2f(xPos + i * 10, yPos);
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, gameOverText[i]);
        }
    }
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(WINDOW_WIDTH - 205, WINDOW_HEIGHT - 51);
    glVertex2f(WINDOW_WIDTH - 100, WINDOW_HEIGHT - 51);
    glVertex2f(WINDOW_WIDTH - 100, WINDOW_HEIGHT);
    glVertex2f(WINDOW_WIDTH - 205, WINDOW_HEIGHT);
    glEnd();
    glLineWidth(1.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(WINDOW_WIDTH - 200, WINDOW_HEIGHT - 40);


    std::string countdownString = "Time: " + std::to_string(countdown);
    for (char c : countdownString) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
    if (!gameOver && !playerWon && gameStarted) {
        if (playerX >= goalX && playerX <= goalX + GOAL_SIZE &&
            playerY >= goalY && playerY <= goalY + GOAL_SIZE) {
            playerWon = true;

            glutPostRedisplay();
        }
    }

    if (playerWon) {
        countdown = 0;
        glColor3f(0.0f, 1.0f, 0.0f);
        int xPos = (WINDOW_WIDTH - 160) / 2;
        int yPos = WINDOW_HEIGHT / 2 - 10;


        const char* winMessage = "You Win!";
        for (int i = 0; i < strlen(winMessage); ++i) {
            glRasterPos2f(xPos + i * 10, yPos);
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, winMessage[i]);
        }
    }
    glutSwapBuffers();
}





void updateTargets(int score) {

    for (int i = 0; i < NUM_TARGETS; ++i) {
        float targetLeft = targetPositions[i][0];
        float targetRight = targetPositions[i][0] + TARGET_SIZE;
        float targetTop = targetPositions[i][1] + TARGET_SIZE;
        float targetBottom = targetPositions[i][1];

        float playerLeft = playerX - PLAYER_SIZE / 2;
        float playerRight = playerX + PLAYER_SIZE / 2;
        float playerTop = playerY + PLAYER_SIZE / 2;
        float playerBottom = playerY - PLAYER_SIZE / 2;

        if (!(playerLeft > targetRight || playerRight < targetLeft || playerTop < targetBottom || playerBottom > targetTop)) {

            playerScore += score;
            targetPositions[i][0] = -TARGET_SIZE;
            targetPositions[i][1] = -TARGET_SIZE;
        }
    }

}
void updateObstacles() {
    int collisionDamage = 20;
    for (int i = 0; i < NUM_OBSTACLES; ++i) {
        float targetLeft = obstaclePositions[i][0];
        float targetRight = obstaclePositions[i][0] + OBSTACLE_SIZE;
        float targetTop = obstaclePositions[i][1] + OBSTACLE_SIZE;
        float targetBottom = obstaclePositions[i][1];

        float playerLeft = playerX - PLAYER_SIZE / 2;
        float playerRight = playerX + PLAYER_SIZE / 2;
        float playerTop = playerY + PLAYER_SIZE / 2;
        float playerBottom = playerY - PLAYER_SIZE / 2;

        if (!(playerLeft > targetRight || playerRight < targetLeft || playerTop < targetBottom || playerBottom > targetTop)) {

            playerHealth -= collisionDamage;
            if (playerHealth <= 0) {
                gameOver = true;
            }
            obstaclePositions[i][0] = -OBSTACLE_SIZE;
            obstaclePositions[i][1] = -OBSTACLE_SIZE;
        }
    }
}
void updatePowerUps1() {

    for (int i = 0; i < NUM_PowerUps1; ++i) {
        float targetLeft = PowerUpsPositions1[i][0];
        float targetRight = PowerUpsPositions1[i][0] + POWERUP_SIZE;
        float targetTop = PowerUpsPositions1[i][1] + POWERUP_SIZE;
        float targetBottom = PowerUpsPositions1[i][1];

        float playerLeft = playerX - PLAYER_SIZE / 2;
        float playerRight = playerX + PLAYER_SIZE / 2;
        float playerTop = playerY + PLAYER_SIZE / 2;
        float playerBottom = playerY - PLAYER_SIZE / 2;

        if (!(playerLeft > targetRight || playerRight < targetLeft || playerTop < targetBottom || playerBottom > targetTop)) {

            activatePowerUp();
            PowerUpsPositions1[i][0] = -POWERUP_SIZE;
            PowerUpsPositions1[i][1] = -POWERUP_SIZE;
        }
    }

}
void updatePowerUps2() {

    for (int i = 0; i < NUM_PowerUps2; ++i) {
        float targetLeft = PowerUpsPositions2[i][0];
        float targetRight = PowerUpsPositions2[i][0] + POWERUP_SIZE;
        float targetTop = PowerUpsPositions2[i][1] + POWERUP_SIZE;
        float targetBottom = PowerUpsPositions2[i][1];

        float playerLeft = playerX - PLAYER_SIZE / 2;
        float playerRight = playerX + PLAYER_SIZE / 2;
        float playerTop = playerY + PLAYER_SIZE / 2;
        float playerBottom = playerY - PLAYER_SIZE / 2;

        if (!(playerLeft > targetRight || playerRight < targetLeft || playerTop < targetBottom || playerBottom > targetTop)) {

            activatePowerUp2();

            PowerUpsPositions2[i][0] = -POWERUP_SIZE;
            PowerUpsPositions2[i][1] = -POWERUP_SIZE;
        }
    }

}

void countdownTimer(int value) {
    if (gameStarted && countdown > 0 && !gameOver) {
        countdown--;

        glutPostRedisplay();
        glutTimerFunc(1000, countdownTimer, 0);

        if (playerWon) {
            // Player has won, stop the timer
            return;
        }
    }
    else if (countdown == 0) {
        gameOver = true;
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (playerWon) {
        return;
    }
    if (!gameOver && !gameStarted) {
        gameStarted = true;
        glutTimerFunc(1000, countdownTimer, 0);
    }
    if (!gameOver) {
        switch (key) {
        case 27:
            exit(0);
            break;
        case 'w':
        case 'W':
            if (playerY + PLAYER_SPEED < WINDOW_HEIGHT - PLAYER_SIZE / 2) {
                playerY += PLAYER_SPEED;
            }
            break;
        case 's':
        case 'S':
            if (playerY - PLAYER_SPEED > PLAYER_SIZE / 2) {
                playerY -= PLAYER_SPEED;
            }
            break;
        case 'a':
        case 'A':
            if (playerX - PLAYER_SPEED > PLAYER_SIZE / 2) {
                playerX -= PLAYER_SPEED;
            }
            break;
        case 'd':
        case 'D':
            if (playerX + PLAYER_SPEED < WINDOW_WIDTH - PLAYER_SIZE / 2) {
                playerX += PLAYER_SPEED;
            }
            break;
        }
    }
    if (playerX < 0) {
        playerX = 0;
    }
    if (playerX > WINDOW_WIDTH - PLAYER_SIZE - 10) {
        playerHealth -= 0.5;
        if (playerHealth <= 0) {
            gameOver = true;
        }
        playerX = WINDOW_WIDTH - PLAYER_SIZE - 10;
    }
    if (playerY < 0) {
        playerY = 0;
    }
    if (playerY > WINDOW_HEIGHT - PLAYER_SIZE - 15) {
        playerHealth -= 0.5;
        if (playerHealth <= 0) {
            gameOver = true;
        }
        playerY = WINDOW_HEIGHT - PLAYER_SIZE - 15;
        printf("health: %f\n", playerHealth);
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    if (playerWon) {
        return;
    }
    if (!gameOver && !gameStarted) {
        gameStarted = true;
        glutTimerFunc(1000, countdownTimer, 0);
    }
    if (!gameOver) {
        switch (key) {
        case GLUT_KEY_UP:
            if (playerY + PLAYER_SPEED < WINDOW_HEIGHT - PLAYER_SIZE / 2) {
                playerY += PLAYER_SPEED;
            }
            break;
        case GLUT_KEY_DOWN:
            if (playerY - PLAYER_SPEED > PLAYER_SIZE / 2) {
                playerY -= PLAYER_SPEED;
            }
            break;
        case GLUT_KEY_LEFT:
            if (playerX - PLAYER_SPEED > PLAYER_SIZE / 2) {
                playerX -= PLAYER_SPEED;
            }
            break;
        case GLUT_KEY_RIGHT:
            if (playerX + PLAYER_SPEED < WINDOW_WIDTH - PLAYER_SIZE / 2) {
                playerX += PLAYER_SPEED;
            }
            break;
        }
    }
    if (playerX < 0) {
        playerX = 0;
    }
    if (playerX > WINDOW_WIDTH - PLAYER_SIZE - 10) {
        playerHealth -= 0.5;
        if (playerHealth <= 0) {
            gameOver = true;
        }
        playerX = WINDOW_WIDTH - PLAYER_SIZE - 10;
    }
    if (playerY < 0) {
        playerY = 0;
    }
    if (playerY > WINDOW_HEIGHT - PLAYER_SIZE - 15) {
        playerHealth -= 0.5;
        if (playerHealth <= 0) {
            gameOver = true;
        }
        playerY = WINDOW_HEIGHT - PLAYER_SIZE - 15;
    }
    glutPostRedisplay();
}


void timer(int value) {
    checkPowerUpDuration();
    checkPowerUpDuration2();
    updateTargets(score);
    updateObstacles();
    updatePowerUps1();
    updatePowerUps2();
    glutPostRedisplay();

    glutTimerFunc(16, timer, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(1000, 600);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("OpenGL - 2D Template");

    init();
    glutDisplayFunc(Display);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}
