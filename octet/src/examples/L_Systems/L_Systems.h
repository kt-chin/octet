#include <fstream>
#define RADIAN 0.01745329251f

namespace octet {

	class L_Systems : public app {
		// scene for drawing box

		ref<material> green;
		ref<material> brown;
		ref<visual_scene> app_scene;
		float branch_Length = 0.01f;
		float angle;
		vec2 start_Pos;
		vec2 current_Pos;
		vec2 current_Dir;
		vec2 current_Angle;
		string axiom;
		string initial_Axiom;
		dynarray<string> rules;
		dynarray<string> tree_Files;
		dynarray<vec2> point_Node;
		dynarray<vec2> point_State;
		dynarray<vec2> direction_State;
		int iteration_Count;
		int tree_Example = 1;
		int current_Iterate = 0;
		//float camera_Speed = 3.0f;

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
			tree_Files.push_back("../../../assets/Tree3.csv");
			tree_Files.push_back("../../../assets/Tree4.csv");
			tree_Files.push_back("../../../assets/Tree5.csv");
			tree_Files.push_back("../../../assets/Tree6.csv");
			load_Data(tree_Files[0]);
			printf("%s", iterate_Rules(axiom, rules));
		}


		void generate_points(string axiom, dynarray<string>& rules, int iteration_Count)
		{
			point_State.reset();
			direction_State.reset();
			point_Node.reset();
			//rules.reset();
			string result = axiom;
			for (int j = 0; j < iteration_Count; j++) {
				result = iterate_Rules(axiom, rules);
			}
			parse_Data(result);
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
					initial_Axiom = (buffer);
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

	string iterate_Rules(string axiom,dynarray<string> &rules) //goes through rules and iterates
	{
		dynarray<char> data;
		for (int i = 0; i < axiom.size(); i++)
		{
			int j = 0;
			for (j = 0; j < rules.size(); j++)
			{
				//printf("Iterated!");
				if (axiom[i] == rules[j][0])
				{
					for (int k = 2; k < rules[j].size(); k++)
					{
						data.push_back(rules[j][k]);//return the 3rd character (ignoring F=)
					} 
					break;
				}
			}
			if (j == rules.size())
			{
				//printf("none");
				data.push_back(axiom[i]);
			}
		}
		data.push_back(0x00);
		return string(data.data());
	}

	void reset_Function()
	{
		point_State.reset();
		direction_State.reset();
		point_Node.reset();
		rules.reset();
		current_Iterate = 0;
	}

	void hotkey_Controls()
	{
		
			if (is_key_down(key_up))
			{
				printf("GO UP");

			}
			if (is_key_down(key_left))
			{
				printf("GO LEFT");

			}
			if (is_key_down(key_down))
			{
				printf("GO DOWN");

			}
			if (is_key_down(key_right))
			{
				printf("GO RIGHT");

			}
		
		else
		/*	cameraToWorld.translate(cameraToWorld.y() * camera_Speed);
		}
		if (is_key_down(key_shift) && is_key_down(a))
		{
			cameraToWorld.translate(cameraToWorld.x() * -camera_Speed);
		}
		if (is_key_down(key_shift) && is_key_down(s))
		{
			cameraToWorld.translate(cameraToWorld.y() * -camera_Speed);
		}
		if (is_key_down(key_shift) && is_key_down(d))
		{
			cameraToWorld.translate(cameraToWorld.x() * camera_Speed);
		}*/
		/*if (is_key_down(w))
		{
			cameraToWorld.translate(cameraToWorld.y() * camera_Speed);
		}
		if (is_key_down(a))
		{
			cameraToWorld.translate(cameraToWorld.x() * -camera_Speed);
		}
		if (is_key_down(s))
		{
			cameraToWorld.translate(cameraToWorld.y() * -camera_Speed);
		}
		if (is_key_down(d))
		{
			cameraToWorld.translate(cameraToWorld.x() * camera_Speed);
		}*/

		if (is_key_going_down(d))
		{
			printf("%i\n", tree_Example);
			if (tree_Example < tree_Files.size()-1)
			{
				reset_Function();
				printf("Should change rules forward");
				tree_Example++;
				load_Data(tree_Files[tree_Example]);
			}
		}
		if (is_key_going_down(w))
		{
			if (current_Iterate <= iteration_Count)
			{
				current_Iterate++;
				generate_points(axiom, rules, current_Iterate);
				printf("Should Iterate upwards\n");
				axiom = iterate_Rules(axiom, rules);
				printf("%s\n", axiom);
			}
		}
		if (is_key_going_down(a))
		{
			printf("%i\n",tree_Example);
			if (tree_Example > 0)
			{
				reset_Function();
				printf("Should change rules backwards");
				tree_Example--;
				load_Data(tree_Files[tree_Example]);
			}
		}
		if (is_key_going_down(s))
		{
			reset_Function();
			load_Data(tree_Files[tree_Example]);
			//axiom = initial_Axiom;
			//printf("%s\n", axiom);
		}
	}

	void get_Centerpointandscale(dynarray<vec2>& point_Node, float& scale, vec2& midpoint)
	{
		scale = 0.0f;
		midpoint = vec2(0.0f);

		vec2 top_right = vec2(0.0f);
		vec2 bottom_left = vec2(0.0f);

		int size = point_Node.size();
		for (int i = 0; i < size; i++)
		{
			top_right.x() = (point_Node[i].x() < top_right.x()) ? top_right.x() : point_Node[i].x();
			top_right.y() = (point_Node[i].y() < top_right.y()) ? top_right.y() : point_Node[i].y();

			bottom_left.x() = (point_Node[i].x() > bottom_left.x()) ? bottom_left.x() : point_Node[i].x();
			bottom_left.y() = (point_Node[i].y() > bottom_left.y()) ? bottom_left.y() : point_Node[i].y();
		}

		scale = (top_right - bottom_left).length();

		/*printf("top_right %f, %f\n", top_right.x(), top_right.y());
		printf("bottom_left %f, %f\n", bottom_left.x(), bottom_left.y());

		printf("Scale of points: %f\n", scale);*/

		midpoint = (top_right + bottom_left) * 0.5f;
		//return scale;
	}

	void draw_line()
	{
		glClearColor(0, 0, 0, 1); //Clears background colour to black
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBegin(GL_LINES);
		glColor3f(0.87f, 0.72f, 0.52f);
		//glColor3f(1, 0, 0);
		for (int i = 0; i < point_Node.size(); i++)
		{
			glVertex2f(point_Node[i].x(), point_Node[i].y());
			++i;
			glVertex2f(point_Node[i].x(), point_Node[i].y());
		}

		glEnd();
	}
	
    // this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
		glViewport(x, y, w, h);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw_line();

	  hotkey_Controls();
      // update matrices. assume 30 fps.

    }
  };
}
