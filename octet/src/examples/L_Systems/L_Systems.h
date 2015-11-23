#include <fstream>
#include "lsystem_Shader.h"

namespace octet {

	class L_Node
	{
	public:
		mat4t transform;
		vec3 colour;
		vec3 end;
	};
	class L_Systems : public app {

		lsystem_Shader shader;
		ref<visual_scene> app_scene;
		float branch_Length = 1.0f;
		float branch_Width = 0.5f;
		float angle;
		float scale;
		vec3 cur_Colour;
		vec3 max;
		vec3 min;
		vec2 start_Pos;
		vec2 current_Pos;
		vec2 current_Dir;
		float current_Angle;
		string axiom;
		string initial_Axiom;
		dynarray<string> rules;
		dynarray<string> tree_Files;
		dynarray<L_Node> point_Node;
		dynarray<L_Node> point_State;
		dynarray<float> angle_State;
		L_Node current_Node;
		dynarray<vec2> direction_State;
		dynarray<float> vertices;
		int iteration_Count;
		int tree_Example = 1;
		int current_Iterate = 0;
		float camera_Speed = 0.2f;
		mat4t cameraToWorld;
		mat4t transform;
		bool light_On = false;

	public:
		// this is called when we construct the class before everything is initialised.
		L_Systems(int argc, char **argv) : app(argc, argv) {
			transform.loadIdentity();
		}

		// this is called once OpenGL is initialized
		void app_init() {
			start_Pos = vec2(0, -1);
			current_Pos = start_Pos;
			current_Angle = 0.0f;
			tree_Files.push_back("../../../assets/Tree1.csv");
			tree_Files.push_back("../../../assets/Tree2.csv");
			tree_Files.push_back("../../../assets/Tree3.csv");
			tree_Files.push_back("../../../assets/Tree4.csv");
			tree_Files.push_back("../../../assets/Tree5.csv");
			tree_Files.push_back("../../../assets/Tree6.csv");
			tree_Files.push_back("../../../assets/Tree7.csv");
			tree_Files.push_back("../../../assets/Tree8.csv");
			load_Data(tree_Files[0]);
			cameraToWorld.loadIdentity();
			cameraToWorld.translate(0.0f, 1.9f, 2.0f);
			glClearColor(0, 0.6f, 0.6f, 1);
		}

		void create_Tree()
		{
			// Reset the axiom to the initial value
			axiom = initial_Axiom;

			// Depending on the number of iterations, run the rule engine recursively on the axiom
			for (int j = 0; j < current_Iterate; j++)
			{
				axiom = iterate_Rules(axiom, rules);
			}

			// Generate all the required data (nodes) using the turtle algorithm
			parse_Data(axiom);

			// Using the previously generated data nodes, generate our geometry (vertices)
			Create_Vertices();
		}

		void load_Data(string file_Data) //loads in the data
		{
			current_Node.transform = transform;
			shader.init();
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
			}
			create_Tree();
			
		}
		void light_Tree()
		{
			float r = ((double)rand() / (RAND_MAX) / 1.0f + 0.001f);
			float g = ((double)rand() / (RAND_MAX) / 1.0f + 0.001f);
			float b = ((double)rand() / (RAND_MAX) / 1.0f + 0.001f);

			cur_Colour = vec3(r, g, b);
			redraw_Trees();
		}

