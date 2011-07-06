#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <cstdio>   // Hack for sprintf
#include <iostream>
#include <cstring>
#include <locale>

#include "main.h"
#include "Object.h"
#include "Player.h"
#include "Track.h"

#define PI 3.14159265

void getPipe(int n, char* s) {
    int i;
    for (i = 0; i < n; ++i) {
        if (i < n) s[i] = '|';
    }
    s[i] = NULL;
}

void setupGraphics()
{
    //Attemp at getting anti-aliasing to work
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND); // Enable alpha channel
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    //Attempt at getting lighting to work
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat al[] =              {0.2, 0.2, 0.2, 1.0};
    GLfloat light_ambient[] =   {1.0, 1.0, 1.0, 1.0};
    GLfloat light_position[] =  {1.0, 1.0, 0.0};
    GLfloat light_diffuse[] =   {1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular[] =  {1.0, 1.0, 1.0, 1.0};
    GLfloat light_emission[] =  {0.0, 0.0, 0.0, 1.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, al);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, light_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, light_emission);


    //Attempt at getting fog to work
    GLfloat fogcolor[4] = {1, 1, 1, 1};
    //glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP);
    glFogfv(GL_FOG_COLOR, fogcolor);
    glFogf (GL_FOG_DENSITY, 0.35);
    //glHint (GL_FOG_HINT, GL_DONT_CARE);
    glFogf (GL_FOG_START, 1.0);
    glFogf (GL_FOG_END, 6.0);


    // Set color and depth clear value
    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 0.f);

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75.f, 1.f, 1.f, 1000.f); //This needs to be set to a shorter value after track preview
}


