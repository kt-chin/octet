In the invaderers game, the objective is to navigate through the maze towards the portal whilst  avoiding enemies that come barreling down along the y axis shooting at the player. Movement controls are similar to old asteroids games, where left and right rotates the player, and forward and back moves the player respectively. 

The first thing to do was to create a method that handled rotation (Thanks to Raul):

void rotateMatrix(float angle)
		{
			modelToWorld.rotateZ(angle);
		}
This method allows us to rotate along the z-axis, which gives the illusion the player actually rotates left and right, due to the game perspective being top-down. This allows us to create the following code:

if (is_key_down(key_left)) {
				sprites[ship_sprite].rotateMatrix(10);
			}
			else if (is_key_down(key_right)) {
				sprites[ship_sprite].rotateMatrix(-10);
			}

Which moves the player left and right. The value withing rotateMatrix() handles the speed at which the player turns in. In the move forward function, we set the movement speed to negative of the value in order to prevent the player from going through the walls.

if (is_key_down(key_up)) {
				sprites[ship_sprite].translate(0, +ship_speed);
				for (int i = 0; i < maze_sprites.size(); i++) {
					if (sprites[ship_sprite].collides_with(maze_sprites[i]))
					{
						sprites[ship_sprite].translate(0, -ship_speed);
					}
				}
			}

The CSV file integration allows us to quickly place sprites that are relevant to the game and alter its position/sprite image via values if necessary. In this case, it would handle the entire maze, and the portal sprite. There were problems integrating what I had originally planned due to issues splitting the character array, but thanks to Jean-Pascal Evette, a workaround whereby converting the CSV to a string worked.
std::string myStr = std::string(csv_buffer);
Dynamic arrays were created for this project in order to better handle calling each sprite for the relevant CSV file, with thanks to Raul. This allows us to re-use sprites within an array, whilst not worrying too much about overloading/underusing memory. This is done via pushing back the relevant sprite in the dynamic array, then utilizing the for-loop to render each sprite at its relevant position.
The shaders in the code were simple overlapping of colours, where the texture_shader handled rendering the sprite’s original colours, whilst my own individual ones (portal_Shader and my_Shader) handled the relevant sprites colour display utilizing the graphics card’s power on the graphics pipeline. Shaders consist of a vertex shader and fragment shader, vertex handling the ‘outline’ or vertex of an object/sprite (via triangles), and fragment shader handles the colours of the pixels within the vertex shader. Portal_Shader simple alters the portal’s colour to gold, whilst my_Shader altered the games background to a black and red look.
My_Shader was created with thanks to Mircea, where it takes into account the gradient of the screen, and slowly transitions between 2 colours. In order to do so, we need to first create the shader, and then feed in 2 parameters which take into account the width and height of the screen. This ensures that we have the correct or as-close-to value to work with gradient colouring integration. Afterwards, in our shader code, we have the main function handling  the colour change:
void main() 
{
				vec2 pos = gl_FragCoord.xy / resolution.xy;
				vec3 vColor = vec3(0.9, 0.1, 0.1) * (1 - pos.y);
				gl_FragColor = vec4(vColor, 1.0);
			}
What happens is it takes the position of the fragment colour co-ords along the x and y axis, divide by the current resolution’s x and y, and set the position.

The colour is then defined in vColor, (in this case more red) multiplied by the position of the pos.y value (so the gradient look, making the transition fade from bottom to top) and finally call the colour in the gl_FragColor method.
This is different in the portal_Shader, where:


void main()
			{
				vec4 textureCol = texture2D(sampler, uv_);
				vec4 color = vec4(1, 0.843f, 0, 1);
				gl_FragColor = textureCol * color;
}

This is simply altering the colour to more gold-like, where the RGB values (after being normalized) allows us to modify over the sprite or image we want to apply the texture to.

I would have liked more time to work on both the shader and the gameplay (Blinking player and portal, and enemies shoot upwards if the player was above the enemies, as well as collision between the player and enemy) however, due to my lack of experience in C++ and Octet, I felt I was wasting a lot more time trying to understand aspects of how the code and framework worked in conjunction with one another.

![Alt text](https://github.com/kt-chin/octet/blob/master/octet/src/examples/my_gameAssignment/Screenshot.png?raw=true"Screenshot of Game")