		void Create_Vertices()
		{
			transform.loadIdentity();
			scale = 5.0f;
			max = vec3(0);
			min = vec3(0);

			int num_nodes = point_Node.size();
			vertices.reset();
			for (int j = 0; j < num_nodes; j++)
			{
				// update the current tree. Shader expects z co-ord regardless of 2D/3D
				max.x() = (point_Node[j].transform.row(3).x() > max.x()) ? point_Node[j].transform.row(3).x() : max.x(); 
				max.y() = (point_Node[j].transform.row(3).y() > max.y()) ? point_Node[j].transform.row(3).y() : max.y();
				max.z() = (point_Node[j].transform.row(3).z() > max.z()) ? point_Node[j].transform.row(3).z() : max.z();
				min.x() = (point_Node[j].transform.row(3).x() < min.x()) ? point_Node[j].transform.row(3).x() : min.x();
				min.y() = (point_Node[j].transform.row(3).y() < min.y()) ? point_Node[j].transform.row(3).y() : min.y();
				min.z() = (point_Node[j].transform.row(3).z() > min.z()) ? point_Node[j].transform.row(3).z() : min.z();

				max.x() = (point_Node[j].end.x() > max.x()) ? point_Node[j].end.x() : max.x();
				max.y() = (point_Node[j].end.y() > max.y()) ? point_Node[j].end.y() : max.y();
				max.z() = (point_Node[j].end.z() > max.z()) ? point_Node[j].end.z() : max.z();
				min.x() = (point_Node[j].end.x() < min.x()) ? point_Node[j].end.x() : min.x();
				min.y() = (point_Node[j].end.y() < min.y()) ? point_Node[j].end.y() : min.y();
				max.z() = (point_Node[j].end.z() < min.z()) ? point_Node[j].end.z() : min.z();

				//Push back initial vertex co-ords
				vertices.push_back(point_Node[j].transform.row(3).x());
				vertices.push_back(point_Node[j].transform.row(3).y());
				vertices.push_back(point_Node[j].transform.row(3).z());

				// Push initial vertex colour
				vertices.push_back(point_Node[j].colour.x());
				vertices.push_back(point_Node[j].colour.y());
				vertices.push_back(point_Node[j].colour.z());

				// Push back final vertex co-ords
				vertices.push_back(point_Node[j].end.x());
				vertices.push_back(point_Node[j].end.y());
				vertices.push_back(point_Node[j].end.z());

				// Push final vertex colour
				vertices.push_back(point_Node[j].colour.x());
				vertices.push_back(point_Node[j].colour.y());
				vertices.push_back(point_Node[j].colour.z());
			}
			scale = (max - min).length();
			
			if (scale > 0.0001f)
			{
				transform.scale(5.0f / scale, 4.0f / scale, 5.0f / scale); //scaled so the tree remains the same size.
			}
		}

		void parse_Data(string data) // Reads rules and parses
		{
			vec3 end_pos;

			// Reset (clear) everything
			point_Node.reset();
			angle_State.reset();
			current_Angle = 0.0f;

			current_Node.transform = transform;
			current_Node.end = vec3(0, 0, 0);
			current_Node.colour = cur_Colour;
			L_Node Node;
			for (int j = 0; j < data.size(); j++) {
				switch (data[j])
				{
				case 'F':
					{
					//Create line segment
					mat4t invertTransform;
					mat4t trans_Node = current_Node.transform;
					trans_Node.loadIdentity();
					trans_Node.translate(current_Node.end);
					trans_Node.invertQuick(invertTransform);
					vec4 localForward = vec4(0.0f, 0.0f, 1.0f, 0.0f) * invertTransform;
					trans_Node.rotate(current_Angle, localForward.x(), localForward.y(), localForward.z());
					Node.transform = trans_Node;
					float r = red;
					float g = green;
					float b = blue;
					Node.colour = current_Node.colour;
					trans_Node.invertQuick(invertTransform);
					vec4 local_Up = vec4(0, 1, 0, 0) * invertTransform;
					Node.end = (trans_Node.row(3).xyz() + (local_Up * branch_Length));
					point_Node.push_back(Node);
					current_Node = Node;
					}
					break;

				case '[':
					{
					// Push (stack) the position and angle
					point_State.push_back(current_Node);
					angle_State.push_back(current_Angle);
					}
					break;

				case ']':
					// Pop (Pull) the position and angle
					{
					current_Node = point_State.back();
					point_State.pop_back();
					current_Angle = angle_State.back();
					angle_State.pop_back();
					}
					break;

				case '+':
					{
					// rotate right
					current_Angle += angle;
					}
					break;

				case '-':
					{
					// rotate left 
					current_Angle -= angle;
					break;
					}
				}

			}
		}

