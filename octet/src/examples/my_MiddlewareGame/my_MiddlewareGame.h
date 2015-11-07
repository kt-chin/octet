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
	//btDiscreteDynamicsWorld* world;

	mouse_look mouse_look_helper;
	helper_fps_controller fps_helper;

	ref<scene_node> player_node;

	ref<camera_instance> the_camera;
	struct example_geometry_source : mesh_terrain::geometry_source {
		mesh::vertex vertex(
			vec3_in bb_min, vec3_in uv_min, vec3_in uv_delta, vec3_in pos
			) {
			static const vec3 bumps[] = {
				vec3(100, 0, 100), vec3(50, 0, 50), vec3(150, 0, 50)
			};
			vec3 p = bb_min + pos;
			vec3 uv = uv_min + vec3((float)pos.x(), (float)pos.z(), 0) * uv_delta;
			return mesh::vertex(p, vec3(0, 1, 0), uv); //made terrain flat thanks to Mircea

			float dy_dx = std::cos(pos.x() * 0.01f);
			float dy_dz = std::cos(pos.z() * 0.03f);

		}
	};
	//bullets
	dynarray<scene_node> bullet_Node;

	example_geometry_source source;

  public:
    my_MiddlewareGame(int argc, char **argv) : app(argc, argv) {
    }

    ~my_MiddlewareGame() {
    }

    /// this is called once OpenGL is initialized

	//vec3 get_Position()
	//{
	//	return 
	//}


    void app_init() {
		mouse_look_helper.init(this, 200.0f / 360.0f, false);
		fps_helper.init(this);
		app_scene = new visual_scene();
		app_scene->create_default_camera_and_lights();
		the_camera = app_scene->get_camera_instance(0);
		the_camera->get_node()->translate(vec3(0, 4, 0));
		the_camera->set_far_plane(10000);
		
      material *red = new material(vec4(1,  0, 0, 1));
      material *green = new material(vec4(0, 1, 0, 1));
      material *blue = new material(vec4(0,0, 1, 1));
	  material *yellow = new material(vec4(1, 0.165f, 0));



      mat4t mat;
      mat.translate(-3, 1, 0);
      

	  //Hanging sign
	  //box
      mat.loadIdentity();
      mat.translate(0, 2, 40);
      scene_node *box_node = app_scene->add_shape(mat, new mesh_box(vec3(2, 2, 2)), red, false);
	  btRigidBody *box_Rb = box_node->get_rigid_body();
	  //sphere
	  mat.loadIdentity();
	  mat.translate(5, 2, 40);
	  scene_node *sphere_node = app_scene->add_shape(mat, new mesh_sphere(vec3(2, 2, 2), 2), green, true);
	  btRigidBody *sphere_Rb = sphere_node->get_rigid_body();

	  //Spring constraint
	  btTransform spring_ConstraintA, spring_ConstraintB;
	  spring_ConstraintA = btTransform::getIdentity();
	  spring_ConstraintA.setOrigin(btVector3(btScalar(2.5f), btScalar(0), btScalar(0)));
	  spring_ConstraintB = btTransform::getIdentity();
	  spring_ConstraintB.setOrigin(btVector3(btScalar(0), btScalar(0), btScalar(0)));

	  btGeneric6DofSpringConstraint* pGen6DOFSpring = new btGeneric6DofSpringConstraint(*box_Rb, *sphere_Rb, spring_ConstraintA, spring_ConstraintB, true);

	  //Assigns position of spring
	  pGen6DOFSpring->setLinearUpperLimit(btVector3(40., 0, 0.));
	  pGen6DOFSpring->setLinearLowerLimit(btVector3(10., 0, 0.));

	  pGen6DOFSpring->setAngularLowerLimit(btVector3(0.f, 0.f, 0.f));
	  pGen6DOFSpring->setAngularUpperLimit(btVector3(50.f, 0.f, 0.f));

	  app_scene->AddSpringConstraint(pGen6DOFSpring);
	  //pGen6DOFSpring->setDbgDrawSize(btScalar(5.f));

	  pGen6DOFSpring->enableSpring(0, true);
	  pGen6DOFSpring->setStiffness(0, 3.0f);
	  pGen6DOFSpring->setDamping(0, 0.5f);
	  pGen6DOFSpring->enableSpring(5, true);
	  pGen6DOFSpring->setStiffness(0, 3.0f);
	  pGen6DOFSpring->setDamping(0, 0.8f);
	  pGen6DOFSpring->setEquilibriumPoint();

      //First Door
      mat.loadIdentity();
	  mat.translate(0, 0, 10);
      scene_node *node1 = app_scene->add_shape(mat, new mesh_cylinder(zcylinder(vec3(0, 4, 0), 0.5f, 4)), blue, false);
	  btRigidBody *rb1 = node1->get_rigid_body();
	  
	  mat.loadIdentity();
	  mat.translate(4,4.5f,10);
	  scene_node *node2 = app_scene->add_shape(mat, new mesh_box(vec3(4, 4, 0.2f)), yellow, true);
	  btRigidBody *rb2 = node2->get_rigid_body();

	  /*btVector3 *anchor1 = new btVector3(2, 8, 0);
	  btVector3 *anchor2 = new btVector3(-4, 4.0f, 0);*/
	  btVector3 *anchor1 = new btVector3(0.8f, 8, 0);
	  btVector3 *anchor2 = new btVector3(-4, 4.0f, 0); //Maybe?


	  btVector3 *axis1 = new btVector3(0, 1, 0);
	  btVector3 *axis2 = new btVector3(0, 1, 0);

	  btHingeConstraint *HingeConstraint = new btHingeConstraint(*rb2, *rb1, *anchor2, *anchor1, *axis2, *axis1, true); //Cylinder joints



	  HingeConstraint->setLimit(-90, 90, 2.0f, 2.0f, 1.0f);

	  app_scene->addHingeConstraint(HingeConstraint);


	  // Second Door
	  mat.loadIdentity();
	  mat.translate(17.5f, 0, 10);
	  scene_node *node3 = app_scene->add_shape(mat, new mesh_cylinder(zcylinder(vec3(0, 4, 0), 0.5f, 4)), blue, false);
	  btRigidBody *rb3 = node3->get_rigid_body();

	  mat.loadIdentity();
	  mat.translate(13.5f, 4, 10);
	  scene_node *node4 = app_scene->add_shape(mat, new mesh_box(vec3(4, 4, 0.2f)), yellow, true);
	  btRigidBody *rb4 = node4->get_rigid_body();

	  /*btVector3 *anchor3 = new btVector3(2, 8, 0);
	  btVector3 *anchor4 = new btVector3(4, 4.0f, 0);*/
	  btVector3 *anchor3 = new btVector3(-0.8f, 8, 0);
	  btVector3 *anchor4 = new btVector3(4,4, 0); //Maybe?

	  btVector3 *axis3 = new btVector3(0, 1, 0);
	  btVector3 *axis4 = new btVector3(0, 1, 0);

	  btHingeConstraint *HingeConstraint2 = new btHingeConstraint(*rb4, *rb3, *anchor4, *anchor3, *axis4, *axis3, true); //Cylinder joints

	  HingeConstraint2->setLimit(-90, 90,2.0f, 2.0f,1.0f); //constraints
	  app_scene->addHingeConstraint(HingeConstraint2);


	  mat.loadIdentity();
	  //mat.translate(0, -0.5f, 0);
	  


      // ground
	  app_scene->add_shape(
		  mat,
		  new mesh_terrain(vec3(100.0f, 0, 100.0f), ivec3(100, 1, 100), source),
		  new material(new image("assets/rocks.jpg")),
		  false, 0
		  );

	  float player_height = 1.5f;
	  float player_radius = 0.25f;
	  float player_mass = 90.0f;

	  mat.loadIdentity();
	  mat.translate(0, player_height*1.5f, 0);

	  scene_node *mi = app_scene->add_shape(
		  mat,
		  new mesh_sphere(vec3(0), player_radius),
		  new material(vec4(0, 0, 1, 1)),
		  true, player_mass,
		  new btCapsuleShape(0.25f, player_height*1.5f)
		  );
	  player_node = mi;


    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);

	  //printf("%f",player_node->get_position().y());
	  //printf("\n");
	  
	  scene_node *camera_node = the_camera->get_node();

	  //Fire button
	  if (is_key_going_down('E'))
	  {
		  material *green = new material(vec4(0, 1, 0, 1));
		  mat4t mat;
		  mat.translate(-3, 1, 0);
		  scene_node *sphere_node = app_scene->add_shape(mat, new mesh_sphere(vec3(2, 2, 2), 2), green, true);
		  printf("Fire!");
	  }

	  mat4t &camera_to_world = camera_node->access_nodeToParent();
	  
	  mouse_look_helper.update(camera_to_world);
	  mouse_look_helper.set_invert_mouse(true); //Inverted as workaround from rotation.
	  //Rotates player camera
	 camera_node->rotate(180.0f, vec3(0, 1, 0));
	 player_node->rotate(180.0f, vec3(0, 1, 0));
	 fps_helper.update(player_node, camera_node);
	 
	 

      // update matrices. assume 60 fps.
      app_scene->update(1.0f/15);

	  
      // draw the scene
      app_scene->render((float)vx / vy);
    }
  };
}
