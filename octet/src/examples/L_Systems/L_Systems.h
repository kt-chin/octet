#include <fstream>
#define RADIAN 0.01745329251f

namespace octet {

  class L_Systems : public app {
    // scene for drawing box

	  ref<material> green;
	  ref<material> brown;
	  ref<visual_scene> app_scene;
	  float branch_Length = 0.1f;
	  float angle;
	  vec2 start_Pos;
	  vec2 current_Pos;
	  vec2 current_Dir;
	  vec2 current_Angle;
	  string axiom;
	  dynarray<string> rules;
	  dynarray<vec2> point_Node;
	  dynarray<vec2> point_State;
	  dynarray<vec2> direction_State;
	  int iteration_Count;
	  
  public:
    // this is called when we construct the class before everything is initialised.
    L_Systems(int argc, char **argv) : app(argc, argv) {

    }


    // this is called once OpenGL is initialized
    void app_init() {
	start_Pos = vec2(0, -1);
	current_Pos = start_Pos;
	current_Angle = 0.0f;
    app_scene =  new visual_scene();
    app_scene->create_default_camera_and_lights();
	green = new material(vec4(0, 1, 0, 1));
	brown = new material(vec4(0.87f, 0.72f, 0.52f, 1));

	string rules = ("../../../assets/Tree1.csv");
	string rules = ("../../../assets/Tree2.csv");
	  //float pos_X = ((rectScreen))

	  //draw_Log(vec3(4,5,0));

	//  draw_Log(vec3(0,0,0));

    }

	void draw_Log(vec3(pos))
	{
		mesh_box *box = new mesh_box(vec3(0.2f, 4, 0));
		scene_node *node = new scene_node();
		node->translate(pos);
		app_scene->add_child(node);
		app_scene->add_mesh_instance(new mesh_instance(node, box, brown));
	}



	void draw_line()
	{
		glClearColor(0, 0, 0, 1); //Clears background colour to black
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBegin(GL_LINES);
		glColor3f(0.87f, 0.72f, 0.52f);
		for (int i = 0; i < point_Node.size(); ++i)
		{
			glVertex2f(point_Node[i].x(), point_Node[i].y());
			glVertex2f(point_Node[i].x(), point_Node[i].y());
		}

		glEnd();
	}

	void rotate_Function(vec2& vec, float degrees)
	{
		float cs = cos(degrees * RADIAN);
		float sn = sin(degrees * RADIAN);
		float pX = vec.x() * cs - vec.y() * sn;
		float pY = vec.x() * sn + vec.y() * cs;
		vec.x() = pX;
		vec.y() = pY;
	}

	void load_Data(string file_Data)
	{
		std::ifstream file(file_Data);

		char buffer[256];

		rules.reset();
		point_State.reset();
		direction_State.reset();

		int col = 0;
		while (!file.eof())
		{
			file.getline(buffer, sizeof(buffer));

			if (buffer[0] != ',')
			{
				if (col == 0)
				{
					axiom = buffer;
				}
				else if (col == 1)
				{
					iteration_Count = atoi(buffer);
				}
				else if (col == 2)
				{
					angle = atof(buffer);
				}
				else if (col >= 3)
				{
					rules.push_back(buffer);
				}
				++col;
			}
		}


		for (int j = 0; j < rules.size(); j++) {
			printf("%s\n", rules[j]);
		}
	}

	void parse_Data(string data)
	{
		vec2 end_Pos;
		current_Angle = 0.0f;

		current_Dir = vec2(0.0f, 1.0f);

		start_Pos = vec2(0.0f, -1.0f);
		current_Pos = start_Pos;
		point_Node.reset();

		for (int j = 0; j < data.size(); j++) {
			switch (data[j])
			{
			case 'F':
				//Create line segment
				current_Dir = current_Dir.normalize();
				end_Pos = current_Pos + current_Dir * branch_Length;
				point_Node.push_back(current_Pos);
				point_Node.push_back(end_Pos);
				current_Pos = end_Pos;
				break;

			case '[':
				// Push (stack) the position and angle
				direction_State.push_back(current_Dir);
				point_State.push_back(current_Pos);
				break;

			case ']':
				// Pop (Pull) the position and angle
				current_Dir = direction_State.back();
				current_Pos = point_State.back();
				point_State.pop_back();
				direction_State.pop_back();
				break;

			case '+':
				// rotate right
				rotate_Function(current_Dir, -angle);
				break;

			case '-':
				// rotate left 
				rotate_Function(current_Dir, angle);
				break;
			}
		}
	}



    // this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);

      // update matrices. assume 30 fps.
      app_scene->update(1.0f/30);

      // draw the scene
      app_scene->render((float)vx / vy);

    }
  };
}
