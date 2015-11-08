#Tools and Middleware

###Controls
####Mouse->Look Around
####E->Fire squares

#####Youtube Link: https://youtu.be/ihSoHMheeM4

In the tools and middleware project, a simple demonstration of a hinge joint and spring joint as well as its relevant limitations are shown. The project was done utilizing an integration of both the example_fps and example_shapes projects.  The project is a simple demonstration of a double door (hinges) and a box with a ball attached (spring). The project had to start off with setting the terrain flat (with thanks to Mircea).

vec3 p = bb_min + pos;
vec3 uv = uv_min + vec3((float)pos.x(), (float)pos.z(), 0) * uv_delta;
return mesh::vertex(p, vec3(0, 1, 0), uv);

This prevents us from creating the object on an uneven plane causing joint issues later on. The next step was to modify the shapes, I had to modify the zcylinder.h script for the cylinder due to the fact that rotating the shape itself not only altered its rotation, but also the local positions of the shape, causing unnecessary delays in finding out the exact co-ordinates to set up a hinge joint. The below code looks like this:

sink.add_vertex(center + vec3(c*radius , -half_extent, s*radius), vec3(c, s, 0), vec3(i * ku, 0, 0));
sink.add_vertex(center + vec3(c*radius, half_extent, s*radius), vec3(c, s, 0), vec3(i * ku, 1, 0));
sink.add_vertex(center + vec3(c*radius, -half_extent, s*radius), vec3(0, 0, -1), vec3(c, s, -1));
sink.add_vertex(center + vec3(c*radius, half_extent, s*radius), vec3(0, 0,  1), vec3(c, s,  1));

The half_extent handled where the length of the cylinder would be drawn, originally along the z axis. Simply altering it so it is drawn along the y-axis fixed the issue of having to rotate the object, instead creating the cylinder to be a ‘ycylinder’ instead. This simple fix then provided us with a simple workaround to an otherwise frustrating problem.

Another change I had to modify was the player camera and its relative rotation, where the view of the current camera did not focus on the z-axis and instead was rotated a little bit. To alter the problem:
 
	 mouse_look_helper.update(camera_to_world);
	 mouse_look_helper.set_invert_mouse(true); 
	 camera_node->rotate(180.0f, vec3(0, 1, 0));
	 player_node->rotate(180.0f, vec3(0, 1, 0));
	 fps_helper.update(player_node, camera_node);

mouse_look_helper.update would update the current camera position relative to the world, the next line mouse_look_helper,set_invert_mouse(true) was necessary for the rotation to work. This is due to the rotate being set after the update function, whereby if the invert did not take place, the player and camera would have irregular rotation patterns, and if the rotate was set before the update function, the rotation would not take place at all.

The next was altering the visual_scene.h file, with major thanks to Alejandro Villanueva, whereby we created our own method of hinge joints and spring joints via altering the mesh instance. Instead of the code:

               mesh_instance *add_shape(…)

we instead replaced it with:

 scene_node *add_shape(…)

and returned the node instead of the result.

This allowed us to perform simpler calls and integrating rigidbodies, whereby we could use:

scene_node *node2 = app_scene->add_shape(mat, new mesh_box(vec3(4, 4, 0.2f)), yellow, true);

Without direct pointing to the nodes themselves.
The CSV integration for this project was relatively simple:

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
Whereby the CSV read the first column of letters and determined the type of objects to add, and the 2nd, 3rd and 4th column read the x,y and z position of the object respectively. This was made to generate a simple border around the player.
Next to be added was a shoot function, in this case, generating squares and adding a force to it, in which this would allow the player to test collision with certain objects in the scene.
The hinge joint with its respective limits allows the user to fire squares at the object in order to test the collision between the 2 rigidbodies, and see the limits of the doors stopping and giving a slightly lower force ‘pushback’ once the limit is reached.
	  
	HingeConstraint2->setLimit(-90, 90, 2.0f, 2.0f, 1.0f);
	app_scene->addHingeConstraint(HingeConstraint2);

The above code sets the limit of rotation in the door joints, where it will not pass the -90 and 90 degree marks.
The spring joint focuses on setting the object (In this case, the green ball) to one specific location with the minimum and maximum position, and any sort of collision via the bullets would either pull or push the green ball to the original position.

![Alt text](https://github.com/kt-chin/octet/blob/master/octet/assets/MiddlewareSC1.png?raw=true"Screenshot of Demo 1")
![Alt text](https://github.com/kt-chin/octet/blob/master/octet/assets/MiddlewareSC2.png?raw=true"Screenshot of Demo 2")