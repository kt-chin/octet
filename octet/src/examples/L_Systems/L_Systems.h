#include <fstream>
#define RADIAN 0.01745329251f

namespace octet {

	class L_Systems : public app {

		ref<material> green;
		ref<material> brown;
		ref<visual_scene> app_scene;
		float branch_Length = 0.1f;
		float branch_Width = 0.5f;
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
		float camera_Speed = 3.0f;
		mat4t cameraToWorld;
		mat4t transform;

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
			tree_Files.push_back("../../../assets/Tree7.csv");
			tree_Files.push_back("../../../assets/Tree8.csv");
			load_Data(tree_Files[0]);
			printf("%s", iterate_Rules(axiom, rules));
			app_scene->get_camera_instance(0)->get_node()->translate(vec3(0.0f, 0.0f, 1.0f));
		}

		void generate_points(string axiom, dynarray<string>& rules, int iteration_Count)
		{
			point_State.reset();
			direction_State.reset();
			point_Node.reset();
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
		mat4t worldToCamera;
		cameraToWorld.invertQuick(worldToCamera);
		if (is_key_down(key_shift))
		{
			if (is_key_down(w))
			{
				printf("GO UP");
				cameraToWorld.translate(vec3(0, 1, 0)*camera_Speed);
			}
			if (is_key_down(a))
			{
				printf("GO LEFT");
				cameraToWorld.translate(vec3(1, 0, 0)*-camera_Speed);
			}
			if (is_key_down(s))
			{
				printf("GO DOWN");
				cameraToWorld.translate(vec3(0, 1, 0)*-camera_Speed);

			}
			if (is_key_down(d))
			{
				printf("GO RIGHT");
				cameraToWorld.translate(vec3(1,0, 0)*camera_Speed);

			}
		}

		else
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
		}
		if (is_key_going_down(e))
		{
			if (branch_Width < 5.0f)
			{
				branch_Width += 0.5f;
			}
		}
		if (is_key_going_down(q))
		{
			if (branch_Width > 0.0f)
			{
				branch_Width -= 0.5f;
			}
		}
		if (is_key_going_down(z))
		{
			angle -= 1;
			generate_points(axiom, rules, current_Iterate);
		}
		if (is_key_going_down(c))
		{
			angle += 1;
			generate_points(axiom, rules, current_Iterate);
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

		midpoint = (top_right + bottom_left) * 0.5f;
	}

	void render(mat4t &cameraToWorld)
	{
		mat4t modelToProjection = mat4t::build_projection_matrix(transform, cameraToWorld);

		glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)point_Node.data());
		glEnableVertexAttribArray(attribute_pos);

		glVertexAttribPointer(attribute_color, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(point_Node.data() + 3));
		glEnableVertexAttribArray(attribute_color);

		glLineWidth(branch_Width);
	}

	void draw_line()
	{
		glClearColor(0, 0, 0, 1); //Clears background colour to black
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBegin(GL_LINES);
		glColor3f(0.87f, 0.72f, 0.52f);
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

			// clear the background to black
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			draw_line();
			hotkey_Controls();
			render(cameraToWorld);

    }
  };
}
