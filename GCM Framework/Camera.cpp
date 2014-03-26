#include "Camera.h"

/*
Set useful default values!
*/
Camera::Camera(void)	{
	yaw		= 0.0f;
	pitch	= 0.0f;
	pad		= JOYPAD_A;
	position = Vector3(0,0,0);

	ypSensitivity = 0.3f;
	invertPitch = true;
}

Camera::~Camera(void)	{
}

/*
This should all be very familiar to you, if you've actually looked
at the tutorial code for the previous module! Only changes are to
use Sony's Matrix4 class instead of mine. See how instead of having
an arbitrary 'rotation' function, that takes a direction vector to
rotate around, Sony provide functions to specifically rotate around
the X, Y, and Z axis
*/
void Camera::Update(float msec) {
	float y, p;

	//If this camera has no joypad controlling it, it can't have moved!
	if(pad == JOYPAD_NONE) {	
		return;
	}

	Input::GetJoypadMovement(y,p,pad);

	yaw += y;// * ypSensitivity;
	pitch -= p;// invertPitch ? -(p*ypSensitivity) : (p*ypSensitivity);
	
	pitch = min(pitch,90.0f);
	pitch = max(pitch,-90.0f);

	if(yaw <0) {
		yaw+= 360.0f;
	}
	if(yaw > 360.0f) {
		yaw -= 360.0f;
	}

	if(Input::ButtonDown(INPUT_UP,pad)) {
		position += ((Matrix4::rotationY(DegToRad(-yaw)) * Vector3(0,0,-1) * msec).getXYZ());
	}
	if(Input::ButtonDown(INPUT_DOWN,pad)) {
		position -= ((Matrix4::rotationY(DegToRad(-yaw)) * Vector3(0,0,-1) * msec).getXYZ());
	}

	if(Input::ButtonDown(INPUT_LEFT,pad)) {
		position += ((Matrix4::rotationY(DegToRad(-yaw)) * Vector3(-1,0,0) * msec).getXYZ());
	}
	if(Input::ButtonDown(INPUT_RIGHT,pad)) {
		position -= ((Matrix4::rotationY(DegToRad(-yaw)) * Vector3(-1,0,0) * msec).getXYZ());
	}

	//Go up and down using the shoulder buttons!
	if(Input::ButtonDown(INPUT_L1,pad)) {
		position[1] -= msec;
	}
	if(Input::ButtonDown(INPUT_R1,pad)) {
		position[1] += msec;
	}
}

Matrix4 Camera::BuildViewMatrix() {
	return	Matrix4::rotationX(DegToRad(-pitch)) * 
		Matrix4::rotationY(DegToRad(yaw)) * 
		Matrix4::translation(-position);
}


Vector3 Camera::GetLookDirection() {
	Vector3 fwd = Vector3(0,0,-1);
	Matrix4 mat1;
	mat1 = mat1.identity();
	mat1 = mat1.rotation(-yaw*(3.1415926f/180.0f), Vector3(0,1,0));

	Matrix4 mat2;
	mat2 = mat2.identity();
	mat2 = mat2.rotation(pitch*(3.1415926f/180.0f), Vector3(1,0,0));

	Vector3 forward = ((mat1 * mat2) * fwd).getXYZ();
	//Vector3 forward = 
	//	(Matrix4::rotation(yaw, Vector3(0,1,0)) * 
	//	(Matrix4::rotation(pitch, Vector3(1,0,0)) * fwd)).getXYZ();

	//forward = normalize(forward);

	//Works until we change the p or y?
	return forward;
}