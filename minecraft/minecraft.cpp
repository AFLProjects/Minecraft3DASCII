#pragma once

#include <Windows.h>
#include <iostream>   
#include <cassert> 
#include <time.h>
#include "profile.h"
#include "vec.h"
#include "mat.h"
#include "math.h"
using namespace std;
using namespace vec3;
using namespace vec2;
using namespace mat4x4;

void __cxa_allocate_exception() { abort(); }
void __cxa_throw() { abort(); }

/* Enables the process to read data from the buffer */
#define READ 0x80000000L 
/* Enables the process to write data to the buffer */
#define WRITE 0x40000000L 
/* Enables console buffer mode */
#define BUFFER_MODE 0x1 

 /* Console Buffer Size */
const uint8_t width = 192, height = 108;
/* Console Font Size */
const uint8_t font_size = 8;

/*
* Creates console, sets up the buffer and the dimensions
* Enables acces rights and inputs for the console
*/
HANDLE setup_console() {
	/* Initialize console screen buffer, enabling reading, writing and buffer mode */
	HANDLE hConsoleHandle = CreateConsoleScreenBuffer(READ | WRITE, 0, NULL, BUFFER_MODE, NULL);

	/* Init console window info using minimal size (1,1) */
	SMALL_RECT lpConsoleWindow = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(hConsoleHandle, TRUE, &lpConsoleWindow);

	/* Set console screen buffer size */
	COORD dwSize = { (short)width, (short)height };
	SetConsoleScreenBufferSize(hConsoleHandle, dwSize);

	/* Activate Screen Buffer */
	SetConsoleActiveScreenBuffer(hConsoleHandle);

	/* Set Console Font settings */
	CONSOLE_FONT_INFOEX lpCurrentConsoleFontEx;
	lpCurrentConsoleFontEx.cbSize = sizeof(lpCurrentConsoleFontEx);
	lpCurrentConsoleFontEx.nFont = 0;
	lpCurrentConsoleFontEx.dwFontSize.X = font_size;
	lpCurrentConsoleFontEx.dwFontSize.Y = font_size;
	lpCurrentConsoleFontEx.FontFamily = FF_DONTCARE;
	lpCurrentConsoleFontEx.FontWeight = FW_NORMAL;
	SetCurrentConsoleFontEx(hConsoleHandle, false, &lpCurrentConsoleFontEx);

	/* Set Physical Console Window Size */
	lpConsoleWindow = { 0, 0, (short)width - 1, (short)height - 1 };
	SetConsoleWindowInfo(hConsoleHandle, TRUE, &lpConsoleWindow);

	/* Enable console mouse and keyboard input */
	SetConsoleMode(hConsoleHandle, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	return hConsoleHandle;
}

/* Char buffer, for rendering */
wchar_t* buffer = new wchar_t[width * height];
/* Clear buffer with blank chars */
#define clear_buffer() for (int i = 0; i < width * height; ++i) { buffer[i] = 0x20; } 
/* Draw buffer on the console */
#define draw_buffer(handle, bytes) WriteConsoleOutputCharacter(handle, buffer, width * height, { 0, 0 }, &bytes);

/* Noise generation functions used to create map */
#define smooth(t) (t*t*t*(t*(t*6.0f-15.0f)+10.0f))
#define ffloor(x) (((x)>=0) ? ((int)x) : ((int)x- 1))
#define lerp(t, a, b) ((a)+(t)*((b)-(a)))
#define xorshift32(x) (x^(x<<13)^((x^(x<<13))>>17))^((x^(x<<13)^((x^(x<<13))>>17))<<5);

/*
*  Generate uint32_t n^2 random grid, O(n^2)
*  Used later for interpolation on a 100n^2 map
*/
inline int* generate_grid(uint32_t& seed, const int size) {
	int* grid = new int[size * size];
	for (int i = 0; i < ceil((size * size) / 10.0f); i++) {
		/* Iterate xor shift */
		seed = xorshift32(seed);
		/* Make sure the value is 10 digits */
		if (seed <= 1e9)
			seed += (uint32_t)1147483647;
		/* Extract the 10 digits */
		uint32_t n = seed, rem;
		for (int j = 0; j < 10; ++j) {
			rem = n % 10;
			n = n / 10;
			grid[(10 * i + j) % (size * size)] = rem;
		}
	}
	return grid;
}

/*
*  Interpolate n^2 random grid into a 100n^2 map
*  using blinear interpolation and smooth functions
*/
inline int* interpolate_grid(const int* grid, const int size) {
	/* Interpolated Map */
	int* map = new int[100 * size * size];
	for (int y = 0; y < size * 10; y++) {
		for (int x = 0; x < size * 10; x++) {
			/* Find corners */
			float px = (float)x / 10.0f;
			float py = (float)y / 10.0f;
			int fx = ffloor(px);
			int fy = ffloor(py);

			/* Interpolate on the x axis */
			float l1 = lerp(px - fx, (float)grid[(size * fy + fx) % (size * size)],
				(float)grid[(size * fy + fx + 1) % (size * size)]);
			float l2 = lerp(px - fx, (float)grid[(size * (fy + 1) + fx) % (size * size)],
				(float)grid[(size * (fy + 1) + fx + 1) % (size * size)]);

			/* Interpolate on the y axis, using smooth function */
			float t = py - fy < 1.0f ? py - fy : 1.0;
			map[y * size * 10 + x] = (int)ffloor(lerp(smooth(t), l1, l2));
		}
	}
	return map;
}

/* Cube vertices */
const float vertices[8][4] = {{0.5, -0.5, -0.5, 1.0}, {0.5, -0.5, 0.5, 1.0}, {-0.5, -0.5, 0.5, 1.0}, {-0.5, -0.5, -0.5, 1.0}, {0.5, 0.5, -0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, {-0.5, 0.5, 0.5, 1.0}, {-0.5, 0.5, -0.5, 1.0}};
/* Normales of all the faces of the cube */
const float normals[12][4] = {{0.0, -1.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 1.0},{-1.0, 0.0, 0.0, 1.0}, {0.0, 0.0, -1.0, 1.0}, {0.0, -1.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0},{1.0, 0.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 1.0}, {-1.0, 0.0, 0.0, 1.0}, {0.0, 0.0, -1.0, 1.0}};
/* Faces created from point indexes */
const int face_vertices[12][3] = {{1, 2, 3}, {7, 6, 5}, {4, 5, 1}, {5, 6, 2},{2, 6, 7}, {0, 3, 7}, {0, 1, 3}, {4, 7, 5},{0, 4, 1}, {1, 5, 2}, {3, 2, 7}, {4, 0, 7}};

/* 
* Temporary line algorithm
*/
void Line(float x1, float y1, float x2, float y2)
{
	const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));

	if (steep) {
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if (x1 > x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	const float dx = x2 - x1;
	const float dy = fabs(y2 - y1);
	float error = dx / 2.0f;

	const int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;
	const int maxX = (int)x2;

	for (int x = (int)x1; x <= maxX; x++) {

		if (steep) {
			buffer[x * 192 + y] = '.';
		}
		else {
			buffer[y * 192 + x] = '.';
		}

		error -= dy;
		if (error < 0) {
			y += ystep;
			error += dx;
		}
	}
}

/* Rendering settings */
const float render_distance = 20;
const float zNear = 0.1f;
const float zFar = 1000.0f;
const float fov = 70.0f;

/* Game settings */
const int map_size = 1000;
const int map_depth = 1;

int main() {
	srand(time(NULL));

	uint32_t seed = rand();
	int* grid = generate_grid(seed, ffloor(map_size / 10.0));
	int* map = interpolate_grid(grid, ffloor(map_size / 10.0));

	/* Create Console */
	HANDLE hConsoleHandle = setup_console();
	DWORD bytesWritten = 0;

	/* Camera */
	float camera_pos[4], camera_rot[4];
	vec3::init(50, 12, 50, camera_pos);
	vec3::init(0, 0, 0, camera_rot);

	/* Creation Projection Matrix */
	float projection[16];
	mat4x4::projection_matrix(fov, (float)(height) / (float)(width), zNear, zFar, projection);

	/* Update Game */
	while (1) {
		/* Clear frame, and profiling */
		PROF_COUNTER cnt0("frame-*");
		clear_buffer();

		camera_pos[2] += 0.3;

		/* Calculate Camera rotation matrices */
		float camera_rotation[16], camera_rx[16], camera_ry[16], camera_view[16];
		mat4x4::identity_matrix(camera_rotation);
		mat4x4::rotation_x(camera_rot[0], camera_rx);
		mat4x4::rotation_y(camera_rot[1], camera_ry);
		mat4x4::mult_mat(camera_rx, camera_ry, camera_rotation);
		mat4x4::quick_inverse(camera_rotation, camera_view);

		/* Init triangles to render */
		vector<vec3::Triangle> rendered_triangles;

		for (int y = 0; y < map_size; y++) {
			for (int x = 0; x < map_size; x++) {
				if (abs(camera_pos[2] - y) <= render_distance && abs(camera_pos[0] - x) <= render_distance) {
					/* Initialize cube position */
					float cube_pos[4];
					vec3::init(x, map[y * map_size + x], y, cube_pos);

					/* Create center at cam vector */
					float center_at_cam[4];
					center_at_cam[0] = cube_pos[0] - camera_pos[0];
					center_at_cam[1] = cube_pos[1] - camera_pos[1];
					center_at_cam[2] = cube_pos[2] - camera_pos[2];
					center_at_cam[3] = 1.0f;

					/* For each face */
					for (int i = 0; i < 12; i++) {
						/* Calculate direction vectors*/
						float point[4], normal[4];
						vec3::cpy(vertices[face_vertices[i][0]], point);
						vec3::inv_translate(point, camera_pos, point);
						vec3::cpy(normals[i], normal);

						/* Determine if the object is facing the camera */
						if (vec3::dot(point, normal) < 0.0) {
							/* Triangle to render */
							vec3::Triangle triangle;
							bool render = true;

							/* For each point */
							for (int j = 0; j < 3; j++) {
								int index = face_vertices[i][j];

								/* Project vertex */
								float vertex_position[4];
								float vertex_rotation[4];
								float vertex_projection[4];
								vec3::cpy(vertices[index], vertex_position);
								vec3::translate(vertex_position, center_at_cam, vertex_position);
								mat4x4::mult_vec(camera_view, vertex_position, vertex_rotation);
								mat4x4::mult_vec(projection, vertex_rotation, vertex_projection);

								/* Denormalize coordinates */
								float screenX = vertex_projection[0] / vertex_projection[3];
								float screenY = vertex_projection[1] / vertex_projection[3];
								screenX = round((screenX + 1.0) * 192.0 / 2.0);
								screenY = 108.0 - round((screenY + 1.0) * 108.0 / 2.0);
								if (screenX < 0 || screenY < 0 || screenX > 192 || screenY > 108 || vertex_projection[3] < 0) {
									render = false;
									break;
								}

								/* Invert depth, for rasterizing */
								vertex_projection[3] = 1.0 / vertex_projection[3];

								/* Save triangle data */
								vector<float> screen_pos = { screenX, screenY };
								triangle.points.push_back(screen_pos);
								triangle.w[j] = vertex_projection[3];
							}
							if (render)
								rendered_triangles.push_back(triangle);
						}
					}
				}
			}
		}

		for (int i = 0; i < rendered_triangles.size(); i++) {
			for (int j = 0; j < 3; j++)
			{
				int x1 = rendered_triangles[i].points[j][0];
				int y1 = rendered_triangles[i].points[j][1];
				int x2 = rendered_triangles[i].points[(j+1)%3][0];
				int y2 = rendered_triangles[i].points[(j+1)%3][1];
				Line(x1, y1, x2, y2);
			}
		}

		draw_buffer(hConsoleHandle, bytesWritten);
		SetConsoleTitleA(cnt0.fps().c_str());
	}
}