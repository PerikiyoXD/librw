#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cmath>
#include <cctype>

#include "rwbase.h"
#include "rwerror.h"
#include "rwplg.h"
#include "rwpipeline.h"
#include "rwobjects.h"
#include "rwengine.h"

namespace rw {

#define PLUGIN_ID 0

int32 version = 0x36003;
int32 build = 0xFFFF;
#ifdef RW_PS2
	int32 platform = PLATFORM_PS2;
#elif RW_WDGL
	int32 platform = PLATFORM_WDGL;
#elif RW_GL3
	int32 platform = PLATFORM_GL3;
#elif RW_D3D9
	int32 platform = PLATFORM_D3D9;
#else
	int32 platform = PLATFORM_NULL;
#endif
bool32 streamAppendFrames = 0;
char *debugFile = nil;

// TODO: comparison tolerances

static Matrix identMat = {
	V3d( 1.0f, 0.0f, 0.0f), 0.0f,
	V3d( 0.0f, 1.0f, 0.0f), 0.0f,
	V3d( 0.0f, 0.0f, 1.0f), 0.0f,
	V3d( 0.0f, 0.0f, 0.0f), 1.0f
};

static Matrix3 identMat3 = {
	V3d( 1.0f, 0.0f, 0.0f),
	V3d( 0.0f, 1.0f, 0.0f),
	V3d( 0.0f, 0.0f, 1.0f)
};

// lazy implementation
int
strncmp_ci(const char *s1, const char *s2, int n)
{
	char c1, c2;
	while(n--){
		c1 = tolower(*s1);
		c2 = tolower(*s2);
		if(c1 != c2)
			return c1 - c2;
		if(c1 == '\0')
			return 0;
		s1++;
		s2++;
	}
	return 0;
}

Quat
mult(const Quat &q, const Quat &p)
{
	return Quat(q.w*p.w - q.x*p.x - q.y*p.y - q.z*p.z,
	            q.w*p.x + q.x*p.w + q.y*p.z - q.z*p.y,
	            q.w*p.y + q.y*p.w + q.z*p.x - q.x*p.z,
	            q.w*p.z + q.z*p.w + q.x*p.y - q.y*p.x);
}

Quat
lerp(const Quat &q, const Quat &p, float32 r)
{
	float32 c;
	Quat q1 = q;
	c = dot(q1, p);
	if(c < 0.0f){
		c = -c;
		q1 = negate(q1);
	}
	return Quat(q1.w + r*(p.w - q1.w),
	            q1.x + r*(p.x - q1.x),
	            q1.y + r*(p.y - q1.y),
	            q1.z + r*(p.z - q1.z));
};

Quat
slerp(const Quat &q, const Quat &p, float32 a)
{
	float32 c;
	Quat q1 = q;
	c = dot(q1, p);
	if(c < 0.0f){
		c = -c;
		q1 = negate(q1);
	}
	float32 phi = acos(c);
	if(phi > 0.00001f){
		float32 s = sin(phi);
		return add(scale(q1, sin((1.0f-a)*phi)/s),
		           scale(p,  sin(a*phi)/s));
	}
	return q1;
}

V3d
cross(const V3d &a, const V3d &b)
{
	return V3d(a.y*b.z - a.z*b.y,
	           a.z*b.x - a.x*b.z,
	           a.x*b.y - a.y*b.x);
}

Matrix*
Matrix::create(void)
{
	Matrix *m = (Matrix*)malloc(sizeof(Matrix));
	m->setIdentity();
	return m;
}

void
Matrix::destroy(void)
{
	free(this);
}

/* q must be normalized */
Matrix
Matrix::makeRotation(const Quat &q)
{
	Matrix res;
	float xx = q.x*q.x;
	float yy = q.y*q.y;
	float zz = q.z*q.z;
	float yz = q.y*q.z;
	float zx = q.z*q.x;
	float xy = q.x*q.y;
	float wx = q.w*q.x;
	float wy = q.w*q.y;
	float wz = q.w*q.z;

	res.right.x = 1.0f - 2.0f*(yy + zz);
	res.right.y =        2.0f*(xy + wz);
	res.right.z =        2.0f*(zx - wy);

	res.up.x =        2.0f*(xy - wz);
	res.up.y = 1.0f - 2.0f*(xx + zz);
	res.up.z =        2.0f*(yz + wx);

	res.at.x =        2.0f*(zx + wy);
	res.at.y =        2.0f*(yz - wx);
	res.at.z = 1.0f - 2.0f*(xx + yy);

	res.pos.x = res.pos.y = res.pos.z = 0.0f;

	res.rightw = res.upw = res.atw = 0.0f;
	res.posw = 1.0f;
	return res;
}

void
Matrix::setIdentity(void)
{
	*this = identMat;
}

void
Matrix::pointInDirection(const V3d &d, const V3d &up)
{
	// this->right is really pointing left
	this->at = normalize(d);
	this->right = normalize(cross(up, this->at));
	this->up = cross(this->at, this->right);
}

V3d
Matrix::transPoint(const V3d &p)
{
	V3d res = this->pos;
	res = add(res, rw::scale(this->right, p.x));
	res = add(res, rw::scale(this->up, p.y));
	res = add(res, rw::scale(this->at, p.z));
	return res;
}

V3d
Matrix::transVec(const V3d &v)
{
	V3d res;
	res = rw::scale(this->right, v.x);
	res = add(res, rw::scale(this->up, v.y));
	res = add(res, rw::scale(this->at, v.z));
	return res;
}

bool32
Matrix::isIdentity(void)
{
	return matrixIsIdentity((float32*)this);
}

void
Matrix::mult(Matrix *m1, Matrix *m2, Matrix *m3)
{
	matrixMult((float32*)m1, (float32*)m2, (float32*)m3);
}

bool32
Matrix::invert(Matrix *m1, Matrix *m2)
{
	return matrixInvert((float32*)m1, (float32*)m2);
}

void
Matrix::invertOrthonormal(Matrix *m1, Matrix *m2)
{
	m1->right.x = m2->right.x;
	m1->right.y = m2->up.x;
	m1->right.z = m2->at.x;
	m1->up.x = m2->right.y;
	m1->up.y = m2->up.y;
	m1->up.z = m2->at.y;
	m1->at.x = m2->right.z;
	m1->at.y = m2->up.z;
	m1->at.z = m2->at.z;
	m1->pos.x = -(m2->pos.x*m2->right.x +
	              m2->pos.y*m2->right.y +
	              m2->pos.z*m2->right.z);
	m1->pos.y = -(m2->pos.x*m2->up.x +
	              m2->pos.y*m2->up.y +
	              m2->pos.z*m2->up.z);
	m1->pos.z = -(m2->pos.x*m2->at.x +
	              m2->pos.y*m2->at.y +
	              m2->pos.z*m2->at.z);
	m1->rightw = 0.0f;
	m1->upw = 0.0f;
	m1->atw = 0.0f;
	m1->posw = 1.0f;
}

void
Matrix::transpose(Matrix *m1, Matrix *m2)
{
	matrixTranspose((float32*)m1, (float32*)m2);
}

void
Matrix::rotate(V3d *axis, float32 angle, CombineOp op)
{
	Matrix tmp;
	V3d v = normalize(*axis);
	angle = angle*M_PI/180.0f;
	float32 s = sin(angle);
	float32 c = cos(angle);
	float32 t = 1.0f - cos(angle);

	Matrix rot = identMat;
	rot.right.x = c + v.x*v.x*t;
	rot.right.y = v.x*v.y*t + v.z*s;
	rot.right.z = v.z*v.x*t - v.y*s;
	rot.up.x = v.x*v.y*t - v.z*s;
	rot.up.y = c + v.y*v.y*t;
	rot.up.z = v.y*v.z*t + v.x*s;
	rot.at.x = v.z*v.x*t + v.y*s;
	rot.at.y = v.y*v.z*t - v.x*s;
	rot.at.z = c + v.z*v.z*t;

	switch(op){
	case COMBINEREPLACE:
		*this = rot;
		break;
	case COMBINEPRECONCAT:
		mult(&tmp, this, &rot);
		*this = tmp;
		break;
	case COMBINEPOSTCONCAT:
		mult(&tmp, &rot, this);
		*this = tmp;
		break;
	}
}

void
Matrix::translate(V3d *translation, CombineOp op)
{
	Matrix tmp;
	Matrix trans = identMat;
	trans.pos = *translation;
	switch(op){
	case COMBINEREPLACE:
		*this = trans;
		break;
	case COMBINEPRECONCAT:
		mult(&tmp, this, &trans);
		*this = tmp;
		break;
	case COMBINEPOSTCONCAT:
		mult(&tmp, &trans, this);
		*this = tmp;
		break;
	}
}

void
Matrix::scale(V3d *scale, CombineOp op)
{
	Matrix tmp;
	Matrix scl = identMat;
	scl.right.x = scale->x;
	scl.right.y = scale->y;
	scl.right.z = scale->z;
	switch(op){
	case COMBINEREPLACE:
		*this = scl;
		break;
	case COMBINEPRECONCAT:
		mult(&tmp, this, &scl);
		*this = tmp;
		break;
	case COMBINEPOSTCONCAT:
		mult(&tmp, &scl, this);
		*this = tmp;
		break;
	}
}



Matrix3
Matrix3::makeRotation(const Quat &q)
{
	Matrix3 res;
	res.right.x = q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z;
	res.right.y = 2*q.w*q.z + 2*q.x*q.y;
	res.right.z = 2*q.x*q.z - 2*q.w*q.y;
	res.up.x    = 2*q.x*q.y - 2*q.w*q.z;
	res.up.y    = q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z;
	res.up.z    = 2*q.w*q.x + 2*q.y*q.z;
	res.at.x    = 2*q.w*q.y + 2*q.x*q.z;
	res.at.y    = 2*q.y*q.z - 2*q.w*q.x;
	res.at.z    = q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z;
	return res;
}

void
Matrix3::setIdentity(void)
{
	*this = identMat3;
}

V3d
Matrix3::transVec(const V3d &v)
{
	V3d res;
	res = scale(this->right, v.x);
	res = add(res, scale(this->up, v.y));
	res = add(res, scale(this->at, v.z));
	return res;
}

bool32
Matrix3::isIdentity(void)
{
	return right.x == 1.0f && right.y == 0.0f && right.z == 0.0f &&
	       up.x == 0.0f    && up.y == 1.0f    && up.z == 0.0f &&
	       at.x == 0.0f    && at.y == 0.0f    && at.z == 1.0f;
}

float32
Matrix3::determinant(void)
{
	return right.x*(up.y*at.z    - up.z*at.y)
	       +  up.x*(at.y*right.z - at.z*right.y)
	       +  at.x*(right.y*up.z - right.z*up.y);
}

void
mult(Matrix3 *m1, Matrix3 *m2, Matrix3 *m3)
{
	m1->right.x = m2->right.x*m3->right.x + m2->up.x*m3->right.y + m2->at.x*m3->right.z;
	m1->right.y = m2->right.x*m3->up.x    + m2->up.x*m3->up.y    + m2->at.x*m3->up.z;
	m1->right.z = m2->right.x*m3->at.x    + m2->up.x*m3->at.y    + m2->at.x*m3->at.z;
	m1->up.x    = m2->right.y*m3->right.x + m2->up.y*m3->right.y + m2->at.y*m3->right.z;
	m1->up.y    = m2->right.y*m3->up.x    + m2->up.y*m3->up.y    + m2->at.y*m3->up.z;
	m1->up.z    = m2->right.y*m3->at.x    + m2->up.y*m3->at.y    + m2->at.y*m3->at.z;
	m1->at.x    = m2->right.z*m3->right.x + m2->up.z*m3->right.y + m2->at.z*m3->right.z;
	m1->at.y    = m2->right.z*m3->up.x    + m2->up.z*m3->up.y    + m2->at.z*m3->up.z;
	m1->at.z    = m2->right.z*m3->at.x    + m2->up.z*m3->at.y    + m2->at.z*m3->at.z;
}

bool32
invert(Matrix3 *m1, Matrix3 *m2)
{
	float32 invdet = m2->determinant();
	if(invdet == 0.0f)
		return 0;
	invdet = 1.0f/invdet;
	m1->right.x = invdet*(m2->up.y * m2->at.z - m2->up.z * m2->at.y);
	m1->right.y = invdet*(m2->at.y * m2->right.z - m2->at.z * m2->right.y);
	m1->right.z = invdet*(m2->right.y * m2->up.z - m2->right.z * m2->up.y);
	m1->up.x = invdet*(m2->up.z * m2->at.x - m1->up.x * m2->at.z);
	m1->up.y = invdet*(m2->at.z * m2->right.x - m2->at.x * m2->right.z);
	m1->up.z = invdet*(m2->right.z * m1->up.x - m2->right.x * m2->up.z);
	m1->at.x = invdet*(m2->up.x * m2->at.y - m2->up.y * m2->at.x);
	m1->at.y = invdet*(m2->at.x * m2->right.y - m2->at.y * m2->right.x);
	m1->at.z = invdet*(m2->right.x * m2->up.y - m2->right.y * m2->up.x);
	return 1;
}

void
transpose(Matrix3 *m1, Matrix3 *m2)
{
	m1->right.x = m2->right.x;
	m1->right.y = m2->up.x;
	m1->right.z = m2->at.x;
	m1->up.x = m2->right.y;
	m1->up.y = m2->up.y;
	m1->up.z = m2->at.y;
	m1->at.x = m2->right.z;
	m1->at.y = m2->up.z;
	m1->at.z = m2->at.z;
}

bool32
equal(const Matrix &m1, const Matrix &m2)
{
	return matrixEqual((float32*)&m1, (float32*)&m2);
}

void
matrixIdentity(float32 *mat)
{
	memset(mat, 0, 64);
	mat[0] = 1.0f;
	mat[5] = 1.0f;
	mat[10] = 1.0f;
	mat[15] = 1.0f;
}

int
matrixEqual(float32 *m1, float32 *m2)
{
	for(int i = 0; i < 16; i++)
		if(m1[i] != m2[i])
			return 0;
	return 1;
}

int
matrixIsIdentity(float32 *mat)
{
	for(int32 i = 0; i < 4; i++)
		for(int32 j = 0; j < 4; j++)
			if(mat[i*4+j] != (i == j ? 1.0f : 0.0))
				return 0;
	return 1;
}

void
matrixMult(float32 *out, float32 *a, float32 *b)
{
	// TODO: replace with platform optimized code
	#define L(i,j) out[i*4+j]
	#define A(i,j) a[i*4+j]
	#define B(i,j) b[i*4+j]
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			L(i,j) = A(0,j)*B(i,0)
			       + A(1,j)*B(i,1)
			       + A(2,j)*B(i,2)
			       + A(3,j)*B(i,3);
	#undef L
	#undef A
	#undef B
}

void
vecTrans(float32 *out, float32 *mat, float32 *vec)
{
	#define M(i,j) mat[i*4+j]
	for(int i = 0; i < 4; i++)
		out[i] = M(0,i)*vec[0]
		       + M(1,i)*vec[1]
		       + M(2,i)*vec[2]
		       + M(3,i)*vec[3];
	#undef M
}

void
matrixTranspose(float32 *out, float32 *in)
{
	#define OUT(i,j) out[i*4+j]
	#define IN(i,j) in[i*4+j]
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			OUT(i,j) = IN(j,i);
	#undef IN
	#undef OUT
}

bool32
matrixInvert(float32 *out, float32 *m)
{
	float32 inv[16], det;
	int i;

	inv[0] = m[5]  * m[10] * m[15] -
	         m[5]  * m[11] * m[14] -
	         m[9]  * m[6]  * m[15] +
	         m[9]  * m[7]  * m[14] +
	         m[13] * m[6]  * m[11] -
	         m[13] * m[7]  * m[10];
	inv[4] = -m[4]  * m[10] * m[15] +
	          m[4]  * m[11] * m[14] +
	          m[8]  * m[6]  * m[15] -
	          m[8]  * m[7]  * m[14] -
	          m[12] * m[6]  * m[11] +
	          m[12] * m[7]  * m[10];
	inv[8] = m[4]  * m[9] * m[15] -
	         m[4]  * m[11] * m[13] -
	         m[8]  * m[5] * m[15] +
	         m[8]  * m[7] * m[13] +
	         m[12] * m[5] * m[11] -
	         m[12] * m[7] * m[9];
	inv[12] = -m[4]  * m[9] * m[14] +
	           m[4]  * m[10] * m[13] +
	           m[8]  * m[5] * m[14] -
	           m[8]  * m[6] * m[13] -
	           m[12] * m[5] * m[10] +
	           m[12] * m[6] * m[9];
	inv[1] = -m[1]  * m[10] * m[15] +
	          m[1]  * m[11] * m[14] +
	          m[9]  * m[2] * m[15] -
	          m[9]  * m[3] * m[14] -
	          m[13] * m[2] * m[11] +
	          m[13] * m[3] * m[10];
	inv[5] = m[0]  * m[10] * m[15] -
	         m[0]  * m[11] * m[14] -
	         m[8]  * m[2] * m[15] +
	         m[8]  * m[3] * m[14] +
	         m[12] * m[2] * m[11] -
	         m[12] * m[3] * m[10];
	inv[9] = -m[0]  * m[9] * m[15] +
	          m[0]  * m[11] * m[13] +
	          m[8]  * m[1] * m[15] -
	          m[8]  * m[3] * m[13] -
	          m[12] * m[1] * m[11] +
	          m[12] * m[3] * m[9];
	inv[13] = m[0]  * m[9] * m[14] -
	          m[0]  * m[10] * m[13] -
	          m[8]  * m[1] * m[14] +
	          m[8]  * m[2] * m[13] +
	          m[12] * m[1] * m[10] -
	          m[12] * m[2] * m[9];
	inv[2] = m[1]  * m[6] * m[15] -
	         m[1]  * m[7] * m[14] -
	         m[5]  * m[2] * m[15] +
	         m[5]  * m[3] * m[14] +
	         m[13] * m[2] * m[7] -
	         m[13] * m[3] * m[6];
	inv[6] = -m[0]  * m[6] * m[15] +
	          m[0]  * m[7] * m[14] +
	          m[4]  * m[2] * m[15] -
	          m[4]  * m[3] * m[14] -
	          m[12] * m[2] * m[7] +
	          m[12] * m[3] * m[6];
	inv[10] = m[0]  * m[5] * m[15] -
	          m[0]  * m[7] * m[13] -
	          m[4]  * m[1] * m[15] +
	          m[4]  * m[3] * m[13] +
	          m[12] * m[1] * m[7] -
	          m[12] * m[3] * m[5];
	inv[14] = -m[0]  * m[5] * m[14] +
	           m[0]  * m[6] * m[13] +
	           m[4]  * m[1] * m[14] -
	           m[4]  * m[2] * m[13] -
	           m[12] * m[1] * m[6] +
	           m[12] * m[2] * m[5];
	inv[3] = -m[1] * m[6] * m[11] +
	          m[1] * m[7] * m[10] +
	          m[5] * m[2] * m[11] -
	          m[5] * m[3] * m[10] -
	          m[9] * m[2] * m[7] +
	          m[9] * m[3] * m[6];
	inv[7] = m[0] * m[6] * m[11] -
	         m[0] * m[7] * m[10] -
	         m[4] * m[2] * m[11] +
	         m[4] * m[3] * m[10] +
	         m[8] * m[2] * m[7] -
	         m[8] * m[3] * m[6];
	inv[11] = -m[0] * m[5] * m[11] +
	           m[0] * m[7] * m[9] +
	           m[4] * m[1] * m[11] -
	           m[4] * m[3] * m[9] -
	           m[8] * m[1] * m[7] +
	           m[8] * m[3] * m[5];
	inv[15] = m[0] * m[5] * m[10] -
	          m[0] * m[6] * m[9] -
	          m[4] * m[1] * m[10] +
	          m[4] * m[2] * m[9] +
	          m[8] * m[1] * m[6] -
	          m[8] * m[2] * m[5];
	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
	if(det == 0)
		return 0;
	det = 1.0f / det;
	for(i = 0; i < 16; i++)
		out[i] = inv[i] * det;
	return 1;
}

void
matrixPrint(float32 *mat)
{
	printf("[ [ %8.4f, %8.4f, %8.4f, %8.4f ]\n"
	       "  [ %8.4f, %8.4f, %8.4f, %8.4f ]\n"
	       "  [ %8.4f, %8.4f, %8.4f, %8.4f ]\n"
	       "  [ %8.4f, %8.4f, %8.4f, %8.4f ] ]\n",
		mat[0], mat[4], mat[8], mat[12],
		mat[1], mat[5], mat[9], mat[13],
		mat[2], mat[6], mat[10], mat[14],
		mat[3], mat[7], mat[11], mat[15]);
}

#define PSEP_C '/'
#define PSEP_S "/"
#ifndef _WIN32
#include <sys/types.h>
#include <dirent.h>
#endif

void
correctPathCase(char *filename)
{
#ifndef _WIN32
	DIR *direct;
	struct dirent *dirent;

	char *dir, *arg;
	char copy[1024], sofar[1024] = ".";
	strncpy(copy, filename, 1024);
	arg = copy;
	// hack for absolute paths
	if(filename[0] == '/'){
		sofar[0] = '/';
		sofar[1] = '/';
		sofar[2] = '\0';
		arg++;
	}
	while(dir = strtok(arg, PSEP_S)){
		arg = nil;
		if(direct = opendir(sofar), dir == nil)
			return;
		while(dirent = readdir(direct), dirent != nil)
			if(strncmp_ci(dirent->d_name, dir, 1024) == 0){
				strncat(sofar, PSEP_S, 1024);
				strncat(sofar, dirent->d_name, 1024);
				break;
			}
		closedir(direct);
		if(dirent == nil)
			return;
	}
	strcpy(filename, sofar+2);
#endif
}

void
makePath(char *filename)
{
	size_t len = strlen(filename);
	for(size_t i = 0; i < len; i++)
		if(filename[i] == '/' || filename[i] == '\\')
			filename[i] = PSEP_C;
#ifndef _WIN32
	correctPathCase(filename);
#endif
}

int32
Stream::writeI8(int8 val)
{
	return write(&val, sizeof(int8));
}

int32
Stream::writeU8(uint8 val)
{
	return write(&val, sizeof(uint8));
}

int32
Stream::writeI16(int16 val)
{
	return write(&val, sizeof(int16));
}

int32
Stream::writeU16(uint16 val)
{
	return write(&val, sizeof(uint16));
}

int32
Stream::writeI32(int32 val)
{
	return write(&val, sizeof(int32));
}

int32
Stream::writeU32(uint32 val)
{
	return write(&val, sizeof(uint32));
}

int32
Stream::writeF32(float32 val)
{
	return write(&val, sizeof(float32));
}

int8
Stream::readI8(void)
{
	int8 tmp;
	read(&tmp, sizeof(int8));
	return tmp;
}

uint8
Stream::readU8(void)
{
	uint8 tmp;
	read(&tmp, sizeof(uint8));
	return tmp;
}

int16
Stream::readI16(void)
{
	int16 tmp;
	read(&tmp, sizeof(int16));
	return tmp;
}

uint16
Stream::readU16(void)
{
	uint16 tmp;
	read(&tmp, sizeof(uint16));
	return tmp;
}

int32
Stream::readI32(void)
{
	int32 tmp;
	read(&tmp, sizeof(int32));
	return tmp;
}

uint32
Stream::readU32(void)
{
	uint32 tmp;
	read(&tmp, sizeof(uint32));
	return tmp;
}

float32
Stream::readF32(void)
{
	float32 tmp;
	read(&tmp, sizeof(float32));
	return tmp;
}



void
StreamMemory::close(void)
{
}

uint32
StreamMemory::write(const void *data, uint32 len)
{
	if(this->eof())
		return 0;
	uint32 l = len;
	if(this->position+l > this->length){
		if(this->position+l > this->capacity)
			l = this->capacity-this->position;
		this->length = this->position+l;
	}
	memcpy(&this->data[this->position], data, l);
	this->position += l;
	if(len != l)
		this->position = S_EOF;
	return l;
}

uint32
StreamMemory::read(void *data, uint32 len)
{
	if(this->eof())
		return 0;
	uint32 l = len;
	if(this->position+l > this->length)
		l = this->length-this->position;
	memcpy(data, &this->data[this->position], l);
	this->position += l;
	if(len != l)
		this->position = S_EOF;
	return l;
}

void
StreamMemory::seek(int32 offset, int32 whence)
{
	if(whence == 0)
		this->position = offset;
	else if(whence == 1)
		this->position += offset;
	else
		this->position = this->length-offset;
	if(this->position > this->length){
		// TODO: ideally this would depend on the mode
		if(this->position > this->capacity)
			this->position = S_EOF;
		else
			this->length = this->position;
	}
}

uint32
StreamMemory::tell(void)
{
	return this->position;
}

bool
StreamMemory::eof(void)
{
	return this->position == S_EOF;
}

StreamMemory*
StreamMemory::open(uint8 *data, uint32 length, uint32 capacity)
{
	this->data = data;
	this->capacity = capacity;
	this->length = length;
	if(this->capacity < this->length)
		this->capacity = this->length;
	this->position = 0;
	return this;
}

uint32
StreamMemory::getLength(void)
{
	return this->length;
}


StreamFile*
StreamFile::open(const char *path, const char *mode)
{
	this->file = fopen(path, mode);
	if(this->file == nil){
		RWERROR((ERR_FILE, path));
		return nil;
	}
	return this;
}

void
StreamFile::close(void)
{
	fclose(this->file);
}

uint32
StreamFile::write(const void *data, uint32 length)
{
	return fwrite(data, length, 1, this->file);
}

uint32
StreamFile::read(void *data, uint32 length)
{
	return fread(data, length, 1, this->file);
}

void
StreamFile::seek(int32 offset, int32 whence)
{
	fseek(this->file, offset, whence);
}

uint32
StreamFile::tell(void)
{
	return ftell(this->file);
}

bool
StreamFile::eof(void)
{
	return ( feof(this->file) != 0 );
}

bool
writeChunkHeader(Stream *s, int32 type, int32 size)
{
	struct {
		int32 type, size;
		uint32 id;
	} buf = { type, size, libraryIDPack(version, build) };
	s->write(&buf, 12);
	return true;
}

bool
readChunkHeaderInfo(Stream *s, ChunkHeaderInfo *header)
{
	struct {
		int32 type, size;
		uint32 id;
	} buf;
	s->read(&buf, 12);
	if(s->eof())
		return false;
	assert(header != nil);
	header->type = buf.type;
	header->length = buf.size;
	header->version = libraryIDUnpackVersion(buf.id);
	header->build = libraryIDUnpackBuild(buf.id);
	return true;
}

bool
findChunk(Stream *s, uint32 type, uint32 *length, uint32 *version)
{
	ChunkHeaderInfo header;
	while(readChunkHeaderInfo(s, &header)){
		if(header.type == ID_NAOBJECT)
			return false;
		if(header.type == type){
			if(length)
				*length = header.length;
			if(version)
				*version = header.version;
			return true;
		}
		s->seek(header.length);
	}
	return false;
}

int32
findPointer(void *p, void **list, int32 num)
{
	int i;
	for(i = 0; i < num; i++)
		if(list[i] == p)
			return i;
	return -1;
}

uint8*
getFileContents(char *name, uint32 *len)
{
	FILE *cf = fopen(name, "rb");
	assert(cf != nil);
	fseek(cf, 0, SEEK_END);
	*len = ftell(cf);
	fseek(cf, 0, SEEK_SET);
	uint8 *data = new uint8[*len];
	fread(data, *len, 1, cf);
	fclose(cf);
	return data;
}

}
