#pragma once

#include <stdio.h>    
#include <string.h>
#include <math.h>  

#define M_PI           3.14159265358979323846
namespace mat4x4 {

	inline void zero_matrix(float out[16]) {
		for (int i = 0; i < 16; i++) { out[i] = 0.0f; }
	}

	inline void identity_matrix(float out[16]) {
		zero_matrix(out);
		out[0] = 1.0f;
		out[5] = 1.0f;
		out[10] = 1.0f;
		out[15] = 1.0f;
	}

	inline void mult_vec(const float mat[16], const float v0[4], float out[4]) {
		out[0] = v0[0] * mat[0] + v0[1] * mat[4] + v0[2] * mat[8] + v0[3] * mat[12];
		out[1] = v0[0] * mat[1] + v0[1] * mat[5] + v0[2] * mat[9] + v0[3] * mat[13];
		out[2] = v0[0] * mat[2] + v0[1] * mat[6] + v0[2] * mat[10] + v0[3] * mat[14];
		out[3] = v0[0] * mat[3] + v0[1] * mat[7] + v0[2] * mat[11] + v0[3] * mat[15];
	}

	inline void mult_mat(const float mat0[16], const float mat1[16], float out[16]) {
		zero_matrix(out);
		for (int c = 0; c < 4; c++)
			for (int r = 0; r < 4; r++)
				out[4*r+c] = mat0[4*r] * mat1[c] + mat0[4*r+1] * mat1[4+c] + mat0[4*r+2] * mat1[8+c] + mat0[4*r+3] * mat1[12+c];
	}

	inline void rotation_x(float x, float out[16]) {
		double sin_x = sin(x / 180.0f * M_PI);
		double cos_x = cos(x / 180.0f * M_PI);
		zero_matrix(out);
		out[0] = 1.0f;
		out[5] = cos_x;
		out[6] = sin_x;
		out[9] = -sin_x;
		out[10] = cos_x;
		out[15] = 1.0f;
	}

	inline void rotation_y(float y, float out[16]) {
		double sin_y = sin(y / 180.0f * M_PI);
		double cos_y = cos(y / 180.0f * M_PI);
		zero_matrix(out);
		out[0] = cos_y;
		out[2] = sin_y;
		out[5] = 1.0f;
		out[8] = -sin_y;
		out[10] = cos_y;
		out[15] = 1.0f;
	}

	inline void translation_matrix(float x, float y, float z, float out[16]) {
		identity_matrix(out);
		out[12] = x;
		out[13] = y;
		out[14] = z;
	}

	inline void projection_matrix(float fFov, float fAspectRatio, float fNear, float fFar, float out[16]) {
		float fFovRad = 1.0f / tanf((fFov * 0.5f) / 180.0f * M_PI);
		zero_matrix(out);
		out[0] = fAspectRatio * fFovRad;
		out[5] = fFovRad;
		out[10] = fFar / (fFar - fNear);
		out[14] = (-fFar * fNear) / (fFar - fNear);
		out[11] = 1.0f;
		out[15] = 0.0f;
	}

	inline void quick_inverse(const float m0[16], float out[16]) {
		out[0] = m0[0]; out[1] = m0[4]; out[2] = m0[8]; out[3] = 0.0f;
		out[4] = m0[1]; out[5] = m0[5]; out[6] = m0[9]; out[7] = 0.0f;
		out[8] = m0[2]; out[9] = m0[6]; out[10] = m0[10]; out[11] = 0.0f;
		out[12] = -(m0[12] * out[0] + m0[13] * out[4] + m0[14] * out[8]);
		out[13] = -(m0[12] * out[1] + m0[13] * out[5] + m0[14] * out[9]);
		out[14] = -(m0[12] * out[2] + m0[13] * out[6] + m0[14] * out[10]);
		out[15] = 1.0f;
	}

	std::string tostring(std::string name, const float mat[16]) {
		std::string str = name + " { \n";
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				str += to_string(mat[4 * i + j]);
				if(4*i+j != 15)
					str += ", ";
			}
			str += "\n";
		}
		str += " }\n";
		return str;
	}
}