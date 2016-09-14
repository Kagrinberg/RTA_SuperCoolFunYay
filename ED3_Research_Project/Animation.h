#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"
#include <fbxsdk.h>
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
//#include "Mesh.h"

// This stores the information of each key frame of each joint
// This is a linked list and each node is a snapshot of the
// global transformation of the joint at a certain frame
struct Keyframe
{
	FbxLongLong mFrameNum;
	FbxAMatrix mGlobalTransform;

	Keyframe()
	{

	}
};

struct CtrlPoint
{
	int controlIndex;
	std::vector<int> jointIndex;
	std::vector<float> jointWeights;

	CtrlPoint()
	{

	}
};

// This is the actual representation of a joint in a game engine
struct Joint
{
	std::string mName;
	int mParentIndex;
	FbxAMatrix mGlobalBindposeInverse;
	std::vector<Keyframe*> mAnimation;
	FbxNode* mNode;

	Joint() : mNode(nullptr)
	{
		mGlobalBindposeInverse.SetIdentity();
		mParentIndex = -1;
	}

	~Joint()
	{
		
	}
};

struct Skeleton
{
	std::vector<Joint> mJoints;

	Skeleton()
	{

	}

	~Skeleton()
	{
		
	}

};

class Animation
{
	FbxScene* mFBXScene;
	bool mHasAnimation;
	Skeleton mSkeleton;
	FbxLongLong mAnimationLength;
	std::string mAnimationName;
	std::unordered_map<unsigned int, CtrlPoint*> mControlPoints;
	FbxNode * meshNode;

public:
	Animation();
	~Animation();
	void CreateSkeleton();
	void SkeleRecursive(FbxNode * currentNode, int curIndex, int parentIndex);
	void setScene(FbxScene* inFBXScene);
	int getJoint(std::string jName);
	bool isAnimated();
	void createWeights();
	void makeCpts();
	void checkControls();
	Skeleton getSkele();
	std::unordered_map<unsigned int, CtrlPoint*>& getMap();
	int getAniLength();

};