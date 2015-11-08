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
	
	ref<material> red;
	ref<material> blue;
	ref<material> yellow;
	ref<material> green;
	ref<material> pink;
	ref<material> orange;

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

	example_geometry_source source;

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

      red = new material(vec4(1,  0, 0, 1));
      green = new material(vec4(0, 1, 0, 1));
	  blue = new material(vec4(0, 0, 1, 1));
	  yellow = new material(vec4(1,1, 0,1));
	  pink = new material(vec4(1, 0.411f, 0.705f,1));
	  orange = new material(vec4(1, 0.27f, 0, 1));
      mat4t mat;
     // mat.translate(-3, 1, 0);
      

	  //First Door
	  mat.loadIdentity();
	  mat.translate(-10, 0, 20);
	  scene_node *node1 = app_scene->add_shape(mat, new mesh_cylinder(zcylinder(vec3(0, 4, 0), 0.5f, 4)), blue, false);
	  btRigidBody *rb1 = node1->get_rigid_body();

	  mat.loadIdentity();
	  mat.translate(-14, 4.5f, 20);
	  scene_node *node2 = app_scene->add_shape(mat, new mesh_box(vec3(4, 4, 0.2f)), yellow, true);
	  btRigidBody *rb2 = node2->get_rigid_body();


	  btVector3 *anchor1 = new btVector3(0.8f, 8, 0);
	  btVector3 *anchor2 = new btVector3(-4, 4.0f, 0);


	  btVector3 *axis1 = new btVector3(0, 1, 0);
	  btVector3 *axis2 = new btVector3(0, 1, 0);

	  btHingeConstraint *HingeConstraint = new btHingeConstraint(*rb2, *rb1, *anchor2, *anchor1, *axis2, *axis1, true); //Cylinder joints



	  HingeConstraint->setLimit(-90, 90, 2.0f, 2.0f, 1.0f);

	  app_scene->addHingeConstraint(HingeConstraint);


	  // Second Door
	  mat.loadIdentity();
	  mat.translate(7.5f, 0, 20);
	  scene_node *node3 = app_scene->add_shape(mat, new mesh_cylinder(zcylinder(vec3(0, 4, 0), 0.5f, 4)), blue, false);
	  btRigidBody *rb3 = node3->get_rigid_body();

	  mat.loadIdentity();
	  mat.translate(3.5f, 4, 20);
	  scene_node *node4 = app_scene->add_shape(mat, new mesh_box(vec3(4, 4, 0.2f)), yellow, true);
	  btRigidBody *rb4 = node4->get_rigid_body();

	  btVector3 *anchor3 = new btVector3(-0.8f, 8, 0);
	  btVector3 *anchor4 = new btVector3(4, 4, 0); 

	  btVector3 *axis3 = new btVector3(0, 1, 0);
	  btVector3 *axis4 = new btVector3(0, 1, 0);

	  btHingeConstraint *HingeConstraint2 = new btHingeConstraint(*rb4, *rb3, *anchor4, *anchor3, *axis4, *axis3, true); //Cylinder joints

	  HingeConstraint2->setLimit(-90, 90, 2.0f, 2.0f, 1.0f); //constraints
	  app_scene->addHingeConstraint(HingeConstraint2);


	  //Spring
	  //box
      mat.loadIdentity();
	  mat.translate(-40, 2, 0);
      //mat.translate(0, 2, 40); x axis
      scene_node *box_node = app_scene->add_shape(mat, new mesh_box(vec3(2, 2, 2)), red, false);
	  btRigidBody *box_Rb = box_node->get_rigid_body();
	  //sphere
	  mat.loadIdentity();
	  mat.translate(-40,2,5);
	  //mat.translate(5, 2, 40); x axis
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
	  pGen6DOFSpring->setLinearUpperLimit(btVector3(0., 0, 30.));
	  pGen6DOFSpring->setLinearLowerLimit(btVector3(0., 0, 10.));

	  pGen6DOFSpring->setAngularLowerLimit(btVector3(0.f, 0.f, 0.f));
	  pGen6DOFSpring->setAngularUpperLimit(btVector3(0.f, 0.f, 40.f));


	  app_scene->AddSpringConstraint(pGen6DOFSpring);


	  pGen6DOFSpring->enableSpring(0, true);
	  pGen6DOFSpring->setStiffness(0, 30.f);
	  pGen6DOFSpring->setDamping(0,30.0f);
	  pGen6DOFSpring->setEquilibriumPoint();

	  csv_Generate();


	  mat.loadIdentity();



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
	
	//CSV file
	void csv_Generate()
	{
		std::ifstream is("../../../assets/wall_Border.csv");
		if (is.bad())
		{
			printf("Error opening csv file\n");
			return;																																													
		}
		// store the line here
		char buffer[2048];

		// loop over lines
		while (!is.eof()) {
			is.getline(buffer, sizeof(buffer));
			vec3 pos;
			mat4t rot;
			//float rotate;
			
			char object_type = 0;

			// loop over columns
			char *b = buffer;
			for (int col = 0;; ++col) {
				char *e = b;
				while (*e != 0 && *e != ',') ++e;

				if (col == 0) {
					object_type = b[0];
				}
				else if (col == 1) {
					pos.x() = atof(b);
				}
				else if (col == 2) {
					pos.y() = atof(b);
				}
				else if (col == 3) {
					pos.z() = atof(b);
				}
				/*else if (col == 4) {
					rot.rotate(atof(b));
				}*/

				if (*e != ',') break;
				b = e + 1;
			}
			// line ended
			add_object(object_type, pos);
		}
	}
	public:

		scene_node* add_object(char type, vec3 pos)
		{
			if (type == 'a')
			{
				return add_wall1(pos);
			}
			else if(type == 'b')
			{
				return add_wall2(pos);
			}
		}
		scene_node* add_wall1(vec3 pos)
		{
			mat4t position;
			position.translate(pos);

			mesh_box *box = new mesh_box(vec3(1000,100,0), position);

			scene_node* box_instance = app_scene->add_shape(position, box, pink, false);

			return box_instance;
		}
		scene_node* add_wall2(vec3 pos)
		{
			mat4t position;
			position.translate(pos);

			mesh_box *box = new mesh_box(vec3(0, 100, 1000), position);

			scene_node* box_instance = app_scene->add_shape(position, box, pink, false);

			return box_instance;
		}
    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);

	  scene_node *camera_node = the_camera->get_node();
	  mat4t &camera_to_world = camera_node->access_nodeToParent();
	  mouse_look_helper.update(camera_to_world);
	  mouse_look_helper.set_invert_mouse(true); //Inverted as workaround from rotation.

	  //Rotates player camera
	 camera_node->rotate(180.0f, vec3(0, 1, 0));
	 player_node->rotate(180.0f, vec3(0, 1, 0));
	 fps_helper.update(player_node, camera_node);

	  //Fire button
	  if (is_key_going_down('E'))
	  {
		  mat4t mat;
		  /*my_MiddlewareGame* app = (my_MiddlewareGame *)data;*/
		  vec3 pos = (player_node->get_position().z()) * camera_to_world;// THIS WORKS
		  scene_node * cam_node = the_camera->get_node();
		  //vec3 pos = (player_node->get_position() + cam_node->get_z() * -10.0f)* camera_to_world;
		  
		  mat.translate(pos);
		 
		  scene_node * box = app_scene->add_shape(mat, new mesh_box(vec3(1, 1, 1)), orange, true);
		  
		  box->translate(pos);
		  box->get_scene_node()->apply_central_force(cam_node->get_z() *-500.0f);
		  btRigidBody *bullet_Rb = box->get_rigid_body();
		  printf("%f", player_node->get_position());
		  printf("\n");
		  printf("Fire!");
	  }

      // update matrices. assume 60 fps.
      app_scene->update(1.0f/15);

	  
      // draw the scene
      app_scene->render((float)vx / vy);
    }
  };
}
