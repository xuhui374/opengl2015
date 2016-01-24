
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define WIDTH 700
#define HEIGHT 700
#define TimePast 1
#define PI 3.1415926535
#define Float2(name, value0, value1) ((name)[0])=(value0), ((name)[1])=(value1)
#define Float3(name, value0, value1, value2) ((name)[0])=(value0), \
((name)[1])=(value1), ((name)[2])=(value2)
#define Float4(name, value0, value1, value2, value3) ((name)[0])=(value0), \
((name)[1])=(value1), ((name)[2])=(value2), ((name)[3])=(value3)
#define RFloat2(name) ((name)[0]), ((name)[1])
#define RFloat3(name) ((name)[0]), ((name)[1]), ((name)[2])
#define RFloat4(name) ((name)[0]), ((name)[1]), ((name)[2]), ((name)[3])
typedef GLfloat (Float2)[2];
typedef GLfloat (Float3)[3];
typedef GLfloat Float;
typedef GLfloat (Float4)[4];


class Planet {
    public:
        Float4 Color;
        Float Radius;
        Float SelfSpeed;
        Float Speed;
        Float Distance;
        Planet * ParentPlanet;
    
        virtual void Draw() { DrawPlanet(); }
        virtual void Update(long TimeSpan);
    
        Planet(Float Radius, Float Distance, Float Speed, Float SelfSpeed, Planet * Parent);
    
        // 对普通的行星进行移动和转动
        void DrawPlanet();
    
    protected:
        Float AlphaSelf, Alpha;
    };

class MatPlanet : public Planet {
    public:
        virtual void Draw() { DrawMat(); DrawPlanet(); }
    
        MatPlanet(Float Radius, Float Distance, Float Speed, Float SelfSpeed,
                               Planet * Parent, Float3 color);
        // 对材质进行设置
        void DrawMat();
    };

class LightPlanet : public MatPlanet {
    public:
        virtual void Draw() { DrawLight(); DrawMat(); DrawPlanet(); }
    
        LightPlanet(Float Radius, Float Distance, Float Speed, Float SelfSpeed,
                                   Planet * Parent, Float3 color);
    
        // 对光线进行设置
        void DrawLight();
    };

Planet::Planet(Float Radius, Float Distance, Float Speed, Float SelfSpeed, Planet * Parent) {
        Float4(Color, 0.8f, 0.8f, 0.8f, 1.0f);
        this->Radius = Radius;
        this->SelfSpeed = SelfSpeed;
        if (Speed > 0)
                this->Speed = 360.0f / Speed;
        AlphaSelf = Alpha= 0;
        this->Distance = Distance;
        ParentPlanet = Parent;
    }

// 对普通的行星进行移动和转动
void Planet::DrawPlanet() {
    
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
    
        int n = 1440;
    
        glPushMatrix();
        {
                // 公转
                if (ParentPlanet != 0 && ParentPlanet->Distance > 0) {
                        glRotatef(ParentPlanet->Alpha, 0, 0, 1);
                        glTranslatef(ParentPlanet->Distance, 0.0, 0.0);
            
                        glBegin(GL_LINES);
                        for(int i=0; i<n; ++i)
                                glVertex2f(Distance * cos(2 * PI * i / n),
                                                                       Distance * sin(2 * PI * i / n));
                        glEnd();
            
                    } else {
                            glBegin(GL_LINES);
                            for(int i=0; i<n; ++i)
                                    glVertex2f(Distance * cos(2 * PI * i / n),
                                                                           Distance * sin(2 * PI * i / n));
                            glEnd();
                        }
                glRotatef(Alpha, 0, 0, 1);
                glTranslatef(Distance, 0.0, 0.0);
        
                // 自转
                glRotatef(AlphaSelf, 0, 0, 1);
        
                // 绘图
                glColor3f(RFloat3(Color));
                glutSolidSphere(Radius, 40, 32);
            }
        glPopMatrix();
    }


void Planet::Update(long TimeSpan) {
        // TimeSpan 是天
        Alpha += TimeSpan * Speed;
        AlphaSelf += SelfSpeed;
    }

MatPlanet::MatPlanet(Float Radius, Float Distance, Float Speed, Float SelfSpeed,
                                       Planet * Parent, Float3 color) : Planet(Radius, Distance, Speed, SelfSpeed, Parent) {
        Float4(Color, color[0], color[1], color[2], 1.0f);
    }




// 对材质进行设置
void MatPlanet::DrawMat() {
        GLfloat mat_ambient[]  = {0.0f, 0.0f, 0.5f, 1.0f};
        GLfloat mat_diffuse[]  = {0.0f, 0.0f, 0.5f, 1.0f};
        GLfloat mat_specular[] = {0.0f, 0.0f, 1.0f, 1.0f};
    
        GLfloat mat_emission[] = {RFloat4(Color)};
        //GLfloat mat_emission[] = {.0f, .0f, .1f, 1.0f};
        GLfloat mat_shininess  = 90.0f;
    
        glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_FRONT, GL_EMISSION,  mat_emission);
        glMaterialf (GL_FRONT, GL_SHININESS, mat_shininess);
    }

LightPlanet::LightPlanet(Float Radius, Float Distance, Float Speed, Float SelfSpeed,
                                               Planet * Parent, Float3 color)
: MatPlanet(Radius, Distance, Speed, SelfSpeed, Parent, color) {}