int main(int argc, char* argv[])
{
    state = 0; //Startup

    std::locale loc;
    const std::collate<char>& coll = std::use_facet<std::collate<char> >(loc); // Used for hashing

    //Parse commandline options
    for (int i = 0; i < argc; i++)
    {
        std::cout << argv[i] << std::endl;
        /*if (argv[i] == "--debug")
        {
            debug = true;
        }*/
        if (strcmp(argv[i], "-s") == 0)
        {
            i++;
            seed = coll.hash(argv[i], argv[i]+strlen(argv[i]));
        }
    }

    int TRACK_LENGTH = 50; //Number of chunks to create

    if (seed == 0)
        seed = time(NULL);
    srand (seed);

    // Create the main window
    sf::RenderWindow App(sf::VideoMode(1000, 1000, 24), "SFML window");
    App.UseVerticalSync(true);
    App.PreserveOpenGLStates(true);

    sf::Clock Clock;

    setupGraphics(); // Located above main()

    // Initialise track
    Track track;
    double time = Clock.GetElapsedTime();
    track.GenChunk(TRACK_LENGTH);
    time = Clock.GetElapsedTime() - time;
    std::cout << "Level generated in " << time << " seconds." << std::endl;

    Point trackAvg = track.GetAvg();
    std::cout << "Track middle point: " << trackAvg << std::endl;

    float max = track.GetMaxDist(trackAvg);
    std::cout << "Track max distance from center: " << max << std::endl;

    // Initialise player
    Player p(track.getStart());

    const sf::Input &in = App.GetInput();

    //hacks
    char pipetext[80];
    state = 2;

	// Start the game loop
    while (App.IsOpened())
    {
        if (state == 2)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glMatrixMode(GL_MODELVIEW);
            glColor3f(1, 1, 1);
            glLoadIdentity();

            //glPushMatrix();
            //glPopMatrix();
            //glRotatef(Clock.GetElapsedTime()*30, 1, 0, 0);
            //glRotatef(Clock.GetElapsedTime()*50, 0, 1, 0);
            //glLoadIdentity();
            gluLookAt(max + 100, 0, 0, 0, 0, 0, 0, 0, 1);
            glRotatef(Clock.GetElapsedTime()*30, 1, 0, 0);
            glRotatef(Clock.GetElapsedTime()*40, 0, 1, 0);
            glRotatef(Clock.GetElapsedTime()*50, 0, 0, 1);
            glTranslatef(-trackAvg.x, -trackAvg.y, -trackAvg.z);

            track.Render(App.GetFrameTime(), 1.0);





            App.Display();
            // Process events
            sf::Event Event;
            while (App.GetEvent(Event))
            {
                // Close window : exit
                if ((Event.Type == sf::Event::Closed) |
                    (Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Escape))
                    App.Close();

                if (Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Return)
                    state = 3;

                if (Event.Type == sf::Event::Resized)
                    glViewport(0, 0, Event.Size.Width, Event.Size.Height);
            }

        }
        else if (state == 3)
        {

            // hack alert
            char s[80];
            getPipe(20 - p.trackDist(p.tp), pipetext);
            sprintf(s, "%f\n%i\n%s", 1/App.GetFrameTime(), (int)p.score, pipetext);
            sf::String Text(s);

            Text.SetColor(sf::Color(255,255,255));

            // Process events
            sf::Event Event;
            while (App.GetEvent(Event))
            {
                // Close window : exit
                if ((Event.Type == sf::Event::Closed) |
                    (Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Escape))
                    App.Close();

                if (Event.Type == sf::Event::Resized)
                    glViewport(0, 0, Event.Size.Width, Event.Size.Height);
            }

            if (in.IsKeyDown(p.controls[A])) {
                p.rv.z = -1;
            }
            else if (in.IsKeyDown(p.controls[D])) {
                p.rv.z = 1;
            }
            else {
                p.rv.z = 0;
            }
            if (in.IsKeyDown(p.controls[Left])) {
                p.rv.y = 1;
            }
            else if (in.IsKeyDown(p.controls[Right])) {
                p.rv.y = -1;
            }
            else {
                p.rv.y = 0;
            }
            if (in.IsKeyDown(p.controls[Up])) {
                p.rv.x = -1;
            }
            else if (in.IsKeyDown(p.controls[Down])) {
                p.rv.x = 1;
            }
            else {
                p.rv.x = 0;
            }


            //Set the camera to be just behind the player


            /*cam.r.x = -pPos.x*10/len;
            cam.r.y = -pPos.y*10/len;
            cam.r.z = -pPos.z*10/len;
            cam.rr.x = p.rr.x;
            cam.rr.y = p.rr.y;
            cam.rr.z = p.rr.z;*/

            //p.v = p.toWorld(Point(0, -2, 0));
            // Clear screen
            //App.Clear();
            //------------
            // OPENGL
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            // CAMERA

            //glRotatef(cam.rr.z, 0,0,1);
            //glRotatef(cam.rr.y, 0,1,0);


            //cam.r.x = p.r.x;
            //cam.r.y = p.r.y - 10;
            //Point pPos = p.GetVector();

            //p.v.x = pPos.x * 5;
            //p.v.y = pPos.y * 5;
            //p.v.z = pPos.z * 5;
            //Point pUp = p.GetUpVector();
            //std::cout << pUp << std::endl;
            //float len = sqrt(pPos.x*pPos.x + pPos.y*pPos.y + pPos.z*pPos.z);

            float camx = p.r.x - p.forward.x*15 + p.up.x*15;
            float camy = p.r.y - p.forward.y*15 + p.up.y*15;
            float camz = p.r.z - p.forward.z*15 + p.up.z*15;
            // EcksJay shitting about
            //p.v = p.toWorld(Point(0,1,0));
            //p.v = p.GetHeading();
            //std::cout << p.v << std::endl;
            //std::cout << p.r.x << " " <<  p.r.y << " " << p.r.z << std::endl;
            //glRotatef(p.rr.x * 180/PI, 1, 0, 0);
            //glRotatef(p.rr.y * 180/PI, 0, 1, 0);
            //glRotatef(p.rr.z * 180/PI, 0, 0, 1);
            float speed = 30 - p.trackDist(p.tp)*2;
            if (speed < 2) {
                speed = 2;
            }
            p.v.x = p.forward.x*speed;
            p.v.y = p.forward.y*speed;
            p.v.z = p.forward.z*speed;
            //std::cout << p.forward << " " << p.v << std::endl;
            //std::cout << "Distance from track: " << p.trackDist(p.tp) << std::endl;

            //DYNAMIC CAMERA
            gluLookAt(camx, camy, camz, p.r.x, p.r.y, p.r.z, p.up.x, p.up.y, p.up.z);

            //STATIC CAMERA
            //glRotatef(-70, 1,0,0);
            //glTranslatef(0, 10, -5);

            // OBJECTS
            // The line
            glLineWidth(1.0);
            float col = 1 - p.trackDist(p.tp)/10;
            glColor3f(col, col, 1);

            glBegin(GL_LINES);
                glVertex3f(p.tp->x, p.tp->y, p.tp->z);
                glVertex3f(p.r.x, p.r.y, p.r.z);
            glEnd();

            track.Render(App.GetFrameTime(), 0.2);
            p.Render(App.GetFrameTime());

            //p.r.y = p.r.y + 0.02f;

            App.Draw(Text);

            //------------

            // Update the window
            App.Display();
        }
    }

    return EXIT_SUCCESS;
}
