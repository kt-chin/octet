namespace octet {

  class L_Systems : public app {
    // scene for drawing box

	  ref<material> green;
	  ref<material> brown;
	  ref<visual_scene> app_scene;

	ref<material> green;
	ref<material> brown;
    ref<visual_scene> app_scene;

  public:
    // this is called when we construct the class before everything is initialised.
    L_Systems(int argc, char **argv) : app(argc, argv) {
    }




    // this is called once OpenGL is initialized

	float branch_length = 0.1;

    /// this is called once OpenGL is initialized

    void app_init() {
      app_scene =  new visual_scene();
      app_scene->create_default_camera_and_lights();
	  green = new material(vec4(0, 1, 0, 1));
	  brown = new material(vec4(0.87f, 0.72f, 0.52f, 1));

	  //float pos_X = ((rectScreen))

	  draw_Log(vec3(4,5,0));

	  draw_Log(vec3(0,0,0));

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