// 对光线进行设置
void LightPlanet::DrawLight() {
        GLfloat light_position[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat light_ambient[]  = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat light_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    }


#define KK .000001
#define sk (.07 * KK)
#define k (.5 * KK)
#define vk (1.5 * KK)
#define fk (.5 * KK)
#define hfk (.4 * KK)
#define ffk (.3 * KK)
#define dk (1.07 * KK)
#define edk (1.12 * KK)
#define lsk (.3 * KK)
#define mk (15000 * KK)
#define mrk (1.6 * KK)
#define tk .3
#define ttk .2
#define tttk .1

// 自转速度（都定义为定值）
#define SelfRotate 3

#define ARRAY_SIZE 10
enum STARS {Sun, Mercury, Venus, Earth, Moon, Mars, Jupiter, Saturn, Uranus, Neptune};
Planet * Planets[ARRAY_SIZE];


void init() {
        Float3 Color;
        // 定义星球，这些星球的数据是经过不同比例变化过的
        // sun
        Float3(Color, 1, 0, 0);
        Planets[Sun] = new LightPlanet(sk * 696300000, 0, 0, SelfRotate, 0, Color);
        // Mercury
        Float3(Color, .2, .2, .5);
        Planets[Mercury] = new MatPlanet(
                                                                           vk * 4880000, dk * 58000000, 87, SelfRotate, Planets[Sun], Color);
        // Venus
        Float3(Color, 1, .7, 0);
        Planets[Venus] = new MatPlanet(
                                                                       vk * 12103600, dk * 108000000, 225, SelfRotate, Planets[Sun], Color);
        // Earth
        Float3(Color, 0, 1, 0);
        Planets[Earth] = new MatPlanet(
                                                                       vk * 12756300, edk * 150000000, 365, SelfRotate, Planets[Sun], Color);
        // Moon
        Float3(Color, 1, 1, 0);
        Planets[Moon] = new MatPlanet(
                                                                     mrk * 3844010.0f , mk * 1734.0f, 30, SelfRotate, Planets[Earth], Color);
        // Mars
        Float3(Color, 1, .5, .5);
        Planets[Mars] = new MatPlanet(
                                                                     vk * 6794000, KK * 228000000, 687, SelfRotate, Planets[Sun], Color);
        // Jupiter
        Float3(Color, 1, 1, .5);
        Planets[Jupiter] = new MatPlanet(
                                                                           lsk * 142984000,  fk * 778000000, tk * 4328, SelfRotate, Planets[Sun], Color);
        // Saturn
        Float3(Color, .5, 1, .5);
        Planets[Saturn] = new MatPlanet(
                                                                         lsk * 120536000, fk * 1427000000, ttk * 10752, SelfRotate, Planets[Sun], Color);
        // Uranus
        Float3(Color, .4, .4, .4);
        Planets[Uranus] = new MatPlanet(k * 51118000,
                                                                         hfk * 2870000000, tttk * 30664, SelfRotate, Planets[Sun], Color);
        // Neptune
        Float3(Color, .5, .5, 1);
        Planets[Neptune] = new MatPlanet(k * 49532000,
                                                                           ffk * 4497000000, tttk * 60148, SelfRotate, Planets[Sun], Color);
    }
// 初始视点（ 视点在(+z, -y)处 ）
#define REST (700000000 * KK)
#define REST_Z (REST)
#define REST_Y (-REST)

// lookAt参数
GLdouble eyeX = 0, eyeY = REST_Y, eyeZ= REST_Z;
GLdouble centerX= 0, centerY= 0, centerZ= 0;
GLdouble upX= 0, upY= 0, upZ= 1;

void OnDraw(void) {
        glClear(GL_COLOR_BUFFER_BIT  |  GL_DEPTH_BUFFER_BIT);
        glClearColor(.7, .7, .7, .1);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(75.0f, 1.0f, 1.0f, 40000000);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(eyeX, eyeY,eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
    
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
    
        // 实际绘制
        for (int i=0; i<ARRAY_SIZE; i++)
                Planets[i]->Draw();
    
        glutSwapBuffers();
    }

void OnUpdate(void) {
        // 实际更新
        for (int i=0; i<ARRAY_SIZE; i++)
                Planets[i]->Update(TimePast);
        OnDraw();
    }

// 每次按键移动的距离
#define OFFSET (20000000 * KK)

// 按键操作变化视点
// w(+y方向)   a(-x方向)   d(+x方向)   x(-y方向)   s(+z 方向)   S(-z 方向)   r(reset)
void keyboard (unsigned char key, int x, int y) {
        switch (key)    {
                    case 'w': eyeY += OFFSET; break;
                    case 's': eyeZ += OFFSET; break;
                    case 'S': eyeZ -= OFFSET; break;
                    case 'a': eyeX -= OFFSET; break;
                    case 'd': eyeX += OFFSET; break;
                    case 'x': eyeY -= OFFSET; break;
                    case 'r':
                        eyeX = 0; eyeY = REST_Y; eyeZ= REST_Z;
                        centerX= 0; centerY= 0; centerZ= 0;
                        upX= 0; upY= 0; upZ= 1;
                        break;
                    case 27: exit(0); break;
                    default: break;
            }
    }

int main(int argc, char*  argv[]) {
        init();
    
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGBA |  GLUT_DOUBLE);
        glutInitWindowPosition(150, 50);
        glutInitWindowSize(WIDTH, HEIGHT);
        glutCreateWindow("solar2");
        glutDisplayFunc(&OnDraw);
        glutIdleFunc(&OnUpdate);
        glutKeyboardFunc(keyboard);
        glutMainLoop();
    
        return 0;
    
}
