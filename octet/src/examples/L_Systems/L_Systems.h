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
		dynarray<string> tree_Files;
		dynarray<vec2> point_Node;
		dynarray<vec2> point_State;
		dynarray<vec2> direction_State;
		int iteration_Count;
		int tree_Example;

	public:
		// this is called when we construct the class before everything is initialised.
		L_Systems(int argc, char **argv) : app(argc, argv) {

		}


		// this is called once OpenGL is initialized
		void app_init() {
			start_Pos = vec2(0, -1);
			current_Pos = start_Pos;
			current_Angle = 0.0f;
			app_scene = new visual_scene();
			app_scene->create_default_camera_and_lights();
			green = new material(vec4(0, 1, 0, 1));
			brown = new material(vec4(0.87f, 0.72f, 0.52f, 1));
			tree_Files.push_back("../../../assets/Tree1.csv");
			tree_Files.push_back("../../../assets/Tree2.csv");
			load_Data(tree_Files[0]);
			printf("%s", iterate_Rules(axiom, rules));
			parse_Data(iterate_Rules(axiom, rules));
		}

		void draw_line()
		{
			glClearColor(0, 0, 0, 1); //Clears background colour to black
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBegin(GL_LINES);
			glColor3f(0.87f, 0.72f, 0.52f);
			//glColor3f(1, 0, 0);
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

		void load_Data(string file_Data) //loads in the data
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

				if (col == 0)
				{
					axiom = (buffer);
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
				col++;

			}
		
	

		for (int j = 0; j < rules.size(); j++) {
			printf("%s\n", rules[j]);
		}
	}

	void parse_Data(string data) // Reads rules and parses
	{
		vec2 end_Pos;
		current_Angle = 0.0f;

		current_Dir = vec2(0.0f, 1.0f);

		start_Pos = vec2(0.0f, -1.0f);
		current_Pos = start_Pos;
		//point_Node.reset();

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
			center_points(point_Node);
			
		}
	}

	string iterate_Rules(string axiom,dynarray<string> &rules) //goes through rules and iterates
	{
		dynarray<char> data;
		for (int i = 0; i < axiom.size(); i++)
		{
			int j = 0;
			for (j = 0; j < rules.size(); j++)
			{
				printf("Iterated!");
				if (axiom[i] == rules[j][0])
				{
					for (int k = 0; k < rules[j].size(); k++)
					{
						data.push_back(rules[j][2]);//return the 3rd line
					} 
					break;
				}
			}
			if (j == rules.size())
			{
				printf("none");
			}
		}
		data.push_back(0x00);
		return string(data.data());
	}

	void hotkey_Controls()
	{
	
		if (is_key_down(key_right))
		{
			iteration_Count++;
		}
		if (is_key_down(key_up))
		{
			tree_Example++;

		}
		if (is_key_down(key_left))
		{
			tree_Example--;
		}
		if (is_key_down(key_down))
		{
			iteration_Count--;
		}
	}


	void get_Centerpointandscale(dynarray<vec2>& points, float& scale, vec2& midpoint)
	{
		scale = 0.0f;
		midpoint = vec2(0.0f);

		vec2 top_right = vec2(0.0f);
		vec2 bottom_left = vec2(0.0f);

		int size = points.size();
		for (int i = 0; i < size; i++)
		{
			top_right.x() = (points[i].x() < top_right.x()) ? top_right.x() : points[i].x();
			top_right.y() = (points[i].y() < top_right.y()) ? top_right.y() : points[i].y();

			bottom_left.x() = (points[i].x() > bottom_left.x()) ? bottom_left.x() : points[i].x();
			bottom_left.y() = (points[i].y() > bottom_left.y()) ? bottom_left.y() : points[i].y();
		}

		scale = (top_right - bottom_left).length();

		printf("top_right %f, %f\n", top_right.x(), top_right.y());
		printf("bottom_left %f, %f\n", bottom_left.x(), bottom_left.y());

		printf("Scale of points: %f\n", scale);

		midpoint = (top_right + bottom_left) * 0.5f;
		//return scale;
	}
	void center_points(dynarray<vec2>& points)
	{
		float scale;
		vec2 center;
		get_Centerpointandscale(points, scale, center);

		if (scale != 0.0f)
			scale = 2.0f / scale;
		else return;

		for (int i = 0; i < points.size(); i++)
		{
			points[i] -= center;
			points[i] *= scale;
		}
	}

	void draw_points()
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBegin(GL_LINES);
		glColor3f(250.0f, 0.0f, 0.0f);
		int size = point_Node.size();
		for (int i = 0; i < size; i++)
		{
			glVertex3f(point_Node[i].x(), point_Node[i].y(), 0);
			++i;
			glVertex3f(point_Node[i].x(), point_Node[i].y(), 0);
		}

		glEnd();
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
