////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
namespace octet {
  /// Scene containing a box with octet.
  class my_MiddlewareGame : public app {
    // scene for drawing box
    ref<visual_scene> app_scene;

	mouse_look mouse_look_helper;
	helper_fps_controller fps_helper;

	ref<scene_node> player_node;

	ref<camera_instance> the_camera;


  public:
    my_MiddlewareGame(int argc, char **argv) : app(argc, argv) {
    }

    ~my_MiddlewareGame() {
    }

    /// this is called once OpenGL is initialized
    void app_init() {
		mouse_look_helper.init(this, 200.0f / 360.0f, false);
		fps_helper.init(this);
		app_scene = new visual_scene();
		app_scene->create_default_camera_and_lights();
		the_camera = app_scene->get_camera_instance(0);
		the_camera->get_node()->translate(vec3(0, 4, 0));
		the_camera->set_far_plane(10000);

      material *red = new material(vec4(1, 0, 0, 1));
      material *green = new material(vec4(0, 1, 0, 1));
      material *blue = new material(vec4(0, 0, 1, 1));

      mat4t mat;
      mat.translate(-3, 6, 0);
      app_scene->add_shape(mat, new mesh_sphere(vec3(2, 2, 2), 2), red, true);

      mat.loadIdentity();
      mat.translate(0, 10, 0);
      app_scene->add_shape(mat, new mesh_box(vec3(2, 2, 2)), red, true);

      mat.loadIdentity();
      mat.translate( 3, 6, 0);
      app_scene->add_shape(mat, new mesh_cylinder(zcylinder(vec3(0, 0, 0), 2, 4)), blue, true);


      // ground
      mat.loadIdentity();
      mat.translate(0, -1, 0);
      app_scene->add_shape(mat, new mesh_box(vec3(200, 1, 200)), green, false);

	  float player_height = 1.83f;
	  float player_radius = 0.25f;
	  float player_mass = 90.0f;

	  mat.loadIdentity();
	  mat.translate(0, player_height*0.5f, -50);

	  mesh_instance *mi = app_scene->add_shape(
		  mat,
		  new mesh_sphere(vec3(0), player_radius),
		  new material(vec4(0, 0, 1, 1)),
		  true, player_mass,
		  new btCapsuleShape(0.25f, player_height)
		  );
	  player_node = mi->get_node();
    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);

	  scene_node *camera_node = the_camera->get_node();
	  mat4t &camera_to_world = camera_node->access_nodeToParent();
	  mouse_look_helper.update(camera_to_world);

	  fps_helper.update(player_node, camera_node);

      // update matrices. assume 30 fps.
      app_scene->update(1.0f/30);

      // draw the scene
      app_scene->render((float)vx / vy);
    }
  };
}
