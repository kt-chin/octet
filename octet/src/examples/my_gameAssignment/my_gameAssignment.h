namespace octet {
	class sprite {
		// where is our sprite (overkill for a 2D game!)


		// half the width of the sprite
		float halfWidth;

		// half the height of the sprite
		float halfHeight;

		
		// what texture is on our sprite
		int texture;

		// true if this sprite is enabled.
		bool enabled;

	public:
		mat4t modelToWorld;

		vec2 get_pos() {
			return modelToWorld.row(3).xy();
		}

		sprite() {
			texture = 0;
			enabled = true;
		}


		void rotateMatrix(float angle)
		{
			modelToWorld.rotateZ(angle);
		}

		void init(int _texture, float x, float y, float w, float h) {
			modelToWorld.loadIdentity();
			modelToWorld.translate(x, y, 0);
			halfWidth = w * 0.5f;
			halfHeight = h * 0.5f;
			texture = _texture;
			enabled = true;
		}

		void render(portal_Shader &shader, mat4t &cameraToWorld) {
			// invisible sprite... used for gameplay.
			if (!texture) return;

			// build a projection matrix: model -> world -> camera -> projection
			// the projection space is the cube -1 <= x/w, y/w, z/w <= 1
			mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);

			// set up opengl to draw textured triangles using sampler 0 (GL_TEXTURE0)
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);

			// use "old skool" rendering
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			shader.render(modelToProjection, 0);

			// this is an array of the positions of the corners of the sprite in 3D
			// a straight "float" here means this array is being generated here at runtime.
			float vertices[] = {
				-halfWidth, -halfHeight, 0,
				halfWidth, -halfHeight, 0,
				halfWidth,  halfHeight, 0,
				-halfWidth,  halfHeight, 0,
			};

			// attribute_pos (=0) is position of each corner
			// each corner has 3 floats (x, y, z)
			// there is no gap between the 3 floats and hence the stride is 3*sizeof(float)
			glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)vertices);
			glEnableVertexAttribArray(attribute_pos);

			// this is an array of the positions of the corners of the texture in 2D
			static const float uvs[] = {
				0,  0,
				1,  0,
				1,  1,
				0,  1,
			};

			// attribute_uv is position in the texture of each corner
			// each corner (vertex) has 2 floats (x, y)
			// there is no gap between the 2 floats and hence the stride is 2*sizeof(float)
			glVertexAttribPointer(attribute_uv, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)uvs);
			glEnableVertexAttribArray(attribute_uv);

			// finally, draw the sprite (4 vertices)
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}
		void render(texture_shader &shader, mat4t &cameraToWorld) {
			if (!texture) return;

			mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);

			shader.render(modelToProjection, 0);

			float vertices[] = {
				-halfWidth, -halfHeight, 0,
				halfWidth, -halfHeight, 0,
				halfWidth,  halfHeight, 0,
				-halfWidth,  halfHeight, 0,
			};

			glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)vertices);
			glEnableVertexAttribArray(attribute_pos);

			static const float uvs[] = {
				0,  0,
				1,  0,
				1,  1,
				0,  1,
			};

			glVertexAttribPointer(attribute_uv, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)uvs);
			glEnableVertexAttribArray(attribute_uv);

			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}

		void render(my_Shader &shader, mat4t &cameraToWorld, int v_width, int v_height) {
			mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);
			shader.render(modelToProjection, vec2(v_width, v_height));

			float vertices[] = {
				-halfWidth, -halfHeight, 0,
				halfWidth, -halfHeight, 0,
				halfWidth, halfHeight, 0,
				-halfWidth, halfHeight, 0,
			};

			glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)vertices);
			glEnableVertexAttribArray(attribute_pos);

			static const float uvs[] = {
				0, 0,
				1, 0,
				1, 1,
				0, 1,
			};

			glVertexAttribPointer(attribute_uv, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)uvs);
			glEnableVertexAttribArray(attribute_uv);

			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}
		void render(my_Shader &shader, mat4t &cameraToWorld) {

			if (!texture) return;

			mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);

			shader.render(modelToProjection, 0);

			float vertices[] = {
				-halfWidth, -halfHeight, 0,
				halfWidth, -halfHeight, 0,
				halfWidth,  halfHeight, 0,
				-halfWidth,  halfHeight, 0,
			};

			glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)vertices);
			glEnableVertexAttribArray(attribute_pos);

			static const float uvs[] = {
				0,  0,
				1,  0,
				1,  1,
				0,  1,
			};

			glVertexAttribPointer(attribute_uv, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)uvs);
			glEnableVertexAttribArray(attribute_uv);

			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}

		void translate(float x, float y) {
			modelToWorld.translate(x, y, 0);
		}

		void set_relative(sprite &rhs, float x, float y) {
			modelToWorld = rhs.modelToWorld;
			modelToWorld.translate(x, y, 0);
		}

		bool collides_with(const sprite &rhs) const {
			float dx = rhs.modelToWorld[3][0] - modelToWorld[3][0];
			float dy = rhs.modelToWorld[3][1] - modelToWorld[3][1];

			return
				(fabsf(dx) < halfWidth + rhs.halfWidth) &&
				(fabsf(dy) < halfHeight + rhs.halfHeight)
				;
		}

		bool is_above(const sprite &rhs, float margin) const {
			float dx = rhs.modelToWorld[3][0] - modelToWorld[3][0];

			return
				(fabsf(dx) < halfWidth + margin)
				;
		}

		bool &is_enabled() {
			return enabled;
		}
	};

	class my_gameAssignment : public octet::app {
		// Matrix to transform points in our camera space to the world.
		// This lets us move our camera
		mat4t cameraToWorld;

		// shader to draw a textured triangle
		texture_shader texture_shader_;
		my_Shader my_Shader_;
		portal_Shader portal_Shader_;
		enum {
			num_sound_sources = 8,
			num_rows = 1,
			num_cols = 9,
			num_missiles = 2,
			num_bombs = 2,
			num_borders = 4,
			num_invaderers = num_rows * num_cols,


			// sprite definitions
			ship_sprite = 0,
			game_over_sprite,

			portal_sprite,

			maze_sprite,

			first_invaderer_sprite,
			last_invaderer_sprite = first_invaderer_sprite + num_invaderers - 1,

			first_missile_sprite,
			last_missile_sprite = first_missile_sprite + num_missiles - 1,

			first_bomb_sprite,
			last_bomb_sprite = first_bomb_sprite + num_bombs - 1,

			first_border_sprite,
			last_border_sprite = first_border_sprite + num_borders - 1,

			num_sprites,

		};

		// timers for missiles and bombs
		int missiles_disabled;
		int bombs_disabled;

		// accounting for bad guys
		int live_invaderers;
		int num_lives;

		// game state
		bool game_over;
		int score;

		// speed of enemy
		float invader_velocity;

		// sounds
		ALuint whoosh;
		ALuint bang;
		unsigned cur_source;
		ALuint sources[num_sound_sources];

		// big array of sprites
		sprite sprites[num_sprites];

		sprite background_Sprite;	

		// random number generator
		class random randomizer;

		// a texture for our text
		GLuint font_texture;

		// information for our text
		bitmap_font font;

		ALuint get_sound_source() { return sources[cur_source++ % num_sound_sources]; }

		vec4 portal_Colour;

		dynarray<sprite> maze_sprites; // thanks to Raul
		dynarray<sprite> portal_sprites;
		// called when we are hit
		void on_hit_ship() {
			ALuint source = get_sound_source();
			alSourcei(source, AL_BUFFER, bang);
			alSourcePlay(source);

			if (--num_lives == 0) {
				game_over = true;
				sprites[game_over_sprite].translate(-20, 0);
			}
		}

		// use the keyboard to move the ship
		void move_ship() {
			const float ship_speed = 0.05f;
			// left and right arrows
			if (is_key_down(key_left)) {
				sprites[ship_sprite].rotateMatrix(10);
			}
			else if (is_key_down(key_right)) {
				sprites[ship_sprite].rotateMatrix(-10);
			}


			if (is_key_down(key_up)) {
				sprites[ship_sprite].translate(0, +ship_speed);
				for (int i = 0; i < maze_sprites.size(); i++) {
					if (sprites[ship_sprite].collides_with(maze_sprites[i]))
					{
						sprites[ship_sprite].translate(0, -ship_speed);						
					}
				}
			}


			else if (is_key_down(key_down)) {
				sprites[ship_sprite].translate(0, -ship_speed);
				for (int i = 0; i < maze_sprites.size(); i++) {
					if (sprites[ship_sprite].collides_with(maze_sprites[i]))
					{
						sprites[ship_sprite].translate(0, +ship_speed);
					}
				}
			}
		}
		


		// pick and invader and fire a bomb
		void fire_bombs() {
			//pBomb *move_bombs;
			if (bombs_disabled) {
				--bombs_disabled;
			}
			else {
				// find an invaderer
				sprite &ship = sprites[ship_sprite];
				for (int j = randomizer.get(0, num_invaderers); j < num_invaderers; ++j) {
					sprite &invaderer = sprites[first_invaderer_sprite + j];
					if (invaderer.is_enabled() && invaderer.is_above(ship, 0.3f)) {
						// find a bomb

						for (int i = 0; i != num_bombs; ++i) {
							if (!sprites[first_bomb_sprite + i].is_enabled()) {
								sprites[first_bomb_sprite + i].set_relative(invaderer, 0, -0.25f);
								sprites[first_bomb_sprite + i].is_enabled() = true;
								bombs_disabled = 30;
								ALuint source = get_sound_source();
								alSourcei(source, AL_BUFFER, whoosh);
								alSourcePlay(source);
								return;
							}
						}
						return;
					}
				}
			}
		}


		// animate the bombs
		void move_bombs() {
			const float speed = 0.2f;
			const float &bomb_speed = speed;
			for (int i = 0; i != num_bombs; ++i) {
				sprite &bomb = sprites[first_bomb_sprite + i];
				bomb.translate(0, -bomb_speed);
				if (bomb.collides_with(sprites[ship_sprite])) {

						bomb.is_enabled() = false;
						bomb.translate(20, 0);
						bombs_disabled = 50;
						on_hit_ship();
						portal_Colour = vec4(0, 1, 0,1 );
						goto next_bomb;
					
				}
				if (bomb.get_pos().y() < -3) {
					bomb.is_enabled() = false;
					bomb.translate(20, 0);
				}

			next_bomb:;
			}
		}

		// move the array of enemies
		void move_invaders(float dx, float dy) {
			for (int j = 0; j != num_invaderers; ++j) {
				sprite &invaderer = sprites[first_invaderer_sprite + j];
				if (invaderer.is_enabled()) {
					invaderer.translate(dx, dy);
				}
			}
		}

		void draw_text(texture_shader &shader, float x, float y, float scale, const char *text) {
			mat4t modelToWorld;
			modelToWorld.loadIdentity();
			modelToWorld.translate(x, y, 0);
			modelToWorld.scale(scale, scale, 1);
			mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);

			enum { max_quads = 32 };
			bitmap_font::vertex vertices[max_quads * 4];
			uint32_t indices[max_quads * 6];
			aabb bb(vec3(0, 0, 0), vec3(256, 256, 0));

			unsigned num_quads = font.build_mesh(bb, vertices, indices, max_quads, text, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, font_texture);

			shader.render(modelToProjection, 0);

			glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, sizeof(bitmap_font::vertex), (void*)&vertices[0].x);
			glEnableVertexAttribArray(attribute_pos);
			glVertexAttribPointer(attribute_uv, 3, GL_FLOAT, GL_FALSE, sizeof(bitmap_font::vertex), (void*)&vertices[0].u);
			glEnableVertexAttribArray(attribute_uv);

			glDrawElements(GL_TRIANGLES, num_quads * 6, GL_UNSIGNED_INT, indices);
		}

	
	public:
		


		// this is called when we construct the class
		my_gameAssignment(int argc, char **argv) : app(argc, argv), font(512, 256, "assets/big.fnt") {
		}

		// this is called once OpenGL is initialized
		void app_init() {
			// set up the shader
			texture_shader_.init();
			my_Shader_.init();
			portal_Shader_.init();
			// set up the matrices with a camera 5 units from the origin
			cameraToWorld.loadIdentity();
			cameraToWorld.translate(0, 0, 3);

			font_texture = resource_dict::get_texture_handle(GL_RGBA, "assets/big_0.gif");

			GLuint ship = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/theShip.gif");
			sprites[ship_sprite].init(ship, 2.755f, -2.70f, 0.25f, 0.25);
			sprites[ship_sprite].rotateMatrix(90);

			GLuint GameOver = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/GameOver.gif");
			sprites[game_over_sprite].init(GameOver, 20, 0, 3, 1.5f);

			GLuint invaderer = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/invaderer.gif");
			for (int j = 0; j != num_rows; ++j) {
				for (int i = 0; i != num_cols; ++i) {
					assert(first_invaderer_sprite + i + j*num_cols <= last_invaderer_sprite);
					sprites[first_invaderer_sprite + i + j*num_cols].init(
						invaderer, ((float)i - num_cols * 0.5f) * 0.65f, 2.50f - ((float)j * 0.5f), 0.25f, 0.25f
						);
					sprites[first_invaderer_sprite + i + j*num_cols].translate(0.28, 0);
				}

			}


			//read CSV file
			std::vector<std::string> borders;
			//std::vector<int> score;
			std::ifstream is("../../../assets/invaderers/Maze2.0.csv");

			// store the line here
			char buffer[2048];
			int currentLine = 0;
			// loop over lines
			while (!is.eof()) {

				//then for each line you store it inside buffer
				is.getline(buffer, sizeof(buffer));

				// loop over columns
				//Then you store the first character of the line inside position of buffer
				char *csv_buffer = buffer;

				//This is going to store the current position of csv_buffer in e, then increase e until you reach the comma
				for (int col = 0; ; ++col) {
					char *e = csv_buffer;
					while (*e != 0 && *e != ',') ++e;


					// now b -> e contains the chars in a column
					
						borders.emplace_back(csv_buffer, e);
						std::string myStr = std::string(csv_buffer); //Convert to string, thanks to Jean-Pascal
						myStr = myStr.substr(0, myStr.find(','));

						if (myStr == "0")
						{
							GLuint maze = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/mazewall.gif");
							sprite mazeSprite;
							mazeSprite.init(maze,col*0.245f, -currentLine * 0.245f, 0.245f, 0.245f);
							mazeSprite.translate(-2.88f, 2.9f);
							maze_sprites.push_back(mazeSprite);
						}
						if (myStr == "1")
						{
							GLuint portal = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/portal.gif");
							sprite portalSprite;
							portalSprite.init(portal, col*0.2f, currentLine * 0.2f, 0.2f, 0.2f);
							portalSprite.translate(-2.8f, 1.5f);
							portal_sprites.push_back(portalSprite);

						}

					if (*e != ',') break;
					csv_buffer = e + 1;

				
				}
				currentLine++;
			}

			// set the border to white for clarity
			GLuint white = resource_dict::get_texture_handle(GL_RGB, "#ffffff");
			GLuint red = resource_dict::get_texture_handle(GL_RGB, "#ff0000");
			GLuint maze = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/mazewall.gif");
			background_Sprite.init(red, 0, 0, 6, 6);

			// use the bomb texture
			GLuint bomb = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/bomb.gif");
			for (int i = 0; i != num_bombs; ++i) {
				// create bombs off-screen
				sprites[first_bomb_sprite + i].init(bomb, 20, 0, 0.0625f, 0.25f);
				sprites[first_bomb_sprite + i].is_enabled() = false;
			}

			// sounds
			whoosh = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "assets/invaderers/pop.wav");
			bang = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "assets/invaderers/bam.wav");
			cur_source = 0;
			alGenSources(num_sound_sources, sources);

			// sundry counters and game state.
			missiles_disabled = 0;
			bombs_disabled = 50;
			invader_velocity = 0.055f;
			live_invaderers = num_invaderers;
			num_lives = 5;
			game_over = false;
			score = 0;
		}
	
		// called every frame to move things
		void simulate() {
			if (game_over) {
				return;
			}

			sprite &ship = sprites[ship_sprite];
			sprite &portal = sprites[portal_sprite];
			sprite &border = sprites[first_border_sprite + (invader_velocity < 0 ? 2 : 3)];
			sprite &invaderer = sprites[first_invaderer_sprite];
			sprite &maze = sprites[maze_sprite];

			for (int i = 0; i < portal_sprites.size(); i++)
			{
				if (ship.collides_with(portal_sprites[i]))
				{
					game_over = true;
					sprites[game_over_sprite].translate(-20, 0);
				}
			}

			move_ship();

			fire_bombs();

			move_bombs();

			move_invaders(0, -invader_velocity);

		
			if (invaderer.get_pos().y() < -3)
			{
				for (int j = 0; j != num_rows; ++j) {
					for (int i = 0; i != num_cols; ++i) {
						sprite &invaderer = sprites[first_invaderer_sprite + i];
						invaderer.translate(0,5.7f);
							
					}
				}
			}
}

		// this is called to draw the world
		void draw_world(int x, int y, int w, int h) {
			simulate();

			// set a viewport - includes whole window area
			glViewport(x, y, w, h);

			// clear the background to black
			glClearColor(0, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// don't allow Z buffer depth testing (closer objects are always drawn in front of far ones)
			glDisable(GL_DEPTH_TEST);

			// allow alpha blend (transparency when alpha channel is 0)
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//Background
			background_Sprite.render(my_Shader_, cameraToWorld, w, h);

			// draw all the sprites
			for (int i = 0; i != num_sprites; ++i) {
				sprites[i].render(texture_shader_, cameraToWorld);
			}

			for (int i = 0; i < maze_sprites.size(); i++)
			{
				maze_sprites[i].render(texture_shader_, cameraToWorld);
			}
			for (int i = 0; i < portal_sprites.size(); i++)
			{
				portal_sprites[i].render(portal_Shader_, cameraToWorld);
			}

			char score_text[32];
			sprintf(score_text, "score: %d   lives: %d\n", score, num_lives);
			draw_text(texture_shader_, -1.75f, 2, 1.0f / 256, score_text);

			// move the listener with the camera
			vec4 &cpos = cameraToWorld.w();
			alListener3f(AL_POSITION, cpos.x(), cpos.y(), cpos.z());
		}
	};
}