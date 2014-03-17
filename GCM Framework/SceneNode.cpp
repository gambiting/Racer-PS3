#include "SceneNode.h"

SceneNode::SceneNode(void)	{

	transform		= Matrix4::identity();
	worldTransform	= Matrix4::identity();
	parent			= NULL;
	mesh			= NULL;

	target			= NULL;

	m_linearVelocity	= Vector3(0.0f, 0.0f, 0.0f);
	m_invMass			= 1.0f / 20.0f;
	m_invInertia		= Matrix4();

	gravity			= Vector3(0.0f, GRAVITY, 0.0f);
	radius			= 1.0f;

}

SceneNode::~SceneNode(void)	{
}

void	SceneNode::Update(float msec) {
	if(parent) {
		worldTransform = parent->worldTransform * transform;
	}
	else{
		worldTransform = transform;
	}

	for(std::vector<SceneNode*>::iterator i = children.begin(); i != children.end(); ++i) {
		(*i)->Update(msec);
	}
}

void	SceneNode::AddChild(SceneNode& s) {
	children.push_back(&s);
	s.parent = this;
}