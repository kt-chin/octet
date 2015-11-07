namespace octet {
	namespace shaders {
		class intro_Shader : public shader {
			GLuint modelToProjectionIndex_;
			GLuint resolutionIndex_;

		public:
			void init() {

				const char vertex_shader[] = SHADER_STR(

					attribute vec4 pos;
				uniform mat4 modelToProjection;

				void main() {
					gl_Position = modelToProjection * pos;
				}
				);

				const char fragment_shader[] = SHADER_STR(

					uniform vec2 resolution;

				void main() {
					vec2 pos = gl_FragCoord.xy / resolution.xy;
					vec3 vColor = vec3(0.209, 0.66, 0.38) * (1 - pos.y);
					gl_FragColor = vec4(vColor, 1.0);
				}
				);

				shader::init(vertex_shader, fragment_shader);

				modelToProjectionIndex_ = glGetUniformLocation(program(), "modelToProjection");
				resolutionIndex_ = glGetUniformLocation(program(), "resolution");
			}

			void render(const mat4t &modelToProjection, const vec2 &resolution) {
				shader::render();
				glUniform2fv(resolutionIndex_, 1, resolution.get());
				glUniformMatrix4fv(modelToProjectionIndex_, 1, GL_FALSE, modelToProjection.get());
			}
		};
	}
}