		string iterate_Rules(string axiom, dynarray<string> &rules) //goes through rules and iterates
		{
			dynarray<char> data;
			for (int i = 0; i < axiom.size(); i++)
			{
				int j = 0;
				for (j = 0; j < rules.size(); j++)
				{					
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
					printf("%S\n","none");
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

		void redraw_Scene()
		{
			int vx = 0, vy = 0;
			get_viewport_size(vx, vy);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		void hotkey_Controls()
		{
			if (is_key_down(key_shift))
			{
				//move camera
				if (is_key_down(w))
				{
					cameraToWorld.translate(cameraToWorld.y()*camera_Speed);
				}
				if (is_key_down(a))
				{
					cameraToWorld.translate(cameraToWorld.x()*-camera_Speed);
				}
				if (is_key_down(s))
				{
					cameraToWorld.translate(cameraToWorld.y()*-camera_Speed);

				}
				if (is_key_down(d))
				{
					cameraToWorld.translate(cameraToWorld.x()*camera_Speed);
				}
				if (is_key_going_down(red))
				{
					//make tree red
					float r = 1;
					float g = 0;
					float b = 0;
					cur_Colour = vec3(r, g, b);
					redraw_Trees();
				}
				if (is_key_going_down(green))
				{
					//make tree green
					float r = 0;
					float g = 1;
					float b = 0;
					cur_Colour = vec3(r, g, b);
					redraw_Trees();
				}
				if (is_key_going_down(blue))
				{
					
					//make tree blue
					float r = 0;
					float g = 0;
					float b = 1;
					cur_Colour = vec3(r, g, b);
					redraw_Trees();
				}
				if (is_key_going_down(yellow))
				{
					//make tree yellow
					float r = 1;
					float g = 1;
					float b = 0;
					cur_Colour = vec3(r, g, b);
					redraw_Trees();
				}
				if (is_key_going_down(h))
				{
					//make tree white
					float r = 1;
					float g = 1;
					float b = 1;
					cur_Colour = vec3(r, g, b);
					redraw_Trees();
				}

			}
			else if (is_key_down(key_ctrl))
			{
				if (is_key_going_down(red))
				{
					//red background
					redraw_Scene();
					glClearColor(1, 0, 0,1);
					
				}
				if (is_key_going_down(green))
				{
					//green background
					redraw_Scene();
					glClearColor(0, 1, 0, 1);
				}
				if (is_key_going_down(blue))
				{
					//blue background
					redraw_Scene();
					glClearColor(0, 0, 1, 1);
				}
				if (is_key_going_down(yellow))
				{
					//yellow background
					redraw_Scene();
					glClearColor(1, 1, 0, 1);
				}
				if (is_key_going_down(h))
				{
					//black background
					redraw_Scene();
					glClearColor(0, 0, 0, 1);
				}
				if (is_key_going_down(x))
				{
					//white background
					redraw_Scene();
					glClearColor(1, 1, 1, 1);
				}
				if (is_key_going_down(c))
				{
					//reset background
					redraw_Scene();
					glClearColor(0, 0.6f, 0.6f, 1);
				}

				//zoom camera
				if (is_key_down(w))
				{
					cameraToWorld.translate(cameraToWorld.z()*-camera_Speed);
				}
				if (is_key_down(s))
				{
					cameraToWorld.translate(cameraToWorld.z()*camera_Speed);
				}
			}

			else
			{
				if (is_key_going_down(w))
				{
					if (current_Iterate <= iteration_Count)
					{
						current_Iterate++;
						create_Tree();
						//Should Iterate upwards
						axiom = iterate_Rules(axiom, rules);
					}
				}
				if (is_key_going_down(d))
				{
					if (tree_Example < tree_Files.size() - 1)
					{
						cur_Colour = vec3(0, 0, 0);
						light_On = false;
						reset_Function();
						//Should change rules forward
						tree_Example++;
						load_Data(tree_Files[tree_Example]);
					}
				}
				if (is_key_going_down(a))
				{
					if (tree_Example > 0)
					{
						cur_Colour = vec3(0, 0, 0);
						light_On = false;
						reset_Function();
						//Should change rules backwards
						tree_Example--;
						load_Data(tree_Files[tree_Example]);
					}
				}
				if (is_key_going_down(s))
				{
					if (current_Iterate > 0)
					{
						current_Iterate--;
						create_Tree();
					}
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
					create_Tree();
				}
				if (is_key_going_down(c))
				{
					angle += 1;
					create_Tree();
				}
				if (is_key_going_down(x))
				{
					if (light_On == false)
					{
						light_On = true;
					}
					else if (light_On == true)
					{
						light_On = false;
					}
				}
			}
		}


		void render(mat4t &cameraToWorld)
		{
			mat4t modelToProjection = mat4t::build_projection_matrix(transform, cameraToWorld);
			shader.render(modelToProjection, 0);

			glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)vertices.data());
			glEnableVertexAttribArray(attribute_pos);

			glVertexAttribPointer(attribute_color, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(vertices.data() + 3));
			glEnableVertexAttribArray(attribute_color);
			glLineWidth(branch_Width);
			glDrawArrays(GL_LINES, 0, point_Node.size() << 1);
		}

		void redraw_Trees()
		{
			create_Tree();
		}

		// this is called to draw the world
		void draw_world(int x, int y, int w, int h) {

			int vx = 0, vy = 0;
			get_viewport_size(vx, vy);
			glViewport(x, y, w, h);
			hotkey_Controls();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDisable(GL_DEPTH_TEST);
			// allow alpha blend (transparency when alpha channel is 0)
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			if (light_On == true)
			{
				light_Tree();
			}
			render(cameraToWorld);

		}
	};
}