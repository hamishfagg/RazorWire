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
#define DEAD_ZONE 30


// Returns n pipes in a string
// Hacks as, bro
void getPipe(int n, char* s) {
    int i;
    for (i = 0; i < n; ++i) {
        s[i] = '|';
    }
    s[i] = '\0';
}

// Sets up the graphics environment
// TODO: Lighting, anti-aliasing and fog
void setupGraphics()
{
    //Attemp at getting anti-aliasing to work
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND); // Enable alpha channel
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    //Attempt at getting lighting to work
    /*glEnable(GL_LIGHTING);
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
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, light_emission);*/


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

// Main method
// Should the main game loop  / operations be in it's own class to save this for arg parsing and stuff?
int main(int argc, char* argv[])
{
	int TRACK_LENGTH = 50; //Number of chunks to create
    state = 0; //Startup

    std::locale loc;
    const std::collate<char>& coll = std::use_facet<std::collate<char> >(loc); // Copypasta used for hashing

    //Parse commandline options
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--debug") == 0)
        {
          std::cout << "DEBUG ENABLED: This could cause lower framerates than normal, and should NOT be used by default." << std::endl;
          debug = true;
        }
        if (strcmp(argv[i], "-s") == 0)
        {
            i++;
            seed = coll.hash(argv[i], argv[i]+strlen(argv[i]));
            if (debug) std::cout << "Seeding with " << seed << "..." << std::endl;
        }
        if ((strcmp(argv[i], "-j") == 0) || (strcmp(argv[i], "--joystick") == 0))
        {
          sf::Joystick::Update();
          for (int j = 0; j < sf::Joystick::Count; j++)
          {
            if (sf::Joystick::IsConnected(j))
            {
              joystick = true;
              if (i+1 < argc)
              {
                if (strcmp(argv[i+1], "xbox") == 0)
                {
                  xbox = true;
                  if (debug) std::cout << "xBox controller mode enabled." << std::endl;
                }
              }
              joy_id = j;
              if (debug) std::cout << "Found joystick " << j << std::endl;
              break;
            }
            else
            {
              if (debug) std::cout << j << " is not connected." << std::endl;
            }
          }
        }
    }

    if (seed == 0) {
        seed = time(NULL);
		if (debug) std::cout << "No seed given - using current time..." << std::endl;
	}
    srand (seed);

    // Create the main window
    sf::RenderWindow App(sf::VideoMode(1000, 1000, 32), "Razor Wire");

    //App.EnableVerticalSync(true);
    //App.SetFramerateLimit(60);

    sf::Clock Clock;

	if (debug) std::cout << "Setting up graphics... ";
    setupGraphics(); // Located above main()
	if (debug) std::cout << "done." << std::endl;

    // Initialise track
    Track track;
    if (debug)
	{
		double time = Clock.GetElapsedTime();
		track.GenChunk(TRACK_LENGTH);
		time = Clock.GetElapsedTime() - time;
		std::cout << "Level generated in " << time << " milliseconds." << std::endl;

		std::cout << "Setting up track preview... ";
	}
	else
	{
		track.GenChunk(TRACK_LENGTH);
	}

    Point trackAvg = track.GetAvg();
    float max = track.GetMaxDist(trackAvg);

    // Initialise player
    Player p(track.getStart());

    char pipetext[80]; // hacks

    state = 2; // Preview

  glMatrixMode(GL_MODELVIEW);

  if (debug) std::cout << "done." << std::endl;

	  // Start the game loop
    while (App.IsOpened())
    {
      float pos_z, pos_y, pos_x;
        if (state == 2)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen
            glColor3f(1, 1, 1); // White
            glLoadIdentity();

            gluLookAt(max + 100, 0, 0, 0, 0, 0, 0, 0, 1); 			// Look at the origin, from a large x distance away
            glRotatef((double)Clock.GetElapsedTime()/1000*30, 1, 0, 0);		// Rotate on all 3 axis at different speeds
            glRotatef((double)Clock.GetElapsedTime()/1000*40, 0, 1, 0);
            glRotatef((double)Clock.GetElapsedTime()/1000*50, 0, 0, 1);
            glTranslatef(-trackAvg.x, -trackAvg.y, -trackAvg.z);	// Translate the avg track pos

            track.Render((double)App.GetFrameTime()/1000, 1.0); 				// Render the track all pretty and such

            App.Display();

            // Process events
            sf::Event Event;
            while (App.PollEvent(Event))
            {
                // Close window : exit
                if ((Event.Type == sf::Event::Closed) |
                    (Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Keyboard::Escape))
                    App.Close();

                // Start track
				if (Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Keyboard::Return)
                    state = 3;

                if (Event.Type == sf::Event::Resized)
                    glViewport(0, 0, Event.Size.Width, Event.Size.Height);
            }

        }
        else if (state == 3) // Playing
        {

            // hack alert
            char s[80];
            getPipe(20 - p.trackDist(p.tp), pipetext);
            if (debug)	sprintf(s, "%f\n%i\n%s", 1/((double)App.GetFrameTime()/1000), (int)p.score, pipetext); // Add a frame rate counter if we're in debug mode
			else			sprintf(s, "%i\n%s", (int)p.score, pipetext);
            sf::Text Text(s);

            Text.SetColor(sf::Color(255,255,255));

            // Process events
            sf::Event Event;
            while (App.PollEvent(Event))
            {
                // Close window : exit
                if ((Event.Type == sf::Event::Closed) |
                    (Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Keyboard::Escape))
                    App.Close();

                if (Event.Type == sf::Event::Resized)
                    glViewport(0, 0, Event.Size.Width, Event.Size.Height);

				// Pause
				if (Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Keyboard::P)
				{
					state = 4;
					if (debug) std::cout << "The game has been paused." << std::endl;
				}
            }

            if (!joystick)
            {
              if (sf::Keyboard::IsKeyPressed(p.controls[A])) {
                p.rv.z = -1;
              }
              else if (sf::Keyboard::IsKeyPressed(p.controls[D])) {
                p.rv.z = 1;
              }
              else {
                p.rv.z = 0;
              }
              if (sf::Keyboard::IsKeyPressed(p.controls[Left])) {
                p.rv.y = 1;
              }
              else if (sf::Keyboard::IsKeyPressed(p.controls[Right])) {
                p.rv.y = -1;
              }
              else {
                p.rv.y = 0;
              }
              if (sf::Keyboard::IsKeyPressed(p.controls[Up])) {
                p.rv.x = -1;
              }
              else if (sf::Keyboard::IsKeyPressed(p.controls[Down])) {
                p.rv.x = 1;
              }
              else {
                p.rv.x = 0;
              }
            }
            // otherwise use the joystick
            else
            {
              if (xbox)
              {
                pos_x = sf::Joystick::GetAxisPosition(joy_id, sf::Joystick::V);
                pos_y = sf::Joystick::GetAxisPosition(joy_id, sf::Joystick::U);
                pos_z = sf::Joystick::GetAxisPosition(joy_id, sf::Joystick::X);
              }
              else
              {
                pos_x = sf::Joystick::GetAxisPosition(joy_id, sf::Joystick::Y);
                pos_y = sf::Joystick::GetAxisPosition(joy_id, sf::Joystick::X);
                pos_z = sf::Joystick::GetAxisPosition(joy_id, sf::Joystick::R);
              }
              if (abs(pos_z) > DEAD_ZONE)
              {
                p.rv.z = (pos_z / 100.f);
              }
              else
              {
                p.rv.z = 0;
              }
              if (abs(pos_y) > DEAD_ZONE)
              {
                p.rv.y = (pos_y / 100.f);
              }
              else
              {
                p.rv.y = 0;
              }
              if (abs(pos_x) > DEAD_ZONE)
              {
                p.rv.x = (pos_x / 100.f);
              }
              else
              {
                p.rv.x = 0;
              }
            }

			float speed = 30 - p.trackDist(p.tp)*2;
            if (speed < 2) {
                speed = 2;
            }

            //------------
            // OPENGL
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glLoadIdentity();

            // CAMERA

			// Set the camer position based on the ship's forward vector
			// Here incremental values could be added to produce a 'soft' camera movement
			// Currently the camera gets further away as speed increases
            float camx = p.r.x - p.forward.x*15 - p.forward.x*speed/5 + p.up.x*15; // No speed modifier on the 'up' term to make the camera be more behind the ship at high speed and less above it
            float camy = p.r.y - p.forward.y*15 - p.forward.y*speed/5 + p.up.y*15;
            float camz = p.r.z - p.forward.z*15 - p.forward.z*speed/5 + p.up.z*15;

			// Update the ships velocity based on the player's ability to not suck
            p.v.x = p.forward.x*speed;
            p.v.y = p.forward.y*speed;
            p.v.z = p.forward.z*speed;

            //DYNAMIC CAMERA
            gluLookAt(camx, camy, camz, p.r.x, p.r.y, p.r.z, p.up.x, p.up.y, p.up.z);

            //STATIC CAMERA - uncomment to have the camera still at the start of the track
            //glRotatef(-70, 1,0,0);
            //glTranslatef(0, 10, -5);

            // OBJECTS
            // The line from ship to current track segment
            glLineWidth(1.0);
            float col = 1 - p.trackDist(p.tp)/10;
            glColor3f(col, col, 1);

            glBegin(GL_LINES);
                glVertex3f(p.tp->x, p.tp->y, p.tp->z);
                glVertex3f(p.r.x, p.r.y, p.r.z);
            glEnd();

            track.Render((double)App.GetFrameTime()/1000, 0.2); // Render track and player
            p.Render((double)App.GetFrameTime()/1000);

            App.SaveGLStates();
            App.Draw(Text);	// Draw HUD text
            App.RestoreGLStates();

            //------------

            // Update the window
            App.Display();
        }
	else if (state == 4) // Game is paused
	{
		sf::Event Event;
		while (App.PollEvent(Event))
		{
			// Close window : exit
			if ((Event.Type == sf::Event::Closed) |
				(Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Keyboard::Escape))
				App.Close();

			// Unpause
			if (Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Keyboard::P)
			{
				state = 3;
				if (debug) std::cout << "The game has been resumed." << std::endl;
			}

			if (Event.Type == sf::Event::Resized)
				glViewport(0, 0, Event.Size.Width, Event.Size.Height);
		}

		App.Display();
	}
    }

    return EXIT_SUCCESS;
}
